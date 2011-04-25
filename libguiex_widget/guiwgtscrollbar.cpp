/** 
* @file guiwgtscrollbar.cpp
* @brief scrollbar
* @author ken
* @date 2006-08-07
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtbutton.h>
#include <libguiex_widget/guiwgtscrollbar.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiexception.h>
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
		/**
		* @brief constructor
		*/
		CGUIWgtScrollbarSlide( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		///initialize
		void InitScrollbarSlide();

	protected:	//!< callback function
		virtual uint32		OnParentChanged( CGUIEventRelativeChange* pEvent );
		virtual uint32		OnDragBegin( CGUIEventDrag* pEvent );
		virtual uint32		OnDragProcess( CGUIEventDrag* pEvent );
		virtual uint32		OnDragEnd( CGUIEventDrag* pEvent );

	protected:
		CGUIWgtScrollbar* m_pScrollbar;

	private:
		static CGUIString	ms_strType;
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
		SetGenerateParentChangeEvent( true );
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

		switch( m_nArrowType)
		{
		case 0:
			m_pScrollbar->DecreasePos();
			m_pScrollbar->GenerateNotifyEvent();
			break;
		case 1:
			m_pScrollbar->IncreasePos();
			m_pScrollbar->GenerateNotifyEvent();
			break;
		default:
			throw CGUIException("[CGUIWgtScrollbarArrow::OnMouseLeftDown]: unknown scrollbar arrow type!");
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
		SetGenerateParentChangeEvent( true );
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

		switch( m_pScrollbar->GetScrollbarType() )
		{
		case eSB_HORIZONAL:
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

		case eSB_VERTICAL:
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
		m_pArrow[1]->SetParent(NULL);
		delete m_pArrow[1] ;
		m_pSlide->SetParent(NULL);
		delete m_pSlide;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::InitScrollbar()
	{
		SetPositionType( eScreenValue_Percentage );
		SetSizeType( eScreenValue_Pixel );

		//create child
		m_pArrow[0] = new CGUIWgtScrollbarArrow(GetName()+"_arrow_0" + GUI_INTERNAL_WIDGET_FLAG, GetSceneName(), 0);
		m_pArrow[0]->SetParent(this);
		m_pArrow[0]->SetPositionType( eScreenValue_Percentage );
		m_pArrow[0]->SetSizeType( eScreenValue_Pixel );
		m_pArrow[1] = new CGUIWgtScrollbarArrow(GetName()+"_arrow_1" + GUI_INTERNAL_WIDGET_FLAG, GetSceneName(), 1);
		m_pArrow[1]->SetParent(this);
		m_pArrow[1]->SetPositionType( eScreenValue_Percentage );
		m_pArrow[1]->SetSizeType( eScreenValue_Pixel );
		m_pSlide = new CGUIWgtScrollbarSlide(GetName()+"_slide" + GUI_INTERNAL_WIDGET_FLAG, GetSceneName() );
		m_pSlide->SetParent(this);
		m_pSlide->SetPositionType( eScreenValue_Percentage );
		m_pSlide->SetSizeType( eScreenValue_Pixel );

		//flag
		m_bAutoNotifyParent = false;
		m_bAutoPosition = false;
		m_eScrollbarType = eSB_VERTICAL;

		//scrollbar parameter
		m_nMaxPos = 0;
		m_nMinPos = 0;
		m_nCurPos = 0;
		m_nPageSize = 1;
		m_pWgtHost = NULL;

		//image
		m_pImageBg = NULL;

		SetFocusable( true );
		SetGenerateParentSizeChangeEvent( true );
		SetGenerateClickEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::SetScrollbarHost( CGUIWidget* pWgtHost )
	{
		m_pWgtHost = pWgtHost;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::SetScrollbarType( EScrollbarType eScrollbarType )
	{
		m_eScrollbarType = eScrollbarType;

		UpdateScrollbar();
	}
	//------------------------------------------------------------------------------
	EScrollbarType CGUIWgtScrollbar::GetScrollbarType( ) const
	{
		return m_eScrollbarType;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "SCROLLBAR_BG")
		{
			m_pImageBg = pImage;
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "SCROLLBAR_ARROW_NORMAL_0") 
		{
			m_pArrow[0]->SetImage("normal", pImage);
			if( pImage && GetSize().IsEqualZero() )
			{
				SetPixelSize(pImage->GetSize());
			}
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_0")
		{
			m_pArrow[0]->SetImage("hover", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_DISABLE_0")
		{
			m_pArrow[0]->SetImage("disable", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_0")
		{
			m_pArrow[0]->SetImage("push", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_OVERLAY_0")
		{
			m_pArrow[0]->SetImage("hover_overlay", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_OVERLAY_0")
		{
			m_pArrow[0]->SetImage("push_overlay", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_NORMAL_1")
		{
			m_pArrow[1]->SetImage("normal", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_1")
		{
			m_pArrow[1]->SetImage("hover", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_DISABLE_1")
		{
			m_pArrow[1]->SetImage("disable", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_1")
		{
			m_pArrow[1]->SetImage("push", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_OVERLAY_1")
		{
			m_pArrow[1]->SetImage("hover_overlay", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_OVERLAY_1")
		{
			m_pArrow[1]->SetImage("push_overlay", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_NORMAL")
		{
			m_pSlide->SetImage("normal", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_HOVER")
		{
			m_pSlide->SetImage("hover", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_DISABLE")
		{
			m_pSlide->SetImage("disable", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_PUSH")
		{
			m_pSlide->SetImage("push", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_HOVER_OVERLAY")
		{
			m_pSlide->SetImage("hover_overlay", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_PUSH_OVERLAY")
		{
			m_pSlide->SetImage("push_overlay", pImage);
		}
		else
		{
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
	void CGUIWgtScrollbar::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBg, GetBoundArea( ) );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::GenerateNotifyEvent()
	{
		//send event to self
		{
			CGUIEventScrollbar aEvent;
			aEvent.SetEventId(eEVENT_SCROLLBAR_SCROLL);
			aEvent.SetScrollbarType(m_eScrollbarType == eSB_VERTICAL);
			aEvent.SetScrollbar(this);
			aEvent.SetReceiver(this);
			aEvent.SetCurrentPos( m_nCurPos );
			aEvent.SetRange( m_nMaxPos - m_nMinPos );
			GSystem->SendEvent( &aEvent);
		}

		//send event to parent
		if( IsNotifyParent() && GetParent())
		{
			CGUIEventScrollbar aEvent;
			aEvent.SetEventId(eEVENT_SCROLLBAR_SCROLL);
			aEvent.SetScrollbarType(m_eScrollbarType == eSB_VERTICAL);
			aEvent.SetScrollbar(this);
			aEvent.SetReceiver(GetParent());
			aEvent.SetCurrentPos( m_nCurPos );
			aEvent.SetRange( m_nMaxPos - m_nMinPos );
			GSystem->SendEvent( &aEvent);
		}

		//send event to host
		if( m_pWgtHost )
		{
			CGUIEventScrollbar aEvent;
			aEvent.SetEventId(eEVENT_SCROLLBAR_SCROLL);
			aEvent.SetScrollbarType(m_eScrollbarType == eSB_VERTICAL);
			aEvent.SetScrollbar(this);
			aEvent.SetReceiver(m_pWgtHost);
			aEvent.SetCurrentPos( m_nCurPos );
			aEvent.SetRange( m_nMaxPos - m_nMinPos );
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbar::UpdateValue()
	{
		//enable or disable arrow
		if( m_nCurPos <= m_nMinPos )
		{
			m_pArrow[0]->SetDisable(true);
		}
		else
		{
			m_pArrow[0]->SetDisable(false);
		}
		if( m_nCurPos >= m_nMaxPos )
		{
			m_pArrow[1]->SetDisable(true);
		}
		else
		{
			m_pArrow[1]->SetDisable(false);
		}

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
		switch( m_eScrollbarType )
		{
			//for vertical scroll bar
		case eSB_VERTICAL:
			{
				aSlideArea.m_fTop += aSlideSize.m_fHeight / 2;
				aSlideArea.m_fBottom -= aSlideSize.m_fHeight / 2;
				real fGap = (m_nCurPos -m_nMinPos) * aSlideArea.GetHeight()/GetRange();
				m_pSlide->SetPixelPosition(
					aSlideArea.m_fLeft + aSlideArea.GetWidth()/2,
					aSlideArea.m_fTop + fGap);
			}
			break;

			//for horizonal scroll bar
		case eSB_HORIZONAL:
			{
				aSlideArea.m_fLeft += aSlideSize.m_fWidth / 2;
				aSlideArea.m_fRight -= aSlideSize.m_fWidth / 2;
				real fGap = (m_nCurPos -m_nMinPos) * aSlideArea.GetWidth()/GetRange();
				m_pSlide->SetPixelPosition(
					aSlideArea.m_fLeft + fGap,
					aSlideArea.m_fTop + aSlideArea.GetHeight()/2);
			}
			break;
		}
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIWgtScrollbar::GetSlideArea( )
	{
		CGUIRect aRect = GetBoundArea();
		aRect.SetPosition(CGUIVector2(0.0f, 0.0f));
		CGUISize aSize0 = m_pArrow[0]->GetPixelSize();
		CGUISize aSize1 = m_pArrow[1]->GetPixelSize();
		if( m_eScrollbarType == eSB_VERTICAL )
		{
			aRect.m_fTop += aSize0.GetHeight();
			aRect.m_fBottom -= aSize1.GetHeight();
		}
		else if( m_eScrollbarType ==  eSB_HORIZONAL )
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
	void	CGUIWgtScrollbar::RefreshSelf()
	{
		CGUIWidget::RefreshSelf();

		UpdateScrollbar();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbar::UpdateScrollbar()
	{
		if( !GetParent() )
		{
			return;
		}

		//set children's pos
		switch(m_eScrollbarType)
		{
		case eSB_VERTICAL:
			m_pArrow[0]->SetAnchorPoint(0.5f, 0.0f);
			m_pArrow[0]->SetPosition(0.5f, 0.0f);
			m_pArrow[1]->SetAnchorPoint(0.5f, 1.0f);
			m_pArrow[1]->SetPosition(0.5f, 1.0f);
			m_pSlide->SetAnchorPoint(0.5f, 0.5f);
			break;

		case eSB_HORIZONAL:
			m_pArrow[0]->SetAnchorPoint(0.0f, 0.5f);
			m_pArrow[0]->SetPosition(0.0f, 0.5f);
			m_pArrow[1]->SetAnchorPoint(1.0f, 0.5f);
			m_pArrow[1]->SetPosition(1.0f, 0.5f);
			m_pSlide->SetAnchorPoint(0.5f, 0.5f);
			break;
		}

		//set self's pos
		if( m_bAutoPosition )
		{
			switch(m_eScrollbarType)
			{
			case eSB_VERTICAL:
				SetAnchorPoint(1.0f, 0.0f);
				SetPosition( 1.0f, 0.0f );
				SetSize( GetPixelSize().GetWidth(), GetParent()->GetPixelSize().GetHeight() );
				break;

			case eSB_HORIZONAL:
				SetAnchorPoint(0.0f, 1.0f);
				SetPosition( 0.0f, 1.0f);
				SetSize( GetParent()->GetPixelSize().GetWidth(), GetPixelSize().GetHeight());
				break;
			}
		}

		UpdateValue();
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::SetPageSize(uint32 nPageSize)
	{
		m_nPageSize = nPageSize;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtScrollbar::GetPageSize() const
	{
		return m_nPageSize;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbar::SetRange( uint32 nRange )
	{
		SetMinPos( 0 );
		SetMaxPos( nRange );
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtScrollbar::GetRange(  ) const
	{
		return (GetMaxPos() - GetMinPos());
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtScrollbar::GetMinPos() const
	{
		return m_nMinPos;
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::SetMinPos(uint32 nMinPos)
	{
		//set value
		m_nMinPos = nMinPos<m_nMaxPos ? nMinPos : m_nMaxPos;
		m_nCurPos = m_nCurPos<m_nMinPos ? m_nMinPos : m_nCurPos;

		//update scrollbar value
		UpdateValue();
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtScrollbar::GetMaxPos() const
	{
		return m_nMaxPos;
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::SetMaxPos(uint32 nMaxPos)
	{
		//set value
		m_nMaxPos = nMaxPos<m_nMinPos ? m_nMinPos : nMaxPos;
		m_nCurPos = m_nCurPos>m_nMaxPos ? m_nMaxPos : m_nCurPos;

		//update scrollbar value
		UpdateValue();
	}
	//------------------------------------------------------------------------------
	uint32	CGUIWgtScrollbar::GetCurrentPos() const
	{
		return m_nCurPos;
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::SetCurrentPos(uint32 nPos)
	{
		m_nCurPos = nPos;
		m_nCurPos = m_nCurPos>m_nMaxPos ? m_nMaxPos : m_nCurPos;
		m_nCurPos = m_nCurPos<m_nMinPos ? m_nMinPos : m_nCurPos;

		//update scrollbar value
		UpdateValue();
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::IncreasePos()
	{
		SetCurrentPos(GetCurrentPos()+1);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::DecreasePos()
	{
		SetCurrentPos(GetCurrentPos()-1);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::EnableAutoPosition(bool	bAutoPos)
	{
		m_bAutoPosition = bAutoPos;

		UpdateScrollbar();
	}
	//------------------------------------------------------------------------------
	bool			CGUIWgtScrollbar::IsAutoPosition() const
	{
		return m_bAutoPosition;
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::EnableNotifyParent(bool bEnable)
	{
		m_bAutoNotifyParent = bEnable;
	}
	//------------------------------------------------------------------------------
	bool			CGUIWgtScrollbar::IsNotifyParent() const
	{
		return m_bAutoNotifyParent;
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbar::OnOpen( CGUIEventNotification* pEvent )
	{
		uint32 result = CGUIWidget::OnOpen(pEvent);
		UpdateScrollbar();

		return result;
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbar::OnClose( CGUIEventNotification* pEvent )
	{
		uint32 result = CGUIWidget::OnClose(pEvent);

		return result;
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbar::OnParentSizeChange( CGUIEventSize* pEvent )
	{
		return CGUIWidget::OnParentSizeChange(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbar::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		CGUIVector2	aMousePoint = pEvent->GetLocalPosition();
		CGUIVector2	aSlidePoint = m_pSlide->GetPixelPosition( );

		switch( m_eScrollbarType )
		{
		case eSB_HORIZONAL:
			if( aMousePoint.x < aSlidePoint.x)
			{
				SetCurrentPos(GetCurrentPos() > GetPageSize()?GetCurrentPos()-GetPageSize():0);
			}
			else
			{
				SetCurrentPos(GetCurrentPos()+GetPageSize());
			}
			break;

		case eSB_VERTICAL:
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
	uint32		CGUIWgtScrollbar::OnScrollbarScroll( CGUIEventScrollbar* pEvent )
	{
		return CGUIWidget::OnScrollbarScroll( pEvent );
	}
	//------------------------------------------------------------------------------
	//CGUIProperty*	CGUIWgtScrollbar::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	//{
		//CGUIProperty* pProperty = NULL;
		//
		//if( rName == "RANGE" && rType == "uint32" )
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::UInt32ToString(GetRange( )));
		//}		
		//else if( rName == "SCROLLBAR_TYPE" && rType=="ENUM")
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		GetScrollbarType() == eSB_VERTICAL ? "SB_VERTICAL" : "SB_HORIZONAL");
		//}
		//else if( rName == "AUTO_POSITION" && rType=="BOOL")
		//{
		//	pProperty = CGUIPropertyManager::Instance()->CreateProperty(
		//		rName, 
		//		rType, 
		//		CGUIStringConvertor::BoolToString(IsAutoPosition( )));
		//}
		//return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
	//	return NULL;
	//}
	////------------------------------------------------------------------------------
	//void			CGUIWgtScrollbar::ProcessProperty( const CGUIProperty* pProperty)
	//{
		//CGUIWidget::ProcessProperty(pProperty);

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		////property for parent
		///*
		//*<property name="RANGE" type="uint32" value="20" />
		//*/
		//if( pProperty->GetName() == "RANGE" && pProperty->GetType()=="uint32")
		//{
		//	SetRange(CGUIStringConvertor::StringToUInt32(pProperty->GetValue()));
		//}
		//else if( pProperty->GetName() == "SCROLLBAR_TYPE" && pProperty->GetType()=="ENUM")
		//{
		//	if( pProperty->GetValue() == "SB_VERTICAL" )
		//	{
		//		SetScrollbarType( eSB_VERTICAL );
		//	}
		//	else if( pProperty->GetValue() == "SB_HORIZONAL" )
		//	{
		//		SetScrollbarType( eSB_HORIZONAL );
		//	}
		//	else
		//	{
		//		throw CGUIException("[CGUIWgtScrollbar::ProcessProperty]: unknown scrollbar type <%s>!", pProperty->GetValue().c_str());
		//	}
		//}
		//else if( pProperty->GetName() == "AUTO_POSITION" && pProperty->GetType()=="BOOL")
		//{
		//	EnableAutoPosition(StringToValue(pProperty->GetValue()));
		//}
	//}
	//------------------------------------------------------------------------------



}//namespace guiex

