/** 
* @file guifontdata_ft2.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\font_ft2\guifontdata_ft2.h>



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
	CGUIFontData_ft2::~CGUIFontData_ft2()
	{
		Unload();
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::CGUIFontData_ft2( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex )
		:CGUIFontData( rName, rSceneName, nFontIndex )
		,m_strPath( rPath )
	{

	}
	//------------------------------------------------------------------------------
	int32	CGUIFontData_ft2::DoLoad() const
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException( "[CGUIFontData::DoLoad]: failed to get font interface" );
			return -1;
		}

		//get full path
		CGUIString strFullPath = CGUISceneInfoManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;
		if( 0 != pFont->LoadFontFace( strFullPath, m_nFontIndex ))
		{
			throw CGUIException( "[CGUIFontData::DoLoad]: failed to get load font from path <%s>", strFullPath.c_str() );
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIFontData_ft2::DoUnload()
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException( "[CGUIFontData::DoUnload]: failed to get font interface" );
			return;
		}
		pFont->UnloadFontFace( m_nFontIndex );
	}
	//------------------------------------------------------------------------------
}//guiex

