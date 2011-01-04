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
									   const CGUIMatrix4& rWorldMatrix,
									   wchar_t charCode, 
									   const CGUIStringInfo& rInfo,
									   const CGUIVector2& rPos,
									   real fAlpha)
	{
		CGUIRect aCharRect( CGUIVector2(rPos.x, rPos.y),
						   CGUISize(rInfo.m_nFontSize,rInfo.m_nFontSize));
		
		pRender->DrawRect( rWorldMatrix, aCharRect, 1, pRender->GetAndIncZ(),
						  rInfo.m_aColor.GetARGB(),
						  rInfo.m_aColor.GetARGB(),
						  rInfo.m_aColor.GetARGB(),
						  rInfo.m_aColor.GetARGB());
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DrawString(IGUIInterfaceRender* pRender, 
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
			
			CGUIRect aCharRect(
							   CGUIVector2(aPos.x, aPos.y+real(rInfo.m_nFontSize)),
							   CGUISize(rInfo.m_nFontSize,rInfo.m_nFontSize));
			
			//dest area size
			pRender->DrawRect( rWorldMatrix, aCharRect, 1, pRender->GetAndIncZ(),
							  rInfo.m_aColor.GetARGB(),
							  rInfo.m_aColor.GetARGB(),
							  rInfo.m_aColor.GetARGB(),
							  rInfo.m_aColor.GetARGB());
			
			aPos.x+=rInfo.m_nFontSize;
		}
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DrawString(IGUIInterfaceRender* pRender, 
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
			CGUIRect aCharRect(
							   CGUIVector2(aPos.x, aPos.y+rInfo.m_nFontSize),
							   CGUISize(rInfo.m_nFontSize,rInfo.m_nFontSize));
			
			pRender->DrawRect( rWorldMatrix, aCharRect, 1, pRender->GetAndIncZ(),
							  rInfo.m_aColor.GetARGB(),
							  rInfo.m_aColor.GetARGB(),
							  rInfo.m_aColor.GetARGB(),
							  rInfo.m_aColor.GetARGB());
			
			aPos.x+=rInfo.m_nFontSize;
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& IGUIFont_dummy::GetCharacterSize(int32 nFontFaceIdx,wchar_t charCode,uint32 nSize)
	{
		static CGUISize aSize;
		aSize.SetValue( real( nSize), real(nSize ));
		return aSize;
	}
	//------------------------------------------------------------------------------
	CGUIFontData* IGUIFont_dummy::CreateFontData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontID )
	{
		return new CGUIFontData_dummy(rName, rSceneName, rPath, nFontID);
	}
	//------------------------------------------------------------------------------
	void IGUIFont_dummy::DestroyFontData( CGUIFontData* pData )
	{
		delete pData;
	}
	//------------------------------------------------------------------------------
}//guiex

