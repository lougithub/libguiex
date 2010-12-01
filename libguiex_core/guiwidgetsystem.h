/** 
* @file guiwidgetsystem.h
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
#include "guiinterfacemouse.h"
#include "guitimer.h"
#include "guistring.h"
#include "guirect.h"
#include "guievent.h"
#include "guiinputprocessor.h"
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
	class CGUIProperty;

	//for singleton
	class CGUIImageManager;
	class CGUIAnimationManager;
	class CGUIAsManager;
	class CGUIFontManager;
	class CGUITextureManager;
	class CGUIInterfaceManager;
	class CGUIMouseCursor;
	class CGUIPropertyManager;
	class CGUIWidgetFactory;
	class CGUIAsFactory;
	class CGUISceneInfoManager;
	class CGUILogMsgManager;
	class CGUIWidgetManager;

	extern CGUISystem* GSystem;
}

//============================================================================//
// enum
//============================================================================// 

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUISystem
	* @brief widget system
	*/
	class GUIEXPORT CGUISystem
	{
	public:
		CGUISystem();
		~CGUISystem();

		static CGUISystem* Instance();

		int Initialize();
		bool IsInitialized() const;
		void Release();

		void Update( real fDeltaTime );
		void Render();

		void SetDrawExtraInfo( bool bDraw );
		bool IsDrawExtraInfo() const;

		void SetDataPath(const CGUIString& rDataPath);
		const CGUIString& GetDataPath() const;

		const char* GenerateAnonymousName();

		void SetFocusWidget(CGUIWidget* pWidget);
		CGUIWidget*	GetFocusWidget();

		CGUIWidget* GetWidgetUnderPoint(const CGUIVector2& rPos);


		//********************************************************
		//	page and dialog
		//********************************************************
		void OpenPage(CGUIWidget* pPage);
		void ClosePage(CGUIWidget* pPage);
		uint32 GetOpenedPageNum() const;
		CGUIWidget* GetOpenedPageByIndex( uint32 nIdx );

		void OpenDialog(CGUIWidget* pDlg);
		CGUIWidget* GetTopestDialog( ) const;
		void CloseDialog(CGUIWidget* pDlg);

		void OpenPopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentPopupWidget( ) const;
		void ClosePopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentRootWidget( ) const;

		void CloseAll();

		//********************************************************
		//	screen size
		//********************************************************
		void SetScreenSize( uint32 width, uint32 height);
		const CGUISize& GetScreenSize( ) const;
		uint32 GetScreenWidth( ) const;
		uint32 GetScreenHeight( ) const;
		const CGUIVector2& GetScreenPos( ) const;
		const CGUIRect& GetScreenRect( ) const;


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
		void ReleaseAllResources(  );

		void DestroyAllWidgets(  );

		void SetRunScript( bool bShouldRun );
		bool ShouldRunScript() const;

		bool ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent );

		real GetSystemTime() const;
		int32 GetFPS() const;

		//********************************************************
		//	singleton
		//********************************************************
		CGUIImageManager* GetImageManager()
		{
			return m_pImageManager;
		}

		CGUIAnimationManager* GetAnimationManager()
		{
			return m_pAnimationManager;
		}
		
		CGUIAsManager* GetAsManager()
		{
			return m_pAsManager;
		}

		CGUIFontManager* GetFontManager()
		{
			return m_pFontManager;
		}

		CGUITextureManager* GetTextureManager()
		{
			return m_pTextureManager;
		}

		CGUIInterfaceManager* GetInterfaceManager()
		{
			return m_pInterfaceManager;
		}

		CGUIMouseCursor* GetMouseCursor()
		{
			return m_pMouseCursor;
		}

		CGUIPropertyManager* GetPropertyManager()
		{
			return m_pPropertyManager;
		}
		
		CGUIWidgetFactory* GetWidgetFactory()
		{
			return m_pWidgetFactory;
		}

		CGUISceneInfoManager* GetSceneInfoManager()
		{
			return m_pSceneInfoManager;
		}

		CGUIAsFactory* GetAsFactory()
		{
			return m_pAsFactory;
		}

		CGUILogMsgManager* GetLogMsgManager()
		{
			return m_pLogMsgManager;
		}

		CGUIWidgetManager* GetWidgetManager()
		{
			return m_pWidgetManager;
		}

	protected:
		friend class CGUIWidget;
		friend class CGUIInputProcessor;

		/// update time and frame
		void UpdateTime(real fDeltaTime);

		void AddToGarbage( CGUIWidget* pWidget );
		void AddToDynamicGarbage( CGUIWidget* pWidget );
		void RefreshGarbage( );
	
		void GenerateRootWidget();
		void DestroyRootWidget();

		void InitializeSingletons();
		void ReleaseSingletons();


	protected:
		void ProcessCommand();
		bool ProcessGlobalKeyEvent(CGUIEventKeyboard* pEvent);

	public:
		static uint32 GUI_SCREEN_WIDTH; //!< size of screen, in pixel
		static uint32 GUI_SCREEN_HEIGHT; //!< size of screen, in pixel

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
		CGUIRect m_aScreenRect; //!< rect of screen, in default
		CGUISize m_aScreenSize; //!< size of screen, in default
		CGUIVector2 m_aScreenPos; //!< pos of screen, in default
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
		std::vector<CGUIWidget*> m_vOpenedPage;
		std::vector<CGUIWidget*> m_vecPageGarbage;
		std::vector<CGUIWidget*> m_vecDynamicPageGarbage;

		//widget modal dialog
		typedef std::list<CGUIWidget*>	TListDialog;	
		TListDialog	m_listOpenedDlg; ///contain modal dialog
		
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

		bool m_bDrawExtraInfo;
		bool m_bShouldRunScript; //!< should system run script

		typedef std::list<CGUIResourceManagerBase*>	TListResourceMgr;
		TListResourceMgr m_listResourceManager;

		//single classes
		CGUIImageManager *m_pImageManager;
		CGUIAnimationManager *m_pAnimationManager;
		CGUIAsManager *m_pAsManager;
		CGUIFontManager *m_pFontManager;
		CGUITextureManager *m_pTextureManager;
		CGUIInterfaceManager* m_pInterfaceManager;
		CGUIMouseCursor* m_pMouseCursor;
		CGUIPropertyManager* m_pPropertyManager;
		CGUIWidgetFactory* m_pWidgetFactory;
		CGUISceneInfoManager* m_pSceneInfoManager;
		CGUIAsFactory* m_pAsFactory;
		CGUILogMsgManager* m_pLogMsgManager;
		CGUIWidgetManager* m_pWidgetManager;
		 
	private:
		bool m_bInitialized;

	private:
		static CGUISystem* m_pSingleton;
	};

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

	GUIEXPORT void OpenPage( CGUIWidget* pPage );
	GUIEXPORT void ClosePage( CGUIWidget* pPage );

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
