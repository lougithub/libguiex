/** 
 * @file guiconfig.h
 * @brief config of this system. 
 * @author ken
 * @date 2006-03-22
 */

#ifndef __GUI_CONFIG_20060322_H__
#define __GUI_CONFIG_20060322_H__

//============================================================================//
// include
//============================================================================//
#pragma warning( disable : 4996 )
#pragma warning( disable : 4251 )



//============================================================================//
// include
//============================================================================//
#include <cassert>
#include <stdlib.h>


//============================================================================//
// define
//============================================================================// 

#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(WIN32)
#define GUIEX_PLATFORM_WIN32 1
#elif defined(__linux__)
#define GUIEX_PLATFORM_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define GUIEX_PLATFORM_MAC 1
#endif


/*************************************************************************
	Dynamic Library import / export control conditional
	Define GUIEX_GENERATE_DLL to export symbols
	Define GUIEX_USE_DLL to import symbols
	none define for use it as a static library
*************************************************************************/

#if defined( GUIEX_PLATFORM_WIN32 )
#	if defined(GUIEX_GENERATE_DLL)
#		define GUIEXPORT		__declspec( dllexport )
#		define GUI_DLL	1
#	elif defined(GUIEX_USE_DLL)
#		define GUIEXPORT		__declspec( dllimport )
#		define GUI_DLL	1
#	else
#		define GUIEXPORT
#		define GUI_DLL	0
#	endif
#else
#	define GUIEXPORT
#	define GUI_DLL	0
#endif



/*************************************************************************
	Message Length, for log.
*************************************************************************/
#ifndef GUI_MAXLOGMSGLEN
#	define GUI_MAXLOGMSGLEN 4 * 1024
#endif



/*************************************************************************
	max path.
*************************************************************************/
#if !defined (PATH_MAX)
#  if defined (_MAX_PATH)
#    define PATH_MAX _MAX_PATH
#  elif defined (MAX_PATH)
#    define PATH_MAX MAX_PATH
#  elif defined(MAXPATHLEN)
#    define PATH_MAX MAXPATHLEN
#  else /* !_MAX_PATH */
#    define PATH_MAX 1024
#  endif /* _MAX_PATH */
#endif /* !PATH_MAX */

#define GUI_MAXPATHLEN  PATH_MAX


//debug
/*************************************************************************
	debug
*************************************************************************/
#define GUI_DEBUG	1

//assert
/*************************************************************************
	assert
*************************************************************************/
#define GUI_ASSERT_ON	1

/*************************************************************************
	exception
*************************************************************************/
//assert it if any exception happens
#define GUI_EXCEPTION_ASSERT	0

/*************************************************************************
	performance monitor
*************************************************************************/
#define GUI_PERFORMANCE_ON	1

#if GUI_PERFORMANCE_ON
#	define GUI_PERF_SECTION_NUM_MAX	100
#	define GUI_PERF_FRAME_COUNT		5000
#endif	//GUI_PERFORMANCE_ON









//ui
/*************************************************************************
	double click time
*************************************************************************/
#define GUI_DBCLICK_TIME 0.250f

/*************************************************************************
	default edit box mask
*************************************************************************/
#define GUI_DEFAULT_EDITBOX_MASK L'*'

/*************************************************************************
	keyboard event
*************************************************************************/

///how many event the keyboard support at one frame
#define GUI_KEYBOARD_EVENT_MAX 50

///how old time should keyboard pressed event generated, in millisecond
//#define GUI_KEYBOARD_PRESSED_TIME 


/*************************************************************************
	mouse event
*************************************************************************/

///how many event the mouse support at one frame
#define GUI_MOUSE_EVENT_MAX 20

/*************************************************************************
	font
*************************************************************************/
#define GUI_FONT_MAX_NUM (32)

#endif //__GUI_CONFIG_20060322_H__


