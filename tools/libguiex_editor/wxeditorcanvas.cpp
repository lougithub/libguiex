/** 
* @file editorcanvas.h
* @brief opengl canvas for rendering
* @author ken
* @date 2007-09-11
*/


//============================================================================//
// include
//============================================================================// 
#include "wxeditorcanvas.h"
#include "wxeditorcanvascontainer.h"
#include "wxmainapp.h"
#include "wxmainframe.h"
#include "editorutility.h"
#include <libguiex_core/guiex.h>

#include <sys/timeb.h>
#include <GL/glu.h>


using namespace guiex;

//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxEditorCanvas, wxGLCanvas)
EVT_SIZE(WxEditorCanvas::OnSize)
EVT_PAINT(WxEditorCanvas::OnPaint)
EVT_ERASE_BACKGROUND(WxEditorCanvas::OnEraseBackground)
EVT_KEY_DOWN( WxEditorCanvas::OnKeyDown )
EVT_KEY_UP( WxEditorCanvas::OnKeyUp )
EVT_ENTER_WINDOW( WxEditorCanvas::OnEnterWindow )
EVT_MOTION(WxEditorCanvas::OnMouseMove)
EVT_LEFT_DOWN(WxEditorCanvas::OnMouseLeftDown) 
EVT_LEFT_UP(WxEditorCanvas::OnMouseLeftUp) 
EVT_TIMER(100, WxEditorCanvas::OnTimer)
END_EVENT_TABLE()

//------------------------------------------------------------------------------
WxEditorCanvas::WxEditorCanvas(wxWindow *parent, int* args, wxWindowID id,
					   const wxPoint& pos, const wxSize& size, long style, const wxString& name)
					   : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name )
					   ,m_timer(this, 100)
					   ,m_hoveredResizePoint(RESIZE_POINT_NONE)
					   ,m_previousHoveredResizePoint(RESIZE_POINT_NONE)
					   ,m_hoveredWindow(NULL)
					   ,m_mousePressed(false)
					   ,m_mouseX(0)
					   ,m_mouseY(0)
					   ,m_previousMouseX(0)
					   ,m_previousMouseY(0)
					   ,m_bWidgetStatusChanged(false)
					   ,m_pContainer((WxEditorCanvasContainer*)parent)
{
	m_timer.Start(33);
}
//------------------------------------------------------------------------------
WxEditorCanvas::~WxEditorCanvas()
{
	DestroyCanvas();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::InitializeCanvas()
{
	SetCurrent();

	wxSize aCanvasSize( GSystem->GetScreenWidth(), GSystem->GetScreenHeight());
	UpdateCanvasSize(aCanvasSize);

	CGUIFrameworkEditor::ms_pFramework->RegisterOpenglInterface();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::DestroyCanvas()
{
	GSystem->DestroyAllWidgets();
	GSystem->UnloadAllResource();

	CGUIFrameworkEditor::ms_pFramework->UnregisterOpenglInterface();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::UpdateWindowBox()
{
	m_aWindowBox.Reset();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnTimer(wxTimerEvent& event)
{
	CGUIFrameworkEditor::ms_pFramework->Update( event.GetInterval() / 1000.f );
	Refresh();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::DrawResizers()
{
	GLfloat x0, y0, x1, y1;

	// For each selected window, draw resize boxes
	if( m_aWindowBox.GetWindow())
	{
		{
			// Draw a rectangle around the window. Don't use DrawRectangle because it fills
			// the area. We don't want that, just lines.
			wxRect winRc = m_aWindowBox.GetWindowRect();

			// Non-filled RED rectangle
			glColor3f (1.0f, 0.0f, 0.0f);
			glBegin(GL_LINE_LOOP);
			x0 = winRc.x;
			y0 = winRc.y;
			x1 = winRc.GetRight();
			y1 = winRc.GetBottom();

			glVertex2f(x0, y0);
			glVertex2f(x1, y0);
			glVertex2f(x1, y1);
			glVertex2f(x0, y1);
			glEnd();	// GL_LINE_LOOP


			// When the window is not locked, draw the eight points as white rectangles with a red border.
			if (!m_aWindowBox.IsLocked())
			{
				for (int i = 0; i < NUM_RESIZE_POINTS; ++i)
				{
					const wxRect& winRc = m_aWindowBox.GetPointRect(i);

					// WHITE filled rectangle for the inside
					glColor3f (1.0f, 1.0f, 1.0f);
					glBegin(GL_QUADS);
					x0 = winRc.x;
					y0 = winRc.y;
					x1 = winRc.GetRight();
					y1 = winRc.GetBottom();

					glVertex2f(x0, y0);
					glVertex2f(x1, y0);
					glVertex2f(x1, y1);
					glVertex2f(x0, y1);
					glEnd();	// GL_QUADS

					// RED surrounding rectangle for the border (just draw over the filled rectangle)
					glColor3f (1.0f, 0.0f, 0.0f);
					glBegin(GL_LINE_LOOP);
					x0 = winRc.x;
					y0 = winRc.y;
					x1 = winRc.GetRight();
					y1 = winRc.GetBottom();

					glVertex2f(x0, y0);
					glVertex2f(x1, y0);
					glVertex2f(x1, y1);
					glVertex2f(x0, y1);
					glEnd();	// GL_LINE_LOOP
				}
			}
		}


		//draw anchor
		{
			const wxRect& winRc = m_aWindowBox.GetAnchorRect();
			// green surrounding rectangle for the border (just draw over the filled rectangle)
			glColor3f (0.0f, 1.0f, 0.0f);
			glBegin(GL_QUADS);
			x0 = winRc.x;
			y0 = winRc.y;
			x1 = winRc.GetRight();
			y1 = winRc.GetBottom();

			glVertex2f(x0, y0);
			glVertex2f(x1, y0);
			glVertex2f(x1, y1);
			glVertex2f(x0, y1);
			glEnd();	// GL_LINE_LOOP
		}
	}
}
//------------------------------------------------------------------------------
void WxEditorCanvas::Render()
{
	wxPaintDC dc(this);

	SetCurrent();

	/* clear color and depth buffers */
	const wxColour& rBGColor = GetMainFrame()->GetBGColor();
	glClearColor( rBGColor.Red() / 255.f, rBGColor.Green() / 255.f, rBGColor.Blue() / 255.f, rBGColor.Alpha() / 255.f );
	glClearStencil( 0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );	// clear screen and depth buffer 

	CGUIFrameworkEditor::ms_pFramework->Render();

	DrawResizers();

	glFlush();
	SwapBuffers();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
	//SetFocus();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	Render();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	wxSize aSize = event.GetSize();

	UpdateCanvasSize(aSize);
	this->m_aWindowBox.Reset( );
}
//------------------------------------------------------------------------------
void WxEditorCanvas::UpdateCanvasSize(const wxSize& rSize)
{
	SetCurrent();
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

//#if 1
///	gluOrtho2D(0.0, rSize.GetWidth(),rSize.GetHeight(),0.0 );
//#else
//	real fPerspectiveDegree = 45;
//	gluPerspective( fPerspectiveDegree, rSize.GetWidth()/rSize.GetHeight(), 0.1, 100000 );
//	real fZDistance = rSize.GetHeight()/2 / CGUIMath::Tan( CGUIDegree(fPerspectiveDegree/2));
//	gluLookAt( 
//		rSize.GetWidth()/2,rSize.GetHeight()/2,-fZDistance,
//		rSize.GetWidth()/2,rSize.GetHeight()/2,0, 
//		0,-1,0);
//#endif

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();	
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnKeyDown( wxKeyEvent& event )
{
	int pixelDeltaX = 0, pixelDeltaY = 0;
	int key = event.GetKeyCode();

	if (key == 'w' || key == 'W' || key == WXK_UP)
	{
		pixelDeltaY = -1;
	}
	else if (key == 's' || key == 'S' || key == WXK_DOWN)
	{
		pixelDeltaY = 1;
	}
	else if (key == 'a' || key == 'A' || key == WXK_LEFT)
	{
		pixelDeltaX = -1;
	}
	else if (key == 'd' || key == 'D' || key == WXK_RIGHT)
	{
		pixelDeltaX = 1;
	}

	if( m_aWindowBox.GetWindow() && (pixelDeltaX != 0 || pixelDeltaY != 0))
	{
		m_aWindowBox.MoveWindowPosition(pixelDeltaX, pixelDeltaY);
		GetMainFrame()->UpdateWidgetSizeAndPos();
		m_pContainer->SetSaveFlag(true);
	}

}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnKeyUp( wxKeyEvent& event )
{
	event.Skip();
}
//------------------------------------------------------------------------------
void WxEditorCanvas::UpdateCursor() 
{
	if (m_hoveredResizePoint != m_previousHoveredResizePoint)
	{
		switch (m_hoveredResizePoint) 
		{
		case RESIZE_POINT_WN:
			// Upper left
			SetCursor(wxCursor(wxCURSOR_SIZENWSE));
			break;
		case RESIZE_POINT_N:
			// Upper middle
			SetCursor(wxCursor(wxCURSOR_SIZENS));
			break;
		case RESIZE_POINT_NE:
			// Upper right
			SetCursor(wxCursor(wxCURSOR_SIZENESW));
			break;
		case RESIZE_POINT_E:
			// Middle right
			SetCursor(wxCursor(wxCURSOR_SIZEWE));
			break;
		case RESIZE_POINT_ES:
			// Bottom right
			SetCursor(wxCursor(wxCURSOR_SIZENWSE));
			break;
		case RESIZE_POINT_S:
			// Bottom middle
			SetCursor(wxCursor(wxCURSOR_SIZENS));
			break;
		case RESIZE_POINT_SW:
			// Bottom left
			SetCursor(wxCursor(wxCURSOR_SIZENESW));
			break;
		case RESIZE_POINT_W:
			// Middle left
			SetCursor(wxCursor(wxCURSOR_SIZEWE));
			break;
		default:
			// Default cursor
			SetCursor(wxCursor(wxCURSOR_ARROW));
			break;
		}
	}
	m_previousHoveredResizePoint = m_hoveredResizePoint;
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnMouseMove(wxMouseEvent& event)
{
	HandleMouseMoved(event.m_x, event.m_y);
}
//------------------------------------------------------------------------------
void WxEditorCanvas::HandleMouseMoved (int aMouseX, int aMouseY)
{

	wxChar statusInfo [512];
	bool bChanges = false;
	float pixelDeltaX, pixelDeltaY ;

	// Copy parameters into members
	m_mouseX = aMouseX ;
	m_mouseY = aMouseY ;

	// Minimum status bar text
	wxSnprintf (statusInfo, 100, wxT("[%d, %d][%3.2f, %3.2f]"), 
		m_mouseX, 
		m_mouseY, 
		(double) m_mouseX / GSystem->GetScreenWidth(), 
		(double) m_mouseY / GSystem->GetScreenHeight());

	// Only proceed when the mouse is pressed
	if (m_mousePressed) 
	{
		// Calculate both value-type, so the code below can choose,
		// based on the metric modes of the elements.
		wxSnprintf (statusInfo, 100, wxT("%s - dragging"), statusInfo) ;

		// Calculate pixel differences with the previous mouse position.
		pixelDeltaX = (float)(m_mouseX - m_previousMouseX) ;
		pixelDeltaY = (float)(m_mouseY - m_previousMouseY) ;

		if (m_hoveredResizePoint != RESIZE_POINT_NONE)
		{	
			//SelectionMover mover (&m_selection);	//Todo: view->GetSelection(); or something.

			// The mouse is pressed on a resize point; resize each selected window accordingly.
			switch (m_hoveredResizePoint) 
			{
			case RESIZE_POINT_WN:
				// Upper left
				m_aWindowBox.SetWindowSize(pixelDeltaX, pixelDeltaY, 0, 0);
				break;
			case RESIZE_POINT_N:
				// Upper middle
				m_aWindowBox.SetWindowSize(0, pixelDeltaY, 0, 0);
				break;
			case RESIZE_POINT_NE:
				// Upper right
				m_aWindowBox.SetWindowSize(0, pixelDeltaY, pixelDeltaX, 0);
				break;
			case RESIZE_POINT_E:
				// Middle right
				m_aWindowBox.SetWindowSize(0, 0, pixelDeltaX, 0);
				break;
			case RESIZE_POINT_ES:
				// Bottom right
				m_aWindowBox.SetWindowSize(0, 0, pixelDeltaX, pixelDeltaY);
				break;
			case RESIZE_POINT_S:
				// Bottom middle
				m_aWindowBox.SetWindowSize(0, 0, 0, pixelDeltaY);
				break;
			case RESIZE_POINT_SW:
				// Bottom left
				m_aWindowBox.SetWindowSize(pixelDeltaX, 0, 0, pixelDeltaY);
				break;
			case RESIZE_POINT_W:
				// Middle left
				m_aWindowBox.SetWindowSize(pixelDeltaX, 0, 0, 0);
				break;
			default:
				break;
			}
			bChanges = true ;
		}
		else if (m_aWindowBox.GetWindow())
		{	
			// The mouse is pressed inside a window; drag entire selection.
			// We drag, so the size doesn't change. Therefore we pass the same two parameters for the right bottom values.
			m_aWindowBox.MoveWindowPosition(pixelDeltaX, pixelDeltaY);
			bChanges = true;
		}

		if (bChanges) 
		{
			m_bWidgetStatusChanged = true;
			m_pContainer->SetSaveFlag(true);
		}
	}
	else 
	{	// Mouse not pressed; find hovered resize point or window
		if (m_aWindowBox.GetWindow()) 
		{
			m_hoveredResizePoint = m_aWindowBox.GetPointAtPosition (wxPoint(m_mouseX, m_mouseY));
		}
		if (m_hoveredResizePoint == RESIZE_POINT_NONE)
		{
			// Didn't find resize point, try window
			SetHoveredWindow(GSystem->GetWidgetUnderPoint(CGUIVector2((float)m_mouseX, (float)m_mouseY)));
		}
	}

	// Hovering is exclusive; either a resize point or a window. Never both.
	if (m_hoveredResizePoint != RESIZE_POINT_NONE)
	{
		SetHoveredWindow( NULL );
		wxSnprintf (statusInfo, 100, wxT("%s Resize Point: %d"), statusInfo, m_hoveredResizePoint);
	}
	else if (m_hoveredWindow != NULL) 
	{
		m_hoveredResizePoint = RESIZE_POINT_NONE;
		wxSnprintf (statusInfo, 100, wxT("%s Window: %s"), statusInfo, Gui2wxString(m_hoveredWindow->GetName()).data());
		// Selectable?
		if( CGUIWidgetManager::IsInternalWidget( m_hoveredWindow->GetName()))
		{
			// No, add to status info
			wxSnprintf (statusInfo, 100, wxT("%s (not selectable)"), statusInfo);
		}
	}

	GetMainFrame()->UpdateStatusBar (statusInfo);
	UpdateCursor();

	//// Reset previous values
	m_previousMouseX = m_mouseX ;
	m_previousMouseY = m_mouseY ;
}
//------------------------------------------------------------------------------
void WxEditorCanvas::SelectWidget( CGUIWidget* pWidget )
{
	m_aWindowBox.SetWindow(pWidget);
	GetMainFrame()->SetPropGridWidget(pWidget);
	GetMainFrame()->SelectWidgetTreeItem(pWidget);

	if( pWidget )
	{
		pWidget->SetFocus( true );
	}
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnMouseLeftDown(wxMouseEvent& event)
{
	while(m_hoveredWindow && CGUIWidgetManager::IsInternalWidget( m_hoveredWindow->GetName()) )
	{   
		// Current window is an internal widget, goto parent (if any)
		SetHoveredWindow( m_hoveredWindow->GetParent() );
	}   

	if (m_hoveredWindow) 
	{  
		SelectWidget(m_hoveredWindow);
		GetMainFrame()->OutputString(std::string("Widget selected: ") + m_hoveredWindow->GetName());
	}     
	else
	{
		if (m_hoveredResizePoint == RESIZE_POINT_NONE)
		{
			SelectWidget(NULL);
		}
	}

	// Save this state for during mouse movements
	m_mousePressed = true;

	SetFocus( );
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnMouseLeftUp(wxMouseEvent& event)
{
	bool mustNotify = (m_hoveredResizePoint != RESIZE_POINT_NONE || m_hoveredWindow);
	m_mousePressed = false ;
	m_hoveredResizePoint = RESIZE_POINT_NONE;
	SetHoveredWindow( NULL );
	if (mustNotify)
	{
		//for (size_t i = 0; i < observers.size(); ++i) 
		//{
		//	observers [i]->SelectionDraggedOrResized();
		//}
	}

	if( m_bWidgetStatusChanged )
	{
		GetMainFrame()->UpdateWidgetSizeAndPos();
		m_pContainer->SetSaveFlag(true);
		m_bWidgetStatusChanged = false;
	}

	HandleMouseMoved(event.m_x, event.m_y);
}
//------------------------------------------------------------------------------
void WxEditorCanvas::OnWidgetDestroyed(CGUIWidget* pWidget)
{
	if( pWidget == m_hoveredWindow )
	{
		m_hoveredWindow = NULL;
	}
}
//------------------------------------------------------------------------------
void WxEditorCanvas::SetHoveredWindow( CGUIWidget* pWidget )
{
	if( pWidget == m_hoveredWindow )
	{
		return;
	}
	if( m_hoveredWindow )
	{
		m_hoveredWindow->GetOnWidgetDestroyedSignal().disconnect( this );
	}
	m_hoveredWindow = pWidget;
	if( m_hoveredWindow )
	{
		m_hoveredWindow->GetOnWidgetDestroyedSignal().connect( this, &WxEditorCanvas::OnWidgetDestroyed );
	}
}
//------------------------------------------------------------------------------
