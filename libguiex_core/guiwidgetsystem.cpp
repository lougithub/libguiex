/** 
* @file guiwidgetsystem.cpp
* @brief widget system
* @author ken
* @date 2006-06-21
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetfactory.h>
#include <libguiex_core/guias.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guitimer.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guimousecursor.h>

#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guianimationmanager.h>
#include <libguiex_core/guifontmanager.h>
#include <libguiex_core/guiasmanager.h>
#include <libguiex_core/guitexturemanager.h>

#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacemouse.h>
#include <libguiex_core/guiinterfaceconfigfile.h>
#include <libguiex_core/guiinterfacekeyboard.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guiinterfacecommand.h>
#include <libguiex_core/guiinterfacescript.h>

#include <libguiex_core/guiperfmonitor.h>

#include <libguiex_core/guiwidgetroot.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>
#include <libguiex_core/guiwidgetmanager.h>

#include <algorithm>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	CGUISystem* GSystem = NULL;

	//------------------------------------------------------------------------------
	GUIEXPORT CGUISystem* GetSystem()
	{
		return GSystem;
	}
	//------------------------------------------------------------------------------
	GUIEXPORT void OpenDialog(CGUIWidget* pDlg)
	{
		return GSystem->OpenDialog( pDlg );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT void CloseDialog(CGUIWidget* pDlg)
	{
		return GSystem->CloseDialog( pDlg );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT void SendUIEvent(const CGUIString& rUIEventName,
		const CGUIString& rArg1,
		const CGUIString& rArg2,
		const CGUIString& rArg3,
		const CGUIString& rArg4,
		const CGUIString& rArg5,
		const CGUIString& rArg6,
		const CGUIString& rArg7,
		const CGUIString& rArg8,
		const CGUIString& rArg9)
	{
		CGUIEventUI aUIEvent;
		aUIEvent.SetArg(0, rArg1 );
		aUIEvent.SetArg(1, rArg2 );
		aUIEvent.SetArg(2, rArg3 );
		aUIEvent.SetArg(3, rArg4 );
		aUIEvent.SetArg(4, rArg5 );
		aUIEvent.SetArg(5, rArg6 );
		aUIEvent.SetArg(6, rArg7 );
		aUIEvent.SetArg(7, rArg8 );
		aUIEvent.SetArg(8, rArg9 );
		aUIEvent.SetUIName( rUIEventName );
		GSystem->SendUIEvent( &aUIEvent );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT void OpenPage( CGUIWidget* pPage )
	{
		return GSystem->OpenPage( pPage );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT void ClosePage( CGUIWidget* pPage )
	{
		return GSystem->ClosePage( pPage );
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	CGUISystem * CGUISystem::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	uint32 CGUISystem::GUI_SCREEN_WIDTH = 0;
	uint32 CGUISystem::GUI_SCREEN_HEIGHT = 0;
	//------------------------------------------------------------------------------
	CGUISystem::CGUISystem()
		:m_pWgtRoot(NULL)
		,m_pPopupWidget(NULL)
		,m_pWgtFocus(NULL)
		,m_aFrame(0)
		,m_nFps(0)
		,m_bInitialized(false)
		,m_nNameGenerateIdx(0)
		,m_bShouldRunScript(true)
		,m_bDrawExtraInfo(false)
		,m_fSystemTime(0.0f)
		,m_fTimerForFrame(0.0f)
		,m_pImageManager( NULL )
		,m_pAnimationManager( NULL )
		,m_pAsManager( NULL )
		,m_pFontManager( NULL )
		,m_pTextureManager( NULL )
		,m_pInterfaceManager( NULL )
		,m_pMouseCursor( NULL )
		,m_pPropertyManager( NULL )
		,m_pWidgetFactory( NULL )
		,m_pSceneInfoManager( NULL )
		,m_pAsFactory( NULL )
		,m_pLogMsgManager( NULL )
		,m_pWidgetManager( NULL )
	{
		GUI_ASSERT( !m_pSingleton, "[CGUISystem::CGUISystem]:instance has been created" ); 
		m_pSingleton = this; 
		GSystem = this;

		SetScreenSize(640,480);
		m_aInputProcessor.SetSystem(this);
	}
	//------------------------------------------------------------------------------
	CGUISystem::~CGUISystem()
	{
		if( IsInitialized() )
		{
			Release();
		}

		m_pSingleton = NULL;
		GSystem = NULL;
	}
	//------------------------------------------------------------------------------
	CGUISystem* CGUISystem::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::InitializeSingletons()
	{
		//init singleton classes
		m_pLogMsgManager = new CGUILogMsgManager;
		m_pWidgetManager = new CGUIWidgetManager;
		m_pImageManager = new CGUIImageManager;
		m_pAnimationManager = new CGUIAnimationManager;
		m_pAsManager = new CGUIAsManager;
		m_pFontManager = new CGUIFontManager;
		m_pTextureManager = new CGUITextureManager;
		m_pInterfaceManager = new CGUIInterfaceManager;
		m_pMouseCursor = new CGUIMouseCursor;
		m_pPropertyManager = new CGUIPropertyManager;
		m_pWidgetFactory = new CGUIWidgetFactory;
		m_pSceneInfoManager = new CGUISceneInfoManager;
		m_pAsFactory = new CGUIAsFactory;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::ReleaseSingletons()
	{
		delete m_pWidgetManager;
		m_pMouseCursor = NULL;
		delete m_pMouseCursor;
		m_pMouseCursor = NULL;
		delete m_pAnimationManager;
		m_pAnimationManager = NULL;
		delete m_pAsManager;
		m_pAsManager = NULL;
		delete m_pFontManager;
		m_pFontManager = NULL;
		delete m_pImageManager;
		m_pImageManager = NULL;
		delete m_pInterfaceManager;
		m_pInterfaceManager = NULL;
		delete m_pPropertyManager;
		m_pPropertyManager = NULL;
		delete m_pSceneInfoManager;
		m_pSceneInfoManager = NULL;
		delete m_pWidgetFactory;
		m_pWidgetFactory = NULL;
		delete m_pAsFactory;
		m_pAsFactory = NULL;
		delete m_pTextureManager;
		m_pTextureManager = NULL;

		delete m_pLogMsgManager;
		m_pLogMsgManager = NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief initialize system
	* @return 0 for success, vice versa
	*/
	int CGUISystem::Initialize()
	{
		InitializeSingletons();

		GUI_TRACE( "[CGUISystem::Initialize]: \n" );

		//register as
		if( 0 != GUIRegisterAllAs())
		{
			return -1;
		}

		//initialize performance monitor if necessary
#if	GUI_PERFORMANCE_ON 
		PERFMON_INIT(GUI_PERF_FRAME_COUNT, GUI_PERF_SECTION_NUM_MAX);
#endif	//GUI_PERFORMANCE_ON

		Reset();

		//register internal root widget
		GUI_REGISTER_WIDGET_LIB(CGUIWidgetRoot);
		GenerateRootWidget();

		m_bInitialized = true;

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::GenerateRootWidget()
	{
		GUI_ASSERT( !m_pWgtRoot, "invalid widget root pointer");
		m_pWgtRoot = CGUIWidgetManager::Instance()->CreateWidget("CGUIWidgetRoot", "__WIDGET_ROOT__auto__", "__SCENE_ROOT_auto__");
		m_pWgtRoot->Create();
		m_pWgtRoot->Open();
	}
	//------------------------------------------------------------------------------
	void CGUISystem::DestroyRootWidget()
	{
		GUI_ASSERT( m_pWgtRoot, "invalid widget root pointer");
		m_pWgtRoot->Close();
		m_pWidgetManager->DestroyWidget(m_pWgtRoot);
		m_pWgtRoot = NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief release system
	*/
	void CGUISystem::Release()
	{
		GUI_ASSERT( m_bInitialized==true, "system has been released" );

		//release performance monitor if necessary
#if	GUI_PERFORMANCE_ON 
		PERFMON_EXIT();
#endif	//GUI_PERFORMANCE_ON

		//release all widgets
		DestroyAllWidgets();

		//destroy root widget
		DestroyRootWidget();

		//release resource
		ReleaseAllResources();

		//destroy singleton instance
		ReleaseSingletons();

		m_bInitialized = false;

		CGUIObjectManager::Instance()->UnregisterAll();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief has system been initialized
	*/
	bool CGUISystem::IsInitialized() const
	{
		return m_bInitialized;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief reset system
	*/
	void CGUISystem::Reset()
	{
		m_bShouldRunScript = true;
		m_nNameGenerateIdx = 0;
		m_pPopupWidget = NULL;
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();
	}
	//------------------------------------------------------------------------------
	void CGUISystem::SetDrawExtraInfo( bool bDraw )
	{
		m_bDrawExtraInfo = bDraw;
	}
	//------------------------------------------------------------------------------
	bool	CGUISystem::IsDrawExtraInfo() const
	{
		return m_bDrawExtraInfo;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::RegisterResourceManager( CGUIResourceManagerBase* pMgr )
	{
		m_listResourceManager.push_back( pMgr );
	}
	//------------------------------------------------------------------------------
	void CGUISystem::UnregisterResourceManager( CGUIResourceManagerBase* pMgr )
	{
		for( TListResourceMgr::iterator itor = m_listResourceManager.begin();
			itor!= m_listResourceManager.end();
			++itor )
		{
			if( *itor == pMgr )
			{
				m_listResourceManager.erase( itor );
				return;
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief free resource by scene name, 
	* only free named image now.
	*/
	//void CGUISystem::FreeResource( const CGUIString& rSceneName )
	//{
	//	for( TListResourceMgr::iterator itor = m_listResourceManager.begin(); 
	//		itor != m_listResourceManager.end();
	//		++itor)
	//	{
	//		(*itor)->ReleaseResourcesByScene(rSceneName);
	//	}
	//}
	//------------------------------------------------------------------------------
	void CGUISystem::UnloadAllResource(  )
	{
		for( TListResourceMgr::iterator itor = m_listResourceManager.begin(); 
			itor != m_listResourceManager.end();
			++itor)
		{
			(*itor)->UnloadAllResources( );
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief free resource by scene name, 
	* only free named image now.
	*/
	void CGUISystem::ReleaseAllResources(  )
	{
		for( TListResourceMgr::iterator itor = m_listResourceManager.begin(); 
			itor != m_listResourceManager.end();
			++itor)
		{
			(*itor)->ReleaseAllResources( );
		}

		CGUISceneInfoManager::Instance()->ClearSceneResourceLoadFlags();
	}
	//------------------------------------------------------------------------------
	void CGUISystem::DestroyAllWidgets(  )
	{
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();

		CloseAll();

		//clear page in garbage
		RefreshGarbage();

		//destroy all page
		m_pWidgetManager->DestroyAllPages();

		IGUIInterfaceScript* pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		pScript->DestroyAllScript();
	}
	//------------------------------------------------------------------------------
	//!< set whether should system run script
	void CGUISystem::SetRunScript( bool bShouldRun )
	{
		m_bShouldRunScript = bShouldRun;
	}
	//------------------------------------------------------------------------------
	//!< return should system run script
	bool CGUISystem::ShouldRunScript() const
	{
		return m_bShouldRunScript;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process mouse and generate relative event
	* @return whether this mouse event has been processed
	*/
	bool CGUISystem::ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent)
	{
		return m_aInputProcessor.ProcessMouse(rMouseEvent);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process keyboard and generate relative event
	* @return whether this keyboard event has been processed
	*/
	bool CGUISystem::ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent )
	{
		return m_aInputProcessor.ProcessKeyboard(rKeyEvent);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Update system
	*/
	void CGUISystem::Update( real fDeltaTime )
	{
		//update time and frame
		UpdateTime( fDeltaTime );

		//execute command if it exist
		ProcessCommand();

		//update page
		if( m_pWgtRoot)
		{
			m_pWgtRoot->Update( fDeltaTime );
		}

		//update dlg
		for(TListDialog::iterator itor = m_listOpenedDlg.begin();
			itor != m_listOpenedDlg.end();
			++itor)
		{
			(*itor)->Update( fDeltaTime );
		}

		//update popup widget
		if( m_pPopupWidget )
		{
			m_pPopupWidget->Update( fDeltaTime );
		}

		RefreshGarbage();


		//update performance monitor
#if	GUI_PERFORMANCE_ON 
		PERFMON_FRAMEUPDATE();
		if( PERFMON_UPDATED() )
		{
			GUI_TRACE("\n");
			GUI_TRACE(GUI_FORMAT("Performance Monitor: %fFPS \n",PERFMON_GETFPS()));
			for( int32 i=0; i<PERFMON_GET_SECTION_NUM(); ++i)
			{
				GUI_TRACE(GUI_FORMAT(
					"%d:   %0.3f%%%%   <call times>%d��   <use time>%d����   <name><%s>\n",
					i, 
					PERFMON_GETRATE(i)*100,
					PERFMON_GETTIMES(i),
					PERFMON_GETMILLIONSEC(i),
					PERFMON_GETNAME(i)));
			}
			GUI_TRACE("\n");
		}

#endif	//GUI_PERFORMANCE_ON
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get system time, begin from 0 and increase each tick
	*/
	real CGUISystem::GetSystemTime() const
	{
		return m_fSystemTime;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the size of screen
	*/
	void CGUISystem::SetScreenSize( uint32 width, uint32 height)
	{
		if( width == GUI_SCREEN_WIDTH && height == GUI_SCREEN_HEIGHT )
		{
			return;
		}

		GUI_SCREEN_WIDTH = width;
		GUI_SCREEN_HEIGHT = height;

		m_aScreenRect = CGUIRect(
			CGUIVector2(0,0),
			CGUISize(real(CGUISystem::GUI_SCREEN_WIDTH), real(CGUISystem::GUI_SCREEN_HEIGHT)));
		m_aScreenSize = m_aScreenRect.GetSize();
		m_aScreenPos = m_aScreenRect.GetPosition();

		//update page
		if( m_pWgtRoot )
		{
			m_pWgtRoot->Refresh();
		}

		//update dlg
		for(TListDialog::iterator itor = m_listOpenedDlg.begin();
			itor != m_listOpenedDlg.end();
			++itor)
		{
			(*itor)->Refresh();
		}

		//update popup widget
		if( m_pPopupWidget )
		{
			m_pPopupWidget->Refresh();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the size of screen, in default
	*/
	const CGUISize& CGUISystem::GetScreenSize( ) const
	{
		return m_aScreenSize;
	}
	//------------------------------------------------------------------------------
	uint32 CGUISystem::GetScreenWidth( ) const
	{
		return uint32(m_aScreenSize.m_fWidth+0.5f);
	}
	//------------------------------------------------------------------------------
	uint32 CGUISystem::GetScreenHeight( ) const
	{
		return uint32(m_aScreenSize.m_fHeight+0.5f);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the position of screen, in default
	*/
	const CGUIVector2& CGUISystem::GetScreenPos( ) const
	{
		return m_aScreenPos;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the rect of screen, in default
	*/
	const CGUIRect& CGUISystem::GetScreenRect( ) const
	{
		return m_aScreenRect;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get fps of system
	*/
	int32 CGUISystem::GetFPS() const
	{
		return m_nFps;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set widget which has focus
	*/
	void CGUISystem::SetFocusWidget(CGUIWidget* pWidget)
	{
		m_pWgtFocus = pWidget;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get widget which has focus
	*/
	CGUIWidget*	CGUISystem::GetFocusWidget()
	{
		return m_pWgtFocus;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief render system
	*/
	void CGUISystem::Render()
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		pRender->SetFontRender(pFont); 

		//init render
		pRender->ResetZValue();

		pRender->BeginRender();

		//render page
		if( m_pWgtRoot )
		{
			m_pWgtRoot->Render(pRender);
		}
		//render dlg
		for(TListDialog::iterator itor = m_listOpenedDlg.begin();
			itor != m_listOpenedDlg.end();
			++itor)
		{
			(*itor)->Render(pRender);
		}
		//update popup widget
		if( m_pPopupWidget )
		{
			m_pPopupWidget->Render(pRender);
		}
		//render mouse
		CGUIMouseCursor::Instance()->Render(pRender);

		if( IsDrawExtraInfo() )
		{
			//draw extra info
			//render page
			if( m_pWgtRoot )
			{
				m_pWgtRoot->RenderExtraInfo(pRender);
			}
			//render dlg
			for(TListDialog::iterator itor = m_listOpenedDlg.begin();
				itor != m_listOpenedDlg.end();
				++itor)
			{
				(*itor)->RenderExtraInfo(pRender);
			}
			//update popup widget
			if( m_pPopupWidget )
			{
				m_pPopupWidget->RenderExtraInfo(pRender);
			}
		}

		pRender->EndRender();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief open a page
	*/
	void CGUISystem::OpenPage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( m_pWidgetManager->HasPage( pPage ) == false )
		{
			throw CGUIException( "[CGUISystem::OpenPage]: the widget <%s> isn't a page!", pPage->GetName().c_str());
		}

		pPage->SetParent( m_pWgtRoot );
		pPage->Open();
		pPage->Refresh();

		m_vOpenedPage.push_back(pPage);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief close a page
	*/
	void CGUISystem::ClosePage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( m_pWidgetManager->HasPage( pPage ) == false )
		{
			throw CGUIException( "[CGUISystem::ClosePage]: the widget <%s> isn't a page!", pPage->GetName().c_str());
		}

		if( pPage->IsOpen() == false )
		{
			throw CGUIException( "[CGUISystem::ClosePage]: the page <%s> has closed!", pPage->GetName().c_str());
		}

		std::vector<CGUIWidget*>::iterator itor = std::find(m_vOpenedPage.begin(), m_vOpenedPage.end(), pPage );
		if( itor == m_vOpenedPage.end())
		{
			throw CGUIException( "[CGUISystem::ClosePage]: can't find page in opend page <%s> list!", pPage->GetName().c_str());
		}
		m_vOpenedPage.erase( itor );
		pPage->Close();
		pPage->SetParent(NULL);

		if( m_pWidgetManager->HasDynamicPage( pPage ) )
		{
			AddToDynamicGarbage( pPage );
		}
		return;
	}
	//------------------------------------------------------------------------------
	///< get opened page num, which should be opened by method OpenPage()
	uint32 CGUISystem::GetOpenedPageNum() const
	{
		return m_vOpenedPage.size();
	}
	//------------------------------------------------------------------------------
	///< get opened page by index
	CGUIWidget*	CGUISystem::GetOpenedPageByIndex( uint32 nIdx )
	{
		if( nIdx >= m_vOpenedPage.size())
		{
			throw CGUIException( "[CGUISystem::GetOpenedPageByIndex]: the given index <%d> is overflow, total opened page size is <%d>",
				nIdx, m_vOpenedPage.size());
		}

		return m_vOpenedPage[nIdx];
	}
	//------------------------------------------------------------------------------
	/**
	* @brief generate a anonymous name
	* @return return a string
	*/
	const char* CGUISystem::GenerateAnonymousName()
	{
		snprintf( m_szAnonymousName, 32, "%d__auto__", m_nNameGenerateIdx++ );
		return m_szAnonymousName;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get widget by position.
	* if the widget is disabled or invisible, this widget will be skipped
	* @return return the widget where the point hits,
	* NULL if there hasn't any point.
	*/
	CGUIWidget*	CGUISystem::GetWidgetUnderPoint(const CGUIVector2& rPos)
	{
		//capture input
		CGUIWidget* pRootWidget = GetCurrentRootWidget();
		if( pRootWidget )
		{
			return pRootWidget->GetWidgetAtPoint(rPos);
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISystem::UpdateTime(real fDeltaTime)
	{
		m_fSystemTime += fDeltaTime;

		//calculate frame
		++m_aFrame;
		if( m_fSystemTime - m_fTimerForFrame >= 1.0f )
		{
			m_fTimerForFrame = m_fSystemTime;
			m_nFps = m_aFrame;
			//GUI_TRACE( GUI_FORMAT( "\n--- FRAME %d --- \n",  m_aFrame));
			m_aFrame = 0;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISystem::AddToGarbage( CGUIWidget* pWidget )
	{
		m_vecPageGarbage.push_back(pWidget);
	}
	//------------------------------------------------------------------------------
	void CGUISystem::AddToDynamicGarbage( CGUIWidget* pWidget )
	{
		m_vecDynamicPageGarbage.push_back(pWidget);
	}
	//------------------------------------------------------------------------------
	void CGUISystem::RefreshGarbage( )
	{
		if( !m_vecPageGarbage.empty() ||
			!m_vecDynamicPageGarbage.empty() )
		{
			m_aInputProcessor.Reset();
		}

		for( std::vector<CGUIWidget*>::iterator itor = m_vecPageGarbage.begin();
			itor != m_vecPageGarbage.end();
			++itor )
		{
			m_pWidgetManager->DestroyPage( *itor );
		}
		m_vecPageGarbage.clear();

		for( std::vector<CGUIWidget*>::iterator itor = m_vecDynamicPageGarbage.begin();
			itor != m_vecDynamicPageGarbage.end();
			++itor )
		{
			m_pWidgetManager->DestroyDynamicPage( *itor );
		}
		m_vecDynamicPageGarbage.clear();
	}
	//------------------------------------------------------------------------------
	///process command
	void CGUISystem::ProcessCommand()
	{
		//IGUIInterfaceCommand* pCommand =  CGUIInterfaceManager::Instance()->GetInterfaceCommand();
		//if( pCommand )
		//{
		//	const char* pBuf = pCommand->Receive();
		//	if( pBuf )
		//	{
		//		IGUIInterfaceScript*	pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		//		try
		//		{
		//			pScript->ExecuteString(pBuf);
		//		}
		//		catch(const CGUIException_Script& rErr )
		//		{
		//			pCommand->Send(rErr.what());
		//		}
		//	}
		//}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief open modal dialog
	*/
	void CGUISystem::OpenDialog(CGUIWidget* pDlg)
	{
		GUI_ASSERT(pDlg, "invalid parameter");
		GUI_ASSERT( pDlg->GetParent() == false, "the modal dialog shouldn't have a parent");

		if( !m_pWidgetManager->HasPage( pDlg ))
		{
			throw CGUIException( "[CGUISystem::OpenDialog]: the dialog <%s:%s> isn't a page", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
		}

		pDlg->SetParent( NULL );

		for( TListDialog::iterator itor = m_listOpenedDlg.begin();
			itor != m_listOpenedDlg.end();
			++itor)
		{
			if( (*itor) == pDlg)
			{
				throw CGUIException( "[CGUISystem::OpenDialog]: failed to open dialog<%s:%s>, it has opened!", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
			}
		}

		m_listOpenedDlg.push_back(pDlg);
		pDlg->Open();

		GUI_TRACE( GUI_FORMAT("OpenDialog <%s : %s> \n", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str()));
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get current modal dialog
	*/
	CGUIWidget*	CGUISystem::GetTopestDialog( ) const 
	{
		if(m_listOpenedDlg.empty())
		{
			return NULL;
		}
		else
		{
			return m_listOpenedDlg.back();
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief close modal dialog
	*/
	void CGUISystem::CloseDialog(CGUIWidget* pDlg)
	{
		GUI_ASSERT(pDlg, "invalid parameter");

		for( TListDialog::iterator itor = m_listOpenedDlg.begin();
			itor != m_listOpenedDlg.end();
			++itor)
		{
			if( (*itor) == pDlg)
			{		
				m_listOpenedDlg.erase(itor);
				UngisterGlobalKeyByRoot(pDlg);
				pDlg->Close();

				if( m_pWidgetManager->HasDynamicPage( pDlg ) )
				{
					AddToDynamicGarbage( pDlg );
				}
				return;
			}
		}

		throw CGUIException( "[CGUISystem::CloseDialog]: failed to close dialog <%s : %s>.", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief open popup widget
	*/
	void CGUISystem::OpenPopupWidget(CGUIWidget* pWidget)
	{
		GUI_ASSERT(pWidget, "invalid parameter");

		if( m_pPopupWidget )
		{
			GUI_FORCE_ASSERT(GUI_FORMAT("failed to open popup widget, a popup widget has opened! <%s>", m_pPopupWidget->GetName().c_str()));
		}

		pWidget->Open();
		m_pPopupWidget = pWidget;

		GUI_TRACE( GUI_FORMAT("OpenPopupWidget <%s> \n", pWidget->GetName().c_str()));
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get current modal dialog
	*/
	CGUIWidget* CGUISystem::GetCurrentPopupWidget( ) const
	{
		return m_pPopupWidget;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief close modal dialog
	*/
	void CGUISystem::ClosePopupWidget(CGUIWidget* pWidget)
	{
		GUI_ASSERT(pWidget, "invalid parameter");

		if( m_pPopupWidget = pWidget )
		{
			pWidget->Close();
			m_pPopupWidget = NULL;
			GUI_TRACE( GUI_FORMAT("ClosePopupWidget <%s> \n", pWidget->GetName().c_str()));
			return;
		}
		else
		{
			GUI_FORCE_ASSERT(GUI_FORMAT("failed to close popup widget <%s>", pWidget->GetName().c_str()));
		}
	}
	//------------------------------------------------------------------------------

	/** 
	* @brief get current root widget, will be a dialog if there has or a page
	*/
	CGUIWidget*	CGUISystem::GetCurrentRootWidget( ) const
	{
		CGUIWidget* pDlgRoot = NULL;
		if( pDlgRoot = GetCurrentPopupWidget())
		{
			return pDlgRoot;
		}
		else if( pDlgRoot = GetTopestDialog())
		{
			return pDlgRoot;
		}
		else
		{
			return m_pWgtRoot;
		}
	}
	//------------------------------------------------------------------------------
	void			CGUISystem::CloseAll()
	{
		//close all popup widget
		while( GetCurrentPopupWidget())
		{
			ClosePopupWidget(GetCurrentPopupWidget());
		}

		//close all modal dialog
		while(GetTopestDialog())
		{
			CloseDialog(GetTopestDialog());
		}

		//close all page
		while( !m_vOpenedPage.empty())
		{
			ClosePage(*m_vOpenedPage.begin());
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register a event for this Widget.this widget should unregister
	* the event manually.
	*/
	void CGUISystem::RegisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget)
	{
		GUI_ASSERT( pWidget, "invalid parameter");

		TMapUIEvent::iterator itor = m_mapUIEvent.find(rEventName);
		if( itor == m_mapUIEvent.end())
		{
			itor = m_mapUIEvent.insert( std::make_pair( rEventName, TEventReceiver())).first;
		}

		if( std::find( itor->second.begin(), itor->second.end(), pWidget ) == itor->second.end())
		{
			itor->second.push_back( pWidget );
		}
		return;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister a event for this Widget.
	*/
	void CGUISystem::UnregisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget)
	{
		GUI_ASSERT( pWidget, "invalid parameter");

		TMapUIEvent::iterator itor = m_mapUIEvent.find(rEventName);
		if( itor == m_mapUIEvent.end())
		{
			return;
		}
		TEventReceiver::iterator itorVector = std::find( itor->second.begin(), itor->second.end(), pWidget );
		if( itorVector != itor->second.end())
		{
			itor->second.erase( itorVector);
		}
		if( itor->second.empty())
		{
			m_mapUIEvent.erase(itor);
		}
		return;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister a event for this Widget.
	*/
	void CGUISystem::UnregisterUIEvent( CGUIWidget* pWidget)
	{
		GUI_ASSERT( pWidget, "invalid parameter");

		for( TMapUIEvent::iterator itor = m_mapUIEvent.begin();
			itor != m_mapUIEvent.end();
			++itor)
		{
			TEventReceiver::iterator itorVector = std::find( itor->second.begin(), itor->second.end(), pWidget );
			while( itorVector != itor->second.end())
			{
				itor->second.erase( itorVector);
				itorVector = std::find( itor->second.begin(), itor->second.end(), pWidget );
			}
		}
		return;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief send a ui event.if there is no widget registered for this event, it
	* will be ignored.
	*/
	void CGUISystem::SendUIEvent( CGUIEventUI* pEvent )
	{
		GUI_ASSERT( pEvent->GetReceiver() == NULL, "invalid parameter");

		GUI_TRACE( GUI_FORMAT("[CGUISystem::SendUIEvent]: %s\n", pEvent->GetUIName().c_str() ));

		TMapUIEvent::iterator itor = m_mapUIEvent.find(pEvent->GetUIName());
		if( itor == m_mapUIEvent.end())
		{
			return;
		}
		TEventReceiver arrayEventReceiver = itor->second;
		for( TEventReceiver::iterator itorVector = arrayEventReceiver.begin();
			itorVector != arrayEventReceiver.end();
			++itorVector )
		{
			pEvent->SetReceiver( *itorVector );
			pEvent->Process();
			if( pEvent->IsConsumed())
			{
				return;
			}
		}
		return;
	}
	//------------------------------------------------------------------------------
	void	CGUISystem::SendEvent(CGUIEvent * pEvent )
	{
		pEvent->Process();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister all ui event
	*/
	void CGUISystem::UnregisterAllUIEvent( )
	{
		m_mapUIEvent.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register global key
	* @param pReceiver the widget which will receive the global key event
	* @pRoot the root widget of this receiver, is a root of page or a root of dialog
	*/
	void CGUISystem::RegisterGlobalKeyReceiver( CGUIWidget* pReceiver, CGUIWidget* pRoot)
	{
		m_vecGlobalKeyObj.push_back(std::make_pair(pReceiver, pRoot));
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief remove key event by root widget
	*/
	void CGUISystem::UngisterGlobalKeyByRoot( CGUIWidget* pRoot)
	{
		for( uint32 i=0;i<m_vecGlobalKeyObj.size();)
		{
			if( m_vecGlobalKeyObj[i].second == pRoot)
			{
				m_vecGlobalKeyObj.erase(m_vecGlobalKeyObj.begin()+i);
			}
			else
			{
				++i;
			}
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief remove all key event
	*/
	void CGUISystem::UngisterAllGlobalKey( )
	{
		m_vecGlobalKeyObj.clear();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief process the global key event
	* @return whether this event is comsumed
	*/
	bool CGUISystem::ProcessGlobalKeyEvent(CGUIEventKeyboard* pEvent)
	{
		CGUIWidget* pRoot = GetCurrentRootWidget();
		if( !pRoot)
		{
			//no root widget here
			return false;
		}


		for( TGlobalKeyObj::reverse_iterator itor = m_vecGlobalKeyObj.rbegin();
			itor != m_vecGlobalKeyObj.rend();
			++itor)
		{
			CGUIWidget* pReceiver = (*itor).first;
			CGUIWidget* pReceiverRoot = (*itor).second;
			if( pReceiverRoot == pRoot )
			{
				pEvent->SetReceiver(pReceiver);
				GSystem->SendEvent(pEvent);
				if( pEvent->IsConsumed())
				{
					break;
				}
			}
		}

		return pEvent->IsConsumed();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief set data path of the ui system, the path should end by "/" or "\"
	*/
	void CGUISystem::SetDataPath(const CGUIString& rDataPath)
	{
		m_strDataPath = rDataPath;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get data path of the ui system, the path will end by "/" or "\"
	*/
	const CGUIString& CGUISystem::GetDataPath() const
	{
		return m_strDataPath;
	}
	//------------------------------------------------------------------------------
}//namespace guiex