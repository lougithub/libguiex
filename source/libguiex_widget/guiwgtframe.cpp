/** 
* @file guiwgtframe.cpp
* @brief just like mfc's frame. is a title now.
* @author ken
* @date 2006-07-20
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtframe.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guimousecursor.h>
#include <libguiex_core\guievent.h>
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiexception.h>

//============================================================================//
// define 
//============================================================================// 
namespace guiex
{
#define WIDGETFRAME_TESTBORDER(border)	\
	if(m_aBorderInfo[border].m_aType == eBorder_Dynamic && GetBorderRect(border).IsPointInRect(rPos))\
	{									\
	return border;		\
}	
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtFrame);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtFrame::ms_strType = "CGUIWgtFrame";
	//------------------------------------------------------------------------------
	CGUIWgtFrame::CGUIWgtFrame( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitFrame();
	}
	//------------------------------------------------------------------------------
	CGUIWgtFrame::CGUIWgtFrame( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
	{
		InitFrame();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtFrame::SetBorderDynamic(uint32 eBorder, bool bDynamic)
	{
		if( eBorder < FRAME_BORDER_NONE)
		{
			m_aBorderInfo[eBorder].m_aType = bDynamic?eBorder_Dynamic:eBorder_Static;
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtFrame::IsBorderDynamic(uint32 eBorder) const
	{
		if( eBorder < FRAME_BORDER_NONE && m_aBorderInfo[eBorder].m_aType == eBorder_Dynamic)
		{
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::InitFrame()
	{
		m_eDragSizing = FRAME_BORDER_NONE;
		m_eTitlebarType = TITLEBAR_NONE;
		m_bHasDecorate = false;
		m_bShowClient = true;

		for( int i=0; i<FRAME_BORDER_NONE; ++i)
		{
			m_aBorderInfo[i].m_pImageInfo = NULL;
			m_aBorderInfo[i].m_aType = eBorder_None;
		}

		m_pImageTitleBar = NULL;
		m_pImageDecorate[0] = NULL;
		m_pImageDecorate[1] = NULL;

		SetFocusable(false);
		SetSelfActivable(true);
		SetFlag(eFLAG_MOVABLE, true);
		SetFlag(eFLAG_EVENT_DBCLICK, true);
		//SetFlag(eFLAG_OPEN_WITH_PARENT, false);

	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::OnSetImage( const CGUIString& rName, CGUIImage* pImage)
	{
		if( rName == "FRAME_TITLEBAR_TOP")
		{
			if( pImage )
			{
				m_eTitlebarType = TITLEBAR_TOP;
				m_pImageTitleBar = pImage;
				m_aSizeTitleBar.SetSize( pImage->GetSize() );
			}
			else
			{
				if( m_eTitlebarType == TITLEBAR_TOP )
				{
					m_pImageTitleBar = NULL;
					m_eTitlebarType = TITLEBAR_NONE;
					m_aSizeTitleBar.SetSize( CGUISize(0.f,0.f) );
				}
			}
		}
		else if( rName == "FRAME_TITLEBAR_BOTTOM")
		{
			if( pImage )
			{
				m_eTitlebarType = TITLEBAR_BOTTOM;
				m_pImageTitleBar = pImage;
				m_aSizeTitleBar.SetSize( pImage->GetSize() );
			}
			else
			{
				if( m_eTitlebarType == TITLEBAR_BOTTOM )
				{
					m_pImageTitleBar = NULL;
					m_eTitlebarType = TITLEBAR_NONE;
					m_aSizeTitleBar.SetSize( CGUISize(0.f,0.f) );
				}
			}
		}
		else if( rName == "FRAME_TITLEBAR_LEFT")
		{
			if( pImage )
			{
				m_eTitlebarType = TITLEBAR_LEFT;
				m_pImageTitleBar = pImage;
				m_aSizeTitleBar.SetSize( pImage->GetSize() );
			}
			else
			{
				if( m_eTitlebarType == TITLEBAR_LEFT )
				{
					m_pImageTitleBar = NULL;
					m_eTitlebarType = TITLEBAR_NONE;
					m_aSizeTitleBar.SetSize( CGUISize(0.f,0.f) );
				}
			}
		}
		else if( rName == "FRAME_TITLEBAR_RIGHT")
		{
			if( pImage )
			{
				m_eTitlebarType = TITLEBAR_RIGHT;
				m_pImageTitleBar = pImage;
				m_aSizeTitleBar.SetSize( pImage->GetSize() );
			}
			else
			{
				if( m_eTitlebarType == TITLEBAR_RIGHT )
				{
					m_pImageTitleBar = NULL;
					m_eTitlebarType = TITLEBAR_NONE;
					m_aSizeTitleBar.SetSize( CGUISize(0.f,0.f) );
				}
			}
		}
		else if( rName == "FRAME_BORDER_LEFT")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_LEFT].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_LEFT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_LEFT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_LEFT].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_LEFT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_LEFT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_RIGHT")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_RIGHT].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_RIGHT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_RIGHT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_RIGHT].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_RIGHT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_RIGHT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_TOP")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_TOP].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_TOP].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_TOP].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_TOP].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_TOP].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_TOP].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_BOTTOM")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_BOTTOM].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_BOTTOM].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_TOPLEFT")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_TOPRIGHT")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_BOTTOMLEFT")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_BORDER_BOTTOMRIGHT")
		{
			if( pImage )
			{
				m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aType = eBorder_Dynamic;
				m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aSize.SetSize(pImage->GetSize());
				m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_pImageInfo = pImage;
			}
			else
			{
				m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aType = eBorder_None;
				m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aSize.SetSize(CGUISize(0.f,0.f));
				m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_pImageInfo = NULL;
			}
		}
		else if( rName == "FRAME_DECORATE_0")
		{
			m_pImageDecorate[0]  = pImage;
		}
		else if( rName == "FRAME_DECORATE_1")
		{
			m_pImageDecorate[1]  = pImage;
		}

	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtFrame::Create()
	{
		if( m_eTitlebarType != TITLEBAR_NONE)
		{
			if( m_pImageDecorate[0] &&m_pImageDecorate[1] )
			{
				m_bHasDecorate = true;
				m_aSizeDecorate[0] = m_pImageDecorate[0] ->GetSize();
				m_aSizeDecorate[1] = m_pImageDecorate[1]->GetSize();
			}
			else
			{
				m_aSizeDecorate[0].m_fHeight = 
					m_aSizeDecorate[0].m_fWidth =
					m_aSizeDecorate[1].m_fHeight = 
					m_aSizeDecorate[1].m_fWidth = 0.0f;
				m_bHasDecorate = false;
			}
		}

		return CGUIWidget::Create();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtFrame::HitTest( const CGUIVector2& rPos) 
	{
		if(IsClientVisible())
		{
			return CGUIWidget::HitTest(rPos);
		}
		else
		{
			return GetTitlebarRect().IsPointInRect(rPos);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtFrame::RenderSelf(IGUIInterfaceRender* pRender)
	{
		
		if( HasTitlebar())
		{
			//draw title bar
			DrawImage( pRender, m_pImageTitleBar, GetTitlebarRect(), pRender->GetAndIncZ(),&GetClipRect() );
			if( HasTitlebarDecorate())
			{
				DrawImage(pRender, m_pImageDecorate[0] , GetDecorateRect(0), pRender->GetAndIncZ(),&GetClipRect() );
				DrawImage(pRender, m_pImageDecorate[1] , GetDecorateRect(1), pRender->GetAndIncZ(),&GetClipRect() );
			}

			//draw title
			if( !GetText().Empty())
			{
				DrawString(pRender, GetText(), GetTitlebarRect(), GetTextAlignment(), &GetClipRect());
			}
		}

		if( IsClientVisible() )
		{
			for( int i=0; i<FRAME_BORDER_NONE; ++i)
			{
				DrawImage(pRender, m_aBorderInfo[i].m_pImageInfo, GetBorderRect(i), pRender->GetAndIncZ(),&GetClipRect() );
			}
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtFrame::HasTitlebar() const
	{
		return m_eTitlebarType != TITLEBAR_NONE;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtFrame::HasTitlebarDecorate()
	{
		return m_bHasDecorate;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtFrame::IsClientVisible() const
	{
		return m_bShowClient;
	}
	//------------------------------------------------------------------------------
	bool		CGUIWgtFrame::IsHorizonalTitlebar() const
	{
		return ( m_eTitlebarType == TITLEBAR_TOP || m_eTitlebarType == TITLEBAR_BOTTOM );
	}
	//------------------------------------------------------------------------------
	bool		CGUIWgtFrame::IsVerticalTitlebar() const
	{
		return ( m_eTitlebarType == TITLEBAR_LEFT || m_eTitlebarType == TITLEBAR_RIGHT );
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::UpdateDirtyRect()
	{
		CGUIWidget::UpdateDirtyRect();

		CGUISize aSizeTitleBar = m_aSizeTitleBar * GetScale();
		CGUISize aSizeDecorate_0 = m_aSizeDecorate[0] * GetScale();
		CGUISize aSizeDecorate_1 = m_aSizeDecorate[1] * GetScale();

		//update title bar rect
		m_aTitleBarRect = m_aWidgetRect;
		switch( m_eTitlebarType )
		{
		case TITLEBAR_TOP:
			m_aTitleBarRect.m_fLeft += aSizeDecorate_0.GetWidth();
			m_aTitleBarRect.m_fRight -= m_aSizeDecorate[1].GetWidth();
			m_aTitleBarRect.m_fBottom = m_aTitleBarRect.m_fTop + aSizeTitleBar.GetHeight();
			break;

		case TITLEBAR_BOTTOM:
			m_aTitleBarRect.m_fLeft += aSizeDecorate_0.GetWidth();
			m_aTitleBarRect.m_fRight -= m_aSizeDecorate[1].GetWidth();
			m_aTitleBarRect.m_fTop = m_aTitleBarRect.m_fBottom - aSizeTitleBar.GetHeight();
			break;

		case TITLEBAR_RIGHT:
			m_aTitleBarRect.m_fLeft =m_aTitleBarRect.m_fRight-aSizeTitleBar.GetWidth();
			m_aTitleBarRect.m_fTop += aSizeDecorate_0.GetHeight();
			m_aTitleBarRect.m_fBottom -= m_aSizeDecorate[1].GetHeight();
			break;

		case TITLEBAR_LEFT:
			m_aTitleBarRect.m_fRight =m_aTitleBarRect.m_fLeft + aSizeTitleBar.GetWidth();
			m_aTitleBarRect.m_fTop += aSizeDecorate_0.GetHeight();
			m_aTitleBarRect.m_fBottom -= m_aSizeDecorate[1].GetHeight();
			break;
		}

		//update decorate rect
		switch( m_eTitlebarType )
		{
			case TITLEBAR_TOP:
			case TITLEBAR_BOTTOM:
				m_aDecorateRect[0].SetPosition( CGUIVector2(
					m_aTitleBarRect.m_fLeft - aSizeDecorate_0.GetWidth(),
					m_aTitleBarRect.m_fTop));
				m_aDecorateRect[0].SetSize( aSizeDecorate_0);
				m_aDecorateRect[1].SetPosition( CGUIVector2(
					m_aTitleBarRect.m_fRight,
					m_aTitleBarRect.m_fTop));
				m_aDecorateRect[1].SetSize( m_aSizeDecorate[1] );
				break;

			case TITLEBAR_LEFT:
			case TITLEBAR_RIGHT:
				m_aDecorateRect[0].SetPosition( CGUIVector2(
					m_aTitleBarRect.m_fLeft,
					m_aTitleBarRect.m_fTop - aSizeDecorate_0.GetHeight()));
				m_aDecorateRect[0].SetSize( aSizeDecorate_0 );
				m_aDecorateRect[1].SetPosition( CGUIVector2(
					m_aTitleBarRect.m_fLeft,
					m_aTitleBarRect.m_fBottom));
				m_aDecorateRect[1].SetSize( m_aSizeDecorate[1] );
				break;
		}

		//client rect
		switch( m_eTitlebarType )
		{
		case TITLEBAR_TOP:
			m_aClientRect.m_fTop += aSizeTitleBar.GetHeight();
			m_aClientRect.m_fLeft += GetBorderWidth(FRAME_BORDER_LEFT);
			m_aClientRect.m_fRight -= GetBorderWidth(FRAME_BORDER_RIGHT);
			m_aClientRect.m_fBottom -= GetBorderHeight(FRAME_BORDER_BOTTOM);
			break;

		case TITLEBAR_BOTTOM:
			m_aClientRect.m_fBottom -= aSizeTitleBar.GetHeight();
			m_aClientRect.m_fTop +=  GetBorderHeight(FRAME_BORDER_TOP);
			m_aClientRect.m_fLeft += GetBorderWidth(FRAME_BORDER_LEFT);
			m_aClientRect.m_fRight -= GetBorderWidth(FRAME_BORDER_RIGHT);
			break;

		case TITLEBAR_RIGHT:
			m_aClientRect.m_fRight -= aSizeTitleBar.GetWidth();
			m_aClientRect.m_fTop +=  GetBorderHeight(FRAME_BORDER_TOP);
			m_aClientRect.m_fLeft += GetBorderWidth(FRAME_BORDER_LEFT);
			m_aClientRect.m_fBottom -= GetBorderHeight(FRAME_BORDER_BOTTOM);
			break;

		case TITLEBAR_LEFT:
			m_aClientRect.m_fLeft += aSizeTitleBar.GetWidth();
			m_aClientRect.m_fTop +=  GetBorderHeight(FRAME_BORDER_TOP);
			m_aClientRect.m_fRight -= GetBorderWidth(FRAME_BORDER_RIGHT);
			m_aClientRect.m_fBottom -= GetBorderHeight(FRAME_BORDER_BOTTOM);
			break;

		case TITLEBAR_NONE:
			m_aClientRect.m_fTop +=  GetBorderHeight(FRAME_BORDER_TOP);
			m_aClientRect.m_fLeft += GetBorderWidth(FRAME_BORDER_LEFT);
			m_aClientRect.m_fRight -= GetBorderWidth(FRAME_BORDER_RIGHT);
			m_aClientRect.m_fBottom -= GetBorderHeight(FRAME_BORDER_BOTTOM);
			break;
		}

		//clip rect for client
		m_aClientClipRect = m_aClientRect;

		//border rect

		if( m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aRenderRect.SetPosition(CGUIVector2(
				m_aClientRect.m_fLeft - GetBorderWidth(FRAME_BORDER_LEFT),
				m_aClientRect.m_fTop - GetBorderHeight(FRAME_BORDER_TOP)));
			m_aBorderInfo[FRAME_BORDER_TOPLEFT].m_aRenderRect.SetSize( GetBorderSize(FRAME_BORDER_TOPLEFT) );
		}

		if( m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aRenderRect.SetPosition(CGUIVector2(
				m_aClientRect.m_fRight+GetBorderWidth(FRAME_BORDER_RIGHT)-GetBorderWidth(FRAME_BORDER_TOPRIGHT),
				m_aClientRect.m_fTop - GetBorderHeight(FRAME_BORDER_TOP)));
			m_aBorderInfo[FRAME_BORDER_TOPRIGHT].m_aRenderRect.SetSize( GetBorderSize(FRAME_BORDER_TOPRIGHT));
		}

		if( m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aRenderRect.SetPosition(CGUIVector2(
				m_aClientRect.m_fLeft - GetBorderWidth(FRAME_BORDER_LEFT),
				m_aClientRect.m_fBottom+GetBorderHeight(FRAME_BORDER_BOTTOM)-GetBorderHeight(FRAME_BORDER_BOTTOMLEFT)));
			m_aBorderInfo[FRAME_BORDER_BOTTOMLEFT].m_aRenderRect.SetSize( GetBorderSize(FRAME_BORDER_BOTTOMLEFT) );
		}

		if( m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aRenderRect.SetPosition(CGUIVector2(
				m_aClientRect.m_fRight+GetBorderWidth(FRAME_BORDER_RIGHT)-GetBorderWidth(FRAME_BORDER_BOTTOMRIGHT),
				m_aClientRect.m_fBottom+GetBorderHeight(FRAME_BORDER_BOTTOM)-GetBorderHeight(FRAME_BORDER_BOTTOMRIGHT)));
			m_aBorderInfo[FRAME_BORDER_BOTTOMRIGHT].m_aRenderRect.SetSize( GetBorderSize(FRAME_BORDER_BOTTOMRIGHT));
		}

		if( m_aBorderInfo[FRAME_BORDER_TOP].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_TOP].m_aRenderRect.SetPosition( CGUIVector2(
				m_aClientRect.m_fLeft-GetBorderWidth(FRAME_BORDER_LEFT)+GetBorderWidth(FRAME_BORDER_TOPLEFT),
				m_aClientRect.m_fTop - GetBorderHeight(FRAME_BORDER_TOP)));
			m_aBorderInfo[FRAME_BORDER_TOP].m_aRenderRect.SetSize( CGUISize(
				m_aClientRect.GetWidth()-GetBorderWidth(FRAME_BORDER_TOPLEFT)-GetBorderWidth(FRAME_BORDER_TOPRIGHT)+GetBorderWidth(FRAME_BORDER_LEFT)+GetBorderWidth(FRAME_BORDER_RIGHT),
				GetBorderHeight(FRAME_BORDER_TOP)));
		}

		if( m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aRenderRect.SetPosition( CGUIVector2(
				m_aClientRect.m_fLeft-GetBorderWidth(FRAME_BORDER_LEFT)+GetBorderWidth(FRAME_BORDER_BOTTOMLEFT),
				m_aClientRect.m_fBottom));
			m_aBorderInfo[FRAME_BORDER_BOTTOM].m_aRenderRect.SetSize( CGUISize(
				m_aClientRect.GetWidth()-GetBorderWidth(FRAME_BORDER_BOTTOMLEFT)-GetBorderWidth(FRAME_BORDER_BOTTOMRIGHT)+GetBorderWidth(FRAME_BORDER_LEFT)+GetBorderWidth(FRAME_BORDER_RIGHT),
				GetBorderHeight(FRAME_BORDER_BOTTOM)));
		}

		if( m_aBorderInfo[FRAME_BORDER_LEFT].m_aType != eBorder_None )
		{

			m_aBorderInfo[FRAME_BORDER_LEFT].m_aRenderRect.SetPosition( CGUIVector2(
				m_aClientRect.m_fLeft-GetBorderWidth(FRAME_BORDER_LEFT),
				m_aClientRect.m_fTop-GetBorderHeight(FRAME_BORDER_TOP)+GetBorderHeight(FRAME_BORDER_TOPLEFT)));
			m_aBorderInfo[FRAME_BORDER_LEFT].m_aRenderRect.SetSize( CGUISize(
				GetBorderWidth(FRAME_BORDER_LEFT),
				m_aClientRect.GetHeight()-GetBorderHeight(FRAME_BORDER_TOPLEFT)-GetBorderHeight(FRAME_BORDER_BOTTOMLEFT)+GetBorderHeight(FRAME_BORDER_TOP)+GetBorderHeight(FRAME_BORDER_BOTTOM)));
		}

		if( m_aBorderInfo[FRAME_BORDER_RIGHT].m_aType != eBorder_None )
		{
			m_aBorderInfo[FRAME_BORDER_RIGHT].m_aRenderRect.SetPosition( CGUIVector2(
				m_aClientRect.m_fRight,
				m_aClientRect.m_fTop-GetBorderHeight(FRAME_BORDER_TOP)+GetBorderHeight(FRAME_BORDER_TOPRIGHT)));
			m_aBorderInfo[FRAME_BORDER_RIGHT].m_aRenderRect.SetSize( CGUISize(
				GetBorderWidth(FRAME_BORDER_RIGHT),
				m_aClientRect.GetHeight()-GetBorderHeight(FRAME_BORDER_TOPRIGHT)-GetBorderHeight(FRAME_BORDER_BOTTOMRIGHT)+GetBorderHeight(FRAME_BORDER_TOP)+GetBorderHeight(FRAME_BORDER_BOTTOM)));
		}
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWgtFrame::GetDecorateRect(int32 nIdx)
	{
		GUI_ASSERT(nIdx>=0 && nIdx<=2, "invalid parameter" );

		UpdateWidgetRect();
		return m_aDecorateRect[nIdx];
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::ShowClient(bool bShow)
	{
		// hide or show any child windows
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			bShow?pWidget->Show() : pWidget->Hide();
			pWidget = pWidget->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveTopBorder(real deltaY)
	{
		SetRectSize(m_aStartSize.GetWidth(), m_aStartSize.GetHeight() - deltaY);
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.y += (m_aStartSize.m_fHeight - GetSize().m_fHeight )*(1.0f-GetTagPoint().y);
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveBottomBorder(real deltaY)
	{
		SetRectSize(m_aStartSize.GetWidth(), m_aStartSize.GetHeight()+deltaY);
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.y += (GetSize().m_fHeight-m_aStartSize.m_fHeight)*GetTagPoint().y;
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveLeftBorder(real deltaX)
	{
		SetRectSize(m_aStartSize.GetWidth()-deltaX, m_aStartSize.GetHeight());
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.x += (m_aStartSize.m_fWidth - GetSize().m_fWidth )*(1.0f-GetTagPoint().x);
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveRightBorder(real deltaX)
	{
		SetRectSize(m_aStartSize.GetWidth()+deltaX, m_aStartSize.GetHeight());
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.x += ( GetSize().m_fWidth-m_aStartSize.m_fWidth )*GetTagPoint().x;
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveTopLeftBorder(real deltaX, real deltaY)
	{
		SetRectSize(m_aStartSize.GetWidth()-deltaX, m_aStartSize.GetHeight()-deltaY);
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.x += (m_aStartSize.m_fWidth - GetSize().m_fWidth )*(1.0f-GetTagPoint().x);
		pLocalPos.y += (m_aStartSize.m_fHeight - GetSize().m_fHeight )*(1.0f-GetTagPoint().y);
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveBottomLeftBorder(real deltaX, real deltaY)
	{
		SetRectSize(m_aStartSize.GetWidth()-deltaX, m_aStartSize.GetHeight()+deltaY);
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.x += (m_aStartSize.m_fWidth - GetSize().m_fWidth )*(1.0f-GetTagPoint().x);
		pLocalPos.y += ( GetSize().m_fHeight-m_aStartSize.m_fHeight )*GetTagPoint().y;
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveTopRightBorder(real deltaX,real deltaY)
	{
		SetRectSize(m_aStartSize.GetWidth()+deltaX, m_aStartSize.GetHeight()-deltaY);
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.x += ( GetSize().m_fWidth-m_aStartSize.m_fWidth )*GetTagPoint().x;
		pLocalPos.y += (m_aStartSize.m_fHeight - GetSize().m_fHeight )*(1.0f-GetTagPoint().y);
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::MoveBottomRightBorder(real deltaX,real deltaY)
	{
		SetRectSize(m_aStartSize.GetWidth()+deltaX, m_aStartSize.GetHeight()+deltaY);
		CGUIVector2 pLocalPos = m_aStartPos;
		pLocalPos.x += ( GetSize().m_fWidth-m_aStartSize.m_fWidth )*GetTagPoint().x;
		pLocalPos.y += ( GetSize().m_fHeight-m_aStartSize.m_fHeight )*GetTagPoint().y;
		SetLocalPosition(pLocalPos);
	}
	//------------------------------------------------------------------------------
	const CGUIRect &	CGUIWgtFrame::GetTitlebarRect()
	{
		UpdateWidgetRect();
		return m_aTitleBarRect;
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWgtFrame::GetBorderRect(int32 eBorder)
	{
		GUI_ASSERT( eBorder < FRAME_BORDER_NONE , "error");
		UpdateWidgetRect();
		return m_aBorderInfo[eBorder].m_aRenderRect;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWgtFrame::IsPointInTitlebar( const CGUIVector2& rPos)
	{
		if( HasTitlebar() == false )
		{
			return false;
		}

		return GetTitlebarRect().IsPointInRect(rPos);
	}
	//------------------------------------------------------------------------------
	CGUIWgtFrame::EBorderLocation	CGUIWgtFrame::GetBorderType( const CGUIVector2& rPos)
	{
		if( IsClientVisible() == false )
		{
			return FRAME_BORDER_NONE;
		}

		WIDGETFRAME_TESTBORDER(FRAME_BORDER_BOTTOMRIGHT);
		WIDGETFRAME_TESTBORDER(FRAME_BORDER_BOTTOMLEFT);
		WIDGETFRAME_TESTBORDER(FRAME_BORDER_TOPRIGHT);
		WIDGETFRAME_TESTBORDER(FRAME_BORDER_TOPLEFT);

		WIDGETFRAME_TESTBORDER(FRAME_BORDER_TOP);
		WIDGETFRAME_TESTBORDER(FRAME_BORDER_RIGHT);
		WIDGETFRAME_TESTBORDER(FRAME_BORDER_BOTTOM);
		WIDGETFRAME_TESTBORDER(FRAME_BORDER_LEFT);	

		return FRAME_BORDER_NONE;
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtFrame::OnMouseLeftDbClick( CGUIEventMouse* pEvent )
	{
		if( HasTitlebar())
		{
			if(GetTitlebarRect().IsPointInRect(pEvent->GetPosition()))
			{
				m_bShowClient = !m_bShowClient;
				ShowClient(m_bShowClient);
			}
		}

		return CGUIWidget::OnMouseLeftDbClick(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtFrame::OnMouseMove( CGUIEventMouse* pEvent )
	{
		//normal process
		switch( GetBorderType(pEvent->GetPosition()))
		{
		case FRAME_BORDER_TOPLEFT:
		case FRAME_BORDER_BOTTOMRIGHT:
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_SIZE_NW2SE");
			break;

		case FRAME_BORDER_TOPRIGHT:
		case FRAME_BORDER_BOTTOMLEFT:
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_SIZE_NE2SW");
			break;

		case FRAME_BORDER_TOP:	
		case FRAME_BORDER_BOTTOM:
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_SIZE_N2S");
			break;

		case FRAME_BORDER_LEFT:
		case FRAME_BORDER_RIGHT:
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_SIZE_E2W");
			break;

		case FRAME_BORDER_NONE:
			CGUIMouseCursor::Instance()->SetCursor("CURSOR_DEFAULT");
			break;
		}

		return CGUIWidget::OnMouseMove(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtFrame::OnDragBegin( CGUIEventDrag* pEvent )
	{
		if( IsPointInTitlebar(pEvent->GetMousePos()))
		{
			//title bar move

		}
		else
		{
			m_eDragSizing = GetBorderType(pEvent->GetMousePos());
			if( m_eDragSizing != FRAME_BORDER_NONE )
			{
				//resize frame
				m_aStartMousePoint = pEvent->GetMousePos();
				m_aStartSize = GetRect().GetSize();
				m_aStartPos = GetLocalPosition();
			}
			else
			{
				//ignore event
				pEvent->Expire(true);
			}
		}

		return CGUIWidget::OnDragBegin( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtFrame::OnDragProcess( CGUIEventDrag* pEvent )
	{
		if( m_eDragSizing != FRAME_BORDER_NONE )
		{
			//drag sizing
			CGUIVector2 aMousePos = pEvent->GetMousePos();

			real deltaX = 0.0f;
			real deltaY = 0.0f;

			switch( m_eDragSizing )
			{
			case FRAME_BORDER_TOPLEFT:
				deltaY = aMousePos.y-m_aStartMousePoint.y;
				deltaX = aMousePos.x-m_aStartMousePoint.x;
				MoveTopLeftBorder(deltaX,deltaY);
				break;
			case FRAME_BORDER_BOTTOMRIGHT:
				deltaY = aMousePos.y-m_aStartMousePoint.y;
				deltaX = aMousePos.x-m_aStartMousePoint.x;
				MoveBottomRightBorder(deltaX,deltaY);
				break;
			case FRAME_BORDER_TOPRIGHT:
				deltaY = aMousePos.y-m_aStartMousePoint.y;
				deltaX = aMousePos.x-m_aStartMousePoint.x;
				MoveTopRightBorder(deltaX,deltaY);
				break;
			case FRAME_BORDER_BOTTOMLEFT:
				deltaY = aMousePos.y-m_aStartMousePoint.y;
				deltaX = aMousePos.x-m_aStartMousePoint.x;
				MoveBottomLeftBorder(deltaX,deltaY);
				break;
			case FRAME_BORDER_TOP:	
				deltaY = aMousePos.y-m_aStartMousePoint.y;
				MoveTopBorder(deltaY);
				break;
			case FRAME_BORDER_BOTTOM:
				deltaY = aMousePos.y-m_aStartMousePoint.y;
				MoveBottomBorder(deltaY);
				break;
			case FRAME_BORDER_LEFT:
				deltaX = aMousePos.x-m_aStartMousePoint.x;
				MoveLeftBorder(deltaX);
				break;
			case FRAME_BORDER_RIGHT:
				deltaX = aMousePos.x-m_aStartMousePoint.x;
				MoveRightBorder(deltaX);
				break;
			}
			pEvent->Expire(true);
		}

		return CGUIWidget::OnDragProcess( pEvent );
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtFrame::OnDragEnd( CGUIEventDrag* pEvent )
	{
		if( m_eDragSizing != FRAME_BORDER_NONE )
		{
			//end sizing
			m_eDragSizing = FRAME_BORDER_NONE;
		}

		return CGUIWidget::OnDragEnd( pEvent );
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIWgtFrame::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		CGUIProperty* pProperty = NULL;
		
		if( rName == "BORDER_TOPLEFT_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_TOPLEFT)));
		}
		else if( rName == "BORDER_TOPRIGHT_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_TOPRIGHT)));
		}
		else if( rName == "BORDER_BOTTOMRIGHT_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_BOTTOMRIGHT)));
		}
		else if( rName == "BORDER_BOTTOMLEFT_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_BOTTOMLEFT)));
		}
		else if( rName == "BORDER_TOP_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_TOP)));
		}
		else if( rName == "BORDER_LEFT_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_LEFT)));
		}
		else if( rName == "BORDER_BOTTOM_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_BOTTOM)));
		}
		else if( rName == "BORDER_RIGHT_DYNAMIC" && rType == "BOOL" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsBorderDynamic(FRAME_BORDER_RIGHT)));
		}

		return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtFrame::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWidget::ProcessProperty(pProperty);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for dynamic or static
		/*
		*<property name="BORDER_TOPLEFT_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_TOPRIGHT_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_BOTTOMRIGHT_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_BOTTOMLEFT_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_TOP_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_LEFT_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_BOTTOM_DYNAMIC" type="BOOL" value="true" />
		*<property name="BORDER_RIGHT_DYNAMIC" type="BOOL" value="true" />
		*/
		if( pProperty->GetName() == "BORDER_TOPLEFT_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_TOPLEFT,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_TOPRIGHT_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_TOPRIGHT,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_BOTTOMRIGHT_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_BOTTOMRIGHT,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_BOTTOMLEFT_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_BOTTOMLEFT,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_TOP_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_TOP,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_LEFT_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_LEFT,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_BOTTOM_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_BOTTOM,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "BORDER_RIGHT_DYNAMIC" && pProperty->GetType()=="BOOL")
		{
			SetBorderDynamic(FRAME_BORDER_RIGHT,CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

