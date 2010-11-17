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
	IGUIInterfaceFont::IGUIInterfaceFont()
		:IGUIInterface("IGUIFont")
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
		if( nEndPos<0 || nEndPos>int32(rString.Size()))
		{
			nEndPos = rString.Size();
		}

		real fWidth = 0.0f;
		for( int32 i=nStartPos; i<nEndPos; ++i )
		{
			const CGUIStringInfo& rInfo = rString.GetInfo(i);
			fWidth += GetCharacterSize( rInfo.m_nFontIdx, rString.GetCharacter(i), rInfo.m_nFontSize).m_fWidth;
		}
		return fWidth;
	}
}//namespace guiex

