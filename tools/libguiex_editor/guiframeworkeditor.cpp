/** 
* @file guiframeworkeditor.cpp
* @author ken
* @date 2010-12-21
*/


//============================================================================//
// include
//============================================================================//
#include "guiframeworkeditor.h"
#include <libguiex_module\render_opengl\guirender_opengl.h>
#include "wxmainapp.h"
#include "wxmainframe.h"
#include "toolsmisc.h"
#include <fstream>

class CEditorLogMsgCallback : public CGUILogMsgCallback
{
public:
	virtual void Log( const CGUILogMsgRecord& rRecord )
	{
		GetMainFrame()->OutputString( rRecord.GetMsgData() );
	}

	static CEditorLogMsgCallback g_MsgCallback;
};
CEditorLogMsgCallback CEditorLogMsgCallback::g_MsgCallback;

//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
CGUIFrameworkEditor* CGUIFrameworkEditor::ms_pFramework = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkEditor::CGUIFrameworkEditor( )
:CGUIFrameworkEditorBase( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::RegisterInterfaces_Render( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::EditorSetupLogSystem( )
{
	GUI_LOG->SetCallbackMsg( &CEditorLogMsgCallback::g_MsgCallback );
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::SetupLogSystem( )
{
	GUI_LOG->Open( "gui_framework_log", CGUILogMsg::FLAG_TIMESTAMP_LITE | CGUILogMsg::FLAG_OSTREAM | CGUILogMsg::FLAG_MSG_CALLBACK );
	GUI_LOG->SetPriorityMask( GUI_LM_DEBUG | GUI_LM_TRACE | GUI_LM_WARNING|GUI_LM_ERROR );
	GUI_LOG->SetOstream( new std::ofstream( "libguiex_editor.log", std::ios_base::out | std::ios_base::trunc ), true );
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::RegisterOpenglInterface()
{
	CGUIFrameworkEditorBase::RegisterInterfaces_Render();
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::UnregisterOpenglInterface()
{
	IGUIInterface * pInterface = CGUIInterfaceManager::Instance()->GetInterfaceRender();
	if( pInterface )
	{
		CGUIInterfaceManager::Instance()->UnregisterInterface( pInterface );
	}
}
//------------------------------------------------------------------------------
