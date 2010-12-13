/** 
* @file guiframework.h
* @brief framework to use this engine
* @author ken
* @date 2010-12-10
*/

#ifndef __GUI_FRAMEWORK_20101210_H__
#define __GUI_FRAMEWORK_20101210_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiex.h>
#include <libguiex_widget/guiwgt.h>

#if defined( GUIEX_PLATFORM_WIN32 )
#include "guiframework_windows.h"
namespace guiex
{
	typedef CGUIFramework_Windows CGUIFramework;
}
#elif defined(GUIEX_PLATFORM_MAC)
#include "guiframework_mac.h"
namespace guiex
{
	typedef CGUIFramework_Mac CGUIFramework;
}
#else
#	error "unknown platform"
#endif

#endif	//__GUI_FRAMEWORK_20101210_H__

