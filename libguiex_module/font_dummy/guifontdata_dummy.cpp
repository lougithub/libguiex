/** 
* @file guifontdata_dummy.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/font_dummy/guifontdata_dummy.h>
#include <libguiex_module/font_dummy/guifont_dummy.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guiinterfacemanager.h>


//============================================================================//
// define
//============================================================================// 


//============================================================================//
// class
//============================================================================// 


//============================================================================//
// function
//============================================================================// 
namespace guiex
{		
	//------------------------------------------------------------------------------
	CGUIFontData_dummy::~CGUIFontData_dummy()
	{
		Unload();
	}
	//------------------------------------------------------------------------------
	CGUIFontData_dummy::CGUIFontData_dummy( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex )
		:CGUIFontData( rName, rSceneName, nFontIndex )
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIFontData_dummy::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIFontData_dummy::DoUnload()
	{
	}
	//------------------------------------------------------------------------------
}//guiex

