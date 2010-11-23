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
	/**
	* @brief initialize system
	* @return 0 for success, vice versa
	*/
	int CGUIWidgetSystem::Initialize()
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
	/**
	* @brief release system
	*/
	void CGUIWidgetSystem::Release()
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
	/** 
	* @brief has system been initialized
	*/
	bool CGUIWidgetSystem::IsInitialized() const
	{
		return m_bInitialized;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief reset system
	*/
	void CGUIWidgetSystem::Reset()
	{
		m_bShouldRunScript = true;
		m_nNameGenerateIdx = 0;
		m_pPopupWidget = NULL;
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetSystem::SetDrawExtraInfo( bool bDraw )
	{
		m_bDrawExtraInfo = bDraw;
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidgetSystem::IsDrawExtraInfo() const
	{
		return m_bDrawExtraInfo;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file
	*/
	CGUIWidget*	CGUIWidgetSystem::LoadPage( const CGUIString& rFileName, const CGUIString& rSceneName )
	{
		//load file
		CGUIString strRelPath = CGUISceneInfoManager::Instance()->GetScenePath( rSceneName ) + rFileName;
		if( 0 != LoadConfigFile( strRelPath, rSceneName, rFileName ))
		{
			throw CGUIException(
				"[CGUIWidgetSystem::LoadPage]: failed to load config file <%s>!",
				strRelPath.c_str());
			return NULL;
		}

		//get page
		CGUIWidget* pPageWidget = GetPageByFilename( rFileName, rSceneName );
		if( !pPageWidget )
		{
			throw CGUIException( "[CGUIWidgetSystem::LoadPage]: failed to get page!" );
			return NULL;
		}
		pPageWidget->NotifyLoaded();

		return pPageWidget;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file
	*/
	CGUIWidget*	CGUIWidgetSystem::LoadDynamicPage( 
		const CGUIString& rPageFileName,
		const CGUIString& rPageSceneName,
		const CGUIString& rWorkingSceneName )
	{
		//load file
		CGUIString strRelPath = CGUISceneInfoManager::Instance()->GetScenePath( rPageSceneName ) + rPageFileName;
		CGUIString strDynamicSceneName = rPageSceneName + GenerateAnonymousName();
		if( 0 != LoadConfigFile( strRelPath, strDynamicSceneName, rPageFileName ))
		{
			throw CGUIException(
				"[CGUIWidgetSystem::LoadDynamicPage]: failed to load config file <%s>!",
				strRelPath.c_str());
			return NULL;
		}

		//get page
		CGUIWidget* pPageWidget = GetPage( rPageFileName, strDynamicSceneName );
		if( !pPageWidget )
		{
			throw CGUIException( "[CGUIWidgetSystem::LoadDynamicPage]: failed to get page!" );
			return NULL;
		}
		pPageWidget->SetWorkingSceneName(rWorkingSceneName);
		m_setDynamicPageList.insert( pPageWidget );
		pPageWidget->NotifyLoaded();
		return pPageWidget;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file and load datas
	*/
	int32 CGUIWidgetSystem::LoadConfigFile( const CGUIString& rPath, const CGUIString& rSceneName, const CGUIString& rFileName)
	{
		//get interface of config file
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			throw CGUIException("[CGUIWidgetSystem::LoadResource]: failed to get interface <IGUIConfigFile>!");
		}

		//get property set
		CGUIProperty aPropertySet;
		if( 0 != pConfigFile->LoadConfigFile(rPath, aPropertySet) )
		{
			throw CGUIException(
				"[CGUIWidgetSystem::LoadResource]: failed to load config file <%s : %s>!", 
				rSceneName.c_str(), 
				rPath.c_str());
		}

		//parse it
		uint32 nSize = aPropertySet.GetPropertyNum();
		for( uint32 i=0; i<nSize; ++i )
		{
			const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
			switch( pProperty->GetType() )
			{
			case ePropertyType_Widget:
				if( 0 != DoLoadConfig_Widget( pProperty, rSceneName, rFileName ))
				{
					return -1;
				}
				break;

			case ePropertyType_Set:
				if( 0 != DoLoadConfig_Set( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			case ePropertyType_Script:
				if( 0 != DoLoadConfig_Script( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			case ePropertyType_ImageDefine:
				if( 0 != DoLoadConfig_Image( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			case ePropertyType_AnimationDefine:
				if( 0 != DoLoadConfig_Animation( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			case ePropertyType_FontDefine:
				if( 0 != DoLoadConfig_Font( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			case ePropertyType_AsDefine:
				if( 0 != DoLoadConfig_As( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			default:
				{
					throw guiex::CGUIException(
						"[CGUIWidgetSystem::LoadConfigFile], unknown property type <%s:%s:%s>", 
						pProperty->GetName().c_str(),
						pProperty->GetTypeAsString().c_str(),
						pProperty->GetValue().c_str());
				}
				return -1;
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_Widget( const CGUIProperty* pPropertySet, const CGUIString& rSceneName, const CGUIString& rFileName )
	{
		/// create widget
		const CGUIString& strWidgetType = pPropertySet->GetValue( );
		const CGUIString& strWidgetName = pPropertySet->GetName( );
		CGUIWidget* pWidget = CGUIWidgetSystem::Instance()->CreateWidget(strWidgetType, strWidgetName, rSceneName);
		if( !pWidget )
		{
			throw CGUIException(
				"[CGUIWidgetSystem::DoLoadConfig_Widget]: failed to create widget <%s:%s>!",
				strWidgetType.c_str(),
				strWidgetName.c_str());
			return -1;
		}
		/// set property to widget
		pWidget->SetProperty( *pPropertySet );
		pWidget->LoadProperty();
		pWidget->Create();

		if( pWidget->GetParent() == NULL )
		{
			AddPage( pWidget, rFileName );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_Set( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		const CGUIString& strSetName = pPropertySet->GetName( );
		//register set
		CGUIPropertyManager::Instance()->RegisterSet( strSetName, *pPropertySet );

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_Script( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		const CGUIString& strRelativePath = pPropertySet->GetValue();
		guiex::IGUIInterfaceScript* pInterfaceScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		if( pInterfaceScript &&
			guiex::CGUIWidgetSystem::Instance()->ShouldRunScript())
		{
			// create script
			pInterfaceScript->CreateScript( rSceneName );

			// load script
			CGUIString strPath = CGUISceneInfoManager::Instance()->GetScenePath( rSceneName ) + strRelativePath;
			pInterfaceScript->ExecuteFile(strPath, rSceneName);
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_Image( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		if( 0 != CGUIImageManager::Instance()->RegisterImage( rSceneName, *pPropertySet ) )
		{
			throw guiex::CGUIException(
				"[IGUIConfigFile_tinyxml::DoLoadResourceImage], failed to create image with name <%s:%s:%s>!", 
				pPropertySet->GetName().c_str(),
				pPropertySet->GetTypeAsString().c_str(),
				pPropertySet->GetValue().c_str());
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_Animation( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		if( 0 != CGUIAnimationManager::Instance()->RegisterAnimation( rSceneName, *pPropertySet ) )
		{
			throw guiex::CGUIException(
				"[CGUIWidgetSystem::DoLoadConfig_Animation], failed to create Animation with name <%s:%s:%s>!", 
				pPropertySet->GetName().c_str(),
				pPropertySet->GetTypeAsString().c_str(),
				pPropertySet->GetValue().c_str());
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_Font( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		if( 0 != CGUIFontManager::Instance()->RegisterFont( rSceneName, *pPropertySet ) )
		{
			throw guiex::CGUIException(
				"[CGUIWidgetSystem::DoLoadConfig_Font], failed to create font with name <%s:%s:%s>!", 
				pPropertySet->GetName().c_str(),
				pPropertySet->GetTypeAsString().c_str(),
				pPropertySet->GetValue().c_str());
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidgetSystem::DoLoadConfig_As( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		if( 0 != CGUIAsManager::Instance()->RegisterAs( rSceneName, *pPropertySet ) )
		{
			throw guiex::CGUIException(
				"[CGUIWidgetSystem::DoLoadConfig_As], failed to create font with name <%s:%s:%s>!", 
				pPropertySet->GetName().c_str(),
				pPropertySet->GetTypeAsString().c_str(),
				pPropertySet->GetValue().c_str());
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief free resource by scene name, 
	* only free named image now.
	*/
	void CGUIWidgetSystem::FreeResource( const CGUIString& rSceneName )
	{
		CGUIImageManager::Instance()->ReleaseResourcesByScene(rSceneName);
		CGUIAnimationManager::Instance()->ReleaseResourcesByScene(rSceneName);
		CGUIFontManager::Instance()->ReleaseResourcesByScene(rSceneName);
		CGUIAsManager::Instance()->ReleaseResourcesByScene(rSceneName);
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetSystem::UnloadAllResource(  )
	{
		CGUIImageManager::Instance()->UnloadAllResources( );
		CGUIAnimationManager::Instance()->UnloadAllResources( );
		CGUIFontManager::Instance()->UnloadAllResources( );
		CGUIAsManager::Instance()->UnloadAllResources( );

		//CGUITextureManager::Instance()->DestroyAllTextureImplement();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief free resource by scene name, 
	* only free named image now.
	*/
	void CGUIWidgetSystem::ReleaseAllResources(  )
	{
		CGUIImageManager::Instance()->ReleaseAllResources( );
		CGUIAnimationManager::Instance()->ReleaseAllResources( );
		CGUIFontManager::Instance()->ReleaseAllResources( );
		CGUIAsManager::Instance()->ReleaseAllResources( );

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
			CGUIWidget* pPage = m_vecPage[0].m_pPage;
			pPage->SetParent(NULL);
			DeletePage(pPage);
		}

		IGUIInterfaceScript*	pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		pScript->DestroyAllScript();
	}
	//------------------------------------------------------------------------------
	//!< set whether should system run script
	void CGUIWidgetSystem::SetRunScript( bool bShouldRun )
	{
		m_bShouldRunScript = bShouldRun;
	}
	//------------------------------------------------------------------------------
	//!< return should system run script
	bool CGUIWidgetSystem::ShouldRunScript() const
	{
		return m_bShouldRunScript;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process mouse and generate relative event
	* @return whether this mouse event has been processed
	*/
	bool CGUIWidgetSystem::ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent)
	{
		return m_aInputProcessor.ProcessMouse(rMouseEvent);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process keyboard and generate relative event
	* @return whether this keyboard event has been processed
	*/
	bool CGUIWidgetSystem::ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent )
	{
		return m_aInputProcessor.ProcessKeyboard(rKeyEvent);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Update system
	*/
	void CGUIWidgetSystem::Update( real fDeltaTime )
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
	/**
	* @brief get global time
	*/
	real CGUIWidgetSystem::GetGlobalTimer() const
	{
		return m_fGlobalTimer;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the size of screen
	*/
	void CGUIWidgetSystem::SetScreenSize( uint32 width, uint32 height)
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
	/**
	* @brief get the size of screen, in default
	*/
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
	/**
	* @brief get the position of screen, in default
	*/
	const CGUIVector2& CGUIWidgetSystem::GetScreenPos( ) const
	{
		return m_aScreenPos;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the rect of screen, in default
	*/
	const CGUIRect& CGUIWidgetSystem::GetScreenRect( ) const
	{
		return m_aScreenRect;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get fps of system
	*/
	int32 CGUIWidgetSystem::GetFPS() const
	{
		return m_nFps;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set widget which has focus
	*/
	void CGUIWidgetSystem::SetFocusWidget(CGUIWidget* pWidget)
	{
		m_pWgtFocus = pWidget;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get widget which has focus
	*/
	CGUIWidget*	CGUIWidgetSystem::GetFocusWidget()
	{
		return m_pWgtFocus;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief render system
	*/
	void CGUIWidgetSystem::Render()
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
	void CGUIWidgetSystem::OpenPage(CGUIWidget* pPage)
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
	/**
	* @brief close a page
	*/
	void CGUIWidgetSystem::ClosePage(CGUIWidget* pPage)
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
	///< get opened page num, which should be opened by method OpenPage()
	uint32 CGUIWidgetSystem::GetOpenedPageNum() const
	{
		return m_vOpenedPage.size();
	}
	//------------------------------------------------------------------------------
	///< get opened page by index
	CGUIWidget*	CGUIWidgetSystem::GetOpenedPageByIndex( uint32 nIdx )
	{
		if( nIdx >= m_vOpenedPage.size())
		{
			throw CGUIException( "[CGUIWidgetSystem::GetOpenedPageByIndex]: the given index <%d> is overflow, total opened page size is <%d>",
				nIdx, m_vOpenedPage.size());
		}

		return m_vOpenedPage[nIdx];
	}
	//------------------------------------------------------------------------------
	/**
	* @brief add a page to map
	*/
	void CGUIWidgetSystem::AddPage( CGUIWidget* pPage, const CGUIString& rConfigFileName)
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
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_strConfigFileName == rConfigFileName && 
				rPageInfo.m_pPage->GetSceneName() == pPage->GetSceneName() )
			{
				throw CGUIException( "[CGUIWidgetSystem::AddPage]: the page's file name <%s> has existed!", rConfigFileName.c_str());
			}
		}

		SPageInfo aInfo;
		aInfo.m_pPage = pPage;
		aInfo.m_strConfigFileName = rConfigFileName;
		m_vecPage.push_back( aInfo );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get a page by name
	*/
	CGUIWidget* CGUIWidgetSystem::GetPage( const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage->GetName() == rWidgetName && 
				rPageInfo.m_pPage->GetSceneName() == rSceneName )
			{
				return rPageInfo.m_pPage;
			}
		}

		throw CGUIException( "[CGUIWidgetSystem::GetPage]: failed to get page whose name is <%s : %s>!", rSceneName.c_str(), rWidgetName.c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get a page page's configure file name
	*/
	CGUIWidget* CGUIWidgetSystem::GetPageByFilename( const CGUIString& rFilename, const CGUIString& rSceneName )
	{
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage->GetSceneName() == rSceneName && 
				rPageInfo.m_strConfigFileName == rFilename )
			{
				return rPageInfo.m_pPage;
			}
		}

		throw CGUIException( "[CGUIWidgetSystem::GetPageByFilename]: failed to get page whose config file name is <%s>!", rFilename.c_str());
		return NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief delete specify page
	*/
	void CGUIWidgetSystem::DeletePage( CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter");

		TVecPage::iterator itor = m_vecPage.begin();
		for( ; itor != m_vecPage.end(); ++itor)
		{
			SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage == pPage )
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
	/**
	* @brief is there a page whose name is given name
	*/
	bool CGUIWidgetSystem::HasPage(const CGUIString& rWidgetName, const CGUIString& rSceneName) const
	{
		for( TVecPage::const_iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage->GetName() == rWidgetName &&
				rPageInfo.m_pPage->GetSceneName() == rSceneName )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetSystem::HasPage(CGUIWidget* pPage) const
	{
		TVecPage::const_iterator itor = m_vecPage.begin();
		for( ; itor != m_vecPage.end(); ++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage == pPage )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a widget by type and name
	* @return pointer of created widget
	*/
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
	/**
	* @brief destroy widget
	*/
	void CGUIWidgetSystem::DestroyWidget(CGUIWidget* pWidget)
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
	/**
	* @brief get widget by given name
	* @return return NULL for error
	*/
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
	/**
	* @brief has widget which has given name
	* @return return true for has, vice versa
	*/
	bool CGUIWidgetSystem::HasWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName )
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
	/**
	* @brief generate a anonymous name
	* @return return a string
	*/
	const char* CGUIWidgetSystem::GenerateAnonymousName()
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
	///process command
	void CGUIWidgetSystem::ProcessCommand()
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
	void CGUIWidgetSystem::OpenDialog(CGUIWidget* pDlg)
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
	/** 
	* @brief get current modal dialog
	*/
	CGUIWidget*	CGUIWidgetSystem::GetTopestDialog( ) const 
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
	/** 
	* @brief open popup widget
	*/
	void CGUIWidgetSystem::OpenPopupWidget(CGUIWidget* pWidget)
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
	CGUIWidget* CGUIWidgetSystem::GetCurrentPopupWidget( ) const
	{
		return m_pPopupWidget;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief close modal dialog
	*/
	void CGUIWidgetSystem::ClosePopupWidget(CGUIWidget* pWidget)
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
	CGUIWidget*	CGUIWidgetSystem::GetCurrentRootWidget( ) const
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
	/**
	* @brief register a event for this Widget.this widget should unregister
	* the event manually.
	*/
	void CGUIWidgetSystem::RegisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget)
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
	void CGUIWidgetSystem::UnregisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget)
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
	void CGUIWidgetSystem::UnregisterUIEvent( CGUIWidget* pWidget)
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
	void CGUIWidgetSystem::SendUIEvent( CGUIEventUI* pEvent )
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
	/**
	* @brief unregister all ui event
	*/
	void CGUIWidgetSystem::UnregisterAllUIEvent( )
	{
		m_mapUIEvent.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register global key
	* @param pReceiver the widget which will receive the global key event
	* @pRoot the root widget of this receiver, is a root of page or a root of dialog
	*/
	void CGUIWidgetSystem::RegisterGlobalKeyReceiver( CGUIWidget* pReceiver, CGUIWidget* pRoot)
	{
		m_vecGlobalKeyObj.push_back(std::make_pair(pReceiver, pRoot));
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief remove key event by root widget
	*/
	void CGUIWidgetSystem::UngisterGlobalKeyByRoot( CGUIWidget* pRoot)
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
	void CGUIWidgetSystem::UngisterAllGlobalKey( )
	{
		m_vecGlobalKeyObj.clear();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief process the global key event
	* @return whether this event is comsumed
	*/
	bool CGUIWidgetSystem::ProcessGlobalKeyEvent(CGUIEventKeyboard* pEvent)
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
	/** 
	* @brief set data path of the ui system, the path should end by "/" or "\"
	*/
	void CGUIWidgetSystem::SetDataPath(const CGUIString& rDataPath)
	{
		m_strDataPath = rDataPath;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get data path of the ui system, the path will end by "/" or "\"
	*/
	const CGUIString& CGUIWidgetSystem::GetDataPath() const
	{
		return m_strDataPath;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
