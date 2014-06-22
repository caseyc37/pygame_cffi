import os

from cffibuilder import Builder

builder = Builder()

builder.cdef("""
const char* WMEnable(void);
int RunningFromBundleWithNSApplication(void);
int InstallNSApplication(void);
""")

builder.build(
    '_macosx',
    libraries=['SDL', 'objc'],
    include_dirs=[
        '/usr/local/include/SDL',
        '/usr/include/SDL',
        'pygame/lib',
    ],
    srcdir=os.path.normpath(os.path.join(os.path.abspath(os.path.dirname(__file__)), '../cffi_modules/')),
    extra_link_args=['-framework', 'Cocoa'],
    source=open('pygame/lib/sdlmain_osx.h', 'rb').read(),
    sources=['pygame/lib/sdlmain_osx.m'],
)
