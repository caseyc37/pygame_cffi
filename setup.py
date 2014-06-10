from distutils.core import setup
import platform


def get_extensions():
    from distutils.core import Extension
    import os, sys
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'pygame'))

    from cffi_modules import get_extensions as get_possible_extensions
    ext_modules = filter(lambda e: not e.name.startswith('_macosx'),
                         get_possible_extensions())

    if platform.system().startswith('Darwin'):
        # Extra stuff to do on OSX.
        ext_modules.insert(0, Extension(
            '_macosx_lib',
            include_dirs=[
                '/usr/include/SDL',
                '/usr/local/include/SDL',
                'pygame/lib',
            ],
            libraries=['SDL', 'objc'],
            extra_link_args=['-framework', 'Cocoa'],
            sources=[
                'pygame/lib/sdlmain_osx.m',
                'pygame/_macosx_built/c/_macosx_lib.c',
            ],
        ))
    return ext_modules


setup(
    name='pygame_cffi',
    version='0.1',
    url='https://github.com/CTPUG/pygame_cffi',
    ext_modules=get_extensions(),
    packages=[
        'pygame', 'pygame.builders', 'pygame.cffi_modules',
        'pygame.cffi_modules._sdl', 'pygame.cffi_modules._sdl_keys',
        'pygame.cffi_modules._jpg', 'pygame.cffi_modules._png',
        'pygame.cffi_modules._macosx'
    ],
    include_package_data=True,
    package_data={
        'pygame': ['lib/*', '*.ttf', 'pygame_icon.*'],
        'pygame.cffi_modules': ['*/BUILD-ARGS.txt', '*/data/*.dat']
    }
)
