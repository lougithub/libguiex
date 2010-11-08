/** 
* @file guiwgtpanel.cpp
* @brief panel, can be used with 
* CGUIWgtFrame as a client area
* @author ken
* @date 2006-08-02
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtpanel.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiimage.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtPanel);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtPanel::ms_strType = "CGUIWgtPanel";
	//------------------------------------------------------------------------------
	CGUIWgtPanel::CGUIWgtPanel(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitPanel();
	}
	//------------------------------------------------------------------------------
	CGUIWgtPanel::CGUIWgtPanel( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitPanel();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPanel::InitPanel()
	{
		m_pImageBG = NULL;

		//flag
		SetFocusable(false);
		SetSelfActivable(false);

		for( int i=0; i<PANEL_BORDER_NONE; ++i)
		{
			m_aBorderInfo[i].m_pImageInfo = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtPanel::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "BGIMAGE")
		{
			m_pImageBG = pImage;
			if( m_pImageBG && NEWGetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "PANEL_BORDER_TOP")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_TOP].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_TOP].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_TOP].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_TOP].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_BOTTOM")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_BOTTOM].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_BOTTOM].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_BOTTOM].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_BOTTOM].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_LEFT")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_LEFT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_LEFT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_LEFT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_LEFT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_RIGHT")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_RIGHT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_RIGHT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_RIGHT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_RIGHT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_TOPLEFT")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_TOPRIGHT")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_BOTTOMLEFT")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "PANEL_BORDER_BOTTOMRIGHT")
		{
			if( pImage )
			{
				m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_pImageInfo = NULL;
			}
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtPanel::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		//client rect
		m_aClientArea.m_fTop +=  GetBorderHeight(PANEL_BORDER_TOP);
		m_aClientArea.m_fLeft += GetBorderWidth(PANEL_BORDER_LEFT);
		m_aClientArea.m_fRight -= GetBorderWidth(PANEL_BORDER_RIGHT);
		m_aClientArea.m_fBottom -= GetBorderHeight(PANEL_BORDER_BOTTOM);

		//clip rect for client
		m_aClipArea = m_aClientArea;

		//border rect
		if( m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fLeft - GetBorderWidth(PANEL_BORDER_LEFT),
				GetClientArea().m_fTop - GetBorderHeight(PANEL_BORDER_TOP)));
			m_aBorderInfo[PANEL_BORDER_TOPLEFT].m_aRenderRect.SetSize( GetBorderSize(PANEL_BORDER_TOPLEFT) );
		}

		if( m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fRight+GetBorderWidth(PANEL_BORDER_RIGHT)-GetBorderWidth(PANEL_BORDER_TOPRIGHT),
				GetClientArea().m_fTop - GetBorderHeight(PANEL_BORDER_TOP)));
			m_aBorderInfo[PANEL_BORDER_TOPRIGHT].m_aRenderRect.SetSize( GetBorderSize(PANEL_BORDER_TOPRIGHT));
		}

		if( m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fLeft - GetBorderWidth(PANEL_BORDER_LEFT),
				GetClientArea().m_fBottom+GetBorderHeight(PANEL_BORDER_BOTTOM)-GetBorderHeight(PANEL_BORDER_BOTTOMLEFT)));
			m_aBorderInfo[PANEL_BORDER_BOTTOMLEFT].m_aRenderRect.SetSize( GetBorderSize(PANEL_BORDER_BOTTOMLEFT) );
		}

		if( m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fRight+GetBorderWidth(PANEL_BORDER_RIGHT)-GetBorderWidth(PANEL_BORDER_BOTTOMRIGHT),
				GetClientArea().m_fBottom+GetBorderHeight(PANEL_BORDER_BOTTOM)-GetBorderHeight(PANEL_BORDER_BOTTOMRIGHT)));
			m_aBorderInfo[PANEL_BORDER_BOTTOMRIGHT].m_aRenderRect.SetSize( GetBorderSize(PANEL_BORDER_BOTTOMRIGHT));
		}

		if( m_aBorderInfo[PANEL_BORDER_TOP].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_TOP].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fLeft-GetBorderWidth(PANEL_BORDER_LEFT)+GetBorderWidth(PANEL_BORDER_TOPLEFT),
				GetClientArea().m_fTop - GetBorderHeight(PANEL_BORDER_TOP)));
			m_aBorderInfo[PANEL_BORDER_TOP].m_aRenderRect.SetSize( CGUISize(
				GetClientArea().GetWidth()-GetBorderWidth(PANEL_BORDER_TOPLEFT)-GetBorderWidth(PANEL_BORDER_TOPRIGHT)+GetBorderWidth(PANEL_BORDER_LEFT)+GetBorderWidth(PANEL_BORDER_RIGHT),
				GetBorderHeight(PANEL_BORDER_TOP)));
		}

		if( m_aBorderInfo[PANEL_BORDER_BOTTOM].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_BOTTOM].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fLeft-GetBorderWidth(PANEL_BORDER_LEFT)+GetBorderWidth(PANEL_BORDER_BOTTOMLEFT),
				GetClientArea().m_fBottom));
			m_aBorderInfo[PANEL_BORDER_BOTTOM].m_aRenderRect.SetSize( CGUISize(
				GetClientArea().GetWidth()-GetBorderWidth(PANEL_BORDER_BOTTOMLEFT)-GetBorderWidth(PANEL_BORDER_BOTTOMRIGHT)+GetBorderWidth(PANEL_BORDER_LEFT)+GetBorderWidth(PANEL_BORDER_RIGHT),
				GetBorderHeight(PANEL_BORDER_BOTTOM)));
		}

		if( m_aBorderInfo[PANEL_BORDER_LEFT].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_LEFT].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fLeft-GetBorderWidth(PANEL_BORDER_LEFT),
				GetClientArea().m_fTop-GetBorderHeight(PANEL_BORDER_TOP)+GetBorderHeight(PANEL_BORDER_TOPLEFT)));
			m_aBorderInfo[PANEL_BORDER_LEFT].m_aRenderRect.SetSize( CGUISize(
				GetBorderWidth(PANEL_BORDER_LEFT),
				GetClientArea().GetHeight()-GetBorderHeight(PANEL_BORDER_TOPLEFT)-GetBorderHeight(PANEL_BORDER_BOTTOMLEFT)+GetBorderHeight(PANEL_BORDER_TOP)+GetBorderHeight(PANEL_BORDER_BOTTOM)));
		}

		if( m_aBorderInfo[PANEL_BORDER_RIGHT].m_pImageInfo )
		{
			m_aBorderInfo[PANEL_BORDER_RIGHT].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fRight,
				GetClientArea().m_fTop-GetBorderHeight(PANEL_BORDER_TOP)+GetBorderHeight(PANEL_BORDER_TOPRIGHT)));
			m_aBorderInfo[PANEL_BORDER_RIGHT].m_aRenderRect.SetSize( CGUISize(
				GetBorderWidth(PANEL_BORDER_RIGHT),
				GetClientArea().GetHeight()-GetBorderHeight(PANEL_BORDER_TOPRIGHT)-GetBorderHeight(PANEL_BORDER_BOTTOMRIGHT)+GetBorderHeight(PANEL_BORDER_TOP)+GetBorderHeight(PANEL_BORDER_BOTTOM)));
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtPanel::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetClientArea( ));
		for( int i=0; i<PANEL_BORDER_NONE; ++i)
		{
			DrawImage(pRender, m_aBorderInfo[i].m_pImageInfo, GetBorderRect(i) );
		}
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWgtPanel::GetBorderRect(int32 eBorder)
	{
		GUI_ASSERT( eBorder < PANEL_BORDER_NONE , "[CGUIWgtPanel::GetBorderRect]:invalid border index");
		return m_aBorderInfo[eBorder].m_aRenderRect;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
