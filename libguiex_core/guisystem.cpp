/** 
* @file guiwidgetsystem.cpp
* @brief widget system
* @author ken
* @date 2006-06-21
*/


//============================================================================//
// include
//============================================================================// 
#include "guisystem.h"
#include "guiwidget.h"
#include "guiwidgetfactory.h"
#include "guias.h"
#include "guievent.h"
#include "guiexception.h"
#include "guitimer.h"
#include "guilogmsgmanager.h"
#include "guimousecursor.h"

#include "guiimagemanager.h"
#include "guianimationmanager.h"
#include "guifontmanager.h"
#include "guiasmanager.h"
#include "guitexturemanager.h"
#include "guisoundmanager.h"
#include "guiparticle2dmanager.h"
#include "guitiledmapmanager.h"
#include "guimusicmanager.h"
#include "guicameramanager.h"
#include "guicanvaslayermanager.h"

#include "guiinterfacemanager.h"
#include "guiinterfacemouse.h"
#include "guiinterfaceconfigfile.h"
#include "guiinterfacekeyboard.h"
#include "guiinterfacerender.h"
#include "guiinterfacefont.h"
#include "guiinterfacecommand.h"
#include "guiinterfacescript.h"

#include "guiperfmonitor.h"

#include "guiscene.h"
#include "guiscenemanager.h"
#include "guiwidgetmanager.h"

#include "guiuicanvaslayer.h"

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

	//------------------------------------------------------------------------------
	CGUISystem * CGUISystem::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUISystem::CGUISystem()
		:m_pWgtFocus(NULL)
		,m_aFrame(0)
		,m_nFps(0)
		,m_bInitialized(false)
		,m_nNameGenerateIdx(0)
		,m_bDrawExtraInfo(false)
		,m_bPlayingAs( true )
		,m_bIsEditorMode( false )
		,m_fSystemTime(0.0f)
		,m_fTimerForFrame(0.0f)
		,m_pImageManager( NULL )
		,m_pAnimationManager( NULL )
		,m_pSoundManager(NULL )
		,m_pMusicManager(NULL )
		,m_pAsManager( NULL )
		,m_pParticle2DManager( NULL )
		,m_pTiledMapManager( NULL )
		,m_pFontManager( NULL )
		,m_pTextureManager( NULL )
		,m_pInterfaceManager( NULL )
		,m_pMouseCursor( NULL )
		,m_pPropertyManager( NULL )
		,m_pWidgetFactory( NULL )
		,m_pSceneInfoManager( NULL )
		,m_pLogMsgManager( NULL )
		,m_pWidgetManager( NULL )
		,m_pCameraManager( NULL )
		,m_pUICanvas( NULL )
		,m_eScreenOrientation( eScreenOrientation_Portrait )
	{
		GUI_ASSERT( !m_pSingleton, "[CGUISystem::CGUISystem]:instance has been created" ); 
		GUI_ASSERT( !GSystem, "[CGUISystem::CGUISystem]:GSystem has been set" ); 
		m_pSingleton = this; 
		GSystem = this;
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
		m_pSoundManager = new CGUISoundManager;
		m_pMusicManager = new CGUIMusicManager;
		m_pAsManager = new CGUIAsManager;
		m_pParticle2DManager = new CGUIParticle2DManager;
		m_pTiledMapManager = new CGUITiledMapManager;
		m_pFontManager = new CGUIFontManager;
		m_pTextureManager = new CGUITextureManager;
		m_pInterfaceManager = new CGUIInterfaceManager;
		m_pMouseCursor = new CGUIMouseCursor;
		m_pPropertyManager = new CGUIPropertyManager;
		m_pWidgetFactory = new CGUIWidgetFactory;
		m_pSceneInfoManager = new CGUISceneManager;
		m_pCameraManager = new CGUICameraManager;
		m_pCanvasLayerManager = new CGUICanvasLayerManager;
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
		delete m_pSoundManager;
		m_pSoundManager = NULL;
		delete m_pMusicManager;
		m_pMusicManager = NULL;
		delete m_pAsManager;
		m_pAsManager = NULL;
		delete m_pParticle2DManager;
		m_pParticle2DManager = NULL;
		delete m_pTiledMapManager;
		m_pTiledMapManager = NULL;
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
		delete m_pTextureManager;
		m_pTextureManager = NULL;
		delete m_pCameraManager;
		m_pCameraManager = NULL;
		delete m_pCanvasLayerManager;
		m_pCanvasLayerManager = NULL;
		

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

		//initialize performance monitor if necessary
		PERFMON_INIT(GUI_PERF_FRAME_COUNT, GUI_PERF_SECTION_NUM_MAX);

		Reset();

		//add ui layer.
		GenerateUICanvas();

		m_bInitialized = true;

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief release system
	*/
	void CGUISystem::Release()
	{
		GUI_ASSERT( m_bInitialized==true, "system has been released" );

		//destroy all canvas
		DestroyAllCanvas();

		//release all widgets
		DestroyAllWidgets();

		//release resource
		ReleaseAllResources();

		//release interface
		m_pInterfaceManager->UnregisterAllInterface();

		//destroy singleton instance
		ReleaseSingletons();

		//release performance monitor if necessary
		PERFMON_EXIT();
		
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
		m_nNameGenerateIdx = 0;
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();
	}
	//------------------------------------------------------------------------------
	void CGUISystem::GenerateUICanvas()
	{
		GUI_ASSERT( !m_pUICanvas, "ui canvas has been generated" );
		m_pUICanvas = new CGUIUICanvasLayer( "ui_layer"GUI_INTERNAL_WIDGET_FLAG );
		m_pUICanvas->Initialize();
		m_pCanvasLayerManager->PushCanvasLayer( m_pUICanvas );	
	}
	//------------------------------------------------------------------------------
	void CGUISystem::DestroyUICanvas()
	{
		if( m_pUICanvas )
		{
			m_pCanvasLayerManager->RemoveCanvasLayer( m_pUICanvas );
			m_pUICanvas->Finalize();
			m_pUICanvas->DestroySelf();
			m_pUICanvas = NULL;
		}
	}
	//------------------------------------------------------------------------------
	CGUIUICanvasLayer* CGUISystem::GetUICanvas()
	{
		return m_pUICanvas;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::SetDrawExtraInfo( bool bDraw )
	{
		m_bDrawExtraInfo = bDraw;
	}
	//------------------------------------------------------------------------------
	bool CGUISystem::IsDrawExtraInfo() const
	{
		return m_bDrawExtraInfo;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::SetPlayingAs( bool bPlaying )
	{
		m_bPlayingAs = bPlaying;
	}
	//------------------------------------------------------------------------------
	bool CGUISystem::IsPlayingAs() const
	{
		return m_bPlayingAs;
	}
	//------------------------------------------------------------------------------
	bool CGUISystem::IsEditorMode( ) const
	{
		return m_bIsEditorMode;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::SetEditorMode( bool bEditorMode )
	{
		m_bIsEditorMode = bEditorMode;
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
	* @brief release resource by scene name, 
	* only free named image now.
	*/
	void CGUISystem::ReleaseResourceByScene( const CGUIString& rSceneName )
	{
		for( TListResourceMgr::iterator itor = m_listResourceManager.begin(); 
			itor != m_listResourceManager.end();
			++itor)
		{
			(*itor)->ReleaseResourceByScene(rSceneName);
		}
	}
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
		CGUISceneManager::Instance()->ReleaseAllResources();

		for( TListResourceMgr::iterator itor = m_listResourceManager.begin(); 
			itor != m_listResourceManager.end();
			++itor)
		{
			(*itor)->ReleaseAllResources( );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISystem::DestroyAllCanvas()
	{
		m_pCanvasLayerManager->DestroyAllCanvasLayer();
		m_pUICanvas = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::DestroyAllWidgets(  )
	{
		m_aInputProcessor.Reset();
		UnregisterAllUIEvent();
		UngisterAllGlobalKey();

		//destroy all page
		m_pWidgetManager->ReleaseAllPages();

		IGUIInterfaceScript* pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		if( pScript )
		{
			pScript->DestroyAllScript();
		}

		//check global register
		GUI_ASSERT( m_vecGlobalKeyObj.empty(), "global key receiver is not cleared" );
		m_vecGlobalKeyObj.clear();
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
		PERFMON_BEGIN(0, "CGUISystem::Update");

		//update sound
		UpdateSound( fDeltaTime );

		//update physics
		UpdatePhysics( fDeltaTime );

		//update time and frame
		UpdateTime( fDeltaTime );

		//execute command if it exist
		ProcessCommand();

		UpdateCanvas( fDeltaTime );

		PERFMON_END( 0 );

		UpdatePerformance( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUISystem::UpdatePerformance( real fDeltaTime )
	{
#if GUI_PERFORMANCE_ON
		//update performance monitor
		CPerfMonitor::GetIt()->FrameUpdate( );
		if( CPerfMonitor::GetIt()->IsUpdated( ) )
		{
			GUI_TRACE("\n");
			GUI_TRACE(GUI_FORMAT("Performance Monitor: %fFPS \n",CPerfMonitor::GetIt()->GetFPS()));
			for( int32 i=0; i<CPerfMonitor::GetIt()->GetSectionNum( ); ++i)
			{
				if( CPerfMonitor::GetIt()->GetTimes(i) > 0 )
				{
					GUI_TRACE(GUI_FORMAT(
						"%d:%0.3f%%%% <call times>%d <use time>%f <name>%s\n",
						i, 
						CPerfMonitor::GetIt()->GetRate(i)*100,
						CPerfMonitor::GetIt()->GetTimes(i),
						CPerfMonitor::GetIt()->GetMillionsec(i) / 1000.0f,
						CPerfMonitor::GetIt()->GetName(i)));
				}
			}
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
	void CGUISystem::SetRawScreenSize( uint32 width, uint32 height)
	{
		SetRawScreenSize( CGUIIntSize( width, height ) );
	}
	//------------------------------------------------------------------------------
	void CGUISystem::SetRawScreenSize( const CGUIIntSize& rScreenSize )
	{
		if( rScreenSize != m_aRawScreenSize)
		{
			m_aRawScreenSize = rScreenSize;

			RefreshScreenSize( );
			
			//notify render
			IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
			if( pRender )
			{
				pRender->OnScreenSizeChange( m_aRawScreenSize );
			}
		}
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUISystem::GetRawScreenSize( ) const
	{
		return m_aRawScreenSize;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::SetScreenOrientation( EScreenOrientation eOrientation )
	{
		if( m_eScreenOrientation != eOrientation )
		{
			m_eScreenOrientation = eOrientation;

			RefreshScreenSize( );
		}
	}
	//------------------------------------------------------------------------------
	EScreenOrientation CGUISystem::GetScreenOrientation( ) const
	{
		return m_eScreenOrientation;
	}
	//------------------------------------------------------------------------------
	void CGUISystem::RefreshScreenSize()
	{
		switch( m_eScreenOrientation )
		{
		case eScreenOrientation_Portrait:
		case eScreenOrientation_PortraitUpsideDown:
			m_aScreenSize = m_aRawScreenSize;
			break;

		case eScreenOrientation_LandscapeLeft:
		case eScreenOrientation_LandscapeRight:
			m_aScreenSize.SetValue( m_aRawScreenSize.m_uHeight, m_aRawScreenSize.m_uWidth );
			break;
		}

		//update ui camera
		m_pCameraManager->SetCameraDefaultValue( m_aRawScreenSize, m_eScreenOrientation );

		//update ui page
		m_pCanvasLayerManager->Refresh();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the size of screen, in default
	*/
	const CGUIIntSize& CGUISystem::GetScreenSize( ) const
	{
		return m_aScreenSize;
	}
	//------------------------------------------------------------------------------
	uint32 CGUISystem::GetScreenWidth( ) const
	{
		return m_aScreenSize.GetWidth();
	}
	//------------------------------------------------------------------------------
	uint32 CGUISystem::GetScreenHeight( ) const
	{
		return m_aScreenSize.GetHeight();
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
		if( pWidget == m_pWgtFocus )
		{
			return;
		}
		if( m_pWgtFocus )
		{
			m_pWgtFocus->GetOnWidgetDestroyedSignal().disconnect( this );
		}
		m_pWgtFocus = pWidget;
		if( m_pWgtFocus )
		{
			m_pWgtFocus->GetOnWidgetDestroyedSignal().connect( this, &CGUISystem::OnWidgetDestroyed );
		}
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
	void CGUISystem::UpdateCanvas( real fDeltaTime )
	{
		m_pCanvasLayerManager->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUISystem::RenderCanvas( IGUIInterfaceRender* pRender )
	{
		m_pCanvasLayerManager->Render( pRender );
		if( IsDrawExtraInfo() )
		{
			//draw extra info
			m_pCanvasLayerManager->RenderExtraInfo( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISystem::BeginRender()
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		pRender->SetFontRender(pFont); 

		//init render
		pRender->BeginRender();
	}
	//------------------------------------------------------------------------------
	void CGUISystem::EndRender()
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
		pRender->EndRender();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief render system
	*/
	void CGUISystem::Render()
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();

		pRender->PushMatrix();
		pRender->MatrixMode(eMatrixMode_MODELVIEW);
		pRender->LoadIdentityMatrix();

		//render canvas
		RenderCanvas( pRender );

		//render cursor
		CGUIMouseCursor::Instance()->Render(pRender);

		pRender->PopMatrix();
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
		return m_pCanvasLayerManager->GetWidgetUnderPoint( rPos );
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
	void CGUISystem::UpdateSound( real fDeltaTime )
	{
		if( m_pInterfaceManager->GetInterfaceSound() )
		{
			m_pInterfaceManager->GetInterfaceSound()->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISystem::UpdatePhysics(real fDeltaTime)
	{
		if( IsEditorMode() == false )
		{
			if( m_pInterfaceManager->GetInterfacePhysics() )
			{
				m_pInterfaceManager->GetInterfacePhysics()->Update( fDeltaTime );
			}
		}
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
	void CGUISystem::RegisterGlobalKeyReceiver( CGUIWidget* pReceiver )
	{
		m_vecGlobalKeyObj.push_back(pReceiver);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief remove key event by root widget
	*/
	void CGUISystem::UngisterGlobalKeyReceiver( CGUIWidget* pReceiver )
	{
		for( TGlobalKeyObj::iterator itor = m_vecGlobalKeyObj.begin();
			itor != m_vecGlobalKeyObj.end();
			++itor)
		{
			if( *itor == pReceiver )
			{
				m_vecGlobalKeyObj.erase( itor );
				return;
			}
		}
		throw CGUIException("[CGUISystem::UngisterGlobalKeyByRoot]: failed to ungister global key!");
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
		for( TGlobalKeyObj::reverse_iterator itor = m_vecGlobalKeyObj.rbegin();
			itor != m_vecGlobalKeyObj.rend();
			++itor)
		{
			pEvent->SetReceiver(*itor);
			GSystem->SendEvent(pEvent);
			if( pEvent->IsConsumed())
			{
				break;
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
	void CGUISystem::OnWidgetDestroyed( CGUIWidget* pWidget )
	{
		if( pWidget == m_pWgtFocus )
		{
			m_pWgtFocus = NULL;
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex
