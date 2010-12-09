/** 
* @file widgetpropertysheet.h
* @brief property sheet used in editor, for widget's config
* @author ken
* @date 2007-09-20
*/

#ifndef __KEN_PROPERTY_SHEET_20070920_H__
#define __KEN_PROPERTY_SHEET_20070920_H__

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
WX_PG_DECLARE_VARIANT_DATA( CGUISize )

class wxGUISizeProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUISizeProperty)
public:
	wxGUISizeProperty( 
		const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUISize& value = CGUISize() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

// -----------------------------------------------------------------------
// wxGUIVector2Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIVector2 )
class wxGUIVector2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUIVector2Property)
public:
	wxGUIVector2Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIVector2& value = CGUIVector2() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


// -----------------------------------------------------------------------
// wxGUIVector3Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIVector3 )

class wxGUIVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUIVector3Property)
public:
	wxGUIVector3Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIVector3& value = CGUIVector3() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

// -----------------------------------------------------------------------
// wxGUIRectProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIRect )

class wxGUIRectProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUIRectProperty)
public:

	wxGUIRectProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIRect& value = CGUIRect() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

// -----------------------------------------------------------------------
// wxGUIStringInfoProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIStringInfo )

class wxGUIStringInfoProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUIStringInfoProperty)
public:
	wxGUIStringInfoProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIStringInfo& value = CGUIStringInfo() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


// -----------------------------------------------------------------------
// wxGUIWidgetPositionProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIWidgetPosition )

class wxGUIWidgetPositionProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUIWidgetPositionProperty)
public:

	wxGUIWidgetPositionProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIWidgetPosition& value = CGUIWidgetPosition());

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

protected:
	wxArrayString m_arrEnums;
};

// -----------------------------------------------------------------------
// wxGUIColorProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIColor )
class wxGuiColorProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGuiColorProperty)
public:
	wxGuiColorProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIColor& value = CGUIColor() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();
};



// -----------------------------------------------------------------------
// wxGUIWidgetSizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIWidgetSize )

class wxGUIWidgetSizeProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxGUIWidgetSizeProperty)
public:

	wxGUIWidgetSizeProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CGUIWidgetSize& value = CGUIWidgetSize() );

	virtual wxVariant ChildChanged( 
		wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

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
