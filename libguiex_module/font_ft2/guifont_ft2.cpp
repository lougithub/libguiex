/** 
* @file guirender_ft2.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2006-02-06
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\font_ft2\guifont_ft2.h>
#include <libguiex_module\font_ft2\guifontdata_ft2.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisize.h>
#include <libguiex_core/guivector2.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiperfmonitor.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>

#include <algorithm>


#include <ft2build.h>
#include FT_FREETYPE_H

//============================================================================//
// define
//============================================================================// 
#define GUI_FT2_TEXTURE_SIZE	512

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIFont_ft2);
	//------------------------------------------------------------------------------
	IGUIFont_ft2::IGUIFont_ft2()
		:m_pFtLib(NULL)
		,m_bEnableKerning(false)
	{
	}
	//------------------------------------------------------------------------------
	IGUIFont_ft2::~IGUIFont_ft2()
	{
	}
	//------------------------------------------------------------------------------
	FT_Library& IGUIFont_ft2::GetFTLibrary()
	{
		return m_pFtLib;
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int IGUIFont_ft2::DoInitialize(void* )
	{
		//initialize library
		if ( FT_Init_FreeType( &m_pFtLib ))
		{
			throw CGUIException("[IGUIFont_ft2::DoInitialize]:Could not init FreeType library!");
		}

		//init datas
		m_arrayFontDatas.resize( GUI_FONT_MAX_NUM, NULL );
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DoDestroy()
	{
		//free library
		if(m_pFtLib)
		{
			FT_Done_FreeType( FT_Library( m_pFtLib ) );
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIFont_ft2::EnableKerning( bool bEnable )
	{
		m_bEnableKerning = bEnable;
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::CGUICharData_ft2* IGUIFont_ft2::LoadFont( 
		CGUIFontData_ft2* pFontFace, 
		CGUIFontData_ft2::CGUICharsData_ft2* pFont,
		wchar_t charCode,
		uint32 nSize)
	{
		if( charCode == L'\n')
		{
			//line break
			CGUIFontData_ft2::CGUICharData_ft2 *pCharData = new CGUIFontData_ft2::CGUICharData_ft2;	
			pCharData->m_pTexture = NULL;
			pCharData->m_nBitmapWidth = 0;
			pCharData->m_nBitmapHeight = 0;
			pCharData->m_nLeftBearing = 0;
			pCharData->m_nBottomBearing = 0;
			pCharData->m_nTopBearing = 0;
			pCharData->m_aSize.m_fWidth = 0;
			pCharData->m_aSize.m_fHeight = real(nSize);
			pCharData->m_nGlyphIdx = 0;

			//add to map
			pFont->m_mapCharsData.insert(std::make_pair(charCode, pCharData));

			return pCharData;
		}
		else
		{
			//normal text

			//set size
			if( FT_Set_Pixel_Sizes( pFontFace->m_aFtFace, nSize, nSize) )
			{
				throw CGUIException( "[IGUIFont_ft2::LoadFontFace]:Could not set char size!");
			}

			//load this font
#if 0
			if( FT_Load_Char( pFontFace->m_aFtFace, charCode, FT_LOAD_RENDER ))
			{
				throw CGUIException("[IGUIFont_ft2::GetFont]:Failed to load char, the code is <%x>!", charCode );
			}
#else
			uint32 uGlyphIdx = FT_Get_Char_Index( pFontFace->m_aFtFace, charCode );

			if( FT_Load_Glyph( pFontFace->m_aFtFace, uGlyphIdx, FT_LOAD_DEFAULT ))
			{
				throw CGUIException("[IGUIFont_ft2::GetFont]:Failed to load glyph, the code is <%x>!", charCode );
			}

			if( FT_Render_Glyph( pFontFace->m_aFtFace->glyph, FT_RENDER_MODE_NORMAL ))
			{
				throw CGUIException("[IGUIFont_ft2::GetFont]:Failed to render glyph, the code is <%x>!", charCode );
			}
#endif

			FT_Bitmap bitmap = pFontFace->m_aFtFace->glyph->bitmap;
			FT_GlyphSlot slot = pFontFace->m_aFtFace->glyph;

			//get information
			CGUIFontData_ft2::CGUICharData_ft2 *pCharData = new CGUIFontData_ft2::CGUICharData_ft2;	
			pCharData->m_nBitmapWidth = bitmap.width;
			pCharData->m_nBitmapHeight = bitmap.rows;
			int textureWidth = CGUITexture::ConvertToTextureSize(pCharData->m_nBitmapWidth);
			int textureHeight = CGUITexture::ConvertToTextureSize(pCharData->m_nBitmapHeight);
			pCharData->m_nLeftBearing = slot->bitmap_left;
			pCharData->m_nBottomBearing = -( bitmap.rows - slot->bitmap_top );
			pCharData->m_nTopBearing = slot->bitmap_top;
			pCharData->m_aSize.m_fWidth = real(slot->advance.x>>6);
			pCharData->m_aSize.m_fHeight = real(nSize);
			pCharData->m_nGlyphIdx = uGlyphIdx;

			//get texture
			if( pFont->m_nX + nSize >= GUI_FT2_TEXTURE_SIZE)
			{
				pFont->m_nX = 0;
				pFont->m_nY += nSize;
			}
			if( pFont->m_vecTexture.empty() || pFont->m_nY + nSize > GUI_FT2_TEXTURE_SIZE )
			{
				pFont->m_nX = pFont->m_nY = 0;
				CGUITexture* pNewTexture = CGUITextureManager::Instance()->CreateTexture(GUI_FT2_TEXTURE_SIZE,GUI_FT2_TEXTURE_SIZE,GUI_PF_ARGB_32);
				pFont->m_vecTexture.push_back(pNewTexture);
			}
			pCharData->m_pTexture = pFont->m_vecTexture.back();

			//copy data
			//get image data
			uint8* pImageData = new uint8[bitmap.width * bitmap.rows * 4];
			uint8* pBufferDst = pImageData;
			uint8* pBufferSrc = bitmap.buffer;
			for( int i=0; i<bitmap.rows; ++i)
			{
				for( int j=0; j<bitmap.width; ++j)
				{
					*pBufferDst++ = 0xFF;//*pBufferSrc;
					*pBufferDst++ = 0xFF;//*pBufferSrc;
					*pBufferDst++ = 0xFF;//*pBufferSrc;
					*pBufferDst++ = *pBufferSrc++;
				}	
			}
			pCharData->m_pTexture->CopySubImage(pFont->m_nX,pFont->m_nY,bitmap.width,bitmap.rows,GUI_PF_ARGB_32,pImageData );
			delete[] pImageData;
			pCharData->m_aUV = CGUIRect(
				real(pFont->m_nX) / GUI_FT2_TEXTURE_SIZE,
				real(pFont->m_nY) / GUI_FT2_TEXTURE_SIZE,
				real(pFont->m_nX+bitmap.width) / GUI_FT2_TEXTURE_SIZE,
				real(pFont->m_nY+bitmap.rows) / GUI_FT2_TEXTURE_SIZE);

			pFont->m_nX += bitmap.width;

			//add to map
			pFont->m_mapCharsData.insert(std::make_pair(charCode, pCharData));

			return pCharData;
		}

	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::CGUICharData_ft2* IGUIFont_ft2::GetFont( uint32 nFontFaceIdx, wchar_t charCode, uint32 nSize )
	{ 
		//get font face
		GUI_ASSERT( nFontFaceIdx < m_arrayFontDatas.size(), "invalid font face index" );
		CGUIFontData_ft2* pFontData = m_arrayFontDatas[nFontFaceIdx];
		if( !pFontData )
		{
			throw CGUIException("[IGUIFont_ft2::GetFont]:The Font which index is <%d> hasn't been loaded!", nFontFaceIdx );
		}
		pFontData->Load();

		//get font
		CGUIFontData_ft2::CGUICharsData_ft2* pSizeChars = NULL;
		CGUIFontData_ft2::TMapSizeChars::iterator itorSizeChars = pFontData->m_mapSizeChars.find (nSize );
		if( itorSizeChars == pFontData->m_mapSizeChars.end() )
		{
			//this font hasn't been loaded
			pSizeChars = new CGUIFontData_ft2::CGUICharsData_ft2;

			//add to map
			pFontData->m_mapSizeChars.insert( std::make_pair( nSize, pSizeChars ));
		}
		else
		{
			//has load this font
			pSizeChars = itorSizeChars->second;
		}

		//get font
		CGUIFontData_ft2::CGUICharsData_ft2::TMapCharData::iterator itorCharData = pSizeChars->m_mapCharsData.find(charCode);
		if( itorCharData != pSizeChars->m_mapCharsData.end() )
		{
			//bingo! got this font data
			return itorCharData->second;
		}
		else
		{
			return LoadFont( pFontData, pSizeChars, charCode, nSize );
		}
	}
	//------------------------------------------------------------------------------
	int32	IGUIFont_ft2::GetKerningGap(
		uint32 nFontFaceIdx, 
		CGUIFontData_ft2::CGUICharData_ft2* pLeftData, 
		CGUIFontData_ft2::CGUICharData_ft2* pRightData,
		uint32 nSize)
	{
		if( !pLeftData || !pRightData)
		{
			return 0;
		}

		//get font face
		GUI_ASSERT( nFontFaceIdx < m_arrayFontDatas.size(), "invalid font face index" );
		CGUIFontData_ft2* pFontData = m_arrayFontDatas[nFontFaceIdx];
		if( !pFontData )
		{
			throw CGUIException("[IGUIFont_ft2::GetKerningGap]:The Font which index is <%d> hasn't been loaded!", nFontFaceIdx );
		}
		pFontData->Load();

		//set size
		if( FT_Set_Pixel_Sizes( pFontData->m_aFtFace, nSize, nSize) )
		{
			throw CGUIException("[IGUIFont_ft2::GetKerningGap]:Failed to set pixel size!");
		}

		FT_Vector  kerning;
		if( FT_Get_Kerning( pFontData->m_aFtFace, pLeftData->m_nGlyphIdx,pRightData->m_nGlyphIdx,FT_KERNING_DEFAULT , &kerning ))
		{
			throw CGUIException("[IGUIFont_ft2::GetKerningGap]:Failed to get kerning!");
		}

		return kerning.x>>6;

	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawCharacter(
		IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		wchar_t charCode, 
		const CGUIStringInfo& rInfo,
		const CGUIVector2& rPos,
		real fAlpha)
	{
		CGUIFontData_ft2::CGUICharData_ft2* pCharData = GetFont( rInfo.m_nFontIdx, charCode, rInfo.m_nFontSize);

		if( pCharData->m_pTexture)
		{
			CGUIRect aCharRect(
				CGUIVector2(rPos.x+real(pCharData->m_nLeftBearing), rPos.y+real(rInfo.m_nFontSize-pCharData->m_nTopBearing)),
				CGUISize(real(pCharData->m_nBitmapWidth),real(pCharData->m_nBitmapHeight)));

			CGUIColor aColor(rInfo.m_aColor);
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( 
				rWorldMatrix,
				aCharRect,
				pRender->GetAndIncZ(), 
				pCharData->m_pTexture->GetTextureImplement(),
				pCharData->m_aUV, 
				eImageOrientation_Normal,
				aColor.GetARGB(),
				aColor.GetARGB(),
				aColor.GetARGB(),
				aColor.GetARGB());
		}
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawString(IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		const CGUIStringEx& rString, 
			const CGUIRect&	rStringRect,
			const uint8&	uTextAlignment,
			real			fAlpha,
			int32 nStartPos,
			int32 nEndPos)
	{
		if( rString.m_strContent.empty())
		{
			//empty string
			return;
		}

		CGUIVector2 aPos;

		real fScaledStringWidth = GetStringWidth(rString);
		real fScaledStringHeight = rString.GetStringInfo().m_nFontSize;

		if(( uTextAlignment & GUI_TA_HORIZON_MASK) == eTextAlignment_Horz_Left )
		{
			//for horizon left
			aPos.x = rStringRect.m_fLeft;
		}
		else if(( uTextAlignment & GUI_TA_HORIZON_MASK) == eTextAlignment_Horz_Right )
		{
			//for horizon right
			aPos.x = rStringRect.m_fRight-fScaledStringWidth;
		}
		else
		{
			//for horizon center
			aPos.x = rStringRect.m_fLeft+(rStringRect.GetWidth()-fScaledStringWidth)/2;
		}

		if(( uTextAlignment & GUI_TA_VERTICAL_MASK) == eTextAlignment_Vert_Up )
		{
			//for vertical up
			aPos.y = rStringRect.m_fTop;
		}
		else if(( uTextAlignment & GUI_TA_VERTICAL_MASK) == eTextAlignment_Vert_Down )
		{
			//for vertical down
			aPos.y = rStringRect.m_fBottom - fScaledStringHeight;
		}
		else
		{
			//for vertical center
			aPos.y = rStringRect.m_fTop + (rStringRect.GetHeight() - fScaledStringHeight) / 2;
		}
		if( nEndPos < 0 || nEndPos >int32( rString.m_strContent.size()))
		{
			nEndPos = rString.m_strContent.size();
		}

		const CGUIStringInfo& rInfo = rString.m_aStringInfo;
		for( int32 i= nStartPos; i<nEndPos; ++i)
		{

			CGUIFontData_ft2::CGUICharData_ft2 * pCharData = GetFont(rInfo.m_nFontIdx, rString.m_strContent[i], rInfo.m_nFontSize);

			if( pCharData->m_pTexture)
			{
				CGUIRect aCharRect(
					CGUIVector2(aPos.x+real(pCharData->m_nLeftBearing), aPos.y+real(rInfo.m_nFontSize-pCharData->m_nTopBearing)),
					CGUISize(real(pCharData->m_nBitmapWidth),real(pCharData->m_nBitmapHeight)));

				//dest area size
				CGUIColor aColor(rInfo.m_aColor);
				aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
				pRender->DrawTile( 
					rWorldMatrix,
					aCharRect,
					pRender->GetAndIncZ(), 
					pCharData->m_pTexture->GetTextureImplement(),
					pCharData->m_aUV, 
					eImageOrientation_Normal,
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB());
			}

			aPos.x+=pCharData->m_aSize.m_fWidth;
		}
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawString(IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		const CGUIStringEx& rString, 
		const CGUIVector2& rPos,
		real fAlpha,
		int32 nStartPos,
		int32 nEndPos)
	{
		if( rString.m_strContent.empty())
		{
			//empty string
			return;
		}
		CGUIVector2 aPos = rPos;

		if( nEndPos<0 || nEndPos>int32(rString.m_strContent.size()))
		{
			nEndPos = rString.m_strContent.size();
		}

		const CGUIStringInfo& rInfo = rString.m_aStringInfo;
		for( int32 i=nStartPos; i<nEndPos; ++i)
		{
			CGUIFontData_ft2::CGUICharData_ft2 * pCharData = GetFont(rInfo.m_nFontIdx, rString.m_strContent[i], rInfo.m_nFontSize);

			if( pCharData->m_pTexture)
			{
				CGUIRect aCharRect(
					CGUIVector2(aPos.x+real(pCharData->m_nLeftBearing), aPos.y+real(rInfo.m_nFontSize-pCharData->m_nTopBearing)),
					CGUISize(real(pCharData->m_nBitmapWidth),real(pCharData->m_nBitmapHeight)));

				//dest area size
				CGUIColor aColor(rInfo.m_aColor);
				aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
				pRender->DrawTile( 
					rWorldMatrix,
					aCharRect,
					pRender->GetAndIncZ(), 
					pCharData->m_pTexture->GetTextureImplement(),
					pCharData->m_aUV, 
					eImageOrientation_Normal,
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB());
			}

			aPos.x+=pCharData->m_aSize.m_fWidth;
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& IGUIFont_ft2::GetCharacterSize(int32 nFontFaceIdx,wchar_t charCode,uint32 nSize)
	{
		CGUIFontData_ft2::CGUICharData_ft2* pCharData = GetFont( nFontFaceIdx, charCode, nSize);

		return pCharData->m_aSize;
	}
	//------------------------------------------------------------------------------
	CGUIFontData* IGUIFont_ft2::CreateFontData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex )
	{
		GUI_ASSERT( nFontIndex < m_arrayFontDatas.size(), "invalid font face index" );

		if( m_arrayFontDatas[nFontIndex] != NULL )
		{
			throw CGUIException("[IGUIFont_ft2::CreateFontData]: the font<%d> has existing", nFontIndex );
			return NULL;
		}
		m_arrayFontDatas[nFontIndex] = new CGUIFontData_ft2( rName, rSceneName, rPath, nFontIndex);
		return m_arrayFontDatas[nFontIndex];
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DestroyFontData( CGUIFontData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		TVecFontData::iterator itor = std::find( m_arrayFontDatas.begin(), m_arrayFontDatas.end(), pData );
		if( itor == m_arrayFontDatas.end() )
		{
			throw CGUIException(
				"[IGUIFont_ft2::DestroyFontData]: the font<%s:%s> doesn't existing", 
				pData->GetName().c_str(),
				pData->GetSceneName().c_str());
			return;
		}
		else
		{
			*itor = NULL;
			delete pData;
		}
	}
	//------------------------------------------------------------------------------
}//guiex

