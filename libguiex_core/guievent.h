/** 
* @file guievent.h
* @brief widget event
* @author ken
* @date 2006-06-12
*/

#ifndef __GUI_EVENT_20060612_H__
#define __GUI_EVENT_20060612_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guivector2.h"

#include "guitimer.h"
#include "guisize.h"
#include <list>


//============================================================================//
// define
//============================================================================//
namespace guiex
{
#define EVENT_DECLARE( event )	friend class event;

#define EVENT_ITEM_LIST()	\
	EVENT_DECLARE( CGUIEvent )\
	EVENT_DECLARE( CGUIEventRelativeChange )\
	EVENT_DECLARE( CGUIEventAlpha )\
	EVENT_DECLARE( CGUIEventSize )\
	EVENT_DECLARE( CGUIEventMouse )\
	EVENT_DECLARE( CGUIEventNotification )\
	EVENT_DECLARE( CGUIEventDrag )\
	EVENT_DECLARE( CGUIEventScrollbar )\
	EVENT_DECLARE( CGUIEventTimer )\
	EVENT_DECLARE( CGUIEventKeyboard )\
	EVENT_DECLARE( CGUIEventListBox )\
	EVENT_DECLARE( CGUIEventComboBox )\
	EVENT_DECLARE( CGUIEventUI )

}//namespace guiex

//============================================================================//
// enum
//============================================================================//
namespace guiex
{
	enum EEventId
	{
		///when the size of widget is changed, this event will be sent
		///CGUIEventSize
		eEVENT_SIZE_CHANGE = 0,	
		eEVENT_PARENT_CHANGE_SIZE,

		///event for changing tree struct, such as adding or removing child
		///CGUIEventRelativeChange
		eEVENT_ADD_CHILD,
		eEVENT_REMOVE_CHILD,
		eEVENT_PARENT_CHANGED,

		///when the alpha is changed, this event will be sent.
		///CGUIEventAlpha
		eEVENT_CHANGE_ALPHA,

		///event for mouse
		///CGUIEventMouse
		eEVENT_MOUSE_ENTER,
		eEVENT_MOUSE_LEAVE,
		eEVENT_MOUSE_DOWN,
		eEVENT_MOUSE_UP,
		eEVENT_MOUSE_CLICK,
		eEVENT_MOUSE_DBCLICK,
		eEVENT_MOUSE_MULTI_CLICK,
		eEVENT_MOUSE_MOVE,
		eEVENT_MOUSE_WHEEL,

		///event for keyboard
		///CGUIEventKeyboard
		eEVENT_KEY_DOWN,
		eEVENT_KEY_UP,
		eEVENT_KEY_PRESSED,
		eEVENT_KEY_CLICKED,

		///event for drag
		///CGUIEventDrag
		eEVENT_DRAG_BEGIN,
		eEVENT_DRAG_PROCESS,
		eEVENT_DRAG_END,

		///notification

		//scale change
		///CGUIEventNotification
		eEVENT_SCALE_CHANGE,

		//active
		///CGUIEventNotification
		eEVENT_ACTIVE,
		eEVENT_INACTIVE,

		//focus
		///CGUIEventNotification
		eEVENT_FOCUS_LOST,
		eEVENT_FOCUS_GET,

		//visible
		///CGUIEventNotification
		eEVENT_VISIBLE,
		eEVENT_INVISIBLE,

		//update
		///CGUIEventNotification
		eEVENT_UPDATE,

		//disable
		///CGUIEventNotification
		eEVENT_DISABLE,
		eEVENT_ENABLE,

		///load, open and close
		///CGUIEventNotification
		eEVENT_LOAD,
		eEVENT_OPEN,
		eEVENT_CLOSE,

		///check and uncheck
		///CGUIEventNotification
		eEVENT_CHECKED,
		eEVENT_UNCHECKED,

		//scrollbar open, close, 
		//CGUIEventScrollbar
		eEVENT_SCROLLBAR_SCROLL,

		//list box event
		//CGUIEventListBox
		eEVENT_LISTBOX_SELECTED,
		eEVENT_LISTBOX_SELECTCHANGED,
		eEVENT_LISTBOX_CLICK_LEFT,
		eEVENT_LISTBOX_CLICK_RIGHT,
		eEVENT_LISTBOX_DBCLICK_LEFT,
		eEVENT_LISTBOX_DBCLICK_RIGHT,

		//combo box event
		//CGUIEventComboBox
		eEVENT_COMBOBOX_SELECTED,

		//timer
		eEVENT_TIMER,


		//UI EVENT
		//CGUIEventUI,
		eEVENT_UI,

		///this event is user defined event
		///CGUIWE_UserDefine
		eEVENT_USER_DEFINE,		

		///this event is user defined event, the event id should be (eEVENT_USER_DEFINE_BEGINING+num)
		///CGUIWE_UserDefine
		eEVENT_USER_DEFINE_BEGINING,

		eEVENT_UNKNOW_EVENT,
	};

}//namespace guiex




//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIWidget;
	class IGUIInterfaceKeyboard;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIEvent
	//*****************************************************************************
	/**
	* @class CGUIEvent
	* @brief base class of widget event.
	*/
	class GUIEXPORT CGUIEvent
	{
	public:
		CGUIEvent(const char* pEventName);

		virtual ~CGUIEvent();

		uint32 GetEventId() const;
		void SetEventId(uint32 uId );
		const CGUIString& GetEventType() const;

		void SetReceiver(CGUIWidget* pReceiver);
		CGUIWidget*	GetReceiver();

		bool IsConsumed();
		void Consume(bool bConsumed);

		virtual uint32 Process() = 0;

	protected:
		///widget generator
		friend class CGUIEventFactory;

	protected:
		/// id of the event
		uint32 m_uEventId;

		///type of this event
		CGUIString m_strEventType;

		///receiver
		CGUIWidget*	m_pReceiver;

		/// has this event been consumed
		bool m_bConsumed;
	};

	//*****************************************************************************
	//	CGUIEventScrollbar
	//*****************************************************************************
	/**
	* @class CGUIEventScrollbar
	* @brief event for 
	*	-eEVENT_SCROLLBAR_SCROLL
	*/
	class GUIEXPORT CGUIEventScrollbar : public CGUIEvent
	{
	public:
		CGUIEventScrollbar();
		virtual uint32 Process();

		void SetScrollbarType( bool bVertical );
		bool IsVertical() const;
		bool IsHorizontal() const;
		
		void SetScrollbar(CGUIWidget * pScrollbar);
		CGUIWidget*	GetScrollbar() const;

		void SetCurrentPos( uint32 nPos );
		uint32 GetCurrentPos() const;

		void SetRange( uint32 nRange );
		uint32 GetRange() const;

	protected:
		bool m_bTypeVertical; /// the type of scrollbar
		uint32	m_nCurrentPos; /// current position
		uint32	m_nRange; /// range
		CGUIWidget*	m_pScrollbar; ///scrollbar
	};

	//*****************************************************************************
	//	CGUIEventTimer
	//*****************************************************************************
	/**
	* @class CGUIEventTimer
	* @brief event for 
	*	- eEVENT_TIMER
	*/
	class GUIEXPORT CGUIEventTimer : public CGUIEvent
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIEventTimer();

		virtual uint32 Process();

		void SetTimerName(const CGUIString& rTimerName);
		const CGUIString& GetTimerName();

	protected:
		CGUIString m_strTimerName;
	};


	//*****************************************************************************
	//	CGUIEventNotification
	//*****************************************************************************
	/**
	* @class CGUIEventNotification
	* @brief event for 
	*	- eEVENT_SCALE_CHANGE
	*	- eEVENT_ACTIVE
	*	- eEVENT_INACTIVE
	*	- eEVENT_FOCUS_LOST,
	*	- eEVENT_FOCUS_GET,
	*	- eEVENT_VISIBLE,
	*	- eEVENT_INVISIBLE
	*	- eEVENT_DISABLE,
	*	- eEVENT_ENABLE,
	*	- eEVENT_CREATE,
	*	- eEVENT_UPDATE
	*/
	class GUIEXPORT CGUIEventNotification : public CGUIEvent
	{
	public:
		CGUIEventNotification();

		virtual uint32 Process();
	};



	//*****************************************************************************
	//	CGUIEventSize
	//*****************************************************************************
	/**
	* @class CGUIEventSize
	* @brief event for eEVENT_SIZE_CHANGE
	*/
	class GUIEXPORT CGUIEventSize : public CGUIEvent
	{
	public:
		CGUIEventSize();
		const CGUISize& GetSize() const;

		void SetSize(const CGUISize& rSize);

		virtual uint32 Process();

	protected:
		CGUISize m_aSize;
	};

	//*****************************************************************************
	//	CGUIEventAlpha
	//*****************************************************************************
	/**
	* @class CGUIEventAlpha
	* @brief event for eEVENT_CHANGE_ALPHA
	*/
	class GUIEXPORT CGUIEventAlpha : public CGUIEvent
	{
	public:
		CGUIEventAlpha();

		real GetAlpha() const;
		void SetAlpha(real fAlpha);

		virtual uint32 Process();

	protected:
		real m_fAlpha;
	};


	//*****************************************************************************
	//	CGUIEventMouse
	//*****************************************************************************
	/**
	* @class CGUIEventMouse
	* @brief event for 
	*		eEVENT_MOUSE_ENTER,
	*		eEVENT_MOUSE_LEAVE,
	*		eEVENT_MOUSE_DOWN,
	*		eEVENT_MOUSE_UP,
	*		eEVENT_MOUSE_DBCLICK,
	*		eEVENT_MOUSE_MULTI_CLICK,
	*		eEVENT_MOUSE_MOVE,
	*		eEVENT_MOUSE_WHEEL,
	*/
	class GUIEXPORT CGUIEventMouse : public CGUIEvent
	{
	public:
		CGUIEventMouse();

		virtual uint32 Process();

		void SetGlobalPosition( const CGUIVector2& rPos );
		const CGUIVector2& GetGlobalPosition() const;
		CGUIVector2 GetLocalPosition() const;
		
		void SetSysKeys(uint32 uSyskeys);
		uint32 GetSysKeys() const;

		void SetWheelChange(real fWheelChange);
		real GetWheelChange( );

		void SetMultiClickCount( uint32 nCount );
		uint32 GetMultiClickCount( ) const;

		void SetButton(int32 nButton);
		int32 GetButton() const;

		IGUIInterfaceKeyboard* GetKeyboardInterface() const;
		void SetKeyboardInterface( IGUIInterfaceKeyboard* pKeyboard );

	protected:
		uint32 m_nButton; //!< mouse's button
		uint32 m_nClickCount; //!< click count
		CGUIVector2 m_aPosition; //!< holds current mouse position.
		CGUIVector2 m_aMoveDelta; //!< holds variation of mouse position from last mouse input
		uint32 m_uSysKeys; //!< current state of the system keys and mouse buttons.
		real m_fWheelChange; //!< Holds the amount the scroll wheel has changed.

		IGUIInterfaceKeyboard* m_pKeyboard;
	};



	//*****************************************************************************
	//	CGUIEventKeyboard
	//*****************************************************************************
	/**
	* @class CGUIEventKeyboard
	* @brief event for 
	*	-eEVENT_KEY_DOWN
	*	-eEVENT_KEY_UP
	*	-eEVENT_KEY_PRESSED,
	*	-eEVENT_KEY_CLICKED,
	*/
	class GUIEXPORT CGUIEventKeyboard: public CGUIEvent
	{
	public:
		CGUIEventKeyboard();

		virtual uint32 Process();

		void SetKeyCode( int32 nKeyCode);
		int32 GetKeyCode() const;

		IGUIInterfaceKeyboard* GetKeyboardInterface() const;
		void SetKeyboardInterface( IGUIInterfaceKeyboard* pKeyboard );

	protected:
		int32 m_nKeyCode; /// key code
		IGUIInterfaceKeyboard* m_pKeyboard;
	};



	//*****************************************************************************
	//	CGUIEventDrag
	//*****************************************************************************
	/**
	* @class CGUIEventDrag
	* @brief event for 
	*		eEVENT_DRAG_BEGIN,
	*		eEVENT_DRAG_PROCESS,
	*		eEVENT_DRAG_END,
	*/
	class GUIEXPORT CGUIEventDrag : public CGUIEvent
	{
	public:
		CGUIEventDrag();

		virtual uint32 Process();

		void SetWidgetLocalPos( const CGUIVector2& rPos );
		void SetMouseGlobalPos( const CGUIVector2& rPos );

		CGUIVector2 GetWidgetGlobalPos() const;
		const CGUIVector2& GetWidgetLocalPos() const;

		const CGUIVector2& GetMouseGlobalPos() const;
		CGUIVector2 GetMouseLocalPos() const;

		void SetButton( int32 nButton);
		const int32 GetButton() const;

		bool IsExpired();
		void Expire(bool bExpired);

	protected:
		CGUIVector2	m_aWidgetPos; //!< holds current widget position. local pos
		CGUIVector2	m_aMousePos; //!< holds current mouse position. global pos
		int32 m_nButton; //!< mouse button

		bool m_bExpired; //!< whether this event is been expired
	};



	//*****************************************************************************
	//	CGUIEventListBox
	//*****************************************************************************
	/**
	* @class CGUIEventListBox
	* @brief event for 
	*	-eEVENT_LISTBOX_SELECTED
	*	-eEVENT_LISTBOX_SELECTCHANGED
	*	-eEVENT_LISTBOX_DOUBLECLICKED
	*/
	class GUIEXPORT CGUIEventListBox: public CGUIEvent
	{
	public:
		CGUIEventListBox();

		virtual uint32 Process();

		void SetDbClickedItemIdx( uint32 nIdx );

		uint32 GetDbClickedItemIdx( ) const;

		void SetSelectedItemIdx( uint32 nIdx );

		uint32 GetSelectedItemIdx( ) const;

	protected:
		uint32 m_nDbClickedItemIdx; /// the index of item which is double clicked
		uint32 m_nSelectedItemIdx; /// the index of item which is selected

	};


	//*****************************************************************************
	//	CGUIEventComboBox
	//*****************************************************************************
	/**
	* @class CGUIEventComboBox
	* @brief event for 
	*	-eEVENT_COMBOBOX_SELECTED
	*/
	class GUIEXPORT CGUIEventComboBox: public CGUIEvent
	{
	public:
		CGUIEventComboBox();

		virtual uint32 Process();

		void SetSelectedItemIdx( uint32 nIdx );
		uint32 GetSelectedItemIdx( ) const;

	protected:
		uint32 m_nSelectedItemIdx; /// the index of item which is selected

	};


	//*****************************************************************************
	//	CGUIEventUI
	//*****************************************************************************
	/**
	* @class CGUIEventUI
	* @brief event for 
	*	-eEVENT_UI
	*/
#define MAX_UIEVENT_ARGS	9
	class GUIEXPORT CGUIEventUI: public CGUIEvent
	{
	public:
		CGUIEventUI();

		virtual uint32 Process();

		void SetExtraData( void* pData );
		void* GetExtraData( ) const;
		
		void SetUIName( const CGUIString& rName );
		const CGUIString& GetUIName( ) const;

		void SetArg(int nIndex, const CGUIString& rArg );
		const CGUIString& GetArg(int nIndex) const;

	protected:
		void* m_pExtraData;
		CGUIString m_strUIEventName;

		CGUIString m_pArgs[MAX_UIEVENT_ARGS];

	};



	//*****************************************************************************
	//	CGUIEventRelativeChange
	//*****************************************************************************
	/**
	* @class CGUIEventRelativeChange
	* @brief event for 
	*	-eEVENT_ADD_CHILD,
	*	-eEVENT_REMOVE_CHILD,
	*	-eEVENT_PARENT_CHANGED,
	*/
	class GUIEXPORT CGUIEventRelativeChange: public CGUIEvent
	{
	public:
		CGUIEventRelativeChange();

		virtual uint32 Process();

		void SetRelative( CGUIWidget* pRelative );
		CGUIWidget*	GetRelative( ) const;

	protected:
		CGUIWidget* m_pRelative;	//maybe child or parent
	};


}//namespace guiex

#endif //__GUI_EVENT_20060612_H__
