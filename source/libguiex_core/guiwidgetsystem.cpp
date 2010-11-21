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

#include <algorithm>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT	CGUIWidgetSystem* GetWidgetSystem()
	{
		return CGUIWidgetSystem::Instance();
	}
	//------------------------------------------------------------------------------
	GUIEXPORT	CGUIWidget* GetWidget(const CGUIString& rWidgetName, const CGUIString& rSceneName)
	{
		return CGUIWidgetSystem::Instance()->GetWidget(rWidgetName, rSceneName);
	}
	//------------------------------------------------------------------------------
	GUIEXPORT CGUIWidget*	LoadDynamicPage( const CGUIString& rPageFileName, const CGUIString& rPageSceneName, const CGUIString& rWorkingSceneName )
	{
		return CGUIWidgetSystem::Instance()->LoadDynamicPage(rPageFileName, rPageSceneName, rWorkingSceneName);
	}
	//------------------------------------------------------------------------------
	GUIEXPORT	void			OpenDialog(CGUIWidget* pDlg)
	{
		return CGUIWidgetSystem::Instance()->OpenDialog( pDlg );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT	void			CloseDialog(CGUIWidget* pDlg)
	{
		return CGUIWidgetSystem::Instance()->CloseDialog( pDlg );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT	void			SendUIEvent(const CGUIString& rUIEventName,
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
		CGUIWidgetSystem::Instance()->SendUIEvent( &aUIEvent );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT	void			OpenPage( CGUIWidget* pPage )
	{
		return CGUIWidgetSystem::Instance()->OpenPage( pPage );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT	void			ClosePage( CGUIWidget* pPage )
	{
		return CGUIWidgetSystem::Instance()->ClosePage( pPage );
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	GUI_SINGLETON_IMPLEMENT_EX(CGUIWidgetSystem);	
	//------------------------------------------------------------------------------
	uint32		CGUIWidgetSystem::GUI_SCREEN_WIDTH = 0;
	uint32		CGUIWidgetSystem::GUI_SCREEN_HEIGHT = 0;
	//------------------------------------------------------------------------------
	CGUIWidgetSystem::CGUIWidgetSystem()
		:m_pWgtRoot(NULL)
		,m_pPopupWidget(NULL)
		,m_pWgtFocus(NULL)
		,m_aFrame(0)
		,m_nFps(0)
		,m_bInitialized(false)
		,m_nNameGenerateIdx(0)
		,m_bShouldRunScript(true)
		,m_bDrawExtraInfo(false)
		,m_fGlobalTimer(0.0f)
		,m_fTimerForFrame(0.0f)
	{
		SetScreenSize(640,480);
		m_aInputProcessor.SetSystem(this);
	}
	//------------------------------------------------------------------------------
	CGUIWidgetSystem::~CGUIWidgetSystem()
	{

	}
	//------------------------------------------------------------------------------
	int		CGUIWidgetSystem::Initialize()
	{
		GUI_TRACE( "[CGUIWidgetSystem::Initialize]: \n" );

		//register as
		if( 0 != GUIRegisterAllAs())
		{
			return -1;
		}

		//initialize performance monitor if necessary
#if	GUI_PERFORMANCE_ON 
		PERFMON_INIT(GUI_PERF_FRAME_COUNT, GUI_PERF_SECTION_NUM_MAX);
#endif	//GUI_PERFORMANCE_ON

		m_strDefaultCode = "UTF-8";

		Reset();

		//register internal root widget
		GUI_REGISTER_WIDGET_LIB(CGUIWidgetRoot);
		m_pWgtRoot = GUI_CREATE_WIDGET("CGUIWidgetRoot", "__WIDGET_ROOT__auto__", "__SCENE_ROOT_auto__");
		m_pWgtRoot->Create();
		m_pWgtRoot->Open();

		m_bInitialized = true;

		return 0;
	}
	//------------------------------------------------------------------------------
	void		CGUIWidgetSystem::Release()
	{
		//release performance monitor if necessary
#if	GUI_PERFORMANCE_ON 
		PERFMON_EXIT();
#endif	//GUI_PERFORMANCE_ON

		
		//mouse cursor
		CGUIMouseCursor::DestroyInstance();

		//release all widgets
		FreeAllWidgets();

		//destroy root widget
		m_pWgtRoot->Close();
		DestroyWidget(m_pWgtRoot);
		m_pWgtRoot = NULL;

		//check
		GUI_ASSERT(m_aMapWidget.empty(), "shouldn't has any widget in map");

		//release resource
		ReleaseAllResources();

		//unregister  generator and module
		CGUIWidgetFactory::Instance()->UnregisterAllGenerator();
		CGUIWidgetFactory::Instance()->UnregisterAllModule();

		// release as
		CGUIAsFactory::Instance()->UnregisterAllGenerator();

		//release interface
		CGUIInterfaceManager::Instance()->UnregisterAllInterface();

		m_bInitialized = false;
	
		CGUIObjectManager::Instance()->UnregisterAll();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::IsInitialized() const
	{
		return m_bInitialized;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::Reset()
	{
		m_bShouldRunScript = true;
		m_nNameGenerateIdx = 0;
		m_pPopupWidget = NULL;
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::SetDrawExtraInfo( bool bDraw )
	{
		m_bDrawExtraInfo = bDraw;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::IsDrawExtraInfo() const
	{
		return m_bDrawExtraInfo;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidgetSystem::LoadPage( const CGUIString& rFileName, const CGUIString& rSceneName)
	{
		CGUIString strRelPath = CGUISceneInfoManager::Instance()->GetScenePath( rSceneName ) + rFileName;
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			throw CGUIException("[CGUIWidgetSystem::LoadPage]: failed to get interface <IGUIConfigFile>!");
		}
		CGUIWidget* pWidget = pConfigFile->LoadWidgetConfigFile(strRelPath, rSceneName);
		if( !pWidget )
		{
			return NULL;
		}
		AddPage(  pWidget, rFileName);
		return pWidget;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidgetSystem::LoadDynamicPage( 
		const CGUIString& rPageFileName,
		const CGUIString& rPageSceneName,
		const CGUIString& rWorkingSceneName )
	{
		CGUIString strRelPath = CGUISceneInfoManager::Instance()->GetScenePath( rPageSceneName ) + rPageFileName;
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			throw CGUIException("[CGUIWidgetSystem::LoadDynamicPage]: failed to get interface <IGUIConfigFile>!");
		}
		CGUIString strDynamicSceneName = 	rPageSceneName + GenerateAnonymousName();
		CGUIWidget* pWidget = pConfigFile->LoadWidgetConfigFile(strRelPath, strDynamicSceneName);
		if( !pWidget )
		{
			return NULL;
		}
		pWidget->SetWorkingSceneName(rWorkingSceneName);
		AddPage(  pWidget, rPageFileName );
		m_setDynamicPageList.insert( pWidget );
		return pWidget;
	}
	//------------------------------------------------------------------------------
	int32	CGUIWidgetSystem::LoadResource( const CGUIString& rFileName, const CGUIString& rSceneName)
	{
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			throw CGUIException("[CGUIWidgetSystem::LoadResource]: failed to get interface <IGUIConfigFile>!");
		}
		if( 0 != pConfigFile->LoadResourceConfigFile(rFileName, rSceneName) )
		{
			throw CGUIException("[CGUIWidgetSystem::LoadResource]: failed to load config file <%s : %s>!", rSceneName.c_str(), rFileName.c_str());
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::FreeResource( const CGUIString& rSceneName )
	{
		CGUIImageManager::Instance()->ReleaseResourcesByScene(rSceneName);
		CGUIAnimationManager::Instance()->ReleaseResourcesByScene(rSceneName);
		CGUIFontManager::Instance()->ReleaseResourcesByScene(rSceneName);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::UnloadAllResource(  )
	{
		CGUIImageManager::Instance()->UnloadAllResources( );
		CGUIAnimationManager::Instance()->UnloadAllResources( );
		CGUIFontManager::Instance()->UnloadAllResources( );

		//CGUITextureManager::Instance()->DestroyAllTextureImplement();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::ReleaseAllResources(  )
	{
		CGUIImageManager::Instance()->ReleaseAllResources( );
		CGUIAnimationManager::Instance()->ReleaseAllResources( );
		CGUIFontManager::Instance()->ReleaseAllResources( );
		
		CGUISceneInfoManager::Instance()->ClearSceneResourceLoadFlags();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::FreeAllWidgets(  )
	{
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();

		CloseAll();

		//clear page in gargabe
		RefreshGarbage();

		//destroy all page
		while(m_vecPage.empty() == false )
		{
			CGUIWidget* pPage = (*m_vecPage.begin()).first;
			pPage->SetParent(NULL);
			DeletePage(pPage);
		}
				
		IGUIInterfaceScript*	pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		pScript->DestroyAllScript();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::SetRunScript( bool bShouldRun )
	{
		m_bShouldRunScript = bShouldRun;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::ShouldRunScript() const
	{
		return m_bShouldRunScript;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent)
	{
		return m_aInputProcessor.ProcessMouse(rMouseEvent);
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent )
	{
		return m_aInputProcessor.ProcessKeyboard(rKeyEvent);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::Update( real fDeltaTime )
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
					"%d:   %0.3f%%%%   <call times>%d¥Œ   <use time>%d∫¡√Î   <name><%s>\n",
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
	real CGUIWidgetSystem::GetGlobalTimer() const
	{
		return m_fGlobalTimer;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::SetScreenSize( uint32 width, uint32 height)
	{
		if( width == GUI_SCREEN_WIDTH && height == GUI_SCREEN_HEIGHT )
		{
			return;
		}

		GUI_SCREEN_WIDTH = width;
		GUI_SCREEN_HEIGHT = height;

		m_aScreenRect = CGUIRect(
			CGUIVector2(0,0),
			CGUISize(real(CGUIWidgetSystem::GUI_SCREEN_WIDTH), real(CGUIWidgetSystem::GUI_SCREEN_HEIGHT)));
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
	const CGUISize& CGUIWidgetSystem::GetScreenSize( ) const
	{
		return m_aScreenSize;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidgetSystem::GetScreenWidth( ) const
	{
		return uint32(m_aScreenSize.m_fWidth+0.5f);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidgetSystem::GetScreenHeight( ) const
	{
		return uint32(m_aScreenSize.m_fHeight+0.5f);
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CGUIWidgetSystem::GetScreenPos( ) const
	{
		return m_aScreenPos;
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CGUIWidgetSystem::GetScreenRect( ) const
	{
		return m_aScreenRect;
	}
	//------------------------------------------------------------------------------
	int32			CGUIWidgetSystem::GetFPS() const
	{
		return m_nFps;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::SetFocusWidget(CGUIWidget* pWidget)
	{
		m_pWgtFocus = pWidget;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidgetSystem::GetFocusWidget()
	{
		return m_pWgtFocus;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::Render()
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
	void			CGUIWidgetSystem::OpenPage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( HasPage( pPage ) == false )
		{
			throw CGUIException( "[CGUIWidgetSystem::OpenPage]: the widget <%s> isn't a page!", pPage->GetName().c_str());
		}

		pPage->SetParent( m_pWgtRoot );
		pPage->Open();
		pPage->Refresh();

		m_vOpenedPage.push_back(pPage);
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::ClosePage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( HasPage( pPage ) == false )
		{
			throw CGUIException( "[CGUIWidgetSystem::ClosePage]: the widget <%s> isn't a page!", pPage->GetName().c_str());
		}

		if( pPage->IsOpen() == false )
		{
			throw CGUIException( "[CGUIWidgetSystem::ClosePage]: the page <%s> has closed!", pPage->GetName().c_str());
		}

		std::vector<CGUIWidget*>::iterator itor = std::find(m_vOpenedPage.begin(), m_vOpenedPage.end(), pPage );
		if( itor == m_vOpenedPage.end())
		{
			throw CGUIException( "[CGUIWidgetSystem::ClosePage]: can't find page in opend page <%s> list!", pPage->GetName().c_str());
		}
		m_vOpenedPage.erase( itor );
		pPage->Close();
		pPage->SetParent(NULL);

		if( m_setDynamicPageList.find( pPage ) != m_setDynamicPageList.end())
		{
			m_setDynamicPageList.erase( pPage );
			AddToGarbage( pPage );
		}
		return;
	}
	//------------------------------------------------------------------------------
	uint32			CGUIWidgetSystem::GetOpenedPageNum() const
	{
		return m_vOpenedPage.size();
	}
	//------------------------------------------------------------------------------
	CGUIWidget*		CGUIWidgetSystem::GetOpenedPageByIndex( uint32 nIdx )
	{
		if( nIdx >= m_vOpenedPage.size())
		{
			throw CGUIException( "[CGUIWidgetSystem::GetOpenedPageByIndex]: the given index <%d> is overflow, total opened page size is <%d>",
				nIdx, m_vOpenedPage.size());
		}

		return m_vOpenedPage[nIdx];
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::AddPage( CGUIWidget* pPage, const CGUIString& rSceneName)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		//check
		if( HasPage( pPage->GetName(), pPage->GetSceneName()))
		{
			throw CGUIException( "[CGUIWidgetSystem::AddPage]: the page name <%s> has existed!", pPage->GetName().c_str());
		}
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			if( itor->second == rSceneName && itor->first->GetSceneName() == pPage->GetSceneName() )
			{
				throw CGUIException( "[CGUIWidgetSystem::AddPage]: the page's file name <%s> has existed!", rSceneName.c_str());
			}
		}

		m_vecPage.push_back( std::make_pair(pPage, rSceneName));
	}
	//------------------------------------------------------------------------------
	CGUIWidget*		CGUIWidgetSystem::GetPage( const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			if( itor->first->GetName() == rWidgetName && itor->first->GetSceneName() == rSceneName )
			{
				return (*itor).first;
			}
		}

		throw CGUIException( "[CGUIWidgetSystem::GetPage]: failed to get page whose name is <%s : %s>!", rSceneName.c_str(), rWidgetName.c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*		CGUIWidgetSystem::GetPageByFilename( const CGUIString& rFilename, const CGUIString& rSceneName )
	{
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			if( itor->first->GetSceneName() == rSceneName && itor->second == rFilename )
			{
				return (*itor).first;
			}
		}

		throw CGUIException( "[CGUIWidgetSystem::GetPageByFilename]: failed to get page whose config file name is <%s>!", rFilename.c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::DeletePage( CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter");

		TVecPage::iterator itor = m_vecPage.begin();
		for( ; itor != m_vecPage.end(); ++itor)
		{
			if( (*itor ).first == pPage )
			{
				break;
			}
		}
		if( itor == m_vecPage.end())
		{
			throw CGUIException( "[CGUIWidgetSystem::DeletePage]: failed to find page whose name is <%s>!", pPage->GetName().c_str());
		}

		if( pPage->IsOpen() )
		{
			throw CGUIException( "[CGUIWidgetSystem::DeletePage]: can't delete widget <%s> who is still opened!", pPage->GetName().c_str());
		}
		if( pPage->GetParent())
		{
			throw CGUIException( "[CGUIWidgetSystem::DeletePage]: can't delete widget <%s> who is still has a father!", pPage->GetName().c_str());
		}

		m_vecPage.erase(itor);
		DestroyWidget(pPage);
	}
	//------------------------------------------------------------------------------
	bool			CGUIWidgetSystem::HasPage(const CGUIString& rWidgetName, const CGUIString& rSceneName) const
	{
		for( TVecPage::const_iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			if( (*itor).first->GetName() == rWidgetName && (*itor).first->GetSceneName() == rSceneName )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	bool			CGUIWidgetSystem::HasPage(CGUIWidget* pPage) const
	{
		TVecPage::const_iterator itor = m_vecPage.begin();
		for( ; itor != m_vecPage.end(); ++itor)
		{
			if( (*itor ).first == pPage )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIWidgetSystem::CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		CGUIWidget* pWidget = NULL;

		if( !rWidgetName.empty() && !rSceneName.empty() )
		{
			//check whether this widget has exist
			TMapWidget::iterator itor = m_aMapWidget.find( rSceneName );
			std::map<CGUIString, CGUIWidget*>* pWidgetMap = NULL;
			if( itor != m_aMapWidget.end())
			{
				if( itor->second.find( rWidgetName ) != itor->second.end())
				{
					throw CGUIException( "[CGUIWidgetSystem::CreateWidget]: the widget [%s : %s] has existed!", rSceneName.c_str(), rWidgetName.c_str());
				}
				pWidgetMap = &itor->second;
			}
			else
			{
				//add scene
				itor = m_aMapWidget.insert( std::make_pair( rSceneName, std::map<CGUIString, CGUIWidget*>())).first;
				pWidgetMap = &itor->second;
			}

			pWidget = CGUIWidgetFactory::Instance()->CreateWidget( rType, rWidgetName, rSceneName );
			pWidgetMap->insert(std::make_pair(rWidgetName, pWidget));
		}
		else
		{
			throw CGUIException( "CGUIWidgetSystem::CreateWidget: lack name for type <%s>!", rType.c_str());
		}

		return pWidget;
	}
	//------------------------------------------------------------------------------
	void					CGUIWidgetSystem::DestroyWidget(CGUIWidget* pWidget)
	{
		if( !pWidget )
		{
			throw CGUIException( "[CGUIWidgetSystem::DestroyWidget]: Invalid parameter");
		}

		if( !pWidget->GetName().empty() && !pWidget->GetSceneName().empty())
		{
			//check
			TMapWidget::iterator itor = m_aMapWidget.find( pWidget->GetSceneName());
			if( itor  == m_aMapWidget.end())
			{
				throw CGUIException( "[CGUIWidgetSystem::DestroyWidget]: the widget <%s : %s> doesn't exist!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str());
			}
			std::map<CGUIString, CGUIWidget*>::iterator  itor_widget = itor->second.find(pWidget->GetName());
			if( itor_widget  == itor->second.end())
			{
				throw CGUIException( "[CGUIWidgetSystem::DestroyWidget]: the widget <%s : %s> doesn't exist!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str());
			}

			//delete it
			if( pWidget->GetParent())
			{
				pWidget->SetParent(NULL);
			}
			CGUIWidgetFactory::Instance()->DestoryWidget(pWidget);
			itor->second.erase(itor_widget);
			if( itor->second.empty())
			{
				m_aMapWidget.erase( itor );
			}
		}
		else
		{
			throw CGUIException( "CGUIWidgetSystem::DestroyWidget: lack name for type <%s>!", pWidget->GetType().c_str());
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIWidgetSystem::GetWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		TMapWidget::iterator itor = m_aMapWidget.find(rSceneName);
		if( itor != m_aMapWidget.end())
		{
			std::map<CGUIString, CGUIWidget*>::iterator itor_widget = itor->second.find( rWidgetName );
			if( itor_widget != itor->second.end())
			{
				return itor_widget->second;
			}
		}

		throw CGUIException("[CGUIWidgetSystem::GetWidget]: failed to get widget by name [%s : %s]",rSceneName.c_str(), rWidgetName.c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	bool			CGUIWidgetSystem::HasWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		TMapWidget::iterator itor = m_aMapWidget.find(rSceneName);
		if( itor != m_aMapWidget.end())
		{
			std::map<CGUIString, CGUIWidget*>::iterator itor_widget = itor->second.find( rWidgetName );
			if( itor_widget != itor->second.end())
			{
				return true;
			}
		}

		return false;
	}
	//------------------------------------------------------------------------------
	const char*		CGUIWidgetSystem::GenerateAnonymousName()
	{
		snprintf( m_szAnonymousName, 32, "%d__auto__", m_nNameGenerateIdx++ );
		return m_szAnonymousName;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidgetSystem::GetWidgetUnderPoint(const CGUIVector2& rPos)
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
	void			CGUIWidgetSystem::UpdateTime(real fDeltaTime)
	{
		m_fGlobalTimer += fDeltaTime;

		//calculate frmae
		++m_aFrame;
		if( m_fGlobalTimer - m_fTimerForFrame >= 1000 )
		{
			m_fTimerForFrame = m_fGlobalTimer;
			m_nFps = m_aFrame;
			//GUI_TRACE( GUI_FORMAT( "\n--- FRAME %d --- \n",  m_aFrame));
			m_aFrame = 0;
		}
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::AddToGarbage( CGUIWidget* pWidget )
	{
		m_vecPageGarbage.push_back(pWidget);
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::RefreshGarbage( )
	{
		if( !m_vecPageGarbage.empty())
		{
			m_aInputProcessor.Reset();
		}

		for( std::vector<CGUIWidget*>::iterator itor = m_vecPageGarbage.begin();
			itor != m_vecPageGarbage.end();
			++itor )
		{
			DeletePage( *itor );
		}
		m_vecPageGarbage.clear();
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::ProcessCommand()
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
	void			CGUIWidgetSystem::OpenDialog(CGUIWidget* pDlg)
	{
		GUI_ASSERT(pDlg, "invalid parameter");
		GUI_ASSERT( pDlg->GetParent() == false, "the modal dialog shouldn't have a parent");

		if( !HasPage( pDlg ))
		{
			throw CGUIException( "[CGUIWidgetSystem::OpenDialog]: the dialog <%s:%s> isn't a page", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
		}

		pDlg->SetParent( NULL );

		for( TListDialog::iterator itor = m_listOpenedDlg.begin();
			itor != m_listOpenedDlg.end();
			++itor)
		{
			if( (*itor) == pDlg)
			{
				throw CGUIException( "[CGUIWidgetSystem::OpenDialog]: failed to open dialog<%s:%s>, it has opened!", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
			}
		}

		m_listOpenedDlg.push_back(pDlg);
		pDlg->Open();

		GUI_TRACE( GUI_FORMAT("OpenDialog <%s : %s> \n", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str()));
	}
	//------------------------------------------------------------------------------
	CGUIWidget*		CGUIWidgetSystem::GetTopestDialog( ) const 
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
	void			CGUIWidgetSystem::CloseDialog(CGUIWidget* pDlg)
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

				if( m_setDynamicPageList.find( pDlg ) != m_setDynamicPageList.end())
				{
					m_setDynamicPageList.erase( pDlg );
					AddToGarbage( pDlg );
				}
				return;
			}
		}

		throw CGUIException( "[CGUIWidgetSystem::CloseDialog]: failed to close dialog <%s : %s>.", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::OpenPopupWidget(CGUIWidget* pWidget)
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
	CGUIWidget*		CGUIWidgetSystem::GetCurrentPopupWidget( ) const
	{
		return m_pPopupWidget;
	}
	//------------------------------------------------------------------------------
	void			CGUIWidgetSystem::ClosePopupWidget(CGUIWidget* pWidget)
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
	CGUIWidget*		CGUIWidgetSystem::GetCurrentRootWidget( ) const
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
	void			CGUIWidgetSystem::CloseAll()
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
	void	CGUIWidgetSystem::RegisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget)
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
	void	CGUIWidgetSystem::UnregisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget)
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
	void	CGUIWidgetSystem::UnregisterUIEvent( CGUIWidget* pWidget)
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
	void	CGUIWidgetSystem::SendUIEvent( CGUIEventUI* pEvent )
	{
		GUI_ASSERT( pEvent->GetReceiver() == NULL, "invalid parameter");

		GUI_TRACE( GUI_FORMAT("[CGUIWidgetSystem::SendUIEvent]: %s\n", pEvent->GetUIName().c_str() ));

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
	void	CGUIWidgetSystem::SendEvent(CGUIEvent * pEvent )
	{
		pEvent->Process();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::UnregisterAllUIEvent( )
	{
		m_mapUIEvent.clear();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::RegisterGlobalKeyReceiver( CGUIWidget* pReceiver, CGUIWidget* pRoot)
	{
		m_vecGlobalKeyObj.push_back(std::make_pair(pReceiver, pRoot));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidgetSystem::UngisterGlobalKeyByRoot( CGUIWidget* pRoot)
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
	void	CGUIWidgetSystem::UngisterAllGlobalKey( )
	{
		m_vecGlobalKeyObj.clear();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::ProcessGlobalKeyEvent(CGUIEventKeyboard* pEvent)
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
				CGUIWidgetSystem::Instance()->SendEvent(pEvent);
				if( pEvent->IsConsumed())
				{
					break;
				}
			}
		}

		return pEvent->IsConsumed();
	}
	//------------------------------------------------------------------------------
	void				CGUIWidgetSystem::SetDataPath(const CGUIString& rDataPath)
	{
		m_strDataPath = rDataPath;
	}
	//------------------------------------------------------------------------------
	const CGUIString&	CGUIWidgetSystem::GetDataPath() const
	{
		return m_strDataPath;
	}
	//------------------------------------------------------------------------------
	void				CGUIWidgetSystem::SetDefaultCode(const char* pDataPath)
	{
		m_strDefaultCode = pDataPath;
	}
	//------------------------------------------------------------------------------
	const char*			CGUIWidgetSystem::GetDefaultCode() const
	{
		return m_strDefaultCode.c_str();
	}
	//------------------------------------------------------------------------------
}//namespace guiex
