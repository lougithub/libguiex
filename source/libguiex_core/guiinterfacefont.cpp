/** 
 * @file guiinterfacefont.cpp
 * @brief interface for font
 * @author ken
 * @date 2006-08-05
 */

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacefont.h"
#include "guiinterfacemanager.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceFont::StaticGetModuleType()
	{ 
		return "IGUIFont";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceFont::IGUIInterfaceFont( const char* szModuleName )
		:IGUIInterface(StaticGetModuleType(), szModuleName)
	{

	}
//------------------------------------------------------------------------------
	IGUIInterfaceFont::~IGUIInterfaceFont()
	{
	}
//------------------------------------------------------------------------------
	real IGUIInterfaceFont::GetStringWidth(	
		const CGUIStringRender& rString,
		int32 nStartPos /*= 0*/,
		int32 nEndPos/* = -1*/)
	{
		if( nEndPos<0 || nEndPos>int32(rString.m_strContent.size()))
		{
			nEndPos = rString.GetContent().size();
		}

		real fWidth = 0.0f;
		const CGUIStringRenderInfo& rInfo = rString.m_aStringInfo;
		for( int32 i=nStartPos; i<nEndPos; ++i )
		{
			fWidth += GetCharacterSize( rString.m_strContent[i], rInfo).m_fWidth;
		}
		return fWidth;
	}
}//namespace guiex

