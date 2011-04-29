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
#include "guiwgtpanel.h"
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
	CGUIWgtPanel::CGUIWgtPanel(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitPanel();
	}
	//------------------------------------------------------------------------------
	CGUIWgtPanel::CGUIWgtPanel( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitPanel();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtPanel::InitPanel()
	{
		m_pImageBG = NULL;

		//flag
		for( int i=0; i<PANEL_BORDER_NONE; ++i)
		{
			m_aBorderInfo[i].m_pImageInfo = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtPanel::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "bg")
		{
			m_pImageBG = pImage;
			if( m_pImageBG && GetSize().IsEqualZero())
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "border_top")
		{
			if( pImage )
			{
				m_aBorderInfo[border_top].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_top].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_top].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_top].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_bottom")
		{
			if( pImage )
			{
				m_aBorderInfo[border_bottom].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_bottom].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_bottom].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_bottom].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_left")
		{
			if( pImage )
			{
				m_aBorderInfo[border_left].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_left].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_left].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_left].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_right")
		{
			if( pImage )
			{
				m_aBorderInfo[border_right].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_right].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_right].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_right].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_topleft")
		{
			if( pImage )
			{
				m_aBorderInfo[border_topleft].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_topleft].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_topleft].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_topleft].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_topright")
		{
			if( pImage )
			{
				m_aBorderInfo[border_topright].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_topright].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_topright].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_topright].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_bottomleft")
		{
			if( pImage )
			{
				m_aBorderInfo[border_bottomleft].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_bottomleft].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_bottomleft].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_bottomleft].m_pImageInfo = NULL;
			}
		}
		else if( rName == "border_bottomright")
		{
			if( pImage )
			{
				m_aBorderInfo[border_bottomright].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[border_bottomright].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[border_bottomright].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[border_bottomright].m_pImageInfo = NULL;
			}
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtPanel::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		//client rect
		m_aClientArea.m_fTop +=  GetBorderHeight(border_top);
		m_aClientArea.m_fLeft += GetBorderWidth(border_left);
		m_aClientArea.m_fRight -= GetBorderWidth(border_right);
		m_aClientArea.m_fBottom -= GetBorderHeight(border_bottom);

		//clip rect for client
		m_aClipArea = m_aClientArea;

		//border rect
		if( m_aBorderInfo[border_topleft].m_pImageInfo )
		{
			m_aBorderInfo[border_topleft].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fLeft - GetBorderWidth(border_left),
				GetClientArea().m_fTop - GetBorderHeight(border_top)));
			m_aBorderInfo[border_topleft].m_aRenderRect.SetSize( GetBorderSize(border_topleft) );
		}

		if( m_aBorderInfo[border_topright].m_pImageInfo )
		{
			m_aBorderInfo[border_topright].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fRight+GetBorderWidth(border_right)-GetBorderWidth(border_topright),
				GetClientArea().m_fTop - GetBorderHeight(border_top)));
			m_aBorderInfo[border_topright].m_aRenderRect.SetSize( GetBorderSize(border_topright));
		}

		if( m_aBorderInfo[border_bottomleft].m_pImageInfo )
		{
			m_aBorderInfo[border_bottomleft].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fLeft - GetBorderWidth(border_left),
				GetClientArea().m_fBottom+GetBorderHeight(border_bottom)-GetBorderHeight(border_bottomleft)));
			m_aBorderInfo[border_bottomleft].m_aRenderRect.SetSize( GetBorderSize(border_bottomleft) );
		}

		if( m_aBorderInfo[border_bottomright].m_pImageInfo )
		{
			m_aBorderInfo[border_bottomright].m_aRenderRect.SetPosition(CGUIVector2(
				GetClientArea().m_fRight+GetBorderWidth(border_right)-GetBorderWidth(border_bottomright),
				GetClientArea().m_fBottom+GetBorderHeight(border_bottom)-GetBorderHeight(border_bottomright)));
			m_aBorderInfo[border_bottomright].m_aRenderRect.SetSize( GetBorderSize(border_bottomright));
		}

		if( m_aBorderInfo[border_top].m_pImageInfo )
		{
			m_aBorderInfo[border_top].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fLeft-GetBorderWidth(border_left)+GetBorderWidth(border_topleft),
				GetClientArea().m_fTop - GetBorderHeight(border_top)));
			m_aBorderInfo[border_top].m_aRenderRect.SetSize( CGUISize(
				GetClientArea().GetWidth()-GetBorderWidth(border_topleft)-GetBorderWidth(border_topright)+GetBorderWidth(border_left)+GetBorderWidth(border_right),
				GetBorderHeight(border_top)));
		}

		if( m_aBorderInfo[border_bottom].m_pImageInfo )
		{
			m_aBorderInfo[border_bottom].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fLeft-GetBorderWidth(border_left)+GetBorderWidth(border_bottomleft),
				GetClientArea().m_fBottom));
			m_aBorderInfo[border_bottom].m_aRenderRect.SetSize( CGUISize(
				GetClientArea().GetWidth()-GetBorderWidth(border_bottomleft)-GetBorderWidth(border_bottomright)+GetBorderWidth(border_left)+GetBorderWidth(border_right),
				GetBorderHeight(border_bottom)));
		}

		if( m_aBorderInfo[border_left].m_pImageInfo )
		{
			m_aBorderInfo[border_left].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fLeft-GetBorderWidth(border_left),
				GetClientArea().m_fTop-GetBorderHeight(border_top)+GetBorderHeight(border_topleft)));
			m_aBorderInfo[border_left].m_aRenderRect.SetSize( CGUISize(
				GetBorderWidth(border_left),
				GetClientArea().GetHeight()-GetBorderHeight(border_topleft)-GetBorderHeight(border_bottomleft)+GetBorderHeight(border_top)+GetBorderHeight(border_bottom)));
		}

		if( m_aBorderInfo[border_right].m_pImageInfo )
		{
			m_aBorderInfo[border_right].m_aRenderRect.SetPosition( CGUIVector2(
				GetClientArea().m_fRight,
				GetClientArea().m_fTop-GetBorderHeight(border_top)+GetBorderHeight(border_topright)));
			m_aBorderInfo[border_right].m_aRenderRect.SetSize( CGUISize(
				GetBorderWidth(border_right),
				GetClientArea().GetHeight()-GetBorderHeight(border_topright)-GetBorderHeight(border_bottomright)+GetBorderHeight(border_top)+GetBorderHeight(border_bottom)));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtPanel::RenderSelf(IGUIInterfaceRender* pRender)
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
