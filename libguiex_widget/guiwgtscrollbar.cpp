/** 
* @file guiwgtscrollbar.cpp
* @brief scrollbar
* @author ken
* @date 2006-08-07
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbutton.h"
#include "guiwgtscrollbar.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiwidgetmanager.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtScrollbarArrow
	* @brief scrollbar's button
	* used image name: (Same as button)
	*		-normal
	*		-hover
	*		-disable
	*		-push
	*		-hover_overlay
	*		-push_overlay
	*/ 
	class GUIEXPORT CGUIWgtScrollbarArrow : public CGUIWgtButton
	{
	public:
		CGUIWgtScrollbarArrow( const CGUIString& rName,const CGUIString& rSceneName, int nType );

	protected:
		void InitScrollbarArrow();

	protected:	//!< callback function
		virtual uint32 OnParentChanged( CGUIEventRelativeChange* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );

	protected:
		int m_nArrowType;		/// arrow type
		CGUIWgtScrollbar* m_pScrollbar;

	private:
		static CGUIString ms_strType;
	};


	/**
	* @class CGUIWgtScrollbarSlide
	* @brief scrollbar's slide
	* used image name: (Same as button)
	*		-normal
	*		-hover
	*		-disable
	*		-push
	*		-hover_overlay
	*		-push_overlay
	*/ 
	class GUIEXPORT CGUIWgtScrollbarSlide : public CGUIWgtButton
	{
	public:
		CGUIWgtScrollbarSlide( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		void InitScrollbarSlide();

	protected:	//!< callback function
		virtual uint32 OnParentChanged( CGUIEventRelativeChange* pEvent );
		virtual uint32 OnDragBegin( CGUIEventDrag* pEvent );
		virtual uint32 OnDragProcess( CGUIEventDrag* pEvent );
		virtual uint32 OnDragEnd( CGUIEventDrag* pEvent );

	protected:
		CGUIWgtScrollbar* m_pScrollbar;

	private:
		static CGUIString ms_strType;
	};
}//namespace guiex



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtScrollbarArrow::ms_strType = "CGUIWgtScrollbarArrow";
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarArrow::CGUIWgtScrollbarArrow( const CGUIString& rName, const CGUIString& rSceneName, int nType )
		:CGUIWgtButton(ms_strType, rName, rSceneName)
		,m_nArrowType(nType)
		,m_pScrollbar(NULL)
	{
		InitScrollbarArrow();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarArrow::InitScrollbarArrow()
	{
		SetFocusAgency( true );
		SetGenerateParentChildEvent( true );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarArrow::OnParentChanged( CGUIEventRelativeChange* pEvent )
	{
		m_pScrollbar = static_cast<CGUIWgtScrollbar*>(pEvent->GetRelative());
		return CGUIWgtButton::OnParentChanged(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarArrow::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		GUI_ASSERT( m_pScrollbar, "haven't parent" );

		if( IsDisable() )
		{
			return CGUIWgtButton::OnMouseLeftDown(pEvent);
		}

		switch( m_nArrowType)
		{
		case 0:
			m_pScrollbar->DecreasePos();
			break;
		case 1:
			m_pScrollbar->IncreasePos();
			break;
		default:
			GUI_THROW( "[CGUIWgtScrollbarArrow::OnMouseLeftDown]: unknown scrollbar arrow type!");
			break;
		}

		return CGUIWgtButton::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	CGUIString CGUIWgtScrollbarSlide::ms_strType = "CGUIWgtScrollbarSlide";
	//------------------------------------------------------------------------------
	CGUIWgtScrollbarSlide::CGUIWgtScrollbarSlide( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtButton(ms_strType, rName, rSceneName)
		,m_pScrollbar(NULL)
	{
		InitScrollbarSlide();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarSlide::InitScrollbarSlide()
	{
		SetMovable(true);
		SetFocusAgency( true );
		SetGenerateParentChildEvent( true );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarSlide::OnParentChanged( CGUIEventRelativeChange* pEvent )
	{
		m_pScrollbar = static_cast<CGUIWgtScrollbar*>(pEvent->GetRelative());
		return CGUIWgtButton::OnParentChanged(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarSlide::OnDragBegin( CGUIEventDrag* pEvent )
	{
		return CGUIWidget::OnDragBegin(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarSlide::OnDragProcess( CGUIEventDrag* pEvent )
	{
		GUI_ASSERT( m_pScrollbar, "haven't parent" );

		CGUIRect aSlideWorkArea = m_pScrollbar->GetSlideArea();
		CGUIVector2	aSlidePoint = GetPixelPosition( );
		CGUIVector2	aWidgetPoint = pEvent->GetWidgetLocalPos();
		CGUIVector2	aMovePos;		/// wanted position
		CGUISize aSlideSize = GetPixelSize();
		uint32 nCurScrollbarPos = 0;	/// the value of scrollbar

		switch( m_pScrollbar->GetOrientation() )
		{
		case eOrientation_Horizontal:
			aSlideWorkArea.m_fLeft += aSlideSize.m_fWidth / 2;
			aSlideWorkArea.m_fRight -= aSlideSize.m_fWidth / 2;

			//get position of slide
			aMovePos.y = aSlidePoint.y;	//fixed
			aMovePos.x = aWidgetPoint.x;
			aMovePos.x = aMovePos.x>aSlideWorkArea.m_fRight?aSlideWorkArea.m_fRight:aMovePos.x;
			aMovePos.x = aMovePos.x<aSlideWorkArea.m_fLeft?aSlideWorkArea.m_fLeft:aMovePos.x;

			//get value of scrollbar
			nCurScrollbarPos = 
				m_pScrollbar->GetMinPos()+
				GUI_FLOAT2UINT_ROUND((aMovePos.x -aSlideWorkArea.m_fLeft)  * m_pScrollbar->GetRange() / aSlideWorkArea.GetWidth());
			break;

		case eOrientation_Vertical:
			aSlideWorkArea.m_fTop += aSlideSize.m_fHeight / 2;
			aSlideWorkArea.m_fBottom -= aSlideSize.m_fHeight / 2;

			aMovePos.x = aSlidePoint.x;	//fixed
			aMovePos.y = aWidgetPoint.y;
			aMovePos.y = aMovePos.y>aSlideWorkArea.m_fBottom?aSlideWorkArea.m_fBottom:aMovePos.y;
			aMovePos.y = aMovePos.y<aSlideWorkArea.m_fTop?aSlideWorkArea.m_fTop:aMovePos.y;

			//get value of scrollbar
			nCurScrollbarPos = 
				m_pScrollbar->GetMinPos()+
				GUI_FLOAT2UINT_ROUND((aMovePos.y-aSlideWorkArea.m_fTop) * m_pScrollbar->GetRange() / aSlideWorkArea.GetHeight());
			break;
		}

		// set current position of scrollbar
		m_pScrollbar->SetCurrentPos(nCurScrollbarPos);

		// set slide position
		pEvent->SetWidgetLocalPos(aMovePos);

		return CGUIWidget::OnDragProcess(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbarSlide::OnDragEnd( CGUIEventDrag* pEvent )
	{
		CGUIWgtScrollbar* pParent = static_cast<CGUIWgtScrollbar*>(GetParent());
		pParent->SetCurrentPos(pParent->GetCurrentPos());

		return CGUIWidget::OnDragEnd(pEvent);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtScrollbar);
	//------------------------------------------------------------------------------
	CGUIWgtScrollbar::CGUIWgtScrollbar(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitScrollbar();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbar::CGUIWgtScrollbar( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitScrollbar();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbar::~CGUIWgtScrollbar()
	{
		m_pArrow[0]->SetParent(NULL);
		delete m_pArrow[0];
		m_pArrow[0] = NULL;

		m_pArrow[1]->SetParent(NULL);
		delete m_pArrow[1] ;
		m_pArrow[1] = NULL;

		m_pSlide->SetParent(NULL);
		delete m_pSlide;
		m_pSlide = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::InitScrollbar()
	{
		//flag
		m_bAutoNotifyParent = false;
		m_eOrientation = eOrientation_Vertical;

		//scrollbar parameter
		m_nMaxPos = 0;
		m_nMinPos = 0;
		m_nCurPos = 0;
		m_nPageSize = 1;

		//image
		m_pImageBg = NULL;

		//create child
		m_pArrow[0] = new CGUIWgtScrollbarArrow(CGUIWidgetManager::MakeInternalName(GetName()+"_0_arrow"), GetSceneName(), 0);
		m_pArrow[0]->SetParent(this);
		m_pArrow[0]->SetPositionType( eScreenValue_Percentage );
		m_pArrow[1] = new CGUIWgtScrollbarArrow(CGUIWidgetManager::MakeInternalName(GetName()+"_1_arrow"), GetSceneName(), 1);
		m_pArrow[1]->SetParent(this);
		m_pArrow[1]->SetPositionType( eScreenValue_Percentage );
		m_pSlide = new CGUIWgtScrollbarSlide(CGUIWidgetManager::MakeInternalName(GetName()+"_slide"), GetSceneName() );
		m_pSlide->SetParent(this);
		m_pSlide->SetPositionType( eScreenValue_Percentage );

		SetFocusable( true );
		SetGenerateClickEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::SetOrientation( EOrientation eScrollbarType )
	{
		m_eOrientation = eScrollbarType;

		RefreshSelf();
	}
	//------------------------------------------------------------------------------
	EOrientation CGUIWgtScrollbar::GetOrientation( ) const
	{
		return m_eOrientation;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "background")
		{
			m_pImageBg = pImage;
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "arrow_0_normal") 
		{
			m_pArrow[0]->SetImage("normal", pImage);
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "arrow_0_hover")
		{
			m_pArrow[0]->SetImage("hover", pImage);
		}
		else if( rName == "arrow_0_disable")
		{
			m_pArrow[0]->SetImage("disable", pImage);
		}
		else if( rName == "arrow_0_push")
		{
			m_pArrow[0]->SetImage("push", pImage);
		}
		else if( rName == "arrow_0_hover_overlay")
		{
			m_pArrow[0]->SetImage("hover_overlay", pImage);
		}
		else if( rName == "arrow_0_push_overlay")
		{
			m_pArrow[0]->SetImage("push_overlay", pImage);
		}
		else if( rName == "arrow_1_normal")
		{
			m_pArrow[1]->SetImage("normal", pImage);
		}
		else if( rName == "arrow_1_hover")
		{
			m_pArrow[1]->SetImage("hover", pImage);
		}
		else if( rName == "arrow_1_disable")
		{
			m_pArrow[1]->SetImage("disable", pImage);
		}
		else if( rName == "arrow_1_push")
		{
			m_pArrow[1]->SetImage("push", pImage);
		}
		else if( rName == "arrow_1_hover_overlay")
		{
			m_pArrow[1]->SetImage("hover_overlay", pImage);
		}
		else if( rName == "arrow_1_push_overlay")
		{
			m_pArrow[1]->SetImage("push_overlay", pImage);
		}
		else if( rName == "slide_normal")
		{
			m_pSlide->SetImage("normal", pImage);
		}
		else if( rName == "slide_hover")
		{
			m_pSlide->SetImage("hover", pImage);
		}
		else if( rName == "slide_disable")
		{
			m_pSlide->SetImage("disable", pImage);
		}
		else if( rName == "slide_push")
		{
			m_pSlide->SetImage("push", pImage);
		}
		else if( rName == "slide_hover_overlay")
		{
			m_pSlide->SetImage("hover_overlay", pImage);
		}
		else if( rName == "slide_push_overlay")
		{
			m_pSlide->SetImage("push_overlay", pImage);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::OnCreate()
	{
		CGUIWidget::OnCreate();

		//for child
		m_pArrow[0]->Create();
		m_pArrow[1]->Create();
		m_pSlide->Create();
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::RenderSelf(IGUIInterfaceRender* pRender)
	{
		CGUIWidget::RenderSelf( pRender );

		if( m_pImageBg )
		{
			DrawImage( pRender, m_pImageBg, GetBoundArea( ) );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::GenerateNotifyEvent()
	{
		CGUIEventScrollbar aEvent;
		aEvent.SetEventId(eEVENT_SCROLLBAR_SCROLL);
		aEvent.SetOrientation(m_eOrientation );
		aEvent.SetScrollbar(this);

		if( IsNotifyParent() && GetParent())
		{
			aEvent.SetReceiver(GetParent());
		}
		else
		{
			aEvent.SetReceiver(this);
		}

		aEvent.SetCurrentPos( m_nCurPos );
		aEvent.SetRange( m_nMaxPos - m_nMinPos );

		GSystem->SendEvent( &aEvent);
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIWgtScrollbar::GetSlideArea( )
	{
		CGUIRect aRect = GetBoundArea();
		aRect.SetPosition(CGUIVector2(0.0f, 0.0f));
		CGUISize aSize0 = m_pArrow[0]->GetPixelSize();
		CGUISize aSize1 = m_pArrow[1]->GetPixelSize();
		if( m_eOrientation == eOrientation_Vertical )
		{
			aRect.m_fTop += aSize0.GetHeight();
			aRect.m_fBottom -= aSize1.GetHeight();
		}
		else if( m_eOrientation ==  eOrientation_Horizontal )
		{
			aRect.m_fLeft += aSize0.GetWidth();
			aRect.m_fRight -= aSize1.GetWidth();
		}
		else
		{

		}
		return aRect;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::AdjustScrollbarPosAndSize()
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		AdjustScrollbarPosAndSize();

		//set children's pos
		switch(m_eOrientation)
		{
		case eOrientation_Vertical:
			m_pArrow[0]->SetAnchorPoint(0.5f, 0.0f);
			m_pArrow[0]->SetPosition(0.5f, 0.0f);
			m_pArrow[1]->SetAnchorPoint(0.5f, 1.0f);
			m_pArrow[1]->SetPosition(0.5f, 1.0f);
			m_pSlide->SetAnchorPoint(0.5f, 0.5f);
			break;

		case eOrientation_Horizontal:
			m_pArrow[0]->SetAnchorPoint(0.0f, 0.5f);
			m_pArrow[0]->SetPosition(0.0f, 0.5f);
			m_pArrow[1]->SetAnchorPoint(1.0f, 0.5f);
			m_pArrow[1]->SetPosition(1.0f, 0.5f);
			m_pSlide->SetAnchorPoint(0.5f, 0.5f);
			break;
		}

		//enable or disable arrow
		m_pArrow[0]->SetDisable( m_nCurPos <= m_nMinPos );
		m_pArrow[1]->SetDisable( m_nCurPos >= m_nMaxPos );

		//show or hide slide
		if( m_nMinPos == m_nMaxPos )
		{
			m_pSlide->SetVisible(false);
		}
		else
		{
			m_pSlide->SetVisible(true);
		}

		//update position
		if( m_nMaxPos <= m_nMinPos || m_nCurPos < m_nMinPos ||m_nCurPos > m_nMaxPos)
		{
			return;
		}

		CGUIRect aSlideArea = GetSlideArea();
		CGUISize aSlideSize = m_pSlide->GetPixelSize();
		switch( m_eOrientation )
		{
			//for vertical scroll bar
		case eOrientation_Vertical:
			{
				aSlideArea.m_fTop += aSlideSize.m_fHeight / 2;
				aSlideArea.m_fBottom -= aSlideSize.m_fHeight / 2;
				real fGap = (m_nCurPos -m_nMinPos) * aSlideArea.GetHeight()/GetRange();
				m_pSlide->SetPixelPosition( aSlideArea.m_fLeft + aSlideArea.GetWidth()/2,aSlideArea.m_fTop + fGap);
			}
			break;

			//for horizontal scroll bar
		case eOrientation_Horizontal:
			{
				aSlideArea.m_fLeft += aSlideSize.m_fWidth / 2;
				aSlideArea.m_fRight -= aSlideSize.m_fWidth / 2;
				real fGap = (m_nCurPos -m_nMinPos) * aSlideArea.GetWidth()/GetRange();
				m_pSlide->SetPixelPosition(aSlideArea.m_fLeft + fGap,aSlideArea.m_fTop + aSlideArea.GetHeight()/2);
			}
			break;
		}
	}
	//------------------------------------------------------------------------------
	///set page size
	void CGUIWgtScrollbar::SetPageSize(uint32 nPageSize)
	{
		m_nPageSize = nPageSize;
	}
	//------------------------------------------------------------------------------
	///get page size
	uint32 CGUIWgtScrollbar::GetPageSize() const
	{
		return m_nPageSize;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set range of scrollbar
	*/
	void CGUIWgtScrollbar::SetRange( uint32 nRange )
	{
		bool bChanged = false;
		if( m_nMinPos != 0 )
		{
			m_nMinPos = 0;
			bChanged = true;
		}
		if( m_nMaxPos != nRange )
		{
			m_nMaxPos = nRange;
			bChanged = true;
		}
		uint32 nOldPos = m_nCurPos;
		m_nCurPos = m_nCurPos<m_nMinPos ? m_nMinPos : m_nCurPos;
		m_nCurPos = m_nCurPos>m_nMaxPos ? m_nMaxPos : m_nCurPos;
		if( nOldPos != m_nCurPos )
		{
			bChanged = true;
		}

		//update scrollbar value
		if( bChanged )
		{
			GenerateNotifyEvent();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get range of scrollbar
	*/
	uint32 CGUIWgtScrollbar::GetRange(  ) const
	{
		return (GetMaxPos() - GetMinPos());
	}
	//------------------------------------------------------------------------------
	///get minimum position of scrollbar slide
	uint32 CGUIWgtScrollbar::GetMinPos() const
	{
		return m_nMinPos;
	}
	//------------------------------------------------------------------------------
	///set minimum position of scrollbar slide
	void CGUIWgtScrollbar::SetMinPos(uint32 nMinPos)
	{
		//set value
		uint32 nOldMinPos = m_nMinPos;
		uint32 nOldPos = m_nCurPos;

		m_nMinPos = nMinPos<m_nMaxPos ? nMinPos : m_nMaxPos;
		m_nCurPos = m_nCurPos<m_nMinPos ? m_nMinPos : m_nCurPos;

		//update scrollbar value
		if( nOldPos != m_nCurPos || nOldMinPos != m_nMinPos )
		{
			GenerateNotifyEvent();
		}
	}
	//------------------------------------------------------------------------------
	///get maximum position of scrollbar slide
	uint32 CGUIWgtScrollbar::GetMaxPos() const
	{
		return m_nMaxPos;
	}
	//------------------------------------------------------------------------------
	///set maximum position of scrollbar slide
	void CGUIWgtScrollbar::SetMaxPos(uint32 nMaxPos)
	{
		//set value
		uint32 nOldMaxPos = m_nMaxPos;
		uint32 nOldPos = m_nCurPos;

		//set value
		m_nMaxPos = nMaxPos<m_nMinPos ? m_nMinPos : nMaxPos;
		m_nCurPos = m_nCurPos>m_nMaxPos ? m_nMaxPos : m_nCurPos;

		//update scrollbar value
		if( nOldPos != m_nCurPos || nOldMaxPos != m_nMaxPos )
		{
			GenerateNotifyEvent();
		}
	}
	//------------------------------------------------------------------------------
	///get current position of scrollbar slide
	uint32	CGUIWgtScrollbar::GetCurrentPos() const
	{
		return m_nCurPos;
	}
	//------------------------------------------------------------------------------
	///set current position of scrollbar slide
	void CGUIWgtScrollbar::SetCurrentPos(uint32 nPos)
	{
		if( m_nCurPos != nPos )
		{
			m_nCurPos = nPos;
			m_nCurPos = m_nCurPos>m_nMaxPos ? m_nMaxPos : m_nCurPos;
			m_nCurPos = m_nCurPos<m_nMinPos ? m_nMinPos : m_nCurPos;

			//update scrollbar value
			GenerateNotifyEvent();
		}
	}
	//------------------------------------------------------------------------------
	///increase position of scrollbar slide
	void CGUIWgtScrollbar::IncreasePos()
	{
		SetCurrentPos(GetCurrentPos()+1);
	}
	//------------------------------------------------------------------------------
	///increase position of scrollbar slide
	void CGUIWgtScrollbar::DecreasePos()
	{
		if( GetCurrentPos() > 0 )
		{
			SetCurrentPos(GetCurrentPos()-1);
		}
	}
	//------------------------------------------------------------------------------
	///whether notify parent when scroll value change
	void CGUIWgtScrollbar::SetNotifyParent(bool bEnable)
	{
		m_bAutoNotifyParent = bEnable;
	}
	//------------------------------------------------------------------------------
	///whether notify parent when scroll value change
	bool CGUIWgtScrollbar::IsNotifyParent() const
	{
		return m_bAutoNotifyParent;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbar::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		if( IsDisable() )
		{
			return CGUIWidget::OnMouseLeftDown(pEvent);
		}

		CGUIVector2	aMousePoint = pEvent->GetLocalPosition();
		CGUIVector2	aSlidePoint = m_pSlide->GetPixelPosition( );

		switch( m_eOrientation )
		{
		case eOrientation_Horizontal:
			if( aMousePoint.x < aSlidePoint.x)
			{
				SetCurrentPos(GetCurrentPos() > GetPageSize()?GetCurrentPos()-GetPageSize():0);
			}
			else
			{
				SetCurrentPos(GetCurrentPos()+GetPageSize());
			}
			break;

		case eOrientation_Vertical:
			if( aMousePoint.y < aSlidePoint.y)
			{
				SetCurrentPos(GetCurrentPos() > GetPageSize()?GetCurrentPos()-GetPageSize():0);
			}
			else
			{
				SetCurrentPos(GetCurrentPos()+GetPageSize());
			}
			break;
		}

		return CGUIWidget::OnMouseLeftDown(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtScrollbar::OnScrollbarScroll( CGUIEventScrollbar* pEvent )
	{
		Refresh();
		return CGUIWidget::OnScrollbarScroll( pEvent );
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtScrollbar::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_Orientation && rProperty.GetName() == "orientation" )
		{
			ValueToProperty( m_eOrientation, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "min_pos" )
		{
			ValueToProperty( m_nMinPos, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "max_pos" )
		{
			ValueToProperty( m_nMaxPos, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "current_pos" )
		{
			ValueToProperty( m_nCurPos, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "page_size" )
		{
			ValueToProperty( m_nPageSize, rProperty );
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::ProcessProperty( const CGUIProperty& rProperty)
	{
		if( rProperty.GetType() == ePropertyType_Orientation && rProperty.GetName() == "orientation")
		{
			EOrientation eOrientation;
			PropertyToValue( rProperty, eOrientation);
			SetOrientation( eOrientation );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "min_pos" )
		{
			uint32 uValue;
			PropertyToValue( rProperty, uValue );
			SetMinPos( uValue );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "max_pos" )
		{
			uint32 uValue;
			PropertyToValue( rProperty, uValue );
			SetMaxPos( uValue );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "current_pos" )
		{
			uint32 uValue;
			PropertyToValue( rProperty, uValue );
			SetCurrentPos( uValue );
		}
		else if( rProperty.GetType() == ePropertyType_UInt32 && rProperty.GetName() == "page_size" )
		{
			uint32 uValue;
			PropertyToValue( rProperty, uValue );
			SetPageSize( uValue );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex

