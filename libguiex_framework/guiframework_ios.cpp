/** 
 * @file guiframework_ios.h
 * @brief framework to use this engine
 * @author ken
 * @date 2011-04-08
 */

//============================================================================//
// include 
//============================================================================// 
#include "guiframework_ios.h"

#include <libguiex_widget/guiwgt.h>
#include <libguiex_widget_box2d/guiwgt_box2d.h>
#include <libguiex_widget_game/guiwgt_game.h>

//libguiex module
#include <libguiex_module/font_ft2/guifont_ft2.h>
#include <libguiex_module/imageloader_tga/guiimageloader_tga.h>
#include <libguiex_module/filesys_cocoa/guifilesys_cocoa.h>
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_module/render_opengles/guirender_opengles.h>
#include <libguiex_module/stringconv_cocoa/guistringconv_cocoa.h>
#include <libguiex_module/stringconv_iconv/guistringconv_iconv.h>
#include <libguiex_module/script_lua/guiscript_lua.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <libguiex_module/sound_dummy/guisound_dummy.h>
#include <libguiex_module/ime_dummy/guiime_dummy.h>
#include <libguiex_module/sound_openal/guisound_openal.h>
#include <libguiex_module/mouse_default/guimouse_default.h>
#include <libguiex_module/localizationloader_tinyxml/guilocalizationloader_tinyxml.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFramework_IOS::CGUIFramework_IOS( )
		:CGUIFrameworkBase( )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFramework_IOS::~CGUIFramework_IOS()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIFramework_IOS::InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath )
	{
		//init system
		if( GSystem != NULL )
		{
			throw CGUIException( "[CGUIFramework_IOS::InitializeSystem] GSystem has been created!" );
			return -1;
		}
		GSystem = new CGUISystem;
		if( 0 != GSystem->Initialize())
		{
			return -1;
		}

		//setup log system
		SetupLogSystem();

		GSystem->SetRawScreenSize( rScreenSize );

		//register interface
		RegisterInterfaces();

		//register widget
		RegisterWidgetGenerators();

		//set date path
		GSystem->SetDataPath( pDataPath );

		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_IOS::SetupLogSystem( )
	{
		GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_STDERR);
		GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_IOS::ReleaseSystem()
	{
		//release system
		if( GSystem == NULL )
		{
			throw CGUIException( "[CGUIFramework_IOS::ReleaseSystem] GSystem has been released!" );
			return;
		}
		GSystem->Release();
		delete GSystem;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_IOS::RegisterInterfaces( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengles);
		GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_tga);
		GUI_REGISTER_INTERFACE_LIB( IGUIStringConv_cocoa);
		GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_cocoa);
		GUI_REGISTER_INTERFACE_LIB( IGUIFont_ft2);
		GUI_REGISTER_INTERFACE_LIB( IGUIIme_dummy);
		GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
		GUI_REGISTER_INTERFACE_LIB( IGUIScript_lua);
		GUI_REGISTER_INTERFACE_LIB( IGUIPhysics_box2d );
		GUI_REGISTER_INTERFACE_LIB( IGUISound_openal ); 
		GUI_REGISTER_INTERFACE_LIB( IGUIMouse_default); 
		GUI_REGISTER_INTERFACE_LIB( IGUILocalizationLoader_tinyxml); 
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_IOS::RegisterWidgetGenerators( )
	{
		//basic widgets
		{
			CGUIWidgetGenerator** pGenerator = GetAllWidgetGenerators();
			while(*pGenerator)
			{
				CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
				pGenerator ++;
			}
		}
		
		
		//box2d widgets
		{
			CGUIWidgetGenerator** pGenerator = GetAllWidgetGenerators_Box2d();
			while(*pGenerator)
			{
				CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
				pGenerator ++;
			}
		}

		//game widgets
		{
			CGUIWidgetGenerator** pGenerator = GetAllWidgetGenerators_Game();
			while(*pGenerator)
			{
				CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
				pGenerator ++;
			}
		}
	}
	//------------------------------------------------------------------------------ 
	int32 CGUIFramework_IOS::InitializeGame( )
	{
		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_IOS::ReleaseGame()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_IOS::PreUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_IOS::PostUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_IOS::OutputFatalError( const char* pErrorDesc )
	{
		printf( "fatal error: %s\n", pErrorDesc );
	}
	//------------------------------------------------------------------------------


}//namespace guiex

