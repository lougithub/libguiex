/** 
* @file guiframework_android.cpp
* @brief framework to use this engine
* @author ken
* @date 2011-04-28
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiframework_android.h"

#include <libguiex_widget/guiwgt.h>
#include <libguiex_widget_box2d/guiwgt_box2d.h>
#include <libguiex_widget_game/guiwgt_game.h>

//libguiex module
#include <libguiex_module/font_dummy/guifont_dummy.h>
#include <libguiex_module/imageloader_tga/guiimageloader_tga.h>
#include <libguiex_module/filesys_stdio/guifilesys_stdio.h>
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_module/render_opengles_android/guirender_opengles_android.h>
#include <libguiex_module/stringconv_android/guistringconv_android.h>
#include <libguiex_module/script_lua/guiscript_lua.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <libguiex_module/sound_dummy/guisound_dummy.h>
#include <libguiex_module/ime_dummy/guiime_dummy.h>
#include <libguiex_module/mouse_default/guimouse_default.h>
#include <libguiex_module/localizationloader_tinyxml/guilocalizationloader_tinyxml.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFramework_Android::CGUIFramework_Android( )
		:CGUIFrameworkBase( )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFramework_Android::~CGUIFramework_Android()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIFramework_Android::InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath )
	{
		//init system
		if( GSystem != NULL )
		{
			CGUIException::ThrowException( "[CGUIFramework_Android::InitializeSystem] GSystem has been created!" );
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
	void CGUIFramework_Android::SetupLogSystem( )
	{
		GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_STDERR);
		GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::ReleaseSystem()
	{
		//release system
		if( GSystem == NULL )
		{
			CGUIException::ThrowException( "[CGUIFramework_Android::ReleaseSystem] GSystem has been released!" );
			return;
		}
		GSystem->Release();
		delete GSystem;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengles_android);
		GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_tga);
		GUI_REGISTER_INTERFACE_LIB( IGUIStringConv_android);
		GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_stdio);
		GUI_REGISTER_INTERFACE_LIB( IGUIFont_dummy);
		GUI_REGISTER_INTERFACE_LIB( IGUIIme_dummy);
		GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
		GUI_REGISTER_INTERFACE_LIB( IGUIScript_lua);
		GUI_REGISTER_INTERFACE_LIB( IGUIPhysics_box2d );
		GUI_REGISTER_INTERFACE_LIB( IGUISound_dummy ); 
		GUI_REGISTER_INTERFACE_LIB( IGUIMouse_default); 
		GUI_REGISTER_INTERFACE_LIB( IGUILocalizationLoader_tinyxml); 
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterWidgetGenerators( )
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
	int32 CGUIFramework_Android::InitializeGame( )
	{
		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::ReleaseGame()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Android::PreUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Android::PostUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Android::OutputFatalError( const char* pErrorDesc )
	{
		printf( "fatal error: %s\n", pErrorDesc );
	}
	//------------------------------------------------------------------------------


}//namespace guiex

