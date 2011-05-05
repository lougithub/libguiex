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

#if defined( GUIEX_TARGET_WIN32 )
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
#elif defined(GUIEX_TARGET_ANDROID)
#include "guiframework_android.h"
namespace guiex
{
	typedef CGUIFramework_Android CGUIFramework;
}
#else
#	error "unknown target"
#endif

#endif	//__GUI_FRAMEWORK_20101210_H__

