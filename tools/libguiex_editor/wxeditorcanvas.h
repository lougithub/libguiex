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
#include "windowbox.h"
#include <wx/wxprec.h>
#include <wx/glcanvas.h>

//============================================================================//
// class
//============================================================================// 
class WxEditorCanvas : public wxGLCanvas, public sigslot::has_slots<>
{
	friend class WxEditorCanvasContainer;

public:
	WxEditorCanvas( wxWindow *parent, int* args,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("EditorCanvas") );
	~WxEditorCanvas();

	//initialize canvas
	void InitializeCanvas();
	void DestroyCanvas();

	//update window's box position
	void UpdateWindowBox();

	//select widget
	void SelectWidget( CGUIWidget* pWidget );

	void OnWidgetDestroyed(CGUIWidget* pWidget);

	void UpdateCanvasSize( const wxSize& rSize);

	void RenderEditorInfo( );

public:
	//event
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
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

	void UpdateCursor() ;

	void HandleMouseMoved (int aMouseX, int aMouseY);

	void SetHoveredWindow( CGUIWidget* pWidget );

private:
	wxTimer m_timer;
	//IGUIInterfaceMouse*	m_pMouse;
	CWindowBox m_aWindowBox;

	/** Resize point above which the mouse cursor is. */
	int m_hoveredResizePoint;
	int m_previousHoveredResizePoint;

	/** Is the mouse currently pressed? */
	bool m_mousePressed ;

	/** Window above which the mouse cursor is. */
	CGUIWidget*	m_hoveredWindow;
	
	/** Current mouseX position. Integer because it's as accurate as it gets.*/
	int m_mouseX ;

	/** Current mouseX position. Integer because it's as accurate as it gets.*/
	int m_mouseY ;

	/** Previous mouseX position, to see differences. Integer because it's as accurate as it gets.*/
	int m_previousMouseX ;

	/** Previous mouseY position, to see differences. Integer because it's as accurate as it gets.*/
	int m_previousMouseY ;

	//is this widget's status changed
	bool m_bWidgetStatusChanged;

	//xml file name, not include path
	wxString rFilename;

	WxEditorCanvasContainer* m_pContainer;

	DECLARE_EVENT_TABLE()
};


#endif //__KEN_OPENGL_CANVAS_20070911_H__
