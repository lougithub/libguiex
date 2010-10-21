/** 
* @file editorcanvas.h
* @brief opengl canvas for rendering
* @author ken
* @date 2007-09-11
*/

#ifndef __KEN_OPENGL_CANVAS_20070911_H__
#define __KEN_OPENGL_CANVAS_20070911_H__


//============================================================================//
// include
//============================================================================// 
#include "tinyxml.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceMouse;
	class CGUIWidget;
	class CGUIProperty;
}
class WxGLCanvas;



//============================================================================//
// class
//============================================================================// 
class WxEditorCanvasContainer : public wxScrolledWindow, public CSaveFileBase
{
public:
	//WxEditorCanvasContainer() {}
	WxEditorCanvasContainer( wxWindow *parent, const std::string& rFilename );
	~WxEditorCanvasContainer(){};

	//update window's box shown in screen
	void	UpdateWindowBox();


	//save file to a new place
	virtual int SaveFileAs(const std::string& rNewFileName);


	//set current selected widget
	void	SetSelectedWidget( const std::string& rWidget );
	void	SetSelectedWidget( guiex::CGUIWidget* pWidget );

	//set screen size
	void	SetScreenSize( int width, int height );

	//get current selected widget
	guiex::CGUIWidget*	GetSelectedWidget();

protected:
	DECLARE_ABSTRACT_CLASS(WxEditorCanvasContainer)


protected:
	WxGLCanvas*		m_pCanvas;



	//DECLARE_DYNAMIC_CLASS(MyCanvas)
	DECLARE_EVENT_TABLE()
};










//============================================================================//
// class
//============================================================================// 
class WxGLCanvas : public wxGLCanvas
{
	friend class WxEditorCanvasContainer;

public:
	WxGLCanvas( wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("EditorCanvas") );

	~WxGLCanvas();

	//initialize canvas
	void	InitializeCanvas();

	//update window's box position
	void	UpdateWindowBox();

	//save widget to file, return 0 for successful
	int		SaveToFile( const std::string& rFileName );

	//select widget
	void	SelectWidget( guiex::CGUIWidget* pWidget );

public:
	//event
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);



protected:
	void Render();
	void DrawResizers();

	void UpdateCanvasSize( const wxSize& rSize);
	void UpdateCursor() ;

	void HandleMouseMoved (int aMouseX, int aMouseY);

	int	 SaveWidgetNodeToDoc( guiex::CGUIWidget* pWidget, TiXmlDocument& rDoc );
	TiXmlElement*	GetElementByName( const wxString& rElementName,const wxString& rAttrName, TiXmlElement* pParentElement );
	void AddTopPropertyElement( const guiex::CGUIProperty* pProperty, TiXmlElement* pWidgetNode);
	void AddSubPropertyElement( const guiex::CGUIProperty* pProperty, TiXmlElement* pParentElement);

private:
	wxTimer m_timer;
	//guiex::IGUIInterfaceMouse*	m_pMouse;
	CWindowBox			m_aWindowBox;

	/** Resize point above which the mouse cursor is. */
	int m_hoveredResizePoint;
	int m_previousHoveredResizePoint;

	/** Is the mouse currently pressed? */
	bool  m_mousePressed ;

	/** Window above which the mouse cursor is. */
	guiex::CGUIWidget*	m_hoveredWindow;
	
	/** Current mouseX position. Integer because it's as accurate as it gets.*/
	int   m_mouseX ;

	/** Current mouseX position. Integer because it's as accurate as it gets.*/
	int   m_mouseY ;

	/** Previous mouseX position, to see differences. Integer because it's as accurate as it gets.*/
	int   m_previousMouseX ;

	/** Previous mouseY position, to see differences. Integer because it's as accurate as it gets.*/
	int   m_previousMouseY ;

	//is this widget's status changed
	bool			m_bWidgetStatusChanged;

	//xml file name, not include path
	wxString		rFilename;

	WxEditorCanvasContainer*	m_pContainer;

	DECLARE_EVENT_TABLE()
};






#endif //__KEN_OPENGL_CANVAS_20070911_H__

