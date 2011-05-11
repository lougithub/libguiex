/** 
* @file guifont_ft2.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2006-02-06
*/



//============================================================================//
// include
//============================================================================// 
#include "guifontface_ft2.h"
#include "guifont_ft2.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFontFace_ft2::CGUIFontFace_ft2( const CGUIString& rFontPath )
	{
		LoadFont( rFontPath );
	}
	//------------------------------------------------------------------------------
	CGUIFontFace_ft2::~CGUIFontFace_ft2()
	{
		UnloadFont();
	}
	//------------------------------------------------------------------------------
	FT_Face CGUIFontFace_ft2::GetFontFace()
	{
		return m_aFtFace;
	}
	//------------------------------------------------------------------------------
	void CGUIFontFace_ft2::LoadFont( const CGUIString& rFontPath )
	{
		//load font face
		//GUI_TRACE( GUI_FORMAT( "[IGUIFont_ft2::LoadFontFace]: Load Font Face File <%s>", rFontPath.c_str()) );

		IGUIFont_ft2* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFontWithTypeCheck<IGUIFont_ft2>();
		if( !pFont )
		{
			CGUIException::ThrowException( "[CGUIFontData::DoLoad]: failed to get font interface" );
		}

#if 1
		CGUIString	strFullPath;
		GSystem->GenerateFullPath( rFontPath, strFullPath );
		FT_Error ret = FT_New_Face( pFont->GetFTLibrary( ), strFullPath.c_str(), 0, &m_aFtFace );
		if(  ret != 0 )
		{
			CGUIException::ThrowException(
				"[CGUIFontData_ft2::DoLoad]:Could not get font face from file <%s>!",
				rFontPath.c_str());
		}
#else
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFontPath, aDataChunk ))
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:Could not get font face from file <%s>!",
				rFontPath.c_str());
		}
		FT_Error ret = FT_New_Memory_Face( pFont->GetFTLibrary( ), aDataChunk.GetDataPtr(),aDataChunk.GetSize(), 0, &m_aFtFace );
		if(  ret != 0 )
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:Could not get font face from file <%s>!",
				rFontPath.c_str());
		}
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIFontFace_ft2::UnloadFont()
	{
		//GUI_TRACE(GUI_FORMAT( "[CGUIFontData_ft2::DoUnload]:\n   Unload Font Face"));

		//unload
		FT_Done_Face(m_aFtFace );
	}
	//------------------------------------------------------------------------------
}//guiex

