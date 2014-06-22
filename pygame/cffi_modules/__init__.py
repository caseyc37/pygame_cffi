
import glob, os, sys
from distutils.core import Extension


# TODO: compile libffi on Windows
# TODO: use pkg-config


def get_build_dir():
    build_dir = os.path.dirname(__file__)
    # If we're being called from setup.py we need to use paths relative to the
    # directory containing it.
    main_file = getattr(sys.modules['__main__'], '__file__', None)
    if main_file is not None and main_file.endswith('/setup.py'):
        # It looks like we're being called from setup.py, so all paths must be
        # relative to that.
        build_dir = os.path.relpath(build_dir, os.path.dirname(main_file))
    return build_dir

def get_extensions(*module_names):
    build_dir = get_build_dir()
    extensions = []
    module_names = set(module_names)
    for fp in glob.glob('%s/*/BUILD-ARGS.txt' % build_dir):
        module_name = fp.rsplit('/', 2)[1]
        if module_names and module_name not in module_names:
            continue
        module_dir = os.path.dirname(fp)
        our_sources = [os.path.join(module_dir, 'c/%s_lib.c' % module_name)]
        with open(fp) as f:
            build_args = f.read()
            build_args = eval(build_args)
        sources = build_args.pop('sources', [])
        sources.extend(our_sources)
        extensions.append(Extension(
            '%s_lib' % module_name,
            sources=sources,
            **build_args
        ))

    if (not module_names or '_cffi_backend' in module_names) and \
            '__pypy__' not in sys.modules:
        extensions.append(Extension(
            name='_cffi_backend',
            include_dirs=['/usr/include/ffi', '/usr/include/libffi',
                          os.path.join(build_dir, 'c')],
            sources=[os.path.join(build_dir, 'c/_cffi_backend.c')],
            libraries=['ffi'],
        ))

    return extensions
