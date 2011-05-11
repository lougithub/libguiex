/** 
 * @file guiframework_mac.h
 * @brief framework to use this engine
 * @author ken
 * @date 2010-12-10
 */

//============================================================================//
// include 
//============================================================================// 
#include "guiframework_mac.h"

#include <libguiex_widget/guiwgt.h>
#include <libguiex_widget_box2d/guiwgt_box2d.h>
#include <libguiex_widget_game/guiwgt_game.h>

//libguiex module
#include <libguiex_module/font_ft2/guifont_ft2.h>
#include <libguiex_module/imageloader_tga/guiimageloader_tga.h>
#include <libguiex_module/filesys_cocoa/guifilesys_cocoa.h>
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_module/render_opengl/guirender_opengl.h>
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
	CGUIFramework_Mac::CGUIFramework_Mac( )
		:CGUIFrameworkBase( )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFramework_Mac::~CGUIFramework_Mac()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIFramework_Mac::InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath )
	{
		//setup log system
		SetupLogSystem();

		//init system
		if( GSystem != NULL )
		{
			CGUIException::ThrowException( "[CGUIFramework_Mac::InitializeSystem] GSystem has been created!" );
			return -1;
		}
		GSystem = new CGUISystem;
		if( 0 != GSystem->Initialize())
		{
			return -1;
		}

		GSystem->SetRawScreenSize( rScreenSize );

		//set date path
		GSystem->SetDataPath( pDataPath );

		//register interface
		RegisterInterfaces();

		//register widget
		RegisterWidgetGenerators();


		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Mac::SetupLogSystem( )
	{
		GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_STDERR);
		GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Mac::ReleaseSystem()
	{
		//release system
		if( GSystem == NULL )
		{
			CGUIException::ThrowException( "[CGUIFramework_Mac::ReleaseSystem] GSystem has been released!" );
			return;
		}
		GSystem->Release();
		delete GSystem;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Mac::RegisterInterfaces( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl);
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
	void CGUIFramework_Mac::RegisterWidgetGenerators( )
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
	int32 CGUIFramework_Mac::InitializeGame( )
	{
		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Mac::ReleaseGame()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Mac::PreUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Mac::PostUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIFramework_Mac::OutputFatalError( const char* pErrorDesc )
	{
		printf( "fatal error: %s\n", pErrorDesc );
	}
	//------------------------------------------------------------------------------


}//namespace guiex

