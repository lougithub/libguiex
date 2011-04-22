/** 
 * @file guirender_dummy.cpp
 * @brief use freetype2 as font engine
 * @author ken
 * @date 2010-11-10
 */



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/font_dummy/guifont_dummy.h>
#include <libguiex_module/font_dummy/guifontdata_dummy.h>
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

#include <algorithm>


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
	GUI_INTERFACE_IMPLEMENT(IGUIFont_dummy);
	//------------------------------------------------------------------------------
	const char* IGUIFont_dummy::StaticGetModuleName()
	{
		return "IGUIFont_dummy";
	}
	//------------------------------------------------------------------------------
	IGUIFont_dummy::IGUIFont_dummy()
		:IGUIInterfaceFont( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUIFont_dummy::~IGUIFont_dummy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int IGUIFont_dummy::DoInitialize(void* )
	{
		//init datas
		m_arrayFontDatas.resize( GUI_FONT_MAX_NUM, NULL );

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void	IGUIFont_dummy::EnableKerning( bool bEnable )
	{
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DrawCharacter(
									   IGUIInterfaceRender* pRender, 
									   wchar_t charCode, 
									   const CGUIStringRenderInfo& rInfo,
									   const CGUIVector2& rPos,
									   real fAlpha)
	{
		CGUIFontData_dummy* pFontData = GetFontData( rInfo.m_uFontID );
		real fFontSize = pFontData->GetFontSize()*rInfo.m_fFontScale;
		CGUIRect aCharRect( CGUIVector2(rPos.x, rPos.y-fFontSize),
						   CGUISize(fFontSize,fFontSize));
		
		pRender->DrawRect( aCharRect, 1, 0,
						  rInfo.m_aColor,
						  rInfo.m_aColor,
						  rInfo.m_aColor,
						  rInfo.m_aColor);
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DrawString(IGUIInterfaceRender* pRender, 
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
		const CGUIStringRenderInfo& rInfo = rString.m_aStringInfo;

		CGUIFontData_dummy* pFontData = GetFontData( rInfo.m_uFontID );
		real fStringWidth = GetStringWidth(rString);
		real fFontSize = pFontData->GetFontSize()*rInfo.m_fFontScale;

		CGUIVector2 aPos;
		switch( uTextAlignmentHorz )
		{
		case eTextAlignment_Horz_Left:
			aPos.x = rStringRect.m_fLeft;
			break;
		case eTextAlignment_Horz_Right:
			aPos.x = rStringRect.m_fRight-fStringWidth;
			break;
		case eTextAlignment_Horz_Center:
		default:
			aPos.x = floor(rStringRect.m_fLeft+(rStringRect.GetWidth()-fStringWidth)/2);
			break;
		}

		switch( uTextAlignmentVert )
		{
		case eTextAlignment_Vert_Up:
			aPos.y = rStringRect.m_fTop;
			break;
		case eTextAlignment_Vert_Down:
			aPos.y = rStringRect.m_fBottom - fFontSize;
			break;
		case eTextAlignment_Vert_Center:
		default:
			aPos.y = ceil(rStringRect.m_fTop + (rStringRect.GetHeight() - fFontSize) / 2);
			break;
		}

		if( nEndPos < 0 || nEndPos >int32( rString.m_strContent.size()))
		{
			nEndPos = rString.m_strContent.size();
		}
		
		CGUISize aFontSize(fFontSize,fFontSize);
		for( int32 i= nStartPos; i<nEndPos; ++i)
		{
			
			CGUIRect aCharRect(
							   CGUIVector2(aPos.x, aPos.y-fFontSize),
							   aFontSize);
			
			//dest area size
			pRender->DrawRect( aCharRect, 1, 0,
							  rInfo.m_aColor,
							  rInfo.m_aColor,
							  rInfo.m_aColor,
							  rInfo.m_aColor);
			
			aPos.x+=aFontSize.m_fWidth;
		}
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DrawString(IGUIInterfaceRender* pRender, 
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

		
		if( nEndPos<0 || nEndPos>int32(rString.m_strContent.size()))
		{
			nEndPos = rString.m_strContent.size();
		}
		
		const CGUIStringRenderInfo& rInfo = rString.m_aStringInfo;
		CGUIFontData_dummy* pFontData = GetFontData( rInfo.m_uFontID );
		real fFontSize = pFontData->GetFontSize()*rInfo.m_fFontScale;
		CGUISize aFontSize(fFontSize,fFontSize);
		CGUIVector2 aPos = rPos;
	
		for( int32 i=nStartPos; i<nEndPos; ++i)
		{
			CGUIRect aCharRect(
							   CGUIVector2(aPos.x, aPos.y-fFontSize),
							   aFontSize);
			
			pRender->DrawRect( aCharRect, 1, 0,
							  rInfo.m_aColor,
							  rInfo.m_aColor,
							  rInfo.m_aColor,
							  rInfo.m_aColor);
			
			aPos.x+=aFontSize.m_fWidth;
		}
	}
	//------------------------------------------------------------------------------
	real IGUIFont_dummy::GetFontHeight( const CGUIStringRenderInfo& rInfo )
	{
		CGUIFontData_dummy* pFontData = GetFontData( rInfo.m_uFontID );
		return pFontData->GetFontSize() * rInfo.m_fFontScale;
	}
	//------------------------------------------------------------------------------
	CGUISize IGUIFont_dummy::GetCharacterSize( wchar_t charCode, const CGUIStringRenderInfo& rInfo )
	{
		CGUIFontData_dummy* pFontData = GetFontData( rInfo.m_uFontID );
		real fFontSize = pFontData->GetFontSize()*rInfo.m_fFontScale;
		return CGUISize(fFontSize,fFontSize);
	}
	//------------------------------------------------------------------------------
	CGUIFontData* IGUIFont_dummy::CreateFontData(
		const CGUIString& rName, 
		const CGUIString& rSceneName,
		const SFontInfo& rFontInfo)
	{
		//check
		if( rFontInfo.m_uID >= m_arrayFontDatas.size() ||
			m_arrayFontDatas[rFontInfo.m_uID] != NULL )
		{
			throw CGUIException("[IGUIFont_dummy::CreateFontData]: invalid font id <%d>", rFontInfo.m_uID );
			return NULL;
		}

		m_arrayFontDatas[rFontInfo.m_uID] = new CGUIFontData_dummy( rName, rSceneName, rFontInfo );
		return m_arrayFontDatas[rFontInfo.m_uID];
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DestroyFontData( CGUIFontData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );

		//check
		CGUIFontData_dummy* pFontData = m_arrayFontDatas[pData->GetFontID()];
		if( pFontData != pData )
		{
			throw CGUIException(
				"[IGUIFont_dummy::DestroyFontData]: the font<%s:%s> doesn't existing", 
				pData->GetName().c_str(),
				pData->GetSceneName().c_str());
			return;
		}

		//clear font
		m_arrayFontDatas[pData->GetFontID()] = NULL;
		delete pData;
	}
	//------------------------------------------------------------------------------
	CGUIFontData_dummy* IGUIFont_dummy::GetFontData( uint16 uFontID )
	{
		//check
		if( uFontID >= m_arrayFontDatas.size() ||
			m_arrayFontDatas[uFontID] == NULL )
		{
			throw CGUIException("[IGUIFont_dummy::GetFontData]: invalid font id <%d>", uFontID );
			return NULL;
		}
		return m_arrayFontDatas[uFontID];
	}
	//------------------------------------------------------------------------------

}//guiex

