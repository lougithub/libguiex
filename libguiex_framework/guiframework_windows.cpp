/** 
 * @file guiframework_windows.h
 * @brief framework to use this engine
 * @author ken
 * @date 2010-12-10
 */

//============================================================================//
// include 
//============================================================================// 
#include "guiframework_windows.h"

#include <libguiex_widget/guiwgt.h>

//libguiex module
#include <libguiex_module/render_opengl/guirender_opengl.h>
#include <libguiex_module/imageloader_tga/guiimageloader_tga.h>
#include <libguiex_module/keyboard_winapi/guikeyboard_winapi.h>
#include <libguiex_module/mouse_winapi/guimouse_winapi.h>
#include <libguiex_module/font_ft2/guifont_ft2.h>
#include <libguiex_module/filesys_stdio/guifilesys_stdio.h>
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_module/script_lua/guiscript_lua.h>
#include <libguiex_module/ime_winapi/guiime_winapi.h>
#include <libguiex_module/stringconv_winapi/guistringconv_winapi.h>

#include <windows.h>
#include <fstream>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFramework_Windows::CGUIFramework_Windows( const CGUISize& rScreenSize, const CGUIString& rDataPath )
		:CGUIFrameworkBase( rScreenSize, rDataPath )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFramework_Windows::~CGUIFramework_Windows()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIFramework_Windows::InitializeSystem( )
	{
		//init system
		if( GSystem != NULL )
		{
			throw CGUIException( "[CGUIFramework_Windows::InitializeSystem] GSystem has been created!" );
			return -1;
		}
		GSystem = new CGUISystem;
		if( 0 != GSystem->Initialize())
		{
			return -1;
		}

		//log
		GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_TIMESTAMP_LITE | CGUILogMsg::FLAG_OSTREAM | CGUILogMsg::FLAG_STDERR);
		GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
		GUI_LOG->SetOstream( new std::ofstream( "libguiex_framework_windows.log", std::ios_base::out | std::ios_base::trunc ), true );
		GSystem->SetScreenSize( GetScreenSize() );

		//register interface
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl);
		GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_tga);
		GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_stdio);
		GUI_REGISTER_INTERFACE_LIB( IGUIMouse_winapi);
		GUI_REGISTER_INTERFACE_LIB( IGUIFont_ft2);
		GUI_REGISTER_INTERFACE_LIB( IGUIKeyboard_winapi);
		GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
		GUI_REGISTER_INTERFACE_LIB( IGUIStringConv_Winapi);
		GUI_REGISTER_INTERFACE_LIB( IGUIScript_lua );
		//GUI_REGISTER_INTERFACE_LIB_ARG( IGUIIme_winapi,  );

		//register widget
		CGUIWidgetGenerator** pGenerator = GetAllGenerators();
		while(*pGenerator)
		{
			CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
			pGenerator ++;
		}

		//set date path
		GSystem->SetDataPath( GetDataPath() );

		////load scene info
		//CGUISceneInfoManager::Instance()->LoadScenes( "/", ".uip" );

		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Windows::ReleaseSystem()
	{
		//release system
		if( GSystem == NULL )
		{
			throw CGUIException( "[CGUIFramework_Windows::ReleaseSystem] GSystem has been released!" );
			return;
		}
		GSystem->Release();
		delete GSystem;
	}
	//------------------------------------------------------------------------------ 
	int32 CGUIFramework_Windows::InitializeGame( )
	{
		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Windows::ReleaseGame()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Windows::PreUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Windows::PostUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Windows::OutputFatalError( const char* pErrorDesc )
	{
		::MessageBoxA( NULL, pErrorDesc, "fatal error", MB_OK );
	}
	//------------------------------------------------------------------------------


}//namespace guiex

