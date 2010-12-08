/** 
 * @file guiinterfacefont.cpp
 * @brief interface for font
 * @author ken
 * @date 2006-08-05
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacemanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceFont::StaticGetModuleName()
	{ 
		return "IGUIFont";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceFont::IGUIInterfaceFont()
		:IGUIInterface(StaticGetModuleName())
	{

	}
//------------------------------------------------------------------------------
	IGUIInterfaceFont::~IGUIInterfaceFont()
	{
	}
//------------------------------------------------------------------------------
	real IGUIInterfaceFont::GetStringWidth(	
		const CGUIStringEx& rString,
		int32 nStartPos /*= 0*/,
		int32 nEndPos/* = -1*/)
	{
		if( nEndPos<0 || nEndPos>int32(rString.m_strContent.size()))
		{
			nEndPos = rString.GetContent().size();
		}

		real fWidth = 0.0f;
		const CGUIStringInfo& rInfo = rString.m_aStringInfo;
		for( int32 i=nStartPos; i<nEndPos; ++i )
		{
			fWidth += GetCharacterSize( rInfo.m_nFontIdx, rString.m_strContent[i], rInfo.m_nFontSize).m_fWidth;
		}
		return fWidth;
	}
}//namespace guiex

