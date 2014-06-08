from distutils.core import setup, Extension
import platform


ext_modules = [
    Extension(
        '_jpg_lib',
        include_dirs=[],
        libraries=['jpeg'],
        sources=['pygame/_jpg_built/c/_jpg_lib.c'],
    ),
    Extension(
        '_png_lib',
        include_dirs=[],
        libraries=['png'],
        sources=['pygame/_png_built/c/_png_lib.c'],
    ),
    Extension(
        '_sdl_lib',
        include_dirs=['/usr/include/SDL', '/usr/local/include/SDL'],
        libraries=['SDL', 'SDL_image', 'SDL_ttf', 'SDL_mixer'],
        sources=['pygame/_sdl_built/c/_sdl_lib.c'],
    ),
    Extension(
        '_sdl_keys_lib',
        include_dirs=['/usr/include/SDL', '/usr/local/include/SDL'],
        libraries=[],
        sources=['pygame/_sdl_keys_built/c/_sdl_keys_lib.c'],
    )
]


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


setup(
    name='pygame_cffi',
    version='0.1',
    url='https://github.com/CTPUG/pygame_cffi',
    ext_modules=ext_modules,
    packages=[
        'pygame', 'pygame._jpg_built', 'pygame._macosx_built',
        'pygame._png_built', 'pygame._sdl_built', 'pygame._sdl_keys_built',
        'pygame.builders',
    ],
    include_package_data=True,
    package_data={
        'pygame': ['*/data/*.dat', 'lib/*', '*.ttf', 'pygame_icon.*'],
    }
)
