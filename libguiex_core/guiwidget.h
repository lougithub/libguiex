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
#include "sigslot.h"

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
	class CGUISoundData;
	class CGUISceneEffect;

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
		CGUIWidget(const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName  );
		virtual ~CGUIWidget();

		void Create();
		bool IsCreate() const;

		void NotifyLoaded();

		virtual void Open();
		bool IsOpen() const;
		virtual void Close();

		// tree function
		void SetParent( CGUIWidget *pParent);
		CGUIWidget* GetParent()  const;
		CGUIWidget* GetChild( ) const;
		CGUIWidget* GetChild( const CGUIString& rChildName ) const;
		CGUIWidget* GetNextSibling( ) const;
		//CGUIWidget* GetExclusiveChild( ) const;

		void MoveToTop();
		void MoveDown();
		void MoveUp();

		const CGUIString& GetName() const;
		const CGUIString& GetSceneName( ) const;

		void SetWorkingSceneName(const CGUIString& rWorkingProjName);
		const CGUIString& GetWorkingSceneName( ) const;

		const CGUIString& GetType() const;

		void SetUserData(void* pData);
		void* GetUserData() const;

		////////////////////////////////////////////////////////////////////////////
		// signal
		////////////////////////////////////////////////////////////////////////////
		sigslot::signal1<CGUIWidget*>& GetOnWidgetDestroyedSignal();

		////////////////////////////////////////////////////////////////////////////
		// text about function
		////////////////////////////////////////////////////////////////////////////
		const CGUIStringEx& GetTooltipText(void) const;
		void SetTooltipText(const CGUIStringEx& rText);

		virtual void SetTextColor(const CGUIColor& rColor);
		virtual void SetTextContent(const CGUIStringW& rStringW);
		virtual const CGUIStringW& GetTextContent() const;
		virtual bool IsTextContentEmpty( ) const;
		void SetTextContentUTF8( const CGUIString& rString);
		CGUIString GetTextContentUTF8() const;

		virtual void SetTextAlignment( uint8 uAlignment );
		virtual uint8 GetTextAlignment( ) const;

		virtual void SetTextInfo(const CGUIStringInfo& rInfo);
		virtual const CGUIStringInfo& GetTextInfo( ) const;

		virtual const CGUIStringEx& GetText() const;

		////////////////////////////////////////////////////////////////////////////
		// flag
		////////////////////////////////////////////////////////////////////////////
		void SetClipChildren( bool bClip );
		bool IsClipChildren( ) const;

		void SetActivable(bool bActivable);
		bool IsActivable() const;
		bool IsDerivedActivable();

		void SetFocus(bool bFocus);
		bool IsFocus() const;

		void SetVisible(bool bVisible);
		bool IsVisible( ) const;
		bool IsDerivedVisible();

		void SetDisable(bool bDisable);
		bool IsDisable( ) const;
		bool IsDerivedDisable();

		void SetAlpha( real fAlpha );
		real GetAlpha()  const;
		real GetDerivedAlpha();
 
		void SetScale( const CGUISize& );
		const CGUISize& GetScale() const;
		const CGUISize& GetDerivedScale();

		void SetColor(const CGUIColor& rColor );
		const CGUIColor& GetColor() const;

		CGUIWidget* GetWidgetAtPoint(const CGUIVector2& rPos);
		virtual bool HitTest( const CGUIVector2& rPos);

		virtual void Render( IGUIInterfaceRender* pRender );
		virtual void RenderExtraInfo(IGUIInterfaceRender* pRender);
		virtual void Update( real fDeltaTime );
		virtual void Refresh( );

		////////////////////////////////////////////////////////////////////////////
		// resource related operation
		////////////////////////////////////////////////////////////////////////////
		CGUIImage* SetImage( const CGUIString& rName, const CGUIString& rImageName );
		void SetImage( const CGUIString& rName, CGUIImage* pImage );
		bool HasImage( const CGUIString& rName );
		CGUIImage* GetImage( const CGUIString& rName );

		void SetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation );
		CGUIAnimation* SetAnimation( const CGUIString& rName, const CGUIString& rAnimationName );
		CGUIAnimation* GetAnimation( const CGUIString& rAnimationName);

		CGUIAs* SetAs( const CGUIString& rName, const CGUIString& rAsName );
		void SetAs( const CGUIString& rName, CGUIAs* pAs );
		bool HasAs( const CGUIString& rName );
		CGUIAs* GetAs( const CGUIString& rName );

		void SetAutoPlayAs( bool bEnable );
		bool IsAutoPlayAs( ) const;
		void PlayAs( const CGUIString& rName );
		void PlayAs( CGUIAs* pAs );
		bool IsAsPlaying( const CGUIString& rName );
		bool IsAsPlaying( CGUIAs* pAs );
		void StopAs( const CGUIString& rName );
		void StopAs( CGUIAs* pAs );
		void StopAllAs( );

		void SetSceneEffect( CGUISceneEffect* pSceneEffect );
		CGUISceneEffect* GetSceneEffect() const;

		void DestroyAllResource( );

		////////////////////////////////////////////////////////////////////////////
		// property related operation
		////////////////////////////////////////////////////////////////////////////
		void SetProperty( const CGUIProperty& rProperty);
		const CGUIProperty& GetProperty() const;
		void ClearProperty();
		void InsertProperty( const CGUIProperty& rProperty);
		void LoadFromProperty();
		void DumpToProperty();
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

	public:
		CGUISoundData* RegisterSound( const CGUIString& strEventName, const CGUIString& rSoundName );
		void UnregisterSound( const CGUIString& strEventName );
		CGUISoundData* GetSound( const CGUIString& strEventName );

		template<class TEVENT> void RegisterNativeCallbackFunc( const CGUIString& strEventName, void (*pFunc)(TEVENT*) );
		void RegisterScriptCallbackFunc( const CGUIString& strEventName, const CGUIString& strFunc );
		void UnregisterScriptCallbackFunc(const CGUIString& strEventName);
		bool HasScriptCallbackFunc(const CGUIString& strEventName) const;
		const CGUIString& GetScriptCallbackFunc(const CGUIString& strEventName) const;
		
		void RegisterNativeTimerFunc( real rWaitingTime, const CGUIString& strEventName, void (*pFunc)(CGUIEventTimer*) );
		void RegisterScriptTimerFunc( real rWaitingTime, const CGUIString& strEventName, const CGUIString& strFunc );
		void UnregisterNativeTimerFunc(const CGUIString& strEventName);
		void UnregisterScriptTimerFunc(const CGUIString& strEventName);

	public:
		////////////////////////////////////////////////////////////////////////////
		// widget operation - position
		////////////////////////////////////////////////////////////////////////////
		const CGUIRect& GetBoundArea() const;
		const CGUIRect&	GetClientArea() const;
		const CGUIRect&	GetClipArea() const;

		void SetPosition( real x, real y );
		void SetPosition( const CGUIVector2&rPos );
		const CGUIVector2&	GetPosition() const; 
		void SetPixelPosition( const CGUIVector2& rPixelPos );
		void SetPixelPosition( real x, real y );
		const CGUIVector2& GetPixelPosition() const;

		void SetGlobalPixelPosition( const CGUIVector2&rPos );
		void SetGlobalPixelPosition( real x, real y );
		CGUIVector2	GetGlobalPixelPosition() const;

		void SetPositionType( EScreenValue rValueType );
		EScreenValue GetPositionType( ) const; 

		void SetSize( real width, real height );
		void SetSize( const CGUISize& rSize );
		const CGUISize&	GetSize() const;
		void SetPixelSize( real width, real height );
		void SetPixelSize( const CGUISize& rPixelSize );
		const CGUISize&	GetPixelSize() const;

		void SetSizeType( EScreenValue eValueType );
		EScreenValue GetSizeType( ) const;

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

		const CGUIVector2& GetAnchorPoint();
		void SetAnchorPoint(const CGUIVector2&rAnchorPoint);
		void SetAnchorPoint(real x, real y);

		void SetMaximumSize(const CGUISize& rSize);	
		const CGUISize&	 GetMaximumSize() const;	
		void SetMinimumSize(const CGUISize& rSize);	
		const CGUISize&	GetMinimumSize() const;		

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

		void SetHitable( bool bFlag ); 
		bool IsHitable( ) const;

		void SetMouseConsumed( bool bFlag ); 
		bool IsMouseConsumed( ) const;

		void SetGenerateParentSizeChangeEvent( bool bFlag ); 
		bool IsGenerateParentSizeChangeEvent( ) const;

		void SetGenerateParentChangeEvent( bool bFlag ); 
		bool IsGenerateParentChangeEvent( ) const;

		void SetGenerateUpdateEvent( bool bFlag ); 
		bool IsGenerateUpdateEvent( ) const;

		void SetGenerateScaleChangeEvent( bool bFlag ); 
		bool IsGenerateScaleChangeEvent( ) const;

		void SetGenerateClickEvent( bool bFlag ); 
		bool IsGenerateClickEvent( ) const;

		void SetGenerateDBClickEvent( bool bFlag ); 
		bool IsGenerateDBClickEvent( ) const;

		void SetGenerateMultiClickEvent( bool bFlag ); 
		bool IsGenerateMultiClickEvent( ) const;

	protected:
		void CallGlobalFunction( const CGUIString& strEventName, CGUIEvent* pEvent );
		void CallScriptFunction( const CGUIString& strEventName, CGUIEvent* pEvent );
		void CallbackFunction( const CGUIString& strEventName, CGUIEvent* pEvent );
		void PlaySound( const CGUIString& strEventName, CGUIEvent* pEvent );

	protected:
		virtual void PushClipRect( IGUIInterfaceRender* pRender  );
		virtual void PopClipRect( IGUIInterfaceRender* pRender );

		virtual void UpdateSelf( real fDeltaTime );
		virtual void RefreshSelf( );
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RenderExtraSelfInfo(IGUIInterfaceRender* pRender);

		void UpdateAsSelf( real fDeltaTime );

		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage ); 
		virtual void OnCreate();
		virtual void OnUpdate( real fDeltaTime );
		virtual void OnDestroy();

		void DrawRect( IGUIInterfaceRender* pRender, const CGUIRect& rDestRect, real fLineWidth, const CGUIColor& rColor );
		void DrawCharacter(IGUIInterfaceRender* pRender, wchar_t charCode, const CGUIStringInfo& rInfo, const CGUIVector2& rPos );
		void DrawString( IGUIInterfaceRender* pRender, const CGUIStringEx& strText, const CGUIRect& rDrawRect, uint8 uTextAlignment, int32 nStartPos = 0, int32 nEndPos = -1 );
		void DrawString( IGUIInterfaceRender* pRender, const CGUIStringEx& strText, const CGUIVector2& rPos, int32 nStartPos = 0, int32 nEndPos = -1);
		void DrawImage( IGUIInterfaceRender* pRender, CGUIImage* pImage, const CGUIRect& rDestRect );
		void DrawImage( IGUIInterfaceRender* pRender, const CGUIString& rName, const CGUIRect& rDestRect );
		void DrawAnimation( IGUIInterfaceRender* pRender, CGUIAnimation* pAnimation, const CGUIRect& rDestRect );

		friend class CGUIWidgetFactory;
		void SetGenerator( const CGUIWidgetGenerator* pGenerator);
		const CGUIWidgetGenerator* GetGenerator() const;

		void RemoveChild( CGUIWidget* pWidget );
		void AddChild( CGUIWidget* pWidget );
		CGUIWidget*	GetLastChild( );

		CGUISize GetParentSize() const;

		virtual CGUINode* NodeGetParent();


	private:
		void SetParentImpl( CGUIWidget* pParent );
		void SetChildImpl( CGUIWidget* pChild );
		void SetNextSiblingImpl( CGUIWidget* pNextSibling );

	protected://event
		//declare friend event
		EVENT_ITEM_LIST();

	protected:
		/// callback function: for change alpha
		virtual uint32 OnAlphaChange( CGUIEventAlpha* pEvent );

		/// callback function: for change size
		virtual uint32 OnSizeChanged( CGUIEventSize* pEvent );
		virtual uint32 OnParentSizeChange( CGUIEventSize* pEvent );

		/// callback function: for mouse event
		virtual uint32 OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMove( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftDbClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftMultiClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseRightDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseRightUp( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseRightClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseRightDbClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseRightMultiClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMiddleDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMiddleUp( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMiddleClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMiddleDbClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMiddleMultiClick( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseWheel( CGUIEventMouse* pEvent );

		/// callback function: for drag
		virtual uint32 OnDragBegin( CGUIEventDrag* pEvent );
		virtual uint32 OnDragProcess( CGUIEventDrag* pEvent );
		virtual uint32 OnDragEnd( CGUIEventDrag* pEvent );

		/// callback function: for scale change
		virtual uint32 OnScaleChange( CGUIEventNotification* pEvent );

		/// callback function: for active
		virtual uint32 OnActive( CGUIEventNotification* pEvent );
		virtual uint32 OnInactive( CGUIEventNotification* pEvent );

		/// callback function: for focus
		virtual uint32 OnGetFocus( CGUIEventNotification* pEvent );
		virtual uint32 OnLostFocus( CGUIEventNotification* pEvent );

		/// callback function: for visable
		virtual uint32 OnVisible( CGUIEventNotification* pEvent );
		virtual uint32 OnInvisible( CGUIEventNotification* pEvent );

		/// callback function: for enable
		virtual uint32 OnEnable( CGUIEventNotification* pEvent );
		virtual uint32 OnDisable( CGUIEventNotification* pEvent );

		/// callback function: for open and close
		virtual uint32 OnLoad( CGUIEventNotification* pEvent );
		virtual uint32 OnOpen( CGUIEventNotification* pEvent );
		virtual uint32 OnClose( CGUIEventNotification* pEvent );

		/// callback function: for ui event
		virtual uint32 OnUIEvent( CGUIEventUI* pEvent );

		/// callback function: for check and uncheck event
		virtual uint32 OnChecked( CGUIEventNotification* pEvent );
		virtual uint32 OnUnchecked( CGUIEventNotification* pEvent );

		/// callback function: for update event
		virtual int32 OnEventUpdate(CGUIEventNotification* pEvent );

		/// callback function: for scrollbar's attach and detach
		virtual uint32 OnScrollbarScroll( CGUIEventScrollbar* pEvent );

		/// callback function: for keyboard
		virtual uint32 OnKeyDown( CGUIEventKeyboard* pEvent );
		virtual uint32 OnKeyUp( CGUIEventKeyboard* pEvent );
		virtual uint32 OnKeyPressed( CGUIEventKeyboard* pEvent );
		virtual uint32 OnKeyClicked( CGUIEventKeyboard* pEvent );

		/// callback function: for timer
		virtual uint32 OnTimer( CGUIEventTimer* pEvent );

		/// callback function: for listbox
		virtual uint32 OnListBoxSelected( CGUIEventListBox* pEvent );
		virtual uint32 OnListBoxSelectChanged( CGUIEventListBox* pEvent );
		virtual uint32 OnListBoxLeftDbClick( CGUIEventListBox* pEvent );
		virtual uint32 OnListBoxRightDbClick( CGUIEventListBox* pEvent );
		virtual uint32 OnListBoxLeftClick( CGUIEventListBox* pEvent );
		virtual uint32 OnListBoxRightClick( CGUIEventListBox* pEvent );

		/// callback function: for combobox
		virtual uint32 OnComboBoxSelected(CGUIEventComboBox* pEvent);

		/// callback function: for relative changes
		virtual uint32 OnAddChild( CGUIEventRelativeChange* pEvent );
		virtual uint32 OnRemoveChild( CGUIEventRelativeChange* pEvent );
		virtual uint32 OnParentChanged( CGUIEventRelativeChange* pEvent );

	protected:
		const CGUIWidgetGenerator* m_pWidgetGenerator;	//!< generator which used to create widget

		///////////////////////////////////////////////////////////////////////
		/// parameter							
		///////////////////////////////////////////////////////////////////////
		CGUIWidgetParamMultiply<CGUISize> m_aParamScale;
		CGUIWidgetParamMultiply<real> m_aParamAlpha;
		CGUIWidgetParamOR<bool> m_aParamDisable;
		CGUIWidgetParamAND<bool> m_aParamVisible;
		CGUIWidgetParamOR<bool> m_aParamActivable;
		CGUIColor m_aColor;

		bool m_bIsOpen;	///widget state
		bool m_bIsCreate; ///widget state

		///////////////////////////////////////////////////////////////////////
		/// widget size and position								
		///////////////////////////////////////////////////////////////////////
		CGUIWidgetPosition m_aWidgetPosition;
		CGUIWidgetSize m_aWidgetSize;
		CGUIQuaternion m_aQuaternion;
		CGUIVector2	m_aWidgetAnchorPoint; //anchor point, the value is from 0.0f to 1.0f

		CGUIRect m_aBoundArea; //widget bound, local
		CGUIRect m_aClientArea; //client area, local
		CGUIRect m_aClipArea; //client area, local

		CGUISize m_aMaxSize; //maximum size of widget,it doesn't work if the value is (0,0)
		CGUISize m_aMinSize; //minimum size of widget,it doesn't work if the value is (0,0)

		CGUIVector3 m_vRotation;

		CGUIRenderRect m_aBound; //!< bound of this rect.

		///////////////////////////////////////////////////////////////////////
		/// widget callback function								
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString,CallbackEventFunc> TMapGlobalFunc;
		TMapGlobalFunc m_mapNativeFunc; //!<event's global callback function

		typedef std::map<CGUIString,CGUIString> TMapScriptFunc;
		TMapScriptFunc m_mapScriptFunc; //!<event's script callback function

		///////////////////////////////////////////////////////////////////////
		/// flag
		///////////////////////////////////////////////////////////////////////
		bool m_bOpenWithParent;
		bool m_bIsAutoPlayAs;
		bool m_bIsClipChildren;
		bool m_bInheritAlpha;
		bool m_bIsFocusAgency;
		bool m_bIsFocusable;
		bool m_bIsMovable;
		bool m_bIsExclusive;
		bool m_bIsHitable;
		bool m_bIsMouseConsumed;

		bool m_bIsGenerateClickEvent; 
		bool m_bIsGenerateDBClickEvent; 
		bool m_bIsGenerateMultiClickEvent; 
		bool m_bIsGenerateParentChangeEvent; 
		bool m_bIsGenerateParentSizeChangeEvent; 
		bool m_bIsGenerateUpdateEvent; 
		bool m_bIsGenerateScaleChangeEvent; 

		///////////////////////////////////////////////////////////////////////
		/// sound
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString,CGUISoundData*> TMapSound;
		TMapSound m_mapEventSound; //!<sound for event

		///////////////////////////////////////////////////////////////////////
		/// Timer							
		///////////////////////////////////////////////////////////////////////
		struct STimer
		{
			real m_fTimeWaiting; // by second
			real m_fTimeLeft;
			STimer( real fTimeWaiting )
				:m_fTimeWaiting( fTimeWaiting )
			{
			}
		};
		typedef std::map<CGUIString, STimer> TMapTimer;
		TMapTimer m_aMapTimer;	/// key is event name

		///////////////////////////////////////////////////////////////////////
		/// for image and animation											
		///////////////////////////////////////////////////////////////////////
		typedef std::map<CGUIString, CGUIImage*> TMapImage;
		TMapImage m_aMapImage;	//!< image info map, key is image name used in ui system

		typedef std::map<CGUIString, CGUIAnimation*> TMapAnimation;
		TMapAnimation m_aMapAnimation; //!< animation map, key is animation name used in ui system

		typedef std::map<CGUIString, CGUIAs*> TMapAs;
		TMapAs m_aMapAs; //!< As map, key is as name used in ui system
		
		typedef std::vector<CGUIAs*> TListAs;
		TListAs	m_listAsPlaying;

		///////////////////////////////////////////////////////////////////////
		/// for text										
		///////////////////////////////////////////////////////////////////////
		CGUIStringEx m_strTooltipText; //!< tooltip for list box item.  
		CGUIStringEx m_strText; //!< text
		uint8 m_uTextAlignment;

		///////////////////////////////////////////////////////////////////////
		/// for widget tree											
		///////////////////////////////////////////////////////////////////////
		//CGUIWidget*	 m_pExclusiveChild;	//!< for exclusive child

		CGUIString m_strType; //!< widget type
		CGUIString m_strName; //!< widget name, should be a unique name in same scene
		CGUIString m_strOwnerSceneName; //!< scene name, should be a unique name
		CGUIString m_strWorkingSceneName; //!< working scene name, should be a unique name

		CGUISceneEffect* m_pSceneEffect;

		void* m_pUserData; //!< user data, used by user.

		///////////////////////////////////////////////////////////////////////
		/// for widget property support
		///////////////////////////////////////////////////////////////////////
		CGUIProperty m_aPropertySet;

		///////////////////////////////////////////////////////////////////////
		/// sigslot
		///////////////////////////////////////////////////////////////////////
		sigslot::signal1<CGUIWidget*> OnWidgetDestroyed; ///< - signal to emit when widget is destroyed

		CGUIWidget* m_pParent; //!< parent
		CGUIWidget* m_pChild; //!< child
		CGUIWidget* m_pNextSibling; //!< sibling
	};


	//inline function
	//------------------------------------------------------------------------------
	template<class TEVENT> 
	inline void	CGUIWidget::RegisterNativeCallbackFunc( const CGUIString& strEventName, void (*pFunc)(TEVENT*) )
	{
		m_mapNativeFunc.insert( std::make_pair(strEventName, reinterpret_cast<CallbackEventFunc>(pFunc)));
	}
	//------------------------------------------------------------------------------

}//namespace guiex

#endif //__GUI_WIDGET_20060607_H__

