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
#include <libguiex_core/guilogmsgmanager.h>


#if defined(GUIEX_TARGET_ANDROID)
#include <ft2build.h>
#include FT_SYSTEM_H

#include <zip.h>
#endif


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
#if defined(GUIEX_TARGET_ANDROID)
	//------------------------------------------------------------------------------
	static unsigned long GUI_FT_Stream_IoFunc( FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count )
	{
		zip_file* pZipFile = (zip_file*)stream->descriptor.pointer;
	}
	//------------------------------------------------------------------------------
	static void GUI_FT_Stream_CloseFunc( FT_Stream  stream )
	{
		zip_file* pZipFile = (zip_file*)stream->descriptor.pointer;
		zip_fclose( pZipFile );
	}
	//------------------------------------------------------------------------------
#endif


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
		GUI_TRACE( GUI_FORMAT( "[CGUIFontFace_ft2::LoadFont]: Load Font Face File <%s>", rFontPath.c_str()) );

		IGUIFont_ft2* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFontWithTypeCheck<IGUIFont_ft2>();
		if( !pFont )
		{
			CGUIException::ThrowException( "[CGUIFontFace_ft2::LoadFont]: failed to get font interface" );
			return;
		}

#if defined( GUIEX_TARGET_WIN32 ) || defined( GUIEX_TARGET_IOS ) || defined(GUIEX_TARGET_MACOS )
		CGUIString	strFullPath;
		GSystem->GenerateFullPath( rFontPath, strFullPath );
		FT_Error ret = FT_New_Face( pFont->GetFTLibrary( ), strFullPath.c_str(), 0, &m_aFtFace );
		if(  ret != 0 )
		{
			CGUIException::ThrowException( "[CGUIFontFace_ft2::LoadFont]:Could not get font face from file <%s>!", rFontPath.c_str());
			return;
		}
#elif defined(GUIEX_TARGET_ANDROID)
#if 1 
		CGUIString aTmpFontPath = "/system/fonts/DroidSerif-Regular.ttf";
		FT_Error ret = FT_New_Face( pFont->GetFTLibrary( ), aTmpFontPath.c_str(), 0, &m_aFtFace );
		if(  ret != 0 )
		{
			CGUIException::ThrowException( "[CGUIFontFace_ft2::LoadFont]:Could not get font face from file <%s>!", aTmpFontPath.c_str());
			return;
		}
#else
		IGUIFileSys_android* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSysWithTypeCheck<IGUIFileSys_android>();
		if( !pFileSys )
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:Could not get font face from file <%s>!",rFontPath.c_str());
			return;
		}

		zip* pAPKArchive = pFileSys->GetAPKArchive();
		if( !zip )
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:Could not get apk archive!" );
			return;
		}

		CGUIString strFullPath;
		GSystem->GenerateFullPath(rFontPath, strFullPath );

		//get file size
		struct zip_stat aZipStat;
		if( 0 != zip_stat( pAPKArchive, strFullPath.c_str(), 0, &aZipStat ))
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:failed in zip_stat, file %s!", strFullPath.c_str() );
			return;
		}

		uint32 nSize = aZipStat.size;
		if( nSize == 0 )
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:file size is zeor, file %s!", strFullPath.c_str() );
			return;
		}

		zip_file* zip_file = zip_fopen( pAPKArchive, strFullPath.c_str(), 0);
		if( !zip_file )
		{
			CGUIException::ThrowException("[IGUIFont_ft2::LoadFontFace]:Could not open %s in apk archive!", strFullPath.c_str() );
			return;
		}

		FT_StreamDesc desc;
		desc.pointer = (void*)zip_file;

		FT_Stream stream;
		memset(&stream, 0, sizeof(FT_Stream));
		stream.base = 0;
		stream.size = nSize;
		stream.pos = 0; 
		stream.descriptor = desc;
		stream.read = GUI_FT_Stream_IoFunc;
		stream.close = GUI_FT_Stream_CloseFunc;

		FT_Open_Args args;
		memset(&args, 0, sizeof(FT_Open_Args));
		args.flags = FT_OPEN_STREAM;
		args.stream = &stream;

		FT_Error ret = FT_Open_Face(pFont->GetFTLibrary( ), &args, 0, &m_aFtFace);
		if( ret != 0 )
		{
			CGUIException::ThrowException( "[CGUIFontFace_ft2::LoadFont]:Could not get font face from file <%s>!", rFontPath.c_str());
			return;
		}
#endif
#else
#	error "unknown target"
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIFontFace_ft2::UnloadFont()
	{
		GUI_TRACE(GUI_FORMAT( "[CGUIFontFace_ft2::UnloadFont]:\n   Unload Font Face"));

		//unload
		FT_Done_Face(m_aFtFace );
	}
	//------------------------------------------------------------------------------
}//guiex

