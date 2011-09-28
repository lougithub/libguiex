/** 
* @file wxresourcepreviewpanel.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
#include "libguiex_core/guiex.h"

using namespace guiex;

//============================================================================//
// class
//============================================================================// 

//============================================================================//
// WxResourceCanvasBase
//============================================================================// 
class WxResourceCanvasBase : public wxGLCanvas
{
public:
	WxResourceCanvasBase( wxWindow *parent );
	~WxResourceCanvasBase();

public:
	//event
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

protected:
	virtual void OnUpdate( float fDeltaTime ) = 0;
	virtual void OnRender( guiex::IGUIInterfaceRender* pRender ) = 0;

protected:
	static int wx_gl_attribs[];
	wxTimer m_timer;
	wxGLContext* m_pGLContext;

protected:
	DECLARE_EVENT_TABLE()
};
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxResourceCanvasBase, wxGLCanvas)
	EVT_PAINT(WxResourceCanvasBase::OnPaint)
	EVT_TIMER(TIMERID_PREVIEWCANVAS_RENDER, WxResourceCanvasBase::OnTimer)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
int WxResourceCanvasBase::wx_gl_attribs[] = {
	WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 
	WX_GL_DEPTH_SIZE, 24, 
	WX_GL_STENCIL_SIZE, 8,
	0};
//------------------------------------------------------------------------------
WxResourceCanvasBase::WxResourceCanvasBase(wxWindow *parent )
	: wxGLCanvas(parent, wxID_ANY,wx_gl_attribs, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE )
	,m_timer(this, TIMERID_PREVIEWCANVAS_RENDER)
	,m_pGLContext(NULL)
{
	m_timer.Start(33);
	m_pGLContext = new wxGLContext( this );
	m_pGLContext->SetCurrent(*this);
}
//------------------------------------------------------------------------------
WxResourceCanvasBase::~WxResourceCanvasBase()
{
	m_timer.Stop();
	delete m_pGLContext;
	m_pGLContext = NULL;
}
//------------------------------------------------------------------------------
void WxResourceCanvasBase::OnPaint(wxPaintEvent& event)
{
	m_pGLContext->SetCurrent(*this);

	const wxSize ClientSize = GetClientSize();
	GSystem->SetRawScreenSize(ClientSize.x,ClientSize.y);

	IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();

	GSystem->BeginRender();

	pRender->PushMatrix();
	pRender->LoadIdentity();
	CGUIMatrix4 matrix;
	matrix.makeTransform( CGUIVector3(ClientSize.x/2, ClientSize.y/2, 0), CGUIVector3(1.0f,1.0f,1.0f), CGUIQuaternion());
	pRender->MultMatrix(matrix);

	OnRender( pRender );

	pRender->PopMatrix();

	GSystem->EndRender();

	glFlush();
	SwapBuffers();
}
//------------------------------------------------------------------------------
void WxResourceCanvasBase::OnTimer(wxTimerEvent& event)
{
	OnUpdate( event.GetInterval() / 1000.f );
	Refresh();
}
//------------------------------------------------------------------------------


//============================================================================//
// WxAnimationCanvas
//============================================================================// 
class WxAnimationCanvas : public WxResourceCanvasBase
{
public:
	WxAnimationCanvas( wxWindow *parent );
	~WxAnimationCanvas();

	void SetAnimationName( const guiex::CGUIString& rImageName );

protected:
	virtual void OnUpdate( float fDeltaTime );
	virtual void OnRender( guiex::IGUIInterfaceRender* pRender );

	void ClearAnimation();

protected:
	guiex::CGUIString m_strAnimationName;
	guiex::CGUIAnimation* m_pGUIAnimation;
};

//------------------------------------------------------------------------------
WxAnimationCanvas::WxAnimationCanvas( wxWindow *parent )
	:WxResourceCanvasBase(parent)
	,m_pGUIAnimation(NULL)
{

}
//------------------------------------------------------------------------------
WxAnimationCanvas::~WxAnimationCanvas()
{
	ClearAnimation();
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::OnUpdate( float fDeltaTime )
{
	if( m_pGUIAnimation )
	{
		m_pGUIAnimation->Update( fDeltaTime );
	}
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::OnRender( guiex::IGUIInterfaceRender* pRender )
{
	if( m_pGUIAnimation )
	{
		const CGUISize& rSize = m_pGUIAnimation->GetSize();

		CGUIRect aRenderRect( 
			-rSize.GetWidth()/2,
			-rSize.GetHeight()/2,
			rSize.GetWidth()/2,
			rSize.GetHeight()/2
			);
		m_pGUIAnimation->Draw( pRender, aRenderRect, 0, 1 );
	}
}
//------------------------------------------------------------------------------
void WxAnimationCanvas::ClearAnimation()
{
	if( m_pGUIAnimation )
	{
		m_pGUIAnimation->RefRelease();
		m_pGUIAnimation = NULL;
	}
}		
//------------------------------------------------------------------------------
void WxAnimationCanvas::SetAnimationName( const guiex::CGUIString& rImageName )
{
	if( m_strAnimationName != rImageName )
	{
		ClearAnimation();
		m_strAnimationName = rImageName;
		if( !m_strAnimationName.empty() )
		{
			m_pGUIAnimation = CGUIAnimationManager::Instance()->AllocateResource( m_strAnimationName );
			if( m_pGUIAnimation )
			{
				m_pGUIAnimation->SetLooping( true );
			}
		}
	}
}
//------------------------------------------------------------------------------


//============================================================================//
// WxImageCanvas
//============================================================================// 
class WxImageCanvas : public WxResourceCanvasBase
{
public:
	WxImageCanvas( wxWindow *parent );
	~WxImageCanvas();

	void SetImageName( const guiex::CGUIString& rImageName );
	void SetUVRect( const wxRect& rTargetRect );

protected:
	virtual void OnUpdate( float fDeltaTime );
	virtual void OnRender( guiex::IGUIInterfaceRender* pRender );

	void ClearImage();

protected:
	guiex::CGUIString m_strImageName;
	guiex::CGUIImage* m_pGUIImage;
	wxRect m_aUVRect;
};
//------------------------------------------------------------------------------
WxImageCanvas::WxImageCanvas( wxWindow *parent )
	:WxResourceCanvasBase(parent)
	,m_pGUIImage(NULL)
{

}
//------------------------------------------------------------------------------
WxImageCanvas::~WxImageCanvas()
{
	ClearImage();
}
//------------------------------------------------------------------------------
void WxImageCanvas::OnUpdate( float fDeltaTime )
{

}
//------------------------------------------------------------------------------
void WxImageCanvas::OnRender( guiex::IGUIInterfaceRender* pRender )
{
	if( m_pGUIImage )
	{
		const CGUISize& rSize = m_pGUIImage->GetSize();

		CGUIRect aRenderRect( 
			-rSize.GetWidth()/2,
			-rSize.GetHeight()/2,
			rSize.GetWidth()/2,
			rSize.GetHeight()/2
			);
		m_pGUIImage->Draw( pRender, aRenderRect, 0, 1 );
	}
}
//------------------------------------------------------------------------------
void WxImageCanvas::ClearImage()
{
	if( m_pGUIImage )
	{
		m_pGUIImage->RefRelease();
		m_pGUIImage = NULL;
	}
}		
//------------------------------------------------------------------------------
void WxImageCanvas::SetImageName( const guiex::CGUIString& rImageName )
{
	if( m_strImageName != rImageName )
	{
		ClearImage();
		m_strImageName = rImageName;
		if( !m_strImageName.empty() )
		{
			m_pGUIImage = CGUIImageManager::Instance()->AllocateResource( m_strImageName );
		}
	}
}
//------------------------------------------------------------------------------
void WxImageCanvas::SetUVRect( const wxRect& rTargetRect )
{
	m_aUVRect = rTargetRect;
}
//------------------------------------------------------------------------------






//============================================================================//
// function
//============================================================================// 

//============================================================================//
// WxResourcePreviewPanelBase
//============================================================================// 
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase::WxResourcePreviewPanelBase( wxWindow* parent, const guiex::CGUIString& rResourceType )
	:wxScrolledWindow( parent )
	,m_strResourceType( rResourceType )
{
}
//------------------------------------------------------------------------------
const CGUIString& WxResourcePreviewPanelBase::GetResourceType( ) const
{
	return m_strResourceType;
}
//------------------------------------------------------------------------------

//============================================================================//
// WxAnimationPreviewPanel
//============================================================================//
//------------------------------------------------------------------------------
WxAnimationPreviewPanel::WxAnimationPreviewPanel( wxWindow *parent )
	: WxResourcePreviewPanelBase( parent, "CGUIAnimationDefine" )
	, m_pAnimationCanvas( NULL )
{
	m_pAnimationCanvas = new WxAnimationCanvas( this ); 

	wxSizer *sizerAnimation = new wxBoxSizer( wxVERTICAL );
	sizerAnimation->Add( m_pAnimationCanvas, 1, wxALL|wxEXPAND );
	SetSizer( sizerAnimation );

	Show(true);
}
//------------------------------------------------------------------------------
void WxAnimationPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	if( m_strResourceName != rResourceName )
	{
		m_strResourceName = rResourceName;

		m_pAnimationCanvas->SetAnimationName( wx2GuiString( m_strResourceName) );

		m_pAnimationCanvas->Refresh();
	}
}
//------------------------------------------------------------------------------




//============================================================================//
// WxFontPreviewPanel
//============================================================================// 
//------------------------------------------------------------------------------
WxFontPreviewPanel::WxFontPreviewPanel( wxWindow* parent )
	:WxResourcePreviewPanelBase( parent, "CGUIFontDefine" )
{
	m_pTextBox = new wxTextCtrl( this, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pTextBox, 0, wxALIGN_CENTER );
	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxFontPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	if( m_strResourceName != rResourceName )
	{
		m_strResourceName = rResourceName;
		wxString strFontDesc = CGUIResourcePool::Instance()->GetFontDesc(  m_strResourceName );
		m_pTextBox->SetLabelText( strFontDesc );
	}
}
//------------------------------------------------------------------------------




//============================================================================//
// WxImagePreviewPanel
//============================================================================// 
//------------------------------------------------------------------------------
WxImagePreviewPanel::WxImagePreviewPanel( wxWindow* parent )
	:WxResourcePreviewPanelBase( parent, "CGUIImageDefine" )
{
	m_pImageCanvas = new WxImageCanvas( this ); 
	m_pFullImageCanvas = new WxImageCanvas( this ); 

	wxSizer *sizerImages = new wxBoxSizer( wxVERTICAL );
	sizerImages->Add( m_pImageCanvas, 1, wxALL|wxEXPAND );
	sizerImages->Add( m_pFullImageCanvas, 1, wxALL|wxEXPAND );
	SetSizer( sizerImages );

	Show(true);
}
//------------------------------------------------------------------------------
void WxImagePreviewPanel::SetResourceName( const wxString& rResourceName ) 
{
	if( m_strResourceName == rResourceName )
	{
		return;
	}
	m_strResourceName = rResourceName;

	m_pImageCanvas->SetImageName( wx2GuiString( m_strResourceName) );
	m_pFullImageCanvas->SetImageName( wx2GuiString( m_strResourceName) );


	////thumbnail
	//wxBitmap* pBitmap = CGUIResourcePool::Instance()->GenerateOriginalImageThumbnail(  m_strResourceName );
	//m_pImageCanvas->SetBitmap( pBitmap );

	////full image
	//m_pFullImageCanvas->SetBitmap( NULL );
	//if( !m_strResourceName.empty() )
	//{
	//	CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource( wx2GuiString( m_strResourceName ) );
	//	if( pImage && pImage->GetImageType() == eImageType_FromFile )
	//	{
	//		wxImage* pWxImage = LoadwxImageByGuiImage( pImage );
	//		if( pWxImage )
	//		{
	//			wxBitmap* pOriginalBitmap = new wxBitmap( *pWxImage );
	//			delete pWxImage;
	//			pWxImage = NULL;

	//			m_pFullImageCanvas->SetBitmap( pOriginalBitmap );
	//			const CGUIRect& rRect = pImage->GetUVRect();
	//			wxSize newSize( 
	//				pOriginalBitmap->GetSize().GetWidth() * rRect.GetWidth(), 
	//				pOriginalBitmap->GetSize().GetHeight()* rRect.GetHeight() );
	//			wxPoint newPoint( 
	//				rRect.GetPosition().x * pOriginalBitmap->GetSize().GetWidth(), 
	//				rRect.GetPosition().y * pOriginalBitmap->GetSize().GetHeight() );
	//			wxRect aUVRect( newPoint, newSize );
	//			m_pFullImageCanvas->SetUVRect( aUVRect );
	//		}
	//	}
	//	if( pImage )
	//	{
	//		pImage->RefRelease();
	//	}
	//}

	m_pImageCanvas->Refresh();
	m_pFullImageCanvas->Refresh();
}
//------------------------------------------------------------------------------



//============================================================================//
// WxLocalizedStringPreviewPanel
//============================================================================// 
//------------------------------------------------------------------------------
WxLocalizedStringPreviewPanel::WxLocalizedStringPreviewPanel( wxWindow* parent )
	:WxResourcePreviewPanelBase( parent, "CGUILocalizationDefine" )
{
	m_pTextBox = new wxTextCtrl( this, wxID_ANY );
	m_pTextBox->SetEditable( false );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( m_pTextBox, 1, wxALL|wxEXPAND );
	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxLocalizedStringPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	m_strResourceName = rResourceName;
	if( !m_strResourceName.empty())
	{
		m_pTextBox->SetLabelText( (wxChar*)CGUILocalizationManager::Instance()->Localize( wx2GuiString(rResourceName)).c_str());
	}
}
//------------------------------------------------------------------------------



//============================================================================//
// WxSoundPreviewPanel
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSoundPreviewPanel, WxResourcePreviewPanelBase )
	EVT_BUTTON( ID_SoundSelect_BTN_PLAY, WxSoundPreviewPanel::OnPlay )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSoundPreviewPanel::WxSoundPreviewPanel( wxWindow* parent )
:WxResourcePreviewPanelBase( parent, "CGUISoundDefine" )
{
	wxButton *pBtnPlay = new wxButton( this, ID_SoundSelect_BTN_PLAY, wxT("Play") );

	wxSizer *sizerTop = new wxBoxSizer( wxVERTICAL );
	sizerTop->Add( pBtnPlay, 0, wxALIGN_CENTER );
	SetSizer( sizerTop );
}
//------------------------------------------------------------------------------
void WxSoundPreviewPanel::SetResourceName( const wxString& rResourceName )
{
	m_strResourceName = rResourceName;
}
//------------------------------------------------------------------------------
void WxSoundPreviewPanel::OnPlay(wxCommandEvent& event)
{
	if( !m_strResourceName.empty())
	{
		CGUIInterfaceManager::Instance()->GetInterfaceSound()->PlayEffect( wx2GuiString(m_strResourceName ) );
	}
}
//------------------------------------------------------------------------------
