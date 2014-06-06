from distutils.core import setup, Extension


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


setup(
    name='pygame_cffi',
    version = '0.1',
    url='https://github.com/CTPUG/pygame_cffi',
    ext_modules=ext_modules
)
