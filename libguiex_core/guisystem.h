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
#include "guiintsize.h"
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
	class CGUITiledMapManager;

	class CGUIUICanvasLayer;

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
		void GenerateUICanvas();
		void DestroyUICanvas();
		CGUIUICanvasLayer* GetUICanvas();


		//********************************************************
		//	screen size
		//********************************************************
		void SetScreenSize( const CGUIIntSize& rScreenSize );
		void SetScreenSize( uint32 width, uint32 height);
		const CGUIIntSize& GetScreenSize( ) const;
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
		void RegisterGlobalKeyReceiver( CGUIWidget* pReceiver );
		void UngisterGlobalKeyReceiver( CGUIWidget* pReceiver);	
		void UngisterAllGlobalKey( );

		void RegisterResourceManager( class CGUIResourceManagerBase* pMgr );
		void UnregisterResourceManager( class CGUIResourceManagerBase* pMgr );
		//void FreeResource( const CGUIString& rSceneName );
		void UnloadAllResource();
		void ReleaseResourceByScene( const CGUIString& rSceneName );
		void ReleaseAllResources( );

		void DestroyAllCanvas( );
		void DestroyAllWidgets( );

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
		CGUITiledMapManager* GetTiledMapManager();
		CGUITextureManager* GetTextureManager();
		CGUIInterfaceManager* GetInterfaceManager();
		CGUIMouseCursor* GetMouseCursor();
		CGUIPropertyManager* GetPropertyManager();
		CGUIWidgetFactory* GetWidgetFactory();
		CGUISceneManager* GetSceneManager();
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

		void InitializeSingletons();
		void ReleaseSingletons();

		void RenderCanvas( class IGUIInterfaceRender* pRender );
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
		CGUIWidget* m_pWgtFocus; ///widget which has focus
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		/// screen size
		CGUIIntSize m_aScreenSize; //!< size of screen, in default
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
		//global ui event
		typedef std::vector<CGUIWidget* > TEventReceiver;
		typedef std::map<CGUIString, TEventReceiver> TMapUIEvent;
		TMapUIEvent m_mapUIEvent;
		//global ui event
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//global key event
		typedef std::vector<CGUIWidget*> TGlobalKeyObj;
		TGlobalKeyObj m_vecGlobalKeyObj;
		//global key event
		//----------------------------------------------------------------------

		CGUIString m_strDataPath; //data path

		friend class CGUIUICanvasLayer;
		CGUIUICanvasLayer* m_pUICanvas; //ui canvas

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
		CGUITiledMapManager* m_pTiledMapManager;
		CGUITextureManager *m_pTextureManager;
		CGUIInterfaceManager* m_pInterfaceManager;
		CGUIMouseCursor* m_pMouseCursor;
		CGUIPropertyManager* m_pPropertyManager;
		CGUIWidgetFactory* m_pWidgetFactory;
		CGUISceneManager* m_pSceneInfoManager;
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

	inline CGUITiledMapManager* CGUISystem::GetTiledMapManager()
	{
		return m_pTiledMapManager;
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

	inline CGUISceneManager* CGUISystem::GetSceneManager()
	{
		return m_pSceneInfoManager;
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
