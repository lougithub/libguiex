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
#include <libguiex_widget_box2d/guiwgt_box2d.h>
#include <libguiex_widget_game/guiwgt_game.h>

#if defined( _GUIEX_TARGET_WIN32 )
#include "guiframework_windows.h"
namespace guiex
{
	typedef CGUIFramework_Windows CGUIFramework;
}
#elif defined(GUIEX_TARGET_IOS)
#include "guiframework_ios.h"
namespace guiex
{
	typedef CGUIFramework_IOS CGUIFramework;
}
#elif defined(GUIEX_TARGET_MACOS)
#include "guiframework_mac.h"
namespace guiex
{
	typedef CGUIFramework_Mac CGUIFramework;
}
#else
#	error "unknown platform"
#endif

#endif	//__GUI_FRAMEWORK_20101210_H__

