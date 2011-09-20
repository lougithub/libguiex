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
		const SFontInfo& rFontInfo)
		:CGUIResource( rName, rSceneName, "FONT", GSystem->GetFontManager())
		,m_aFontInfo(rFontInfo)
	{
		
	}
	//------------------------------------------------------------------------------
	CGUIFontData::~CGUIFontData()
	{
	}
	//------------------------------------------------------------------------------
	uint16 CGUIFontData::GetFontID() const
	{
		return m_aFontInfo.m_uID;
	}
	//------------------------------------------------------------------------------
	uint16 CGUIFontData::GetFontSize() const
	{
		return m_aFontInfo.m_uSize;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIFontData::GetFontDesc() const
	{
		return m_aFontInfo.m_strDesc;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIFontData::GetFontPath() const
	{
		return m_aFontInfo.m_strPath;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUIFontData::GetTextureSize() const
	{
		return m_aFontInfo.m_aTextureSize;
	}
	//------------------------------------------------------------------------------
}
