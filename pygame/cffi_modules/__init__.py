
import glob, os
from distutils.core import Extension


libraries = ['ffi']
include_dirs = ['/usr/include/ffi',
                '/usr/include/libffi',
                os.path.join(os.path.dirname(__file__), 'c')]


def get_extensions(*module_names):
    build_folder = os.path.dirname(__file__)
    extensions = []
    module_names = set(module_names)
    for fp in glob.glob('%s/*/BUILD-ARGS.txt' % build_folder):
        module_name = fp.rsplit('/', 2)[1]
        if module_names and module_name not in module_names:
            continue
        module_dir = os.path.dirname(fp)
        sources = [os.path.join(module_dir, 'c/%s_lib.c' % module_name)]
        with open(fp) as f:
            build_args = f.read()
            build_args = eval(build_args)
        build_args.setdefault('libraries', [])
        build_args.setdefault('include_dirs', [])
        build_args['libraries'].extend(libraries)
        build_args['include_dirs'].extend(include_dirs)
        extensions.append(Extension(
            '%s_lib' % module_name,
            sources=sources,
            **build_args
        ))
    return extensions
