/** 
* @file guifont.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-27
*/






//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guibase.h>
#include <libguiex_core/guifont.h>
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
	CGUIFont::CGUIFont( const CGUIString& rName,
		const CGUIString& rSceneName, 
		const CGUIString& rPath,
		uint32	nFontIndex)
		:CGUIResource( rName, rSceneName, "FONT" )
		,m_nFontIndex( nFontIndex )
		,m_strPath(rPath)
	{
		Load();
	}
	//------------------------------------------------------------------------------
	CGUIFont::~CGUIFont()
	{
		Unload();
	}
	//------------------------------------------------------------------------------
	int32	CGUIFont::DoLoad()
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException( "[CGUIFont::DoLoad]: failed to get font interface" );
			return -1;
		}

		//get full path
		CGUIString strFullPath = CGUISceneInfoManager::Instance()->GetSceneFileRootPath( m_strSceneName ) + m_strPath;
		if( 0 != pFont->LoadFontFace( strFullPath, m_nFontIndex ))
		{
			throw CGUIException( "[CGUIFont::DoLoad]: failed to get load font from path <%s>", strFullPath.c_str() );
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIFont::DoUnload()
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			throw CGUIException( "[CGUIFont::DoUnload]: failed to get font interface" );
			return;
		}
		pFont->UnloadFontFace( m_nFontIndex );
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------


}
