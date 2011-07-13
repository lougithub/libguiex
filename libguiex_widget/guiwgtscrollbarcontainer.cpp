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
	class GUIEXPORT CGUIWgtScrollbarForContainer : public CGUIWgtScrollbar
	{
	public:
		CGUIWgtScrollbarForContainer( const CGUIString& rName,const CGUIString& rSceneName );

	protected:
		void InitScrollbarForContainer();
		virtual bool IsIgnoreParentClipRect() const;
		virtual void AdjustScrollbarPosAndSize();

	protected:	//!< callback function
		virtual uint32 OnParentChanged( CGUIEventRelativeChange* pEvent );

	protected:
		CGUIWgtScrollbarContainer* m_pContainer;
		CGUIMatrix4 aOffsetMatrix;

	private:
		static CGUIString ms_strType;
	};
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtScrollbarForContainer::ms_strType = "CGUIWgtScrollbarForContainer";
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarForContainer::CGUIWgtScrollbarForContainer( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtScrollbar(ms_strType, rName, rSceneName)
		,m_pContainer(NULL)
	{
		InitScrollbarForContainer();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarForContainer::InitScrollbarForContainer()
	{
		SetGenerateParentChildEvent( true );
		SetNotifyParent(true);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarForContainer::OnParentChanged( CGUIEventRelativeChange* pEvent )
	{
		m_pContainer = static_cast<CGUIWgtScrollbarContainer*>(pEvent->GetRelative());
		return CGUIWgtScrollbar::OnParentChanged(pEvent);
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtScrollbarForContainer::IsIgnoreParentClipRect() const
	{
		return true;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarForContainer::AdjustScrollbarPosAndSize()
	{
		if( !m_pContainer )
		{
			return;
		}
		if( m_eOrientation == eOrientation_Vertical )
		{
			SetPixelPosition(
				m_pContainer->GetVisibleClientArea().m_fRight - m_pContainer->GetClientArea().m_fLeft,
				m_pContainer->GetVisibleClientArea().m_fTop - m_pContainer->GetClientArea().m_fTop );
		}
		else
		{
			SetPixelPosition(
				m_pContainer->GetVisibleClientArea().m_fLeft - m_pContainer->GetClientArea().m_fLeft, 
				m_pContainer->GetVisibleClientArea().m_fBottom - m_pContainer->GetClientArea().m_fTop );
		}
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	//GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtScrollbarContainer);
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

		m_bForceShowVertScrollbar = false;
		m_bForceShowHorzScrollbar = false;

		//create scrollbar
		m_pScrollbarVert = new CGUIWgtScrollbarForContainer(CGUIWidgetManager::MakeInternalName(GetName()+"_scrollbar_vert"), GetSceneName());
		m_pScrollbarVert->SetParent(this);
		m_pScrollbarVert->SetFocusAgency( true );
		m_pScrollbarVert->SetOrientation(eOrientation_Vertical);
		m_pScrollbarVert->SetVisible( false );

		m_pScrollbarHorz = new CGUIWgtScrollbarForContainer(CGUIWidgetManager::MakeInternalName(GetName()+"_scrollbar_hort"), GetSceneName());
		m_pScrollbarHorz->SetParent(this);
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
	void CGUIWgtScrollbarContainer::ForceShowVertScrollbar( bool bForce )
	{
		if( m_bForceShowVertScrollbar != bForce )
		{
			m_bForceShowVertScrollbar = bForce;

			if( bForce )
			{
				ShowVertScrollbar( true );
			}
			Refresh();
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtScrollbarContainer::IsForceShowVertScrollbar() const
	{
		return m_bForceShowVertScrollbar;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarContainer::ForceShowHorzScrollbar( bool bForce )
	{
		if( m_bForceShowHorzScrollbar != bForce )
		{
			m_bForceShowHorzScrollbar = bForce;

			if( bForce )
			{
				ShowHorzScrollbar( true );
			}
			Refresh();
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWgtScrollbarContainer::IsForceShowHorzScrollbar() const
	{
		return m_bForceShowHorzScrollbar;
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

		CGUIVector2 aVirtualClientPos = GetClientArea().GetPosition();
		CGUISize aDesiredVirtualClientSize = GetDesiredVirtualClientSize();
		m_aVisibleClientArea = GetClientArea();

		if( IsVertScrollbarShow() )
		{
			m_aVisibleClientArea.m_fRight -= m_pScrollbarVert->GetPixelSize().GetWidth();
		}
		if( IsHorzScrollbarShow() )
		{
			m_aVisibleClientArea.m_fBottom -= m_pScrollbarHorz->GetPixelSize().GetHeight();
		}

		//virtual client size
		if( aDesiredVirtualClientSize.GetWidth() < m_aVisibleClientArea.GetWidth() )
		{
			aDesiredVirtualClientSize.SetWidth( m_aVisibleClientArea.GetWidth());
		}
		if( aDesiredVirtualClientSize.GetHeight() < m_aVisibleClientArea.GetHeight() )
		{
			aDesiredVirtualClientSize.SetHeight( m_aVisibleClientArea.GetHeight());
		}

		//client area position
		aVirtualClientPos.x -=  m_pScrollbarHorz->GetCurrentPos()*GetPixelPerHorzValue();
		aVirtualClientPos.y -= m_pScrollbarVert->GetCurrentPos()*GetPixelPerVertValue();

		//set virtual area rect
		m_aClientArea.SetPosition( aVirtualClientPos );
		m_aClientArea.SetSize( aDesiredVirtualClientSize );

		//update scrollbar's size
		m_pScrollbarVert->SetPixelSize( m_pScrollbarVert->GetPixelSize().GetWidth(), m_aVisibleClientArea.GetHeight());
		m_pScrollbarHorz->SetPixelSize( m_aVisibleClientArea.GetWidth(), m_pScrollbarHorz->GetPixelSize().GetHeight() );

		//update vertical scrollbar range
		if( aDesiredVirtualClientSize.GetHeight() > GetVisibleClientArea().GetHeight() )
		{
			ShowVertScrollbar( true );
			real fPixelDif = aDesiredVirtualClientSize.GetHeight() - GetVisibleClientArea().GetHeight();
			SetVertRange( GUI_FLOAT2UINT_ROUND(ceil(fPixelDif / GetPixelPerVertValue())));
		}
		else
		{
			if( IsForceShowVertScrollbar() )
			{
				SetVertRange( 0 );
			}
			else
			{
				ShowVertScrollbar( false );
			}
		}

		//update horizontal scrollbar range
		if( aDesiredVirtualClientSize.GetWidth() > GetVisibleClientArea().GetWidth() )
		{
			ShowHorzScrollbar( true );
			real fPixelDif = aDesiredVirtualClientSize.GetWidth() - GetVisibleClientArea().GetWidth();
			SetHorzRange( GUI_FLOAT2UINT_ROUND(ceil(fPixelDif / GetPixelPerHorzValue())));
		}
		else
		{
			if(IsForceShowHorzScrollbar() )
			{
				SetHorzRange( 0 );
			}
			else
			{
				ShowHorzScrollbar( false );
			}
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
		else if( rProperty.GetType() == ePropertyType_Bool&& rProperty.GetName() == "force_vert_scrollbar" )
		{
			ValueToProperty( IsForceShowVertScrollbar(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Bool&& rProperty.GetName() == "force_horz_scrollbar" )
		{
			ValueToProperty( IsForceShowHorzScrollbar(), rProperty );
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
		else if( rProperty.GetType() == ePropertyType_Bool&& rProperty.GetName() == "force_vert_scrollbar" )
		{
			bool bForceShow = false;
			PropertyToValue( rProperty, bForceShow );
			ForceShowVertScrollbar( bForceShow );
		}
		else if( rProperty.GetType() == ePropertyType_Bool&& rProperty.GetName() == "force_horz_scrollbar" )
		{
			bool bForceShow = false;
			PropertyToValue( rProperty, bForceShow );
			ForceShowHorzScrollbar( bForceShow );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
