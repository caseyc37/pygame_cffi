
#include <Python.h>
#include <stddef.h>

#if defined(MS_WIN32) && !defined(_STDINT_H)
#include <malloc.h>   /* for alloca() */
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#if !defined(_MSC_VER) || _MSC_VER <= 1700
typedef unsigned char _Bool;
#endif
#else
#if (defined (__SVR4) && defined (__sun)) || defined(_AIX)
#  include <alloca.h>
#endif
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ?   /* unsigned */                                   \
        (sizeof(type) < sizeof(long) ? PyInt_FromLong(x) :               \
         sizeof(type) == sizeof(long) ? PyLong_FromUnsignedLong(x) :     \
                                        PyLong_FromUnsignedLongLong(x))  \
      : (sizeof(type) <= sizeof(long) ? PyInt_FromLong(x) :              \
                                        PyLong_FromLongLong(x)))

#define _cffi_to_c_int(o, type)                                          \
    (sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), 0))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/



    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>

    Uint8 _pygame_SDL_BUTTON(Uint8 X) {
        return SDL_BUTTON(X);
    }

    /*******************************************************/
    /* tga code by Mattias Engdegard, in the public domain */
    /*******************************************************/
    struct TGAheader
    {
        Uint8 infolen;                /* length of info field */
        Uint8 has_cmap;                /* 1 if image has colormap, 0 otherwise */
        Uint8 type;

        Uint8 cmap_start[2];        /* index of first colormap entry */
        Uint8 cmap_len[2];                /* number of entries in colormap */
        Uint8 cmap_bits;                /* bits per colormap entry */

        Uint8 yorigin[2];                /* image origin (ignored here) */
        Uint8 xorigin[2];
        Uint8 width[2];                /* image size */
        Uint8 height[2];
        Uint8 pixel_bits;                /* bits/pixel */
        Uint8 flags;
    };

    enum tga_type
    {
        TGA_TYPE_INDEXED = 1,
        TGA_TYPE_RGB = 2,
        TGA_TYPE_BW = 3,
        TGA_TYPE_RLE = 8                /* additive */
    };

    #define TGA_INTERLEAVE_MASK        0xc0
    #define TGA_INTERLEAVE_NONE        0x00
    #define TGA_INTERLEAVE_2WAY        0x40
    #define TGA_INTERLEAVE_4WAY        0x80

    #define TGA_ORIGIN_MASK                0x30
    #define TGA_ORIGIN_LEFT                0x00
    #define TGA_ORIGIN_RIGHT        0x10
    #define TGA_ORIGIN_LOWER        0x00
    #define TGA_ORIGIN_UPPER        0x20

    /* read/write unaligned little-endian 16-bit ints */
    #define LE16(p) ((p)[0] + ((p)[1] << 8))
    #define SETLE16(p, v) ((p)[0] = (v), (p)[1] = (v) >> 8)

    #ifndef MIN
    #define MIN(a, b) ((a) < (b) ? (a) : (b))
    #endif

    #define TGA_RLE_MAX 128                /* max length of a TGA RLE chunk */
    /* return the number of bytes in the resulting buffer after RLE-encoding
       a line of TGA data */
    static int
    rle_line (Uint8 *src, Uint8 *dst, int w, int bpp)
    {
        int x = 0;
        int out = 0;
        int raw = 0;
        while (x < w)
        {
            Uint32 pix;
            int x0 = x;
            memcpy (&pix, src + x * bpp, bpp);
            x++;
            while (x < w && memcmp (&pix, src + x * bpp, bpp) == 0
                   && x - x0 < TGA_RLE_MAX)
                x++;
            /* use a repetition chunk iff the repeated pixels would consume
               two bytes or more */
            if ((x - x0 - 1) * bpp >= 2 || x == w)
            {
                /* output previous raw chunks */
                while (raw < x0)
                {
                    int n = MIN (TGA_RLE_MAX, x0 - raw);
                    dst[out++] = n - 1;
                    memcpy (dst + out, src + raw * bpp, n * bpp);
                    out += n * bpp;
                    raw += n;
                }

                if (x - x0 > 0)
                {
                    /* output new repetition chunk */
                    dst[out++] = 0x7f + x - x0;
                    memcpy (dst + out, &pix, bpp);
                    out += bpp;
                }
                raw = x;
            }
        }
        return out;
    }

    /*
     * Save a surface to an output stream in TGA format.
     * 8bpp surfaces are saved as indexed images with 24bpp palette, or with
     *     32bpp palette if colourkeying is used.
     * 15, 16, 24 and 32bpp surfaces are saved as 24bpp RGB images,
     * or as 32bpp RGBA images if alpha channel is used.
     *
     * RLE compression is not used in the output file.
     *
     * Returns -1 upon error, 0 if success
     */
    static int
    _pygame_SaveTGA_RW (SDL_Surface *surface, SDL_RWops *out, int rle)
    {
        SDL_Surface *linebuf = NULL;
        int alpha = 0;
        int ckey = -1;
        struct TGAheader h;
        int srcbpp;
        unsigned surf_flags;
        unsigned surf_alpha;
        Uint32 rmask, gmask, bmask, amask;
        SDL_Rect r;
        int bpp;
        Uint8 *rlebuf = NULL;

        h.infolen = 0;
        SETLE16 (h.cmap_start, 0);

        srcbpp = surface->format->BitsPerPixel;
        if (srcbpp < 8)
        {
            SDL_SetError ("cannot save <8bpp images as TGA");
            return -1;
        }

        if (srcbpp == 8)
        {
            h.has_cmap = 1;
            h.type = TGA_TYPE_INDEXED;
            if (surface->flags & SDL_SRCCOLORKEY)
            {
                ckey = surface->format->colorkey;
                h.cmap_bits = 32;
            }
            else
                h.cmap_bits = 24;
            SETLE16 (h.cmap_len, surface->format->palette->ncolors);
            h.pixel_bits = 8;
            rmask = gmask = bmask = amask = 0;
        }
        else
        {
            h.has_cmap = 0;
            h.type = TGA_TYPE_RGB;
            h.cmap_bits = 0;
            SETLE16 (h.cmap_len, 0);
            if (surface->format->Amask)
            {
                alpha = 1;
                h.pixel_bits = 32;
            }
            else
                h.pixel_bits = 24;
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                int s = alpha ? 0 : 8;
                amask = 0x000000ff >> s;
                rmask = 0x0000ff00 >> s;
                gmask = 0x00ff0000 >> s;
                bmask = 0xff000000 >> s;
            }
            else
            {
                amask = alpha ? 0xff000000 : 0;
                rmask = 0x00ff0000;
                gmask = 0x0000ff00;
                bmask = 0x000000ff;
            }
        }
        bpp = h.pixel_bits >> 3;
        if (rle)
            h.type += TGA_TYPE_RLE;

        SETLE16 (h.yorigin, 0);
        SETLE16 (h.xorigin, 0);
        SETLE16 (h.width, surface->w);
        SETLE16 (h.height, surface->h);
        h.flags = TGA_ORIGIN_UPPER | (alpha ? 8 : 0);

        if (!SDL_RWwrite (out, &h, sizeof (h), 1))
            return -1;

        if (h.has_cmap)
        {
            int i;
            SDL_Palette *pal = surface->format->palette;
            Uint8 entry[4];
            for (i = 0; i < pal->ncolors; i++)
            {
                entry[0] = pal->colors[i].b;
                entry[1] = pal->colors[i].g;
                entry[2] = pal->colors[i].r;
                entry[3] = (i == ckey) ? 0 : 0xff;
                if (!SDL_RWwrite (out, entry, h.cmap_bits >> 3, 1))
                    return -1;
            }
        }

        linebuf = SDL_CreateRGBSurface (SDL_SWSURFACE, surface->w, 1, h.pixel_bits,
                                        rmask, gmask, bmask, amask);
        if (!linebuf)
            return -1;
        if (h.has_cmap)
            SDL_SetColors (linebuf, surface->format->palette->colors, 0,
                           surface->format->palette->ncolors);
        if (rle)
        {
            rlebuf = malloc (bpp * surface->w + 1 + surface->w / TGA_RLE_MAX);
            if (!rlebuf)
            {
                SDL_SetError ("out of memory");
                goto error;
            }
        }

        /* Temporarily remove colourkey and alpha from surface so copies are
           opaque */
        surf_flags = surface->flags & (SDL_SRCALPHA | SDL_SRCCOLORKEY);
        surf_alpha = surface->format->alpha;
        if (surf_flags & SDL_SRCALPHA)
            SDL_SetAlpha (surface, 0, 255);
        if (surf_flags & SDL_SRCCOLORKEY)
            SDL_SetColorKey (surface, 0, surface->format->colorkey);

        r.x = 0;
        r.w = surface->w;
        r.h = 1;
        for (r.y = 0; r.y < surface->h; r.y++)
        {
            int n;
            void *buf;
            if (SDL_BlitSurface (surface, &r, linebuf, NULL) < 0)
                break;
            if (rle)
            {
                buf = rlebuf;
                n = rle_line (linebuf->pixels, rlebuf, surface->w, bpp);
            }
            else
            {
                buf = linebuf->pixels;
                n = surface->w * bpp;
            }
            if (!SDL_RWwrite (out, buf, n, 1))
                break;
        }

        /* restore flags */
        if (surf_flags & SDL_SRCALPHA)
            SDL_SetAlpha (surface, SDL_SRCALPHA, (Uint8)surf_alpha);
        if (surf_flags & SDL_SRCCOLORKEY)
            SDL_SetColorKey (surface, SDL_SRCCOLORKEY, surface->format->colorkey);

    error:
        free (rlebuf);
        SDL_FreeSurface (linebuf);
        return 0;
    }

    /*
  pygame - Python Game Library
  Copyright (C) 2000-2001  Pete Shinners
  Copyright (C) 2007 Marcus von Appen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Pete Shinners
  pete@shinners.org
*/

#ifndef SURFACE_H
#define SURFACE_H

/* This is defined in SDL.h */
#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif

#define PYGAME_BLEND_ADD  0x1
#define PYGAME_BLEND_SUB  0x2
#define PYGAME_BLEND_MULT 0x3
#define PYGAME_BLEND_MIN  0x4
#define PYGAME_BLEND_MAX  0x5

#define PYGAME_BLEND_RGB_ADD  0x1
#define PYGAME_BLEND_RGB_SUB  0x2
#define PYGAME_BLEND_RGB_MULT 0x3
#define PYGAME_BLEND_RGB_MIN  0x4
#define PYGAME_BLEND_RGB_MAX  0x5

#define PYGAME_BLEND_RGBA_ADD  0x6
#define PYGAME_BLEND_RGBA_SUB  0x7
#define PYGAME_BLEND_RGBA_MULT 0x8
#define PYGAME_BLEND_RGBA_MIN  0x9
#define PYGAME_BLEND_RGBA_MAX  0x10
#define PYGAME_BLEND_PREMULTIPLIED  0x11





#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define GET_PIXEL_24(b) (b[0] + (b[1] << 8) + (b[2] << 16))
#else
#define GET_PIXEL_24(b) (b[2] + (b[1] << 8) + (b[0] << 16))
#endif

#define GET_PIXEL(pxl, bpp, source)               \
    switch (bpp)                                  \
    {                                             \
    case 2:                                       \
        pxl = *((Uint16 *) (source));             \
        break;                                    \
    case 4:                                       \
        pxl = *((Uint32 *) (source));             \
        break;                                    \
    default:                                      \
    {                                             \
        Uint8 *b = (Uint8 *) source;              \
        pxl = GET_PIXEL_24(b);                    \
    }                                             \
    break;                                        \
    }

#define GET_PIXELVALS(_sR, _sG, _sB, _sA, px, fmt, ppa)               \
    _sR = ((px & fmt->Rmask) >> fmt->Rshift);                         \
    _sR = (_sR << fmt->Rloss) + (_sR >> (8 - (fmt->Rloss << 1)));     \
    _sG = ((px & fmt->Gmask) >> fmt->Gshift);                         \
    _sG = (_sG << fmt->Gloss) + (_sG >> (8 - (fmt->Gloss << 1)));     \
    _sB = ((px & fmt->Bmask) >> fmt->Bshift);                         \
    _sB = (_sB << fmt->Bloss) + (_sB >> (8 - (fmt->Bloss << 1)));     \
    if (ppa)                                                          \
    {                                                                 \
        _sA = ((px & fmt->Amask) >> fmt->Ashift);                     \
        _sA = (_sA << fmt->Aloss) + (_sA >> (8 - (fmt->Aloss << 1))); \
    }                                                                 \
    else                                                              \
    {                                                                 \
        _sA = 255;                                                    \
    }

#define GET_PIXELVALS_1(sr, sg, sb, sa, _src, _fmt)    \
    sr = _fmt->palette->colors[*((Uint8 *) (_src))].r; \
    sg = _fmt->palette->colors[*((Uint8 *) (_src))].g; \
    sb = _fmt->palette->colors[*((Uint8 *) (_src))].b; \
    sa = 255;







#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SET_OFFSETS_24(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0 ? 0 :                  \
          fmt->Rshift == 8 ? 1 :                  \
                         2   );                   \
    og = (fmt->Gshift == 0 ? 0 :                  \
          fmt->Gshift == 8 ? 1 :                  \
                         2   );                   \
    ob = (fmt->Bshift == 0 ? 0 :                  \
          fmt->Bshift == 8 ? 1 :                  \
                         2   );                   \
    }

#define SET_OFFSETS_32(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0  ? 0 :                 \
          fmt->Rshift == 8  ? 1 :                 \
          fmt->Rshift == 16 ? 2 :                 \
                          3   );                  \
    og = (fmt->Gshift == 0  ? 0 :                 \
          fmt->Gshift == 8  ? 1 :                 \
          fmt->Gshift == 16 ? 2 :                 \
                          3   );                  \
    ob = (fmt->Bshift == 0  ? 0 :                 \
          fmt->Bshift == 8  ? 1 :                 \
          fmt->Bshift == 16 ? 2 :                 \
                          3   );                  \
    }
#else
#define SET_OFFSETS_24(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0 ? 2 :                  \
          fmt->Rshift == 8 ? 1 :                  \
                         0   );                   \
    og = (fmt->Gshift == 0 ? 2 :                  \
          fmt->Gshift == 8 ? 1 :                  \
                         0   );                   \
    ob = (fmt->Bshift == 0 ? 2 :                  \
          fmt->Bshift == 8 ? 1 :                  \
                         0   );                   \
    }

#define SET_OFFSETS_32(or, og, ob, fmt)           \
    {                                             \
    or = (fmt->Rshift == 0  ? 3 :                 \
          fmt->Rshift == 8  ? 2 :                 \
          fmt->Rshift == 16 ? 1 :                 \
                          0   );                  \
    og = (fmt->Gshift == 0  ? 3 :                 \
          fmt->Gshift == 8  ? 2 :                 \
          fmt->Gshift == 16 ? 1 :                 \
                          0   );                  \
    ob = (fmt->Bshift == 0  ? 3 :                 \
          fmt->Bshift == 8  ? 2 :                 \
          fmt->Bshift == 16 ? 1 :                 \
                          0   );                  \
    }
#endif


#define CREATE_PIXEL(buf, r, g, b, a, bp, ft)     \
    switch (bp)                                   \
    {                                             \
    case 2:                                       \
        *((Uint16 *) (buf)) =                     \
            ((r >> ft->Rloss) << ft->Rshift) |    \
            ((g >> ft->Gloss) << ft->Gshift) |    \
            ((b >> ft->Bloss) << ft->Bshift) |    \
            ((a >> ft->Aloss) << ft->Ashift);     \
        break;                                    \
    case 4:                                       \
        *((Uint32 *) (buf)) =                     \
            ((r >> ft->Rloss) << ft->Rshift) |    \
            ((g >> ft->Gloss) << ft->Gshift) |    \
            ((b >> ft->Bloss) << ft->Bshift) |    \
            ((a >> ft->Aloss) << ft->Ashift);     \
        break;                                    \
    }

/* Pretty good idea from Tom Duff :-). */
#define LOOP_UNROLLED4(code, n, width) \
    n = (width + 3) / 4;               \
    switch (width & 3)                 \
    {                                  \
    case 0: do { code;                 \
        case 3: code;                  \
        case 2: code;                  \
        case 1: code;                  \
        } while (--n > 0);             \
    }

/* Used in the srcbpp == dstbpp == 1 blend functions */
#define REPEAT_3(code) \
    code;              \
    code;              \
    code;

#define REPEAT_4(code) \
    code;              \
    code;              \
    code;              \
    code;


#define BLEND_ADD(tmp, sR, sG, sB, sA, dR, dG, dB, dA)  \
    tmp = dR + sR; dR = (tmp <= 255 ? tmp : 255);       \
    tmp = dG + sG; dG = (tmp <= 255 ? tmp : 255);       \
    tmp = dB + sB; dB = (tmp <= 255 ? tmp : 255);

#define BLEND_SUB(tmp, sR, sG, sB, sA, dR, dG, dB, dA) \
    tmp = dR - sR; dR = (tmp >= 0 ? tmp : 0);          \
    tmp = dG - sG; dG = (tmp >= 0 ? tmp : 0);          \
    tmp = dB - sB; dB = (tmp >= 0 ? tmp : 0);

#define BLEND_MULT(sR, sG, sB, sA, dR, dG, dB, dA) \
    dR = (dR && sR) ? (dR * sR) >> 8 : 0;          \
    dG = (dG && sG) ? (dG * sG) >> 8 : 0;          \
    dB = (dB && sB) ? (dB * sB) >> 8 : 0;

#define BLEND_MIN(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR < dR) { dR = sR; }                      \
    if(sG < dG) { dG = sG; }                      \
    if(sB < dB) { dB = sB; }

#define BLEND_MAX(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR > dR) { dR = sR; }                      \
    if(sG > dG) { dG = sG; }                      \
    if(sB > dB) { dB = sB; }






#define BLEND_RGBA_ADD(tmp, sR, sG, sB, sA, dR, dG, dB, dA)  \
    tmp = dR + sR; dR = (tmp <= 255 ? tmp : 255);       \
    tmp = dG + sG; dG = (tmp <= 255 ? tmp : 255);       \
    tmp = dB + sB; dB = (tmp <= 255 ? tmp : 255);       \
    tmp = dA + sA; dA = (tmp <= 255 ? tmp : 255);

#define BLEND_RGBA_SUB(tmp, sR, sG, sB, sA, dR, dG, dB, dA) \
    tmp = dR - sR; dR = (tmp >= 0 ? tmp : 0);          \
    tmp = dG - sG; dG = (tmp >= 0 ? tmp : 0);          \
    tmp = dB - sB; dB = (tmp >= 0 ? tmp : 0);          \
    tmp = dA - sA; dA = (tmp >= 0 ? tmp : 0);

#define BLEND_RGBA_MULT(sR, sG, sB, sA, dR, dG, dB, dA) \
    dR = (dR && sR) ? (dR * sR) >> 8 : 0;          \
    dG = (dG && sG) ? (dG * sG) >> 8 : 0;          \
    dB = (dB && sB) ? (dB * sB) >> 8 : 0;          \
    dA = (dA && sA) ? (dA * sA) >> 8 : 0;

#define BLEND_RGBA_MIN(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR < dR) { dR = sR; }                      \
    if(sG < dG) { dG = sG; }                      \
    if(sB < dB) { dB = sB; }                      \
    if(sA < dA) { dA = sA; }

#define BLEND_RGBA_MAX(sR, sG, sB, sA, dR, dG, dB, dA) \
    if(sR > dR) { dR = sR; }                      \
    if(sG > dG) { dG = sG; }                      \
    if(sB > dB) { dB = sB; }                      \
    if(sA > dA) { dA = sA; }











#if 1
/* Choose an alpha blend equation. If the sign is preserved on a right shift
 * then use a specialized, faster, equation. Otherwise a more general form,
 * where all additions are done before the shift, is needed.
*/
#if (-1 >> 1) < 0
#define ALPHA_BLEND_COMP(sC, dC, sA) ((((sC - dC) * sA + sC) >> 8) + dC)
#else
#define ALPHA_BLEND_COMP(sC, dC, sA) (((dC << 8) + (sC - dC) * sA + sC) >> 8)
#endif

#define ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB, dA) \
    do {                                            \
        if (dA)                                     \
        {                                           \
            dR = ALPHA_BLEND_COMP(sR, dR, sA);      \
            dG = ALPHA_BLEND_COMP(sG, dG, sA);      \
            dB = ALPHA_BLEND_COMP(sB, dB, sA);      \
            dA = sA + dA - ((sA * dA) / 255);       \
        }                                           \
        else                                        \
        {                                           \
            dR = sR;                                \
            dG = sG;                                \
            dB = sB;                                \
            dA = sA;                                \
        }                                           \
    } while(0)

#define ALPHA_BLEND_PREMULTIPLIED_COMP(sC, dC, sA) (sC + dC - ((dC * sA) >> 8))

#define ALPHA_BLEND_PREMULTIPLIED(tmp, sR, sG, sB, sA, dR, dG, dB, dA) \
    do {                                            \
            tmp = ALPHA_BLEND_PREMULTIPLIED_COMP(sR, dR, sA); dR = (tmp > 255 ? 255 : tmp); \
            tmp = ALPHA_BLEND_PREMULTIPLIED_COMP(sG, dG, sA); dG = (tmp > 255 ? 255 : tmp); \
            tmp = ALPHA_BLEND_PREMULTIPLIED_COMP(sB, dB, sA); dB = (tmp > 255 ? 255 : tmp); \
            dA = sA + dA - ((sA * dA) / 255);       \
    } while(0)
#elif 0

#define ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB, dA)    \
    do {                                               \
        if(sA){                                        \
            if(dA && sA < 255){                        \
                int dContrib = dA*(255 - sA)/255;      \
                dA = sA+dA - ((sA*dA)/255);            \
                dR = (dR*dContrib + sR*sA)/dA;         \
                dG = (dG*dContrib + sG*sA)/dA;         \
                dB = (dB*dContrib + sB*sA)/dA;         \
            }else{                                     \
                dR = sR;                               \
                dG = sG;                               \
                dB = sB;                               \
                dA = sA;                               \
            }                                          \
        }                                              \
    } while(0)
#endif

int
surface_fill_blend (SDL_Surface *surface, SDL_Rect *rect, Uint32 color,
                    int blendargs);

void
surface_respect_clip_rect (SDL_Surface *surface, SDL_Rect *rect);

int
pygame_AlphaBlit (SDL_Surface * src, SDL_Rect * srcrect,
                  SDL_Surface * dst, SDL_Rect * dstrect, int the_args);

int
pygame_Blit (SDL_Surface * src, SDL_Rect * srcrect,
             SDL_Surface * dst, SDL_Rect * dstrect, int the_args);

#endif /* SURFACE_H */


    /*
  pygame - Python Game Library
  Copyright (C) 2000-2001  Pete Shinners
  Copyright (C) 2006 Rene Dudfield
  Copyright (C) 2007 Marcus von Appen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Pete Shinners
  pete@shinners.org
*/

#define NO_PYGAME_C_API

/* The structure passed to the low level blit functions */
typedef struct
{
    int              width;
    int              height;
    Uint8           *s_pixels;
    int              s_pxskip;
    int              s_skip;
    Uint8           *d_pixels;
    int              d_pxskip;
    int              d_skip;
    SDL_PixelFormat *src;
    SDL_PixelFormat *dst;
    Uint32           src_flags;
    Uint32           dst_flags;
} SDL_BlitInfo;

static void alphablit_alpha (SDL_BlitInfo * info);
static void alphablit_colorkey (SDL_BlitInfo * info);
static void alphablit_solid (SDL_BlitInfo * info);
static void blit_blend_add (SDL_BlitInfo * info);
static void blit_blend_sub (SDL_BlitInfo * info);
static void blit_blend_mul (SDL_BlitInfo * info);
static void blit_blend_min (SDL_BlitInfo * info);
static void blit_blend_max (SDL_BlitInfo * info);

static void blit_blend_rgba_add (SDL_BlitInfo * info);
static void blit_blend_rgba_sub (SDL_BlitInfo * info);
static void blit_blend_rgba_mul (SDL_BlitInfo * info);
static void blit_blend_rgba_min (SDL_BlitInfo * info);
static void blit_blend_rgba_max (SDL_BlitInfo * info);

static void blit_blend_premultiplied (SDL_BlitInfo * info);


static int
SoftBlitPyGame (SDL_Surface * src, SDL_Rect * srcrect,
                SDL_Surface * dst, SDL_Rect * dstrect, int the_args);
extern int  SDL_RLESurface (SDL_Surface * surface);
extern void SDL_UnRLESurface (SDL_Surface * surface, int recode);

static int
SoftBlitPyGame (SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst,
                SDL_Rect * dstrect, int the_args)
{
    int okay;
    int src_locked;
    int dst_locked;

    /* Everything is okay at the beginning...  */
    okay = 1;

    /* Lock the destination if it's in hardware */
    dst_locked = 0;
    if (SDL_MUSTLOCK (dst))
    {
        if (SDL_LockSurface (dst) < 0)
            okay = 0;
        else
            dst_locked = 1;
    }
    /* Lock the source if it's in hardware */
    src_locked = 0;
    if (SDL_MUSTLOCK (src))
    {
        if (SDL_LockSurface (src) < 0)
            okay = 0;
        else
            src_locked = 1;
    }

    /* Set up source and destination buffer pointers, and BLIT! */
    if (okay && srcrect->w && srcrect->h)
    {
        SDL_BlitInfo    info;

        /* Set up the blit information */
        info.width = srcrect->w;
        info.height = srcrect->h;
        info.s_pixels = (Uint8 *) src->pixels + src->offset +
            (Uint16) srcrect->y * src->pitch +
            (Uint16) srcrect->x * src->format->BytesPerPixel;
        info.s_pxskip = src->format->BytesPerPixel;
        info.s_skip = src->pitch - info.width * src->format->BytesPerPixel;
        info.d_pixels = (Uint8 *) dst->pixels + dst->offset +
            (Uint16) dstrect->y * dst->pitch +
            (Uint16) dstrect->x * dst->format->BytesPerPixel;
        info.d_pxskip = dst->format->BytesPerPixel;
        info.d_skip = dst->pitch - info.width * dst->format->BytesPerPixel;
        info.src = src->format;
        info.dst = dst->format;
        info.src_flags = src->flags;
        info.dst_flags = dst->flags;

        if (info.d_pixels > info.s_pixels)
        {
            int span = info.width * info.src->BytesPerPixel;
            Uint8 *srcpixend =
                info.s_pixels + (info.height - 1) * src->pitch + span;

            if (info.d_pixels < srcpixend)
            {
                int dstoffset = (info.d_pixels - info.s_pixels) % src->pitch;

                if (dstoffset < span || dstoffset > src->pitch - span)
                {
                    /* Overlapping Self blit with positive destination offset.
                       Reverse direction of the blit.
                    */
                    info.s_pixels = srcpixend - info.s_pxskip;
                    info.s_pxskip = -info.s_pxskip;
                    info.s_skip = -info.s_skip;
                    info.d_pixels = (info.d_pixels +
                                     (info.height - 1) * dst->pitch +
                                     span - info.d_pxskip);
                    info.d_pxskip = -info.d_pxskip;
                    info.d_skip = -info.d_skip;
                }
            }
        }

        switch (the_args)
        {
        case 0:
        {
            if (src->flags & SDL_SRCALPHA && src->format->Amask)
                alphablit_alpha (&info);
            else if (src->flags & SDL_SRCCOLORKEY)
                alphablit_colorkey (&info);
            else
                alphablit_solid (&info);
            break;
        }
        case PYGAME_BLEND_ADD:
        {
            blit_blend_add (&info);
            break;
        }
        case PYGAME_BLEND_SUB:
        {
            blit_blend_sub (&info);
            break;
        }
        case PYGAME_BLEND_MULT:
        {
            blit_blend_mul (&info);
            break;
        }
        case PYGAME_BLEND_MIN:
        {
            blit_blend_min (&info);
            break;
        }
        case PYGAME_BLEND_MAX:
        {
            blit_blend_max (&info);
            break;
        }

        case PYGAME_BLEND_RGBA_ADD:
        {
        blit_blend_rgba_add (&info);
        break;
        }
        case PYGAME_BLEND_RGBA_SUB:
        {
            blit_blend_rgba_sub (&info);
            break;
        }
        case PYGAME_BLEND_RGBA_MULT:
        {
            blit_blend_rgba_mul (&info);
            break;
        }
        case PYGAME_BLEND_RGBA_MIN:
        {
            blit_blend_rgba_min (&info);
            break;
        }
        case PYGAME_BLEND_RGBA_MAX:
        {
            blit_blend_rgba_max (&info);
            break;
        }
        case PYGAME_BLEND_PREMULTIPLIED:
        {
            blit_blend_premultiplied (&info);
            break;
        }




        default:
        {
            SDL_SetError ("Invalid argument passed to blit.");
            okay = 0;
            break;
        }
        }
    }
    /* We need to unlock the surfaces if they're locked */
    if (dst_locked)
        SDL_UnlockSurface (dst);
    if (src_locked)
        SDL_UnlockSurface (src);
    /* Blit is done! */
    return (okay ? 0 : -1);
}








/* --------------------------------------------------------- */


static void
blit_blend_rgba_add (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    Uint32          tmp;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (!dstppa)
    {
    blit_blend_add (info);
    return;
    }

    if (srcbpp == 4 && dstbpp == 4 &&
    srcfmt->Rmask == dstfmt->Rmask &&
    srcfmt->Gmask == dstfmt->Gmask &&
    srcfmt->Bmask == dstfmt->Bmask &&
    srcfmt->Amask == dstfmt->Amask &&
    info->src_flags & SDL_SRCALPHA)
    {
    int incr = srcpxskip > 0 ? 1 : -1;
    if (incr < 0)
    {
        src += 3;
        dst += 3;
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                REPEAT_4(
                {
                    tmp = (*dst) + (*src);
                    (*dst) = (tmp <= 255 ? tmp : 255);
                    src += incr;
                    dst += incr;
                });
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_rgba_sub (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    Sint32          tmp2;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (!dstppa)
    {
    blit_blend_sub (info);
    return;
    }

    if (srcbpp == 4 && dstbpp == 4 &&
    srcfmt->Rmask == dstfmt->Rmask &&
    srcfmt->Gmask == dstfmt->Gmask &&
    srcfmt->Bmask == dstfmt->Bmask &&
    srcfmt->Amask == dstfmt->Amask &&
    info->src_flags & SDL_SRCALPHA)
    {
    int incr = srcpxskip > 0 ? 1 : -1;
    if (incr < 0)
    {
        src += 3;
        dst += 3;
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                REPEAT_4(
                {
                    tmp2 = (*dst) - (*src);
                    (*dst) = (tmp2 >= 0 ? tmp2 : 0);
                    src += incr;
                    dst += incr;
                });
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_rgba_mul (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    Uint32          tmp;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (!dstppa)
    {
    blit_blend_mul (info);
    return;
    }

    if (srcbpp == 4 && dstbpp == 4 &&
    srcfmt->Rmask == dstfmt->Rmask &&
    srcfmt->Gmask == dstfmt->Gmask &&
    srcfmt->Bmask == dstfmt->Bmask &&
    srcfmt->Amask == dstfmt->Amask &&
    info->src_flags & SDL_SRCALPHA)
    {
    int incr = srcpxskip > 0 ? 1 : -1;
    if (incr < 0)
    {
        src += 3;
        dst += 3;
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                REPEAT_4(
                {
                    tmp = ((*dst) && (*src)) ? ((*dst) * (*src)) >> 8 : 0;
                    (*dst) = (tmp <= 255 ? tmp : 255);
                    src += incr;
                    dst += incr;
                });
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_rgba_min (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (!dstppa)
    {
    blit_blend_min (info);
    return;
    }

    if (srcbpp == 4 && dstbpp == 4 &&
    srcfmt->Rmask == dstfmt->Rmask &&
    srcfmt->Gmask == dstfmt->Gmask &&
    srcfmt->Bmask == dstfmt->Bmask &&
    srcfmt->Amask == dstfmt->Amask &&
    info->src_flags & SDL_SRCALPHA)
    {
    int incr = srcpxskip > 0 ? 1 : -1;
    if (incr < 0)
    {
        src += 3;
        dst += 3;
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                REPEAT_4(
                {
                    if ((*src) < (*dst))
                        (*dst) = (*src);
                    src += incr;
                    dst += incr;
                });
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_rgba_max (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (!dstppa)
    {
    blit_blend_max (info);
    return;
    }

    if (srcbpp == 4 && dstbpp == 4 &&
    srcfmt->Rmask == dstfmt->Rmask &&
    srcfmt->Gmask == dstfmt->Gmask &&
    srcfmt->Bmask == dstfmt->Bmask &&
    srcfmt->Amask == dstfmt->Amask &&
    info->src_flags & SDL_SRCALPHA)
    {
    int incr = srcpxskip > 0 ? 1 : -1;
    if (incr < 0)
    {
        src += 3;
        dst += 3;
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                REPEAT_4(
                {
                    if ((*src) > (*dst))
                        (*dst) = (*src);
                    src += incr;
                    dst += incr;
                });
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }


    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_RGBA_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_RGBA_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_premultiplied (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);
    int tmp;

    /*
    printf ("Premultiplied alpha blit with %d and %d\n", srcbpp, dstbpp);
    */

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND_PREMULTIPLIED (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND_PREMULTIPLIED (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND_PREMULTIPLIED (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND_PREMULTIPLIED (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}













/* --------------------------------------------------------- */


static void
blit_blend_add (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    Uint32          tmp;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (srcbpp >= 3 && dstbpp >= 3 && !(info->src_flags & SDL_SRCALPHA))
    {
    size_t srcoffsetR, srcoffsetG, srcoffsetB;
    size_t dstoffsetR, dstoffsetG, dstoffsetB;
    if (srcbpp == 3)
    {
        SET_OFFSETS_24 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    else
    {
        SET_OFFSETS_32 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    if (dstbpp == 3)
    {
        SET_OFFSETS_24 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
    else
    {
        SET_OFFSETS_32 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
        tmp = dst[dstoffsetR] + src[srcoffsetR];
        dst[dstoffsetR] = (tmp <= 255 ? tmp : 255);
        tmp = dst[dstoffsetG] + src[srcoffsetG];
        dst[dstoffsetG] = (tmp <= 255 ? tmp : 255);
        tmp = dst[dstoffsetB] + src[srcoffsetB];
        dst[dstoffsetB] = (tmp <= 255 ? tmp : 255);
                src += srcpxskip;
                dst += dstpxskip;
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
            GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
            GET_PIXEL(pixel, srcbpp, src);
            GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_ADD (tmp, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_sub (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    Sint32          tmp2;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (srcbpp >= 3 && dstbpp >= 3 && !(info->src_flags & SDL_SRCALPHA))
    {
    size_t srcoffsetR, srcoffsetG, srcoffsetB;
    size_t dstoffsetR, dstoffsetG, dstoffsetB;
    if (srcbpp == 3)
    {
        SET_OFFSETS_24 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    else
    {
        SET_OFFSETS_32 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    if (dstbpp == 3)
    {
        SET_OFFSETS_24 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
    else
    {
        SET_OFFSETS_32 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
        tmp2 = dst[dstoffsetR] - src[srcoffsetR];
        dst[dstoffsetR] = (tmp2 >= 0 ? tmp2 : 0);
        tmp2 = dst[dstoffsetG] - src[srcoffsetG];
        dst[dstoffsetG] = (tmp2 >= 0 ? tmp2 : 0);
        tmp2 = dst[dstoffsetB] - src[srcoffsetB];
        dst[dstoffsetB] = (tmp2 >= 0 ? tmp2 : 0);
                src += srcpxskip;
                dst += dstpxskip;
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
            GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstfmt);
            BLEND_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
        {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_SUB (tmp2, sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_mul (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    Uint32          tmp;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (srcbpp >= 3 && dstbpp >= 3 && !(info->src_flags & SDL_SRCALPHA))
    {
    size_t srcoffsetR, srcoffsetG, srcoffsetB;
    size_t dstoffsetR, dstoffsetG, dstoffsetB;
    if (srcbpp == 3)
    {
        SET_OFFSETS_24 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    else
    {
        SET_OFFSETS_32 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    if (dstbpp == 3)
    {
        SET_OFFSETS_24 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
    else
    {
        SET_OFFSETS_32 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
        tmp = ((dst[dstoffsetR] && src[srcoffsetR]) ?
               (dst[dstoffsetR] * src[srcoffsetR]) >> 8 : 0);
        dst[dstoffsetR] = (tmp <= 255 ? tmp : 255);
        tmp = ((dst[dstoffsetG] && src[srcoffsetG]) ?
               (dst[dstoffsetG] * src[srcoffsetG]) >> 8 : 0);
        dst[dstoffsetG] = (tmp <= 255 ? tmp : 255);
        tmp = ((dst[dstoffsetB] && src[srcoffsetB]) ?
               (dst[dstoffsetB] * src[srcoffsetB]) >> 8 : 0);
        dst[dstoffsetB] = (tmp <= 255 ? tmp : 255);
                src += srcpxskip;
                dst += dstpxskip;
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
            GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_MULT (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_min (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (srcbpp >= 3 && dstbpp >= 3 && !(info->src_flags & SDL_SRCALPHA))
    {
    size_t srcoffsetR, srcoffsetG, srcoffsetB;
    size_t dstoffsetR, dstoffsetG, dstoffsetB;
    if (srcbpp == 3)
    {
        SET_OFFSETS_24 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    else
    {
        SET_OFFSETS_32 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    if (dstbpp == 3)
    {
        SET_OFFSETS_24 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
    else
    {
        SET_OFFSETS_32 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
        if (src[srcoffsetR] < dst[dstoffsetR])
        {
            dst[dstoffsetR] = src[srcoffsetR];
        }
        if (src[srcoffsetG] < dst[dstoffsetG])
        {
            dst[dstoffsetG] = src[srcoffsetG];
        }
        if (src[srcoffsetB] < dst[dstoffsetB])
        {
            dst[dstoffsetB] = src[srcoffsetB];
        }
                src += srcpxskip;
                dst += dstpxskip;
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
            GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_MIN (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
blit_blend_max (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    Uint8           dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    if (srcbpp >= 3 && dstbpp >= 3 && !(info->src_flags & SDL_SRCALPHA))
    {
    size_t srcoffsetR, srcoffsetG, srcoffsetB;
    size_t dstoffsetR, dstoffsetG, dstoffsetB;
    if (srcbpp == 3)
    {
        SET_OFFSETS_24 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    else
    {
        SET_OFFSETS_32 (srcoffsetR, srcoffsetG, srcoffsetB, srcfmt);
    }
    if (dstbpp == 3)
    {
        SET_OFFSETS_24 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
    else
    {
        SET_OFFSETS_32 (dstoffsetR, dstoffsetG, dstoffsetB, dstfmt);
    }
        while (height--)
        {
            LOOP_UNROLLED4(
            {
        if (src[srcoffsetR] > dst[dstoffsetR])
        {
            dst[dstoffsetR] = src[srcoffsetR];
        }
        if (src[srcoffsetG] > dst[dstoffsetG])
        {
            dst[dstoffsetG] = src[srcoffsetG];
        }
        if (src[srcoffsetB] > dst[dstoffsetB])
        {
            dst[dstoffsetB] = src[srcoffsetB];
        }
                src += srcpxskip;
                dst += dstpxskip;
            }, n, width);
            src += srcskip;
            dst += dstskip;
        }
        return;
    }

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
            GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    BLEND_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            BLEND_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    BLEND_MAX (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}





/* --------------------------------------------------------- */




























static void
alphablit_alpha (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    int             dR, dG, dB, dA, sR, sG, sB, sA;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    /*
    printf ("Alpha blit with %d and %d\n", srcbpp, dstbpp);
    */

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
alphablit_colorkey (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    int             dR, dG, dB, dA, sR, sG, sB, sA;
    int             alpha = srcfmt->alpha;
    Uint32          colorkey = srcfmt->colorkey;
    Uint32          pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    /*
    printf ("Colorkey blit with %d and %d\n", srcbpp, dstbpp);
    */

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    sA = (*src == colorkey) ? 0 : alpha;
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    sA = (*src == colorkey) ? 0 : alpha;
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    sA = (pixel == colorkey) ? 0 : alpha;
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
    {
            /* This is interim code until SDL can properly handle self
           blits of surfaces with blanket alpha.
        */
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
            sA = (pixel == colorkey) ? 0 : alpha;
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    sA = (pixel == colorkey) ? 0 : alpha;
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND (sR, sG, sB, sA, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

static void
alphablit_solid (SDL_BlitInfo * info)
{
    int             n;
    int             width = info->width;
    int             height = info->height;
    Uint8          *src = info->s_pixels;
    int             srcpxskip = info->s_pxskip;
    int             srcskip = info->s_skip;
    Uint8          *dst = info->d_pixels;
    int             dstpxskip = info->d_pxskip;
    int             dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    int             srcbpp = srcfmt->BytesPerPixel;
    int             dstbpp = dstfmt->BytesPerPixel;
    int             dR, dG, dB, dA, sR, sG, sB, sA;
    int             alpha = srcfmt->alpha;
    int             pixel;
    int             srcppa = (info->src_flags & SDL_SRCALPHA && srcfmt->Amask);
    int             dstppa = (info->dst_flags & SDL_SRCALPHA && dstfmt->Amask);

    /*
    printf ("Solid blit with %d and %d\n", srcbpp, dstbpp);
    */

    if (srcbpp == 1)
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
        else /* dstbpp > 1 */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXELVALS_1(sR, sG, sB, sA, src, srcfmt);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
    else /* srcbpp > 1 */
    {
        if (dstbpp == 1)
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXELVALS_1(dR, dG, dB, dA, dst, dstfmt);
                    ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB, dA);
            *dst = (Uint8) SDL_MapRGB (dstfmt, dR, dG, dB);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n, width);
                src += srcskip;
                dst += dstskip;
            }

        }
    else if (dstbpp == 3)
    {
            /* This is interim code until SDL can properly handle self
           blits of surfaces with blanket alpha.
        */
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, dstfmt);
        while (height--)
        {
        LOOP_UNROLLED4(
        {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
            GET_PIXEL (pixel, dstbpp, dst);
            GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
            ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB, dA);
            dst[offsetR] = dR;
            dst[offsetG] = dG;
            dst[offsetB] = dB;
            src += srcpxskip;
            dst += dstpxskip;
        }, n, width);
        src += srcskip;
        dst += dstskip;
        }
    }
        else /* even dstbpp */
        {
            while (height--)
            {
                LOOP_UNROLLED4(
                {
                    GET_PIXEL(pixel, srcbpp, src);
                    GET_PIXELVALS (sR, sG, sB, sA, pixel, srcfmt, srcppa);
                    GET_PIXEL (pixel, dstbpp, dst);
                    GET_PIXELVALS (dR, dG, dB, dA, pixel, dstfmt, dstppa);
                    ALPHA_BLEND (sR, sG, sB, alpha, dR, dG, dB, dA);
                    CREATE_PIXEL(dst, dR, dG, dB, dA, dstbpp, dstfmt);
                    src += srcpxskip;
                    dst += dstpxskip;
                }, n ,width);
                src += srcskip;
                dst += dstskip;
            }
        }
    }
}

/*we assume the "dst" has pixel alpha*/
int
pygame_Blit (SDL_Surface * src, SDL_Rect * srcrect,
             SDL_Surface * dst, SDL_Rect * dstrect, int the_args)
{
    SDL_Rect        fulldst;
    int             srcx, srcy, w, h;

    /* Make sure the surfaces aren't locked */
    if (!src || !dst)
    {
        SDL_SetError ("SDL_UpperBlit: passed a NULL surface");
        return (-1);
    }
    if (src->locked || dst->locked)
    {
        SDL_SetError ("Surfaces must not be locked during blit");
        return (-1);
    }

    /* If the destination rectangle is NULL, use the entire dest surface */
    if (dstrect == NULL)
    {
        fulldst.x = fulldst.y = 0;
        dstrect = &fulldst;
    }

    /* clip the source rectangle to the source surface */
    if (srcrect)
    {
        int             maxw, maxh;

        srcx = srcrect->x;
        w = srcrect->w;
        if (srcx < 0)
        {
            w += srcx;
            dstrect->x -= srcx;
            srcx = 0;
        }
        maxw = src->w - srcx;
        if (maxw < w)
            w = maxw;

        srcy = srcrect->y;
        h = srcrect->h;
        if (srcy < 0)
        {
            h += srcy;
            dstrect->y -= srcy;
            srcy = 0;
        }
        maxh = src->h - srcy;
        if (maxh < h)
            h = maxh;

    }
    else
    {
        srcx = srcy = 0;
        w = src->w;
        h = src->h;
    }

    /* clip the destination rectangle against the clip rectangle */
    {
        SDL_Rect       *clip = &dst->clip_rect;
        int             dx, dy;

        dx = clip->x - dstrect->x;
        if (dx > 0)
        {
            w -= dx;
            dstrect->x += dx;
            srcx += dx;
        }
        dx = dstrect->x + w - clip->x - clip->w;
        if (dx > 0)
            w -= dx;

        dy = clip->y - dstrect->y;
        if (dy > 0)
        {
            h -= dy;
            dstrect->y += dy;
            srcy += dy;
        }
        dy = dstrect->y + h - clip->y - clip->h;
        if (dy > 0)
            h -= dy;
    }

    if (w > 0 && h > 0)
    {
        SDL_Rect        sr;

        sr.x = srcx;
        sr.y = srcy;
        sr.w = dstrect->w = w;
        sr.h = dstrect->h = h;
        return SoftBlitPyGame (src, &sr, dst, dstrect, the_args);
    }
    dstrect->w = dstrect->h = 0;
    return 0;
}

int
pygame_AlphaBlit (SDL_Surface * src, SDL_Rect * srcrect,
                  SDL_Surface * dst, SDL_Rect * dstrect, int the_args)
{
    return pygame_Blit (src, srcrect, dst, dstrect, the_args);
}


    /*
  pygame - Python Game Library
  Copyright (C) 2007 Marcus von Appen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#define NO_PYGAME_C_API

/*
 * Changes SDL_Rect to respect any clipping rect defined on the surface.
 * Necessary when modifying surface->pixels directly instead of through an
 * SDL interface.
 */
void surface_respect_clip_rect (SDL_Surface *surface, SDL_Rect *rect)
{
    SDL_Rect tmp;
    SDL_Rect *A, *B;
    int x, y, w, h;

    A = rect;
    B = &tmp;
    SDL_GetClipRect(surface, B);

    /* Code here is nearly identical to rect_clip in rect.c */

    /* Left */
    if ((A->x >= B->x) && (A->x < (B->x + B->w)))
        x = A->x;
    else if ((B->x >= A->x) && (B->x < (A->x + A->w)))
        x = B->x;
    else
        return;

    /* Right */
    if (((A->x + A->w) > B->x) && ((A->x + A->w) <= (B->x + B->w)))
        w = (A->x + A->w) - x;
    else if (((B->x + B->w) > A->x) && ((B->x + B->w) <= (A->x + A->w)))
        w = (B->x + B->w) - x;
    else
        return;

    /* Top */
    if ((A->y >= B->y) && (A->y < (B->y + B->h)))
        y = A->y;
    else if ((B->y >= A->y) && (B->y < (A->y + A->h)))
        y = B->y;
    else
        return;

    /* Bottom */
    if (((A->y + A->h) > B->y) && ((A->y + A->h) <= (B->y + B->h)))
        h = (A->y + A->h) - y;
    else if (((B->y + B->h) > A->y) && ((B->y + B->h) <= (A->y + A->h)))
        h = (B->y + B->h) - y;
    else
        return;

    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

static int
surface_fill_blend_add (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    Uint32 tmp;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_ADD (tmp, cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    case 3:
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, fmt);
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_ADD (tmp, cR, cG, cB, cA, sR, sG, sB, sA);
        pixels[offsetR] = sR;
        pixels[offsetG] = sG;
        pixels[offsetB] = sB;
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_ADD (tmp, cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_sub (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    Sint32 tmp2;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_SUB (tmp2, cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    case 3:
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, fmt);
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_SUB (tmp2, cR, cG, cB, cA, sR, sG, sB, sA);
        pixels[offsetR] = sR;
        pixels[offsetG] = sG;
        pixels[offsetB] = sB;
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_SUB (tmp2, cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_mult (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_MULT (cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    case 3:
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, fmt);
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_MULT (cR, cG, cB, cA, sR, sG, sB, sA);
        pixels[offsetR] = sR;
        pixels[offsetG] = sG;
        pixels[offsetB] = sB;
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_MULT (cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_min (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_MIN (cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    case 3:
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, fmt);
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_MIN (cR, cG, cB, cA, sR, sG, sB, sA);
        pixels[offsetR] = sR;
        pixels[offsetG] = sG;
        pixels[offsetB] = sB;
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_MIN (cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_max (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_MAX (cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    case 3:
    {
        size_t offsetR, offsetG, offsetB;
        SET_OFFSETS_24 (offsetR, offsetG, offsetB, fmt);
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_MAX (cR, cG, cB, cA, sR, sG, sB, sA);
        pixels[offsetR] = sR;
        pixels[offsetG] = sG;
        pixels[offsetB] = sB;
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_MAX (cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}


/* ------------------------- */

static int
surface_fill_blend_rgba_add (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    Uint32 tmp;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    if (!ppa)
    {
        return surface_fill_blend_add (surface, rect, color);
    }

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_RGBA_ADD (tmp, cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_RGBA_ADD (tmp, cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_rgba_sub (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    Sint32 tmp2;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    if (!ppa)
    {
        return surface_fill_blend_sub (surface, rect, color);
    }

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_RGBA_SUB (tmp2, cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_RGBA_SUB (tmp2, cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_rgba_mult (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    if (!ppa)
    {
        return surface_fill_blend_mult (surface, rect, color);
    }

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_RGBA_MULT (cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_RGBA_MULT (cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_rgba_min (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    if (!ppa)
    {
        return surface_fill_blend_min (surface, rect, color);
    }

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_RGBA_MIN (cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_RGBA_MIN (cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}

static int
surface_fill_blend_rgba_max (SDL_Surface *surface, SDL_Rect *rect, Uint32 color)
{
    Uint8 *pixels;
    int width = rect->w;
    int height = rect->h;
    int skip;
    int bpp = surface->format->BytesPerPixel;
    int n;
    SDL_PixelFormat *fmt = surface->format;
    Uint8 sR, sG, sB, sA, cR, cG, cB, cA;
    Uint32 pixel;
    int result = -1;
    int ppa = (surface->flags & SDL_SRCALPHA && fmt->Amask);

    if (!ppa)
    {
        return surface_fill_blend_max (surface, rect, color);
    }

    pixels = (Uint8 *) surface->pixels + surface->offset +
        (Uint16) rect->y * surface->pitch + (Uint16) rect->x * bpp;
    skip = surface->pitch - width * bpp;

    switch (bpp)
    {
    case 1:
    {
        SDL_GetRGBA (color, fmt, &cR, &cG, &cB, &cA);
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXELVALS_1 (sR, sG, sB, sA, pixels, fmt);
                BLEND_RGBA_MAX (cR, cG, cB, cA, sR, sG, sB, sA);
                *pixels = SDL_MapRGBA (fmt, sR, sG, sB, sA);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    default:
    {
        GET_PIXELVALS (cR, cG, cB, cA, color, fmt, ppa);
        /*
        printf ("Color: %d, %d, %d, %d, BPP is: %d\n", cR, cG, cB, cA, bpp);
        */
        while (height--)
        {
            LOOP_UNROLLED4(
            {
                GET_PIXEL (pixel, bpp, pixels);
                GET_PIXELVALS (sR, sG, sB, sA, pixel, fmt, ppa);
                BLEND_RGBA_MAX (cR, cG, cB, cA, sR, sG, sB, sA);
                CREATE_PIXEL(pixels, sR, sG, sB, sA, bpp, fmt);
                pixels += bpp;
            }, n, width);
            pixels += skip;
        }
        result = 0;
        break;
    }
    }
    return result;
}


int
surface_fill_blend (SDL_Surface *surface, SDL_Rect *rect, Uint32 color,
                    int blendargs)
{
    int result = -1;
    int locked = 0;

    surface_respect_clip_rect(surface, rect);

    /* Lock the surface, if needed */
    if (SDL_MUSTLOCK (surface))
    {
        if (SDL_LockSurface (surface) < 0)
            return -1;
        locked = 1;
    }

    switch (blendargs)
    {
    case PYGAME_BLEND_ADD:
    {
        result = surface_fill_blend_add (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_SUB:
    {
        result = surface_fill_blend_sub (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_MULT:
    {
        result =  surface_fill_blend_mult (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_MIN:
    {
        result =  surface_fill_blend_min (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_MAX:
    {
        result =  surface_fill_blend_max (surface, rect, color);
        break;
    }



    case PYGAME_BLEND_RGBA_ADD:
    {
        result = surface_fill_blend_rgba_add (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_RGBA_SUB:
    {
        result = surface_fill_blend_rgba_sub (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_RGBA_MULT:
    {
        result =  surface_fill_blend_rgba_mult (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_RGBA_MIN:
    {
        result =  surface_fill_blend_rgba_min (surface, rect, color);
        break;
    }
    case PYGAME_BLEND_RGBA_MAX:
    {
        result =  surface_fill_blend_rgba_max (surface, rect, color);
        break;
    }




    default:
    {
        result = -1;
        break;
    }
    }

    if (locked)
    {
        SDL_UnlockSurface (surface);
    }
    return result;
}


    /*
    pygame - Python Game Library
    Copyright (C) 2000-2001  Pete Shinners

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Pete Shinners
    pete@shinners.org
*/

/*
   This implements the AdvanceMAME Scale2x feature found on this page,
   http://advancemame.sourceforge.net/scale2x.html

   It is an incredibly simple and powerful image doubling routine that does
   an astonishing job of doubling game graphic data while interpolating out
   the jaggies. Congrats to the AdvanceMAME team, I'm very impressed and
   surprised with this code!
*/



#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
// already defined
//#define MIN(a,b)    (((a) < (b)) ? (a) : (b))


#define READINT24(x)      ((x)[0]<<16 | (x)[1]<<8 | (x)[2])
#define WRITEINT24(x, i)  {(x)[0]=i>>16; (x)[1]=(i>>8)&0xff; x[2]=i&0xff; }

/*
  this requires a destination surface already setup to be twice as
  large as the source. oh, and formats must match too. this will just
  blindly assume you didn't flounder.
*/

void scale2x(SDL_Surface *src, SDL_Surface *dst)
{
    int looph, loopw;

    Uint8* srcpix = (Uint8*)src->pixels;
    Uint8* dstpix = (Uint8*)dst->pixels;

    const int srcpitch = src->pitch;
    const int dstpitch = dst->pitch;
    const int width = src->w;
    const int height = src->h;

    switch(src->format->BytesPerPixel)
    {
    case 1: {
        Uint8 E0, E1, E2, E3, B, D, E, F, H;
        for(looph = 0; looph < height; ++looph)
        {
            for(loopw = 0; loopw < width; ++ loopw)
            {
                    B = *(Uint8*)(srcpix + (MAX(0,looph-1)*srcpitch) + (1*loopw));
                    D = *(Uint8*)(srcpix + (looph*srcpitch) + (1*MAX(0,loopw-1)));
                    E = *(Uint8*)(srcpix + (looph*srcpitch) + (1*loopw));
                    F = *(Uint8*)(srcpix + (looph*srcpitch) + (1*MIN(width-1,loopw+1)));
                    H = *(Uint8*)(srcpix + (MIN(height-1,looph+1)*srcpitch) + (1*loopw));

                E0 = D == B && B != F && D != H ? D : E;
                                E1 = B == F && B != D && F != H ? F : E;
                E2 = D == H && D != B && H != F ? D : E;
                E3 = H == F && D != H && B != F ? F : E;

                *(Uint8*)(dstpix + looph*2*dstpitch + loopw*2*1) = E0;
                *(Uint8*)(dstpix + looph*2*dstpitch + (loopw*2+1)*1) = E1;
                *(Uint8*)(dstpix + (looph*2+1)*dstpitch + loopw*2*1) = E2;
                *(Uint8*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*1) = E3;
            }
        }break;}
    case 2: {
        Uint16 E0, E1, E2, E3, B, D, E, F, H;
        for(looph = 0; looph < height; ++looph)
        {
            for(loopw = 0; loopw < width; ++ loopw)
            {
                    B = *(Uint16*)(srcpix + (MAX(0,looph-1)*srcpitch) + (2*loopw));
                    D = *(Uint16*)(srcpix + (looph*srcpitch) + (2*MAX(0,loopw-1)));
                    E = *(Uint16*)(srcpix + (looph*srcpitch) + (2*loopw));
                    F = *(Uint16*)(srcpix + (looph*srcpitch) + (2*MIN(width-1,loopw+1)));
                    H = *(Uint16*)(srcpix + (MIN(height-1,looph+1)*srcpitch) + (2*loopw));

                E0 = D == B && B != F && D != H ? D : E;
                                E1 = B == F && B != D && F != H ? F : E;
                E2 = D == H && D != B && H != F ? D : E;
                E3 = H == F && D != H && B != F ? F : E;

                *(Uint16*)(dstpix + looph*2*dstpitch + loopw*2*2) = E0;
                *(Uint16*)(dstpix + looph*2*dstpitch + (loopw*2+1)*2) = E1;
                *(Uint16*)(dstpix + (looph*2+1)*dstpitch + loopw*2*2) = E2;
                *(Uint16*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*2) = E3;
            }
        }break;}
    case 3: {
        int E0, E1, E2, E3, B, D, E, F, H;
        for(looph = 0; looph < height; ++looph)
        {
            for(loopw = 0; loopw < width; ++ loopw)
            {
                    B = READINT24(srcpix + (MAX(0,looph-1)*srcpitch) + (3*loopw));
                    D = READINT24(srcpix + (looph*srcpitch) + (3*MAX(0,loopw-1)));
                    E = READINT24(srcpix + (looph*srcpitch) + (3*loopw));
                    F = READINT24(srcpix + (looph*srcpitch) + (3*MIN(width-1,loopw+1)));
                    H = READINT24(srcpix + (MIN(height-1,looph+1)*srcpitch) + (3*loopw));

                E0 = D == B && B != F && D != H ? D : E;
                                E1 = B == F && B != D && F != H ? F : E;
                E2 = D == H && D != B && H != F ? D : E;
                E3 = H == F && D != H && B != F ? F : E;

                WRITEINT24((dstpix + looph*2*dstpitch + loopw*2*3), E0);
                WRITEINT24((dstpix + looph*2*dstpitch + (loopw*2+1)*3), E1);
                WRITEINT24((dstpix + (looph*2+1)*dstpitch + loopw*2*3), E2);
                WRITEINT24((dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*3), E3);
            }
        }break;}
    default: { /*case 4:*/
        Uint32 E0, E1, E2, E3, B, D, E, F, H;
        for(looph = 0; looph < height; ++looph)
        {
            for(loopw = 0; loopw < width; ++ loopw)
            {
                    B = *(Uint32*)(srcpix + (MAX(0,looph-1)*srcpitch) + (4*loopw));
                    D = *(Uint32*)(srcpix + (looph*srcpitch) + (4*MAX(0,loopw-1)));
                    E = *(Uint32*)(srcpix + (looph*srcpitch) + (4*loopw));
                    F = *(Uint32*)(srcpix + (looph*srcpitch) + (4*MIN(width-1,loopw+1)));
                    H = *(Uint32*)(srcpix + (MIN(height-1,looph+1)*srcpitch) + (4*loopw));

                E0 = D == B && B != F && D != H ? D : E;
                                E1 = B == F && B != D && F != H ? F : E;
                E2 = D == H && D != B && H != F ? D : E;
                E3 = H == F && D != H && B != F ? F : E;

                *(Uint32*)(dstpix + looph*2*dstpitch + loopw*2*4) = E0;
                *(Uint32*)(dstpix + looph*2*dstpitch + (loopw*2+1)*4) = E1;
                *(Uint32*)(dstpix + (looph*2+1)*dstpitch + loopw*2*4) = E2;
                *(Uint32*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*4) = E3;
            }
        }break;}
    }
}


    static void
rotate90 (SDL_Surface *src, SDL_Surface *dst, int angle)
{
    int numturns = (angle / 90) % 4;
    int dstwidth, dstheight;
    char *srcpix, *dstpix, *srcrow, *dstrow;
    int srcstepx, srcstepy, dststepx, dststepy;
    int loopx, loopy;

    dstwidth = dst->w;
    dstheight = dst->h;

    if (numturns < 0)
        numturns = 4 + numturns;

    srcrow = (char*) src->pixels;
    dstrow = (char*) dst->pixels;
    srcstepx = dststepx = src->format->BytesPerPixel;
    srcstepy = src->pitch;
    dststepy = dst->pitch;

    switch (numturns)
    {
    case 1:
        srcrow += ((src->w - 1) * srcstepx);
        srcstepy = -srcstepx;
        srcstepx = src->pitch;
        break;
    case 2:
        srcrow += ((src->h - 1) * srcstepy) + ((src->w - 1) * srcstepx);
        srcstepx = -srcstepx;
        srcstepy = -srcstepy;
        break;
    case 3:
        srcrow += ((src->h - 1) * srcstepy);
        srcstepx = -srcstepy;
        srcstepy = src->format->BytesPerPixel;
        break;
    }

    switch (src->format->BytesPerPixel)
    {
    case 1:
        for (loopy = 0; loopy < dstheight; ++loopy)
        {
            dstpix = dstrow;
            srcpix = srcrow;
            for (loopx = 0; loopx < dstwidth; ++loopx)
            {
                *dstpix = *srcpix;
                srcpix += srcstepx;
                dstpix += dststepx;
            }
            dstrow += dststepy;
            srcrow += srcstepy;
        }
        break;
    case 2:
        for (loopy = 0; loopy < dstheight; ++loopy)
        {
            dstpix = dstrow;
            srcpix = srcrow;
            for (loopx = 0; loopx < dstwidth; ++loopx)
            {
                *(Uint16*)dstpix = *(Uint16*)srcpix;
                srcpix += srcstepx;
                dstpix += dststepx;
            }
            dstrow += dststepy;
            srcrow += srcstepy;
        }
        break;
    case 3:
        for (loopy = 0; loopy < dstheight; ++loopy)
        {
            dstpix = dstrow;
            srcpix = srcrow;
            for (loopx = 0; loopx < dstwidth; ++loopx)
            {
                dstpix[0] = srcpix[0];
                dstpix[1] = srcpix[1];
                dstpix[2] = srcpix[2];
                srcpix += srcstepx;
                dstpix += dststepx;
            }
            dstrow += dststepy;
            srcrow += srcstepy;
        }
        break;
    case 4:
        for (loopy = 0; loopy < dstheight; ++loopy)
        {
            dstpix = dstrow;
            srcpix = srcrow;
            for (loopx = 0; loopx < dstwidth; ++loopx)
            {
                *(Uint32*)dstpix = *(Uint32*)srcpix;
                srcpix += srcstepx;
                dstpix += dststepx;
            }
            dstrow += dststepy;
            srcrow += srcstepy;
        }
        break;
    }
}


static void
rotate (SDL_Surface *src, SDL_Surface *dst, Uint32 bgcolor, double sangle,
        double cangle)
{
    int x, y, dx, dy;

    Uint8 *srcpix = (Uint8*) src->pixels;
    Uint8 *dstrow = (Uint8*) dst->pixels;
    int srcpitch = src->pitch;
    int dstpitch = dst->pitch;

    int cy = dst->h / 2;
    int xd = ((src->w - dst->w) << 15);
    int yd = ((src->h - dst->h) << 15);

    int isin = (int)(sangle * 65536);
    int icos = (int)(cangle * 65536);

    int ax = ((dst->w) << 15) - (int)(cangle * ((dst->w - 1) << 15));
    int ay = ((dst->h) << 15) - (int)(sangle * ((dst->w - 1) << 15));

    int xmaxval = ((src->w) << 16) - 1;
    int ymaxval = ((src->h) << 16) - 1;

    switch (src->format->BytesPerPixel)
    {
    case 1:
        for (y = 0; y < dst->h; y++)
        {
            Uint8 *dstpos = (Uint8*)dstrow;
            dx = (ax + (isin * (cy - y))) + xd;
            dy = (ay - (icos * (cy - y))) + yd;
            for (x = 0; x < dst->w; x++)
            {
                if(dx < 0 || dy < 0 || dx > xmaxval || dy > ymaxval)
                    *dstpos++ = bgcolor;
                else
                    *dstpos++ = *(Uint8*)
                        (srcpix + ((dy >> 16) * srcpitch) + (dx >> 16));
                dx += icos;
                dy += isin;
            }
            dstrow += dstpitch;
        }
        break;
    case 2:
        for (y = 0; y < dst->h; y++)
        {
            Uint16 *dstpos = (Uint16*)dstrow;
            dx = (ax + (isin * (cy - y))) + xd;
            dy = (ay - (icos * (cy - y))) + yd;
            for (x = 0; x < dst->w; x++)
            {
                if (dx < 0 || dy < 0 || dx > xmaxval || dy > ymaxval)
                    *dstpos++ = bgcolor;
                else
                    *dstpos++ = *(Uint16*)
                        (srcpix + ((dy >> 16) * srcpitch) + (dx >> 16 << 1));
                dx += icos;
                dy += isin;
            }
            dstrow += dstpitch;
        }
        break;
    case 4:
        for (y = 0; y < dst->h; y++)
        {
            Uint32 *dstpos = (Uint32*)dstrow;
            dx = (ax + (isin * (cy - y))) + xd;
            dy = (ay - (icos * (cy - y))) + yd;
            for (x = 0; x < dst->w; x++)
            {
                if (dx < 0 || dy < 0 || dx > xmaxval || dy > ymaxval)
                    *dstpos++ = bgcolor;
                else
                    *dstpos++ = *(Uint32*)
                        (srcpix + ((dy >> 16) * srcpitch) + (dx >> 16 << 2));
                dx += icos;
                dy += isin;
            }
            dstrow += dstpitch;
        }
        break;
    default: /*case 3:*/
        for (y = 0; y < dst->h; y++)
        {
            Uint8 *dstpos = (Uint8*)dstrow;
            dx = (ax + (isin * (cy - y))) + xd;
            dy = (ay - (icos * (cy - y))) + yd;
            for (x = 0; x < dst->w; x++)
            {
                if (dx < 0 || dy < 0 || dx > xmaxval || dy > ymaxval)
                {
                    dstpos[0] = ((Uint8*) &bgcolor)[0];
                    dstpos[1] = ((Uint8*) &bgcolor)[1];
                    dstpos[2] = ((Uint8*) &bgcolor)[2];
                    dstpos += 3;
                }
                else
                {
                    Uint8* srcpos = (Uint8*)
                        (srcpix + ((dy >> 16) * srcpitch) + ((dx >> 16) * 3));
                    dstpos[0] = srcpos[0];
                    dstpos[1] = srcpos[1];
                    dstpos[2] = srcpos[2];
                    dstpos += 3;
                }
                dx += icos; dy += isin;
            }
            dstrow += dstpitch;
        }
        break;
    }
}

    

static int _cffi_const_SDLK_UNKNOWN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_UNKNOWN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_UNKNOWN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return 0;
}

static int _cffi_const_SDLK_LAST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LAST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LAST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_UNKNOWN(lib);
}

static int _cffi_const_KMOD_NONE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_NONE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_NONE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LAST(lib);
}

static int _cffi_const_KMOD_RESERVED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_RESERVED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_RESERVED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_NONE(lib);
}

static int _cffi_e__SDL_EventType(PyObject *lib)
{
  if ((SDL_NOEVENT) < 0 || (unsigned long)(SDL_NOEVENT) != 0UL) {
    char buf[64];
    if ((SDL_NOEVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_NOEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_NOEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_NOEVENT", buf, "0");
    return -1;
  }
  if ((SDL_ACTIVEEVENT) < 0 || (unsigned long)(SDL_ACTIVEEVENT) != 1UL) {
    char buf[64];
    if ((SDL_ACTIVEEVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_ACTIVEEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_ACTIVEEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_ACTIVEEVENT", buf, "1");
    return -1;
  }
  if ((SDL_KEYDOWN) < 0 || (unsigned long)(SDL_KEYDOWN) != 2UL) {
    char buf[64];
    if ((SDL_KEYDOWN) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_KEYDOWN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_KEYDOWN));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_KEYDOWN", buf, "2");
    return -1;
  }
  if ((SDL_KEYUP) < 0 || (unsigned long)(SDL_KEYUP) != 3UL) {
    char buf[64];
    if ((SDL_KEYUP) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_KEYUP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_KEYUP));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_KEYUP", buf, "3");
    return -1;
  }
  if ((SDL_MOUSEMOTION) < 0 || (unsigned long)(SDL_MOUSEMOTION) != 4UL) {
    char buf[64];
    if ((SDL_MOUSEMOTION) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_MOUSEMOTION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_MOUSEMOTION));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_MOUSEMOTION", buf, "4");
    return -1;
  }
  if ((SDL_MOUSEBUTTONDOWN) < 0 || (unsigned long)(SDL_MOUSEBUTTONDOWN) != 5UL) {
    char buf[64];
    if ((SDL_MOUSEBUTTONDOWN) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_MOUSEBUTTONDOWN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_MOUSEBUTTONDOWN));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_MOUSEBUTTONDOWN", buf, "5");
    return -1;
  }
  if ((SDL_MOUSEBUTTONUP) < 0 || (unsigned long)(SDL_MOUSEBUTTONUP) != 6UL) {
    char buf[64];
    if ((SDL_MOUSEBUTTONUP) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_MOUSEBUTTONUP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_MOUSEBUTTONUP));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_MOUSEBUTTONUP", buf, "6");
    return -1;
  }
  if ((SDL_JOYAXISMOTION) < 0 || (unsigned long)(SDL_JOYAXISMOTION) != 7UL) {
    char buf[64];
    if ((SDL_JOYAXISMOTION) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_JOYAXISMOTION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_JOYAXISMOTION));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_JOYAXISMOTION", buf, "7");
    return -1;
  }
  if ((SDL_JOYBALLMOTION) < 0 || (unsigned long)(SDL_JOYBALLMOTION) != 8UL) {
    char buf[64];
    if ((SDL_JOYBALLMOTION) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_JOYBALLMOTION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_JOYBALLMOTION));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_JOYBALLMOTION", buf, "8");
    return -1;
  }
  if ((SDL_JOYHATMOTION) < 0 || (unsigned long)(SDL_JOYHATMOTION) != 9UL) {
    char buf[64];
    if ((SDL_JOYHATMOTION) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_JOYHATMOTION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_JOYHATMOTION));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_JOYHATMOTION", buf, "9");
    return -1;
  }
  if ((SDL_JOYBUTTONDOWN) < 0 || (unsigned long)(SDL_JOYBUTTONDOWN) != 10UL) {
    char buf[64];
    if ((SDL_JOYBUTTONDOWN) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_JOYBUTTONDOWN));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_JOYBUTTONDOWN));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_JOYBUTTONDOWN", buf, "10");
    return -1;
  }
  if ((SDL_JOYBUTTONUP) < 0 || (unsigned long)(SDL_JOYBUTTONUP) != 11UL) {
    char buf[64];
    if ((SDL_JOYBUTTONUP) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_JOYBUTTONUP));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_JOYBUTTONUP));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_JOYBUTTONUP", buf, "11");
    return -1;
  }
  if ((SDL_QUIT) < 0 || (unsigned long)(SDL_QUIT) != 12UL) {
    char buf[64];
    if ((SDL_QUIT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_QUIT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_QUIT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_QUIT", buf, "12");
    return -1;
  }
  if ((SDL_SYSWMEVENT) < 0 || (unsigned long)(SDL_SYSWMEVENT) != 13UL) {
    char buf[64];
    if ((SDL_SYSWMEVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_SYSWMEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_SYSWMEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_SYSWMEVENT", buf, "13");
    return -1;
  }
  if ((SDL_EVENT_RESERVEDA) < 0 || (unsigned long)(SDL_EVENT_RESERVEDA) != 14UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVEDA) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVEDA));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVEDA));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVEDA", buf, "14");
    return -1;
  }
  if ((SDL_EVENT_RESERVEDB) < 0 || (unsigned long)(SDL_EVENT_RESERVEDB) != 15UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVEDB) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVEDB));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVEDB));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVEDB", buf, "15");
    return -1;
  }
  if ((SDL_VIDEORESIZE) < 0 || (unsigned long)(SDL_VIDEORESIZE) != 16UL) {
    char buf[64];
    if ((SDL_VIDEORESIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_VIDEORESIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_VIDEORESIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_VIDEORESIZE", buf, "16");
    return -1;
  }
  if ((SDL_VIDEOEXPOSE) < 0 || (unsigned long)(SDL_VIDEOEXPOSE) != 17UL) {
    char buf[64];
    if ((SDL_VIDEOEXPOSE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_VIDEOEXPOSE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_VIDEOEXPOSE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_VIDEOEXPOSE", buf, "17");
    return -1;
  }
  if ((SDL_EVENT_RESERVED2) < 0 || (unsigned long)(SDL_EVENT_RESERVED2) != 18UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVED2) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVED2));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVED2));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVED2", buf, "18");
    return -1;
  }
  if ((SDL_EVENT_RESERVED3) < 0 || (unsigned long)(SDL_EVENT_RESERVED3) != 19UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVED3) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVED3));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVED3));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVED3", buf, "19");
    return -1;
  }
  if ((SDL_EVENT_RESERVED4) < 0 || (unsigned long)(SDL_EVENT_RESERVED4) != 20UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVED4) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVED4));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVED4));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVED4", buf, "20");
    return -1;
  }
  if ((SDL_EVENT_RESERVED5) < 0 || (unsigned long)(SDL_EVENT_RESERVED5) != 21UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVED5) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVED5));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVED5));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVED5", buf, "21");
    return -1;
  }
  if ((SDL_EVENT_RESERVED6) < 0 || (unsigned long)(SDL_EVENT_RESERVED6) != 22UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVED6) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVED6));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVED6));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVED6", buf, "22");
    return -1;
  }
  if ((SDL_EVENT_RESERVED7) < 0 || (unsigned long)(SDL_EVENT_RESERVED7) != 23UL) {
    char buf[64];
    if ((SDL_EVENT_RESERVED7) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_EVENT_RESERVED7));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_EVENT_RESERVED7));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_EVENT_RESERVED7", buf, "23");
    return -1;
  }
  if ((SDL_USEREVENT) < 0 || (unsigned long)(SDL_USEREVENT) != 24UL) {
    char buf[64];
    if ((SDL_USEREVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_USEREVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_USEREVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_USEREVENT", buf, "24");
    return -1;
  }
  if ((SDL_NUMEVENTS) < 0 || (unsigned long)(SDL_NUMEVENTS) != 32UL) {
    char buf[64];
    if ((SDL_NUMEVENTS) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_NUMEVENTS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_NUMEVENTS));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_EventType", "SDL_NUMEVENTS", buf, "32");
    return -1;
  }
  return 0;
}

static int _cffi_e__SDL_GLattr(PyObject *lib)
{
  if ((SDL_GL_RED_SIZE) < 0 || (unsigned long)(SDL_GL_RED_SIZE) != 0UL) {
    char buf[64];
    if ((SDL_GL_RED_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_RED_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_RED_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_RED_SIZE", buf, "0");
    return -1;
  }
  if ((SDL_GL_GREEN_SIZE) < 0 || (unsigned long)(SDL_GL_GREEN_SIZE) != 1UL) {
    char buf[64];
    if ((SDL_GL_GREEN_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_GREEN_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_GREEN_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_GREEN_SIZE", buf, "1");
    return -1;
  }
  if ((SDL_GL_BLUE_SIZE) < 0 || (unsigned long)(SDL_GL_BLUE_SIZE) != 2UL) {
    char buf[64];
    if ((SDL_GL_BLUE_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_BLUE_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_BLUE_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_BLUE_SIZE", buf, "2");
    return -1;
  }
  if ((SDL_GL_ALPHA_SIZE) < 0 || (unsigned long)(SDL_GL_ALPHA_SIZE) != 3UL) {
    char buf[64];
    if ((SDL_GL_ALPHA_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_ALPHA_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_ALPHA_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_ALPHA_SIZE", buf, "3");
    return -1;
  }
  if ((SDL_GL_BUFFER_SIZE) < 0 || (unsigned long)(SDL_GL_BUFFER_SIZE) != 4UL) {
    char buf[64];
    if ((SDL_GL_BUFFER_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_BUFFER_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_BUFFER_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_BUFFER_SIZE", buf, "4");
    return -1;
  }
  if ((SDL_GL_DOUBLEBUFFER) < 0 || (unsigned long)(SDL_GL_DOUBLEBUFFER) != 5UL) {
    char buf[64];
    if ((SDL_GL_DOUBLEBUFFER) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_DOUBLEBUFFER));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_DOUBLEBUFFER));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_DOUBLEBUFFER", buf, "5");
    return -1;
  }
  if ((SDL_GL_DEPTH_SIZE) < 0 || (unsigned long)(SDL_GL_DEPTH_SIZE) != 6UL) {
    char buf[64];
    if ((SDL_GL_DEPTH_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_DEPTH_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_DEPTH_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_DEPTH_SIZE", buf, "6");
    return -1;
  }
  if ((SDL_GL_STENCIL_SIZE) < 0 || (unsigned long)(SDL_GL_STENCIL_SIZE) != 7UL) {
    char buf[64];
    if ((SDL_GL_STENCIL_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_STENCIL_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_STENCIL_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_STENCIL_SIZE", buf, "7");
    return -1;
  }
  if ((SDL_GL_ACCUM_RED_SIZE) < 0 || (unsigned long)(SDL_GL_ACCUM_RED_SIZE) != 8UL) {
    char buf[64];
    if ((SDL_GL_ACCUM_RED_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_ACCUM_RED_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_ACCUM_RED_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_ACCUM_RED_SIZE", buf, "8");
    return -1;
  }
  if ((SDL_GL_ACCUM_GREEN_SIZE) < 0 || (unsigned long)(SDL_GL_ACCUM_GREEN_SIZE) != 9UL) {
    char buf[64];
    if ((SDL_GL_ACCUM_GREEN_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_ACCUM_GREEN_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_ACCUM_GREEN_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_ACCUM_GREEN_SIZE", buf, "9");
    return -1;
  }
  if ((SDL_GL_ACCUM_BLUE_SIZE) < 0 || (unsigned long)(SDL_GL_ACCUM_BLUE_SIZE) != 10UL) {
    char buf[64];
    if ((SDL_GL_ACCUM_BLUE_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_ACCUM_BLUE_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_ACCUM_BLUE_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_ACCUM_BLUE_SIZE", buf, "10");
    return -1;
  }
  if ((SDL_GL_ACCUM_ALPHA_SIZE) < 0 || (unsigned long)(SDL_GL_ACCUM_ALPHA_SIZE) != 11UL) {
    char buf[64];
    if ((SDL_GL_ACCUM_ALPHA_SIZE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_ACCUM_ALPHA_SIZE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_ACCUM_ALPHA_SIZE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_ACCUM_ALPHA_SIZE", buf, "11");
    return -1;
  }
  if ((SDL_GL_STEREO) < 0 || (unsigned long)(SDL_GL_STEREO) != 12UL) {
    char buf[64];
    if ((SDL_GL_STEREO) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_STEREO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_STEREO));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_STEREO", buf, "12");
    return -1;
  }
  if ((SDL_GL_MULTISAMPLEBUFFERS) < 0 || (unsigned long)(SDL_GL_MULTISAMPLEBUFFERS) != 13UL) {
    char buf[64];
    if ((SDL_GL_MULTISAMPLEBUFFERS) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_MULTISAMPLEBUFFERS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_MULTISAMPLEBUFFERS));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_MULTISAMPLEBUFFERS", buf, "13");
    return -1;
  }
  if ((SDL_GL_MULTISAMPLESAMPLES) < 0 || (unsigned long)(SDL_GL_MULTISAMPLESAMPLES) != 14UL) {
    char buf[64];
    if ((SDL_GL_MULTISAMPLESAMPLES) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_MULTISAMPLESAMPLES));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_MULTISAMPLESAMPLES));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_MULTISAMPLESAMPLES", buf, "14");
    return -1;
  }
  if ((SDL_GL_ACCELERATED_VISUAL) < 0 || (unsigned long)(SDL_GL_ACCELERATED_VISUAL) != 15UL) {
    char buf[64];
    if ((SDL_GL_ACCELERATED_VISUAL) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_ACCELERATED_VISUAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_ACCELERATED_VISUAL));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_ACCELERATED_VISUAL", buf, "15");
    return -1;
  }
  if ((SDL_GL_SWAP_CONTROL) < 0 || (unsigned long)(SDL_GL_SWAP_CONTROL) != 16UL) {
    char buf[64];
    if ((SDL_GL_SWAP_CONTROL) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GL_SWAP_CONTROL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GL_SWAP_CONTROL));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_GLattr", "SDL_GL_SWAP_CONTROL", buf, "16");
    return -1;
  }
  return _cffi_e__SDL_EventType(lib);
}

static int _cffi_const_SDL_GRAB_QUERY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_GRAB_QUERY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_GRAB_QUERY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_RESERVED(lib);
}

static int _cffi_const_SDL_GRAB_OFF(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_GRAB_OFF);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_GRAB_OFF", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_GRAB_QUERY(lib);
}

static int _cffi_const_SDL_GRAB_ON(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_GRAB_ON);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_GRAB_ON", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_GRAB_OFF(lib);
}

static void _cffi_check__SDL_Palette(SDL_Palette *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->ncolors) << 1);
  { SDL_Color * *tmp = &p->colors; (void)tmp; }
}
static PyObject *
_cffi_layout__SDL_Palette(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; SDL_Palette y; };
  static Py_ssize_t nums[] = {
    sizeof(SDL_Palette),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(SDL_Palette, ncolors),
    sizeof(((SDL_Palette *)0)->ncolors),
    offsetof(SDL_Palette, colors),
    sizeof(((SDL_Palette *)0)->colors),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check__SDL_Palette(0);
}

static int _cffi_e__SDL_bool(PyObject *lib)
{
  if ((SDL_FALSE) < 0 || (unsigned long)(SDL_FALSE) != 0UL) {
    char buf[64];
    if ((SDL_FALSE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_FALSE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_FALSE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_bool", "SDL_FALSE", buf, "0");
    return -1;
  }
  if ((SDL_TRUE) < 0 || (unsigned long)(SDL_TRUE) != 1UL) {
    char buf[64];
    if ((SDL_TRUE) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_TRUE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_TRUE));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_bool", "SDL_TRUE", buf, "1");
    return -1;
  }
  return _cffi_e__SDL_GLattr(lib);
}

static int _cffi_e__SDL_eventaction(PyObject *lib)
{
  if ((SDL_ADDEVENT) < 0 || (unsigned long)(SDL_ADDEVENT) != 0UL) {
    char buf[64];
    if ((SDL_ADDEVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_ADDEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_ADDEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_eventaction", "SDL_ADDEVENT", buf, "0");
    return -1;
  }
  if ((SDL_PEEKEVENT) < 0 || (unsigned long)(SDL_PEEKEVENT) != 1UL) {
    char buf[64];
    if ((SDL_PEEKEVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_PEEKEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_PEEKEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_eventaction", "SDL_PEEKEVENT", buf, "1");
    return -1;
  }
  if ((SDL_GETEVENT) < 0 || (unsigned long)(SDL_GETEVENT) != 2UL) {
    char buf[64];
    if ((SDL_GETEVENT) < 0)
        snprintf(buf, 63, "%ld", (long)(SDL_GETEVENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(SDL_GETEVENT));
    PyErr_Format(_cffi_VerificationError,
                 "enum %s: %s has the real value %s, not %s",
                 "SDL_eventaction", "SDL_GETEVENT", buf, "2");
    return -1;
  }
  return _cffi_e__SDL_bool(lib);
}

static PyObject *
_cffi_f_IMG_GetError(PyObject *self, PyObject *no_arg)
{
  char * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = IMG_GetError(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(0));
}

static PyObject *
_cffi_f_IMG_Load(PyObject *self, PyObject *arg0)
{
  char const * x0;
  Py_ssize_t datasize;
  SDL_Surface * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = IMG_Load(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_IMG_LoadTyped_RW(PyObject *self, PyObject *args)
{
  SDL_RWops * x0;
  int x1;
  char * x2;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:IMG_LoadTyped_RW", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(0), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = IMG_LoadTyped_RW(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_Mix_AllocateChannels(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_AllocateChannels(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_ChannelFinished(PyObject *self, PyObject *arg0)
{
  void(* x0)(int);

  x0 = (void(*)(int))_cffi_to_c_pointer(arg0, _cffi_type(4));
  if (x0 == (void(*)(int))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_ChannelFinished(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_CloseAudio(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_CloseAudio(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_FadeInChannelTimed(PyObject *self, PyObject *args)
{
  int x0;
  Mix_Chunk * x1;
  int x2;
  int x3;
  int x4;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:Mix_FadeInChannelTimed", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(6), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, int);
  if (x4 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_FadeInChannelTimed(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_FadeInMusicPos(PyObject *self, PyObject *args)
{
  Mix_Music * x0;
  int x1;
  int x2;
  double x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:Mix_FadeInMusicPos", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(7), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_double(arg3);
  if (x3 == (double)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_FadeInMusicPos(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_FadeOutChannel(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_FadeOutChannel", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_FadeOutChannel(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_FadeOutGroup(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_FadeOutGroup", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_FadeOutGroup(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_FadeOutMusic(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_FadeOutMusic(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_FreeChunk(PyObject *self, PyObject *arg0)
{
  Mix_Chunk * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(6), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_FreeChunk(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_FreeMusic(PyObject *self, PyObject *arg0)
{
  Mix_Music * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(7), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_FreeMusic(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_GroupAvailable(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_GroupAvailable(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_GroupChannel(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_GroupChannel", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_GroupChannel(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_GroupCount(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_GroupCount(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_GroupOldest(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_GroupOldest(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_HaltChannel(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_HaltChannel(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_HaltMusic(PyObject *self, PyObject *no_arg)
{
  int result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_HaltMusic(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_HookMusicFinished(PyObject *self, PyObject *arg0)
{
  void(* x0)(void);

  x0 = (void(*)(void))_cffi_to_c_pointer(arg0, _cffi_type(8));
  if (x0 == (void(*)(void))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_HookMusicFinished(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_LoadMUS(PyObject *self, PyObject *arg0)
{
  char const * x0;
  Py_ssize_t datasize;
  Mix_Music * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_LoadMUS(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(7));
}

static PyObject *
_cffi_f_Mix_LoadMUS_RW(PyObject *self, PyObject *arg0)
{
  SDL_RWops * x0;
  Py_ssize_t datasize;
  Mix_Music * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_LoadMUS_RW(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(7));
}

static PyObject *
_cffi_f_Mix_LoadWAV_RW(PyObject *self, PyObject *args)
{
  SDL_RWops * x0;
  int x1;
  Py_ssize_t datasize;
  Mix_Chunk * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_LoadWAV_RW", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_LoadWAV_RW(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(6));
}

static PyObject *
_cffi_f_Mix_OpenAudio(PyObject *self, PyObject *args)
{
  int x0;
  uint16_t x1;
  int x2;
  int x3;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:Mix_OpenAudio", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, uint16_t);
  if (x1 == (uint16_t)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_OpenAudio(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_Pause(PyObject *self, PyObject *arg0)
{
  int x0;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_Pause(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_PauseMusic(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_PauseMusic(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_PausedMusic(PyObject *self, PyObject *no_arg)
{
  int result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_PausedMusic(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_PlayChannelTimed(PyObject *self, PyObject *args)
{
  int x0;
  Mix_Chunk * x1;
  int x2;
  int x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:Mix_PlayChannelTimed", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(6), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_PlayChannelTimed(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_PlayMusic(PyObject *self, PyObject *args)
{
  Mix_Music * x0;
  int x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_PlayMusic", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(7), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_PlayMusic(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_Playing(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_Playing(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_PlayingMusic(PyObject *self, PyObject *no_arg)
{
  int result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_PlayingMusic(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_QuerySpec(PyObject *self, PyObject *args)
{
  int * x0;
  uint16_t * x1;
  int * x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:Mix_QuerySpec", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(10), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(9), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_QuerySpec(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_ReserveChannels(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_ReserveChannels(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_Resume(PyObject *self, PyObject *arg0)
{
  int x0;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_Resume(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_ResumeMusic(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_ResumeMusic(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_RewindMusic(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_RewindMusic(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_SetMusicPosition(PyObject *self, PyObject *arg0)
{
  double x0;
  int result;

  x0 = _cffi_to_c_double(arg0);
  if (x0 == (double)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_SetMusicPosition(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_SetPanning(PyObject *self, PyObject *args)
{
  int x0;
  uint8_t x1;
  uint8_t x2;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:Mix_SetPanning", &arg0, &arg1, &arg2))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, uint8_t);
  if (x1 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, uint8_t);
  if (x2 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_SetPanning(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_SetPostMix(PyObject *self, PyObject *args)
{
  void(* x0)(void *, uint8_t *, int);
  void * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_SetPostMix", &arg0, &arg1))
    return NULL;

  x0 = (void(*)(void *, uint8_t *, int))_cffi_to_c_pointer(arg0, _cffi_type(11));
  if (x0 == (void(*)(void *, uint8_t *, int))NULL && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(12), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { Mix_SetPostMix(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_Mix_Volume(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_Volume", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_Volume(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_VolumeChunk(PyObject *self, PyObject *args)
{
  Mix_Chunk * x0;
  int x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:Mix_VolumeChunk", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(6), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_VolumeChunk(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_Mix_VolumeMusic(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = Mix_VolumeMusic(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_AddTimer(PyObject *self, PyObject *args)
{
  uint32_t x0;
  uint32_t(* x1)(uint32_t, void *);
  void * x2;
  Py_ssize_t datasize;
  struct _SDL_TimerID * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_AddTimer", &arg0, &arg1, &arg2))
    return NULL;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x1 = (uint32_t(*)(uint32_t, void *))_cffi_to_c_pointer(arg1, _cffi_type(13));
  if (x1 == (uint32_t(*)(uint32_t, void *))NULL && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(12), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_AddTimer(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(14));
}

static PyObject *
_cffi_f_SDL_AllocRW(PyObject *self, PyObject *no_arg)
{
  SDL_RWops * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_AllocRW(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(3));
}

static PyObject *
_cffi_f_SDL_BlitSurface(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Rect * x1;
  SDL_Surface * x2;
  SDL_Rect * x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:SDL_BlitSurface", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(15), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(2), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca(datasize);
    memset((void *)x3, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(15), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_BlitSurface(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_ClearError(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_ClearError(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_ConvertSurface(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_PixelFormat * x1;
  uint32_t x2;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_ConvertSurface", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(16), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, uint32_t);
  if (x2 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_ConvertSurface(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_CreateCursor(PyObject *self, PyObject *args)
{
  uint8_t * x0;
  uint8_t * x1;
  int x2;
  int x3;
  int x4;
  int x5;
  Py_ssize_t datasize;
  SDL_Cursor * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;

  if (!PyArg_ParseTuple(args, "OOOOOO:SDL_CreateCursor", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(17), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(17), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, int);
  if (x4 == (int)-1 && PyErr_Occurred())
    return NULL;

  x5 = _cffi_to_c_int(arg5, int);
  if (x5 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_CreateCursor(x0, x1, x2, x3, x4, x5); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(18));
}

static PyObject *
_cffi_f_SDL_CreateRGBSurface(PyObject *self, PyObject *args)
{
  uint32_t x0;
  int x1;
  int x2;
  int x3;
  uint32_t x4;
  uint32_t x5;
  uint32_t x6;
  uint32_t x7;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;
  PyObject *arg6;
  PyObject *arg7;

  if (!PyArg_ParseTuple(args, "OOOOOOOO:SDL_CreateRGBSurface", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7))
    return NULL;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, uint32_t);
  if (x4 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x5 = _cffi_to_c_int(arg5, uint32_t);
  if (x5 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x6 = _cffi_to_c_int(arg6, uint32_t);
  if (x6 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x7 = _cffi_to_c_int(arg7, uint32_t);
  if (x7 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_CreateRGBSurface(x0, x1, x2, x3, x4, x5, x6, x7); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_CreateRGBSurfaceFrom(PyObject *self, PyObject *args)
{
  void * x0;
  int x1;
  int x2;
  int x3;
  int x4;
  uint32_t x5;
  uint32_t x6;
  uint32_t x7;
  uint32_t x8;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;
  PyObject *arg6;
  PyObject *arg7;
  PyObject *arg8;

  if (!PyArg_ParseTuple(args, "OOOOOOOOO:SDL_CreateRGBSurfaceFrom", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(12), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, int);
  if (x4 == (int)-1 && PyErr_Occurred())
    return NULL;

  x5 = _cffi_to_c_int(arg5, uint32_t);
  if (x5 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x6 = _cffi_to_c_int(arg6, uint32_t);
  if (x6 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x7 = _cffi_to_c_int(arg7, uint32_t);
  if (x7 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x8 = _cffi_to_c_int(arg8, uint32_t);
  if (x8 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_CreateRGBSurfaceFrom(x0, x1, x2, x3, x4, x5, x6, x7, x8); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_Delay(PyObject *self, PyObject *arg0)
{
  uint32_t x0;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_Delay(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_DisplayFormat(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;
  SDL_Surface * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_DisplayFormat(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_DisplayFormatAlpha(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;
  SDL_Surface * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_DisplayFormatAlpha(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_EnableKeyRepeat(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_EnableKeyRepeat", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_EnableKeyRepeat(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_EnableUNICODE(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_EnableUNICODE(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_EventState(PyObject *self, PyObject *args)
{
  uint8_t x0;
  int x1;
  uint8_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_EventState", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, uint8_t);
  if (x0 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_EventState(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint8_t);
}

static PyObject *
_cffi_f_SDL_FillRect(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Rect * x1;
  uint32_t x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_FillRect", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(15), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, uint32_t);
  if (x2 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_FillRect(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_Flip(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_Flip(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_FreeCursor(PyObject *self, PyObject *arg0)
{
  SDL_Cursor * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(18), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(18), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_FreeCursor(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_FreeSurface(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_FreeSurface(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_GL_GetAttribute(PyObject *self, PyObject *args)
{
  SDL_GLattr x0;
  int * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_GL_GetAttribute", &arg0, &arg1))
    return NULL;

  if (_cffi_to_c((char *)&x0, _cffi_type(19), arg0) < 0)
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(9), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GL_GetAttribute(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_GL_SetAttribute(PyObject *self, PyObject *args)
{
  SDL_GLattr x0;
  int x1;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_GL_SetAttribute", &arg0, &arg1))
    return NULL;

  if (_cffi_to_c((char *)&x0, _cffi_type(19), arg0) < 0)
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GL_SetAttribute(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_GL_SwapBuffers(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_GL_SwapBuffers(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_GetAppState(PyObject *self, PyObject *no_arg)
{
  uint8_t result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetAppState(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint8_t);
}

static PyObject *
_cffi_f_SDL_GetClipRect(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Rect * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_GetClipRect", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(15), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_GetClipRect(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_GetCursor(PyObject *self, PyObject *no_arg)
{
  SDL_Cursor * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetCursor(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(18));
}

static PyObject *
_cffi_f_SDL_GetError(PyObject *self, PyObject *no_arg)
{
  char * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetError(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(0));
}

static PyObject *
_cffi_f_SDL_GetKeyName(PyObject *self, PyObject *arg0)
{
  SDLKey x0;
  char * result;

  if (_cffi_to_c((char *)&x0, _cffi_type(20), arg0) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetKeyName(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(0));
}

static PyObject *
_cffi_f_SDL_GetKeyRepeat(PyObject *self, PyObject *args)
{
  int * x0;
  int * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_GetKeyRepeat", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(9), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_GetKeyRepeat(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_GetKeyState(PyObject *self, PyObject *arg0)
{
  int * x0;
  Py_ssize_t datasize;
  uint8_t * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetKeyState(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(17));
}

static PyObject *
_cffi_f_SDL_GetModState(PyObject *self, PyObject *no_arg)
{
  SDLMod result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetModState(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_deref((char *)&result, _cffi_type(21));
}

static PyObject *
_cffi_f_SDL_GetMouseState(PyObject *self, PyObject *args)
{
  int * x0;
  int * x1;
  Py_ssize_t datasize;
  uint8_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_GetMouseState", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(9), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetMouseState(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint8_t);
}

static PyObject *
_cffi_f_SDL_GetRGBA(PyObject *self, PyObject *args)
{
  uint32_t x0;
  SDL_PixelFormat * x1;
  uint8_t * x2;
  uint8_t * x3;
  uint8_t * x4;
  uint8_t * x5;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;

  if (!PyArg_ParseTuple(args, "OOOOOO:SDL_GetRGBA", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5))
    return NULL;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(16), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(17), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca(datasize);
    memset((void *)x3, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(17), arg3) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca(datasize);
    memset((void *)x4, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(17), arg4) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg5, (char **)&x5);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x5 = alloca(datasize);
    memset((void *)x5, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x5, _cffi_type(17), arg5) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_GetRGBA(x0, x1, x2, x3, x4, x5); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_GetRelativeMouseState(PyObject *self, PyObject *args)
{
  int * x0;
  int * x1;
  Py_ssize_t datasize;
  uint8_t result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_GetRelativeMouseState", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(9), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetRelativeMouseState(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint8_t);
}

static PyObject *
_cffi_f_SDL_GetTicks(PyObject *self, PyObject *no_arg)
{
  uint32_t result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetTicks(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint32_t);
}

static PyObject *
_cffi_f_SDL_GetVideoInfo(PyObject *self, PyObject *no_arg)
{
  SDL_VideoInfo const * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetVideoInfo(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(22));
}

static PyObject *
_cffi_f_SDL_GetVideoSurface(PyObject *self, PyObject *no_arg)
{
  SDL_Surface * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_GetVideoSurface(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_Init(PyObject *self, PyObject *arg0)
{
  uint32_t x0;
  int result;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_Init(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_InitSubSystem(PyObject *self, PyObject *arg0)
{
  uint32_t x0;
  int result;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_InitSubSystem(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_Linked_Version(PyObject *self, PyObject *no_arg)
{
  SDL_version const * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_Linked_Version(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(23));
}

static PyObject *
_cffi_f_SDL_ListModes(PyObject *self, PyObject *args)
{
  SDL_PixelFormat * x0;
  uint32_t x1;
  Py_ssize_t datasize;
  SDL_Rect * * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_ListModes", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(16), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint32_t);
  if (x1 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_ListModes(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(24));
}

static PyObject *
_cffi_f_SDL_LoadBMP(PyObject *self, PyObject *arg0)
{
  char const * x0;
  Py_ssize_t datasize;
  SDL_Surface * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_LoadBMP(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_LoadBMP_RW(PyObject *self, PyObject *args)
{
  SDL_RWops * x0;
  int x1;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_LoadBMP_RW", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_LoadBMP_RW(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_LockSurface(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_LockSurface(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_MapRGBA(PyObject *self, PyObject *args)
{
  SDL_PixelFormat * x0;
  uint8_t x1;
  uint8_t x2;
  uint8_t x3;
  uint8_t x4;
  Py_ssize_t datasize;
  uint32_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:SDL_MapRGBA", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(16), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint8_t);
  if (x1 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, uint8_t);
  if (x2 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, uint8_t);
  if (x3 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, uint8_t);
  if (x4 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_MapRGBA(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint32_t);
}

static PyObject *
_cffi_f_SDL_PeepEvents(PyObject *self, PyObject *args)
{
  SDL_Event * x0;
  int x1;
  SDL_eventaction x2;
  uint32_t x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:SDL_PeepEvents", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(25), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(25), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  if (_cffi_to_c((char *)&x2, _cffi_type(26), arg2) < 0)
    return NULL;

  x3 = _cffi_to_c_int(arg3, uint32_t);
  if (x3 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_PeepEvents(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_PollEvent(PyObject *self, PyObject *arg0)
{
  SDL_Event * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(25), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(25), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_PollEvent(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_PumpEvents(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_PumpEvents(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_PushEvent(PyObject *self, PyObject *arg0)
{
  SDL_Event * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(25), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(25), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_PushEvent(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_Quit(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_Quit(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_QuitSubSystem(PyObject *self, PyObject *arg0)
{
  uint32_t x0;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_QuitSubSystem(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_RWFromFP(PyObject *self, PyObject *args)
{
  FILE * x0;
  int x1;
  Py_ssize_t datasize;
  SDL_RWops * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_RWFromFP", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(27), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(27), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_RWFromFP(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(3));
}

static PyObject *
_cffi_f_SDL_RWFromFile(PyObject *self, PyObject *args)
{
  char const * x0;
  char const * x1;
  Py_ssize_t datasize;
  SDL_RWops * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_RWFromFile", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_RWFromFile(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(3));
}

static PyObject *
_cffi_f_SDL_RWclose(PyObject *self, PyObject *arg0)
{
  SDL_RWops * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_RWclose(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_RWwrite(PyObject *self, PyObject *args)
{
  SDL_RWops * x0;
  void const * x1;
  size_t x2;
  size_t x3;
  Py_ssize_t datasize;
  size_t result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:SDL_RWwrite", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(28), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(28), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, size_t);
  if (x2 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, size_t);
  if (x3 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_RWwrite(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, size_t);
}

static PyObject *
_cffi_f_SDL_RemoveTimer(PyObject *self, PyObject *arg0)
{
  struct _SDL_TimerID * x0;
  Py_ssize_t datasize;
  SDL_bool result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(14), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_RemoveTimer(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_deref((char *)&result, _cffi_type(29));
}

static PyObject *
_cffi_f_SDL_SaveBMP(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  char const * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_SaveBMP", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SaveBMP(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetAlpha(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  uint32_t x1;
  uint8_t x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_SetAlpha", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint32_t);
  if (x1 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, uint8_t);
  if (x2 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetAlpha(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetClipRect(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Rect * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_SetClipRect", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(15), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_SetClipRect(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_SetColorKey(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  uint32_t x1;
  uint32_t x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_SetColorKey", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint32_t);
  if (x1 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, uint32_t);
  if (x2 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetColorKey(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetColors(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Color * x1;
  int x2;
  int x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:SDL_SetColors", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(30), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(30), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetColors(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetCursor(PyObject *self, PyObject *arg0)
{
  SDL_Cursor * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(18), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(18), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_SetCursor(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static int _cffi_const_SDL_SetError(PyObject *lib)
{
  PyObject *o;
  int res;
  void(* i)(char const *, ...);
  i = (SDL_SetError);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(31));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_SetError", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_e__SDL_eventaction(lib);
}

static PyObject *
_cffi_f_SDL_SetGamma(PyObject *self, PyObject *args)
{
  float x0;
  float x1;
  float x2;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_SetGamma", &arg0, &arg1, &arg2))
    return NULL;

  x0 = _cffi_to_c_float(arg0);
  if (x0 == (float)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_float(arg1);
  if (x1 == (float)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_float(arg2);
  if (x2 == (float)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetGamma(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetGammaRamp(PyObject *self, PyObject *args)
{
  uint16_t * x0;
  uint16_t * x1;
  uint16_t * x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_SetGammaRamp", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(10), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(10), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(10), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetGammaRamp(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetModState(PyObject *self, PyObject *arg0)
{
  SDLMod x0;

  if (_cffi_to_c((char *)&x0, _cffi_type(21), arg0) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_SetModState(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_SetPalette(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  int x1;
  SDL_Color * x2;
  int x3;
  int x4;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:SDL_SetPalette", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(30), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(30), arg2) < 0)
      return NULL;
  }

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, int);
  if (x4 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetPalette(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_SetVideoMode(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int x2;
  uint32_t x3;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:SDL_SetVideoMode", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, uint32_t);
  if (x3 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_SetVideoMode(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_SDL_ShowCursor(PyObject *self, PyObject *arg0)
{
  int x0;
  int result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_ShowCursor(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_UnlockSurface(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_UnlockSurface(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_UpdateRect(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  int x1;
  int x2;
  int x3;
  int x4;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:SDL_UpdateRect", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_int(arg4, int);
  if (x4 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_UpdateRect(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_UpdateRects(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  int x1;
  SDL_Rect * x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:SDL_UpdateRects", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(15), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_UpdateRects(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_VideoDriverName(PyObject *self, PyObject *args)
{
  char * x0;
  int x1;
  Py_ssize_t datasize;
  char * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_VideoDriverName", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_VideoDriverName(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(0));
}

static PyObject *
_cffi_f_SDL_VideoInit(PyObject *self, PyObject *args)
{
  char const * x0;
  uint32_t x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_VideoInit", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint32_t);
  if (x1 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_VideoInit(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_VideoModeOK(PyObject *self, PyObject *args)
{
  int x0;
  int x1;
  int x2;
  uint32_t x3;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:SDL_VideoModeOK", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, uint32_t);
  if (x3 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_VideoModeOK(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_WM_GetCaption(PyObject *self, PyObject *args)
{
  char * * x0;
  char * * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_WM_GetCaption", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(32), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(32), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(32), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(32), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_WM_GetCaption(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_WM_GrabInput(PyObject *self, PyObject *arg0)
{
  SDL_GrabMode x0;
  SDL_GrabMode result;

  if (_cffi_to_c((char *)&x0, _cffi_type(33), arg0) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_WM_GrabInput(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_deref((char *)&result, _cffi_type(33));
}

static PyObject *
_cffi_f_SDL_WM_IconifyWindow(PyObject *self, PyObject *no_arg)
{
  int result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_WM_IconifyWindow(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_WM_SetCaption(PyObject *self, PyObject *args)
{
  char const * x0;
  char const * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_WM_SetCaption", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_WM_SetCaption(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_WM_SetIcon(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  uint8_t * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_WM_SetIcon", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(17), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_WM_SetIcon(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_WM_ToggleFullScreen(PyObject *self, PyObject *arg0)
{
  SDL_Surface * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_WM_ToggleFullScreen(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_WaitEvent(PyObject *self, PyObject *arg0)
{
  SDL_Event * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(25), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(25), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_WaitEvent(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_SDL_WarpMouse(PyObject *self, PyObject *args)
{
  uint16_t x0;
  uint16_t x1;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:SDL_WarpMouse", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, uint16_t);
  if (x0 == (uint16_t)-1 && PyErr_Occurred())
    return NULL;

  x1 = _cffi_to_c_int(arg1, uint16_t);
  if (x1 == (uint16_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { SDL_WarpMouse(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_SDL_WasInit(PyObject *self, PyObject *arg0)
{
  uint32_t x0;
  uint32_t result;

  x0 = _cffi_to_c_int(arg0, uint32_t);
  if (x0 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = SDL_WasInit(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint32_t);
}

static PyObject *
_cffi_f_TTF_CloseFont(PyObject *self, PyObject *arg0)
{
  TTF_Font * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { TTF_CloseFont(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_TTF_FontAscent(PyObject *self, PyObject *arg0)
{
  TTF_Font * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_FontAscent(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_FontDescent(PyObject *self, PyObject *arg0)
{
  TTF_Font * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_FontDescent(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_FontHeight(PyObject *self, PyObject *arg0)
{
  TTF_Font * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_FontHeight(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_FontLineSkip(PyObject *self, PyObject *arg0)
{
  TTF_Font * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_FontLineSkip(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_GetError(PyObject *self, PyObject *no_arg)
{
  char * result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_GetError(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(0));
}

static PyObject *
_cffi_f_TTF_GetFontStyle(PyObject *self, PyObject *arg0)
{
  TTF_Font const * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(35), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(35), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_GetFontStyle(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_GlyphMetrics(PyObject *self, PyObject *args)
{
  TTF_Font * x0;
  uint16_t x1;
  int * x2;
  int * x3;
  int * x4;
  int * x5;
  int * x6;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;
  PyObject *arg6;

  if (!PyArg_ParseTuple(args, "OOOOOOO:TTF_GlyphMetrics", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, uint16_t);
  if (x1 == (uint16_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(9), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca(datasize);
    memset((void *)x3, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(9), arg3) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca(datasize);
    memset((void *)x4, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(9), arg4) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg5, (char **)&x5);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x5 = alloca(datasize);
    memset((void *)x5, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x5, _cffi_type(9), arg5) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg6, (char **)&x6);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x6 = alloca(datasize);
    memset((void *)x6, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x6, _cffi_type(9), arg6) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_GlyphMetrics(x0, x1, x2, x3, x4, x5, x6); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_Init(PyObject *self, PyObject *no_arg)
{
  int result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_Init(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_OpenFont(PyObject *self, PyObject *args)
{
  char const * x0;
  int x1;
  Py_ssize_t datasize;
  TTF_Font * result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:TTF_OpenFont", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_OpenFont(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(34));
}

static PyObject *
_cffi_f_TTF_OpenFontIndexRW(PyObject *self, PyObject *args)
{
  SDL_RWops * x0;
  int x1;
  int x2;
  long x3;
  Py_ssize_t datasize;
  TTF_Font * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:TTF_OpenFontIndexRW", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, long);
  if (x3 == (long)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_OpenFontIndexRW(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(34));
}

static PyObject *
_cffi_f_TTF_OpenFontRW(PyObject *self, PyObject *args)
{
  SDL_RWops * x0;
  int x1;
  int x2;
  Py_ssize_t datasize;
  TTF_Font * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:TTF_OpenFontRW", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(3), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_OpenFontRW(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(34));
}

static PyObject *
_cffi_f_TTF_Quit(PyObject *self, PyObject *no_arg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { TTF_Quit(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_TTF_RenderUTF8_Blended(PyObject *self, PyObject *args)
{
  TTF_Font * x0;
  char const * x1;
  SDL_Color x2;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:TTF_RenderUTF8_Blended", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x2, _cffi_type(36), arg2) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_RenderUTF8_Blended(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_TTF_RenderUTF8_Shaded(PyObject *self, PyObject *args)
{
  TTF_Font * x0;
  char const * x1;
  SDL_Color x2;
  SDL_Color x3;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:TTF_RenderUTF8_Shaded", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x2, _cffi_type(36), arg2) < 0)
    return NULL;

  if (_cffi_to_c((char *)&x3, _cffi_type(36), arg3) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_RenderUTF8_Shaded(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_TTF_RenderUTF8_Solid(PyObject *self, PyObject *args)
{
  TTF_Font * x0;
  char const * x1;
  SDL_Color x2;
  Py_ssize_t datasize;
  SDL_Surface * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:TTF_RenderUTF8_Solid", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  if (_cffi_to_c((char *)&x2, _cffi_type(36), arg2) < 0)
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_RenderUTF8_Solid(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_pointer((char *)result, _cffi_type(2));
}

static PyObject *
_cffi_f_TTF_SetFontStyle(PyObject *self, PyObject *args)
{
  TTF_Font * x0;
  int x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:TTF_SetFontStyle", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { TTF_SetFontStyle(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_TTF_SizeUTF8(PyObject *self, PyObject *args)
{
  TTF_Font * x0;
  char const * x1;
  int * x2;
  int * x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:TTF_SizeUTF8", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(34), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(34), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(9), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca(datasize);
    memset((void *)x3, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(9), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_SizeUTF8(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_TTF_WasInit(PyObject *self, PyObject *no_arg)
{
  int result;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = TTF_WasInit(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f__pygame_SDL_BUTTON(PyObject *self, PyObject *arg0)
{
  uint8_t x0;
  uint8_t result;

  x0 = _cffi_to_c_int(arg0, uint8_t);
  if (x0 == (uint8_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = _pygame_SDL_BUTTON(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, uint8_t);
}

static PyObject *
_cffi_f__pygame_SaveTGA_RW(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_RWops * x1;
  int x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:_pygame_SaveTGA_RW", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = _pygame_SaveTGA_RW(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_free(PyObject *self, PyObject *arg0)
{
  void * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(12), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { free(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_pygame_Blit(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Rect * x1;
  SDL_Surface * x2;
  SDL_Rect * x3;
  int x4;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:pygame_Blit", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(15), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca(datasize);
    memset((void *)x2, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(2), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca(datasize);
    memset((void *)x3, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(15), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, int);
  if (x4 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = pygame_Blit(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_rotate(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Surface * x1;
  uint32_t x2;
  double x3;
  double x4;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:rotate", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(2), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, uint32_t);
  if (x2 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_double(arg3);
  if (x3 == (double)-1 && PyErr_Occurred())
    return NULL;

  x4 = _cffi_to_c_double(arg4);
  if (x4 == (double)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { rotate(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_rotate90(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Surface * x1;
  int x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:rotate90", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(2), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { rotate90(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_scale2x(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Surface * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:scale2x", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(2), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { scale2x(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_surface_fill_blend(PyObject *self, PyObject *args)
{
  SDL_Surface * x0;
  SDL_Rect * x1;
  uint32_t x2;
  int x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:surface_fill_blend", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca(datasize);
    memset((void *)x0, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(2), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca(datasize);
    memset((void *)x1, 0, datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(15), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, uint32_t);
  if (x2 == (uint32_t)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = surface_fill_blend(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  return _cffi_from_c_int(result, int);
}

static int _cffi_const_AUDIO_S16SYS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AUDIO_S16SYS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AUDIO_S16SYS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_SetError(lib);
}

static int _cffi_const_AUDIO_S8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AUDIO_S8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AUDIO_S8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AUDIO_S16SYS(lib);
}

static int _cffi_const_AUDIO_U16SYS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AUDIO_U16SYS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AUDIO_U16SYS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AUDIO_S8(lib);
}

static int _cffi_const_AUDIO_U8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AUDIO_U8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AUDIO_U8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AUDIO_U16SYS(lib);
}

static int _cffi_const_MIX_CHANNELS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MIX_CHANNELS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MIX_CHANNELS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AUDIO_U8(lib);
}

static int _cffi_const_SDL_ALLEVENTS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_ALLEVENTS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_ALLEVENTS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MIX_CHANNELS(lib);
}

static int _cffi_const_SDL_ANYFORMAT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_ANYFORMAT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_ANYFORMAT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_ALLEVENTS(lib);
}

static int _cffi_const_SDL_APPACTIVE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_APPACTIVE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_APPACTIVE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_ANYFORMAT(lib);
}

static int _cffi_const_SDL_APPINPUTFOCUS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_APPINPUTFOCUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_APPINPUTFOCUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_APPACTIVE(lib);
}

static int _cffi_const_SDL_APPMOUSEFOCUS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_APPMOUSEFOCUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_APPMOUSEFOCUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_APPINPUTFOCUS(lib);
}

static int _cffi_const_SDL_ASYNCBLIT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_ASYNCBLIT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_ASYNCBLIT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_APPMOUSEFOCUS(lib);
}

static int _cffi_const_SDL_BYTEORDER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_BYTEORDER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_BYTEORDER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_ASYNCBLIT(lib);
}

static int _cffi_const_SDL_DISABLE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_DISABLE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_DISABLE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_BYTEORDER(lib);
}

static int _cffi_const_SDL_DOUBLEBUF(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_DOUBLEBUF);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_DOUBLEBUF", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_DISABLE(lib);
}

static int _cffi_const_SDL_ENABLE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_ENABLE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_ENABLE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_DOUBLEBUF(lib);
}

static int _cffi_const_SDL_FULLSCREEN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_FULLSCREEN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_FULLSCREEN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_ENABLE(lib);
}

static int _cffi_const_SDL_HAT_DOWN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HAT_DOWN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HAT_DOWN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_FULLSCREEN(lib);
}

static int _cffi_const_SDL_HAT_LEFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HAT_LEFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HAT_LEFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HAT_DOWN(lib);
}

static int _cffi_const_SDL_HAT_RIGHT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HAT_RIGHT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HAT_RIGHT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HAT_LEFT(lib);
}

static int _cffi_const_SDL_HAT_UP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HAT_UP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HAT_UP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HAT_RIGHT(lib);
}

static int _cffi_const_SDL_HWACCEL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HWACCEL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HWACCEL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HAT_UP(lib);
}

static int _cffi_const_SDL_HWPALETTE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HWPALETTE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HWPALETTE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HWACCEL(lib);
}

static int _cffi_const_SDL_HWSURFACE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_HWSURFACE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_HWSURFACE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HWPALETTE(lib);
}

static int _cffi_const_SDL_IGNORE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_IGNORE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_IGNORE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_HWSURFACE(lib);
}

static int _cffi_const_SDL_INIT_AUDIO(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_INIT_AUDIO);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_INIT_AUDIO", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_IGNORE(lib);
}

static int _cffi_const_SDL_INIT_EVENTTHREAD(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_INIT_EVENTTHREAD);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_INIT_EVENTTHREAD", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_INIT_AUDIO(lib);
}

static int _cffi_const_SDL_INIT_EVERYTHING(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_INIT_EVERYTHING);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_INIT_EVERYTHING", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_INIT_EVENTTHREAD(lib);
}

static int _cffi_const_SDL_INIT_NOPARACHUTE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_INIT_NOPARACHUTE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_INIT_NOPARACHUTE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_INIT_EVERYTHING(lib);
}

static int _cffi_const_SDL_INIT_TIMER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_INIT_TIMER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_INIT_TIMER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_INIT_NOPARACHUTE(lib);
}

static int _cffi_const_SDL_INIT_VIDEO(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_INIT_VIDEO);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_INIT_VIDEO", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_INIT_TIMER(lib);
}

static int _cffi_const_SDL_LIL_ENDIAN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_LIL_ENDIAN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_LIL_ENDIAN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_INIT_VIDEO(lib);
}

static int _cffi_const_SDL_LOGPAL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_LOGPAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_LOGPAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_LIL_ENDIAN(lib);
}

static int _cffi_const_SDL_NOFRAME(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_NOFRAME);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_NOFRAME", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_LOGPAL(lib);
}

static int _cffi_const_SDL_OPENGL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_OPENGL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_OPENGL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_NOFRAME(lib);
}

static int _cffi_const_SDL_OPENGLBLIT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_OPENGLBLIT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_OPENGLBLIT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_OPENGL(lib);
}

static int _cffi_const_SDL_PHYSPAL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_PHYSPAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_PHYSPAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_OPENGLBLIT(lib);
}

static int _cffi_const_SDL_PREALLOC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_PREALLOC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_PREALLOC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_PHYSPAL(lib);
}

static int _cffi_const_SDL_QUERY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_QUERY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_QUERY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_PREALLOC(lib);
}

static int _cffi_const_SDL_RESIZABLE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_RESIZABLE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_RESIZABLE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_QUERY(lib);
}

static int _cffi_const_SDL_RLEACCEL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_RLEACCEL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_RLEACCEL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_RESIZABLE(lib);
}

static int _cffi_const_SDL_RLEACCELOK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_RLEACCELOK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_RLEACCELOK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_RLEACCEL(lib);
}

static int _cffi_const_SDL_SRCALPHA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_SRCALPHA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_SRCALPHA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_RLEACCELOK(lib);
}

static int _cffi_const_SDL_SRCCOLORKEY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_SRCCOLORKEY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_SRCCOLORKEY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_SRCALPHA(lib);
}

static int _cffi_const_SDL_SWSURFACE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDL_SWSURFACE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDL_SWSURFACE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_SRCCOLORKEY(lib);
}

static int _cffi_const_TTF_STYLE_BOLD(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(TTF_STYLE_BOLD);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "TTF_STYLE_BOLD", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDL_SWSURFACE(lib);
}

static int _cffi_const_TTF_STYLE_ITALIC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(TTF_STYLE_ITALIC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "TTF_STYLE_ITALIC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_TTF_STYLE_BOLD(lib);
}

static int _cffi_const_TTF_STYLE_NORMAL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(TTF_STYLE_NORMAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "TTF_STYLE_NORMAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_TTF_STYLE_ITALIC(lib);
}

static int _cffi_const_TTF_STYLE_STRIKETHROUGH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(TTF_STYLE_STRIKETHROUGH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "TTF_STYLE_STRIKETHROUGH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_TTF_STYLE_NORMAL(lib);
}

static int _cffi_const_TTF_STYLE_UNDERLINE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(TTF_STYLE_UNDERLINE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "TTF_STYLE_UNDERLINE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_TTF_STYLE_STRIKETHROUGH(lib);
}

static void _cffi_check_struct_Mix_Chunk(struct Mix_Chunk *p)
{
  /* only to generate compile-time warnings or errors */
  { uint8_t * *tmp = &p->abuf; (void)tmp; }
  (void)((p->alen) << 1);
}
static PyObject *
_cffi_layout_struct_Mix_Chunk(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct Mix_Chunk y; };
  static Py_ssize_t nums[] = {
    sizeof(struct Mix_Chunk),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct Mix_Chunk, abuf),
    sizeof(((struct Mix_Chunk *)0)->abuf),
    offsetof(struct Mix_Chunk, alen),
    sizeof(((struct Mix_Chunk *)0)->alen),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_Mix_Chunk(0);
}

static void _cffi_check_struct_SDL_ActiveEvent(struct SDL_ActiveEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->gain) << 1);
  (void)((p->state) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_ActiveEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_ActiveEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_ActiveEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_ActiveEvent, type),
    sizeof(((struct SDL_ActiveEvent *)0)->type),
    offsetof(struct SDL_ActiveEvent, gain),
    sizeof(((struct SDL_ActiveEvent *)0)->gain),
    offsetof(struct SDL_ActiveEvent, state),
    sizeof(((struct SDL_ActiveEvent *)0)->state),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_ActiveEvent(0);
}

static void _cffi_check_struct_SDL_Color(struct SDL_Color *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->r) << 1);
  (void)((p->b) << 1);
  (void)((p->g) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_Color(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_Color y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_Color),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_Color, r),
    sizeof(((struct SDL_Color *)0)->r),
    offsetof(struct SDL_Color, b),
    sizeof(((struct SDL_Color *)0)->b),
    offsetof(struct SDL_Color, g),
    sizeof(((struct SDL_Color *)0)->g),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_Color(0);
}

static void _cffi_check_struct_SDL_Cursor(struct SDL_Cursor *p)
{
  /* only to generate compile-time warnings or errors */
  { SDL_Rect *tmp = &p->area; (void)tmp; }
  (void)((p->hot_x) << 1);
  (void)((p->hot_y) << 1);
  { uint8_t * *tmp = &p->data; (void)tmp; }
  { uint8_t * *tmp = &p->mask; (void)tmp; }
  { uint8_t *(*tmp)[2] = &p->save; (void)tmp; }
  { WMcursor * *tmp = &p->wm_cursor; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_SDL_Cursor(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_Cursor y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_Cursor),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_Cursor, area),
    sizeof(((struct SDL_Cursor *)0)->area),
    offsetof(struct SDL_Cursor, hot_x),
    sizeof(((struct SDL_Cursor *)0)->hot_x),
    offsetof(struct SDL_Cursor, hot_y),
    sizeof(((struct SDL_Cursor *)0)->hot_y),
    offsetof(struct SDL_Cursor, data),
    sizeof(((struct SDL_Cursor *)0)->data),
    offsetof(struct SDL_Cursor, mask),
    sizeof(((struct SDL_Cursor *)0)->mask),
    offsetof(struct SDL_Cursor, save),
    sizeof(((struct SDL_Cursor *)0)->save),
    offsetof(struct SDL_Cursor, wm_cursor),
    sizeof(((struct SDL_Cursor *)0)->wm_cursor),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_Cursor(0);
}

static void _cffi_check_struct_SDL_ExposeEvent(struct SDL_ExposeEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_ExposeEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_ExposeEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_ExposeEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_ExposeEvent, type),
    sizeof(((struct SDL_ExposeEvent *)0)->type),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_ExposeEvent(0);
}

static void _cffi_check_struct_SDL_JoyAxisEvent(struct SDL_JoyAxisEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->axis) << 1);
  (void)((p->value) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_JoyAxisEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_JoyAxisEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_JoyAxisEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_JoyAxisEvent, type),
    sizeof(((struct SDL_JoyAxisEvent *)0)->type),
    offsetof(struct SDL_JoyAxisEvent, which),
    sizeof(((struct SDL_JoyAxisEvent *)0)->which),
    offsetof(struct SDL_JoyAxisEvent, axis),
    sizeof(((struct SDL_JoyAxisEvent *)0)->axis),
    offsetof(struct SDL_JoyAxisEvent, value),
    sizeof(((struct SDL_JoyAxisEvent *)0)->value),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_JoyAxisEvent(0);
}

static void _cffi_check_struct_SDL_JoyBallEvent(struct SDL_JoyBallEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->ball) << 1);
  (void)((p->xrel) << 1);
  (void)((p->yrel) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_JoyBallEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_JoyBallEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_JoyBallEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_JoyBallEvent, type),
    sizeof(((struct SDL_JoyBallEvent *)0)->type),
    offsetof(struct SDL_JoyBallEvent, which),
    sizeof(((struct SDL_JoyBallEvent *)0)->which),
    offsetof(struct SDL_JoyBallEvent, ball),
    sizeof(((struct SDL_JoyBallEvent *)0)->ball),
    offsetof(struct SDL_JoyBallEvent, xrel),
    sizeof(((struct SDL_JoyBallEvent *)0)->xrel),
    offsetof(struct SDL_JoyBallEvent, yrel),
    sizeof(((struct SDL_JoyBallEvent *)0)->yrel),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_JoyBallEvent(0);
}

static void _cffi_check_struct_SDL_JoyButtonEvent(struct SDL_JoyButtonEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->button) << 1);
  (void)((p->state) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_JoyButtonEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_JoyButtonEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_JoyButtonEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_JoyButtonEvent, type),
    sizeof(((struct SDL_JoyButtonEvent *)0)->type),
    offsetof(struct SDL_JoyButtonEvent, which),
    sizeof(((struct SDL_JoyButtonEvent *)0)->which),
    offsetof(struct SDL_JoyButtonEvent, button),
    sizeof(((struct SDL_JoyButtonEvent *)0)->button),
    offsetof(struct SDL_JoyButtonEvent, state),
    sizeof(((struct SDL_JoyButtonEvent *)0)->state),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_JoyButtonEvent(0);
}

static void _cffi_check_struct_SDL_JoyHatEvent(struct SDL_JoyHatEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->hat) << 1);
  (void)((p->value) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_JoyHatEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_JoyHatEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_JoyHatEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_JoyHatEvent, type),
    sizeof(((struct SDL_JoyHatEvent *)0)->type),
    offsetof(struct SDL_JoyHatEvent, which),
    sizeof(((struct SDL_JoyHatEvent *)0)->which),
    offsetof(struct SDL_JoyHatEvent, hat),
    sizeof(((struct SDL_JoyHatEvent *)0)->hat),
    offsetof(struct SDL_JoyHatEvent, value),
    sizeof(((struct SDL_JoyHatEvent *)0)->value),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_JoyHatEvent(0);
}

static void _cffi_check_struct_SDL_KeyboardEvent(struct SDL_KeyboardEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->state) << 1);
  { SDL_keysym *tmp = &p->keysym; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_SDL_KeyboardEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_KeyboardEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_KeyboardEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_KeyboardEvent, type),
    sizeof(((struct SDL_KeyboardEvent *)0)->type),
    offsetof(struct SDL_KeyboardEvent, which),
    sizeof(((struct SDL_KeyboardEvent *)0)->which),
    offsetof(struct SDL_KeyboardEvent, state),
    sizeof(((struct SDL_KeyboardEvent *)0)->state),
    offsetof(struct SDL_KeyboardEvent, keysym),
    sizeof(((struct SDL_KeyboardEvent *)0)->keysym),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_KeyboardEvent(0);
}

static void _cffi_check_struct_SDL_MouseButtonEvent(struct SDL_MouseButtonEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->button) << 1);
  (void)((p->state) << 1);
  (void)((p->x) << 1);
  (void)((p->y) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_MouseButtonEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_MouseButtonEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_MouseButtonEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_MouseButtonEvent, type),
    sizeof(((struct SDL_MouseButtonEvent *)0)->type),
    offsetof(struct SDL_MouseButtonEvent, which),
    sizeof(((struct SDL_MouseButtonEvent *)0)->which),
    offsetof(struct SDL_MouseButtonEvent, button),
    sizeof(((struct SDL_MouseButtonEvent *)0)->button),
    offsetof(struct SDL_MouseButtonEvent, state),
    sizeof(((struct SDL_MouseButtonEvent *)0)->state),
    offsetof(struct SDL_MouseButtonEvent, x),
    sizeof(((struct SDL_MouseButtonEvent *)0)->x),
    offsetof(struct SDL_MouseButtonEvent, y),
    sizeof(((struct SDL_MouseButtonEvent *)0)->y),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_MouseButtonEvent(0);
}

static void _cffi_check_struct_SDL_MouseMotionEvent(struct SDL_MouseMotionEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->which) << 1);
  (void)((p->state) << 1);
  (void)((p->x) << 1);
  (void)((p->y) << 1);
  (void)((p->xrel) << 1);
  (void)((p->yrel) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_MouseMotionEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_MouseMotionEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_MouseMotionEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_MouseMotionEvent, type),
    sizeof(((struct SDL_MouseMotionEvent *)0)->type),
    offsetof(struct SDL_MouseMotionEvent, which),
    sizeof(((struct SDL_MouseMotionEvent *)0)->which),
    offsetof(struct SDL_MouseMotionEvent, state),
    sizeof(((struct SDL_MouseMotionEvent *)0)->state),
    offsetof(struct SDL_MouseMotionEvent, x),
    sizeof(((struct SDL_MouseMotionEvent *)0)->x),
    offsetof(struct SDL_MouseMotionEvent, y),
    sizeof(((struct SDL_MouseMotionEvent *)0)->y),
    offsetof(struct SDL_MouseMotionEvent, xrel),
    sizeof(((struct SDL_MouseMotionEvent *)0)->xrel),
    offsetof(struct SDL_MouseMotionEvent, yrel),
    sizeof(((struct SDL_MouseMotionEvent *)0)->yrel),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_MouseMotionEvent(0);
}

static void _cffi_check_struct_SDL_PixelFormat(struct SDL_PixelFormat *p)
{
  /* only to generate compile-time warnings or errors */
  { SDL_Palette * *tmp = &p->palette; (void)tmp; }
  (void)((p->BitsPerPixel) << 1);
  (void)((p->BytesPerPixel) << 1);
  (void)((p->Rloss) << 1);
  (void)((p->Gloss) << 1);
  (void)((p->Bloss) << 1);
  (void)((p->Aloss) << 1);
  (void)((p->Rshift) << 1);
  (void)((p->Gshift) << 1);
  (void)((p->Bshift) << 1);
  (void)((p->Ashift) << 1);
  (void)((p->Rmask) << 1);
  (void)((p->Gmask) << 1);
  (void)((p->Bmask) << 1);
  (void)((p->Amask) << 1);
  (void)((p->colorkey) << 1);
  (void)((p->alpha) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_PixelFormat(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_PixelFormat y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_PixelFormat),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_PixelFormat, palette),
    sizeof(((struct SDL_PixelFormat *)0)->palette),
    offsetof(struct SDL_PixelFormat, BitsPerPixel),
    sizeof(((struct SDL_PixelFormat *)0)->BitsPerPixel),
    offsetof(struct SDL_PixelFormat, BytesPerPixel),
    sizeof(((struct SDL_PixelFormat *)0)->BytesPerPixel),
    offsetof(struct SDL_PixelFormat, Rloss),
    sizeof(((struct SDL_PixelFormat *)0)->Rloss),
    offsetof(struct SDL_PixelFormat, Gloss),
    sizeof(((struct SDL_PixelFormat *)0)->Gloss),
    offsetof(struct SDL_PixelFormat, Bloss),
    sizeof(((struct SDL_PixelFormat *)0)->Bloss),
    offsetof(struct SDL_PixelFormat, Aloss),
    sizeof(((struct SDL_PixelFormat *)0)->Aloss),
    offsetof(struct SDL_PixelFormat, Rshift),
    sizeof(((struct SDL_PixelFormat *)0)->Rshift),
    offsetof(struct SDL_PixelFormat, Gshift),
    sizeof(((struct SDL_PixelFormat *)0)->Gshift),
    offsetof(struct SDL_PixelFormat, Bshift),
    sizeof(((struct SDL_PixelFormat *)0)->Bshift),
    offsetof(struct SDL_PixelFormat, Ashift),
    sizeof(((struct SDL_PixelFormat *)0)->Ashift),
    offsetof(struct SDL_PixelFormat, Rmask),
    sizeof(((struct SDL_PixelFormat *)0)->Rmask),
    offsetof(struct SDL_PixelFormat, Gmask),
    sizeof(((struct SDL_PixelFormat *)0)->Gmask),
    offsetof(struct SDL_PixelFormat, Bmask),
    sizeof(((struct SDL_PixelFormat *)0)->Bmask),
    offsetof(struct SDL_PixelFormat, Amask),
    sizeof(((struct SDL_PixelFormat *)0)->Amask),
    offsetof(struct SDL_PixelFormat, colorkey),
    sizeof(((struct SDL_PixelFormat *)0)->colorkey),
    offsetof(struct SDL_PixelFormat, alpha),
    sizeof(((struct SDL_PixelFormat *)0)->alpha),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_PixelFormat(0);
}

static void _cffi_check_struct_SDL_QuitEvent(struct SDL_QuitEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_QuitEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_QuitEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_QuitEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_QuitEvent, type),
    sizeof(((struct SDL_QuitEvent *)0)->type),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_QuitEvent(0);
}

static void _cffi_check_struct_SDL_RWops(struct SDL_RWops *p)
{
  /* only to generate compile-time warnings or errors */
}
static PyObject *
_cffi_layout_struct_SDL_RWops(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_RWops y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_RWops),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_RWops(0);
}

static void _cffi_check_struct_SDL_Rect(struct SDL_Rect *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->x) << 1);
  (void)((p->y) << 1);
  (void)((p->w) << 1);
  (void)((p->h) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_Rect(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_Rect y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_Rect),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_Rect, x),
    sizeof(((struct SDL_Rect *)0)->x),
    offsetof(struct SDL_Rect, y),
    sizeof(((struct SDL_Rect *)0)->y),
    offsetof(struct SDL_Rect, w),
    sizeof(((struct SDL_Rect *)0)->w),
    offsetof(struct SDL_Rect, h),
    sizeof(((struct SDL_Rect *)0)->h),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_Rect(0);
}

static void _cffi_check_struct_SDL_ResizeEvent(struct SDL_ResizeEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->w) << 1);
  (void)((p->h) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_ResizeEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_ResizeEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_ResizeEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_ResizeEvent, type),
    sizeof(((struct SDL_ResizeEvent *)0)->type),
    offsetof(struct SDL_ResizeEvent, w),
    sizeof(((struct SDL_ResizeEvent *)0)->w),
    offsetof(struct SDL_ResizeEvent, h),
    sizeof(((struct SDL_ResizeEvent *)0)->h),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_ResizeEvent(0);
}

static void _cffi_check_struct_SDL_Surface(struct SDL_Surface *p)
{
  /* only to generate compile-time warnings or errors */
  { SDL_PixelFormat * *tmp = &p->format; (void)tmp; }
  (void)((p->w) << 1);
  (void)((p->h) << 1);
  (void)((p->offset) << 1);
  { void * *tmp = &p->pixels; (void)tmp; }
  (void)((p->pitch) << 1);
  (void)((p->flags) << 1);
  { SDL_Rect *tmp = &p->clip_rect; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_SDL_Surface(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_Surface y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_Surface),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_Surface, format),
    sizeof(((struct SDL_Surface *)0)->format),
    offsetof(struct SDL_Surface, w),
    sizeof(((struct SDL_Surface *)0)->w),
    offsetof(struct SDL_Surface, h),
    sizeof(((struct SDL_Surface *)0)->h),
    offsetof(struct SDL_Surface, offset),
    sizeof(((struct SDL_Surface *)0)->offset),
    offsetof(struct SDL_Surface, pixels),
    sizeof(((struct SDL_Surface *)0)->pixels),
    offsetof(struct SDL_Surface, pitch),
    sizeof(((struct SDL_Surface *)0)->pitch),
    offsetof(struct SDL_Surface, flags),
    sizeof(((struct SDL_Surface *)0)->flags),
    offsetof(struct SDL_Surface, clip_rect),
    sizeof(((struct SDL_Surface *)0)->clip_rect),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_Surface(0);
}

static void _cffi_check_struct_SDL_SysWMEvent(struct SDL_SysWMEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  { SDL_SysWMmsg * *tmp = &p->msg; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_SDL_SysWMEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_SysWMEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_SysWMEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_SysWMEvent, type),
    sizeof(((struct SDL_SysWMEvent *)0)->type),
    offsetof(struct SDL_SysWMEvent, msg),
    sizeof(((struct SDL_SysWMEvent *)0)->msg),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_SysWMEvent(0);
}

static void _cffi_check_struct_SDL_UserEvent(struct SDL_UserEvent *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  (void)((p->code) << 1);
  { void * *tmp = &p->data1; (void)tmp; }
  { void * *tmp = &p->data2; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_SDL_UserEvent(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_UserEvent y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_UserEvent),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_UserEvent, type),
    sizeof(((struct SDL_UserEvent *)0)->type),
    offsetof(struct SDL_UserEvent, code),
    sizeof(((struct SDL_UserEvent *)0)->code),
    offsetof(struct SDL_UserEvent, data1),
    sizeof(((struct SDL_UserEvent *)0)->data1),
    offsetof(struct SDL_UserEvent, data2),
    sizeof(((struct SDL_UserEvent *)0)->data2),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_UserEvent(0);
}

static void _cffi_check_struct_SDL_VideoInfo(struct SDL_VideoInfo *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->hw_available) << 1);
  (void)((p->wm_available) << 1);
  (void)((p->blit_hw) << 1);
  (void)((p->blit_hw_CC) << 1);
  (void)((p->blit_hw_A) << 1);
  (void)((p->blit_sw) << 1);
  (void)((p->blit_sw_CC) << 1);
  (void)((p->blit_sw_A) << 1);
  (void)((p->blit_fill) << 1);
  (void)((p->video_mem) << 1);
  { SDL_PixelFormat * *tmp = &p->vfmt; (void)tmp; }
  (void)((p->current_w) << 1);
  (void)((p->current_h) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_VideoInfo(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_VideoInfo y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_VideoInfo),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_VideoInfo, video_mem),
    sizeof(((struct SDL_VideoInfo *)0)->video_mem),
    offsetof(struct SDL_VideoInfo, vfmt),
    sizeof(((struct SDL_VideoInfo *)0)->vfmt),
    offsetof(struct SDL_VideoInfo, current_w),
    sizeof(((struct SDL_VideoInfo *)0)->current_w),
    offsetof(struct SDL_VideoInfo, current_h),
    sizeof(((struct SDL_VideoInfo *)0)->current_h),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_VideoInfo(0);
}

static void _cffi_check_struct_SDL_keysym(struct SDL_keysym *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->scancode) << 1);
  { SDLKey *tmp = &p->sym; (void)tmp; }
  { SDLMod *tmp = &p->mod; (void)tmp; }
  (void)((p->unicode) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_keysym(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_keysym y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_keysym),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_keysym, scancode),
    sizeof(((struct SDL_keysym *)0)->scancode),
    offsetof(struct SDL_keysym, sym),
    sizeof(((struct SDL_keysym *)0)->sym),
    offsetof(struct SDL_keysym, mod),
    sizeof(((struct SDL_keysym *)0)->mod),
    offsetof(struct SDL_keysym, unicode),
    sizeof(((struct SDL_keysym *)0)->unicode),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_keysym(0);
}

static void _cffi_check_struct_SDL_version(struct SDL_version *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->major) << 1);
  (void)((p->minor) << 1);
  (void)((p->patch) << 1);
}
static PyObject *
_cffi_layout_struct_SDL_version(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SDL_version y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SDL_version),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct SDL_version, major),
    sizeof(((struct SDL_version *)0)->major),
    offsetof(struct SDL_version, minor),
    sizeof(((struct SDL_version *)0)->minor),
    offsetof(struct SDL_version, patch),
    sizeof(((struct SDL_version *)0)->patch),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SDL_version(0);
}

static void _cffi_check_union_SDL_Event(union SDL_Event *p)
{
  /* only to generate compile-time warnings or errors */
  (void)((p->type) << 1);
  { SDL_ActiveEvent *tmp = &p->active; (void)tmp; }
  { SDL_KeyboardEvent *tmp = &p->key; (void)tmp; }
  { SDL_MouseMotionEvent *tmp = &p->motion; (void)tmp; }
  { SDL_MouseButtonEvent *tmp = &p->button; (void)tmp; }
  { SDL_JoyAxisEvent *tmp = &p->jaxis; (void)tmp; }
  { SDL_JoyBallEvent *tmp = &p->jball; (void)tmp; }
  { SDL_JoyHatEvent *tmp = &p->jhat; (void)tmp; }
  { SDL_JoyButtonEvent *tmp = &p->jbutton; (void)tmp; }
  { SDL_ResizeEvent *tmp = &p->resize; (void)tmp; }
  { SDL_ExposeEvent *tmp = &p->expose; (void)tmp; }
  { SDL_QuitEvent *tmp = &p->quit; (void)tmp; }
  { SDL_UserEvent *tmp = &p->user; (void)tmp; }
  { SDL_SysWMEvent *tmp = &p->syswm; (void)tmp; }
}
static PyObject *
_cffi_layout_union_SDL_Event(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; union SDL_Event y; };
  static Py_ssize_t nums[] = {
    sizeof(union SDL_Event),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(union SDL_Event, type),
    sizeof(((union SDL_Event *)0)->type),
    offsetof(union SDL_Event, active),
    sizeof(((union SDL_Event *)0)->active),
    offsetof(union SDL_Event, key),
    sizeof(((union SDL_Event *)0)->key),
    offsetof(union SDL_Event, motion),
    sizeof(((union SDL_Event *)0)->motion),
    offsetof(union SDL_Event, button),
    sizeof(((union SDL_Event *)0)->button),
    offsetof(union SDL_Event, jaxis),
    sizeof(((union SDL_Event *)0)->jaxis),
    offsetof(union SDL_Event, jball),
    sizeof(((union SDL_Event *)0)->jball),
    offsetof(union SDL_Event, jhat),
    sizeof(((union SDL_Event *)0)->jhat),
    offsetof(union SDL_Event, jbutton),
    sizeof(((union SDL_Event *)0)->jbutton),
    offsetof(union SDL_Event, resize),
    sizeof(((union SDL_Event *)0)->resize),
    offsetof(union SDL_Event, expose),
    sizeof(((union SDL_Event *)0)->expose),
    offsetof(union SDL_Event, quit),
    sizeof(((union SDL_Event *)0)->quit),
    offsetof(union SDL_Event, user),
    sizeof(((union SDL_Event *)0)->user),
    offsetof(union SDL_Event, syswm),
    sizeof(((union SDL_Event *)0)->syswm),
    -1
  };
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_union_SDL_Event(0);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_TTF_STYLE_UNDERLINE(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"_cffi_layout__SDL_Palette", _cffi_layout__SDL_Palette, METH_NOARGS, NULL},
  {"IMG_GetError", _cffi_f_IMG_GetError, METH_NOARGS, NULL},
  {"IMG_Load", _cffi_f_IMG_Load, METH_O, NULL},
  {"IMG_LoadTyped_RW", _cffi_f_IMG_LoadTyped_RW, METH_VARARGS, NULL},
  {"Mix_AllocateChannels", _cffi_f_Mix_AllocateChannels, METH_O, NULL},
  {"Mix_ChannelFinished", _cffi_f_Mix_ChannelFinished, METH_O, NULL},
  {"Mix_CloseAudio", _cffi_f_Mix_CloseAudio, METH_NOARGS, NULL},
  {"Mix_FadeInChannelTimed", _cffi_f_Mix_FadeInChannelTimed, METH_VARARGS, NULL},
  {"Mix_FadeInMusicPos", _cffi_f_Mix_FadeInMusicPos, METH_VARARGS, NULL},
  {"Mix_FadeOutChannel", _cffi_f_Mix_FadeOutChannel, METH_VARARGS, NULL},
  {"Mix_FadeOutGroup", _cffi_f_Mix_FadeOutGroup, METH_VARARGS, NULL},
  {"Mix_FadeOutMusic", _cffi_f_Mix_FadeOutMusic, METH_O, NULL},
  {"Mix_FreeChunk", _cffi_f_Mix_FreeChunk, METH_O, NULL},
  {"Mix_FreeMusic", _cffi_f_Mix_FreeMusic, METH_O, NULL},
  {"Mix_GroupAvailable", _cffi_f_Mix_GroupAvailable, METH_O, NULL},
  {"Mix_GroupChannel", _cffi_f_Mix_GroupChannel, METH_VARARGS, NULL},
  {"Mix_GroupCount", _cffi_f_Mix_GroupCount, METH_O, NULL},
  {"Mix_GroupOldest", _cffi_f_Mix_GroupOldest, METH_O, NULL},
  {"Mix_HaltChannel", _cffi_f_Mix_HaltChannel, METH_O, NULL},
  {"Mix_HaltMusic", _cffi_f_Mix_HaltMusic, METH_NOARGS, NULL},
  {"Mix_HookMusicFinished", _cffi_f_Mix_HookMusicFinished, METH_O, NULL},
  {"Mix_LoadMUS", _cffi_f_Mix_LoadMUS, METH_O, NULL},
  {"Mix_LoadMUS_RW", _cffi_f_Mix_LoadMUS_RW, METH_O, NULL},
  {"Mix_LoadWAV_RW", _cffi_f_Mix_LoadWAV_RW, METH_VARARGS, NULL},
  {"Mix_OpenAudio", _cffi_f_Mix_OpenAudio, METH_VARARGS, NULL},
  {"Mix_Pause", _cffi_f_Mix_Pause, METH_O, NULL},
  {"Mix_PauseMusic", _cffi_f_Mix_PauseMusic, METH_NOARGS, NULL},
  {"Mix_PausedMusic", _cffi_f_Mix_PausedMusic, METH_NOARGS, NULL},
  {"Mix_PlayChannelTimed", _cffi_f_Mix_PlayChannelTimed, METH_VARARGS, NULL},
  {"Mix_PlayMusic", _cffi_f_Mix_PlayMusic, METH_VARARGS, NULL},
  {"Mix_Playing", _cffi_f_Mix_Playing, METH_O, NULL},
  {"Mix_PlayingMusic", _cffi_f_Mix_PlayingMusic, METH_NOARGS, NULL},
  {"Mix_QuerySpec", _cffi_f_Mix_QuerySpec, METH_VARARGS, NULL},
  {"Mix_ReserveChannels", _cffi_f_Mix_ReserveChannels, METH_O, NULL},
  {"Mix_Resume", _cffi_f_Mix_Resume, METH_O, NULL},
  {"Mix_ResumeMusic", _cffi_f_Mix_ResumeMusic, METH_NOARGS, NULL},
  {"Mix_RewindMusic", _cffi_f_Mix_RewindMusic, METH_NOARGS, NULL},
  {"Mix_SetMusicPosition", _cffi_f_Mix_SetMusicPosition, METH_O, NULL},
  {"Mix_SetPanning", _cffi_f_Mix_SetPanning, METH_VARARGS, NULL},
  {"Mix_SetPostMix", _cffi_f_Mix_SetPostMix, METH_VARARGS, NULL},
  {"Mix_Volume", _cffi_f_Mix_Volume, METH_VARARGS, NULL},
  {"Mix_VolumeChunk", _cffi_f_Mix_VolumeChunk, METH_VARARGS, NULL},
  {"Mix_VolumeMusic", _cffi_f_Mix_VolumeMusic, METH_O, NULL},
  {"SDL_AddTimer", _cffi_f_SDL_AddTimer, METH_VARARGS, NULL},
  {"SDL_AllocRW", _cffi_f_SDL_AllocRW, METH_NOARGS, NULL},
  {"SDL_BlitSurface", _cffi_f_SDL_BlitSurface, METH_VARARGS, NULL},
  {"SDL_ClearError", _cffi_f_SDL_ClearError, METH_NOARGS, NULL},
  {"SDL_ConvertSurface", _cffi_f_SDL_ConvertSurface, METH_VARARGS, NULL},
  {"SDL_CreateCursor", _cffi_f_SDL_CreateCursor, METH_VARARGS, NULL},
  {"SDL_CreateRGBSurface", _cffi_f_SDL_CreateRGBSurface, METH_VARARGS, NULL},
  {"SDL_CreateRGBSurfaceFrom", _cffi_f_SDL_CreateRGBSurfaceFrom, METH_VARARGS, NULL},
  {"SDL_Delay", _cffi_f_SDL_Delay, METH_O, NULL},
  {"SDL_DisplayFormat", _cffi_f_SDL_DisplayFormat, METH_O, NULL},
  {"SDL_DisplayFormatAlpha", _cffi_f_SDL_DisplayFormatAlpha, METH_O, NULL},
  {"SDL_EnableKeyRepeat", _cffi_f_SDL_EnableKeyRepeat, METH_VARARGS, NULL},
  {"SDL_EnableUNICODE", _cffi_f_SDL_EnableUNICODE, METH_O, NULL},
  {"SDL_EventState", _cffi_f_SDL_EventState, METH_VARARGS, NULL},
  {"SDL_FillRect", _cffi_f_SDL_FillRect, METH_VARARGS, NULL},
  {"SDL_Flip", _cffi_f_SDL_Flip, METH_O, NULL},
  {"SDL_FreeCursor", _cffi_f_SDL_FreeCursor, METH_O, NULL},
  {"SDL_FreeSurface", _cffi_f_SDL_FreeSurface, METH_O, NULL},
  {"SDL_GL_GetAttribute", _cffi_f_SDL_GL_GetAttribute, METH_VARARGS, NULL},
  {"SDL_GL_SetAttribute", _cffi_f_SDL_GL_SetAttribute, METH_VARARGS, NULL},
  {"SDL_GL_SwapBuffers", _cffi_f_SDL_GL_SwapBuffers, METH_NOARGS, NULL},
  {"SDL_GetAppState", _cffi_f_SDL_GetAppState, METH_NOARGS, NULL},
  {"SDL_GetClipRect", _cffi_f_SDL_GetClipRect, METH_VARARGS, NULL},
  {"SDL_GetCursor", _cffi_f_SDL_GetCursor, METH_NOARGS, NULL},
  {"SDL_GetError", _cffi_f_SDL_GetError, METH_NOARGS, NULL},
  {"SDL_GetKeyName", _cffi_f_SDL_GetKeyName, METH_O, NULL},
  {"SDL_GetKeyRepeat", _cffi_f_SDL_GetKeyRepeat, METH_VARARGS, NULL},
  {"SDL_GetKeyState", _cffi_f_SDL_GetKeyState, METH_O, NULL},
  {"SDL_GetModState", _cffi_f_SDL_GetModState, METH_NOARGS, NULL},
  {"SDL_GetMouseState", _cffi_f_SDL_GetMouseState, METH_VARARGS, NULL},
  {"SDL_GetRGBA", _cffi_f_SDL_GetRGBA, METH_VARARGS, NULL},
  {"SDL_GetRelativeMouseState", _cffi_f_SDL_GetRelativeMouseState, METH_VARARGS, NULL},
  {"SDL_GetTicks", _cffi_f_SDL_GetTicks, METH_NOARGS, NULL},
  {"SDL_GetVideoInfo", _cffi_f_SDL_GetVideoInfo, METH_NOARGS, NULL},
  {"SDL_GetVideoSurface", _cffi_f_SDL_GetVideoSurface, METH_NOARGS, NULL},
  {"SDL_Init", _cffi_f_SDL_Init, METH_O, NULL},
  {"SDL_InitSubSystem", _cffi_f_SDL_InitSubSystem, METH_O, NULL},
  {"SDL_Linked_Version", _cffi_f_SDL_Linked_Version, METH_NOARGS, NULL},
  {"SDL_ListModes", _cffi_f_SDL_ListModes, METH_VARARGS, NULL},
  {"SDL_LoadBMP", _cffi_f_SDL_LoadBMP, METH_O, NULL},
  {"SDL_LoadBMP_RW", _cffi_f_SDL_LoadBMP_RW, METH_VARARGS, NULL},
  {"SDL_LockSurface", _cffi_f_SDL_LockSurface, METH_O, NULL},
  {"SDL_MapRGBA", _cffi_f_SDL_MapRGBA, METH_VARARGS, NULL},
  {"SDL_PeepEvents", _cffi_f_SDL_PeepEvents, METH_VARARGS, NULL},
  {"SDL_PollEvent", _cffi_f_SDL_PollEvent, METH_O, NULL},
  {"SDL_PumpEvents", _cffi_f_SDL_PumpEvents, METH_NOARGS, NULL},
  {"SDL_PushEvent", _cffi_f_SDL_PushEvent, METH_O, NULL},
  {"SDL_Quit", _cffi_f_SDL_Quit, METH_NOARGS, NULL},
  {"SDL_QuitSubSystem", _cffi_f_SDL_QuitSubSystem, METH_O, NULL},
  {"SDL_RWFromFP", _cffi_f_SDL_RWFromFP, METH_VARARGS, NULL},
  {"SDL_RWFromFile", _cffi_f_SDL_RWFromFile, METH_VARARGS, NULL},
  {"SDL_RWclose", _cffi_f_SDL_RWclose, METH_O, NULL},
  {"SDL_RWwrite", _cffi_f_SDL_RWwrite, METH_VARARGS, NULL},
  {"SDL_RemoveTimer", _cffi_f_SDL_RemoveTimer, METH_O, NULL},
  {"SDL_SaveBMP", _cffi_f_SDL_SaveBMP, METH_VARARGS, NULL},
  {"SDL_SetAlpha", _cffi_f_SDL_SetAlpha, METH_VARARGS, NULL},
  {"SDL_SetClipRect", _cffi_f_SDL_SetClipRect, METH_VARARGS, NULL},
  {"SDL_SetColorKey", _cffi_f_SDL_SetColorKey, METH_VARARGS, NULL},
  {"SDL_SetColors", _cffi_f_SDL_SetColors, METH_VARARGS, NULL},
  {"SDL_SetCursor", _cffi_f_SDL_SetCursor, METH_O, NULL},
  {"SDL_SetGamma", _cffi_f_SDL_SetGamma, METH_VARARGS, NULL},
  {"SDL_SetGammaRamp", _cffi_f_SDL_SetGammaRamp, METH_VARARGS, NULL},
  {"SDL_SetModState", _cffi_f_SDL_SetModState, METH_O, NULL},
  {"SDL_SetPalette", _cffi_f_SDL_SetPalette, METH_VARARGS, NULL},
  {"SDL_SetVideoMode", _cffi_f_SDL_SetVideoMode, METH_VARARGS, NULL},
  {"SDL_ShowCursor", _cffi_f_SDL_ShowCursor, METH_O, NULL},
  {"SDL_UnlockSurface", _cffi_f_SDL_UnlockSurface, METH_O, NULL},
  {"SDL_UpdateRect", _cffi_f_SDL_UpdateRect, METH_VARARGS, NULL},
  {"SDL_UpdateRects", _cffi_f_SDL_UpdateRects, METH_VARARGS, NULL},
  {"SDL_VideoDriverName", _cffi_f_SDL_VideoDriverName, METH_VARARGS, NULL},
  {"SDL_VideoInit", _cffi_f_SDL_VideoInit, METH_VARARGS, NULL},
  {"SDL_VideoModeOK", _cffi_f_SDL_VideoModeOK, METH_VARARGS, NULL},
  {"SDL_WM_GetCaption", _cffi_f_SDL_WM_GetCaption, METH_VARARGS, NULL},
  {"SDL_WM_GrabInput", _cffi_f_SDL_WM_GrabInput, METH_O, NULL},
  {"SDL_WM_IconifyWindow", _cffi_f_SDL_WM_IconifyWindow, METH_NOARGS, NULL},
  {"SDL_WM_SetCaption", _cffi_f_SDL_WM_SetCaption, METH_VARARGS, NULL},
  {"SDL_WM_SetIcon", _cffi_f_SDL_WM_SetIcon, METH_VARARGS, NULL},
  {"SDL_WM_ToggleFullScreen", _cffi_f_SDL_WM_ToggleFullScreen, METH_O, NULL},
  {"SDL_WaitEvent", _cffi_f_SDL_WaitEvent, METH_O, NULL},
  {"SDL_WarpMouse", _cffi_f_SDL_WarpMouse, METH_VARARGS, NULL},
  {"SDL_WasInit", _cffi_f_SDL_WasInit, METH_O, NULL},
  {"TTF_CloseFont", _cffi_f_TTF_CloseFont, METH_O, NULL},
  {"TTF_FontAscent", _cffi_f_TTF_FontAscent, METH_O, NULL},
  {"TTF_FontDescent", _cffi_f_TTF_FontDescent, METH_O, NULL},
  {"TTF_FontHeight", _cffi_f_TTF_FontHeight, METH_O, NULL},
  {"TTF_FontLineSkip", _cffi_f_TTF_FontLineSkip, METH_O, NULL},
  {"TTF_GetError", _cffi_f_TTF_GetError, METH_NOARGS, NULL},
  {"TTF_GetFontStyle", _cffi_f_TTF_GetFontStyle, METH_O, NULL},
  {"TTF_GlyphMetrics", _cffi_f_TTF_GlyphMetrics, METH_VARARGS, NULL},
  {"TTF_Init", _cffi_f_TTF_Init, METH_NOARGS, NULL},
  {"TTF_OpenFont", _cffi_f_TTF_OpenFont, METH_VARARGS, NULL},
  {"TTF_OpenFontIndexRW", _cffi_f_TTF_OpenFontIndexRW, METH_VARARGS, NULL},
  {"TTF_OpenFontRW", _cffi_f_TTF_OpenFontRW, METH_VARARGS, NULL},
  {"TTF_Quit", _cffi_f_TTF_Quit, METH_NOARGS, NULL},
  {"TTF_RenderUTF8_Blended", _cffi_f_TTF_RenderUTF8_Blended, METH_VARARGS, NULL},
  {"TTF_RenderUTF8_Shaded", _cffi_f_TTF_RenderUTF8_Shaded, METH_VARARGS, NULL},
  {"TTF_RenderUTF8_Solid", _cffi_f_TTF_RenderUTF8_Solid, METH_VARARGS, NULL},
  {"TTF_SetFontStyle", _cffi_f_TTF_SetFontStyle, METH_VARARGS, NULL},
  {"TTF_SizeUTF8", _cffi_f_TTF_SizeUTF8, METH_VARARGS, NULL},
  {"TTF_WasInit", _cffi_f_TTF_WasInit, METH_NOARGS, NULL},
  {"_pygame_SDL_BUTTON", _cffi_f__pygame_SDL_BUTTON, METH_O, NULL},
  {"_pygame_SaveTGA_RW", _cffi_f__pygame_SaveTGA_RW, METH_VARARGS, NULL},
  {"free", _cffi_f_free, METH_O, NULL},
  {"pygame_Blit", _cffi_f_pygame_Blit, METH_VARARGS, NULL},
  {"rotate", _cffi_f_rotate, METH_VARARGS, NULL},
  {"rotate90", _cffi_f_rotate90, METH_VARARGS, NULL},
  {"scale2x", _cffi_f_scale2x, METH_VARARGS, NULL},
  {"surface_fill_blend", _cffi_f_surface_fill_blend, METH_VARARGS, NULL},
  {"_cffi_layout_struct_Mix_Chunk", _cffi_layout_struct_Mix_Chunk, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_ActiveEvent", _cffi_layout_struct_SDL_ActiveEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_Color", _cffi_layout_struct_SDL_Color, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_Cursor", _cffi_layout_struct_SDL_Cursor, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_ExposeEvent", _cffi_layout_struct_SDL_ExposeEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_JoyAxisEvent", _cffi_layout_struct_SDL_JoyAxisEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_JoyBallEvent", _cffi_layout_struct_SDL_JoyBallEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_JoyButtonEvent", _cffi_layout_struct_SDL_JoyButtonEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_JoyHatEvent", _cffi_layout_struct_SDL_JoyHatEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_KeyboardEvent", _cffi_layout_struct_SDL_KeyboardEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_MouseButtonEvent", _cffi_layout_struct_SDL_MouseButtonEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_MouseMotionEvent", _cffi_layout_struct_SDL_MouseMotionEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_PixelFormat", _cffi_layout_struct_SDL_PixelFormat, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_QuitEvent", _cffi_layout_struct_SDL_QuitEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_RWops", _cffi_layout_struct_SDL_RWops, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_Rect", _cffi_layout_struct_SDL_Rect, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_ResizeEvent", _cffi_layout_struct_SDL_ResizeEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_Surface", _cffi_layout_struct_SDL_Surface, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_SysWMEvent", _cffi_layout_struct_SDL_SysWMEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_UserEvent", _cffi_layout_struct_SDL_UserEvent, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_VideoInfo", _cffi_layout_struct_SDL_VideoInfo, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_keysym", _cffi_layout_struct_SDL_keysym, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SDL_version", _cffi_layout_struct_SDL_version, METH_NOARGS, NULL},
  {"_cffi_layout_union_SDL_Event", _cffi_layout_union_SDL_Event, METH_NOARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "_sdl_lib",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit__sdl_lib(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (_cffi_const_SDL_GRAB_ON(lib) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
init_sdl_lib(void)
{
  PyObject *lib;
  lib = Py_InitModule("_sdl_lib", _cffi_methods);
  if (lib == NULL)
    return;
  if (_cffi_const_SDL_GRAB_ON(lib) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
