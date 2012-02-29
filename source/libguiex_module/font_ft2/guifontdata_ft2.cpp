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
#include <libguiex_core/guimath.h>

#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftstroke.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	struct Span
	{
		Span() { }
		Span(int32 _x, int32 _y, uint32 _width, uint8 _coverage)
			: x(_x), y(_y), width(_width), coverage(_coverage) { }
		int32 x, y;
		uint32 width;
		uint8 coverage;
	};
	typedef std::vector<Span> Spans;
	// Each time the renderer calls us back we just push another span entry on
	// our list.
	static void RasterCallback(int32 y, int32 count, const FT_Span * const spans, void * const user ) 
	{
		Spans *sptr = (Spans *)user;
		for (int32 i = 0; i < count; ++i) 
		{
			sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
		}
	}
	static void RenderSpans(FT_Library &library, FT_Outline * const outline, Spans *spans) 
	{
		FT_Raster_Params params;
		memset(&params, 0, sizeof(params));
		params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
		params.gray_spans = RasterCallback;
		params.user = spans;
		FT_Outline_Render(library, outline, &params);
	}
	struct SFontRect
	{
		SFontRect() { }
		SFontRect(int32 left, int32 top, int32 right, int32 bottom)
			: xmin(left), xmax(right), ymin(top), ymax(bottom) { }
		void Include(int32 x, int32 y)
		{
			xmin = GUIMin(xmin, x);
			ymin = GUIMin(ymin, y);
			xmax = GUIMax(xmax, x);
			ymax = GUIMax(ymax, y);
		}

		uint32 Width() const { return xmax - xmin + 1; }
		uint32 Height() const { return ymax - ymin + 1; }

		int32 xmin, xmax, ymin, ymax;
	};
	//------------------------------------------------------------------------------




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
			pCharData->m_fBearingX = 0;
			pCharData->m_fBearingY = 0;
			pCharData->m_aSize.m_fWidth = 0;
			pCharData->m_aSize.m_fHeight = real(uFontSize);
			pCharData->m_nGlyphIdx = 0;

			//add to map
			m_mapCharsData.insert(std::make_pair(charCode, pCharData));

			return pCharData;
		}

		SCharData_ft2 * pCharData = NULL;

		//set size
		FT_Face pFontFace = GetFontFace();
		if( FT_Set_Char_Size( pFontFace, 0, uFontSize << 6, 96, 96) )
		{
			GUI_THROW( "[IGUIFont_ft2::LoadCharData]:Could not set char size!");
		}
		//load this font
		uint32 uGlyphIdx = FT_Get_Char_Index( pFontFace, charCode );

		if( m_aFontInfo.m_bHasOutline )
		{
			pCharData = LoadCharDataWithStroke( pFontFace, uGlyphIdx, charCode );
		}
		else
		{
			pCharData = LoadCharDataWithoutStroke( pFontFace, uGlyphIdx, charCode );
		}

		//add to map
		m_mapCharsData.insert(std::make_pair(charCode, pCharData));
		return pCharData;
	}
	//------------------------------------------------------------------------------
	SCharData_ft2* CGUIFontData_ft2::LoadCharDataWithStroke( FT_Face pFontFace,uint32 uGlyphIdx, wchar charCode )
	{
		uint16 uFontSize = GetFontSize();

		bool bUseRGBATexture = false;
		if( m_aFontInfo.m_aOutlineColor.GetAsABGR() != 0xFF000000 ||
			m_aFontInfo.m_aFontColor.GetAsABGR() != 0xFFFFFFFF )
		{
			bUseRGBATexture = true;
		}

		//Load the Glyph for our character.
		if (FT_Load_Glyph(pFontFace, uGlyphIdx, FT_LOAD_NO_BITMAP) )
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithStroke]:Failed to load glyph, the code is <%x>!", charCode ));
		}
		// Need an outline for this to work.
		if (pFontFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithStroke]:Need an outline for this to work, the code is <%x>!", charCode ));
		}

		// Render the basic glyph to a span list.
		Spans spans;
		RenderSpans(m_pFontFace->GetFTLibrary(), &pFontFace->glyph->outline, &spans);
		if( spans.empty() )
		{
			//line break
			SCharData_ft2 *pCharData = new SCharData_ft2;	
			pCharData->m_pTexture = NULL;
			pCharData->m_fBitmapWidth = 0;
			pCharData->m_fBitmapHeight = 0;
			pCharData->m_fBearingX = 0;
			pCharData->m_fBearingY = 0;
			pCharData->m_aSize.m_fWidth = real(uFontSize);
			pCharData->m_aSize.m_fHeight = real(uFontSize);
			pCharData->m_nGlyphIdx = 0;

			return pCharData;
		}

		// Next we need the spans for the outline.
		Spans outlineSpans;
		// Set up a stroker.
		FT_Stroker stroker;
		FT_Stroker_New(m_pFontFace->GetFTLibrary(), &stroker);
		FT_Stroker_Set(stroker, m_aFontInfo.m_uOutlineWidth << 6, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

		FT_Glyph glyph;
		if (FT_Get_Glyph(pFontFace->glyph, &glyph))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithStroke]:FT_Get_Glyph failed, the code is <%x>!", charCode ));
		}

		if (FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithStroke]:FT_Glyph_StrokeBorder failed, the code is <%x>!", charCode ));
		}
		// Again, this needs to be an outline to work.
		if (glyph->format != FT_GLYPH_FORMAT_OUTLINE)
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithStroke]:Need an outline for this to work, the code is <%x>!", charCode ));
		}
		
		// Render the outline spans to the span list
		FT_Outline *o = &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
		RenderSpans(m_pFontFace->GetFTLibrary(), o, &outlineSpans);

		// Clean up afterwards.
		FT_Stroker_Done(stroker);
		FT_Done_Glyph(glyph);

		// Now we need to put it all together.
		SFontRect rect(spans.front().x,spans.front().y,spans.front().x,spans.front().y);

		for (Spans::iterator s = spans.begin();s != spans.end(); ++s)
		{
			rect.Include(s->x, s->y);
			rect.Include(s->x + s->width - 1, s->y);
		}
		for (Spans::iterator s = outlineSpans.begin();
			s != outlineSpans.end(); ++s)
		{
			rect.Include(s->x, s->y);
			rect.Include(s->x + s->width - 1, s->y);
		}
		
		// Get some metrics of our image.
		uint32 imgWidth = rect.Width();
		uint32 imgHeight = rect.Height();
		uint32 imgSize = imgWidth * imgHeight;


		//get information
		SCharData_ft2 *pCharData = new SCharData_ft2;	
		pCharData->m_fBitmapWidth = real(imgWidth);
		pCharData->m_fBitmapHeight = real(imgHeight);
		pCharData->m_fBearingX = real(pFontFace->glyph->metrics.horiBearingX >> 6) - m_aFontInfo.m_uOutlineWidth;
		pCharData->m_fBearingY = real(pFontFace->glyph->metrics.horiBearingY >> 6) + m_aFontInfo.m_uOutlineWidth;
		pCharData->m_aSize.m_fWidth = real((pFontFace->glyph->advance.x>>6)/*+m_aFontInfo.m_uOutlineWidth*2*/);
		//pCharData->m_aSize.m_fHeight = real(uFontSize+m_aFontInfo.m_uOutlineWidth*2);
		pCharData->m_aSize.m_fHeight = real(pFontFace->size->metrics.ascender>>6);
		pCharData->m_nGlyphIdx = uGlyphIdx;

		//get texture
		uint32 nTextureWidth = GetTextureSize().GetWidth();
		uint32 nTextureHeight = GetTextureSize().GetHeight();
		if( m_uTexturePosX + imgWidth > nTextureWidth)
		{
			m_uTexturePosX = 0;
			m_uTexturePosY += m_uMaxHeight;
			m_uMaxHeight = uFontSize;
		}
		if( m_vecTexture.empty() || m_uTexturePosY + imgHeight > nTextureHeight )
		{
			m_uTexturePosX = m_uTexturePosY = 0;
			m_uMaxHeight = uFontSize;
			CGUITexture* pNewTexture = CGUITextureManager::Instance()->CreateTexture(nTextureWidth,nTextureHeight, bUseRGBATexture ? GUI_PF_RGBA_32 : GUI_PF_LUMINANCE_ALPHA_16 );
			m_vecTexture.push_back(pNewTexture);
		}
		pCharData->m_pTexture = m_vecTexture.back();


		// Allocate data for our image and clear it out to transparent.
		uint32 uPixelSize = bUseRGBATexture ? sizeof(GUIRGBA) : sizeof(uint16);
		uint8 *pImageData = new uint8[imgSize*uPixelSize];
		memset(pImageData, 0, uPixelSize * imgSize);

		// Loop over the outline spans and just draw them into the image.
		for (Spans::iterator s = outlineSpans.begin();s != outlineSpans.end(); ++s)
		{
			uint32 spanPos = (imgHeight-1-(s->y-rect.ymin))*imgWidth+s->x-rect.xmin;
			for (uint32 w = 0; w < s->width; ++w)
			{
				if( bUseRGBATexture )
				{
					CGUIColor aColor(m_aFontInfo.m_aOutlineColor);
					aColor.SetAlphaAsByte( s->coverage );
					reinterpret_cast<GUIABGR*>(pImageData)[spanPos + w] = aColor.GetAsABGR();
				}
				else
				{
					uint32 uIdx = (spanPos + w) * uPixelSize;
					pImageData[uIdx+1] = s->coverage;
				}
			}
		}

		// Then loop over the regular glyph spans and blend them into the image.
		for (Spans::iterator s = spans.begin();s != spans.end(); ++s)
		{
			uint32 spanPos = (imgHeight-1-(s->y-rect.ymin))*imgWidth+s->x-rect.xmin;
			for (uint32 w = 0; w < s->width; ++w)
			{
				if( bUseRGBATexture )
				{
					CGUIColor aColorSrc = m_aFontInfo.m_aFontColor;
					aColorSrc.SetAlphaAsByte(s->coverage);
					CGUIColor aColorDst;
					aColorDst.SetAsABGR( reinterpret_cast<GUIABGR*>(pImageData)[spanPos + w] );

					aColorDst.SetRed( aColorDst.GetRed() + ( aColorSrc.GetRed() - aColorDst.GetRed()) * aColorSrc.GetAlpha() );
					aColorDst.SetGreen( aColorDst.GetGreen() + ( aColorSrc.GetGreen() - aColorDst.GetGreen()) * aColorSrc.GetAlpha() );
					aColorDst.SetBlue( aColorDst.GetBlue() + ( aColorSrc.GetBlue() - aColorDst.GetBlue()) * aColorSrc.GetAlpha() );
					//aColorDst.SetAlpha( GUIMin( 1.0f, aColorDst.GetAlpha() + aColorSrc.GetAlpha()));
					aColorDst.SetAlpha( 1.0f );

					reinterpret_cast<GUIABGR*>(pImageData)[spanPos + w] = aColorDst.GetAsABGR();
				}
				else
				{
					uint32 uIdx = (spanPos + w) * uPixelSize;
					//pImageData[uIdx] = 0xFF;
					//pImageData[uIdx] = 0xFF * (s->coverage / 255 );
					//pImageData[uIdx+1] = s->coverage;
					pImageData[uIdx] = GUIMin( 255, int(pImageData[uIdx] + (0xFF - pImageData[uIdx])*(s->coverage / 255.0f)));
					pImageData[uIdx+1] = GUIMin( 255, pImageData[uIdx+1] + s->coverage);
				}
			}
		}

		pCharData->m_pTexture->CopySubImage(m_uTexturePosX,m_uTexturePosY,imgWidth,imgHeight, bUseRGBATexture ? GUI_PF_RGBA_32 : GUI_PF_LUMINANCE_ALPHA_16,pImageData );

		delete [] pImageData;

		pCharData->m_aUV = CGUIRect(
			real(m_uTexturePosX) / nTextureWidth,
			real(m_uTexturePosY) / nTextureHeight,
			real(m_uTexturePosX+imgWidth) / nTextureWidth,
			real(m_uTexturePosY+imgHeight) / nTextureHeight);

		m_uTexturePosX += imgWidth;
		if( imgHeight > m_uMaxHeight )
		{
			m_uMaxHeight = imgHeight;
		}


		return pCharData;
	}
	//------------------------------------------------------------------------------
	SCharData_ft2* CGUIFontData_ft2::LoadCharDataWithoutStroke( FT_Face pFontFace, uint32 uGlyphIdx, wchar charCode )
	{
		uint16 uFontSize = GetFontSize();

		//Load the Glyph for our character.
		if( FT_Load_Glyph( pFontFace, uGlyphIdx, FT_LOAD_DEFAULT/*FT_LOAD_DEFAULT FT_LOAD_RENDER*/ ))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithoutStroke]:Failed to load glyph, the code is <%x>!", charCode ));
		}

		//Move the face's glyph into a Glyph object.
		FT_Glyph glyph;
		if(FT_Get_Glyph( pFontFace->glyph, &glyph ))
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontData_ft2::LoadCharDataWithoutStroke]:FT_Get_Glyph failed, the code is <%x>!", charCode ));
		}

		//Convert the glyph to a bitmap.
		FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		//This reference will make accessing the bitmap easier
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		//get information
		SCharData_ft2 *pCharData = new SCharData_ft2;	
		pCharData->m_fBitmapWidth = real(pFontFace->glyph->metrics.width >> 6);
		pCharData->m_fBitmapHeight = real(pFontFace->glyph->metrics.height >> 6);
		pCharData->m_fBearingX = real(pFontFace->glyph->metrics.horiBearingX >> 6);
		pCharData->m_fBearingY = real(pFontFace->glyph->metrics.horiBearingY >> 6);
		pCharData->m_aSize.m_fWidth = real(pFontFace->glyph->advance.x>>6);
		pCharData->m_aSize.m_fHeight = real(pFontFace->size->metrics.ascender>>6);
		//pCharData->m_aSize.m_fHeight = real(uFontSize);
		pCharData->m_nGlyphIdx = uGlyphIdx;

		//get texture
		uint32 nTextureWidth = GetTextureSize().GetWidth();
		uint32 nTextureHeight = GetTextureSize().GetHeight();
		if( m_uTexturePosX + bitmap.width > nTextureWidth)
		{
			m_uTexturePosX = 0;
			m_uTexturePosY += m_uMaxHeight;
			m_uMaxHeight = uFontSize;
		}
		if( m_vecTexture.empty() || m_uTexturePosY + bitmap.rows > nTextureHeight )
		{
			m_uTexturePosX = m_uTexturePosY = 0;
			m_uMaxHeight = uFontSize;
			CGUITexture* pNewTexture = CGUITextureManager::Instance()->CreateTexture(nTextureWidth,nTextureHeight,GUI_PF_LUMINANCE_ALPHA_16);
			m_vecTexture.push_back(pNewTexture);
		}
		pCharData->m_pTexture = m_vecTexture.back();

		//copy data
		//get image data
		uint8* pImageData = new uint8[bitmap.width * bitmap.rows * 2];
		uint8* pBufferDst = pImageData;
		uint8* pBufferSrc = bitmap.buffer;
		for( int i=0; i<bitmap.rows; ++i)
		{
			for( int j=0; j<bitmap.width; ++j)
			{
				pBufferDst[0] = 0xff;
				pBufferDst[1] = *pBufferSrc;
				++pBufferSrc;
				pBufferDst += 2;
			}	
		}
		pCharData->m_pTexture->CopySubImage(m_uTexturePosX,m_uTexturePosY,bitmap.width,bitmap.rows,GUI_PF_LUMINANCE_ALPHA_16,pImageData );
		delete[] pImageData;
		pCharData->m_aUV = CGUIRect(
			real(m_uTexturePosX) / nTextureWidth,
			real(m_uTexturePosY) / nTextureHeight,
			real(m_uTexturePosX+bitmap.width) / nTextureWidth,
			real(m_uTexturePosY+bitmap.rows) / nTextureHeight);

		m_uTexturePosX += bitmap.width;
		if( uint32(bitmap.rows) > m_uMaxHeight )
		{
			m_uMaxHeight = uint32(bitmap.rows);
		}

		FT_Done_Glyph(glyph);

		return pCharData;
	}
	//------------------------------------------------------------------------------
}//guiex

