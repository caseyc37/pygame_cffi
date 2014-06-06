
import os, pickle

import _sdl_keys_lib as _libmodule
from cffibuilder.api import FFI


__all__ = ['lib', 'ffi']


# load the serialized parser
_parserfile = os.path.join(os.path.abspath(os.path.dirname(__file__)),
                           'data/parser.dat')
with open(_parserfile) as f:
    _parser = pickle.loads(f.read())


ffi = FFI(parser=_parser)


def get_extension():
    from distutils.core import Extension
    return Extension(
        '_sdl_keys_lib',
        include_dirs=['/usr/include/SDL', '/usr/local/include/SDL'],
        libraries=[],
        sources=['pygame/_sdl_keys_built/c/_sdl_keys_lib.c'],
    )

from cffibuilder import ffiplatform, model
from cffibuilder.genengine_cpy import GenCPythonEngine


class FFILibraryMeta(type):

    def __init__(self, name, bases, attrs):
        self._struct_pending_verification = {}
        self._types_of_builtin_functions = {}

        # build the FFILibrary class
        self._cffi_python_module = _libmodule
        self._cffi_dir = []

        _libmodule._cffi_original_ffi = ffi
        _libmodule._cffi_types_of_builtin_funcs = self._types_of_builtin_functions

        # call loading_cpy_struct() to get the struct layout inferred by
        # the C compiler
        self._load('loading')
        # the C code will need the <ctype> objects.  Collect them in
        # order in a list.
        engine = GenCPythonEngine('', '', '', _parser)
        engine.collect_types()
        revmapping = dict([(value, key)
                           for (key, value) in engine._typesdict.items()])
        lst = [revmapping[i] for i in range(len(revmapping))]
        with ffi._lock:
            self._ctypes_ordered = list(map(ffi._get_cached_btype, lst))
        super(FFILibraryMeta, self).__init__(name, bases, attrs)

    def _get_declarations(self):
        return sorted(_parser._declarations.items())

    def _load(self, step_name, **kwds):
        for name, tp in self._get_declarations():
            kind, realname = name.split(' ', 1)
            method = getattr(self, '_%s_cpy_%s' % (step_name, kind))
            try:
                method(tp, realname, **kwds)
            except Exception as e:
                model.attach_exception_info(e, name)
                raise

    def _loaded_noop(self, tp, name, **kwds):
        pass

    _loading_cpy_typedef = _loaded_noop
    _loaded_cpy_typedef = _loaded_noop
    _loading_cpy_function = _loaded_noop
    _loading_cpy_constant = _loaded_noop
    _loaded_cpy_constant = _loaded_noop
    _loading_cpy_macro = _loaded_noop
    _loaded_cpy_macro  = _loaded_noop
    _loading_cpy_variable = _loaded_noop

    def _loaded_cpy_function(self, tp, name, library):
        if tp.ellipsis:
            return
        func = getattr(_libmodule, name)
        setattr(library, name, func)
        self._types_of_builtin_functions[func] = tp

    def _loading_cpy_struct(self, tp, name):
        self._loading_struct_or_union(tp, 'struct', name)
    def _loaded_cpy_struct(self, tp, name, **kwds):
        self._loaded_struct_or_union(tp)
    def _loading_cpy_union(self, tp, name):
        self._loading_struct_or_union(tp, 'union', name)
    def _loaded_cpy_union(self, tp, name, **kwds):
        self._loaded_struct_or_union(tp)

    def _loading_struct_or_union(self, tp, prefix, name):
        if tp.fldnames is None:
            return     # nothing to do with opaque structs
        layoutfuncname = '_cffi_layout_%s_%s' % (prefix, name)
        #
        function = getattr(_libmodule, layoutfuncname)
        layout = function()
        if isinstance(tp, model.StructOrUnion) and tp.partial:
            # use the function()'s sizes and offsets to guide the
            # layout of the struct
            totalsize = layout[0]
            totalalignment = layout[1]
            fieldofs = layout[2::2]
            fieldsize = layout[3::2]
            tp.force_flatten()
            assert len(fieldofs) == len(fieldsize) == len(tp.fldnames)
            tp.fixedlayout = fieldofs, fieldsize, totalsize, totalalignment
        else:
            cname = ('%s %s' % (prefix, name)).strip()
            self._struct_pending_verification[tp] = layout, cname

    def _loaded_struct_or_union(self, tp):
        if tp.fldnames is None:
            return     # nothing to do with opaque structs
        ffi._get_cached_btype(tp)   # force 'fixedlayout' to be considered

        if tp in self._struct_pending_verification:
            # check that the layout sizes and offsets match the real ones
            def check(realvalue, expectedvalue, msg):
                if realvalue != expectedvalue:
                    raise ffiplatform.VerificationError(
                        "%s (we have %d, but C compiler says %d)"
                        % (msg, expectedvalue, realvalue))
            BStruct = ffi._get_cached_btype(tp)
            layout, cname = self._struct_pending_verification.pop(tp)
            check(layout[0], ffi.sizeof(BStruct), "wrong total size")
            check(layout[1], ffi.alignof(BStruct), "wrong total alignment")
            i = 2
            for fname, ftype, fbitsize in tp.enumfields():
                if fbitsize >= 0:
                    continue        # xxx ignore fbitsize for now
                check(layout[i], ffi.offsetof(BStruct, fname),
                      "wrong offset for field %r" % (fname,))
                if layout[i+1] != 0:
                    BField = ffi._get_cached_btype(ftype)
                    check(layout[i+1], ffi.sizeof(BField),
                          "wrong size for field %r" % (fname,))
                i += 2
            assert i == len(layout)

    def _loading_cpy_anonymous(self, tp, name):
        if isinstance(tp, model.EnumType):
            self._loading_cpy_enum(tp, name)
        else:
            self._loading_struct_or_union(tp, '', name)

    def _loaded_cpy_anonymous(self, tp, name, **kwds):
        if isinstance(tp, model.EnumType):
            self._loaded_cpy_enum(tp, name, **kwds)
        else:
            self._loaded_struct_or_union(tp)

    def _loading_cpy_enum(self, tp, name):
        if tp.partial:
            enumvalues = [getattr(_libmodule, enumerator)
                          for enumerator in tp.enumerators]
            tp.enumvalues = tuple(enumvalues)
            tp.partial_resolved = True

    def _loaded_cpy_enum(self, tp, name, library):
        for enumerator, enumvalue in zip(tp.enumerators, tp.enumvalues):
            setattr(library, enumerator, enumvalue)

    def _loaded_cpy_variable(self, tp, name, library):
        value = getattr(library, name)
        if isinstance(tp, model.ArrayType):   # int a[5] is "constant" in the
                                              # sense that "a=..." is forbidden
            if tp.length == '...':
                assert isinstance(value, tuple)
                (value, size) = value
                BItemType = ffi._get_cached_btype(tp.item)
                length, rest = divmod(size, ffi.sizeof(BItemType))
                if rest != 0:
                    raise ffiplatform.VerificationError(
                        "bad size: %r does not seem to be an array of %s" %
                        (name, tp.item))
                tp = tp.resolve_length(length)
            # 'value' is a <cdata 'type *'> which we have to replace with
            # a <cdata 'type[N]'> if the N is actually known
            if tp.length is not None:
                BArray = ffi._get_cached_btype(tp)
                value = ffi.cast(BArray, value)
                setattr(library, name, value)
            return
        # remove ptr=<cdata 'int *'> from the library instance, and replace
        # it by a property on the class, which reads/writes into ptr[0].
        ptr = value
        delattr(library, name)
        def getter(library):
            return ptr[0]
        def setter(library, value):
            ptr[0] = value
        setattr(type(library), name, property(getter, setter))
        type(library)._cffi_dir.append(name)


class FFILibrary(object):
    __metaclass__ = FFILibraryMeta

    def __init__(self):
        # Build FFILibrary instance and call _cffi_setup().
        # this will set up some fields like '_cffi_types', and only then
        # it will invoke the chained list of functions that will really
        # build (notably) the constant objects, as <cdata> if they are
        # pointers, and store them as attributes on the 'library' object.
        super(FFILibrary, self).__init__()
        if _libmodule._cffi_setup(FFILibrary._ctypes_ordered,
                                  ffiplatform.VerificationError,
                                  self):
            import warnings
            warnings.warn("reimporting %r might overwrite older definitions"
                          % (_libmodule.__name__))
        # finally, call the loaded_cpy_xxx() functions.  This will perform
        # the final adjustments, like copying the Python->C wrapper
        # functions from the module to the 'library' object, and setting
        # up the FFILibrary class with properties for the global C variables.
        with ffi._lock:
            FFILibrary._load('loaded', library=self)


lib = FFILibrary()
