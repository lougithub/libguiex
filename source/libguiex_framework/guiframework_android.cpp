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
#include <libguiex_module/keyboard_default/guikeyboard_default.h>
#include <libguiex_module/font_ft2/guifont_ft2.h>
#include <libguiex_module/font_dummy/guifont_dummy.h>
#include <libguiex_module/filesys_android/guifilesys_android.h>
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_module/stringconv_internal/guistringconv_internal.h>
#include <libguiex_module/script_lua/guiscript_lua.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <libguiex_module/sound_dummy/guisound_dummy.h>
#include <libguiex_module/sound_android/guisound_android.h>
#include <libguiex_module/ime_dummy/guiime_dummy.h>
#include <libguiex_module/mouse_default/guimouse_default.h>
#include <libguiex_module/localizationloader_tinyxml/guilocalizationloader_tinyxml.h>

#if defined( GUIEX_RENDER_OPENGL )
#	include <libguiex_module/render_opengl/guirender_opengl.h>
#elif defined(GUIEX_RENDER_OPENGL_ES1 )
#	include <libguiex_module/render_opengl_es1/guirender_opengl_es1.h>
#elif defined(GUIEX_RENDER_OPENGL_ES2 )
#	include <libguiex_module/render_opengl_es2/guirender_opengl_es2.h>
#else
#error "unknown render type"	
#endif

#include <android/log.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	class CAndroidLogMsgCallback : public CGUILogMsgCallback
	{
		public:
			virtual void Log( const CGUILogMsgRecord& rRecord )
			{
				__android_log_print( ANDROID_LOG_INFO, "guiex", rRecord.GetMsgData());
			}

			static CAndroidLogMsgCallback g_MsgCallback;
	};
	CAndroidLogMsgCallback CAndroidLogMsgCallback::g_MsgCallback;
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIString CGUIFramework_Android::ms_strApkPath;
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
	void CGUIFramework_Android::SetApkPath( const char* szApkPath )
	{
		ms_strApkPath = szApkPath;
	}
	//------------------------------------------------------------------------------
	int32 CGUIFramework_Android::InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath )
	{
		//setup log system
		SetupLogSystem();

		//init system
		if( GSystem != NULL )
		{
			GUI_THROW( "[CGUIFramework_Android::InitializeSystem] GSystem has been created!" );
			return -1;
		}
		GSystem = new CGUISystem;

		if( 0 != GSystem->Initialize())
		{
			return -1;
		}

		//set screen size
		GSystem->SetRawScreenSize( rScreenSize );

		//set date path
		GSystem->SetDataPath( pDataPath );
		GSystem->SetApkPath( ms_strApkPath );

		//register interface
		RegisterInterfaces();

		//register widget
		RegisterWidgetGenerators();

		return 0;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::SetupLogSystem( )
	{
		GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_STDERR | CGUILogMsg::FLAG_MSG_CALLBACK);
		GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
		GUI_LOG->SetCallbackMsg( &CAndroidLogMsgCallback::g_MsgCallback );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::ReleaseSystem()
	{
		//release system
		if( GSystem == NULL )
		{
			GUI_THROW( "[CGUIFramework_Android::ReleaseSystem] GSystem has been released!" );
			return;
		}
		GSystem->Release();
		delete GSystem;
		GSystem = NULL;
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Render( )
	{
#if defined( GUIEX_RENDER_OPENGL )
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl);
#elif defined(GUIEX_RENDER_OPENGL_ES1 )
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl_es1);
#elif defined(GUIEX_RENDER_OPENGL_ES2 )
		GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl_es2);
#else
#	error "unknown render type"	
#endif
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_ImageLoader( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_png);
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_FileSys( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_android );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Mouse( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIMouse_default); 
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Keyboard( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIKeyboard_default);
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Font( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIFont_ft2);
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_ConfigFile( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_StringConv( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIStringConv_internal);
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Script( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIScript_lua );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Physics( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIPhysics_box2d );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Sound( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUISound_android );
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_Ime( )
	{
		GUI_REGISTER_INTERFACE_LIB( IGUIIme_dummy);
	}
	//------------------------------------------------------------------------------ 
	void CGUIFramework_Android::RegisterInterfaces_LocalizationLoader( )
	{
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

