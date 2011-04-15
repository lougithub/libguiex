/** 
* @file guiwgtemptynode.cpp
* @brief empty node, used to manage widget
* for example, used to group radio button
* @author ken
* @date 2006-08-03
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgttag.h>
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
		
		SetAnchorPoint( 0.5f, 0.5f );
		SetSize( 5.0f, 5.0f );
		SetFocusable(false);
		SetActivable(false);
		
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
			throw CGUIException( "[CGUIWgtTag::Update]: CGUIWgtTag shouldn't has any child!" );
		}
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIWgtTag::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
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

