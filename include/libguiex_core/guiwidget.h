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
#include "guiwidgetparameter.h"
#include "guirenderrect.h"
#include "guitypes.h"
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

	typedef void (*CallbackEventFunc)(CGUIEvent* pEvent);
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUIWidgetPosition
	{
	public:
		CGUIWidgetPosition()
			:m_eType(eScreenValue_Pixel)
		{
		}
		EScreenValue m_eType;
		CGUIVector2 m_aValue;
		CGUIVector2 m_aPixelValue;
	};

	class CGUIWidgetSize
	{
	public:
		CGUIWidgetSize()
			:m_eType(eScreenValue_Pixel)
		{
		}
		EScreenValue m_eType;
		CGUISize m_aValue;
		CGUISize m_aPixelValue;
	};

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
		CGUIWidget(const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName  );

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
		virtual void Open();

		/**
		* @brief open the widget
		*/
		bool IsOpen() const;

		/**
		* @brief close the widget
		* now just send a close event and make this widget invisible
		*/
		virtual void Close();

		////////////////////////////////////////////////////////////////////////////
		// tree function
		////////////////////////////////////////////////////////////////////////////

		/**
		* @brief set parent
		*/
		void SetParent( CGUIWidget *pParent);

		/**
		* @brief get parent
		*/
		CGUIWidget * GetParent()  const;

		/// get child 
		CGUIWidget* GetChild( ) const;

		/// get child by given name
		CGUIWidget* GetChild( const CGUIString& rChildName ) const;

		/// get next sibling
		CGUIWidget* GetNextSibling( ) const;

		//get exclusive child
		CGUIWidget* GetExclusiveChild( ) const;

		/**
		* @brief move this widget to the top between
		* his sibling.
		*/
		void MoveToTop();

		/**
		* @brief move down this widget between his sibling
		*/
		void MoveDown();

		/**
		* @brief move up this widget between his sibling
		*/
		void MoveUp();

		//!< get widget name
		const CGUIString& GetName() const
		{
			return m_strName;
		}

		//!< get scene name
		const CGUIString& GetSceneName( ) const
		{
			return m_strOwnerSceneName;
		}

		//!< set working scene name for this widget and all of it's child
		void SetWorkingSceneName(const CGUIString& rWorkingProjName);

		//!< get working scene name
		const CGUIString& GetWorkingSceneName( ) const
		{
			return m_strWorkingSceneName;
		}

		/**
		* @brief get type
		*/
		const CGUIString& GetType() const
		{
			return m_strType;
		}

		/// set user data
		void SetUserData(void* pData);

		//get user data
		void* GetUserData() const;

		////////////////////////////////////////////////////////////////////////////
		// as
		////////////////////////////////////////////////////////////////////////////
		/** 
		 * @brief add a action sequence to as list
		 */
		void AddAs( CGUIAs* pAs);
			

		////////////////////////////////////////////////////////////////////////////
		// text about function
		////////////////////////////////////////////////////////////////////////////

		/**
		* @brief get the tooltips of this widget.
		*/
		const CGUIStringEx& GetTooltipText(void) const;

		/**
		* @brief set the tooltips of this widget.
		*/
		void SetTooltipText(const CGUIStringEx& rText);

		virtual void SetTextColor(const CGUIColor& rColor);

		/** 
		* @brief set widget text
		*/
		virtual void SetTextContent(const wchar_t* pText);

		/**
		* @brief get widget text value
		*/
		virtual const wchar_t* GetTextContent() const;

		/**
		* @brief get whether text content is empty
		*/
		virtual bool IsTextContentEmpty( ) const;

		/** 
		* @brief set widget text
		*/
		void SetTextContentUTF8( const CGUIString& rString);

		/**
		* @brief get widget text value
		*/
		CGUIString GetTextContentUTF8() const;

		virtual void SetTextAlignment( uint8 uAlignment );

		virtual uint8 GetTextAlignment( ) const;

		/** 
		* @brief set widget text info
		*/
		virtual void SetTextInfo(const CGUIStringInfo& rInfo);

		/** 
		* @brief get widget text info
		*/
		virtual const CGUIStringInfo& GetTextInfo( ) const;

		/**
		* @brief get widget text value
		*/
		virtual const CGUIStringEx& GetText() const;

		////////////////////////////////////////////////////////////////////////////
		// flag
		////////////////////////////////////////////////////////////////////////////
		/**
		* @brief flag name
		*/
		enum
		{
			/******* whether receive this kind of event *********/
			eFLAG_EVENT_PARENTSIZECHANGE,	//!< could this widget receive parent change event

			eFLAG_MAX,
		};

		/// set this widget activable
		void SetSelfActivable(bool bActivable);

		/// is this widget activable
		bool IsActivable() const;

		/// is this widget activable
		bool IsDerivedActivable();

		/// set this widget focus
		void SetFocus(bool bFocus);

		/// whether this widget has focus
		bool IsFocus() const;

		/// set this widget visible
		void SetVisible(bool bVisible);

		/// is this widget visible
		bool IsVisible( ) const;

		/// is this widget visible in screen
		bool IsDerivedVisible();

		///set this widget disable
		void SetDisable(bool bDisable);

		/// is this widget disable
		bool IsDisable( ) const;

		/// is this widget disable, the value will be affect by the parent's state
		bool IsDerivedDisable();


		///set flag
		void SetFlag( uint32 nFlag, bool bEnable);

		///get flag
		bool GetFlag( uint32 nFlag) const;

		/// set widget alpha
		void SetAlpha( real fAlpha );

		/// get widget alpha
		real GetAlpha()  const;

		/**
		* @brief get the alpha which used to render this widget.
		* this value will be affected by both this widget's alpha and inheritable flag.
		*/
		real GetDerivedAlpha();
 

		/// set scale 
		void SetScale( const CGUISize& );

		/// get scale
		const CGUISize& GetScale() const;

		/// get final scale
		const CGUISize& GetDerivedScale();

		//!< set color
		void SetColor(const CGUIColor& rColor )
		{
			m_aColor = rColor;
		}

		//!< get color
		const CGUIColor& GetColor() const
		{
			return m_aColor;
		}

		/**
		* @brief give the child which is pinted
		*/
		CGUIWidget* GetWidgetAtPoint(const CGUIVector2& rPos);

		/**
		* @brief test whether the point is in this widget
		* @param rPos in pixel format
		*/
		virtual bool HitTest( const CGUIVector2& rPos);

		/**
		* @brief render this widget, 
		* it will render this widget and all children.
		* @param pRender current render
		*/
		void Render( IGUIInterfaceRender* pRender );
		void RenderExtraInfo(IGUIInterfaceRender* pRender);

		/**
		* @brief update this widget, 
		* it will update this widget and all children.
		* @param pRender current render
		*/
		virtual void Update( real fDeltaTime );



		////////////////////////////////////////////////////////////////////////////
		// image related operation
		////////////////////////////////////////////////////////////////////////////
		/**
		* @brief set the image of widget.
		* @param rImageName name of the image
		*/
		CGUIImage* SetImage( const CGUIString& rName, const CGUIString& rImageName );

		void SetImage( const CGUIString& rName, CGUIImage* pImage );

		/**
		* @brief has the image been load.
		*/
		bool HasImage( const CGUIString& rName );

		/**
		* @brief get image info
		* @return NULL for failed to find image by given name
		*/
		CGUIImage* GetImage( const CGUIString& rName );

		/**
		* @brief add an  animation
		*/
		void SetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation );

		/**
		* @brief add an  animation
		*/
		CGUIAnimation* SetAnimation( const CGUIString& rName, const CGUIString& rAnimationName );

		/**
		* @brief get an animation by name
		*/
		CGUIAnimation* GetAnimation( const CGUIString& rAnimationName);

		/**
		* @brief destroy all image and animation
		*/
		void DestroyAllResource( );

		////////////////////////////////////////////////////////////////////////////
		// property related operation
		////////////////////////////////////////////////////////////////////////////
		/**
		* @brief set property set of this widget, if this widget has gotten the 
		* property, the old property will be destroyed.
		*/
		void SetPropertySet( const CGUIProperty& rProperty);

		/**
		* @brief set property of this widget, if this widget has exist, reset it.
		* if,this widget hasn't this property, add it
		*/
		void SetProperty( const CGUIProperty& rProperty);

		/**
		* @brief clear current property
		*/
		void ClearProperty();

		/**
		* @brief get property of this widget
		*/
		const CGUIProperty& GetProperty() const;

		/**
		* @brief load widget config from property
		*/
		void LoadProperty();

		/**
		* @brief load widget config from property
		*/
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		/** 
		 * @brief process property
		 */
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		/** 
		* @brief set widget parameter's value
		* for example: SetValue("alpha", "0.5")
		*/
		virtual void SetValue(const CGUIString& rName, const CGUIString& rValue);

		/**
		* @brief get widget parameter's value
		* for example: GetValue("alpha_local") will return "0.5"
		*/
		virtual CGUIString GetValue(const CGUIString& rName) const;

	public:
		/// register ui event
		void RegisterUIEvent( const CGUIString& rUIEventName );
		void UnregisterUIEvent( const CGUIString& rUIEventName );

		/// register sound
		void RegisterSound( const CGUIString& strEventName, int32 nSoundIdx );

		/// unregister sound
		void UnregisterSound( const CGUIString& strEventName );

		/// register a global callback function for event
		template<class TEVENT> void RegisterGlobalCallbackFunc( 
			const CGUIString& strEventName, 
			void (*pFunc)(TEVENT*) );

		/// add a script callback function for event
		void RegisterScriptCallbackFunc( 
			const CGUIString& strEventName, 
			const CGUIString& strFunc );

		/// unregister script function
		void UnregisterScriptCallbackFunc(const CGUIString& strEventName);

		/// whether is there the script function for given event name
		bool HasScriptCallbackFunc(const CGUIString& strEventName) const;

		/// get script function value
		const CGUIString& GetScriptCallbackFunc(const CGUIString& strEventName) const;

		/// add a global timer function
		void RegisterGlobalTimerFunc( 
			real rWaitingTime, 
			const CGUIString& strEventName,  
			void (*pFunc)(CGUIEventTimer*) );

		/// add a script timer function
		void RegisterScriptTimerFunc( 
			real rWaitingTime, 
			const CGUIString& strEventName, 
			const CGUIString& strFunc );

		/// unregister timer function for global function
		void UnregisterGlobalTimerFunc(const CGUIString& strEventName);

		///unregister timer function for script function
		void UnregisterScriptTimerFunc(const CGUIString& strEventName);

	public:
		////////////////////////////////////////////////////////////////////////////
		// widget operation - position
		////////////////////////////////////////////////////////////////////////////

		const CGUIRect& GetBoundArea() const;
		//void SetBoundArea( const CGUIRect& rBoundsRect );

		const CGUIRect&	GetClientArea() const;
		const CGUIRect&	GetClipArea() const;

		CGUISize GetParentSize() const;

		void SetPosition( real x, real y );	//set widget position relative to parent, from anchor point to parent's top-left
		void SetPosition( const CGUIVector2&rPos );	//set widget position relative to parent, from anchor point to parent's top-left
		const CGUIVector2&	GetPosition() const; //get widget position in given value relative to parent
		void SetPixelPosition( const CGUIVector2& rPixelPos ); //set widget position in pixel format
		void SetPixelPosition( real x, real y ); //set widget position in pixel format
		const CGUIVector2&	GetPixelPosition() const; //get widget position in pixel format

		void SetPositionType( EScreenValue rValueType ); //set position type
		EScreenValue GetPositionType( ) const; //get position type

		void SetSize( real width, real height );	//set widget size, according size value type
		void SetSize( const CGUISize& rSize );	//set widget size, according size value type
		const CGUISize&	GetSize() const; //get widget size by given size type
		void SetPixelSize( real width, real height ); //set widget size in pixel.
		void SetPixelSize( const CGUISize& rPixelSize ); //set widget size in pixel.
		const CGUISize&	GetPixelSize() const; //get widget size in pixel.

		void SetSizeType( EScreenValue eValueType ); //set size type
		EScreenValue GetSizeType( ) const; //get size type

		void Refresh( );

		/**
		 * in degree
		 */
		void SetRotation(real x, real y, real z);
		void SetRotation(const CGUIVector3& rRotation);
		const CGUIVector3& GetRotation( ) const;

		const CGUIMatrix4& GetFullTransform(void);

		void LocalToWorld( CGUIVector2& rPos );
		void WorldToLocal( CGUIVector2& rPos );

		void LocalToWorld( CGUIVector3& rPos );
		void WorldToLocal( CGUIVector3& rPos );

		void ParentToWorld( CGUIVector2& rPos );
		void WorldToParent( CGUIVector2& rPos );

		void ParentToLocal( CGUIVector2& rPos );
		void LocalToParent( CGUIVector2& rPos );

		void LocalToWorld( const CGUIRect& rRect, CGUIRenderRect& rRenderRect );

		const CGUIVector2&	GetAnchorPoint();								//get anchor point, the value of it is from 0.0f to 1.0f
		void				SetAnchorPoint(const CGUIVector2&rAnchorPoint);		//set anchor point
		void				SetAnchorPoint(real x, real y);				//set anchor point


		void				SetMaximumSize(const CGUISize& rSize);		//set maximum size of this widget
		const CGUISize&		GetMaximumSize() const;						//get maximum size of this widget
		void				SetMinimumSize(const CGUISize& rSize);		//set minimum size of this widget
		const CGUISize&		GetMinimumSize() const;						//get minimum size of this widget

		void SetOpenWithParent( bool bFlag ); 
		bool IsOpenWithParent( ) const;

		void SetInheritAlpha( bool bFlag ); 
		bool IsInheritAlpha( ) const;

		void SetFocusAgency( bool bFlag ); 
		bool IsFocusAgency( ) const;

		void SetFocusable( bool bFlag ); 
		bool IsFocusable( ) const;
		bool IsDerivedFocusable( ) const;

		void SetMovable( bool bFlag ); 
		bool IsMovable( ) const;

		void SetExclusive( bool bFlag ); 
		bool IsExclusive( ) const;

		void SetHitable( bool bFlag ); 
		bool IsHitable( ) const;

		void SetMouseConsumed( bool bFlag ); 
		bool IsMouseConsumed( ) const;

		void SetResponseUpdateEvent( bool bFlag ); 
		bool IsResponseUpdateEvent( ) const;

		void SetGenerateClickEvent( bool bFlag ); 
		bool IsGenerateClickEvent( ) const;

		void SetGenerateDBClickEvent( bool bFlag ); 
		bool IsGenerateDBClickEvent( ) const;

		void SetGenerateMultiClickEvent( bool bFlag ); 
		bool IsGenerateMultiClickEvent( ) const;

	protected:
		/// for global function defined in c++
		void	CallGlobalFunction(const CGUIString& strEventName, CGUIEvent* pEvent);

		///for global function defined in script
		void	CallScriptFunction(const CGUIString& strEventName, CGUIEvent* pEvent);

		///call callback function
		void	CallbackFunction(const CGUIString& strEventName, CGUIEvent* pEvent);



	protected:

		virtual void PushClipRect( IGUIInterfaceRender* pRender  );
		virtual void PopClipRect( IGUIInterfaceRender* pRender );

		virtual void RefreshSelf( );

		/**
		* @brief render this widget only
		*/
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RenderExtraSelfInfo(IGUIInterfaceRender* pRender);

		/**
		* @brief update this widget only
		*/
		virtual void UpdateSelf( real fDeltaTime );

		/** 
		 * @brief update action sequence
		 */
		void UpdateAs( real fDeltaTime );

		void DrawRect( IGUIInterfaceRender* pRender,
			const CGUIRect& rDestRect, 
			real fLineWidth,
			const CGUIColor& rColor );

		void DrawCharacter(IGUIInterfaceRender* pRender, 
			wchar_t charCode, 
			const CGUIStringInfo& rInfo,
			const CGUIVector2& rPos);
		/**
		* @brief draw a string
		*/
		void	DrawString( IGUIInterfaceRender* pRender, 
			const CGUIStringEx& strText,
			const CGUIRect& rDrawRect,
			uint8 uTextAlignment,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		void	DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringEx& strText,
			const CGUIVector2& rPos,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		void	DrawImage( IGUIInterfaceRender* pRender,
			const CGUIImage* pImage, 
			const CGUIRect& rDestRect);

		void	DrawImage( IGUIInterfaceRender* pRender, 
			const CGUIString& rName, 
			const CGUIRect& rDestRect);

		void	DrawAnimation( IGUIInterfaceRender* pRender,
			const CGUIAnimation* pAnimation, 
			const CGUIRect& rDestRect);

		/**
		* @brief callback of set the image of widget.
		*/
		virtual void	OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	private:
		void SetParentImpl( CGUIWidget* pParent );

	protected://event
		//declare friend event
		EVENT_ITEM_LIST();

	protected:
		/// callback function: for change alpha
		virtual uint32		OnAlphaChange( CGUIEventAlpha* pEvent );
		/// callback function: for change size
		virtual uint32		OnSizeChanged( CGUIEventSize* pEvent );
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
		virtual uint32		OnParentChanged( CGUIEventRelativeChange* pEvent );

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
		CGUIWidgetPosition m_aWidgetPosition;
		CGUIWidgetSize m_aWidgetSize;
		CGUIQuaternion m_aQuaternion;
		CGUIVector2	m_aWidgetAnchorPoint;			//anchor point, the value is from 0.0f to 1.0f

		CGUIRect		m_aBoundArea;				//widget bound, local
		CGUIRect		m_aClientArea;				//client area, local
		CGUIRect		m_aClipArea;				//client area, local


		CGUISize		m_aMaxSize;					//maximum size of widget,it doesn't work if the value is (0,0)
		CGUISize		m_aMinSize;					//minimum size of widget,it doesn't work if the value is (0,0)

		CGUIVector3		m_vRotation;

		CGUIRenderRect		m_aBound;				//!< bound of this rect.

		///////////////////////////////////////////////////////////////////////
		/// widget callback function								
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString,CallbackEventFunc> TMapGlobalFunc;
		TMapGlobalFunc m_mapGlobalFunc;		//!<event's global callback function

		typedef std::map<CGUIString,CGUIString> TMapScriptFunc;
		TMapScriptFunc m_mapScriptFunc;		//!<event's script callback function

		///////////////////////////////////////////////////////////////////////
		/// flag
		///////////////////////////////////////////////////////////////////////
		bool m_bOpenWithParent;
		bool m_bInheritAlpha;
		bool m_bIsFocusAgency;
		bool m_bIsFocusable;
		bool m_bIsMovable;
		bool m_bIsExclusive;
		bool m_bIsHitable;
		bool m_bIsMouseConsumed;
		bool m_bIsResponseUpdateEvent; 
		bool m_bIsGenerateClickEvent; 
		bool m_bIsGenerateDBClickEvent; 
		bool m_bIsGenerateMultiClickEvent; 
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
			real				m_fTimeWaiting;		// by second
			real				m_fTimeLeft;
			STimer( real fTimeWaiting )
				:m_fTimeWaiting( fTimeWaiting )
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
		CGUIString		m_strName;			//!< widget name, should be a unique name in same scene
		CGUIString		m_strOwnerSceneName;		//!< scene name, should be a unique name
		CGUIString		m_strWorkingSceneName;	//!< working scene name, should be a unique name

		void*			m_pUserData;		//!< user data, used by user.

		///////////////////////////////////////////////////////////////////////
		/// for widget property support
		///////////////////////////////////////////////////////////////////////
		CGUIProperty		m_aPropertySet;
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

