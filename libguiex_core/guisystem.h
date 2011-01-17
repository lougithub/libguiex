/** 
* @file guisystem.h
* @brief system
* @author ken
* @date 2006-06-21
*/


#ifndef __GUI_SYSTEM_20060621_H__
#define __GUI_SYSTEM_20060621_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guirect.h"
#include "guievent.h"
#include "guiinputprocessor.h"
#include "guiinterfacemouse.h"
#include "sigslot.h"
#include <map>
#include <set>
#include <list>
#include <vector>



//============================================================================//
// declare
//============================================================================// 
namespace guiex
{	
	class CGUIWidget;
	class CGUIEventKeyboard;

	//for singleton
	class CGUIImageManager;
	class CGUIAnimationManager;
	class CGUISoundManager;
	class CGUIMusicManager;
	class CGUIAsManager;
	class CGUIFontManager;
	class CGUITextureManager;
	class CGUIInterfaceManager;
	class CGUIMouseCursor;
	class CGUIPropertyManager;
	class CGUIWidgetFactory;
	class CGUIAsFactory;
	class CGUISceneManager;
	class CGUILogMsgManager;
	class CGUIWidgetManager;
	class CGUICameraManager;
	class CGUICanvasLayerManager;
	class CGUIParticle2DManager;

	extern CGUISystem* GSystem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUISystem
	* @brief widget system
	*/
	class GUIEXPORT CGUISystem : public sigslot::has_slots<>
	{
	public:
		CGUISystem();
		~CGUISystem();

		static CGUISystem* Instance();

		int Initialize();
		bool IsInitialized() const;
		void Release();

		void Update( real fDeltaTime );
		void BeginRender();
		void Render();
		void EndRender();

		void SetDrawExtraInfo( bool bDraw );
		bool IsDrawExtraInfo() const;

		void SetPlayingAs( bool bPlaying );
		bool IsPlayingAs() const;

		void SetDataPath(const CGUIString& rDataPath);
		const CGUIString& GetDataPath() const;

		bool IsEditorMode( ) const;
		void SetEditorMode( bool bEditorMode );

		const char* GenerateAnonymousName();

		void SetFocusWidget(CGUIWidget* pWidget);
		CGUIWidget*	GetFocusWidget();

		CGUIWidget* GetWidgetUnderPoint(const CGUIVector2& rPos);


		//********************************************************
		//	page and dialog
		//********************************************************
		void OpenUIPage(CGUIWidget* pPage);
		void CloseUIPage(CGUIWidget* pPage);
		uint32 GetOpenedPageNum() const;
		CGUIWidget* GetOpenedPageByIndex( uint32 nIdx );

		void OpenDialog(CGUIWidget* pDlg);
		void CloseDialog(CGUIWidget* pDlg);
		CGUIWidget* GetTopestDialog( ) const;

		void OpenPopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentPopupWidget( ) const;
		void ClosePopupWidget(CGUIWidget* pWidget);

		CGUIWidget* GetCurrentRootWidget( ) const;

		void CloseByAutoSelect( CGUIWidget* pWidget );
		void CloseAll();

		//********************************************************
		//	screen size
		//********************************************************
		void SetScreenSize( const CGUISize& rScreenSize );
		void SetScreenSize( uint32 width, uint32 height);
		const CGUISize& GetScreenSize( ) const;
		uint32 GetScreenWidth( ) const;
		uint32 GetScreenHeight( ) const;


		//********************************************************
		//	ui event
		//********************************************************
		void RegisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget);
		void UnregisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget);
		void UnregisterUIEvent( CGUIWidget* pWidget);
		void UnregisterAllUIEvent( );

		void SendUIEvent(CGUIEventUI* pEvent );
		void SendEvent(CGUIEvent* pEvent );

		//********************************************************
		//	global key event
		//********************************************************
		void RegisterGlobalKeyReceiver( CGUIWidget* pReceiver, CGUIWidget* pRoot);
		void UngisterGlobalKeyByRoot( CGUIWidget* pRoot);	
		void UngisterAllGlobalKey( );

		void RegisterResourceManager( class CGUIResourceManagerBase* pMgr );
		void UnregisterResourceManager( class CGUIResourceManagerBase* pMgr );
		//void FreeResource( const CGUIString& rSceneName );
		void UnloadAllResource();
		void ReleaseResourceByScene( const CGUIString& rSceneName );
		void ReleaseAllResources(  );

		void DestroyAllWidgets(  );

		bool ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent );

		real GetSystemTime() const;
		int32 GetFPS() const;

		void OnWidgetDestroyed(CGUIWidget* pWidget);

		//********************************************************
		//	singleton
		//********************************************************
		CGUIImageManager* GetImageManager();
		CGUIAnimationManager* GetAnimationManager();
		CGUISoundManager* GetSoundManager();
		CGUIMusicManager* GetMusicManager();
		CGUIAsManager* GetAsManager();
		CGUIFontManager* GetFontManager();
		CGUIParticle2DManager* GetParticle2DManager();
		CGUITextureManager* GetTextureManager();
		CGUIInterfaceManager* GetInterfaceManager();
		CGUIMouseCursor* GetMouseCursor();
		CGUIPropertyManager* GetPropertyManager();
		CGUIWidgetFactory* GetWidgetFactory();
		CGUISceneManager* GetSceneInfoManager();
		CGUIAsFactory* GetAsFactory();
		CGUILogMsgManager* GetLogMsgManager();
		CGUIWidgetManager* GetWidgetManager();
		CGUICameraManager* GetCameraManager();
		CGUICanvasLayerManager* GetCanvasLayerManager();

	protected:
		friend class CGUIWidget;
		friend class CGUIInputProcessor;

		/// update time and frame
		void UpdateTime(real fDeltaTime);
		void UpdateSound( real fDeltaTime );
		void UpdatePhysics(real fDeltaTime);

		void AddToGarbage( CGUIWidget* pWidget );
		void AddToDynamicGarbage( CGUIWidget* pWidget );
		void RefreshGarbage( );
	
		void GenerateRootWidget();
		void DestroyRootWidget();

		void InitializeSingletons();
		void ReleaseSingletons();

		void RenderUI( class IGUIInterfaceRender* pRender );
		void RenderCanvas( class IGUIInterfaceRender* pRender );
		void UpdateUI( real fDeltaTime );
		void UpdateCanvas( real fDeltaTime );
		void UpdatePerformance( real fDeltaTime  );
		
	protected:
		void ProcessCommand();
		bool ProcessGlobalKeyEvent(CGUIEventKeyboard* pEvent);

	protected:
		void Reset();

	private:
		//----------------------------------------------------------------------
		/// the root widget of widget tree
		CGUIWidget* m_pWgtRoot; ///widget root
		CGUIWidget* m_pWgtFocus; ///widget which has focus
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		/// screen size
		CGUISize m_aScreenSize; //!< size of screen, in default
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//time related
		real m_fSystemTime; ///system time
		real m_fTimerForFrame; ///time used to calculate frame
		int32 m_aFrame; ///frame of system
		int32 m_nFps; ///current fps
		//----------------------------------------------------------------------

		CGUIInputProcessor m_aInputProcessor; ///input processor

		//----------------------------------------------------------------------
		//page list
		typedef std::vector<CGUIWidget*>	TArrayWidget;	
		TArrayWidget m_arrayOpenedPage;
		TArrayWidget m_vecPageGarbage;
		TArrayWidget m_vecDynamicPageGarbage;

		//widget modal dialog
		TArrayWidget m_arrayOpenedDlg; ///contain modal dialog
		
		//popup widget
		CGUIWidget*	m_pPopupWidget; ///contain popup widget shown on top
		//----------------------------------------------------------------------


		//----------------------------------------------------------------------
		//global ui event
		typedef std::vector<CGUIWidget* > TEventReceiver;
		typedef std::map<CGUIString, TEventReceiver> TMapUIEvent;
		TMapUIEvent m_mapUIEvent;
		//global ui event
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//global key event
		/*receiver*/	/*root of this receiver*/
		typedef std::vector<std::pair<CGUIWidget*, CGUIWidget*> > TGlobalKeyObj;
		TGlobalKeyObj m_vecGlobalKeyObj;
		//global key event
		//----------------------------------------------------------------------

		CGUIString m_strDataPath; //data path

		//----------------------------------------------------------------------
		//index for name generating
		uint32 m_nNameGenerateIdx;
		char m_szAnonymousName[32];
		//index for name generating
		//----------------------------------------------------------------------

		bool m_bIsEditorMode;
		bool m_bPlayingAs;
		bool m_bDrawExtraInfo;
		bool m_bShouldRunScript; //!< should system run script

		typedef std::list<CGUIResourceManagerBase*>	TListResourceMgr;
		TListResourceMgr m_listResourceManager;

		//single classes
		CGUIImageManager *m_pImageManager;
		CGUIAnimationManager *m_pAnimationManager;
		CGUISoundManager *m_pSoundManager;
		CGUIMusicManager *m_pMusicManager;
		CGUIAsManager *m_pAsManager;
		CGUIFontManager *m_pFontManager;
		CGUIParticle2DManager* m_pParticle2DManager;
		CGUITextureManager *m_pTextureManager;
		CGUIInterfaceManager* m_pInterfaceManager;
		CGUIMouseCursor* m_pMouseCursor;
		CGUIPropertyManager* m_pPropertyManager;
		CGUIWidgetFactory* m_pWidgetFactory;
		CGUISceneManager* m_pSceneInfoManager;
		CGUIAsFactory* m_pAsFactory;
		CGUILogMsgManager* m_pLogMsgManager;
		CGUIWidgetManager* m_pWidgetManager;
		CGUICameraManager* m_pCameraManager;
		CGUICanvasLayerManager* m_pCanvasLayerManager;
		
	private:
		bool m_bInitialized;

	private:
		static CGUISystem* m_pSingleton;
	};

	
	
	inline CGUIImageManager* CGUISystem::GetImageManager()
	{
		return m_pImageManager;
	}

	inline CGUIAnimationManager* CGUISystem::GetAnimationManager()
	{
		return m_pAnimationManager;
	}

	inline CGUISoundManager* CGUISystem::GetSoundManager()
	{
		return m_pSoundManager;
	}

	inline CGUIMusicManager* CGUISystem::GetMusicManager()
	{
		return m_pMusicManager;
	}

	inline CGUIAsManager* CGUISystem::GetAsManager()
	{
		return m_pAsManager;
	}

	inline CGUIFontManager* CGUISystem::GetFontManager()
	{
		return m_pFontManager;
	}

	inline CGUIParticle2DManager* CGUISystem::GetParticle2DManager()
	{
		return m_pParticle2DManager;
	}

	inline CGUITextureManager* CGUISystem::GetTextureManager()
	{
		return m_pTextureManager;
	}

	inline CGUIInterfaceManager* CGUISystem::GetInterfaceManager()
	{
		return m_pInterfaceManager;
	}

	inline CGUIMouseCursor* CGUISystem::GetMouseCursor()
	{
		return m_pMouseCursor;
	}

	inline CGUIPropertyManager* CGUISystem::GetPropertyManager()
	{
		return m_pPropertyManager;
	}

	inline CGUIWidgetFactory* CGUISystem::GetWidgetFactory()
	{
		return m_pWidgetFactory;
	}

	inline CGUISceneManager* CGUISystem::GetSceneInfoManager()
	{
		return m_pSceneInfoManager;
	}

	inline CGUIAsFactory* CGUISystem::GetAsFactory()
	{
		return m_pAsFactory;
	}

	inline CGUILogMsgManager* CGUISystem::GetLogMsgManager()
	{
		return m_pLogMsgManager;
	}

	inline CGUIWidgetManager* CGUISystem::GetWidgetManager()
	{
		return m_pWidgetManager;
	}

	inline CGUICameraManager* CGUISystem::GetCameraManager()
	{
		return m_pCameraManager;
	}

	inline CGUICanvasLayerManager* CGUISystem::GetCanvasLayerManager()
	{
		return m_pCanvasLayerManager;
	}
	
}//namespace guiex



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief get widget system
	*/
	GUIEXPORT CGUISystem* GetSystem();

	GUIEXPORT void OpenDialog(CGUIWidget* pDlg);
	GUIEXPORT void CloseDialog(CGUIWidget* pDlg);

	GUIEXPORT void OpenUIPage( CGUIWidget* pPage );
	GUIEXPORT void CloseUIPage( CGUIWidget* pPage );

	GUIEXPORT void SendUIEvent(const CGUIString& rUIEventName,
		const CGUIString& rArg1 = CGUIString(),
		const CGUIString& rArg2 = CGUIString(),
		const CGUIString& rArg3 = CGUIString(),
		const CGUIString& rArg4 = CGUIString(),
		const CGUIString& rArg5 = CGUIString(),
		const CGUIString& rArg6 = CGUIString(),
		const CGUIString& rArg7 = CGUIString(),
		const CGUIString& rArg8 = CGUIString(),
		const CGUIString& rArg9 = CGUIString());

}//namespace guiex

#endif //__GUI_SYSTEM_20060621_H__
