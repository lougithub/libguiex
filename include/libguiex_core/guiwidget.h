/** 
* @file guiwidget.h
* @brief base class, define most operation of widget
* @author ken
* @date 2006-06-07
*/

#ifndef __GUI_WIDGET_20060607_H__
#define __GUI_WIDGET_20060607_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guievent.h"
#include "guirect.h"
#include "guinode.h"
#include "guicolorrect.h"
#include "guistringex.h"
#include "guistring.h"
#include "guitimer.h"
#include "guicolorrect.h"
#include "guiproperty.h"
#include "guiimage.h"
#include "guiwidgetparameter.h"
#include "guirenderrect.h"

#include <bitset>
#include <vector>
#include <map>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidgetGenerator;
	class IGUIInterfaceRender;
	class CGUIImage;
	class CGUIAnimation;
	class CGUIAs;

	typedef	void (*CallbackEventFunc)(CGUIEvent* pEvent);
}





//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWidget
	* @brief base class of widget 
	*/
	class GUIEXPORT CGUIWidget : public CGUINode
	{
	public:
		/**
		* @brief default constructor
		*/
		CGUIWidget(const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName  );

		/**
		* @brief default destructor
		*/
		virtual ~CGUIWidget();

		/**
		* @brief create this widget
		* @return return 0 for success.
		*/
		virtual int32 Create();

		/**
		* @brief open the widget
		*/
		virtual void	Open();

		/**
		* @brief open the widget
		*/
		bool	IsOpen() const;

		/**
		* @brief close the widget
		* now just send a close event and make this widget invisible
		*/
		virtual void	Close();

		////////////////////////////////////////////////////////////////////////////
		// tree function
		////////////////////////////////////////////////////////////////////////////

		/**
		* @brief set parent
		*/
		void			SetParent( CGUIWidget *pParent);

		/**
		* @brief get parent
		*/
		CGUIWidget *	GetParent()  const;

		/**
		* @brief get root widget of this widget.
		*/
		CGUIWidget *	GetRoot();

		/// get child 
		CGUIWidget*	GetChild( ) const;

		/// get child by given name
		CGUIWidget*	GetChild( const CGUIString& rChildName ) const;

		/// get next sibling
		CGUIWidget*	GetNextSibling( ) const;

		//get exclusive child
		CGUIWidget*	GetExclusiveChild( ) const;

		/**
		* @brief move this widget to the top between
		* his children.
		*/
		void	MoveToTop();

		void	MoveDown();
		void	MoveUp();


		//!< get widget name
		const CGUIString&	GetName() const
		{
			return m_strName;
		}

		//!< get project name
		const CGUIString&	GetProjectName( ) const
		{
			return m_strOwnerProjectName;
		}

		//!< set working projname for this widget and all of it's child
		void	SetWorkingProjectName(const CGUIString& rWorkingProjName);

		//!< get working project name
		const CGUIString&	GetWorkingProjectName( ) const
		{
			return m_strWorkingProjectName;
		}

		/**
		* @brief get type
		*/
		const CGUIString& GetType() const
		{
			return m_strType;
		}


		/// set user data
		void	SetUserData(void*	pData);

		//get user data
		void*	GetUserData() const;

		////////////////////////////////////////////////////////////////////////////
		// set value, or send command by string
		////////////////////////////////////////////////////////////////////////////
		/** 
		* @brief set widget parameter's value
		* for example: SetValue("alpha", "0.5")
		*/
		virtual void	SetValue(const CGUIString& rName, const CGUIString& rValue);

		/**
		* @brief get widget parameter's value
		* for example: GetValue("alpha_local") will return "0.5"
		*/
		virtual CGUIString	GetValue(const CGUIString& rName) const;


		////////////////////////////////////////////////////////////////////////////
		// as
		////////////////////////////////////////////////////////////////////////////
		/** 
		 * @brief add a action sequence to as list
		 */
		void	AddAs( CGUIAs* pAs);
			

		////////////////////////////////////////////////////////////////////////////
		// text about function
		////////////////////////////////////////////////////////////////////////////

		/**
		* @brief get the tooltips of this widget.
		*/
		const CGUIStringEx&	GetTooltipText(void) const;

		/**
		* @brief set the tooltips of this widget.
		*/
		void		SetTooltipText(const CGUIStringEx& rText);

		virtual void	SetTextColor(const CGUIColor& rColor);


		/** 
		* @brief set widget text
		*/
		virtual void	SetTextContent(const wchar_t* pText);

		/**
		* @brief get widget text value
		*/
		virtual const wchar_t*	GetTextContent() const;

		virtual bool	IsTextContentEmpty( ) const;

		/** 
		* @brief set widget text
		*/
		void	SetTextContentUTF8( const CGUIString& rString);

		/**
		* @brief get widget text value
		*/
		CGUIString	GetTextContentUTF8() const;

		virtual void	SetTextAlignment( uint8 uAlignment );

		virtual uint8  GetTextAlignment( ) const;

		/** 
		* @brief set widget text info
		*/
		virtual void						SetTextInfo(const CGUIStringExInfo& rInfo);

		/** 
		* @brief get widget text info
		*/
		virtual const CGUIStringExInfo&		GetTextInfo( ) const;


		/**
		* @brief get widget text value
		*/
		virtual const CGUIStringEx&		GetText() const;

		////////////////////////////////////////////////////////////////////////////
		// flag
		////////////////////////////////////////////////////////////////////////////
		/**
		* @brief flag name
		*/
		enum
		{
			/******* status of widget ***************************/
			//eFLAG_OPEN = 0,				///!< has this widget been opened
			eFLAG_HIDE=0,					//!< is this widget hidden by parent

			/******* ability of widget **************************/
			eFLAG_INHERIT_ALPHA,
			eFLAG_FOCUS_AGENCY,				//!< this widget is an agency of it's parent
			eFLAG_FOCUSABLE,				//!< could this widget be set focus
			eFLAG_HITABLE,					//!< could this widget be hitted
			eFLAG_MOVABLE,					//!< could this widget be dragged by mouse
			eFLAG_EXCLUSIVE,				//!< is this widget a exclusive widget		
			eFLAG_SCROLLBAR_AUTOPOSITION,	//!< could this widget auto-position scroll bar
			eFLAG_OPEN_WITH_PARENT,			//!< should this widget be opened with parent's open
			eFLAG_PARENT_CLIENTRECT,		//!< use parent client rect or whole rect as a client rect
			eFLAG_MOUSE_CONSUMED,			//!< should this mouse consume mouse event

			/******* whether receive this kind of event *********/
			eFLAG_EVENT_CLICK,				//!< could this widget generate click event
			eFLAG_EVENT_DBCLICK,			//!< could this widget generate double click event
			eFLAG_EVENT_MULTICLICK,			//!< could this widget generate multi click event
			eFLAG_EVENT_UPDATE,				//!< could this widget generate update event
			eFLAG_EVENT_PARENTSIZECHANGE,	//!< could this widget receive parent change event

			eFLAG_MAX,
		};

		/// set this widget activable
		void	SetSelfActivable(bool bActivable);

		/// is this widget activable
		bool	IsSelfActivable() const;

		/// is this widget activable
		bool	IsActivable();

		/// set this widget focusable
		void	SetFocusable(bool bFocusable);

		/// is this widget focusable
		bool	IsFocusable() const;

		/// set this widget focus
		void	SetFocus(bool bFocus);

		/// whether this widget has focus
		bool	IsFocus() const;

		/// set this widget visible
		void	SetSelfVisible(bool bVisible);

		/// is this widget visible
		bool	IsSelfVisible( ) const;

		/// is this widget visible
		bool	IsVisible();

		/**
		* @brief hide this widget 
		*/
		void	Hide( );

		/**
		* @brief show this widget 
		*/
		void	Show( );



		///set this widget disable
		void	SetSelfDisable(bool bDisable);

		/// is this widget disable
		bool	IsSelfDisable( ) const;

		/// is this widget disable, the value will be affect by the parent's state
		bool	IsDisable();


		///set flag
		void	SetFlag( uint32	nFlag, bool bEnable);

		///get flag
		bool	GetFlag( uint32	nFlag) const;

		/// set widget alpha
		void	SetSelfAlpha( real fAlpha );

		/// get widget alpha
		real	GetSelfAlpha()  const;

		/**
		* @brief get the alpha which used to render this widget.
		* this value will be affected by both this widget's alpha and inheritable flag.
		*/
		real	GetAlpha() ;
 

		/// set scale 
		void	SetSelfScale( const CGUISize& );

		/// get scale
		const CGUISize&	GetSelfScale() const;

		/// get final scale
		const CGUISize&	GetScale();

		//!< set color
		void	SetColor(const CGUIColor& rColor )
		{
			m_aColor = rColor;
		}

		//!< get color
		const CGUIColor&	GetColor() const
		{
			return m_aColor;
		}

		/**
		* @brief give the child which is pinted
		*/
		CGUIWidget*		GetWidgetAtPoint(const CGUIVector2& rPos);

		/**
		* @brief test whether the point is in this widget
		* @param rPos in pixel format
		*/
		virtual bool	HitTest( const CGUIVector2& rPos);

		/**
		* @brief render this widget, 
		* it will render this widget and all children.
		* @param pRender current render
		*/
		virtual void	Render( IGUIInterfaceRender* pRender );

		/**
		* @brief update this widget, 
		* it will update this widget and all children.
		* @param pRender current render
		*/
		virtual void	Update();



		////////////////////////////////////////////////////////////////////////////
		// image related operation
		////////////////////////////////////////////////////////////////////////////
		/**
		* @brief set the image of widget.
		* @param pImage must be created by CGUIImageManager
		*/
		void	SetImage( const CGUIString& rName, CGUIImage* pImage );

		/**
		* @brief set the image of widget.
		* @param rImageName name of the image
		*/
		CGUIImage*	SetImage( const CGUIString& rName, const CGUIString& rImageName );

		/**
		* @brief add an  animation
		*/
		void	SetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation );

		/**
		* @brief add an  animation
		*/
		CGUIAnimation*	SetAnimation( const CGUIString& rName, const CGUIString& rAnimationName );

		/**
		* @brief get an animation by name
		*/
		CGUIAnimation*	GetAnimation( const CGUIString& rAnimationName);

		/**
		* @brief has the image been load.
		*/
		bool		HasImage( const CGUIString& rName );

		/**
		* @brief get image info
		* @return NULL for failed to find image by given name
		*/
		CGUIImage*	GetImage( const CGUIString& rName );


		/**
		* @brief destroy all image and animation
		*/
		void	DestroyAllResource( );

		////////////////////////////////////////////////////////////////////////////
		// property related operation
		////////////////////////////////////////////////////////////////////////////
		/**
		* @brief set property set of this widget, if this widget has gotten the 
		*	property, the old property will be destroyed.
		*/
		void			SetPropertySet( const CGUIPropertySet&	rProperty);

		/**
		* @brief set property of this widget, if this widget has exist, reset it.
		* if,this widget hasn't this property, add it
		*/
		void			SetProperty( const CGUIProperty&	rProperty);

		/**
		* @brief clear current property
		*/
		void					ClearProperty();

		/**
		* @brief get property of this widget
		*/
		const CGUIPropertySet&	GetProperty() const;

		/**
		* @brief load widget config from property
		*/
		void					LoadProperty();

		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty*	GenerateProperty(const CGUIString& rName, const CGUIString& rType );

		/** 
		 * @brief process property
		 */
		virtual void			ProcessProperty( const CGUIProperty* pProperty);

	public:
		/// register ui event
		void	RegisterUIEvent( const CGUIString& rUIEventName );
		void	UnregisterUIEvent( const CGUIString& rUIEventName );

		/// register sound
		void	RegisterSound( const CGUIString& strEventName, int32 nSoundIdx );

		/// unregister sound
		void	UnregisterSound( const CGUIString& strEventName );

		/// register a global callback function for event
		template<class TEVENT> void	RegisterGlobalCallbackFunc( 
			const CGUIString& strEventName, 
			void (*pFunc)(TEVENT*) );

		/// add a script callback function for event
		void	RegisterScriptCallbackFunc( 
			const CGUIString& strEventName, 
			const CGUIString& strFunc );

		/// unregister script function
		void	UnregisterScriptCallbackFunc(const CGUIString& strEventName);

		/// whether is there the script function for given event name
		bool	HasScriptCallbackFunc(const CGUIString& strEventName) const;

		/// get script function value
		CGUIString	GetScriptCallbackFunc(const CGUIString& strEventName) const;

		/// add a global timer function
		void RegisterGlobalTimerFunc( 
			uint32 nTimeGap, 
			const CGUIString& strEventName,  
			void (*pFunc)(CGUIEventTimer*) );

		/// add a script timer function
		void	RegisterScriptTimerFunc( 
			uint32 nTimeGap, 
			const CGUIString& strEventName, 
			const CGUIString& strFunc );

		/// unregister timer function for global function
		void	UnregisterGlobalTimerFunc(const CGUIString& strEventName);

		///unregister timer function for script function
		void	UnregisterScriptTimerFunc(const CGUIString& strEventName);

	public:
		////////////////////////////////////////////////////////////////////////////
		// widget operation - position
		////////////////////////////////////////////////////////////////////////////

		//!< get render vector list
		const CGUIRenderRect&	GetRenderRect();

		/**
		* @brief set whether position is relative
		*/
		void			SetPositionRelative( bool bRelative );

		/** 
		 * @brief get whether position is relative
		 */
		bool			IsPositionRelative() const;

		/**
		* @brief set whether size is relative
		*/
		void			SetSizeRelative( bool bRelative );

		/** 
		* @brief get whether size is relative
		*/
		bool			IsSizeRelative() const;

		/**
		* @brief update widget's size
		*/
		void			UpdateSize();

		/**
		* @brief client coordinate to global coordinate
		*/
		void ClientToGlobal( CGUIVector2& rPos );

		/**
		* @brief global coordinate to client coordinate
		*/
		void GlobalToClient( CGUIVector2& rPos );

		/**
		* @brief client coordinate to global coordinate
		*/
		void LocalToWorld( CGUIVector2& rPos );

		/**
		* @brief global coordinate to client coordinate
		*/
		void WorldToLocal( CGUIVector2& rPos );

		/**
		* @brief client coordinate to global coordinate
		*/
		void LocalToWorld( const CGUIRect& rRect, CGUIRenderRect& rRenderRect );

		/**
		* @brief parent coordinate to global coordinate
		*/
		void ParentToWorld( CGUIVector2& rPos );

		/**
		* @brief global coordinate to parent coordinate
		*/
		void WorldToParent( CGUIVector2& rPos );

		const CGUIRect&				GetRect();								//get widget rect, global
		void						SetRect(const CGUIRect& rRect);			//set widget rect, global
		virtual	 const CGUIRect&	GetClientRect();						//get rect for client, global
		virtual  const CGUIRect&	GetClipRect();							//get clip rect, global
		virtual  const CGUIRect&	GetClientClipRect();					//get clip rect for client, global


		const CGUIVector2&	GetGlobalPosition();							//get widget global position
		void				SetGlobalPosition(const CGUIVector2&rPos);		//set widget global position

		const CGUIVector2&	GetLocalPosition();								//get widget position, local, relative to his parent
		void				SetLocalPosition(const CGUIVector2&rPos);			//set widget position, local, from tag point to tag point
		void				SetLocalPosition(real x, real y);				//set widget position, local, from tag point to tag point

		const CGUIVector2&	GetPositionRatio();							//get widget position ratio, compared with parent,value is from 0.0 to 1.0 normal
		void				SetPositionRatio(const CGUIVector2&rPos);	//set widget position ratio, compared with parent,value is from 0.0 to 1.0 normal
		void				SetPositionRatio(real x, real y);			//set widget position ratio, compared with parent,value is from 0.0 to 1.0 normal

		const CGUISize&		GetSizeRatio();								//get widget relative size, compared with parent,value is from 0.0 to 1.0 normal
		void				SetSizeRatio(const CGUISize&rSize);			//set widget relative size, compared with parent,value is from 0.0 to 1.0 normal

		const CGUIVector2&	GetTagPoint();								//get tag point, the value of it is from 0.0f to 1.0f
		void				SetTagPoint(const CGUIVector2&rTagPoint);		//set tag point
		void				SetTagPoint(real x, real y);				//set tag point

		const CGUISize&		GetSize() ;									//get widget size
		void				SetSize(const CGUISize&rSize);				//set widget size
		void				SetSize(real width, real height);			//set widget size
		void				SetRectSize(const CGUISize&rSize);			//set widget rect size, it's the size showing in the screen now
		void				SetRectSize(real width, real height);		//set widget rect size, it's the size showing in the screen now


		void				SetMaximumSize(const CGUISize& rSize);		//set maximum size of this widget
		const CGUISize&		GetMaximumSize() const;						//get maximum size of this widget
		void				SetMinimumSize(const CGUISize& rSize);		//set minimum size of this widget
		const CGUISize&		GetMinimumSize() const;						//get minimum size of this widget

		void				SetRectDirty();		//set dirty flag for rect size and position

	protected:
		void				OnUpdatedFromParent(void);

		void				ResetRectDirty()	//reset dirty flag
		{
			m_bRectDirtyFlag = false;
		}
		bool				IsRectDirty()		//get dirty flag
		{
			return m_bRectDirtyFlag;
		}

		void				UpdateWidgetRect();		//update rect if it is dirty, return true if this dirty rect is updated
		virtual void		PreUpdateDirtyRect();	//update dirty rect, same way for all widgets
		virtual void		UpdateDirtyRect();		//update rect, which may different for different widget
		virtual void		PostUpdateDirtyRect();	//update dirty rect, same way for all widgets

		void				PreUpdateDirtyRect_Imp(const CGUIRect& rParentRect, const CGUISize& rParentSize);


	protected:
		/// for global function defined in c++
		void	CallGlobalFunction(const CGUIString& strEventName, CGUIEvent* pEvent);

		///for global function defined in script
		void	CallScriptFunction(const CGUIString& strEventName, CGUIEvent* pEvent);

		///call callback function
		void	CallbackFunction(const CGUIString& strEventName, CGUIEvent* pEvent);



	protected:

		/**
		* @brief render this widget only
		*/
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief update this widget only
		*/
		virtual void	UpdateSelf();

		/** 
		 * @brief update action sequence
		 */
		void			UpdateAs();

		/**
		* @brief draw a string
		*/
		void	CGUIWidget::DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringEx& strText,
			const CGUIRect& rDrawRect,
			uint8 uTextAlignment,
			const CGUIRect* pClipRect,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		void	CGUIWidget::DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringEx& strText,
			const CGUIVector2& rPos,
			const CGUIRect* pClipRect,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		/**
		* @brief draw a image
		* @param rDestRect in default format
		* @param rClipRect in default format
		*/
		void	DrawImage(
			IGUIInterfaceRender* pRender,
			CGUIImage* pImage, 
			const CGUIRect& rDestRect, 
			real z, 
			const CGUIRect* pClipRect);

		/**
		* @brief draw a image by given name
		* @param rDestRect in default format
		* @param rClipRect in default format
		*/
		void	DrawImage(
			IGUIInterfaceRender* pRender, 
			const CGUIString& rName, 
			const CGUIRect& rDestRect, 
			real z,
			const CGUIRect* pClipRect);

		
		/**
		* @brief draw a image
		* @param rDestRect in default format
		* @param rClipRect in default format
		*/
		void	DrawImage(
			IGUIInterfaceRender* pRender,
			CGUIImage* pImage, 
			const CGUIRenderRect& rRenderRect, 
			real z, 
			const CGUIRect* pClipRect);


		/**
		* @brief callback of set the image of widget.
		*/
		virtual void	OnSetImage( const CGUIString& rName, CGUIImage* pImage );



	protected://event
		//declare friend event
		//friend class CGUIEvent;
#define ITEM_BEGIN_TOKEN friend class 
#define ITEM_END_TOKEN ;
		EVENT_ITEM_LIST();
#undef ITEM_BEGIN_TOKEN
#undef ITEM_END_TOKEN

	protected:
		/// callback function: for change alpha
		virtual uint32		OnAlphaChange( CGUIEventAlpha* pEvent );
		/// callback function: for change size
		virtual uint32		OnSizeChange( CGUIEventSize* pEvent );
		virtual uint32		OnParentSizeChange( CGUIEventSize* pEvent );

		/// callback function: for mouse event
		virtual uint32		OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMove( CGUIEventMouse* pEvent );

		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDbClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftMultiClick( CGUIEventMouse* pEvent );

		virtual uint32		OnMouseRightDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightDbClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseRightMultiClick( CGUIEventMouse* pEvent );

		virtual uint32		OnMouseMiddleDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMiddleUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMiddleClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMiddleDbClick( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseMiddleMultiClick( CGUIEventMouse* pEvent );

		virtual int32		OnMouseWheel( CGUIEventMouse* pEvent );

		/// callback function: for drag
		virtual uint32		OnDragBegin( CGUIEventDrag* pEvent );
		virtual uint32		OnDragProcess( CGUIEventDrag* pEvent );
		virtual uint32		OnDragEnd( CGUIEventDrag* pEvent );


		/// callback function: for scale change
		virtual uint32		OnScaleChange( CGUIEventNotification* pEvent );

		/// callback function: for active
		virtual uint32		OnActive( CGUIEventNotification* pEvent );
		virtual uint32		OnInactive( CGUIEventNotification* pEvent );

		/// callback function: for focus
		virtual uint32		OnGetFocus( CGUIEventNotification* pEvent );
		virtual uint32		OnLostFocus( CGUIEventNotification* pEvent );

		/// callback function: for visable
		virtual uint32		OnVisible( CGUIEventNotification* pEvent );
		virtual uint32		OnInvisible( CGUIEventNotification* pEvent );

		/// callback function: for enable
		virtual uint32		OnEnable( CGUIEventNotification* pEvent );
		virtual uint32		OnDisable( CGUIEventNotification* pEvent );

		/// callback function: for open and close
		virtual uint32		OnLoad( CGUIEventNotification* pEvent );
		virtual uint32		OnOpen( CGUIEventNotification* pEvent );
		virtual uint32		OnClose( CGUIEventNotification* pEvent );

		/// callback function: for ui event
		virtual uint32		OnUIEvent( CGUIEventUI* pEvent );

		/// callback function: for check and uncheck event
		virtual uint32		OnChecked( CGUIEventNotification* pEvent );
		virtual uint32		OnUnchecked( CGUIEventNotification* pEvent );

		/// callback function: for update event
		virtual int32		OnUpdate(CGUIEventNotification* pEvent );

		/// callback function: for scrollbar's attach and detach
		virtual uint32		OnScrollbarScroll( CGUIEventScrollbar* pEvent );


		/// callback function: for keyboard
		virtual uint32		OnKeyDown( CGUIEventKeyboard* pEvent );
		virtual uint32		OnKeyUp( CGUIEventKeyboard* pEvent );
		virtual uint32		OnKeyPressed( CGUIEventKeyboard* pEvent );
		virtual uint32		OnKeyClicked( CGUIEventKeyboard* pEvent );

		/// callback function: for timer
		virtual uint32		OnTimer( CGUIEventTimer* pEvent );

		/// callback function: for listbox
		virtual uint32		OnListBoxSelected( CGUIEventListBox* pEvent );
		virtual uint32		OnListBoxSelectChanged( CGUIEventListBox* pEvent );
		virtual uint32		OnListBoxLeftDbClick( CGUIEventListBox* pEvent );
		virtual uint32		OnListBoxRightDbClick( CGUIEventListBox* pEvent );
		virtual uint32		OnListBoxLeftClick( CGUIEventListBox* pEvent );
		virtual uint32		OnListBoxRightClick( CGUIEventListBox* pEvent );


		/// callback function: for combobox
		virtual uint32		OnComboBoxSelected(CGUIEventComboBox* pEvent);

		/// callback function: for relative changes
		virtual uint32		OnAddChild( CGUIEventRelativeChange* pEvent );
		virtual uint32		OnRemoveChild( CGUIEventRelativeChange* pEvent );
		virtual uint32		OnChangeParent( CGUIEventRelativeChange* pEvent );

	protected:
		///widget generator
		friend class CGUIWidgetFactory;
		///set generator
		void	SetGenerator( const CGUIWidgetGenerator* pGenerator);
		///get generator
		const	CGUIWidgetGenerator* GetGenerator() const;

		/**
		* @brief remove child 
		*/
		void	RemoveChild( CGUIWidget* pWidget );

		/**
		* @brief add child 
		*/
		void	AddChild( CGUIWidget* pWidget );

		CGUIWidget*	GetLastChild( );

		void	RegisterPropertyableObject( const CGUIString& rName, uint32 uPropertyType, CGUIPropertyable* pPropertyableObj );
		CGUIPropertyable*	FindPropertyableObject( const CGUIString& rName, uint32 uPropertyType );


	protected:


	protected:
		const CGUIWidgetGenerator* m_pWidgetGenerator;	//!< generator which used to create widget


		///////////////////////////////////////////////////////////////////////
		/// for flag										
		///////////////////////////////////////////////////////////////////////
		std::bitset<eFLAG_MAX>		m_aBitFlag;	//!< flag, use bit to save result.
		bool						m_bEvent_ChangeParentSize;


		///////////////////////////////////////////////////////////////////////
		/// parameter							
		///////////////////////////////////////////////////////////////////////
		CGUIWidgetParamMultiply<CGUISize>	m_aParamScale;
		CGUIWidgetParamMultiply<real>	m_aParamAlpha;
		CGUIWidgetParamOR<bool>			m_aParamDisable;
		CGUIWidgetParamAND<bool>		m_aParamVisible;
		CGUIWidgetParamOR<bool>			m_aParamActivable;
		CGUIColor						m_aColor;

		bool			m_bIsOpen;		///widget state

		///////////////////////////////////////////////////////////////////////
		/// widget size and position								
		///////////////////////////////////////////////////////////////////////
		bool			m_bRelativePos;
		bool			m_bRelativeSize;			
		CGUIRect		m_aWidgetRect;				//global rect
		CGUIRect		m_aClientRect;				//client rect, in global dimension
		CGUIRect		m_aClipRect;				//clip rect, in global dimension
		CGUIRect		m_aClientClipRect;			//clip rect for client,in global dimension


		CGUISize		m_aWidgetSize;				//widget size
		CGUISize		m_aSizeRatio;				//relative size compared with parent

		CGUIVector2		m_aGlobalPos;				//widget global position by tag point
		CGUIVector2		m_aWidgetClientPosGlobal;	//client rect position, global,
		CGUIVector2		m_aLocalPos;				//widget local position by tag point
		CGUIVector2		m_aWidgetTagPoint;			//tag point, the value is from 0.0f to 1.0f
		CGUIVector2		m_aPosRatio;				//relative position compared with parent by tag point

		CGUISize		m_aMaxSize;					//maximum size of widget,it doesn't work if the value is (0,0)
		CGUISize		m_aMinSize;					//minimum size of widget,it doesn't work if the value is (0,0)

		bool			m_bRectDirtyFlag;

		CGUIRenderRect		m_aRenderRect;		//!< contain render vertex and it's parameter

		///////////////////////////////////////////////////////////////////////
		/// widget callback function								
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString,CallbackEventFunc> TMapGlobalFunc;
		TMapGlobalFunc m_mapGlobalFunc;		//!<event's global callback function

		typedef std::map<CGUIString,CGUIString> TMapScriptFunc;
		TMapScriptFunc m_mapScriptFunc;		//!<event's script callback function


		///////////////////////////////////////////////////////////////////////
		/// sound
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString,int32> TMapSound;
		TMapSound m_mapEventSound;		//!<sound for event


		///////////////////////////////////////////////////////////////////////
		/// Timer							
		///////////////////////////////////////////////////////////////////////
		struct STimer
		{
			CGUITimer			m_aPrevTime;
			uint32				m_uTimeGap;		// by millisecond
			STimer( uint32 uTimeGap )
				:m_uTimeGap(uTimeGap)
			{
			}
		};
		typedef std::map<CGUIString, STimer>		TMapTimer;
		TMapTimer			m_aMapTimer;	/// key is event name

		///////////////////////////////////////////////////////////////////////
		/// as							
		///////////////////////////////////////////////////////////////////////
		typedef std::vector<CGUIAs*> TListAs;
		TListAs	m_listAs;

		///////////////////////////////////////////////////////////////////////
		/// for image and animation											
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString, CGUIImage*>	TMapImage;
		TMapImage	m_aMapImage;	//!< image info map, key is image name used in ui system

		typedef std::map<CGUIString, CGUIAnimation*>	TMapAnimation;
		TMapAnimation	m_aMapAnimation;	//!< animation map, key is animation name used in ui system



		///////////////////////////////////////////////////////////////////////
		/// for text										
		///////////////////////////////////////////////////////////////////////
		CGUIStringEx			m_strTooltipText;	//!< tooltip for list box item.  
		CGUIStringEx			m_strText;				//!< text
		uint8					m_uTextAlignment;

		///////////////////////////////////////////////////////////////////////
		/// for widget tree											
		///////////////////////////////////////////////////////////////////////
		CGUIWidget*		m_pExclusiveChild;	//!< for exclusive child


		CGUIString		m_strType;			//!< widget type
		CGUIString		m_strName;			//!< widget name, should be a unique name in same project
		CGUIString		m_strOwnerProjectName;		//!< project name, should be a unique name
		CGUIString		m_strWorkingProjectName;	//!< working project name, should be a unique name

		void*			m_pUserData;		//!< user data, used by user.

		///////////////////////////////////////////////////////////////////////
		/// for widget property support
		///////////////////////////////////////////////////////////////////////
		CGUIPropertySet		m_aPropertySet;
		
		typedef std::vector<CGUIPropertyable*>	TArrayPropertyableObjects;
		TArrayPropertyableObjects		m_arrayPropertyableObjects;
		typedef std::map< std::pair<CGUIString, uint32>, CGUIPropertyable* > TMapPropertyableObjects;
		TMapPropertyableObjects			m_mapPropertyableObjects;
	};





	//inline function
	//------------------------------------------------------------------------------
	template<class TEVENT> 
	inline void	CGUIWidget::RegisterGlobalCallbackFunc( 
		const CGUIString& strEventName, 
		void (*pFunc)(TEVENT*))
	{
		m_mapGlobalFunc.insert( std::make_pair(strEventName, reinterpret_cast<CallbackEventFunc>(pFunc)));
	}
	//------------------------------------------------------------------------------

}//namespace guiex

#endif //__GUI_WIDGET_20060607_H__

