#ifndef glex_INCLUDED
#define glex_INCLUDED
/*-------------------------------------------------------------*/

#ifdef _WIN32
#  define GLEX_WGL_PLATFORM 1
#  define GLEX_GLX_PLATFORM 0
#elif defined(___GLX___F_I_X___M_E___) /* TODO: glx */
#  define GLEX_WGL_PLATFORM 0
#  define GLEX_GLX_PLATFORM 1
#else
#  error "unsupported platform"
#endif

/*-------------------------------------------------------------*/

/* Windows specific section: */

#if GLEX_WGL_PLATFORM

/* Setting to nonzero causes <windows.h> to be not #included by any glex header. */
/* Instead equivalent (hopefully) types and macros will be defined. */
/* Results in *much* faster compiling with Borland C++ Builder 5 */
/* Set to 0 if causes problems. */
/* Ignored on non-windows. */
#define GLEX_FAST_WINDOWS 1

#if GLEX_FAST_WINDOWS

#define CALLBACK        __stdcall
#define WINAPI          __stdcall
#define APIENTRY        WINAPI
#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINGDIAPI       DECLSPEC_IMPORT

typedef void      WGLvoid;
typedef void*     WGLhandle;
typedef int       WGLint;
typedef unsigned  WGLuint;
typedef int       WGLboolean;
typedef float     WGLfloat;
typedef double    WGLdouble;

typedef WGLhandle WGLhandleDC;
typedef WGLhandle WGLhandlePB;

#else /* GLEX_FAST_WINDOWS */

#include <windows.h>

typedef void      WGLvoid;
typedef HANDLE    WGLhandle;
typedef int       WGLint;
typedef UINT      WGLuint;
typedef BOOL      WGLboolean;
typedef float     WGLfloat;
typedef double    WGLdouble;

DECLARE_HANDLE(HPBUFFERARB);

typedef HDC WGLhandleDC;
typedef HPBUFFERARB WGLhandlePB;

#endif /* GLEX_FAST_WINDOWS */
#endif /* GLEX_WGL_PLATFORM */

/*-------------------------------------------------------------*/

/* GLX specific section: */

#if GLEX_GLX_PLATFORM

/* TODO: glx */

#endif /* GLEX_GLX_PLATFORM */

/*-------------------------------------------------------------*/

#include <GL/gl.h>

#include <GL/glex_config.h>

/* define extensions' tokens: */

#include <GL/glex_tokens.h>

/* declare extension IDs: */

enum
{
#define GLEX_DECLARE(n) GLEX_##n ,
#include <GL/glex_names.h>
	GLEX_NAME_COUNT
};

/*-------------------------------------------------------------*/

/* declare function types and pointers: */

#if GLEX_WGL_PLATFORM
#  define GLEX_DECLARE(extname,rettype,funcname,args) typedef rettype (APIENTRY* funcname##_t) args; extern funcname##_t funcname;
#elif GLEX_GLX_PLATFORM
#  define GLEX_DECLARE(extname,rettype,funcname,args) /* TODO: glx */
#endif

#include <GL/glex_functions.h>

/*-------------------------------------------------------------*/

/* glex user interface: */

#ifdef __cplusplus
extern "C" {
#endif

int glexInitialize(void);
int glexIsExtensionSupported(int iExtensionId);
const char* glexGetExtensionName(int iExtensionId);

#ifdef __cplusplus
}
#endif

/*-------------------------------------------------------------*/
#endif
