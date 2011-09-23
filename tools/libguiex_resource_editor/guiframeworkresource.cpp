/** 
* @file guiframeworkresource.h
* @author ken
* @date 2011-09-08
*/

//============================================================================//
// include
//============================================================================//
#include "guiframeworkresource.h"
#include <libguiex_module\render_opengl\guirender_opengl.h>
#include "wxmainframe.h"
#include <fstream>

class CResourceLogMsgCallback : public CGUILogMsgCallback
{
public:
	virtual void Log( const CGUILogMsgRecord& rRecord )
	{
		//GetMainFrame()->OutputString( rRecord.GetMsgData() );
	}

	static CResourceLogMsgCallback g_MsgCallback;
};
CResourceLogMsgCallback CResourceLogMsgCallback::g_MsgCallback;

//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
CGUIFrameworkResource* CGUIFrameworkResource::ms_pFramework = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkResource::CGUIFrameworkResource( )
:CGUIFrameworkEditorBase( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkResource::RegisterInterfaces_Render( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkResource::RegisterWidgetGenerators( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkResource::EditorSetupLogSystem( )
{
	GUI_LOG->SetCallbackMsg( &CResourceLogMsgCallback::g_MsgCallback );
}
//------------------------------------------------------------------------------
void CGUIFrameworkResource::SetupLogSystem( )
{
	GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_TIMESTAMP_LITE | CGUILogMsg::FLAG_OSTREAM | CGUILogMsg::FLAG_MSG_CALLBACK );
	GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
	GUI_LOG->SetOstream( new std::ofstream( "libguiex_resource.log", std::ios_base::out | std::ios_base::trunc ), true );
}
//------------------------------------------------------------------------------
void CGUIFrameworkResource::RegisterOpenglInterface()
{
	CGUIFrameworkEditorBase::RegisterInterfaces_Render();
}
//------------------------------------------------------------------------------
void CGUIFrameworkResource::UnregisterOpenglInterface()
{
	IGUIInterface * pInterface = CGUIInterfaceManager::Instance()->GetInterfaceRender();
	if( pInterface )
	{
		CGUIInterfaceManager::Instance()->UnregisterInterface( pInterface );
	}
}
//------------------------------------------------------------------------------
