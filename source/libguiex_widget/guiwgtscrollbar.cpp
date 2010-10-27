/** 
* @file guiwgtscrollbar.cpp
* @brief scrollbar
* @author ken
* @date 2006-08-07
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget\guiwgtbutton.h>
#include <libguiex_widget\guiwgtscrollbar.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guipropertymanager.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtScrollbarArrow
	* @brief scrollbar's button
	* used image name: (Same as button)
	*		-BTN_NORMAL
	*		-BTN_HOVER
	*		-BTN_DISABLE
	*		-BTN_PUSH
	*		-BTN_HOVER_OVERLAY
	*		-BTN_PUSH_OVERLAY
	*/ 
	class GUIEXPORT CGUIWgtScrollbarArrow : public CGUIWgtButton
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtScrollbarArrow( const CGUIString& rName,const CGUIString& rProjectName, int nType );

	protected:
		///initialize
		void InitScrollbarArrow();

	protected:	//!< callback function
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );

	protected:
		int		m_nArrowType;		/// arrow type

	private:
		static CGUIString	ms_strType;
	};


	/**
	* @class CGUIWgtScrollbarSlide
	* @brief scrollbar's slide
	* used image name: (Same as button)
	*		-BTN_NORMAL
	*		-BTN_HOVER
	*		-BTN_DISABLE
	*		-BTN_PUSH
	*		-BTN_HOVER_OVERLAY
	*		-BTN_PUSH_OVERLAY
	*/ 
	class GUIEXPORT CGUIWgtScrollbarSlide : public CGUIWgtButton
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtScrollbarSlide( const CGUIString& rName, const CGUIString& rProjectName );

	protected:
		///initialize
		void InitScrollbarSlide();

	protected:	//!< callback function
		virtual uint32		OnDragBegin( CGUIEventDrag* pEvent );
		virtual uint32		OnDragProcess( CGUIEventDrag* pEvent );
		virtual uint32		OnDragEnd( CGUIEventDrag* pEvent );

	protected:

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
	CGUIWgtScrollbarArrow::CGUIWgtScrollbarArrow( const CGUIString& rName, const CGUIString& rProjectName, int nType )
		:CGUIWgtButton(ms_strType, rName, rProjectName)
		,m_nArrowType(nType)
	{
		InitScrollbarArrow();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarArrow::InitScrollbarArrow()
	{
		SetFlag(eFLAG_FOCUS_AGENCY, true);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarArrow::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		switch( m_nArrowType)
		{
		case 0:
			static_cast<CGUIWgtScrollbar*>(GetParent())->DecreasePos();
			break;
		case 1:
			static_cast<CGUIWgtScrollbar*>(GetParent())->IncreasePos();
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
	CGUIWgtScrollbarSlide::CGUIWgtScrollbarSlide( const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWgtButton(ms_strType, rName, rProjectName)
	{
		InitScrollbarSlide();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtScrollbarSlide::InitScrollbarSlide()
	{
		SetFlag(eFLAG_MOVABLE, true);
		SetFlag(eFLAG_FOCUS_AGENCY, true);

	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarSlide::OnDragBegin( CGUIEventDrag* pEvent )
	{

		return CGUIWidget::OnDragBegin(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarSlide::OnDragProcess( CGUIEventDrag* pEvent )
	{
		CGUIWgtScrollbar* pParent = static_cast<CGUIWgtScrollbar*>(GetParent());
		GUI_ASSERT(pParent, "haven't parent");

		CGUIRect	aSlideWorkArea = pParent->GetSlideArea();
		CGUIVector2	aSlidePoint = GetLocalPosition( );
		CGUIVector2	aWidgetPoint = pEvent->GetWidgetPos();
		CGUIVector2	aMovePos;		/// wanted position
		CGUISize		aSlideSize = GetSize();
		uint32		nCurScrollbarPos = 0;	/// the value of scrollbar

		switch( pParent->GetScrollbarType() )
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
				pParent->GetMinPos()+
				GUI_FLOAT2UINT_ROUND((aMovePos.x -aSlideWorkArea.m_fLeft)  * pParent->GetRange() / aSlideWorkArea.GetWidth());
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
				pParent->GetMinPos()+
				GUI_FLOAT2UINT_ROUND((aMovePos.y-aSlideWorkArea.m_fTop) * pParent->GetRange() / aSlideWorkArea.GetHeight());
			break;
		}

		// set current position of scrollbar
		pParent->SetCurPosWithoutUpdate(nCurScrollbarPos);

		// set slide position
		pEvent->SetWidgetPos(aMovePos);

		return CGUIWidget::OnDragProcess(pEvent);
	}
	//------------------------------------------------------------------------------
	uint32		CGUIWgtScrollbarSlide::OnDragEnd( CGUIEventDrag* pEvent )
	{
		CGUIWgtScrollbar* pParent = static_cast<CGUIWgtScrollbar*>(GetParent());
		pParent->SetCurrentPos(pParent->GetCurrentPos());

		return CGUIWidget::OnDragEnd(pEvent);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtScrollbar);
	//------------------------------------------------------------------------------
	CGUIString CGUIWgtScrollbar::ms_strType = "CGUIWgtScrollbar";
	//------------------------------------------------------------------------------
	CGUIWgtScrollbar::CGUIWgtScrollbar(const CGUIString& rName, const CGUIString& rProjectName)
		:CGUIWidget(ms_strType, rName, rProjectName)
	{
		InitScrollbar();
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbar::CGUIWgtScrollbar( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName )
		:CGUIWidget(rType, rName, rProjectName)
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
		SetPositionRelative(true);
		SetSizeRelative(false);

		//create child
		m_pArrow[0] = new CGUIWgtScrollbarArrow(GetName()+"_arrow_0__auto__", GetProjectName(), 0);
		m_pArrow[0]->SetParent(this);
		m_pArrow[0]->SetPositionRelative(true);
		m_pArrow[0]->SetSizeRelative(false);
		m_pArrow[1] = new CGUIWgtScrollbarArrow(GetName()+"_arrow_1__auto__", GetProjectName(), 1);
		m_pArrow[1]->SetParent(this);
		m_pArrow[1]->SetPositionRelative(true);
		m_pArrow[1]->SetSizeRelative(false);
		m_pSlide = new CGUIWgtScrollbarSlide(GetName()+"_slide__auto__", GetProjectName() );
		m_pSlide->SetParent(this);
		m_pSlide->SetPositionRelative(true);
		m_pSlide->SetSizeRelative(false);

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

		SetFlag(eFLAG_MOVABLE, false);
		SetFocusable(true);
		SetSelfActivable(false);
		SetFlag(eFLAG_EVENT_PARENTSIZECHANGE, true);
		SetFlag(eFLAG_EVENT_DBCLICK, true);
		SetFlag(eFLAG_PARENT_CLIENTRECT, false);

	}
	//------------------------------------------------------------------------------
	const CGUIString&	CGUIWgtScrollbar::GetWidgetType()
	{
		return ms_strType;
	}
	//------------------------------------------------------------------------------
	CGUIWgtScrollbar*	CGUIWgtScrollbar::FromWidget( CGUIWidget* pWidget )
	{
		if( !pWidget )
		{
			throw CGUIException("[CGUIWgtScrollbar::FromWidget]: the given widget is nil" );
		}
		if( pWidget->GetType() != ms_strType )
		{
			throw CGUIException("[CGUIWgtScrollbar::FromWidget]: the real type of given widget is <%s>!", pWidget->GetType().c_str());
		}
		return dynamic_cast<CGUIWgtScrollbar *>(pWidget);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::SetScrollbarHost( CGUIWidget* pWgtHost )
	{
		m_pWgtHost = pWgtHost;
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::SetScrollbarType( EScrollbarType eScrollbarType )
	{
		m_eScrollbarType = eScrollbarType;

		UpdateScrollbar();
	}
	//------------------------------------------------------------------------------
	EScrollbarType	CGUIWgtScrollbar::GetScrollbarType( ) const
	{
		return m_eScrollbarType;
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtScrollbar::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "SCROLLBAR_BG")
		{
			m_pImageBg = pImage;
			if( GetSize().IsEqualZero() && pImage )
			{
				SetSize(pImage->GetSize());
			}
		}
		else if( rName == "SCROLLBAR_ARROW_NORMAL_0") 
		{
			m_pArrow[0]->SetImage("BTN_NORMAL", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_0")
		{
			m_pArrow[0]->SetImage("BTN_HOVER", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_DISABLE_0")
		{
			m_pArrow[0]->SetImage("BTN_DISABLE", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_0")
		{
			m_pArrow[0]->SetImage("BTN_PUSH", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_OVERLAY_0")
		{
			m_pArrow[0]->SetImage("BTN_HOVER_OVERLAY", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_OVERLAY_0")
		{
			m_pArrow[0]->SetImage("BTN_PUSH_OVERLAY", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_NORMAL_1")
		{
			m_pArrow[1]->SetImage("BTN_NORMAL", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_1")
		{
			m_pArrow[1]->SetImage("BTN_HOVER", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_DISABLE_1")
		{
			m_pArrow[1]->SetImage("BTN_DISABLE", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_1")
		{
			m_pArrow[1]->SetImage("BTN_PUSH", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_HOVER_OVERLAY_1")
		{
			m_pArrow[1]->SetImage("BTN_HOVER_OVERLAY", pImage);
		}
		else if( rName == "SCROLLBAR_ARROW_PUSH_OVERLAY_1")
		{
			m_pArrow[1]->SetImage("BTN_PUSH_OVERLAY", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_NORMAL")
		{
			m_pSlide->SetImage("BTN_NORMAL", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_HOVER")
		{
			m_pSlide->SetImage("BTN_HOVER", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_DISABLE")
		{
			m_pSlide->SetImage("BTN_DISABLE", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_PUSH")
		{
			m_pSlide->SetImage("BTN_PUSH", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_HOVER_OVERLAY")
		{
			m_pSlide->SetImage("BTN_HOVER_OVERLAY", pImage);
		}
		else if( rName == "SCROLLBAR_SLIDE_PUSH_OVERLAY")
		{
			m_pSlide->SetImage("BTN_PUSH_OVERLAY", pImage);
		}
		else
		{
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtScrollbar::Create()
	{
		int32 result = CGUIWidget::Create();

		//for child
		m_pArrow[0]->Create();
		m_pArrow[1]->Create();
		m_pSlide->Create();

		return result;
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbar::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBg, GetRect( ), pRender->GetAndIncZ(),&GetClipRect() );
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbar::UpdateValue()
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
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
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
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}

		//enable or disable arrow
		if( m_nCurPos <= m_nMinPos )
		{
			m_pArrow[0]->SetSelfDisable(true);
		}
		else
		{
			m_pArrow[0]->SetSelfDisable(false);
		}
		if( m_nCurPos >= m_nMaxPos )
		{
			m_pArrow[1]->SetSelfDisable(true);
		}
		else
		{
			m_pArrow[1]->SetSelfDisable(false);
		}

		//show or hide slide
		if( m_nMinPos == m_nMaxPos )
		{
			m_pSlide->SetSelfVisible(false);
		}
		else
		{
			m_pSlide->SetSelfVisible(true);
		}

		//update position
		if( m_nMaxPos <= m_nMinPos || m_nCurPos < m_nMinPos ||m_nCurPos > m_nMaxPos)
		{
			return;
		}

		CGUIRect aSlideArea = GetSlideArea();
		CGUISize aSlideSize = m_pSlide->GetSize();
		switch( m_eScrollbarType )
		{
			//for vertical scroll bar
		case eSB_VERTICAL:
			{
				aSlideArea.m_fTop += aSlideSize.m_fHeight / 2;
				aSlideArea.m_fBottom -= aSlideSize.m_fHeight / 2;
				real fGap = (m_nCurPos -m_nMinPos) * aSlideArea.GetHeight()/GetRange();
				m_pSlide->SetLocalPosition(
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
				m_pSlide->SetLocalPosition(
					aSlideArea.m_fLeft + fGap,
					aSlideArea.m_fTop + aSlideArea.GetHeight()/2);
			}
			break;
		}
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIWgtScrollbar::GetSlideArea( )
	{
		CGUIRect aRect = GetRect();
		aRect.SetPosition(CGUIVector2(0.0f, 0.0f));
		CGUISize aSize0 = m_pArrow[0]->GetSize();
		CGUISize aSize1 = m_pArrow[1]->GetSize();
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
	void	CGUIWgtScrollbar::UpdateScrollbar()
	{
		if( IsOpen()==false || !GetParent() )
		{
			return;
		}

		switch(m_eScrollbarType)
		{
		case eSB_VERTICAL:
			//set children's pos
			m_pArrow[0]->SetTagPoint(0.5f, 0.0f);
			m_pArrow[0]->SetPositionRatio(0.5f, 0.0f);
			m_pArrow[1]->SetTagPoint(0.5f, 1.0f);
			m_pArrow[1]->SetPositionRatio(0.5f, 1.0f);
			m_pSlide->SetTagPoint(0.5f, 0.5f);
			break;

		case eSB_HORIZONAL:
			//set self's pos
			if( m_bAutoPosition )
			{

			}

			//set children's pos
			m_pArrow[0]->SetTagPoint(0.0f, 0.5f);
			m_pArrow[0]->SetPositionRatio(0.0f, 0.5f);
			m_pArrow[1]->SetTagPoint(1.0f, 0.5f);
			m_pArrow[1]->SetPositionRatio(1.0f, 0.5f);
			m_pSlide->SetTagPoint(0.5f, 0.5f);
			break;
		}

		if( m_bAutoPosition )
		{
			//get client area of parent
			CGUISize aClientArea = 
				(m_bAutoPosition && GetParent()->GetFlag(eFLAG_SCROLLBAR_AUTOPOSITION)) ?
				GetParent()->GetRect( ).GetSize() : this->GetSize();

			switch(m_eScrollbarType)
			{
			case eSB_VERTICAL:
				SetTagPoint(1.0f, 0.0f);
				SetPositionRatio( CGUIVector2(1.0f, 0.0f));
				SetRectSize( GetSize().GetWidth() * GetDerivedScale().GetWidth(), aClientArea.GetHeight());
				break;

			case eSB_HORIZONAL:
				SetTagPoint(0.0f, 1.0f);
				SetPositionRatio( CGUIVector2(0.0f, 1.0f));
				SetRectSize( aClientArea.GetWidth(), GetSize().GetHeight() * GetDerivedScale().GetHeight());
				break;
			}
		}

		UpdateValue();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtScrollbar::SetValue(const CGUIString& rName, const CGUIString& rValue)
	{
		if( rName == "MaxPos")
		{
			SetMaxPos( CGUIStringConvertor::StringToUInt(rValue));
		}
		else if( rName == "MinPos")
		{
			SetMinPos( CGUIStringConvertor::StringToUInt(rValue));
		}
		else if( rName == "Range")
		{
			SetRange( CGUIStringConvertor::StringToUInt(rValue));
		}
		else if( rName == "CurrentPos")
		{
			SetCurrentPos( CGUIStringConvertor::StringToUInt(rValue));
		}
		else if( rName == "AutoPosition")
		{
			EnableAutoPosition( CGUIStringConvertor::StringToBool(rValue));
		}
		else
		{
			CGUIWidget::SetValue(rName, rValue);
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWgtScrollbar::GetValue(const CGUIString& rName) const
	{
		if( rName == "MaxPos")
		{
			return CGUIStringConvertor::UIntToString(GetMaxPos());
		}
		else	if( rName == "MinPos")
		{
			return CGUIStringConvertor::UIntToString(GetMinPos());
		}
		else	if( rName == "Range")
		{
			return CGUIStringConvertor::UIntToString(GetRange());
		}
		else	if( rName == "CurrentPos")
		{
			return CGUIStringConvertor::UIntToString(GetCurrentPos());
		}
		else if( rName == "AutoPosition")
		{
			return CGUIStringConvertor::BoolToString(IsAutoPosition());
		}
		else
		{
			return CGUIWidget::GetValue(rName);
		}
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
		SetCurPosWithoutUpdate(nPos);

		//update scrollbar value
		UpdateValue();
	}
	//------------------------------------------------------------------------------
	void		CGUIWgtScrollbar::SetCurPosWithoutUpdate(uint32 nPos)
	{
		m_nCurPos = nPos;
		m_nCurPos = m_nCurPos>m_nMaxPos ? m_nMaxPos : m_nCurPos;
		m_nCurPos = m_nCurPos<m_nMinPos ? m_nMinPos : m_nCurPos;

		//send event to self
		{
			CGUIEventScrollbar aEvent;
			aEvent.SetEventId(eEVENT_SCROLLBAR_SCROLL);
			aEvent.SetScrollbarType(m_eScrollbarType == eSB_VERTICAL);
			aEvent.SetScrollbar(this);
			aEvent.SetReceiver(this);
			aEvent.SetCurrentPos( m_nCurPos );
			aEvent.SetRange( m_nMaxPos - m_nMinPos );
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
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
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
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
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
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
		CGUIVector2		aMousePoint = pEvent->GetPosition();
		GlobalToClient(aMousePoint);
		CGUIVector2	aSlidePoint = m_pSlide->GetLocalPosition( );

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
	CGUIProperty*	CGUIWgtScrollbar::GenerateProperty(const CGUIString& rName, const CGUIString& rType )
	{
		CGUIProperty* pProperty = NULL;
		
		if( rName == "RANGE" && rType == "UINT" )
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::UIntToString(GetRange( )));
		}		
		else if( rName == "SCROLLBAR_TYPE" && rType=="ENUM")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				GetScrollbarType() == eSB_VERTICAL ? "SB_VERTICAL" : "SB_HORIZONAL");
		}
		else if( rName == "AUTO_POSITION" && rType=="BOOL")
		{
			pProperty = CGUIPropertyManager::Instance()->CreateProperty(
				rName, 
				rType, 
				CGUIStringConvertor::BoolToString(IsAutoPosition( )));
		}
		return pProperty ? pProperty : CGUIWidget::GenerateProperty(rName, rType);
	}
	//------------------------------------------------------------------------------
	void			CGUIWgtScrollbar::ProcessProperty( const CGUIProperty* pProperty)
	{
		CGUIWidget::ProcessProperty(pProperty);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for parent
		/*
		*<property name="RANGE" type="UINT" value="20" />
		*/
		if( pProperty->GetName() == "RANGE" && pProperty->GetType()=="UINT")
		{
			SetRange(CGUIStringConvertor::StringToUInt(pProperty->GetValue()));
		}
		else if( pProperty->GetName() == "SCROLLBAR_TYPE" && pProperty->GetType()=="ENUM")
		{
			if( pProperty->GetValue() == "SB_VERTICAL" )
			{
				SetScrollbarType( eSB_VERTICAL );
			}
			else if( pProperty->GetValue() == "SB_HORIZONAL" )
			{
				SetScrollbarType( eSB_HORIZONAL );
			}
			else
			{
				throw CGUIException("[CGUIWgtScrollbar::ProcessProperty]: unknown scrollbar type <%s>!", pProperty->GetValue().c_str());
			}
		}
		else if( pProperty->GetName() == "AUTO_POSITION" && pProperty->GetType()=="BOOL")
		{
			EnableAutoPosition(CGUIStringConvertor::StringToBool(pProperty->GetValue()));
		}
	}
	//------------------------------------------------------------------------------



}//namespace guiex

