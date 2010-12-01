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
	enum	EEventId
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
// function
//============================================================================//
namespace guiex
{
	/**
	* @brief register all events
	* called by CGUISystem
	*/
	//extern int32 GUIRegisterAllEvents();
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
		/**
		* @brief constructor
		*/
		CGUIEvent(const char* pEventName);

		/**
		* @brief destructor
		*/
		virtual ~CGUIEvent();

		/**
		* @brief get event id
		* @return event id in int32 type
		*/
		uint32 GetEventId() const;

		/**
		* @brief set event id
		*/
		void	SetEventId(uint32 uId );

		/**
		* @brief get event type
		*/
		const CGUIString&		GetEventType() const;

		/**
		* @brief set widget which receives this event
		*/
		void	SetReceiver(CGUIWidget* pReceiver);

		/**
		* @brief Get widget which receives this event
		*/
		CGUIWidget*	GetReceiver();

		//!< has this event been consumed
		bool	IsConsumed();

		//!< set whether this event has been consumed
		void	Consume(bool bConsumed);

		/**
		* @brief process the event
		*/
		virtual uint32	Process() = 0;

		///is this event created by CGUIEventFactory
		//bool	IsCreateByFactory() const
		//{
		//	return m_pEventGenerator!=NULL;
		//}

	protected:
		///widget generator
		friend class CGUIEventFactory;
		///set generator
		//void	SetGenerator( const CGUIEventGenerator* pGenerator);
		///get generator
		//const CGUIEventGenerator* GetGenerator() const;

	protected:
		/// id of the event
		uint32		m_uEventId;

		///type of this event
		CGUIString	m_strEventType;

		///receiver
		CGUIWidget*	m_pReceiver;

		/// has this event been consumed
		bool		m_bConsumed;

	private:
		/**
		* @brief generator which used to create event
		*/
		//const CGUIEventGenerator*	m_pEventGenerator;
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
		/**
		* @brief constructor
		*/
		CGUIEventScrollbar();

		/**
		* @brief process the event.
		* now it works for 
		*	- <eEVENT_SCROLLBAR_SCROLL>
		*/
		virtual uint32	Process();

		/// set type of scrollbar
		void	SetScrollbarType( bool bVertical );

		/// is a vertical scrollbar 
		bool	IsVertical() const;

		/// is a horizontal scrollbar
		bool	IsHorizontal() const;

		/// set scrollbar
		void	SetScrollbar(CGUIWidget * pScrollbar);

		/// get scrollbar
		CGUIWidget*	GetScrollbar() const;

		/// set scrollbar value
		void	SetCurrentPos( uint32 nPos );

		///get scrollbar value
		uint32 GetCurrentPos() const;

		/// set scrollbar range
		void	SetRange( uint32 nRange );

		///get scrollbar range
		uint32 GetRange() const;

	protected:
		bool	m_bTypeVertical;				/// the type of scrollbar
		uint32	m_nCurrentPos;		/// current position
		uint32	m_nRange;			/// range
		CGUIWidget*	m_pScrollbar;	///scrollbar
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventScrollbar );



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

		/**
		* @brief process the event.
		* now it works for 
		*	- <eEVENT_TIMER>,
		*/
		virtual uint32	Process();

		void	SetTimerName(const CGUIString& rTimerName);

		const CGUIString&	GetTimerName();

	protected:
		CGUIString				m_strTimerName;
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventTimer );




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
		/**
		* @brief constructor
		*/
		CGUIEventNotification();

		/**
		* @brief process the event.
		* now it works for 
		*	- <eEVENT_ACTIVE>,
		*	- <eEVENT_INACTIVE>,
		*	- <eEVENT_FOCUS_LOST>,
		*	- <eEVENT_FOCUS_GET>,
		*	- <eEVENT_VISIBLE>,
		*	- <eEVENT_INVISIBLE>
		*	- <eEVENT_DISABLE>,
		*	- <eEVENT_ENABLE>,
		*	- <eEVENT_CREATE>,
		* - <eEVENT_UPDATE>
		*/
		virtual uint32	Process();


	protected:
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventNotification );





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
		/**
		* @brief constructor
		*/
		CGUIEventSize();

		/**
		* @brief get size
		*/
		const CGUISize& GetSize() const;

		/**
		* @brief set size
		*/
		void	SetSize(const CGUISize& rSize);

		/**
		* @brief process the event.
		* now it works for <eEVENT_SIZE_CHANGE>,
		*/
		virtual uint32	Process();

	protected:
		CGUISize		m_aSize;
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventSize );


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
		/**
		* @brief constructor
		*/
		CGUIEventAlpha();

		/**
		* @brief get alpha
		*/
		real GetAlpha() const;

		/**
		* @brief set alpha
		*/
		void SetAlpha(real fAlpha);

		/**
		* @brief process the event.
		* now it works for <eEVENT_CHANGE_ALPHA>,
		*/
		virtual uint32	Process();

	protected:
		real		m_fAlpha;
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventAlpha );


	//*****************************************************************************
	//	CGUIUserDefineEvent
	//*****************************************************************************





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
		/**
		* @brief constructor
		*/
		CGUIEventMouse();

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

		/**
		* @brief set mouse position, in global
		*/
		void		SetPosition( const CGUIVector2& rPos );

		/**
		* @brief get mouse position, in global
		*/
		const CGUIVector2& GetGlobalPosition() const;

		/**
		* @brief get mouse position, in global
		*/
		CGUIVector2 GetLocalPosition() const;

		/**
		* @brief set system key
		*/
		void		SetSysKeys(uint32 uSyskeys);

		/**
		* @brief get system key
		*/
		uint32		GetSysKeys() const;

		/**
		* @brief set changes of wheel
		*/
		void		SetWheelChange(real fWheelChange);

		/**
		* @brief get changes of wheel
		*/
		real		GetWheelChange( );

		/**
		* @brief set count of click
		*/
		void		SetMultiClickCount( uint32 nCount );

		/**
		* @brief get count of click
		*/
		uint32		GetMultiClickCount( ) const;

		/**
		* @brief set button
		*/
		void		SetButton(int32 nButton);

		/**
		* @brief Get button
		*/
		int32		GetButton() const;

		///get keyboard interface
		IGUIInterfaceKeyboard* GetKeyboardInterface() const;

		///set keyboard interface
		void	SetKeyboardInterface( IGUIInterfaceKeyboard* pKeyboard );

	protected:
		uint32			m_nButton;			//!< mouse's button
		uint32			m_nClickCount;		//!< click count
		CGUIVector2		m_aPosition;		//!< holds current mouse position.
		CGUIVector2		m_aMoveDelta;		//!< holds variation of mouse position from last mouse input
		uint32			m_uSysKeys;			//!< current state of the system keys and mouse buttons.
		real			m_fWheelChange;		//!< Holds the amount the scroll wheel has changed.

		IGUIInterfaceKeyboard* m_pKeyboard;
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventMouse );






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
		/**
		* @brief constructor
		*/
		CGUIEventKeyboard();

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

		/// set key code
		void	SetKeyCode( int32 nKeyCode);

		///get key code
		int32	GetKeyCode() const;

		///get keyboard interface
		IGUIInterfaceKeyboard* GetKeyboardInterface() const;

		///set keyboard interface
		void	SetKeyboardInterface( IGUIInterfaceKeyboard* pKeyboard );

	protected:
		int32	m_nKeyCode;		/// key code
		IGUIInterfaceKeyboard* m_pKeyboard;
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventKeyboard);





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
		/**
		* @brief constructor
		*/
		CGUIEventDrag();

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

		/**
		* @brief set widget position, local
		*/
		void SetWidgetPos( const CGUIVector2& rPos );

		/**
		* @brief set mouse position, global
		*/
		void SetMousePos( const CGUIVector2& rPos );

		/**
		* @brief get widget position
		*/
		CGUIVector2 GetWidgetGlobalPos() const;
		const CGUIVector2& GetWidgetLocalPos() const;

		/**
		* @brief get mouse position
		*/
		const CGUIVector2& GetMouseGlobalPos() const;
		CGUIVector2 GetMouseLocalPos() const;

		/**
		* @brief set button
		*/
		void SetButton( int32 nButton);

		/**
		* @brief get mouse button
		*/
		const int32 GetButton() const;

		//!< has this event been consumed
		bool IsExpired();

		//!< set whether this event has been consumed
		void Expire(bool bExpired);

	protected:
		CGUIVector2		m_aWidgetPos;		//!< holds current widget position. local pos
		CGUIVector2		m_aMousePos;		//!< holds current mouse position. global pos
		int32			m_nButton;			//!< mouse button

		bool			m_bExpired;			//!< whether this event is been expired
	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventDrag );





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
		/**
		* @brief constructor
		*/
		CGUIEventListBox();

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

		/// set index of item which is double clicked
		void			SetDbClickedItemIdx( uint32 nIdx );

		/// get index of item which is double clicked
		uint32			GetDbClickedItemIdx( ) const;

		/// set index of item which is double clicked
		void			SetSelectedItemIdx( uint32 nIdx );

		/// get index of item which is double clicked
		uint32			GetSelectedItemIdx( ) const;

	protected:
		uint32		m_nDbClickedItemIdx;		/// the index of item which is double clicked
		uint32		m_nSelectedItemIdx;		/// the index of item which is selected

	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventListBox);



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
		/**
		* @brief constructor
		*/
		CGUIEventComboBox();

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

		/// set index of item which is double clicked
		void				SetSelectedItemIdx( uint32 nIdx );

		/// get index of item which is double clicked
		uint32		GetSelectedItemIdx( ) const;

	protected:
		uint32		m_nSelectedItemIdx;		/// the index of item which is selected

	};
	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventComboBox);



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
		/**
		* @brief constructor
		*/
		CGUIEventUI();

		/**
		* @brief process the event.
		*/
		virtual uint32	Process();

		/// set extra data for this event
		void			SetExtraData( void* pData );

		/// get extra data for this event
		void*			GetExtraData( ) const;

		/// set name of this ui event
		void			SetUIName( const CGUIString& rName );

		/// get name of this ui event
		const CGUIString&	GetUIName( ) const;

		void			SetArg(int nIndex, const CGUIString& rArg );
		const CGUIString&	GetArg(int nIndex) const;

	protected:
		void*			m_pExtraData;
		CGUIString		m_strUIEventName;

		CGUIString			m_pArgs[MAX_UIEVENT_ARGS];

	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventUI);




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
		/**
		* @brief constructor
		*/
		CGUIEventRelativeChange();

		/**
		* @brief process the event.
		*/
		virtual uint32		Process();


		/// set extra data for this event
		void				SetRelative( CGUIWidget* pRelative );

		/// get extra data for this event
		CGUIWidget*	GetRelative( ) const;

	protected:
		CGUIWidget*			m_pRelative;	//maybe child or parent

	};

	//GUI_EVENT_GENERATOR_DECLARE( CGUIEventRelativeChange);



}//namespace guiex






#endif //__GUI_EVENT_20060612_H__


