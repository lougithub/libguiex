/** 
* @file guiwidgetscrollbarcontainer.cpp
* @brief widget: scrollbar container
* @author ken
* @date 2007-05-18
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtscrollbarcontainer.h>
#include <libguiex_widget/guiwgtscrollbar.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtScrollbarContainer);
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarContainer::CGUIWgtScrollbarContainer(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitScrollbarContainer();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarContainer::CGUIWgtScrollbarContainer( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitScrollbarContainer();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarContainer::~CGUIWgtScrollbarContainer(  )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::InitScrollbarContainer()
	{
		m_bForceVertScroll = false;
		m_bForceHorzScroll = false;

		//create scrollbar
		m_pScrollbarVert = static_cast<CGUIWgtScrollbar*>(GUI_CREATE_WIDGET("CGUIWgtScrollbar", GetName()+"_scrollbar_vert" + GUI_INTERNAL_WIDGET_FLAG, GetSceneName()));
		m_pScrollbarVert->SetParent(this);
		m_pScrollbarVert->SetFocusAgency( true );
		m_pScrollbarVert->EnableNotifyParent(true);
		m_pScrollbarVert->EnableAutoPosition(true);
		m_pScrollbarVert->SetScrollbarType(eSB_VERTICAL);

		m_pScrollbarHorz = static_cast<CGUIWgtScrollbar*>(GUI_CREATE_WIDGET("CGUIWgtScrollbar", GetName()+"_scrollbar_hort" + GUI_INTERNAL_WIDGET_FLAG, GetSceneName()));
		m_pScrollbarHorz->SetParent(this);
		m_pScrollbarHorz->SetFocusAgency( true );
		m_pScrollbarHorz->EnableNotifyParent(true);
		m_pScrollbarHorz->EnableAutoPosition(true);
		m_pScrollbarHorz->SetScrollbarType(eSB_HORIZONAL);

		//set flag
		SetFocusable(true);
		SetGenerateClickEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::Create()
	{
		//create scrollbar
		m_pScrollbarHorz->Create();
		m_pScrollbarVert->Create();

		CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtScrollbarContainer::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "SCROLLBAR_VERTIC_BG")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_BG", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_NORMAL_0")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_NORMAL_0", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_HOVER_0")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_HOVER_0", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_DISABLE_0")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_DISABLE_0", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_PUSH_0")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_PUSH_0", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_NORMAL_1")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_NORMAL_1", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_HOVER_1")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_HOVER_1", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_DISABLE_1")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_DISABLE_1", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_ARROW_PUSH_1")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_ARROW_PUSH_1", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_SLIDE_NORMAL")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_SLIDE_NORMAL", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_SLIDE_HOVER")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_SLIDE_HOVER", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_SLIDE_DISABLE")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_SLIDE_DISABLE", pImage);
		}
		else if( rName == "SCROLLBAR_VERTIC_SLIDE_PUSH")
		{
			m_pScrollbarVert->SetImage("SCROLLBAR_SLIDE_PUSH", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_BG")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_BG", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_NORMAL_0")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_NORMAL_0", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_HOVER_0")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_HOVER_0", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_DISABLE_0")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_DISABLE_0", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_PUSH_0")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_PUSH_0", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_NORMAL_1")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_NORMAL_1", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_HOVER_1")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_HOVER_1", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_DISABLE_1")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_DISABLE_1", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_ARROW_PUSH_1")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_ARROW_PUSH_1", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_SLIDE_NORMAL")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_SLIDE_NORMAL", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_SLIDE_HOVER")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_SLIDE_HOVER", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_SLIDE_DISABLE")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_SLIDE_DISABLE", pImage);
		}
		else if( rName == "SCROLLBAR_HORIZON_SLIDE_PUSH")
		{
			m_pScrollbarHorz->SetImage("SCROLLBAR_SLIDE_PUSH", pImage);
		}
		else
		{
		}

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::RenderSelf(IGUIInterfaceRender* pRender)
	{
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtScrollbarContainer::IsVertScrollbarAlwaysShown(void) const
	{
		return m_bForceVertScroll;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::ForceVertScrollbar(bool bSetting)
	{
		if (m_bForceVertScroll != bSetting)
		{
			m_bForceVertScroll = bSetting;
			UpdateScrollbars();
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtScrollbarContainer::IsHorzScrollbarAlwaysShown(void) const
	{
		return m_bForceHorzScroll;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::ForceHorzScrollbar(bool bSetting)
	{
		if (m_bForceHorzScroll != bSetting)
		{
			m_bForceHorzScroll = bSetting;
			UpdateScrollbars();
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		UpdateScrollbars();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::UpdateClientArea(void)
	{
		//calculate client and clip area
		/*
		* usage:	GetClientArea(): the area where the client widget can render, maybe 
		*			bigger than container bounds.
		*			GetClipArea(): the area where the client can show, 
		*			clip)area = bound_area - scrollbar_area
		*/
		//client area size
		CGUISize aClientAreaSize = GetBoundArea().GetSize();
		real fVertScrollbarWidth = m_pScrollbarVert->GetPixelSize().GetWidth();
		if( aClientAreaSize.GetWidth() < fVertScrollbarWidth )
		{
			aClientAreaSize.SetWidth( 0.0f );
		}
		else
		{
			aClientAreaSize.SetWidth(aClientAreaSize.GetWidth() - fVertScrollbarWidth);
		}
		real fHorzScrollbarHeight = m_pScrollbarHorz->GetPixelSize().GetHeight();
		if( aClientAreaSize.GetHeight() < fHorzScrollbarHeight )
		{
			aClientAreaSize.SetHeight( 0.0f );
		}
		else
		{
			aClientAreaSize.SetHeight( aClientAreaSize.GetHeight() - fHorzScrollbarHeight );
		}

		//client area position
		CGUIVector2 aClientAreaPos = GetBoundArea().GetPosition();
		aClientAreaPos.x -=  m_pScrollbarHorz->GetCurrentPos();
		aClientAreaPos.y -= m_pScrollbarVert->GetCurrentPos();

		m_aClipArea.SetSize( aClientAreaSize );
		m_aClientArea.SetPosition( aClientAreaPos );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::UpdateScrollbars(void)
	{
		UpdateClientArea();

		//update scrollbar's size
		m_pScrollbarVert->SetPixelSize(
			m_pScrollbarVert->GetPixelSize().GetWidth(),
			GetPixelSize().GetHeight());
		m_pScrollbarHorz->SetPixelSize(
			GetPixelSize().GetWidth(),
			m_pScrollbarHorz->GetPixelSize().GetHeight());

		uint32 nVertRange = 
			static_cast<uint32>(GetClientArea().GetHeight()>GetClipArea().GetHeight()?GetClientArea().GetHeight()-GetClipArea().GetHeight():0);
		uint32 nHorzRange = 
			static_cast<uint32>(GetClientArea().GetWidth()>GetClipArea().GetWidth()?GetClientArea().GetWidth()-GetClipArea().GetWidth():0);


		// for vertical scrollbar
		if( m_bForceVertScroll )
		{
			if( m_pScrollbarVert->IsVisible() ==false  )
			{
				m_pScrollbarVert->SetVisible( true );
			}
		}
		else
		{
			if( nVertRange > 0 && m_pScrollbarVert->IsVisible()==false)
			{
				m_pScrollbarVert->SetVisible( true);
			}
			else if( nVertRange == 0 && m_pScrollbarVert->IsVisible() == true )
			{
				m_pScrollbarVert->SetVisible( false);
			}
		}

		// for horizontal scrollbar
		if( m_bForceHorzScroll )
		{
			if( m_pScrollbarHorz->IsVisible()==false )
			{
				m_pScrollbarHorz->SetVisible(true);
			}
		}
		else
		{
			if( nHorzRange > 0 && m_pScrollbarHorz->IsVisible()==false )
			{
				m_pScrollbarHorz->SetVisible(true);
			}
			else if( nHorzRange == 0 && m_pScrollbarHorz->IsVisible() == true )
			{
				m_pScrollbarHorz->SetVisible( false);
			}
		}

		m_pScrollbarVert->SetRange(nVertRange);
		m_pScrollbarVert->SetPageSize(static_cast<uint32>(GetClipArea().GetHeight()));

		m_pScrollbarHorz->SetRange(nHorzRange);
		m_pScrollbarHorz->SetPageSize(static_cast<uint32>(GetClipArea().GetWidth()));
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::OnScrollbarScroll( CGUIEventScrollbar* pEvent )
	{
		UpdateScrollbars();
		return CGUIWidget::OnScrollbarScroll( pEvent);
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetScrollbarAutoPosition( bool bFlag )
	{
		m_bIsScrollbarAutoPosition = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtScrollbarContainer::IsScrollbarAutoPosition( ) const
	{
		return m_bIsScrollbarAutoPosition;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
