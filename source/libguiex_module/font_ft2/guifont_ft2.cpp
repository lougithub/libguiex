/** 
* @file guifont_ft2.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2006-02-06
*/



//============================================================================//
// include
//============================================================================// 
#include "guifont_ft2.h"
#include "guifontdata_ft2.h"
#include "guifontface_ft2.h"
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
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiperfmonitor.h>
#include <libguiex_core/guiscene.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guishader.h>

#include <algorithm>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIFont_ft2);
	//------------------------------------------------------------------------------
	const char* IGUIFont_ft2::StaticGetModuleName()
	{
		return "IGUIFont_ft2";
	}
	//------------------------------------------------------------------------------
	IGUIFont_ft2::IGUIFont_ft2()
		:IGUIInterfaceFont( StaticGetModuleName() )
		,m_pFtLib(NULL)
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
			GUI_THROW( "[IGUIFont_ft2::DoInitialize]:Could not init FreeType library!");
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
	void IGUIFont_ft2::EnableKerning( bool bEnable )
	{
		m_bEnableKerning = bEnable;
	}
	//------------------------------------------------------------------------------
	int32	IGUIFont_ft2::GetKerningGap(
		uint16 uFontID, 
		SCharData_ft2* pLeftData, 
		SCharData_ft2* pRightData,
		uint32 nSize)
	{
		if( !pLeftData || !pRightData)
		{
			return 0;
		}

		//get font face
		GUI_ASSERT( uFontID < m_arrayFontDatas.size(), "invalid font face index" );
		CGUIFontData_ft2* pFontData = m_arrayFontDatas[uFontID];
		if( !pFontData )
		{
			GUI_THROW( GUI_FORMAT("[IGUIFont_ft2::GetKerningGap]:The Font which index is <%d> hasn't been loaded!", uFontID ));
		}
		pFontData->Load();

		//set size
		if( FT_Set_Pixel_Sizes( pFontData->GetFontFace(), nSize, nSize) )
		{
			GUI_THROW( "[IGUIFont_ft2::GetKerningGap]:Failed to set pixel size!");
		}

		FT_Vector  kerning;
		if( FT_Get_Kerning( pFontData->GetFontFace(), pLeftData->m_nGlyphIdx,pRightData->m_nGlyphIdx,FT_KERNING_DEFAULT , &kerning ))
		{
			GUI_THROW( "[IGUIFont_ft2::GetKerningGap]:Failed to get kerning!");
		}

		return kerning.x>>6;
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawCharacter(
		IGUIInterfaceRender* pRender, 
		wchar charCode, 
		const CGUIStringRenderInfo& rInfo,
		const CGUIVector2& rPos,
		real fAlpha)
	{
		//apply shader
		CGUIShader* pOldShader = NULL;
		if( GSystem->GetDefaultShader_Font() )
		{
			pOldShader = GSystem->GetDefaultShader_Font()->Use(pRender);
		}

		CGUIFontData_ft2* pFontData = GetFontData( rInfo.m_uFontID );
		SCharData_ft2* pCharData = pFontData->GetCharData( charCode );

		if( pCharData->m_pTexture)
		{
			CGUIRect aCharRect(
				CGUIVector2(rPos.x+pCharData->m_fBearingX*rInfo.m_fFontScale, rPos.y-pCharData->m_fBearingY*rInfo.m_fFontScale),
				CGUISize(pCharData->m_fBitmapWidth*rInfo.m_fFontScale,pCharData->m_fBitmapHeight*rInfo.m_fFontScale));

			CGUIColor aColor(rInfo.m_aColor);
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( 
				aCharRect,
				0, 
				pCharData->m_pTexture,
				pCharData->m_aUV, 
				eImageOrientation_Normal,
				aColor,
				aColor,
				aColor,
				aColor);
		}

		//restore shader
		if( pOldShader )
		{
			pOldShader->Use( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawString(IGUIInterfaceRender* pRender, 
		const CGUIStringRender& rString, 
		const CGUIRect&	rStringRect,
		ETextAlignmentHorz uTextAlignmentHorz,
		ETextAlignmentVert uTextAlignmentVert,
		real fAlpha,
		int32 nStartPos,
		int32 nEndPos)
	{
		if( rString.m_strContent.empty())
		{
			//empty string
			return;
		}

		//apply shader
		CGUIShader* pOldShader = NULL;
		if( GSystem->GetDefaultShader_Font() )
		{
			pOldShader = GSystem->GetDefaultShader_Font()->Use(pRender);
		}

		const CGUIStringRenderInfo& rInfo = rString.m_aStringInfo;
		CGUIFontData_ft2* pFontData = GetFontData( rInfo.m_uFontID );
		real fScaledStringWidth = GetStringWidth(rString);
		real fScaledStringHeight = pFontData->GetFontSize() * rInfo.m_fFontScale;

		CGUIVector2 aPos;
		switch( uTextAlignmentHorz )
		{
		case eTextAlignment_Horz_Left:
			aPos.x = rStringRect.m_fLeft;
			break;
		case eTextAlignment_Horz_Right:
			aPos.x = rStringRect.m_fRight-fScaledStringWidth;
			break;
		case eTextAlignment_Horz_Center:
		default:
			aPos.x = floor(rStringRect.m_fLeft+(rStringRect.GetWidth()-fScaledStringWidth)/2);
			break;
		}

		switch( uTextAlignmentVert )
		{
		case eTextAlignment_Vert_Up:
			aPos.y = rStringRect.m_fTop;
			break;
		case eTextAlignment_Vert_Down:
			aPos.y = rStringRect.m_fBottom - fScaledStringHeight;
			break;
		case eTextAlignment_Vert_Center:
		default:
			aPos.y = ceil(rStringRect.m_fTop + (rStringRect.GetHeight() - fScaledStringHeight) / 2);
			break;
		}
		aPos.y += fScaledStringHeight;

		if( nEndPos < 0 || nEndPos >int32( rString.m_strContent.size()))
		{
			nEndPos = rString.m_strContent.size();
		}

		for( int32 i= nStartPos; i<nEndPos; ++i)
		{
			SCharData_ft2 * pCharData = pFontData->GetCharData( rString.m_strContent[i] );

			if( pCharData->m_pTexture)
			{
				CGUIRect aCharRect(
					CGUIVector2(aPos.x+pCharData->m_fBearingX*rInfo.m_fFontScale, aPos.y-pCharData->m_fBearingY*rInfo.m_fFontScale),
					CGUISize(pCharData->m_fBitmapWidth*rInfo.m_fFontScale,pCharData->m_fBitmapHeight*rInfo.m_fFontScale));

				//dest area size
				CGUIColor aColor(rInfo.m_aColor);
				aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
				pRender->DrawTile( 
					aCharRect,
					0, 
					pCharData->m_pTexture,
					pCharData->m_aUV, 
					eImageOrientation_Normal,
					aColor,
					aColor,
					aColor,
					aColor);
			}

			aPos.x+=pCharData->m_aSize.m_fWidth*rInfo.m_fFontScale;
		}

		//restore shader
		if( pOldShader )
		{
			pOldShader->Use( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DrawString(IGUIInterfaceRender* pRender, 
		const CGUIStringRender& rString, 
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

		//apply shader
		CGUIShader* pOldShader = NULL;
		if( GSystem->GetDefaultShader_Font() )
		{
			pOldShader = GSystem->GetDefaultShader_Font()->Use(pRender);
		}

		CGUIVector2 aPos = rPos;

		if( nEndPos<0 || nEndPos>int32(rString.m_strContent.size()))
		{
			nEndPos = rString.m_strContent.size();
		}

		const CGUIStringRenderInfo& rInfo = rString.m_aStringInfo;
		CGUIFontData_ft2* pFontData = GetFontData( rInfo.m_uFontID );

		for( int32 i=nStartPos; i<nEndPos; ++i)
		{
			SCharData_ft2 * pCharData = pFontData->GetCharData( rString.m_strContent[i] );

			if( pCharData->m_pTexture)
			{
				CGUIRect aCharRect(
					CGUIVector2(aPos.x+pCharData->m_fBearingX*rInfo.m_fFontScale, aPos.y-pCharData->m_fBearingY*rInfo.m_fFontScale),
					CGUISize(pCharData->m_fBitmapWidth*rInfo.m_fFontScale,pCharData->m_fBitmapHeight*rInfo.m_fFontScale));

				//dest area size
				CGUIColor aColor(rInfo.m_aColor);
				aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
				pRender->DrawTile( 
					aCharRect,
					0, 
					pCharData->m_pTexture,
					pCharData->m_aUV, 
					eImageOrientation_Normal,
					aColor,
					aColor,
					aColor,
					aColor);
			}

			aPos.x+=pCharData->m_aSize.m_fWidth*rInfo.m_fFontScale;
		}

		//restore shader
		if( pOldShader )
		{
			pOldShader->Use( pRender );
		}
	}
	//------------------------------------------------------------------------------
	real IGUIFont_ft2::GetFontHeight( const CGUIStringRenderInfo& rInfo )
	{
		CGUIFontData_ft2* pFontData = GetFontData( rInfo.m_uFontID );
		return pFontData->GetFontSize() * rInfo.m_fFontScale;
	}
	//------------------------------------------------------------------------------
	CGUISize IGUIFont_ft2::GetCharacterSize( wchar charCode, const CGUIStringRenderInfo& rInfo )
	{
		CGUIFontData_ft2* pFontData = GetFontData( rInfo.m_uFontID );
		SCharData_ft2 * pCharData = pFontData->GetCharData( charCode );
		return pCharData->m_aSize * rInfo.m_fFontScale;
	}
	//------------------------------------------------------------------------------
	CGUIFontData* IGUIFont_ft2::CreateFontData( 
		const CGUIString& rName, 
		const CGUIString& rSceneName,
		const SFontInfo& rFontInfo )
	{
		//check
		if( rFontInfo.m_uID >= m_arrayFontDatas.size() ||
			m_arrayFontDatas[rFontInfo.m_uID] != NULL )
		{
			GUI_THROW( GUI_FORMAT("[IGUIFont_ft2::CreateFontData]: invalid font id <%d>", rFontInfo.m_uID ));
			return NULL;
		}

		//create font face
		CGUIFontFace_ft2* pFontFace = NULL;
		CGUIString strFullFontPath = CGUISceneManager::Instance()->GetScenePath( rSceneName ) + rFontInfo.m_strPath;
		TMapFontFace::iterator itor = m_mapFontFace.find( rFontInfo.m_strPath );
		if( itor == m_mapFontFace.end() )
		{
			pFontFace = new CGUIFontFace_ft2( this, strFullFontPath );
			pFontFace->RefRetain();
			m_mapFontFace.insert( std::make_pair( rFontInfo.m_strPath, pFontFace ) );
		}
		else
		{
			pFontFace = itor->second;
			pFontFace->RefRetain();
		}

		m_arrayFontDatas[rFontInfo.m_uID] = new CGUIFontData_ft2( rName, rSceneName, rFontInfo, pFontFace );
		return m_arrayFontDatas[rFontInfo.m_uID];
	}
	//------------------------------------------------------------------------------
	void IGUIFont_ft2::DestroyFontData( CGUIFontData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );

		//check
		CGUIFontData_ft2* pFontData = m_arrayFontDatas[pData->GetFontID()];
		if( pFontData != pData )
		{
			GUI_THROW( GUI_FORMAT(
				"[IGUIFont_ft2::DestroyFontData]: the font<%s:%s> doesn't existing", 
				pData->GetName().c_str(),
				pData->GetSceneName().c_str()));
			return;
		}

		//update font face
		TMapFontFace::iterator itor = m_mapFontFace.find( pData->GetFontPath() );
		if( itor == m_mapFontFace.end() )
		{
			GUI_THROW( GUI_FORMAT(
				"[IGUIFont_ft2::DestroyFontData]: failed to font font face by path <%s>", 
				pData->GetFontPath().c_str()));
			return;
		}
		itor->second->RefRelease();
		//delete font face
		if( itor->second->GetRefCount() == 0 )
		{
			delete itor->second;
			m_mapFontFace.erase( itor );
		}

		//clear font
		m_arrayFontDatas[pData->GetFontID()] = NULL;
		delete pData;
	}
	//------------------------------------------------------------------------------
	CGUIFontData_ft2* IGUIFont_ft2::GetFontData( uint16 uFontID )
	{
		//check
		if( uFontID >= m_arrayFontDatas.size() ||
			m_arrayFontDatas[uFontID] == NULL )
		{
			GUI_THROW( GUI_FORMAT("[IGUIFont_ft2::GetFontData]: invalid font id <%d>", uFontID ));
			return NULL;
		}
		return m_arrayFontDatas[uFontID];
	}
	//------------------------------------------------------------------------------
}//guiex

