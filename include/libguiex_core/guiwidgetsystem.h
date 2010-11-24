/** 
* @file guiwidgetsystem.h
* @brief widget system
* @author ken
* @date 2006-06-21
*/


#ifndef __GUI_WIDGETSYSTEM_20060621_H__
#define __GUI_WIDGETSYSTEM_20060621_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisingleton.h"
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
}

//============================================================================//
// enum
//============================================================================// 

//============================================================================//
// define
//============================================================================// 
#define GUI_CREATE_WIDGET(type, widgetname, scenename)		\
	::guiex::CGUIWidgetSystem::Instance()->CreateWidget(type,widgetname, scenename)

#define GUI_DESTROY_WIDGET(widget)		\
	::guiex::CGUIWidgetSystem::Instance()->DestroyWidget(widget)

#define GUI_GET_WIDGET(widgetname, scenename)		\
	::guiex::CGUIWidgetSystem::Instance()->GetWidget(widgetname, scenename)


#define GUI_WIDGET_SYSTEM( )		\
	::guiex::CGUIWidgetSystem::Instance()




//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWidgetSystem
	* @brief widget system
	*/
	class GUIEXPORT CGUIWidgetSystem
	{
	public:
		CGUIWidgetSystem();
		~CGUIWidgetSystem();

		static CGUIWidgetSystem* Instance();

		int Initialize();
		void Release();
		bool IsInitialized() const;
		void Update( real fDeltaTime );
		void Render();

		void SetDrawExtraInfo( bool bDraw );
		bool IsDrawExtraInfo() const;

		real GetGlobalTimer() const;

		void SetDataPath(const CGUIString& rDataPath);
		const CGUIString& GetDataPath() const;

		CGUIWidget* CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rSceneName );
		void DestroyWidget(CGUIWidget* pWidget);
		CGUIWidget* GetWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName );
		bool HasWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName );

		const char*	 GenerateAnonymousName();

		void SetFocusWidget(CGUIWidget* pWidget);
		CGUIWidget*	GetFocusWidget();

		//********************************************************
		//	page and dialog
		//********************************************************
		void OpenPage(CGUIWidget* pPage);
		void ClosePage(CGUIWidget* pPage);
		uint32 GetOpenedPageNum() const;
		CGUIWidget* GetOpenedPageByIndex( uint32 nIdx );
		void AddPage(  CGUIWidget* pPage, const CGUIString& rConfigFileName = CGUIString());
		CGUIWidget* GetPage( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		CGUIWidget* GetPageByFilename( const CGUIString& rFilename, const CGUIString& rSceneName );
		bool HasPage(const CGUIString& rWidgetName, const CGUIString& rSceneName) const;
		bool HasPage( CGUIWidget* pPage) const;
		void DeletePage( CGUIWidget* pPage);

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

		int32 GetFPS() const;

		CGUIWidget* GetWidgetUnderPoint(const CGUIVector2& rPos);

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

		CGUIWidget*	LoadDynamicPage( const CGUIString& rPageFileName, const CGUIString& rPageSceneName, const CGUIString& rWorkingSceneName );
		CGUIWidget*	LoadPage( const CGUIString& rFileName, const CGUIString& rSceneName);

		void FreeResource( const CGUIString& rSceneName );
		void UnloadAllResource();
		void FreeAllWidgets(  );

		void ReleaseAllResources(  );

		void SetRunScript( bool bShouldRun );
		bool ShouldRunScript() const;

		bool ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent );

		int32 LoadConfigFile( const CGUIString& rPath, const CGUIString& rSceneName, const CGUIString& rFileName );

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

	protected:
		friend class CGUIWidget;
		friend class CGUIInputProcessor;

		/// update time and frame
		void UpdateTime(real fDeltaTime);

		void AddToGarbage( CGUIWidget* pWidget );
		void RefreshGarbage( );

		int32 DoLoadConfig_Widget( const CGUIProperty* pPropertySet, const CGUIString& rSceneName, const CGUIString& rFileName );
		int32 DoLoadConfig_Set( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );
		int32 DoLoadConfig_Script( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );
		int32 DoLoadConfig_Image( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );
		int32 DoLoadConfig_Animation( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );
		int32 DoLoadConfig_Font( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );
		int32 DoLoadConfig_As( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );


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
		real m_fGlobalTimer; ///global time
		real m_fTimerForFrame; ///time used to calculate frame
		int32 m_aFrame; ///frame of system
		int32 m_nFps; ///current fps
		//----------------------------------------------------------------------

		CGUIInputProcessor m_aInputProcessor; ///input processor

		//----------------------------------------------------------------------
		//widget list
		typedef std::map<CGUIString , std::map<CGUIString, CGUIWidget*> > TMapWidget; //scene name, widget name, widget
		TMapWidget m_aMapWidget; ///contain all widget create by system which has name
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//page list
		struct SPageInfo
		{
			CGUIWidget* m_pPage;
			CGUIString m_strConfigFileName;
		};
		typedef std::vector< SPageInfo > TVecPage; //<widget pointer, configure file
		TVecPage m_vecPage;
		std::set<CGUIWidget*> m_setDynamicPageList;
		std::vector<CGUIWidget*> m_vOpenedPage;

		std::vector<CGUIWidget*> m_vecPageGarbage;
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//widget modal dialog
		typedef std::list<CGUIWidget*>	TListDialog;	
		TListDialog	m_listOpenedDlg; ///contain modal dialog
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
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

		CGUIString m_strDefaultCode; //multi bytes code
		CGUIString m_strDataPath; //data path

		//----------------------------------------------------------------------
		//index for name generating
		uint32 m_nNameGenerateIdx;
		char m_szAnonymousName[32];
		//index for name generating
		//----------------------------------------------------------------------

		bool m_bDrawExtraInfo;

		bool m_bShouldRunScript; //!< should system run script

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

	private:
		bool m_bInitialized;

	private:
		static CGUIWidgetSystem* m_pSingleton;
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
	GUIEXPORT CGUIWidgetSystem* GetWidgetSystem();

	GUIEXPORT CGUIWidget* GetWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName );

	GUIEXPORT CGUIWidget* LoadDynamicPage( const CGUIString& rPageFileName, const CGUIString& rPageSceneName, const CGUIString& rWorkingSceneName );
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

#endif //__GUI_WIDGETSYSTEM_20060621_H__
