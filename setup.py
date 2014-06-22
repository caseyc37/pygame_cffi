from distutils.core import setup
import platform


def get_extensions():
    import os, sys
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'pygame'))

    from cffi_modules import get_extensions as get_possible_extensions
    ext_modules = get_possible_extensions()
    if not platform.system().startswith('Darwin'):
        # Exclude extra OSX stuff if we're not on OSX.
        ext_modules = filter(lambda e: not e.name.startswith('_macosx'),
                             ext_modules)
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
