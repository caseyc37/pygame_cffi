from pygame.cffi_modules._sdl import ffi, lib as sdl


def get_sdl_version():
    """ get_sdl_version() -> major, minor, patch
    get the version number of SDL
    """
    v = sdl.SDL_Linked_Version()
    return (v.major, v.minor, v.patch)


def get_sdl_byteorder():
    return sdl.SDL_BYTEORDER


__all__ = ['ffi', 'sdl', 'get_sdl_version', 'get_sdl_byteorder']
