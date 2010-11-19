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


#include <ft2build.h>
#include FT_FREETYPE_H

//============================================================================//
// define
//============================================================================// 
#define GUI_FT2_TEXTURE_SIZE	512

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CFontFace 
	*/
	class CFontFace
	{
	public:
		~CFontFace();

		FT_Face		m_aFtFace;

		typedef std::map<uint32, CGUIFont_ft2*> TMapSizeFont;
		TMapSizeFont	m_mapFont;
	};


	/**
	* @class CGUIFont_ft2 
	* @brief contain all size of font used by system
	*/
	class CGUIFont_ft2
	{
	public:
		/// constructor
		CGUIFont_ft2();
		~CGUIFont_ft2();


	public:
		typedef	std::vector<CGUITexture*>	TVecTexture;
		TVecTexture		m_vecTexture;
		uint32			m_nX,m_nY;	//start pos of current free texture area

		typedef std::map<wchar_t, CGUIFontData_ft2*> TMapFontData;
		TMapFontData	m_mapFontData;
	};


	/**
	* @class CGUIFontData_ft2 
	* @brief data of a specified font and specified size
	*/
	class  CGUIFontData_ft2
	{
	public:
		CGUIFontData_ft2();
		~CGUIFontData_ft2();

	public:
		CGUITexture*	m_pTexture;
		CGUIRect		m_aUV;


		uint32			m_nGlyphIdx;		//index of glyph
		int32			m_nBitmapWidth;		//size of bitmap
		int32			m_nBitmapHeight;	//size of bitmap
		int32			m_nLeftBearing;		//left bearing of font
		int32			m_nBottomBearing;	//bottom bearing of font
		int32			m_nTopBearing;		//top bearing of font
		CGUISize		m_aSize;			//size of font
	};	


}//namespace guiex


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIFont_ft2);
	//------------------------------------------------------------------------------
	CGUIFont_ft2::CGUIFont_ft2()
	{
		m_nX = m_nY = 0;
	}
	//------------------------------------------------------------------------------
	CGUIFont_ft2::~CGUIFont_ft2()
	{
		//release font data
		while(m_mapFontData.empty() == false)
		{
			delete m_mapFontData.begin()->second;
			m_mapFontData.erase(m_mapFontData.begin());
		}

		//clear texture
		while(!m_vecTexture.empty())
		{
			CGUITextureManager::Instance()->DestroyTexture(m_vecTexture.front());
			m_vecTexture.erase(m_vecTexture.begin());
		}
	}
	//------------------------------------------------------------------------------
	CFontFace::~CFontFace()
	{
		while( m_mapFont.empty() == false)
		{
			delete m_mapFont.begin()->second;
			m_mapFont.erase(m_mapFont.begin());
		}
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::CGUIFontData_ft2()
		:m_pTexture(NULL)
		,m_nLeftBearing(0)
		,m_nBottomBearing(0)
	{
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2::~CGUIFontData_ft2()
	{
		m_pTexture = NULL;
	}
	//------------------------------------------------------------------------------





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
	void	IGUIFont_ft2::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int IGUIFont_ft2::DoInitialize(void* )
	{
		//initialize library
		if ( FT_Init_FreeType( (FT_Library*)(&m_pFtLib )))
		{
			throw CGUIException("[IGUIFont_ft2::DoInitialize]:Could not init FreeType library!");
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DoDestroy()
	{
		ReleaseAllResource();

		//free library
		if(m_pFtLib)
		{
			FT_Done_FreeType(FT_Library(m_pFtLib));
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIFont_ft2::ReleaseAllResource()
	{
		//free font face
		while( m_mapFace.empty() == false )
		{
			UnloadFontFace(m_mapFace.begin()->first);
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIFont_ft2::EnableKerning( bool bEnable )
	{
		m_bEnableKerning = bEnable;
	}
	//------------------------------------------------------------------------------
	int32	IGUIFont_ft2::LoadFontFace( const CGUIString& rFilePathName, int32 nFontFaceIdx )
	{
		GUI_TRACE(GUI_FORMAT( "[IGUIFont_ft2::LoadFontFace]:\n   Load Font Face <%d> from File <%s>",
			nFontFaceIdx,rFilePathName.c_str()));

		if( m_mapFace.find(nFontFaceIdx) != m_mapFace.end())
		{
			throw CGUIException("[IGUIFont_ft2::LoadFontFace]:The Font <%d> has been loaded!", nFontFaceIdx );
		}

		//load
#if 1
		FT_Face aFace;
		CGUIString strFullPath = GUI_WIDGET_SYSTEM()->GetDataPath() + rFilePathName;
		FT_Error ret = FT_New_Face( FT_Library(m_pFtLib),strFullPath.c_str(), 0, &aFace );
		if(  ret != 0 )
		{
			throw CGUIException("[IGUIFont_ft2::LoadFontFace]:Could not get font face from file <%s>!",
				rFilePathName.c_str());
		}
#else
		IGUIInterfaceFileSys* pFileSys = GUI_GET_INTERFACE(IGUIInterfaceFileSys, "IGUIFileSys");
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFilePathName, aDataChunk ))
		{
			throw CGUIException("[IGUIFont_ft2::LoadFontFace]:Could not get font face from file <%s>!",
				rFilePathName.c_str());
		}
		FT_Face aFace;
		FT_Error ret = FT_New_Memory_Face( FT_Library(m_pFtLib),aDataChunk.GetDataPtr(),aDataChunk.GetSize(), 0, &aFace );
		if(  ret != 0 )
		{
			throw CGUIException("[IGUIFont_ft2::LoadFontFace]:Could not get font face from file <%s>!",
				rFilePathName.c_str());
		}
#endif

		//add to map
		CFontFace* pFace = new CFontFace;
		pFace->m_aFtFace = aFace;
		m_mapFace.insert(std::make_pair(nFontFaceIdx, pFace));

		return 0;
	}
	//------------------------------------------------------------------------------
	int32		IGUIFont_ft2::UnloadFontFace( int32 nFontFaceIdx )
	{
		GUI_TRACE(GUI_FORMAT( "[IGUIFont_ft2::UnloadFontFace]:\n   Unload Font Face <%d>",
			nFontFaceIdx));

		TMapFace::iterator itor = m_mapFace.find(nFontFaceIdx);
		if( itor == m_mapFace.end())
		{
			//throw CGUIException("[IGUIFont_ft2::UnloadFontFace]:The Font <%d> hasn't been loaded!", nFontFaceIdx );
			return 0;
		}

		//unload
		FT_Done_Face( itor->second->m_aFtFace );
		delete itor->second;
		m_mapFace.erase(itor);

		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2* IGUIFont_ft2::LoadFont(CFontFace* pFontFace,CGUIFont_ft2* pFont,wchar_t charCode,uint32 nSize)
	{
		if( charCode == L'\n')
		{
			//line break
			CGUIFontData_ft2 *pFontData = new CGUIFontData_ft2;	
			pFontData->m_pTexture = NULL;
			pFontData->m_nBitmapWidth = 0;
			pFontData->m_nBitmapHeight = 0;
			pFontData->m_nLeftBearing = 0;
			pFontData->m_nBottomBearing = 0;
			pFontData->m_nTopBearing = 0;
			pFontData->m_aSize.m_fWidth = 0;
			pFontData->m_aSize.m_fHeight = real(nSize);
			pFontData->m_nGlyphIdx = 0;

			//add to map
			pFont->m_mapFontData.insert(std::make_pair(charCode, pFontData));

			return pFontData;
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
			CGUIFontData_ft2 *pFontData = new CGUIFontData_ft2;	
			pFontData->m_nBitmapWidth = bitmap.width;
			pFontData->m_nBitmapHeight = bitmap.rows;
			int textureWidth = CGUITexture::ConvertToTextureSize(pFontData->m_nBitmapWidth);
			int textureHeight = CGUITexture::ConvertToTextureSize(pFontData->m_nBitmapHeight);
			pFontData->m_nLeftBearing = slot->bitmap_left;
			pFontData->m_nBottomBearing = -( bitmap.rows - slot->bitmap_top );
			pFontData->m_nTopBearing = slot->bitmap_top;
			pFontData->m_aSize.m_fWidth = real(slot->advance.x>>6);
			pFontData->m_aSize.m_fHeight = real(nSize);
			pFontData->m_nGlyphIdx = uGlyphIdx;

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
			pFontData->m_pTexture = pFont->m_vecTexture.back();

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
			pFontData->m_pTexture->CopySubImage(pFont->m_nX,pFont->m_nY,bitmap.width,bitmap.rows,GUI_PF_ARGB_32,pImageData );
			delete[] pImageData;
			pFontData->m_aUV = CGUIRect(
				real(pFont->m_nX) / GUI_FT2_TEXTURE_SIZE,
				real(pFont->m_nY) / GUI_FT2_TEXTURE_SIZE,
				real(pFont->m_nX+bitmap.width) / GUI_FT2_TEXTURE_SIZE,
				real(pFont->m_nY+bitmap.rows) / GUI_FT2_TEXTURE_SIZE);

			pFont->m_nX += bitmap.width;


			//add to map
			pFont->m_mapFontData.insert(std::make_pair(charCode, pFontData));

			return pFontData;
		}

	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2* IGUIFont_ft2::GetFont( int32 nFontFaceIdx, wchar_t charCode,uint32 nSize )
	{ 
		//get font face
		TMapFace::iterator itor = m_mapFace.find( nFontFaceIdx );
		if( itor == m_mapFace.end())
		{
			throw CGUIException("[IGUIFont_ft2::GetFont]:The Font which index is <%d> hasn't been loaded!", nFontFaceIdx );
		}

		//get font
		CGUIFont_ft2* pFont = NULL;
		CFontFace* pFontFace = itor->second;
		CFontFace::TMapSizeFont::iterator itorFont =  pFontFace->m_mapFont.find(nSize);
		if(itorFont == pFontFace->m_mapFont.end())
		{
			//this font hasn't been loaded
			pFont = new CGUIFont_ft2;

			//add to map
			pFontFace->m_mapFont.insert(std::make_pair(nSize, pFont));
		}
		else
		{
			//has load this font
			pFont = itorFont->second;
		}

		//get font
		CGUIFont_ft2::TMapFontData::iterator itorFontData = pFont->m_mapFontData.find(charCode);
		if( itorFontData != pFont->m_mapFontData.end())
		{
			//bingo! got this font data
			return itorFontData->second;
		}
		else
		{
			return LoadFont(pFontFace, pFont,charCode,nSize );
		}
	}
	//------------------------------------------------------------------------------
	int32	IGUIFont_ft2::GetKerningGap(int32 nFontFaceIdx, 
		CGUIFontData_ft2* pLeftData, 
		CGUIFontData_ft2* pRightData,
		uint32 nSize)
	{
		if( !pLeftData || !pRightData)
		{
			return 0;
		}

		//get font face
		TMapFace::iterator itor = m_mapFace.find( nFontFaceIdx );
		if( itor == m_mapFace.end())
		{
			throw CGUIException("[IGUIFont_ft2::GetKerningGap]:The Font which index is <%d> hasn't been loaded!", nFontFaceIdx );
		}

		//set size
		if( FT_Set_Pixel_Sizes( itor->second->m_aFtFace, nSize, nSize) )
		{
			throw CGUIException("[IGUIFont_ft2::GetKerningGap]:Failed to set pixel size!");
		}

		FT_Vector  kerning;
		if( FT_Get_Kerning( itor->second->m_aFtFace, pLeftData->m_nGlyphIdx,pRightData->m_nGlyphIdx,FT_KERNING_DEFAULT , &kerning ))
		{
			throw CGUIException("[IGUIFont_ft2::GetKerningGap]:Failed to get kerning!");
		}

		return kerning.x>>6;

	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawCharacter(IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		wchar_t charCode, 
		const CGUIStringInfo& rInfo,
		const CGUIVector2& rPos,
		real	fAlpha)
	{
		CGUIFontData_ft2* pFontData = GetFont( rInfo.m_nFontIdx, charCode, rInfo.m_nFontSize);

		if( pFontData->m_pTexture)
		{
			CGUIRect aCharRect(
				CGUIVector2(rPos.x+real(pFontData->m_nLeftBearing), rPos.y+real(rInfo.m_nFontSize-pFontData->m_nTopBearing)),
				CGUISize(real(pFontData->m_nBitmapWidth),real(pFontData->m_nBitmapHeight)));

			CGUIColor aColor(rInfo.m_aColor);
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( 
				rWorldMatrix,
				aCharRect,
				pRender->GetAndIncZ(), 
				pFontData->m_pTexture->GetTextureImplement(),
				pFontData->m_aUV, 
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

			CGUIFontData_ft2 * pFontData = GetFont(rInfo.m_nFontIdx, rString.m_strContent[i], rInfo.m_nFontSize);

			if( pFontData->m_pTexture)
			{
				CGUIRect aCharRect(
					CGUIVector2(aPos.x+real(pFontData->m_nLeftBearing), aPos.y+real(rInfo.m_nFontSize-pFontData->m_nTopBearing)),
					CGUISize(real(pFontData->m_nBitmapWidth),real(pFontData->m_nBitmapHeight)));

				//dest area size
				CGUIColor aColor(rInfo.m_aColor);
				aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
				pRender->DrawTile( 
					rWorldMatrix,
					aCharRect,
					pRender->GetAndIncZ(), 
					pFontData->m_pTexture->GetTextureImplement(),
					pFontData->m_aUV, 
					eImageOrientation_Normal,
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB());
			}

			aPos.x+=pFontData->m_aSize.m_fWidth;
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
			CGUIFontData_ft2 * pFontData = GetFont(rInfo.m_nFontIdx, rString.m_strContent[i], rInfo.m_nFontSize);

			if( pFontData->m_pTexture)
			{
				CGUIRect aCharRect(
					CGUIVector2(aPos.x+real(pFontData->m_nLeftBearing), aPos.y+real(rInfo.m_nFontSize-pFontData->m_nTopBearing)),
					CGUISize(real(pFontData->m_nBitmapWidth),real(pFontData->m_nBitmapHeight)));

				//dest area size
				CGUIColor aColor(rInfo.m_aColor);
				aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
				pRender->DrawTile( 
					rWorldMatrix,
					aCharRect,
					pRender->GetAndIncZ(), 
					pFontData->m_pTexture->GetTextureImplement(),
					pFontData->m_aUV, 
					eImageOrientation_Normal,
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB(),
					aColor.GetARGB());
			}

			aPos.x+=pFontData->m_aSize.m_fWidth;
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& IGUIFont_ft2::GetCharacterSize(int32 nFontFaceIdx,wchar_t charCode,uint32 nSize)
	{
		CGUIFontData_ft2* pFontData = GetFont( nFontFaceIdx, charCode, nSize);

		return pFontData->m_aSize;
	}
	//------------------------------------------------------------------------------

}//guiex

