/** 
* @file wxresourcepreviewpanelbase.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanelbase.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"
#include "libguiex_core/guiex.h"

using namespace guiex;
//============================================================================//
// function
//============================================================================// 


//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxResourcePreviewPanelBase, wxPanel )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase::WxResourcePreviewPanelBase( wxWindow* parent )
:wxScrolledWindow( parent )
{
}
//------------------------------------------------------------------------------




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

	pRender->BeginRender();
	pRender->PushMatrix();
	pRender->LoadIdentityMatrix();
	CGUIMatrix4 matrix;
	matrix.makeTransform( CGUIVector3(ClientSize.x/2, ClientSize.y/2, 0), CGUIVector3(1.0f,1.0f,1.0f), CGUIQuaternion());
	pRender->MultMatrix(matrix);

	OnRender( pRender );
	
	pRender->PopMatrix();
	pRender->EndRender();

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
