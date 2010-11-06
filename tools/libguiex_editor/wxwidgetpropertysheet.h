/** 
* @file widgetpropertysheet.h
* @brief property sheet used in editor, for widget's config
* @author ken
* @date 2007-09-20
*/

#ifndef __KEN_PROPERTY_SHEET_20070920_H__
#define __KEN_PROPERTY_SHEET_20070920_H__


//============================================================================//
// include
//============================================================================// 


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
	class CGUIProperty;
}
class CProperty;



//============================================================================//
// class
//============================================================================// 

using namespace guiex;
// -----------------------------------------------------------------------
// wxGUISizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(wxGUISizeVariantData, CGUISize, wxPG_NO_DECL)

class wxGUISizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGUISizeProperty)
public:

    wxGUISizeProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const CGUISize& value = CGUISize() );
    virtual ~wxGUISizeProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
};

// -----------------------------------------------------------------------
// wxGUIVector2Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(wxGUIVector2VariantData, CGUIVector2, wxPG_NO_DECL)

class wxGUIVector2Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGUIVector2Property)
public:

    wxGUIVector2Property( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const CGUIVector2& value = CGUIVector2() );
    virtual ~wxGUIVector2Property();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
};


// -----------------------------------------------------------------------
// wxGUIRectProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(wxGUIRectVariantData, CGUIRect, wxPG_NO_DECL)

class wxGUIRectProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGUIRectProperty)
public:

    wxGUIRectProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const CGUIRect& value = CGUIRect() );
    virtual ~wxGUIRectProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
};

// -----------------------------------------------------------------------
// wxGUIStringInfoProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(wxGUIStringInfoVariantData, CGUIStringInfo, wxPG_NO_DECL)

class wxGUIStringInfoProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGUIStringInfoProperty)
public:

    wxGUIStringInfoProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const CGUIStringInfo& value = CGUIStringInfo() );
    virtual ~wxGUIStringInfoProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
};


// -----------------------------------------------------------------------
// wxGUIWidgetPositionProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(wxGUIWidgetPositionVariantData, CGUIWidgetPosition, wxPG_NO_DECL)

class wxGUIWidgetPositionProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGUIWidgetPositionProperty)
public:

    wxGUIWidgetPositionProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
                        const CGUIWidgetPosition& value = CGUIWidgetPosition());
    virtual ~wxGUIWidgetPositionProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
	wxArrayString m_arrEnums;
};





// -----------------------------------------------------------------------
// wxGUIWidgetSizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(wxGUIWidgetSizeVariantData, CGUIWidgetSize, wxPG_NO_DECL)

class wxGUIWidgetSizeProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxGUIWidgetSizeProperty)
public:

    wxGUIWidgetSizeProperty( const wxString& label = wxPG_LABEL,
                        const wxString& name = wxPG_LABEL,
						const CGUIWidgetSize& value = CGUIWidgetSize() );
    virtual ~wxGUIWidgetSizeProperty();

    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
	wxArrayString m_arrEnums;
};


// -----------------------------------------------------------------------

class WxWidgetPropertySheet : public wxPropertyGridManager
{
public:
	WxWidgetPropertySheet( wxWindow *  parent,  
		wxWindowID  id = wxID_ANY,  
		const wxPoint &  pos = wxDefaultPosition,  
		const wxSize &  size = wxDefaultSize,  
		long  style = 
            // passed to the embedded wxPropertyGrid.
            wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|
            // Include toolbar.
            wxPG_TOOLBAR |
            // Include description box.
            wxPG_DESCRIPTION |
            // Plus defaults.
            wxPGMAN_DEFAULT_STYLE
	);


	//set related widget to property grid
	void			SetPropGridWidget(guiex::CGUIWidget* pWidget);
	
	//reset property grid
	void			ResetContent();

	//called when property is changed
	int				OnPropertyChanged( wxPropertyGridEvent& event );

	//update grid when size or position is changed
	void			UpdateWidgetSizeAndPos();


protected:

protected:
	guiex::CGUIWidget*	m_pPropWidget;


};


#endif	//__KEN_PROPERTY_SHEET_20070920_H__
