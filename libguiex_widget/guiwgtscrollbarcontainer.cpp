/** 
* @file guiwidgetscrollbarcontainer.cpp
* @brief widget: scrollbar container
* @author ken
* @date 2007-05-18
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtscrollbarcontainer.h"
#include "guiwgtscrollbar.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guistringconvertor.h>

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
		m_pScrollbarVert->SetParent(NULL);
		delete m_pScrollbarVert;
		m_pScrollbarVert = NULL;

		m_pScrollbarHorz->SetParent(NULL);
		delete m_pScrollbarHorz;
		m_pScrollbarHorz = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::InitScrollbarContainer()
	{
		m_nPixelPerVertValue = 1;
		m_nPixelPerHorzValue = 1;

		//create scrollbar
		m_pScrollbarVert = new CGUIWgtScrollbar(CGUIWidgetManager::MakeInternalName(GetName()+"_scrollbar_vert"), GetSceneName());
		m_pScrollbarVert->SetParent(this);
		m_pScrollbarVert->SetNotifyParent(true);
		m_pScrollbarVert->SetPositionType( eScreenValue_Percentage );
		m_pScrollbarVert->SetAnchorPoint( 1.0f, 0.0f );
		m_pScrollbarVert->SetPosition( 1.0f, 0.0f );
		m_pScrollbarVert->SetFocusAgency( true );
		m_pScrollbarVert->SetOrientation(eOrientation_Vertical);
		m_pScrollbarVert->SetVisible( false );

		m_pScrollbarHorz = new CGUIWgtScrollbar(CGUIWidgetManager::MakeInternalName(GetName()+"_scrollbar_hort"), GetSceneName());
		m_pScrollbarHorz->SetParent(this);
		m_pScrollbarHorz->SetNotifyParent(true);
		m_pScrollbarHorz->SetPositionType( eScreenValue_Percentage );
		m_pScrollbarHorz->SetAnchorPoint( 0.0f, 1.0f );
		m_pScrollbarHorz->SetPosition( 0.0f, 1.0f );
		m_pScrollbarHorz->SetFocusAgency( true );
		m_pScrollbarHorz->SetOrientation(eOrientation_Horizontal);
		m_pScrollbarHorz->SetVisible( false );

		//set flag
		SetFocusable( true );
		SetClipChildren( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::OnCreate()
	{
		CGUIWidget::OnCreate();

		//create scrollbar
		m_pScrollbarHorz->Create();
		m_pScrollbarVert->Create();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName.find( "scrollbar_v_" ) == 0 )
		{
			m_pScrollbarVert->SetImage( rName.substr(strlen("scrollbar_v_")), pImage );
		}
		else if( rName.find( "scrollbar_h_" ) == 0 )
		{
			m_pScrollbarHorz->SetImage( rName.substr(strlen("scrollbar_v_")), pImage );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderSelf( pRender );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::RenderExtraSelfInfo(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderExtraSelfInfo( pRender );

		//draw client area
		pRender->DrawRect( GetVirtualClientArea(), 1.0f, 0.0f, CGUIColor( 0.f,1.f,1.f,0.8f) );
	}

	//------------------------------------------------------------------------------
	/** 
	* @brief Return whether the vertical scroll bar is always shown.
	*/
	bool CGUIWgtScrollbarContainer::IsVertScrollbarShow(void) const
	{
		return m_pScrollbarVert->IsVisible( );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set whether the vertical scroll bar should always be shown.
	*/
	void CGUIWgtScrollbarContainer::ShowVertScrollbar(bool bShow)
	{
		if (IsVertScrollbarShow() != bShow)
		{
			m_pScrollbarVert->SetVisible( bShow );
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return whether the horizontal scroll bar is always shown.
	*/
	bool CGUIWgtScrollbarContainer::IsHorzScrollbarShow(void) const
	{
		return m_pScrollbarHorz->IsVisible( );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Set whether the horizontal scroll bar should always be shown.
	*/
	void CGUIWgtScrollbarContainer::ShowHorzScrollbar(bool bShow)
	{
		if (IsHorzScrollbarShow() != bShow)
		{
			m_pScrollbarHorz->SetVisible( bShow );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetPixelPerVertValue( uint32 nPixelPerValue )
	{
		m_nPixelPerVertValue = nPixelPerValue;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetPixelPerVertValue( ) const
	{
		return m_nPixelPerVertValue;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetPixelPerHorzValue( uint32 nPixelPerValue )
	{
		m_nPixelPerHorzValue = nPixelPerValue;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetPixelPerHorzValue( ) const
	{
		return m_nPixelPerHorzValue;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetVertRange( uint32 nRange )
	{
		m_pScrollbarVert->SetRange( nRange );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetVertRange( ) const
	{
		return m_pScrollbarVert->GetRange();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetHorzRange( uint32 nRange )
	{
		m_pScrollbarHorz->SetRange( nRange );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetHorzRange( ) const
	{
		return m_pScrollbarHorz->GetRange();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetVertPos( uint32 nPos )
	{
		m_pScrollbarVert->SetCurrentPos( nPos );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetHorzPos( uint32 nPos )
	{
		m_pScrollbarHorz->SetCurrentPos( nPos );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetVertPos( ) const
	{
		return m_pScrollbarVert->GetCurrentPos();
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetHorzPos( ) const
	{
		return m_pScrollbarHorz->GetCurrentPos();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetVertPageSize( uint32 nPage )
	{
		m_pScrollbarVert->SetPageSize( nPage );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetVertPageSize( ) const
	{
		return m_pScrollbarVert->GetPageSize();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::SetHorzPageSize( uint32 nPage )
	{
		m_pScrollbarHorz->SetPageSize( nPage );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::GetHorzPageSize( ) const
	{
		return m_pScrollbarHorz->GetPageSize();
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CGUIWgtScrollbarContainer::GetVirtualClientArea() const
	{
		return m_aVirtualClientArea;
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CGUIWgtScrollbarContainer::GetVisibleClientArea() const
	{
		return m_aVisibleClientArea;
	}
	//------------------------------------------------------------------------------
	const CGUIRect*	CGUIWgtScrollbarContainer::GetClipArea() const
	{
		return &m_aVisibleClientArea;
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtScrollbarContainer::IsAddChildToTail() const
	{
		return false;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		CGUISize aVirtualClientAreaSize = GetClientArea().GetSize();
		CGUIVector2 aVirtualClientAreaPos = GetClientArea().GetPosition();
		m_aVisibleClientArea = GetClientArea();

		//client area size
		if( IsVertScrollbarShow() )
		{
			m_aVisibleClientArea.m_fRight -= m_pScrollbarVert->GetPixelSize().GetWidth();
			aVirtualClientAreaSize.m_fWidth -= m_pScrollbarVert->GetPixelSize().GetWidth();
			aVirtualClientAreaSize.m_fHeight += m_pScrollbarVert->GetRange() * GetPixelPerVertValue();
		}
		if( IsHorzScrollbarShow() )
		{
			m_aVisibleClientArea.m_fBottom -= m_pScrollbarHorz->GetPixelSize().GetHeight();
			aVirtualClientAreaSize.m_fHeight -= m_pScrollbarHorz->GetPixelSize().GetHeight();
			aVirtualClientAreaSize.m_fWidth += m_pScrollbarHorz->GetRange() * GetPixelPerHorzValue();
		}

		//client area position
		aVirtualClientAreaPos.x -=  m_pScrollbarHorz->GetCurrentPos()*GetPixelPerHorzValue();
		aVirtualClientAreaPos.y -= m_pScrollbarVert->GetCurrentPos()*GetPixelPerVertValue();

		//set virtual area rect
		m_aVirtualClientArea.SetPosition( aVirtualClientAreaPos );
		m_aVirtualClientArea.SetSize( aVirtualClientAreaSize );

		//update scrollbar's size
		if( IsVertScrollbarShow() )
		{
			real fHeight = IsHorzScrollbarShow() ? GetClientArea().GetHeight() - m_pScrollbarHorz->GetPixelSize().GetHeight() : GetClientArea().GetHeight();
			m_pScrollbarVert->SetPixelSize( m_pScrollbarVert->GetPixelSize().GetWidth(), fHeight);
		}
		if( IsHorzScrollbarShow() )
		{
			real fWidth = IsVertScrollbarShow() ? GetClientArea().GetWidth() - m_pScrollbarVert->GetPixelSize().GetWidth() : GetClientArea().GetWidth();
			m_pScrollbarHorz->SetPixelSize( fWidth, m_pScrollbarHorz->GetPixelSize().GetHeight() );
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarContainer::OnScrollbarScroll( CGUIEventScrollbar* pEvent )
	{
		Refresh();
		return CGUIWidget::OnScrollbarScroll( pEvent);
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtScrollbarContainer::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_UInt32&& rProperty.GetName() == "pixel_per_vert" )
		{
			ValueToProperty( GetPixelPerVertValue(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32&& rProperty.GetName() == "pixel_per_horz" )
		{
			ValueToProperty( GetPixelPerHorzValue(), rProperty );
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::ProcessProperty( const CGUIProperty& rProperty)
	{
		if( rProperty.GetType() == ePropertyType_UInt32&& rProperty.GetName() == "pixel_per_vert" )
		{
			uint32 uValue = 1;
			PropertyToValue( rProperty, uValue );
			SetPixelPerVertValue( uValue );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32&& rProperty.GetName() == "pixel_per_horz" )
		{
			uint32 uValue = 1;
			PropertyToValue( rProperty, uValue );
			SetPixelPerHorzValue( uValue );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
