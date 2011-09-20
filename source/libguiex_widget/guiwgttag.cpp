/** 
 * @file guiwgttag.cpp
 * @brief used to get position info
 * @author ken
 * @date 2011-04-15
 */


//============================================================================//
// include 
//============================================================================// 
#include "guiwgttag.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtTag);
	//------------------------------------------------------------------------------
	CGUIWgtTag::CGUIWgtTag(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitTag();
	}
	//------------------------------------------------------------------------------
	CGUIWgtTag::CGUIWgtTag( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitTag();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTag::InitTag()
	{
		m_pImageBG = NULL;
		
		SetMouseConsumed(false);
		SetHitable( false );				
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTag::Render( IGUIInterfaceRender* pRender )
	{
		if( GSystem->IsEditorMode() )
		{
			CGUIWidget::Render( pRender );
		}
	}
	//------------------------------------------------------------------------------	
	void CGUIWgtTag::RenderExtraInfo(IGUIInterfaceRender* pRender)
	{
		if( GSystem->IsEditorMode() )
		{
			CGUIWidget::RenderExtraInfo( pRender );
		}		
	}
	//------------------------------------------------------------------------------	
	void CGUIWgtTag::Update( real fDeltaTime )
	{
#if GUI_DEBUG
		if( GetChild() )
		{
			GUI_THROW( "[CGUIWgtTag::Update]: CGUIWgtTag shouldn't has any child!" );
		}
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTag::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "bg")
		{
			m_pImageBG = pImage;
			if( GetSize().IsEqualZero() && m_pImageBG )
			{
				SetPixelSize(m_pImageBG->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTag::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetBoundArea( ));
	}
	//------------------------------------------------------------------------------
	
	
}//namespace guiex

