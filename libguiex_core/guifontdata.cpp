/** 
* @file guifontdata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-27
*/

//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guifontdata.h"
#include "guiinterfacefont.h"
#include "guiinterfacemanager.h"
#include "guiexception.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guisystem.h"
#include "guifontmanager.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIFontData::CGUIFontData( 
		const CGUIString& rName,
		const CGUIString& rSceneName, 
		uint32 nFontID)
		:CGUIResource( rName, rSceneName, "FONT", GSystem->GetFontManager())
		,m_nFontIndex( nFontID )
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
