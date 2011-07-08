/** 
* @file guievent.cpp
* @brief widget event
* @author ken
* @date 2006-06-12
*/

//============================================================================//
// include
//============================================================================// 
#include "guievent.h"
#include "guiwidget.h"
#include "guisystem.h"
#include "guiinterfacemouse.h"
#include "guiinterfacekeyboard.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 


//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIEvent
	//*****************************************************************************

	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUIEvent::CGUIEvent(const char* pEventName)
		:m_uEventId(eEVENT_UNKNOW_EVENT)
		,m_strEventType(pEventName)
		,m_pReceiver(NULL)
		,m_bConsumed(false)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destructor
	*/
	CGUIEvent::~CGUIEvent()
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief set event id
	*/
	void CGUIEvent::SetEventId(uint32 uId )
	{
		m_uEventId = uId;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get event id
	* @return event id in int32 type
	*/
	uint32 CGUIEvent::GetEventId() const
	{
		return m_uEventId;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get event type
	*/
	const CGUIString& CGUIEvent::GetEventType() const
	{
		return m_strEventType;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set widget which receives this event
	*/
	void CGUIEvent::SetReceiver(CGUIWidget* pReceiver)
	{
		m_pReceiver = pReceiver;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Get widget which receives this event
	*/
	CGUIWidget*	CGUIEvent::GetReceiver()
	{
		return m_pReceiver;
	}
	//------------------------------------------------------------------------------
	//!< has this event been consumed
	bool CGUIEvent::IsConsumed()
	{
		return m_bConsumed;
	}
	//------------------------------------------------------------------------------
	//!< set whether this event has been consumed
	void CGUIEvent::Consume(bool bConsumed)
	{
		m_bConsumed = bConsumed;
	}




	//*****************************************************************************
	//	CGUIEventScrollbar
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventScrollbar );
	//------------------------------------------------------------------------------
	CGUIEventScrollbar::CGUIEventScrollbar()
		:CGUIEvent("CGUIEventScrollbar")
		,m_eOrientation(eOrientation_Vertical)
		,m_nRange(0)
		,m_nCurrentPos(0)
		,m_pScrollbar(NULL)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventScrollbar::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch( GetEventId())
		{
		case eEVENT_SCROLLBAR_SCROLL:
			return m_pReceiver->OnScrollbarScroll(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventScrollbar::SetOrientation( EOrientation eOrientation )
	{
		m_eOrientation = eOrientation;
	}
	//------------------------------------------------------------------------------
	EOrientation CGUIEventScrollbar::GetOrientation( ) const
	{
		return m_eOrientation;
	}
	//------------------------------------------------------------------------------
	void CGUIEventScrollbar::SetScrollbar(CGUIWidget * pScrollbar)
	{
		m_pScrollbar = pScrollbar;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIEventScrollbar::GetScrollbar() const
	{
		return m_pScrollbar;
	}
	//------------------------------------------------------------------------------
	void CGUIEventScrollbar::SetCurrentPos( uint32 nPos )
	{
		m_nCurrentPos = nPos;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventScrollbar::GetCurrentPos() const
	{
		return m_nCurrentPos;
	}
	//------------------------------------------------------------------------------
	void CGUIEventScrollbar::SetRange( uint32 nRange )
	{
		m_nRange = nRange;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventScrollbar::GetRange() const
	{
		return m_nRange;
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIEventNotification
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventNotification );
	//------------------------------------------------------------------------------
	CGUIEventNotification::CGUIEventNotification()
		:CGUIEvent("CGUIEventNotification")
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventNotification::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch( GetEventId())
		{
		case eEVENT_SCALE_CHANGE:
			return m_pReceiver->OnScaleChange(this);

		case eEVENT_ACTIVE:
			return m_pReceiver->OnActive(this);

		case eEVENT_INACTIVE:
			return m_pReceiver->OnInactive(this);

		case eEVENT_FOCUS_LOST:
			return m_pReceiver->OnLostFocus(this);

		case eEVENT_FOCUS_GET:
			return m_pReceiver->OnGetFocus(this);

		case eEVENT_VISIBLE:
			return m_pReceiver->OnVisible(this);

		case eEVENT_INVISIBLE:
			return m_pReceiver->OnInvisible(this);

		case eEVENT_DISABLE:
			return m_pReceiver->OnEnable(this);

		case eEVENT_ENABLE:
			return m_pReceiver->OnDisable(this);

		case eEVENT_LOAD:
			return m_pReceiver->OnLoad(this);

		case eEVENT_OPEN:
			return m_pReceiver->OnOpen(this);

		case eEVENT_CLOSE:
			return m_pReceiver->OnClose(this);

			//case eEVENT_CREATE:
			//	return m_pReceiver->OnCreate(this);

		case eEVENT_CHECKED:
			return m_pReceiver->OnChecked(this);

		case eEVENT_UNCHECKED:
			return m_pReceiver->OnUnchecked(this);

		case eEVENT_UPDATE:
			return m_pReceiver->OnEventUpdate(this);


		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIEventTimer
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventTimer );
	//------------------------------------------------------------------------------
	CGUIEventTimer::CGUIEventTimer()
		:CGUIEvent("CGUIEventTimer")
		,m_fDuration(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventTimer::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch( GetEventId())
		{
		case eEVENT_TIMER:
			return m_pReceiver->OnTimer(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventTimer::SetDuration(real fDuration)
	{
		m_fDuration = fDuration;
	}
	//------------------------------------------------------------------------------
	real CGUIEventTimer::GetDuration() const
	{
		return m_fDuration;
	}
	//------------------------------------------------------------------------------
	void CGUIEventTimer::SetTimerName(const CGUIString& rTimerName)
	{
		m_strTimerName = rTimerName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIEventTimer::GetTimerName()
	{
		return m_strTimerName;
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIEventSize
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventSize );
	//------------------------------------------------------------------------------
	CGUIEventSize::CGUIEventSize()
		:CGUIEvent("CGUIEventSize")
		,m_aSize()
	{
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIEventSize::GetSize() const
	{
		return m_aSize;
	}
	//------------------------------------------------------------------------------
	void CGUIEventSize::SetSize(const CGUISize& rSize)
	{
		m_aSize = rSize;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventSize::Process()
	{
		switch( GetEventId())
		{
		case eEVENT_SIZE_CHANGE:
			GUI_ASSERT( m_pReceiver,"havn't receiver" );
			return m_pReceiver->OnSizeChanged(this);

		case eEVENT_PARENT_CHANGE_SIZE:
			return m_pReceiver->OnParentSizeChange(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIEventAlpha
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventAlpha );
	//------------------------------------------------------------------------------
	CGUIEventAlpha::CGUIEventAlpha()
		:CGUIEvent("CGUIEventAlpha")
		,m_fAlpha(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	real CGUIEventAlpha::GetAlpha() const
	{
		return m_fAlpha;
	}
	//------------------------------------------------------------------------------
	void CGUIEventAlpha::SetAlpha(real fAlpha)
	{
		m_fAlpha = fAlpha;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventAlpha::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch( GetEventId())
		{
		case eEVENT_CHANGE_ALPHA:
			return m_pReceiver->OnAlphaChange(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIEventMouse
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventMouse );
	//------------------------------------------------------------------------------
	CGUIEventMouse::CGUIEventMouse()
		:CGUIEvent("CGUIEventMouse")
		,m_fWheelChange(0.0f)
		,m_uSysKeys(0)
		,m_pKeyboard(NULL)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventMouse::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_MOUSE_ENTER:
			return m_pReceiver->OnMouseEnter(this);

		case eEVENT_MOUSE_LEAVE:
			return m_pReceiver->OnMouseLeave(this);

		case eEVENT_MOUSE_UP:
			switch(m_nButton)
			{
			case MOUSE_LEFT:
				return m_pReceiver->OnMouseLeftUp(this);
			case MOUSE_RIGHT:
				return m_pReceiver->OnMouseRightUp(this);
			case MOUSE_MIDDLE:
				return m_pReceiver->OnMouseMiddleUp(this);
			default:
				return GUI_INVALID;
			}

		case eEVENT_MOUSE_DOWN:
			switch(m_nButton)
			{
			case MOUSE_LEFT:
				return m_pReceiver->OnMouseLeftDown(this);
			case MOUSE_RIGHT:
				return m_pReceiver->OnMouseRightDown(this);
			case MOUSE_MIDDLE:
				return m_pReceiver->OnMouseMiddleDown(this);
			default:
				return GUI_INVALID;
			}

		case eEVENT_MOUSE_DBCLICK:
			switch(m_nButton)
			{
			case MOUSE_LEFT:
				return m_pReceiver->OnMouseLeftDbClick(this);
			case MOUSE_RIGHT:
				return m_pReceiver->OnMouseRightDbClick(this);
			case MOUSE_MIDDLE:
				return m_pReceiver->OnMouseMiddleDbClick(this);
			default:
				return GUI_INVALID;
			}

		case eEVENT_MOUSE_MULTI_CLICK:
			switch(m_nButton)
			{
			case MOUSE_LEFT:
				return m_pReceiver->OnMouseLeftMultiClick(this);
			case MOUSE_RIGHT:
				return m_pReceiver->OnMouseRightMultiClick(this);
			case MOUSE_MIDDLE:
				return m_pReceiver->OnMouseMiddleMultiClick(this);
			default:
				return GUI_INVALID;
			}

		case eEVENT_MOUSE_CLICK:
			switch(m_nButton)
			{
			case MOUSE_LEFT:
				return m_pReceiver->OnMouseLeftClick(this);
			case MOUSE_RIGHT:
				return m_pReceiver->OnMouseRightClick(this);
			case MOUSE_MIDDLE:
				return m_pReceiver->OnMouseMiddleClick(this);
			default:
				return GUI_INVALID;
			}

		case eEVENT_MOUSE_MOVE:
			return m_pReceiver->OnMouseMove(this);

		case eEVENT_MOUSE_WHEEL:		
			return m_pReceiver->OnMouseWheel(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventMouse::SetGlobalPosition( const CGUIVector2& rPos )
	{
		m_aPosition = rPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIEventMouse::GetGlobalPosition() const
	{
		return m_aPosition;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIEventMouse::GetLocalPosition() const
	{
		CGUIVector2 aTempPos = m_aPosition;
		m_pReceiver->WorldToLocal( aTempPos );
		return aTempPos;
	}
	//------------------------------------------------------------------------------
	void CGUIEventMouse::SetSysKeys(uint32 uSyskeys)
	{
		m_uSysKeys = uSyskeys;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventMouse::GetSysKeys() const
	{
		return m_uSysKeys;
	}
	//------------------------------------------------------------------------------
	void CGUIEventMouse::SetWheelChange(real fWheelChange)
	{
		m_fWheelChange = fWheelChange;
	}
	//------------------------------------------------------------------------------
	real CGUIEventMouse::GetWheelChange( )
	{
		return m_fWheelChange;
	}
	//------------------------------------------------------------------------------
	void CGUIEventMouse::SetButton(int32 nButton)
	{
		GUI_ASSERT(nButton < _MOUSE_BUTTON_MAX_, "unknown button");
		m_nButton = nButton;
	}
	//------------------------------------------------------------------------------
	int32 CGUIEventMouse::GetButton() const
	{
		return m_nButton;
	}
	//------------------------------------------------------------------------------
	void CGUIEventMouse::SetMultiClickCount( uint32 nCount )
	{
		m_nClickCount = nCount;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventMouse::GetMultiClickCount( ) const
	{
		return m_nClickCount;
	}
	//------------------------------------------------------------------------------
	void CGUIEventMouse::SetKeyboardInterface( IGUIInterfaceKeyboard*pKeyboard)
	{
		m_pKeyboard = pKeyboard;
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceKeyboard*	CGUIEventMouse::GetKeyboardInterface() const
	{
		return m_pKeyboard;
	}
	//------------------------------------------------------------------------------





	//*****************************************************************************
	//	CGUIEventKeyboard
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventKeyboard );
	//------------------------------------------------------------------------------
	CGUIEventKeyboard::CGUIEventKeyboard()
		:CGUIEvent("CGUIEventKeyboard")
		,m_pKeyboard(NULL)
		,m_nKeyCode(KC_NONE)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventKeyboard::Process()
	{	
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_KEY_DOWN:
			return m_pReceiver->OnKeyDown(this);

		case eEVENT_KEY_UP:
			return m_pReceiver->OnKeyUp(this);

		case eEVENT_KEY_PRESSED:
			return m_pReceiver->OnKeyPressed(this);

		case eEVENT_KEY_CLICKED:
			return m_pReceiver->OnKeyClicked(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventKeyboard::SetKeyCode( int32 nKeyCode)
	{
		m_nKeyCode = nKeyCode;
	}
	//------------------------------------------------------------------------------
	int32 CGUIEventKeyboard::GetKeyCode() const
	{
		return m_nKeyCode;
	}
	//------------------------------------------------------------------------------
	void CGUIEventKeyboard::SetKeyboardInterface( IGUIInterfaceKeyboard*pKeyboard)
	{
		m_pKeyboard = pKeyboard;
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceKeyboard*	CGUIEventKeyboard::GetKeyboardInterface() const
	{
		return m_pKeyboard;
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIEventDrag
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventDrag );
	//------------------------------------------------------------------------------
	CGUIEventDrag::CGUIEventDrag()
		:CGUIEvent("CGUIEventDrag")
		,m_aWidgetPos()
		,m_bExpired(false)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventDrag::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_DRAG_BEGIN:
			return m_pReceiver->OnDragBegin(this);

		case eEVENT_DRAG_PROCESS:
			return m_pReceiver->OnDragProcess(this);

		case eEVENT_DRAG_END:
			return m_pReceiver->OnDragEnd(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventDrag::SetWidgetLocalPos( const CGUIVector2& rPos )
	{
		m_aWidgetPos = rPos;
	}
	//------------------------------------------------------------------------------
	void CGUIEventDrag::SetMouseGlobalPos( const CGUIVector2& rPos )
	{
		m_aMousePos = rPos;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIEventDrag::GetWidgetGlobalPos() const
	{
		CGUIVector2 aTempPos = m_aMousePos;
		m_pReceiver->LocalToWorld( aTempPos );
		return aTempPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIEventDrag::GetWidgetLocalPos() const
	{
		return m_aWidgetPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIEventDrag::GetMouseGlobalPos() const
	{
		return m_aMousePos;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIEventDrag::GetMouseLocalPos() const
	{
		CGUIVector2 aTempPos = m_aMousePos;
		m_pReceiver->WorldToLocal( aTempPos );
		return aTempPos;
	}
	//------------------------------------------------------------------------------
	void CGUIEventDrag::SetButton( int32 nButton)
	{
		m_nButton = nButton;
	}
	//------------------------------------------------------------------------------
	const int32 CGUIEventDrag::GetButton( ) const
	{
		return m_nButton;
	}
	//------------------------------------------------------------------------------
	bool CGUIEventDrag::IsExpired()
	{
		return m_bExpired;
	}
	//------------------------------------------------------------------------------
	void CGUIEventDrag::Expire(bool bExpired)
	{
		m_bExpired = bExpired;
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIEventListBox
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventListBox );
	//------------------------------------------------------------------------------
	CGUIEventListBox::CGUIEventListBox()
		:CGUIEvent("CGUIEventListBox")
		,m_nDbClickedItemIdx(0)
		,m_nSelectedItemIdx(0)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventListBox::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_LISTBOX_SELECTED:
			return m_pReceiver->OnListBoxSelected(this);

		case eEVENT_LISTBOX_DBCLICK_LEFT:
			return m_pReceiver->OnListBoxLeftDbClick(this);

		case eEVENT_LISTBOX_DBCLICK_RIGHT:
			return m_pReceiver->OnListBoxRightDbClick(this);

		case eEVENT_LISTBOX_CLICK_LEFT:
			return m_pReceiver->OnListBoxLeftClick(this);

		case eEVENT_LISTBOX_CLICK_RIGHT:
			return m_pReceiver->OnListBoxRightClick(this);

		case eEVENT_LISTBOX_SELECTCHANGED:
			return m_pReceiver->OnListBoxSelectChanged(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventListBox::SetDbClickedItemIdx( uint32 nIdx )
	{
		m_nDbClickedItemIdx = nIdx;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventListBox::GetDbClickedItemIdx( ) const
	{
		return m_nDbClickedItemIdx;
	}
	//------------------------------------------------------------------------------
	void CGUIEventListBox::SetSelectedItemIdx( uint32 nIdx )
	{
		m_nSelectedItemIdx = nIdx;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventListBox::GetSelectedItemIdx( ) const
	{
		return m_nSelectedItemIdx;
	}


	//*****************************************************************************
	//	CGUIEventComboBox
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventComboBox );
	//------------------------------------------------------------------------------
	CGUIEventComboBox::CGUIEventComboBox()
		:CGUIEvent("CGUIEventComboBox")
		,m_nSelectedItemIdx(0)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventComboBox::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_COMBOBOX_SELECTED:
			return m_pReceiver->OnComboBoxSelected(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventComboBox::SetSelectedItemIdx( uint32 nIdx )
	{
		m_nSelectedItemIdx = nIdx;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIEventComboBox::GetSelectedItemIdx( ) const
	{
		return m_nSelectedItemIdx;
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIEventComboBox
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventUI );
	//------------------------------------------------------------------------------
	CGUIEventUI::CGUIEventUI()
		:CGUIEvent("CGUIEventUI")
		,m_pExtraData(0)
	{
		m_uEventId = eEVENT_UI;
	}
	//------------------------------------------------------------------------------
	void CGUIEventUI::SetArg(int nIndex, const CGUIString& rArg )
	{
		if( nIndex >= MAX_UIEVENT_ARGS )
		{
			GUI_WARNING( "the arg index of ui event is overflow");
			nIndex = nIndex % MAX_UIEVENT_ARGS;
		}
		m_pArgs[nIndex] = rArg;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIEventUI::GetArg(int nIndex) const
	{
		if( nIndex >= MAX_UIEVENT_ARGS )
		{
			GUI_WARNING( "the arg index of ui event is overflow");
			nIndex = nIndex % MAX_UIEVENT_ARGS;
		}
		return m_pArgs[nIndex];
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventUI::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_UI:
			return m_pReceiver->OnUIEvent(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventUI::SetExtraData( void* pData )
	{
		m_pExtraData = pData;
	}
	//------------------------------------------------------------------------------
	void* CGUIEventUI::GetExtraData( ) const
	{
		return m_pExtraData;
	}
	//------------------------------------------------------------------------------
	void CGUIEventUI::SetUIName( const CGUIString& rName )
	{
		m_strUIEventName = rName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIEventUI::GetUIName( ) const
	{
		return m_strUIEventName;
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIEventComboBox
	//*****************************************************************************
	//GUI_EVENT_GENERATOR_IMPLEMENT( CGUIEventRelativeChange );
	//------------------------------------------------------------------------------
	CGUIEventRelativeChange::CGUIEventRelativeChange()
		:CGUIEvent("CGUIEventRelativeChange")
		,m_pRelative(0)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process the event
	*/
	uint32 CGUIEventRelativeChange::Process()
	{
		GUI_ASSERT( m_pReceiver,"havn't receiver" );

		switch(GetEventId())
		{
		case eEVENT_ADD_CHILD:
			return m_pReceiver->OnAddChild(this);

		case eEVENT_REMOVE_CHILD:
			return m_pReceiver->OnRemoveChild(this);

		case eEVENT_PARENT_CHANGED:
			return m_pReceiver->OnParentChanged(this);

		default:
			return GUI_INVALID;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIEventRelativeChange::SetRelative( CGUIWidget* pRelative )
	{
		m_pRelative = pRelative;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIEventRelativeChange::GetRelative( ) const
	{
		return m_pRelative;
	}
}//namespace guiex


