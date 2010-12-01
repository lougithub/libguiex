/** 
* @file guifontdata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-27
*/

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guibase.h>
#include <libguiex_core/guifontdata.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>



//============================================================================//
// function
//============================================================================//


namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIFontData::CGUIFontData( 
		const CGUIString& rName,
		const CGUIString& rSceneName, 
		uint32 nFontIndex)
		:CGUIResource( rName, rSceneName, "FONT" )
		,m_nFontIndex( nFontIndex )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFontData::~CGUIFontData()
	{
	}
	//------------------------------------------------------------------------------
	uint32 CGUIFontData::GetFontIndex() const
	{
		return m_nFontIndex;
	}
	//------------------------------------------------------------------------------
}
