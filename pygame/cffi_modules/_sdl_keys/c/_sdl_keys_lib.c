
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



    #include <SDL_keysym.h>
    

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

static int _cffi_const_SDLK_FIRST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_FIRST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_FIRST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_UNKNOWN(lib);
}

static int _cffi_const_SDLK_BACKSPACE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_BACKSPACE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_BACKSPACE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_FIRST(lib);
}

static int _cffi_const_SDLK_TAB(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_TAB);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_TAB", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_BACKSPACE(lib);
}

static int _cffi_const_SDLK_CLEAR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_CLEAR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_CLEAR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_TAB(lib);
}

static int _cffi_const_SDLK_RETURN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RETURN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RETURN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_CLEAR(lib);
}

static int _cffi_const_SDLK_PAUSE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_PAUSE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_PAUSE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RETURN(lib);
}

static int _cffi_const_SDLK_ESCAPE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_ESCAPE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_ESCAPE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_PAUSE(lib);
}

static int _cffi_const_SDLK_SPACE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_SPACE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_SPACE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_ESCAPE(lib);
}

static int _cffi_const_SDLK_EXCLAIM(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_EXCLAIM);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_EXCLAIM", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_SPACE(lib);
}

static int _cffi_const_SDLK_QUOTEDBL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_QUOTEDBL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_QUOTEDBL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_EXCLAIM(lib);
}

static int _cffi_const_SDLK_HASH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_HASH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_HASH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_QUOTEDBL(lib);
}

static int _cffi_const_SDLK_DOLLAR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_DOLLAR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_DOLLAR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_HASH(lib);
}

static int _cffi_const_SDLK_AMPERSAND(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_AMPERSAND);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_AMPERSAND", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_DOLLAR(lib);
}

static int _cffi_const_SDLK_QUOTE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_QUOTE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_QUOTE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_AMPERSAND(lib);
}

static int _cffi_const_SDLK_LEFTPAREN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LEFTPAREN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LEFTPAREN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_QUOTE(lib);
}

static int _cffi_const_SDLK_RIGHTPAREN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RIGHTPAREN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RIGHTPAREN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LEFTPAREN(lib);
}

static int _cffi_const_SDLK_ASTERISK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_ASTERISK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_ASTERISK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RIGHTPAREN(lib);
}

static int _cffi_const_SDLK_PLUS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_PLUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_PLUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_ASTERISK(lib);
}

static int _cffi_const_SDLK_COMMA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_COMMA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_COMMA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_PLUS(lib);
}

static int _cffi_const_SDLK_MINUS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_MINUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_MINUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_COMMA(lib);
}

static int _cffi_const_SDLK_PERIOD(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_PERIOD);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_PERIOD", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_MINUS(lib);
}

static int _cffi_const_SDLK_SLASH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_SLASH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_SLASH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_PERIOD(lib);
}

static int _cffi_const_SDLK_0(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_0);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_0", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_SLASH(lib);
}

static int _cffi_const_SDLK_1(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_1);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_1", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_0(lib);
}

static int _cffi_const_SDLK_2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_1(lib);
}

static int _cffi_const_SDLK_3(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_3);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_3", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_2(lib);
}

static int _cffi_const_SDLK_4(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_4);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_4", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_3(lib);
}

static int _cffi_const_SDLK_5(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_5);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_5", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_4(lib);
}

static int _cffi_const_SDLK_6(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_6);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_6", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_5(lib);
}

static int _cffi_const_SDLK_7(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_7);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_7", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_6(lib);
}

static int _cffi_const_SDLK_8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_7(lib);
}

static int _cffi_const_SDLK_9(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_9);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_9", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_8(lib);
}

static int _cffi_const_SDLK_COLON(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_COLON);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_COLON", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_9(lib);
}

static int _cffi_const_SDLK_SEMICOLON(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_SEMICOLON);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_SEMICOLON", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_COLON(lib);
}

static int _cffi_const_SDLK_LESS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LESS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LESS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_SEMICOLON(lib);
}

static int _cffi_const_SDLK_EQUALS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_EQUALS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_EQUALS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LESS(lib);
}

static int _cffi_const_SDLK_GREATER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_GREATER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_GREATER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_EQUALS(lib);
}

static int _cffi_const_SDLK_QUESTION(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_QUESTION);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_QUESTION", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_GREATER(lib);
}

static int _cffi_const_SDLK_AT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_AT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_AT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_QUESTION(lib);
}

static int _cffi_const_SDLK_LEFTBRACKET(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LEFTBRACKET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LEFTBRACKET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_AT(lib);
}

static int _cffi_const_SDLK_BACKSLASH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_BACKSLASH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_BACKSLASH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LEFTBRACKET(lib);
}

static int _cffi_const_SDLK_RIGHTBRACKET(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RIGHTBRACKET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RIGHTBRACKET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_BACKSLASH(lib);
}

static int _cffi_const_SDLK_CARET(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_CARET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_CARET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RIGHTBRACKET(lib);
}

static int _cffi_const_SDLK_UNDERSCORE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_UNDERSCORE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_UNDERSCORE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_CARET(lib);
}

static int _cffi_const_SDLK_BACKQUOTE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_BACKQUOTE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_BACKQUOTE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_UNDERSCORE(lib);
}

static int _cffi_const_SDLK_a(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_a);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_a", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_BACKQUOTE(lib);
}

static int _cffi_const_SDLK_b(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_b);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_b", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_a(lib);
}

static int _cffi_const_SDLK_c(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_c);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_c", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_b(lib);
}

static int _cffi_const_SDLK_d(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_d);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_d", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_c(lib);
}

static int _cffi_const_SDLK_e(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_e);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_e", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_d(lib);
}

static int _cffi_const_SDLK_f(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_f);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_f", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_e(lib);
}

static int _cffi_const_SDLK_g(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_g);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_g", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_f(lib);
}

static int _cffi_const_SDLK_h(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_h);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_h", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_g(lib);
}

static int _cffi_const_SDLK_i(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_i);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_i", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_h(lib);
}

static int _cffi_const_SDLK_j(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_j);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_j", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_i(lib);
}

static int _cffi_const_SDLK_k(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_k);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_k", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_j(lib);
}

static int _cffi_const_SDLK_l(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_l);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_l", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_k(lib);
}

static int _cffi_const_SDLK_m(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_m);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_m", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_l(lib);
}

static int _cffi_const_SDLK_n(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_n);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_n", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_m(lib);
}

static int _cffi_const_SDLK_o(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_o);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_o", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_n(lib);
}

static int _cffi_const_SDLK_p(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_p);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_p", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_o(lib);
}

static int _cffi_const_SDLK_q(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_q);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_q", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_p(lib);
}

static int _cffi_const_SDLK_r(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_r);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_r", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_q(lib);
}

static int _cffi_const_SDLK_s(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_s);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_s", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_r(lib);
}

static int _cffi_const_SDLK_t(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_t);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_t", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_s(lib);
}

static int _cffi_const_SDLK_u(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_u);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_u", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_t(lib);
}

static int _cffi_const_SDLK_v(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_v);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_v", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_u(lib);
}

static int _cffi_const_SDLK_w(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_w);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_w", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_v(lib);
}

static int _cffi_const_SDLK_x(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_x);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_x", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_w(lib);
}

static int _cffi_const_SDLK_y(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_y);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_y", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_x(lib);
}

static int _cffi_const_SDLK_z(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_z);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_z", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_y(lib);
}

static int _cffi_const_SDLK_DELETE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_DELETE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_DELETE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_z(lib);
}

static int _cffi_const_SDLK_WORLD_0(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_0);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_0", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_DELETE(lib);
}

static int _cffi_const_SDLK_WORLD_1(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_1);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_1", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_0(lib);
}

static int _cffi_const_SDLK_WORLD_2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_1(lib);
}

static int _cffi_const_SDLK_WORLD_3(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_3);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_3", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_2(lib);
}

static int _cffi_const_SDLK_WORLD_4(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_4);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_4", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_3(lib);
}

static int _cffi_const_SDLK_WORLD_5(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_5);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_5", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_4(lib);
}

static int _cffi_const_SDLK_WORLD_6(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_6);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_6", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_5(lib);
}

static int _cffi_const_SDLK_WORLD_7(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_7);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_7", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_6(lib);
}

static int _cffi_const_SDLK_WORLD_8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_7(lib);
}

static int _cffi_const_SDLK_WORLD_9(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_9);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_9", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_8(lib);
}

static int _cffi_const_SDLK_WORLD_10(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_10);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_10", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_9(lib);
}

static int _cffi_const_SDLK_WORLD_11(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_11);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_11", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_10(lib);
}

static int _cffi_const_SDLK_WORLD_12(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_12);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_12", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_11(lib);
}

static int _cffi_const_SDLK_WORLD_13(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_13);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_13", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_12(lib);
}

static int _cffi_const_SDLK_WORLD_14(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_14);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_14", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_13(lib);
}

static int _cffi_const_SDLK_WORLD_15(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_15);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_15", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_14(lib);
}

static int _cffi_const_SDLK_WORLD_16(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_16);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_16", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_15(lib);
}

static int _cffi_const_SDLK_WORLD_17(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_17);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_17", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_16(lib);
}

static int _cffi_const_SDLK_WORLD_18(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_18);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_18", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_17(lib);
}

static int _cffi_const_SDLK_WORLD_19(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_19);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_19", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_18(lib);
}

static int _cffi_const_SDLK_WORLD_20(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_20);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_20", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_19(lib);
}

static int _cffi_const_SDLK_WORLD_21(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_21);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_21", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_20(lib);
}

static int _cffi_const_SDLK_WORLD_22(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_22);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_22", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_21(lib);
}

static int _cffi_const_SDLK_WORLD_23(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_23);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_23", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_22(lib);
}

static int _cffi_const_SDLK_WORLD_24(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_24);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_24", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_23(lib);
}

static int _cffi_const_SDLK_WORLD_25(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_25);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_25", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_24(lib);
}

static int _cffi_const_SDLK_WORLD_26(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_26);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_26", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_25(lib);
}

static int _cffi_const_SDLK_WORLD_27(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_27);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_27", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_26(lib);
}

static int _cffi_const_SDLK_WORLD_28(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_28);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_28", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_27(lib);
}

static int _cffi_const_SDLK_WORLD_29(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_29);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_29", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_28(lib);
}

static int _cffi_const_SDLK_WORLD_30(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_30);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_30", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_29(lib);
}

static int _cffi_const_SDLK_WORLD_31(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_31);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_31", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_30(lib);
}

static int _cffi_const_SDLK_WORLD_32(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_32);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_32", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_31(lib);
}

static int _cffi_const_SDLK_WORLD_33(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_33);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_33", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_32(lib);
}

static int _cffi_const_SDLK_WORLD_34(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_34);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_34", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_33(lib);
}

static int _cffi_const_SDLK_WORLD_35(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_35);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_35", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_34(lib);
}

static int _cffi_const_SDLK_WORLD_36(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_36);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_36", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_35(lib);
}

static int _cffi_const_SDLK_WORLD_37(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_37);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_37", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_36(lib);
}

static int _cffi_const_SDLK_WORLD_38(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_38);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_38", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_37(lib);
}

static int _cffi_const_SDLK_WORLD_39(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_39);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_39", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_38(lib);
}

static int _cffi_const_SDLK_WORLD_40(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_40);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_40", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_39(lib);
}

static int _cffi_const_SDLK_WORLD_41(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_41);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_41", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_40(lib);
}

static int _cffi_const_SDLK_WORLD_42(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_42);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_42", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_41(lib);
}

static int _cffi_const_SDLK_WORLD_43(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_43);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_43", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_42(lib);
}

static int _cffi_const_SDLK_WORLD_44(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_44);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_44", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_43(lib);
}

static int _cffi_const_SDLK_WORLD_45(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_45);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_45", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_44(lib);
}

static int _cffi_const_SDLK_WORLD_46(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_46);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_46", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_45(lib);
}

static int _cffi_const_SDLK_WORLD_47(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_47);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_47", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_46(lib);
}

static int _cffi_const_SDLK_WORLD_48(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_48);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_48", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_47(lib);
}

static int _cffi_const_SDLK_WORLD_49(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_49);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_49", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_48(lib);
}

static int _cffi_const_SDLK_WORLD_50(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_50);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_50", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_49(lib);
}

static int _cffi_const_SDLK_WORLD_51(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_51);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_51", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_50(lib);
}

static int _cffi_const_SDLK_WORLD_52(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_52);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_52", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_51(lib);
}

static int _cffi_const_SDLK_WORLD_53(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_53);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_53", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_52(lib);
}

static int _cffi_const_SDLK_WORLD_54(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_54);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_54", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_53(lib);
}

static int _cffi_const_SDLK_WORLD_55(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_55);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_55", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_54(lib);
}

static int _cffi_const_SDLK_WORLD_56(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_56);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_56", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_55(lib);
}

static int _cffi_const_SDLK_WORLD_57(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_57);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_57", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_56(lib);
}

static int _cffi_const_SDLK_WORLD_58(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_58);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_58", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_57(lib);
}

static int _cffi_const_SDLK_WORLD_59(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_59);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_59", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_58(lib);
}

static int _cffi_const_SDLK_WORLD_60(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_60);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_60", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_59(lib);
}

static int _cffi_const_SDLK_WORLD_61(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_61);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_61", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_60(lib);
}

static int _cffi_const_SDLK_WORLD_62(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_62);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_62", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_61(lib);
}

static int _cffi_const_SDLK_WORLD_63(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_63);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_63", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_62(lib);
}

static int _cffi_const_SDLK_WORLD_64(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_64);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_64", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_63(lib);
}

static int _cffi_const_SDLK_WORLD_65(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_65);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_65", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_64(lib);
}

static int _cffi_const_SDLK_WORLD_66(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_66);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_66", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_65(lib);
}

static int _cffi_const_SDLK_WORLD_67(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_67);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_67", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_66(lib);
}

static int _cffi_const_SDLK_WORLD_68(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_68);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_68", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_67(lib);
}

static int _cffi_const_SDLK_WORLD_69(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_69);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_69", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_68(lib);
}

static int _cffi_const_SDLK_WORLD_70(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_70);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_70", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_69(lib);
}

static int _cffi_const_SDLK_WORLD_71(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_71);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_71", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_70(lib);
}

static int _cffi_const_SDLK_WORLD_72(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_72);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_72", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_71(lib);
}

static int _cffi_const_SDLK_WORLD_73(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_73);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_73", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_72(lib);
}

static int _cffi_const_SDLK_WORLD_74(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_74);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_74", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_73(lib);
}

static int _cffi_const_SDLK_WORLD_75(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_75);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_75", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_74(lib);
}

static int _cffi_const_SDLK_WORLD_76(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_76);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_76", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_75(lib);
}

static int _cffi_const_SDLK_WORLD_77(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_77);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_77", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_76(lib);
}

static int _cffi_const_SDLK_WORLD_78(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_78);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_78", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_77(lib);
}

static int _cffi_const_SDLK_WORLD_79(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_79);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_79", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_78(lib);
}

static int _cffi_const_SDLK_WORLD_80(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_80);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_80", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_79(lib);
}

static int _cffi_const_SDLK_WORLD_81(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_81);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_81", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_80(lib);
}

static int _cffi_const_SDLK_WORLD_82(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_82);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_82", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_81(lib);
}

static int _cffi_const_SDLK_WORLD_83(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_83);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_83", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_82(lib);
}

static int _cffi_const_SDLK_WORLD_84(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_84);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_84", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_83(lib);
}

static int _cffi_const_SDLK_WORLD_85(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_85);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_85", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_84(lib);
}

static int _cffi_const_SDLK_WORLD_86(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_86);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_86", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_85(lib);
}

static int _cffi_const_SDLK_WORLD_87(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_87);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_87", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_86(lib);
}

static int _cffi_const_SDLK_WORLD_88(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_88);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_88", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_87(lib);
}

static int _cffi_const_SDLK_WORLD_89(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_89);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_89", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_88(lib);
}

static int _cffi_const_SDLK_WORLD_90(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_90);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_90", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_89(lib);
}

static int _cffi_const_SDLK_WORLD_91(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_91);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_91", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_90(lib);
}

static int _cffi_const_SDLK_WORLD_92(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_92);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_92", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_91(lib);
}

static int _cffi_const_SDLK_WORLD_93(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_93);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_93", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_92(lib);
}

static int _cffi_const_SDLK_WORLD_94(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_94);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_94", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_93(lib);
}

static int _cffi_const_SDLK_WORLD_95(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_WORLD_95);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_WORLD_95", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_94(lib);
}

static int _cffi_const_SDLK_KP0(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP0);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP0", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_WORLD_95(lib);
}

static int _cffi_const_SDLK_KP1(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP1);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP1", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP0(lib);
}

static int _cffi_const_SDLK_KP2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP1(lib);
}

static int _cffi_const_SDLK_KP3(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP3);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP3", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP2(lib);
}

static int _cffi_const_SDLK_KP4(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP4);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP4", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP3(lib);
}

static int _cffi_const_SDLK_KP5(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP5);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP5", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP4(lib);
}

static int _cffi_const_SDLK_KP6(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP6);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP6", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP5(lib);
}

static int _cffi_const_SDLK_KP7(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP7);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP7", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP6(lib);
}

static int _cffi_const_SDLK_KP8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP7(lib);
}

static int _cffi_const_SDLK_KP9(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP9);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP9", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP8(lib);
}

static int _cffi_const_SDLK_KP_PERIOD(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_PERIOD);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_PERIOD", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP9(lib);
}

static int _cffi_const_SDLK_KP_DIVIDE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_DIVIDE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_DIVIDE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_PERIOD(lib);
}

static int _cffi_const_SDLK_KP_MULTIPLY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_MULTIPLY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_MULTIPLY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_DIVIDE(lib);
}

static int _cffi_const_SDLK_KP_MINUS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_MINUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_MINUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_MULTIPLY(lib);
}

static int _cffi_const_SDLK_KP_PLUS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_PLUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_PLUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_MINUS(lib);
}

static int _cffi_const_SDLK_KP_ENTER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_ENTER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_ENTER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_PLUS(lib);
}

static int _cffi_const_SDLK_KP_EQUALS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_KP_EQUALS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_KP_EQUALS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_ENTER(lib);
}

static int _cffi_const_SDLK_UP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_UP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_UP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_KP_EQUALS(lib);
}

static int _cffi_const_SDLK_DOWN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_DOWN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_DOWN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_UP(lib);
}

static int _cffi_const_SDLK_RIGHT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RIGHT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RIGHT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_DOWN(lib);
}

static int _cffi_const_SDLK_LEFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LEFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LEFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RIGHT(lib);
}

static int _cffi_const_SDLK_INSERT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_INSERT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_INSERT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LEFT(lib);
}

static int _cffi_const_SDLK_HOME(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_HOME);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_HOME", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_INSERT(lib);
}

static int _cffi_const_SDLK_END(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_END);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_END", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_HOME(lib);
}

static int _cffi_const_SDLK_PAGEUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_PAGEUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_PAGEUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_END(lib);
}

static int _cffi_const_SDLK_PAGEDOWN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_PAGEDOWN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_PAGEDOWN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_PAGEUP(lib);
}

static int _cffi_const_SDLK_F1(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F1);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F1", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_PAGEDOWN(lib);
}

static int _cffi_const_SDLK_F2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F1(lib);
}

static int _cffi_const_SDLK_F3(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F3);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F3", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F2(lib);
}

static int _cffi_const_SDLK_F4(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F4);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F4", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F3(lib);
}

static int _cffi_const_SDLK_F5(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F5);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F5", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F4(lib);
}

static int _cffi_const_SDLK_F6(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F6);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F6", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F5(lib);
}

static int _cffi_const_SDLK_F7(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F7);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F7", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F6(lib);
}

static int _cffi_const_SDLK_F8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F7(lib);
}

static int _cffi_const_SDLK_F9(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F9);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F9", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F8(lib);
}

static int _cffi_const_SDLK_F10(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F10);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F10", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F9(lib);
}

static int _cffi_const_SDLK_F11(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F11);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F11", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F10(lib);
}

static int _cffi_const_SDLK_F12(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F12);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F12", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F11(lib);
}

static int _cffi_const_SDLK_F13(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F13);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F13", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F12(lib);
}

static int _cffi_const_SDLK_F14(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F14);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F14", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F13(lib);
}

static int _cffi_const_SDLK_F15(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_F15);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_F15", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F14(lib);
}

static int _cffi_const_SDLK_NUMLOCK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_NUMLOCK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_NUMLOCK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_F15(lib);
}

static int _cffi_const_SDLK_CAPSLOCK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_CAPSLOCK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_CAPSLOCK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_NUMLOCK(lib);
}

static int _cffi_const_SDLK_SCROLLOCK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_SCROLLOCK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_SCROLLOCK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_CAPSLOCK(lib);
}

static int _cffi_const_SDLK_RSHIFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RSHIFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RSHIFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_SCROLLOCK(lib);
}

static int _cffi_const_SDLK_LSHIFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LSHIFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LSHIFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RSHIFT(lib);
}

static int _cffi_const_SDLK_RCTRL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RCTRL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RCTRL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LSHIFT(lib);
}

static int _cffi_const_SDLK_LCTRL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LCTRL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LCTRL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RCTRL(lib);
}

static int _cffi_const_SDLK_RALT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RALT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RALT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LCTRL(lib);
}

static int _cffi_const_SDLK_LALT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LALT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LALT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RALT(lib);
}

static int _cffi_const_SDLK_RMETA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RMETA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RMETA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LALT(lib);
}

static int _cffi_const_SDLK_LMETA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LMETA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LMETA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RMETA(lib);
}

static int _cffi_const_SDLK_LSUPER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_LSUPER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_LSUPER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LMETA(lib);
}

static int _cffi_const_SDLK_RSUPER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_RSUPER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_RSUPER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_LSUPER(lib);
}

static int _cffi_const_SDLK_MODE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_MODE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_MODE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_RSUPER(lib);
}

static int _cffi_const_SDLK_COMPOSE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_COMPOSE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_COMPOSE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_MODE(lib);
}

static int _cffi_const_SDLK_HELP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_HELP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_HELP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_COMPOSE(lib);
}

static int _cffi_const_SDLK_PRINT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_PRINT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_PRINT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_HELP(lib);
}

static int _cffi_const_SDLK_SYSREQ(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_SYSREQ);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_SYSREQ", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_PRINT(lib);
}

static int _cffi_const_SDLK_BREAK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_BREAK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_BREAK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_SYSREQ(lib);
}

static int _cffi_const_SDLK_MENU(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_MENU);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_MENU", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_BREAK(lib);
}

static int _cffi_const_SDLK_POWER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_POWER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_POWER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_MENU(lib);
}

static int _cffi_const_SDLK_EURO(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_EURO);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_EURO", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_POWER(lib);
}

static int _cffi_const_SDLK_UNDO(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(SDLK_UNDO);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "SDLK_UNDO", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_SDLK_EURO(lib);
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
  return _cffi_const_SDLK_UNDO(lib);
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

static int _cffi_const_KMOD_LSHIFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_LSHIFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_LSHIFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_NONE(lib);
}

static int _cffi_const_KMOD_RSHIFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_RSHIFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_RSHIFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_LSHIFT(lib);
}

static int _cffi_const_KMOD_LCTRL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_LCTRL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_LCTRL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_RSHIFT(lib);
}

static int _cffi_const_KMOD_RCTRL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_RCTRL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_RCTRL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_LCTRL(lib);
}

static int _cffi_const_KMOD_LALT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_LALT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_LALT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_RCTRL(lib);
}

static int _cffi_const_KMOD_RALT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_RALT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_RALT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_LALT(lib);
}

static int _cffi_const_KMOD_LMETA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_LMETA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_LMETA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_RALT(lib);
}

static int _cffi_const_KMOD_RMETA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_RMETA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_RMETA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_LMETA(lib);
}

static int _cffi_const_KMOD_NUM(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_NUM);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_NUM", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_RMETA(lib);
}

static int _cffi_const_KMOD_CAPS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_CAPS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_CAPS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_NUM(lib);
}

static int _cffi_const_KMOD_MODE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_MODE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_MODE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_CAPS(lib);
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
  return _cffi_const_KMOD_MODE(lib);
}

static int _cffi_const_KMOD_ALT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_ALT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_ALT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return 0;
}

static int _cffi_const_KMOD_CTRL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_CTRL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_CTRL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_ALT(lib);
}

static int _cffi_const_KMOD_META(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_META);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_META", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_CTRL(lib);
}

static int _cffi_const_KMOD_SHIFT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(KMOD_SHIFT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "KMOD_SHIFT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_KMOD_META(lib);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_KMOD_SHIFT(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "_sdl_keys_lib",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit__sdl_keys_lib(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (_cffi_const_KMOD_RESERVED(lib) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
init_sdl_keys_lib(void)
{
  PyObject *lib;
  lib = Py_InitModule("_sdl_keys_lib", _cffi_methods);
  if (lib == NULL)
    return;
  if (_cffi_const_KMOD_RESERVED(lib) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
