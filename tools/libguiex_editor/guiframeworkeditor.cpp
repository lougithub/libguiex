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
#include <libguiex_module\imageloader_png\guiimageloader_png.h>

#include "wxeditorcanvas.h"
#include "wxmainapp.h"
#include "wxmainframe.h"
#include "editorutility.h"

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
// interface image loader
//============================================================================//
namespace guiex
{
	class GUIEXPORT IGUIImageLoader_Editor : public IGUIInterfaceImageLoader
	{
	public:
		IGUIImageLoader_Editor();
		virtual ~IGUIImageLoader_Editor(){}

		virtual CGUIImageData* LoadFromFile( const CGUIString& rFileName  );
		virtual CGUIImageData* LoadFromMemory( uint8* pFileData, size_t nSize ){return NULL;}
		virtual void DestroyImageData(CGUIImageData* pImageData){delete pImageData;}
		virtual void DeleteSelf(){delete this;}

	protected:
		virtual int DoInitialize(void* ) {return 0;}
		virtual void DoDestroy(){}

	public: 
		static const char* StaticGetModuleName();
	};

	//------------------------------------------------------------------------------
	const char* IGUIImageLoader_Editor::StaticGetModuleName()
	{
		return "IGUIImageLoader_Editor";
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_Editor::IGUIImageLoader_Editor()
		:IGUIInterfaceImageLoader( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_Editor::LoadFromFile( const CGUIString& rFileName  )
	{
		wxString rImagePath = Gui2wxString( GSystem->GetDataPath() + rFileName );
		wxFileName filename( rImagePath );
		if ( !filename.FileExists() )
		{
			GUI_THROW( "[IGUIImageLoader_Editor::LoadFromFile] - Failed to load image!");
			return NULL;
		}

		wxImage* pWxImage = NULL;
		if( filename.GetExt().CmpNoCase(L"tga") == 0)
		{
			pWxImage = new wxImage( filename.GetFullPath(), wxBITMAP_TYPE_TGA );
		}
		else if( filename.GetExt().CmpNoCase(L"png") == 0)
		{
			pWxImage = new wxImage( filename.GetFullPath(), wxBITMAP_TYPE_PNG );
		}

		if ( !pWxImage || !pWxImage->Ok() )
		{
			GUI_THROW( "[IGUIImageLoader_Editor::LoadFromFile] - Failed to load image!");
			if( pWxImage )
			{
				delete pWxImage;
			}
			return NULL;
		}

		CGUIImageData* pData = new CGUIImageData(this);
		EGuiPixelFormat	eImageFormat;	
		if( pWxImage->HasAlpha() )
		{
			eImageFormat = GUI_PF_RGBA_32;
		}
		else
		{
			eImageFormat = GUI_PF_RGB_24;
		}

		uint8* tmpBuff = pData->SetImageData( pWxImage->GetWidth(), pWxImage->GetHeight(), eImageFormat);
		uint8* pRGB = pWxImage->GetData();
		uint8* pAlpha = pWxImage->GetAlpha();
		uint32 nPixelCount = pWxImage->GetWidth() * pWxImage->GetHeight();
		for(uint32 i = 0; i < nPixelCount; ++i )
		{
			memcpy( tmpBuff, pRGB, 3 );
			tmpBuff += 3;
			pRGB += 3;
			if( eImageFormat == GUI_PF_RGBA_32 )
			{
				*tmpBuff = *pAlpha;
				++tmpBuff;
				++pAlpha;
			}
		}

		return pData;
	}
	//------------------------------------------------------------------------------
}

//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
CGUIFrameworkEditor* CGUIFrameworkEditor::ms_pFramework = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkEditor::CGUIFrameworkEditor( )
:CGUIFramework( )
,m_pCurrentCanvas( NULL )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::RegisterInterfaces_Render( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::RegisterInterfaces_ImageLoader( )
{
	GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_Editor);
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
	GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl );
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::UnregisterOpenglInterface()
{
	GUI_UNREGISTER_INTERFACE_LIB( IGUIRender_opengl);
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::SetCurrentCanvas( WxEditorCanvas* pCanvas )
{
	m_pCurrentCanvas = pCanvas;
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::PostRender( )
{
	if( m_pCurrentCanvas )
	{
		m_pCurrentCanvas->RenderEditorInfo();
	}

	CGUIFramework::PostRender();
}
//------------------------------------------------------------------------------
