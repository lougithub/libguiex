/** 
* @file guiwidgetscrollbarcontainer.cpp
* @brief widget: scrollbar container
* @author ken
* @date 2007-05-18
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtscrollbarcontainer.h>
#include <libguiex_widget\guiwgtscrollbar.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtScrollbarContainer);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtScrollbarContainer::ms_strType = "CGUIWgtScrollbarContainer";
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarContainer::CGUIWgtScrollbarContainer(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitScrollbarContainer();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarContainer::CGUIWgtScrollbarContainer( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitScrollbarContainer();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarContainer::~CGUIWgtScrollbarContainer(  )
	{
		//delete m_pScrollbarVert;
		//delete m_pScrollbarHorz;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::InitScrollbarContainer()
	{
		m_bForceVertScroll = false;
		m_bForceHorzScroll = false;

		//create scrollbar
		m_pScrollbarVert = static_cast<CGUIWgtScrollbar*>(GUI_CREATE_WIDGET("CGUIWgtScrollbar", GetName()+"_scrollbar_vert__auto__", GetProjectName()));
		m_pScrollbarVert->SetParent(this);
		m_pScrollbarVert->SetFlag(eFLAG_FOCUS_AGENCY, true);
		m_pScrollbarVert->SetFlag(eFLAG_OPEN_WITH_PARENT, false);
		m_pScrollbarVert->EnableNotifyParent(true);
		m_pScrollbarVert->EnableAutoPosition(true);

		m_pScrollbarHorz = static_cast<CGUIWgtScrollbar*>(GUI_CREATE_WIDGET("CGUIWgtScrollbar", GetName()+"_scrollbar_hort__auto__", GetProjectName()));
		m_pScrollbarHorz->SetParent(this);
		m_pScrollbarHorz->SetFlag(eFLAG_FOCUS_AGENCY, true);
		m_pScrollbarHorz->SetFlag(eFLAG_OPEN_WITH_PARENT, false);
		m_pScrollbarHorz->EnableNotifyParent(true);
		m_pScrollbarHorz->EnableAutoPosition(true);

		//set flag
		SetFocusable(true);
		SetFlag(eFLAG_EVENT_CLICK, false);
		SetFlag(eFLAG_OPEN_WITH_PARENT, true);
		SetFlag(eFLAG_SCROLLBAR_AUTOPOSITION, true);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtScrollbarContainer::Create()
	{
		//create scrollbar
		m_pScrollbarHorz->Create();
		m_pScrollbarVert->Create();

		return CGUIWidget::Create();
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
	void	CGUIWgtScrollbarContainer::UpdateDirtyRect()
	{
		GUI_FORCE_ASSERT("can't call function here, use PreUpdateDirtyRect() and PostUpdateDirtyRect() instead of this function");
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtScrollbarContainer::UpdateDirtyRect_SC_Begin()
	{
		CGUIWidget::UpdateDirtyRect();

		//clip rect for client
		if( m_pScrollbarVert->IsOpen() )
		{
			m_aClientClipRect.SetWidth(m_aWidgetRect.GetWidth() - m_pScrollbarVert->GetSize().GetWidth()*m_pScrollbarVert->GetDerivedScale().GetWidth());
		}
		if( m_pScrollbarHorz->IsOpen() )
		{
			m_aClientClipRect.SetHeight(m_aWidgetRect.GetHeight() - m_pScrollbarHorz->GetSize().GetHeight()*m_pScrollbarHorz->GetDerivedScale().GetHeight());
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtScrollbarContainer::UpdateDirtyRect_SC_End()
	{
		//client rect
		CGUIVector2 aWorkPos(m_aClientRect.m_fLeft, m_aClientRect.m_fTop);
		if (m_pScrollbarHorz->IsOpen())
		{
			aWorkPos.x -=  m_pScrollbarHorz->GetCurrentPos()*GetDerivedScale().m_fWidth;
		}
		if ( m_pScrollbarVert->IsOpen())
		{
			aWorkPos.y -= m_pScrollbarVert->GetCurrentPos()*GetDerivedScale().m_fHeight;
		}
		m_aClientRect.SetPosition(aWorkPos);

	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbarContainer::UpdateScrollbars(void)
	{
		if( IsOpen()==false)
		{
			if( m_pScrollbarVert->IsOpen()  )
			{
				m_pScrollbarVert->Close();
			}
			if( m_pScrollbarHorz->IsOpen()  )
			{
				m_pScrollbarHorz->Close();
			}
			return;
		}

		const CGUISize& rClientClipSize = GetClientClipRect().GetSize();
		const CGUISize&		rPageSize = GetClientRect().GetSize();

		uint32 nVertRange = 
			static_cast<uint32>(rPageSize.m_fHeight>rClientClipSize.GetHeight()?rPageSize.m_fHeight-rClientClipSize.GetHeight():0);
		uint32 nHorzRange = 
			static_cast<uint32>(rPageSize.m_fWidth>rClientClipSize.GetWidth()?rPageSize.m_fWidth-rClientClipSize.GetWidth():0);


		// for vertical scrollbar
		if( m_bForceVertScroll )
		{
			if( m_pScrollbarVert->IsOpen()==false  )
			{
				m_pScrollbarVert->Open();
//				SetRectDirty();
			}
		}
		else
		{
			if( nVertRange > 0 && m_pScrollbarVert->IsOpen()==false)
			{
				m_pScrollbarVert->Open();
//				SetRectDirty();
			}
			else if( nVertRange == 0 && m_pScrollbarVert->IsOpen() )
			{
				m_pScrollbarVert->Close();
//				SetRectDirty();
			}
		}

		// for horizontal scrollbar
		if( m_bForceHorzScroll )
		{
			if( m_pScrollbarHorz->IsOpen()==false )
			{
				m_pScrollbarHorz->Open();
//				SetRectDirty();
			}
		}
		else
		{
			if( nHorzRange > 0 && m_pScrollbarHorz->IsOpen()==false )
			{
				m_pScrollbarHorz->Open();
//				SetRectDirty();
			}
			else if( nHorzRange == 0 && m_pScrollbarHorz->IsOpen() )
			{
				m_pScrollbarHorz->Close();
//				SetRectDirty();
			}
		}

		nVertRange = static_cast<uint32>(
			GetClientRect().GetHeight()>GetClientClipRect().GetHeight()?
			GetClientRect().GetHeight()-GetClientClipRect().GetHeight():
			0);
		nHorzRange = static_cast<uint32>(
			GetClientRect().GetWidth()>GetClientClipRect().GetWidth()?
			GetClientRect().GetWidth()-GetClientClipRect().GetWidth():
			0);


		m_pScrollbarVert->SetRange(nVertRange);
		m_pScrollbarVert->SetPageSize(static_cast<uint32>(GetClientClipRect().GetHeight()));

		m_pScrollbarHorz->SetRange(nHorzRange);
		m_pScrollbarHorz->SetPageSize(static_cast<uint32>(GetClientClipRect().GetWidth()));
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarContainer::OnOpen( CGUIEventNotification* pEvent )
	{
		UpdateScrollbars();
		return CGUIWidget::OnOpen( pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarContainer::OnScaleChange( CGUIEventNotification* pEvent )
	{
		UpdateScrollbars();
		return CGUIWidget::OnScaleChange(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarContainer::OnSizeChange( CGUIEventSize* pEvent )
	{
		UpdateScrollbars();
		return CGUIWidget::OnSizeChange(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarContainer::OnScrollbarScroll( CGUIEventScrollbar* pEvent )
	{
//		SetRectDirty();
		return CGUIWidget::OnScrollbarScroll( pEvent);
	}
	//------------------------------------------------------------------------------

}//namespace guiex
