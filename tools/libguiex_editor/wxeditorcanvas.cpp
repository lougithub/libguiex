/** 
* @file editorcanvas.h
* @brief opengl canvas for rendering
* @author ken
* @date 2007-09-11
*/


//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"



#include <sys/timeb.h>
#include <GL\glu.h>

//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(WxEditorCanvasContainer, wxScrolledWindow)

BEGIN_EVENT_TABLE(WxEditorCanvasContainer, wxScrolledWindow)
END_EVENT_TABLE()
//------------------------------------------------------------------------------

WxEditorCanvasContainer::WxEditorCanvasContainer( wxWindow *parent, const std::string& rFilename )
: wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxTAB_TRAVERSAL, _T("canvas container") )
,CSaveFileBase(rFilename)

{
	SetScrollRate( 10, 10 );
	SetVirtualSize( guiex::GSystem->GetScreenWidth(), guiex::GSystem->GetScreenHeight() );

	SetBackgroundColour( *wxLIGHT_GREY );

	//create canvas
	wxSize aCanvasSize( guiex::GSystem->GetScreenWidth(), guiex::GSystem->GetScreenHeight());
	int wx_gl_attribs[] = {
		WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 
		WX_GL_DEPTH_SIZE, 24, 
		WX_GL_STENCIL_SIZE, 8,
		0};
	m_pCanvas = new WxGLCanvas(this, wx_gl_attribs, wxID_ANY, wxDefaultPosition, aCanvasSize);
	m_pCanvas->InitializeCanvas();
	//m_pCanvas->SetNextHandler(  this );
}
//------------------------------------------------------------------------------
void	WxEditorCanvasContainer::UpdateWindowBox()
{
	m_pCanvas->UpdateWindowBox();
}
//------------------------------------------------------------------------------
int 	WxEditorCanvasContainer::SaveFileAs(const std::string& rNewFileName)
{
	return m_pCanvas->SaveToFile(rNewFileName);
}
//------------------------------------------------------------------------------
void	WxEditorCanvasContainer::SetSelectedWidget( const std::string& rWidget )
{
	guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->GetWidget( rWidget.c_str(), GetMainFrame()->GetCurrentSceneName());
	SetSelectedWidget(pWidget);
}
//------------------------------------------------------------------------------
void	WxEditorCanvasContainer::SetSelectedWidget( guiex::CGUIWidget* pWidget )
{
	m_pCanvas->SelectWidget(pWidget);
}
//------------------------------------------------------------------------------
guiex::CGUIWidget*	WxEditorCanvasContainer::GetSelectedWidget()
{
	return m_pCanvas->m_aWindowBox.GetWindow();
}
//------------------------------------------------------------------------------
void	WxEditorCanvasContainer::SetScreenSize( int width, int height )
{
	if( m_pCanvas )
	{
		m_pCanvas->SetSize(width, height);
		m_pCanvas->Refresh();
		SetVirtualSize( width,height );
		Refresh();
	}
}
//------------------------------------------------------------------------------







//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(WxGLCanvas, wxGLCanvas)
EVT_SIZE(WxGLCanvas::OnSize)
EVT_PAINT(WxGLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(WxGLCanvas::OnEraseBackground)
EVT_KEY_DOWN( WxGLCanvas::OnKeyDown )
EVT_KEY_UP( WxGLCanvas::OnKeyUp )
EVT_ENTER_WINDOW( WxGLCanvas::OnEnterWindow )
EVT_MOTION(WxGLCanvas::OnMouseMove)
EVT_LEFT_DOWN(WxGLCanvas::OnMouseLeftDown) 
EVT_LEFT_UP(WxGLCanvas::OnMouseLeftUp) 
EVT_TIMER(100, WxGLCanvas::OnTimer)
END_EVENT_TABLE()


//------------------------------------------------------------------------------
WxGLCanvas::WxGLCanvas(wxWindow *parent, int* args, wxWindowID id,
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
WxGLCanvas::~WxGLCanvas()
{
	//free widgets
	guiex::GSystem->DestroyAllWidgets();
	guiex::GSystem->UnloadAllResource();

	GUI_UNREGISTER_INTERFACE_LIB( IGUIRender_opengl);
}
//------------------------------------------------------------------------------
void WxGLCanvas::InitializeCanvas()
{
	SetCurrent();

	wxSize aCanvasSize( guiex::GSystem->GetScreenWidth(), guiex::GSystem->GetScreenHeight());
	UpdateCanvasSize(aCanvasSize);

	GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl );
}
//------------------------------------------------------------------------------
void WxGLCanvas::UpdateWindowBox()
{
	m_aWindowBox.Reset();
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnTimer(wxTimerEvent& event)
{
	guiex::GSystem->Update( event.GetInterval() / 1000.f );
	Refresh();
}
//------------------------------------------------------------------------------
void WxGLCanvas::DrawResizers()
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
void WxGLCanvas::Render()
{
	wxPaintDC dc(this);

#ifndef __WXMOTIF__
	if (!GetContext()) return;
#endif

	SetCurrent();

	/* clear color and depth buffers */
	const wxColour& rBGColor = GetMainFrame()->GetBGColor();
	glClearColor( rBGColor.Red() / 255.f, rBGColor.Green() / 255.f, rBGColor.Blue() / 255.f, rBGColor.Alpha() / 255.f );
	glClearStencil( 0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );	// clear screen and depth buffer 

	guiex::GSystem->Render();

	DrawResizers();

	glFlush();
	SwapBuffers();
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
	//SetFocus();
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	Render();
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	wxSize aSize = event.GetSize();

	UpdateCanvasSize(aSize);
	this->m_aWindowBox.Reset( );
}
//------------------------------------------------------------------------------
void WxGLCanvas::UpdateCanvasSize(const wxSize& rSize)
{
	SetCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#if 1
	gluOrtho2D(0.0, rSize.GetWidth(),rSize.GetHeight(),0.0 );
#else
	real fPerspectiveDegree = 45;
	gluPerspective( fPerspectiveDegree, rSize.GetWidth()/rSize.GetHeight(), 0.1, 100000 );
	real fZDistance = rSize.GetHeight()/2 / guiex::CGUIMath::Tan( CGUIDegree(fPerspectiveDegree/2));
	gluLookAt( 
		rSize.GetWidth()/2,rSize.GetHeight()/2,-fZDistance,
		rSize.GetWidth()/2,rSize.GetHeight()/2,0, 
		0,-1,0);
#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	glViewport(0,0,rSize.x,rSize.y); //定义视口 
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
}
//------------------------------------------------------------------------------
void WxGLCanvas::OnKeyDown( wxKeyEvent& event )
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
		GetMainFrame()->GetPropGrid()->UpdateWidgetSizeAndPos();
		m_pContainer->SetSaveFlag(true);
	}

}
//------------------------------------------------------------------------------
void WxGLCanvas::OnKeyUp( wxKeyEvent& event )
{
	event.Skip();
}
//------------------------------------------------------------------------------
void WxGLCanvas::UpdateCursor() 
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
void WxGLCanvas::OnMouseMove(wxMouseEvent& event)
{
	HandleMouseMoved(event.m_x, event.m_y);
}
//------------------------------------------------------------------------------
void WxGLCanvas::HandleMouseMoved (int aMouseX, int aMouseY)
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
		(double) m_mouseX / guiex::GSystem->GetScreenWidth(), 
		(double) m_mouseY / guiex::GSystem->GetScreenHeight());

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
			m_hoveredWindow = guiex::GSystem->GetWidgetUnderPoint(guiex::CGUIVector2((float)m_mouseX, (float)m_mouseY)) ;
		}
	}

	// Hovering is exclusive; either a resize point or a window. Never both.
	if (m_hoveredResizePoint != RESIZE_POINT_NONE)
	{
		m_hoveredWindow = NULL;
		wxSnprintf (statusInfo, 100, wxT("%s Resize Point: %d"), statusInfo, m_hoveredResizePoint);
	}
	else if (m_hoveredWindow != NULL) 
	{
		m_hoveredResizePoint = RESIZE_POINT_NONE;
		wxSnprintf (statusInfo, 100, wxT("%s Window: %s"), statusInfo, wxConvUTF8.cMB2WC(m_hoveredWindow->GetName().c_str()).data());
		// Selectable?
		if(m_hoveredWindow->GetName().find("__auto__") != guiex::CGUIString::npos)
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
void	WxGLCanvas::SelectWidget( guiex::CGUIWidget* pWidget )
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
void WxGLCanvas::OnMouseLeftDown(wxMouseEvent& event)
{
	while(m_hoveredWindow && m_hoveredWindow->GetName().find("__auto__") != guiex::CGUIString::npos )
	{   
		// Current window is an __auto__, goto parent (if any)
		m_hoveredWindow = m_hoveredWindow->GetParent();
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
void WxGLCanvas::OnMouseLeftUp(wxMouseEvent& event)
{
	bool mustNotify = (m_hoveredResizePoint != RESIZE_POINT_NONE || m_hoveredWindow);
	m_mousePressed = false ;
	m_hoveredResizePoint = RESIZE_POINT_NONE;
	m_hoveredWindow = NULL ;
	if (mustNotify)
	{
		//for (size_t i = 0; i < observers.size(); ++i) 
		//{
		//	observers [i]->SelectionDraggedOrResized();
		//}
	}

	if( m_bWidgetStatusChanged )
	{
		GetMainFrame()->GetPropGrid()->UpdateWidgetSizeAndPos();
		m_pContainer->SetSaveFlag(true);
		m_bWidgetStatusChanged = false;
	}

	HandleMouseMoved(event.m_x, event.m_y);
}
//------------------------------------------------------------------------------
int	WxGLCanvas::SaveToFile( const std::string& rFilename)
{
	//open xml file
	TiXmlDocument aDoc;
	aDoc.LoadFile(rFilename.c_str());
	if( aDoc.Error())
	{
		////failed to parse
		//wxChar buf[1024];
		//wxSnprintf( buf, 1024, "Failed to read config file! \n\n%s", aDoc.ErrorDesc());
		//wxMessageBox(buf, "Error", wxICON_ERROR, GetMainFrame() );
		//return -1;

		TiXmlElement  aNewToppestNode("guiex");
		aDoc.InsertEndChild(aNewToppestNode);
	}

	//remove old widget from doc
	TiXmlElement* pRootNode = aDoc.RootElement();
	wxASSERT(pRootNode);
	TiXmlElement* pFindNode = NULL;
	TiXmlElement* pNode = pRootNode->FirstChildElement("property");
	while( pNode )
	{
		TiXmlElement* pNextNode = pNode->NextSiblingElement("property");
		if( std::string(pNode->Attribute("type")) == "CGUIWidgetDefine" )
		{
			pRootNode->RemoveChild(pNode);
		}
		pNode = pNextNode;
	}

	//get page
	if( guiex::GSystem->GetOpenedPageNum() != 0 )
	{
		guiex::CGUIWidget* pWidget = guiex::GSystem->GetOpenedPageByIndex(0);
		if( pWidget )
		{
			//save it to doc
			if( 0 != SaveWidgetNodeToDoc( pWidget, aDoc ))
			{
				//failed
				return -1;
			}
		}
	}

	//save it to file
	if( false == aDoc.SaveFile( rFilename.c_str()))
	{
		//failed to save as file
		return -1;
	}

	return 0;
}
//------------------------------------------------------------------------------
int WxGLCanvas::SaveWidgetNodeToDoc( guiex::CGUIWidget* pWidget, TiXmlDocument& rDoc )
{
	wxASSERT( pWidget );
	if(pWidget->GetName().find("__auto__") != guiex::CGUIString::npos)
	{
		//a "auto" created element, ignore it
		return 0;
	}

	TiXmlElement* pRootNode = rDoc.RootElement();
	wxASSERT(pRootNode);

	//save widget to doc
	TiXmlElement* pWidgetNode = NULL;
	//insert a widget
	TiXmlElement aNewNode("property");
	aNewNode.SetAttribute("name", pWidget->GetName().c_str());
	aNewNode.SetAttribute("type", "CGUIWidgetDefine");
	aNewNode.SetAttribute("value", pWidget->GetType().c_str());

	if( guiex::CGUIWidgetManager::Instance()->HasPage( pWidget))
	{
		pWidgetNode = (TiXmlElement*)pRootNode->InsertEndChild(aNewNode);
	}
	else
	{
		TiXmlElement* pParentNode = GetElementByName(_T("property"), wxConvUTF8.cMB2WC(pWidget->GetParent()->GetName().c_str()).data(), pRootNode);
		wxASSERT(pParentNode);
		pWidgetNode = (TiXmlElement*)pRootNode->InsertEndChild(aNewNode);
	}
	wxASSERT(pWidgetNode);

	//save property to doc
	guiex::CGUIProperty aSet = pWidget->GetProperty();
	//process parent first
	if( aSet.HasProperty("parent"))
	{
		guiex::CGUIProperty* pProperty = aSet.GetProperty("parent");
		AddTopPropertyElement(*pProperty, pWidgetNode);
		aSet.RemoveProperty(*pProperty);
	}

	//process image
	std::vector<guiex::CGUIProperty>	aImgVector;
	for( unsigned i=0; i<aSet.GetPropertyNum(); ++i)
	{
		const guiex::CGUIProperty* pProperty = aSet.GetProperty(i);
		if( pProperty->GetType() == guiex::ePropertyType_Image )
		{
			aImgVector.push_back(*pProperty);
		}
	}
	for( std::vector<guiex::CGUIProperty>::iterator itor = aImgVector.begin();
		itor != aImgVector.end();
		++itor )
	{
		AddTopPropertyElement(*itor, pWidgetNode);
		aSet.RemoveProperty(*itor);
	}

	//process left
	for( unsigned i=0; i<aSet.GetPropertyNum(); ++i)
	{
		const guiex::CGUIProperty* pProperty = aSet.GetProperty(i);

		AddTopPropertyElement(*pProperty, pWidgetNode);
	}

	//process it's child
	guiex::CGUIWidget* pChild = pWidget->GetChild();
	while( pChild )
	{
		if( 0 != SaveWidgetNodeToDoc( pChild, rDoc ))
		{
			//failed
			return -1;
		}

		pChild = pChild->GetNextSibling();
	}

	return 0;
}
//------------------------------------------------------------------------------
void WxGLCanvas::AddTopPropertyElement( const guiex::CGUIProperty& rProperty, TiXmlElement* pWidgetNode)
{
	if( rProperty.GetValue().empty() && rProperty.GetPropertyNum() == 0 )
	{
		//empty property, ignore it
		return;
	}

	//get exist's one
	TiXmlElement* pOldNode = GetElementByName(_T("property"), wxConvUTF8.cMB2WC(rProperty.GetName().c_str()).data(), pWidgetNode);

	//add toppest element
	TiXmlElement* pToppestNode = NULL;
	TiXmlElement  aNewToppestNode("property");
	aNewToppestNode.SetAttribute("name",rProperty.GetName().c_str());
	aNewToppestNode.SetAttribute("type",rProperty.GetTypeAsString().c_str());
	if( !rProperty.GetValue().empty())
	{
		aNewToppestNode.SetAttribute("value",rProperty.GetValue().c_str());
	}

	//insert it
	if( pOldNode )
	{
		pToppestNode = (TiXmlElement*)pWidgetNode->InsertAfterChild(pOldNode, aNewToppestNode);
	}
	else
	{
		pToppestNode = (TiXmlElement*)pWidgetNode->InsertEndChild( aNewToppestNode);
	}
	wxASSERT(pToppestNode);

	//remove old one
	if( pOldNode )
	{
		pWidgetNode->RemoveChild(pOldNode);
	}

	//add all sub-property
	if( rProperty.GetPropertyNum() > 0)
	{
		for( unsigned i=0; i<rProperty.GetPropertyNum(); ++i)
		{
			AddSubPropertyElement( *rProperty.GetProperty(i), pToppestNode );
		}
	}
}
//------------------------------------------------------------------------------
TiXmlElement*	WxGLCanvas::GetElementByName( 
	const wxString& rElementName,
	const wxString& rAttrName, 
	TiXmlElement* pParentElement )
{
	//get widget node
	TiXmlElement* pFindNode = NULL;
	TiXmlElement* pNode = pParentElement->FirstChildElement(rElementName.char_str(wxConvUTF8).data());
	while( pNode )
	{
		if( rAttrName == wxConvUTF8.cMB2WC(pNode->Attribute("name")).data())
		{
			pFindNode = pNode;
			break;
		}
		pNode = pNode->NextSiblingElement(rElementName.char_str(wxConvUTF8).data());
	}

	return pNode;
}
//------------------------------------------------------------------------------
void WxGLCanvas::AddSubPropertyElement( const guiex::CGUIProperty& rProperty, TiXmlElement* pParentElement)
{
	//add current property
	TiXmlElement* pToppestNode = NULL;
	TiXmlElement  aNewToppestNode("property");
	aNewToppestNode.SetAttribute("name",rProperty.GetName().c_str());
	aNewToppestNode.SetAttribute("type",rProperty.GetTypeAsString().c_str());
	if( !rProperty.GetValue().empty())
	{
		aNewToppestNode.SetAttribute("value",rProperty.GetValue().c_str());
	}

	//insert it
	pToppestNode = (TiXmlElement*)pParentElement->InsertEndChild( aNewToppestNode);
	wxASSERT(pToppestNode);

	//add all sub-property
	if( rProperty.GetPropertyNum() > 0)
	{
		for( unsigned i=0; i<rProperty.GetPropertyNum(); ++i)
		{
			AddSubPropertyElement( *rProperty.GetProperty(i), pToppestNode );
		}
	}
}
//------------------------------------------------------------------------------



