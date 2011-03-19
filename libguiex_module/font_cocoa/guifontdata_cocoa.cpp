/** 
* @file guifontdata_cocoa.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/font_cocoa/guifontdata_cocoa.h>
#include <libguiex_module/font_cocoa/guifont_cocoa.h>
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
	// CGUICharData_cocoa
	CGUICharData_cocoa::CGUICharData_cocoa()
		:m_pTexture(NULL)
		,m_nLeftBearing(0)
		,m_nBottomBearing(0)
		,m_nGlyphIdx(0)
		,m_nBitmapWidth(0)
		,m_nBitmapHeight(0)
		,m_nTopBearing(0)
	{
	}
	//------------------------------------------------------------------------------
	CGUICharData_cocoa::~CGUICharData_cocoa()
	{
		m_pTexture = NULL;
	}
	//------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------
	CGUICharsData_cocoa::CGUICharsData_cocoa()
		:m_nX( 0 )
		,m_nY( 0 )
	{
	}
	//------------------------------------------------------------------------------
	CGUICharsData_cocoa::~CGUICharsData_cocoa()
	{
		// release characters data
		while ( m_mapCharsData.empty() == false ) 
		{
			delete m_mapCharsData.begin()->second;
			m_mapCharsData.erase( m_mapCharsData.begin() );
		}
		
		// clear texture
		while ( !m_vecTexture.empty() ) 
		{
			CGUITextureManager::Instance()->DestroyTexture( m_vecTexture.front() );
			m_vecTexture.erase( m_vecTexture.begin() );
		}
	}
	//------------------------------------------------------------------------------
	
	
	
	//------------------------------------------------------------------------------
	CGUIFontData_cocoa::~CGUIFontData_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	CGUIFontData_cocoa::CGUIFontData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex )
		:CGUIFontData( rName, rSceneName, nFontIndex )
		,m_strPath( rPath )
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIFontData_cocoa::DoLoad()
	{
		// load font face
		CGUIString strFullPath = GSystem->GetDataPath() 
								+ CGUISceneManager::Instance()->GetScenePath( m_strSceneName )
								+ m_strPath;
		
		GUI_TRACE( GUI_FORMAT( "[IGUIFont_cocoa::LoadFontFace]: Load Font Face<%d> from File <%s>",
							  m_nFontIndex, strFullPath.c_str()) );
		IGUIFont_cocoa* pFont = reinterpret_cast<IGUIFont_cocoa*> ( CGUIInterfaceManager::Instance()->GetInterfaceFont() );
		if( !pFont )
		{
			throw CGUIException( "[CGUIFontData::DoLoad]: failed to get font interface" );
			return -1;
		}
		
#if 1
		FT_Error ret = FT_New_Face( pFont->GetFTLibrary( ), strFullPath.c_str(), 0, &m_aFtFace );
		if(  ret != 0 )
		{
			throw CGUIException(
								"[CGUIFontData_cocoa::DoLoad]:Could not get font face from file <%s>!",
								strFullPath.c_str());
			return -1;
		}
#else
		IGUIInterfaceFileSys* pFileSys = GUI_GET_INTERFACE(IGUIInterfaceFileSys, "IGUIFileSys");
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( strFullPath, aDataChunk ))
		{
			throw CGUIException("[IGUIFont_cocoa::LoadFontFace]:Could not get font face from file <%s>!",
								strFullPath.c_str());
		}
		FT_Error ret = FT_New_Memory_Face( pFont->GetFTLibrary( ), aDataChunk.GetDataPtr(),aDataChunk.GetSize(), 0, &m_aFtFace );
		if(  ret != 0 )
		{
			throw CGUIException("[IGUIFont_cocoa::LoadFontFace]:Could not get font face from file <%s>!",
								strFullPath.c_str());
		}
#endif
		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIFontData_cocoa::DoUnload()
	{
		GUI_TRACE(GUI_FORMAT( "[CGUIFontData_cocoa::DoUnload]:\n   Unload Font Face <%d>",
							 m_nFontIndex));
		
		//remove data
		for( TMapSizeChars::iterator itor = m_mapSizeChars.begin();
			itor != m_mapSizeChars.end();
			++itor )
		{
			delete itor->second;
		}
		m_mapSizeChars.clear();
		
		//unload
		FT_Done_Face(m_aFtFace );
	}
	//------------------------------------------------------------------------------
}//guiex

