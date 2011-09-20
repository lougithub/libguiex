/** 
* @file wxwidgetpropertygridextend.h.h.h
* @brief property sheet used in editor, for widget's config
* @author ken
* @date 2007-09-20
*/

#ifndef __KEN_PROPERTY_GRID_EXTEND_20070920_H__
#define __KEN_PROPERTY_GRID_EXTEND_20070920_H__


//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <libguiex_core/guiex.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

using namespace guiex;



//============================================================================//
// class
//============================================================================// 
// -----------------------------------------------------------------------
// WxGUISizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUISize )
class WxGUISizeProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUISizeProperty)
public:
	WxGUISizeProperty( const wxString& label = wxPG_LABEL, const CGUISize& value = CGUISize() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


// -----------------------------------------------------------------------
// WxGUIVector2Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIVector2 )
class WxGUIVector2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIVector2Property)
public:
	WxGUIVector2Property( const wxString& label = wxPG_LABEL, const CGUIVector2& value = CGUIVector2() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


// -----------------------------------------------------------------------
// WxGUIVector3Property
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIVector3 )
class WxGUIVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIVector3Property)
public:
	WxGUIVector3Property( const wxString& label = wxPG_LABEL, const CGUIVector3& value = CGUIVector3() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

// -----------------------------------------------------------------------
// WxGUIRotatorProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIRotator )
class WxGUIRotatorProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIRotatorProperty)
public:
	WxGUIRotatorProperty( const wxString& label = wxPG_LABEL, const CGUIRotator& value = CGUIRotator() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

// -----------------------------------------------------------------------
// WxGUIRectProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIRect )
class WxGUIRectProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIRectProperty)
public:
	WxGUIRectProperty( const wxString& label = wxPG_LABEL, const CGUIRect& value = CGUIRect() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


// -----------------------------------------------------------------------
// WxGUIStringInfoProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIStringRenderInfo )
class WxGUIStringInfoProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIStringInfoProperty)
public:
	WxGUIStringInfoProperty( const wxString& label = wxPG_LABEL, const CGUIStringRenderInfo& value = CGUIStringRenderInfo() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


// -----------------------------------------------------------------------
// WxGUIWidgetPositionProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIWidgetPosition )
class WxGUIWidgetPositionProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIWidgetPositionProperty)
public:
	WxGUIWidgetPositionProperty( const wxString& label = wxPG_LABEL, const CGUIWidgetPosition& value = CGUIWidgetPosition());
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();

protected:
	wxArrayString m_arrEnums;
};


// -----------------------------------------------------------------------
// wxGUIColorProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIColor )
class WxGuiColorProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGuiColorProperty)
public:
	WxGuiColorProperty( const wxString& label = wxPG_LABEL, const CGUIColor& value = CGUIColor() );
	virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
	virtual wxString ValueToString( wxVariant& value, int argFlags ) const;
	virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const;

	virtual wxSize OnMeasureImage( int item ) const;
	virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
};



// -----------------------------------------------------------------------
// WxGUILocalizedStringProperty
// -----------------------------------------------------------------------
class WxGUILocalizedStringProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUILocalizedStringProperty)
public:
	WxGUILocalizedStringProperty( const wxString& label = wxPG_LABEL, const wxString& rLocalizedString=wxEmptyString );
	virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
	virtual wxString ValueToString( wxVariant& value, int argFlags ) const;
	virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const;
};


// -----------------------------------------------------------------------
// WxGUIImageProperty
// -----------------------------------------------------------------------
class WxGUIImageProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIImageProperty)
public:
	WxGUIImageProperty( const wxString& label = wxPG_LABEL, const wxString& rImage=wxEmptyString );
	virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
	virtual void OnSetValue();
	virtual wxString ValueToString( wxVariant& value, int argFlags ) const;
	virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const;

	virtual wxSize OnMeasureImage( int item ) const;
	virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
};



// -----------------------------------------------------------------------
// WxGUIAnimationProperty
// -----------------------------------------------------------------------
class WxGUIAnimationProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIAnimationProperty)
public:
	WxGUIAnimationProperty( const wxString& label = wxPG_LABEL, const wxString& rImage=wxEmptyString );
	virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
	virtual void OnSetValue();
	virtual wxString ValueToString( wxVariant& value, int argFlags ) const;
	virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const;

	virtual wxSize OnMeasureImage( int item ) const;
	virtual void OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata );
};

// -----------------------------------------------------------------------
// WxGUISoundProperty
// -----------------------------------------------------------------------
class WxGUISoundProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUISoundProperty)
public:
	WxGUISoundProperty( const wxString& label = wxPG_LABEL, const wxString& rSound=wxEmptyString );
	virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
	virtual void OnSetValue();
	virtual wxString ValueToString( wxVariant& value, int argFlags ) const;
	virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const;
};


// -----------------------------------------------------------------------
// WxGUIWidgetSizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA( CGUIWidgetSize )
class WxGUIWidgetSizeProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIWidgetSizeProperty)
public:
	WxGUIWidgetSizeProperty( const wxString& label = wxPG_LABEL, const CGUIWidgetSize& value = CGUIWidgetSize() );
	virtual wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();

protected:
	wxArrayString m_arrEnums;
};


// -----------------------------------------------------------------------
// WxGUIFontProperty
// -----------------------------------------------------------------------
class WxGUIFontProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxGUIFontProperty)
public:
	WxGUIFontProperty( const wxString& label = wxPG_LABEL, guiex::uint16 uFontID = 0 );
	virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
	virtual void OnSetValue();
	virtual wxString ValueToString( wxVariant& value, int argFlags ) const;
	virtual bool StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const;
};


#endif	//__KEN_PROPERTY_GRID_EXTEND_20070920_H__
