/** 
 * @file guidynlib.h
 * @brief to load dynamically library 
 * @author ken
 * @date 2006-06-10
 */

#ifndef __GUI_WIDGETFACTORY_H_20060610__
#define __GUI_WIDGETFACTORY_H_20060610__


//============================================================================//
// include
//============================================================================// 
#if defined(__WIN32__) || defined(_WIN32)
#	include <windows.h>
#elif defined(__linux__)
#   include <dlfcn.h>
#elif defined(__APPLE__) && defined(__MACH__)
#   include <dlfcn.h>
#else
#	error "unknown platform"
#endif


namespace guiex
{
//============================================================================//
// define
//============================================================================// 
#if defined(__WIN32__) || defined(_WIN32)
#    define GUI_DYNLIB_HANDLE HINSTANCE
#    define GUI_DYNLIB_LOAD( a ) LoadLibraryA( a )
#    define GUI_DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define GUI_DYNLIB_UNLOAD( a ) FreeLibrary( a )
#    define GUI_DYNLIB_ERROR( )  "Unknown Error"
#elif defined(__linux__)
#    define GUI_DYNLIB_HANDLE void*
#    define GUI_DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define GUI_DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define GUI_DYNLIB_UNLOAD( a ) dlclose( a )
#    define GUI_DYNLIB_ERROR( ) dlerror( )
#elif defined(__APPLE__) && defined(__MACH__)
#    define GUI_DYNLIB_HANDLE void*
#    define GUI_DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define GUI_DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define GUI_DYNLIB_UNLOAD( a ) dlclose( a )
#    define GUI_DYNLIB_ERROR( ) dlerror( )	
#else
#	error "unknown platform"
#endif
}


#endif //__GUI_WIDGETFACTORY_H_20060610__
