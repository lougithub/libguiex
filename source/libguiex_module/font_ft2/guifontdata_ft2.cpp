/** 
* @file guifontdata_ft2.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/



//============================================================================//
// include
//============================================================================// 
#include "guifontdata_ft2.h"
#include "guifontface_ft2.h"
#include "guifont_ft2.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guiinterfacemanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::CGUIFontData_ft2( 
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const SFontInfo& rFontInfo,
		CGUIFontFace_ft2* pFontFace)
		:CGUIFontData( rName, rSceneName, rFontInfo )
		,m_pFontFace(pFontFace)
		,m_uTexturePosX(0)
		,m_uTexturePosY(0)
		,m_uMaxHeight(0)
	{
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::~CGUIFontData_ft2()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIFontData_ft2::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIFontData_ft2::DoUnload()
	{
		//release characters data
		while( m_mapCharsData.empty() == false )
		{
			delete m_mapCharsData.begin()->second;
			m_mapCharsData.erase( m_mapCharsData.begin( ) );
		}

		//clear texture
		while(!m_vecTexture.empty())
		{
			CGUITextureManager::Instance( )->DestroyTexture( m_vecTexture.front( ) );
			m_vecTexture.erase(m_vecTexture.begin( ) );
		}
	}
	//------------------------------------------------------------------------------
	FT_Face CGUIFontData_ft2::GetFontFace()
	{
		return m_pFontFace->GetFontFace();
	}
	//------------------------------------------------------------------------------
	SCharData_ft2* CGUIFontData_ft2::GetCharData( wchar charCode )
	{
		Load();

		//get font
		TMapCharData::iterator itorCharData = m_mapCharsData.find(charCode);
		if( itorCharData != m_mapCharsData.end() )
		{
			//bingo! got this font data
			return itorCharData->second;
		}
		else
		{
			return LoadCharData( charCode );
		}
	}
	//------------------------------------------------------------------------------
	SCharData_ft2* CGUIFontData_ft2::LoadCharData( wchar charCode )
	{
		uint16 uFontSize = GetFontSize();
		if( charCode == L'\n')
		{
			//line break
			SCharData_ft2 *pCharData = new SCharData_ft2;	
			pCharData->m_pTexture = NULL;
			pCharData->m_fBitmapWidth = 0;
			pCharData->m_fBitmapHeight = 0;
			pCharData->m_fLeftBearing = 0;
			pCharData->m_fTopBearing = 0;
			pCharData->m_aSize.m_fWidth = 0;
			pCharData->m_aSize.m_fHeight = real(uFontSize);
			pCharData->m_nGlyphIdx = 0;

			//add to map
			m_mapCharsData.insert(std::make_pair(charCode, pCharData));

			return pCharData;
		}

		//normal text

		//set size
		FT_Face pFontFace = GetFontFace();
#if 0
		if( FT_Set_Pixel_Sizes( pFontFace, uFontSize, uFontSize) )
#else
		if( FT_Set_Char_Size( pFontFace, uFontSize << 6, uFontSize << 6, 96, 96) )
#endif
		{
			GUI_THROW( "[IGUIFont_ft2::LoadFontFace]:Could not set char size!");
		}
		//load this font
#if 0
		if( FT_Load_Char( pFontFace, charCode, FT_LOAD_RENDER ))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharData]:Failed to load char, the code is <%x>!", charCode ));
		}
#else
		uint32 uGlyphIdx = FT_Get_Char_Index( pFontFace, charCode );

		if( FT_Load_Glyph( pFontFace, uGlyphIdx, /*FT_LOAD_DEFAULT*/FT_LOAD_RENDER ))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharData]:Failed to load glyph, the code is <%x>!", charCode ));
		}

		if( FT_Render_Glyph( pFontFace->glyph, FT_RENDER_MODE_NORMAL ))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharData]:Failed to render glyph, the code is <%x>!", charCode ));
		}
#endif

		const FT_GlyphSlot& glyph = pFontFace->glyph;

		//get information
		SCharData_ft2 *pCharData = new SCharData_ft2;	
		pCharData->m_fBitmapWidth = real(glyph->bitmap.width);
		pCharData->m_fBitmapHeight = real(glyph->bitmap.rows);
		pCharData->m_fLeftBearing = real(glyph->bitmap_left);
		pCharData->m_fTopBearing = real(glyph->bitmap_top);
		pCharData->m_aSize.m_fWidth = real(glyph->advance.x>>6);
		pCharData->m_aSize.m_fHeight = real(uFontSize);
		pCharData->m_nGlyphIdx = uGlyphIdx;

		//get texture
		uint32 nTextureWidth = GetTextureSize().GetWidth();
		uint32 nTextureHeight = GetTextureSize().GetHeight();
		if( m_uTexturePosX + (glyph->bitmap.width+1) >= nTextureWidth)
		{
			m_uTexturePosX = 0;
			m_uTexturePosY += (m_uMaxHeight+1);
			m_uMaxHeight = uFontSize;
		}
		if( m_vecTexture.empty() || m_uTexturePosY + glyph->bitmap.rows > nTextureHeight )
		{
			m_uTexturePosX = m_uTexturePosY = 0;
			m_uMaxHeight = uFontSize;
			CGUITexture* pNewTexture = CGUITextureManager::Instance()->CreateTexture(nTextureWidth,nTextureHeight,GUI_PF_LUMINANCE_ALPHA_16);
			m_vecTexture.push_back(pNewTexture);
		}
		pCharData->m_pTexture = m_vecTexture.back();

		//copy data
		//get image data
		uint8* pImageData = new uint8[glyph->bitmap.width * glyph->bitmap.rows * 2];
		uint8* pBufferDst = pImageData;
		uint8* pBufferSrc = glyph->bitmap.buffer;
		for( int i=0; i<glyph->bitmap.rows; ++i)
		{
			for( int j=0; j<glyph->bitmap.width; ++j)
			{
				*pBufferDst++ = 0xFF;//*pBufferSrc;//*pBufferSrc;
				*pBufferDst++ = *pBufferSrc;
				++pBufferSrc;
			}	
		}
		pCharData->m_pTexture->CopySubImage(m_uTexturePosX,m_uTexturePosY,glyph->bitmap.width,glyph->bitmap.rows,GUI_PF_LUMINANCE_ALPHA_16,pImageData );
		delete[] pImageData;
		pCharData->m_aUV = CGUIRect(
			//TODO: why add add 0.5f for fix bug.but don't known why add it...
			real(m_uTexturePosX/*+0.5f*/) / nTextureWidth,
			real(m_uTexturePosY) / nTextureHeight,
			real(m_uTexturePosX+glyph->bitmap.width) / nTextureWidth,
			real(m_uTexturePosY+glyph->bitmap.rows) / nTextureHeight);

		m_uTexturePosX += (glyph->bitmap.width+1);
		if( uint32(glyph->bitmap.rows) > m_uMaxHeight )
		{
			m_uMaxHeight = glyph->bitmap.rows;
		}

		//add to map
		m_mapCharsData.insert(std::make_pair(charCode, pCharData));

		return pCharData;
	}
	//------------------------------------------------------------------------------
}//guiex

