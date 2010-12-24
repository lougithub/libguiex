/** 
* @file editorcanvas.h
* @brief opengl canvas for rendering
* @author ken
* @date 2007-09-11
*/


//============================================================================//
// include
//============================================================================// 
#include "wxeditorcanvascontainer.h"
#include "wxeditorcanvas.h"
#include "wxmainapp.h"
#include "wxmainframe.h"
#include "editorutility.h"

#include <libguiex_core/guiex.h>
#include <wx/glcanvas.h>
#include <tinyxml.h>

using namespace guiex;

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
	SetVirtualSize( GSystem->GetScreenWidth(), GSystem->GetScreenHeight() );

	SetBackgroundColour( *wxLIGHT_GREY );

	//create canvas
	wxSize aCanvasSize( GSystem->GetScreenWidth(), GSystem->GetScreenHeight());
	int wx_gl_attribs[] = {
		WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 
		WX_GL_DEPTH_SIZE, 24, 
		WX_GL_STENCIL_SIZE, 8,
		0};
	m_pCanvas = new WxEditorCanvas(this, wx_gl_attribs, wxID_ANY, wxDefaultPosition, aCanvasSize);
	m_pCanvas->InitializeCanvas();
	//m_pCanvas->SetNextHandler(  this );
}
//------------------------------------------------------------------------------
void WxEditorCanvasContainer::UpdateWindowBox()
{
	m_pCanvas->UpdateWindowBox();
}
//------------------------------------------------------------------------------
int WxEditorCanvasContainer::SaveFileAs(const std::string& rNewFileName)
{
	//open xml file
	TiXmlDocument aDoc;
	TiXmlElement  aNewToppestNode("guiex");
	aDoc.InsertEndChild(aNewToppestNode);

	//get page
	if( GSystem->GetOpenedPageNum() != 0 )
	{
		CGUIWidget* pWidget = GSystem->GetOpenedPageByIndex(0);
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
	if( false == aDoc.SaveFile( rNewFileName.c_str()))
	{
		//failed to save as file
		return -1;
	}

	return 0;
}
//------------------------------------------------------------------------------
int WxEditorCanvasContainer::SaveWidgetNodeToDoc( CGUIWidget* pWidget, TiXmlDocument& rDoc )
{
	wxASSERT( pWidget );
	if(CGUIWidgetManager::IsInternalWidget( pWidget->GetName()))
	{
		//internal widget should be ignored
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

	pWidgetNode = (TiXmlElement*)pRootNode->InsertEndChild(aNewNode);
	wxASSERT(pWidgetNode);

	//save property to doc
	CGUIProperty aSet = pWidget->GetProperty();

	//process parent first
	if( aSet.HasProperty("parent"))
	{
		CGUIProperty* pProperty = aSet.GetProperty("parent");
		AddTopPropertyElement(*pProperty, pWidgetNode);
		aSet.RemoveProperty(*pProperty);
	}

	//process image
	std::vector<CGUIProperty> aImgVector;
	for( unsigned i=0; i<aSet.GetPropertyNum(); ++i)
	{
		const CGUIProperty* pProperty = aSet.GetProperty(i);
		if( pProperty->GetType() == ePropertyType_Image )
		{
			aImgVector.push_back(*pProperty);
		}
	}
	for( std::vector<CGUIProperty>::iterator itor = aImgVector.begin();
		itor != aImgVector.end();
		++itor )
	{
		AddTopPropertyElement(*itor, pWidgetNode);
		aSet.RemoveProperty(*itor);
	}

	//process left
	for( unsigned i=0; i<aSet.GetPropertyNum(); ++i)
	{
		const CGUIProperty* pProperty = aSet.GetProperty(i);
		AddTopPropertyElement(*pProperty, pWidgetNode);
	}

	//process it's child
	CGUIWidget* pChild = pWidget->GetChild();
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
void WxEditorCanvasContainer::AddTopPropertyElement( const CGUIProperty& rProperty, TiXmlElement* pWidgetNode)
{
	if( rProperty.GetValue().empty() && rProperty.GetPropertyNum() == 0 )
	{
		//empty property, ignore it
		return;
	}

	//get exist's one
	TiXmlElement* pOldNode = GetElementByName(_T("property"), Gui2wxString(rProperty.GetName()), pWidgetNode);

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
void WxEditorCanvasContainer::SetSelectedWidget( const std::string& rWidget )
{
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetWidget( rWidget.c_str(), GetMainFrame()->GetCurrentSceneName());
	SetSelectedWidget(pWidget);
}
//------------------------------------------------------------------------------
void WxEditorCanvasContainer::SetSelectedWidget( CGUIWidget* pWidget )
{
	m_pCanvas->SelectWidget(pWidget);
}
//------------------------------------------------------------------------------
CGUIWidget*	WxEditorCanvasContainer::GetSelectedWidget()
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
TiXmlElement* WxEditorCanvasContainer::GetElementByName( 
	const wxString& rElementName,
	const wxString& rAttrName, 
	TiXmlElement* pParentElement )
{
	//get widget node
	TiXmlElement* pFindNode = NULL;
	TiXmlElement* pNode = pParentElement->FirstChildElement(rElementName.char_str(wxConvUTF8).data());
	while( pNode )
	{
		if( rAttrName == Gui2wxString(pNode->Attribute("name")))
		{
			pFindNode = pNode;
			break;
		}
		pNode = pNode->NextSiblingElement(rElementName.char_str(wxConvUTF8).data());
	}

	return pNode;
}
//------------------------------------------------------------------------------
void WxEditorCanvasContainer::AddSubPropertyElement( const CGUIProperty& rProperty, TiXmlElement* pParentElement)
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

