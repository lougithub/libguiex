/** 
* @file editorcanvas.h
* @brief opengl canvas for rendering
* @author ken
* @date 2007-09-11
*/

#ifndef __KEN_OPENGL_CANVASEDITOR_20070911_H__
#define __KEN_OPENGL_CANVASEDITOR_20070911_H__

//============================================================================//
// include
//============================================================================// 
#include "savefilebase.h"
#include "windowbox.h"

#include <wx/wxprec.h>
#include <tinyxml.h>

//============================================================================//
// declare
//============================================================================// 
class WxEditorCanvas;


//============================================================================//
// class
//============================================================================// 
class WxEditorCanvasContainer : public wxScrolledWindow, public CSaveFileBase
{
public:
	WxEditorCanvasContainer( wxWindow *parent, const std::string& rFilename );
	~WxEditorCanvasContainer(){}

	//update window's box shown in screen
	void UpdateWindowBox();

	//save file to a new place
	virtual int SaveFileAs(const std::string& rNewFileName);

	//set current selected widget
	void SetSelectedWidget( const std::string& rWidget );
	void SetSelectedWidget( CGUIWidget* pWidget );
	CGUIWidget*	GetSelectedWidget();

	//set screen size
	void SetScreenSize( int width, int height );

	void RenderEditorInfo( );

protected:
	int SaveWidgetNodeToDoc( CGUIWidget* pWidget, TiXmlDocument& rDoc );
	TiXmlElement* GetElementByName( const wxString& rElementName,const wxString& rAttrName, TiXmlElement* pParentElement );
	void AddTopPropertyElement( const CGUIWidget* pWidget, const CGUIProperty& rPropertyTemplate, const CGUIProperty& rProperty, TiXmlElement* pWidgetNode);
	void AddSubPropertyElement( const CGUIProperty& rProperty, TiXmlElement* pParentElement);

protected:
	WxEditorCanvas*	m_pCanvas;

	DECLARE_EVENT_TABLE()
};

#endif //__KEN_OPENGL_CANVASEDITOR_20070911_H__
