/** 
* @file widgetpropertysheet.cpp
* @brief property sheet used in editor, for widget's config
* @author ken
* @date 2007-09-20
*/

//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"


//============================================================================//
// define
//============================================================================// 



//============================================================================//
// function
//============================================================================// 




// -----------------------------------------------------------------------
// wxGUISizeProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUISizeVariantData, CGUISize)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUISizeProperty,wxPGProperty,
                               CGUISize,const CGUISize&,TextCtrl)


wxGUISizeProperty::wxGUISizeProperty( const wxString& label,
                                                  const wxString& name,
                                                  const CGUISize& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUISizeToVariant(value) );
    AddPrivateChild( new wxFloatProperty(wxT("width"), wxT("WIDTH"),value.m_fWidth) );
    AddPrivateChild( new wxFloatProperty(wxT("height"), wxT("HEIGHT"),value.m_fHeight) );
}

wxGUISizeProperty::~wxGUISizeProperty() { }

void wxGUISizeProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    CGUISize& size = CGUISizeFromVariant(m_value);
	Item(0)->SetValue( size.m_fWidth );
    Item(1)->SetValue( size.m_fHeight );
}

void wxGUISizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    CGUISize& size = CGUISizeFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: size.m_fWidth = childValue.GetDouble(); break;
	case 1: size.m_fHeight = childValue.GetDouble(); break;
    }
}



// -----------------------------------------------------------------------
// wxGUIVector2Property
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIVector2VariantData, CGUIVector2)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIVector2Property,wxPGProperty,
                               CGUIVector2,const CGUIVector2&,TextCtrl)


wxGUIVector2Property::wxGUIVector2Property( const wxString& label,
                                                  const wxString& name,
                                                  const CGUIVector2& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUIVector2ToVariant(value) );
	AddPrivateChild( new wxFloatProperty(wxT("x"), wxT("X"),value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("y"), wxT("Y"),value.y) );
}

wxGUIVector2Property::~wxGUIVector2Property() { }

void wxGUIVector2Property::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    CGUIVector2& vector = CGUIVector2FromVariant(m_value);
	Item(0)->SetValue( vector.x );
    Item(1)->SetValue( vector.y);
}

void wxGUIVector2Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    CGUIVector2& vector = CGUIVector2FromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
    }
}






// -----------------------------------------------------------------------
// wxGUIRectProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIRectVariantData, CGUIRect)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIRectProperty,wxPGProperty,
                               CGUIVector2,const CGUIVector2&,TextCtrl)


wxGUIRectProperty::wxGUIRectProperty( const wxString& label,
                                                  const wxString& name,
                                                  const CGUIRect& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUIRectToVariant(value) );
	AddPrivateChild( new wxFloatProperty(wxT("left"), wxT("LEFT"),value.m_fLeft) );
	AddPrivateChild( new wxFloatProperty(wxT("top"), wxT("TOP"),value.m_fTop) );
	AddPrivateChild( new wxFloatProperty(wxT("right"), wxT("RIGHT"),value.m_fRight) );
	AddPrivateChild( new wxFloatProperty(wxT("bottom"), wxT("BOTTOM"),value.m_fBottom) );
}

wxGUIRectProperty::~wxGUIRectProperty() { }

void wxGUIRectProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    CGUIRect& rect = CGUIRectFromVariant(m_value);
	Item(0)->SetValue( rect.m_fLeft );
    Item(1)->SetValue( rect.m_fTop);
    Item(2)->SetValue( rect.m_fRight);
    Item(3)->SetValue( rect.m_fBottom);
}

void wxGUIRectProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    CGUIRect& rect = CGUIRectFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: rect.m_fLeft = childValue.GetDouble(); break;
	case 1: rect.m_fTop = childValue.GetDouble(); break;
	case 2: rect.m_fRight = childValue.GetDouble(); break;
	case 3: rect.m_fBottom = childValue.GetDouble(); break;
    }
}





// -----------------------------------------------------------------------
// wxGUIStringInfoProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIStringInfoVariantData, CGUIStringExInfo)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIStringInfoProperty,wxPGProperty,
                               CGUIStringExInfo,const CGUIStringExInfo&,TextCtrl)


wxGUIStringInfoProperty::wxGUIStringInfoProperty( const wxString& label,
                                                  const wxString& name,
                                                  const CGUIStringExInfo& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUIStringExInfoToVariant(value) );
	AddPrivateChild( new wxUIntProperty(wxT("size"), wxT("SIZE"), value.m_nFontSize) );
	AddPrivateChild( new wxUIntProperty(wxT("index"), wxT("FONT_INDEX"),value.m_nFontIdx) );

	guiex::CGUIColor aColor = value.m_aColor;
	wxColour aWxColor(aColor.GetRed()*255.f,aColor.GetGreen()*255.f,aColor.GetBlue()*255.f,aColor.GetAlpha()*255.f);
	AddPrivateChild( new wxColourProperty(wxT("color"), wxT("COLOR"), aWxColor) );
}

wxGUIStringInfoProperty::~wxGUIStringInfoProperty() { }

void wxGUIStringInfoProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    CGUIStringExInfo& string_info = CGUIStringExInfoFromVariant(m_value);
	Item(0)->SetValue( string_info.m_nFontSize );
	Item(1)->SetValue( string_info.m_nFontIdx);

	guiex::CGUIColor aColor = string_info.m_aColor;
	wxColour aWxColor(aColor.GetRed()*255.f,aColor.GetGreen()*255.f,aColor.GetBlue()*255.f,aColor.GetAlpha()*255.f);
	wxVariant variant;
	variant << aWxColor;
	Item(2)->SetValue( variant );
}

void wxGUIStringInfoProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    CGUIStringExInfo& string_info = CGUIStringExInfoFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: string_info.m_nFontSize = childValue.GetInteger(); break;
	case 1: string_info.m_nFontIdx = childValue.GetInteger(); break;
	case 2:
		wxColour aWxColor;
		aWxColor << childValue;
		//= wxColourPropertyValueFromVariant( childValue ).m_colour;
		guiex::CGUIColor aColor(aWxColor.Red()/255.f, aWxColor.Green()/255.f, aWxColor.Blue()/255.f, aWxColor.Alpha()/255.f );
		string_info.m_aColor = aColor; 
		break;
    }
}



// -----------------------------------------------------------------------
// wxGUIWidgetPositionProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIWidgetPositionVariantData, SWidgetPosition)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIWidgetPositionProperty,wxPGProperty,
                               SWidgetPosition,const SWidgetPosition&,TextCtrl)


wxGUIWidgetPositionProperty::wxGUIWidgetPositionProperty( const wxString& label,
                                                  const wxString& name,
                                                  const SWidgetPosition& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( SWidgetPositionToVariant(value) );
	AddPrivateChild( new wxBoolProperty(wxT("relative"), wxT("RELATIVE"), value.m_bIsRelative) );
	AddPrivateChild( new wxGUIVector2Property(wxT("position"), wxT("POSITION"),value.m_aPosition) );
}

wxGUIWidgetPositionProperty::~wxGUIWidgetPositionProperty() { }

void wxGUIWidgetPositionProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    SWidgetPosition& widget_pos = SWidgetPositionFromVariant(m_value);
	Item(0)->SetValue( widget_pos.m_bIsRelative );
	Item(1)->SetValue( CGUIVector2ToVariant(widget_pos.m_aPosition));
}

void wxGUIWidgetPositionProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    SWidgetPosition& widget_pos = SWidgetPositionFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: widget_pos.m_bIsRelative = childValue.GetBool(); break;
	case 1: widget_pos.m_aPosition = CGUIVector2FromVariant( childValue ); break;
    }
}






// -----------------------------------------------------------------------
// wxGUIWidgetSizeProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIWidgetSizeVariantData, SWidgetSize)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIWidgetSizeProperty,wxPGProperty,
                               SWidgetSize,const SWidgetSize&,TextCtrl)


wxGUIWidgetSizeProperty::wxGUIWidgetSizeProperty( const wxString& label,
                                                  const wxString& name,
                                                  const SWidgetSize& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( SWidgetSizeToVariant(value) );
	AddPrivateChild( new wxBoolProperty(wxT("relative"), wxT("RELATIVE"), value.m_bIsRelative) );
	AddPrivateChild( new wxGUISizeProperty(wxT("Size"), wxT("SIZE"),value.m_aSize) );
	AddPrivateChild( new wxGUISizeProperty(wxT("MaxSize"), wxT("SIZE"),value.m_aMaxSize) );
	AddPrivateChild( new wxGUISizeProperty(wxT("MinSize"), wxT("SIZE"),value.m_aMinSize) );
}

wxGUIWidgetSizeProperty::~wxGUIWidgetSizeProperty() { }

void wxGUIWidgetSizeProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    SWidgetSize& widget_size = SWidgetSizeFromVariant(m_value);
	Item(0)->SetValue( widget_size.m_bIsRelative );
	Item(1)->SetValue( CGUISizeToVariant(widget_size.m_aSize));
	Item(2)->SetValue( CGUISizeToVariant(widget_size.m_aMaxSize));
	Item(3)->SetValue( CGUISizeToVariant(widget_size.m_aMinSize));
}

void wxGUIWidgetSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    SWidgetSize& widget_size = SWidgetSizeFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: widget_size.m_bIsRelative = childValue.GetBool(); break;
	case 1: widget_size.m_aSize = CGUISizeFromVariant( childValue ); break;
	case 2: widget_size.m_aMaxSize = CGUISizeFromVariant( childValue ); break;
	case 3: widget_size.m_aMinSize = CGUISizeFromVariant( childValue ); break;
    }
}


//------------------------------------------------------------------------------
WxWidgetPropertySheet::WxWidgetPropertySheet( wxWindow * parent, wxWindowID  id, const wxPoint &  pos, const wxSize &  size, long  style )
:wxPropertyGridManager(parent,id, pos,size,style )
,m_pPropWidget(NULL)
{
	//add page
	AddPage(wxT("Property"));
	AddPage(wxT("Image"));
	AddPage(wxT("Event"));
	SelectPage(NOTEBOOK_PAGE_PROP);
	Refresh();
}
//------------------------------------------------------------------------------
void			WxWidgetPropertySheet::ResetContent()
{
	m_pPropWidget = NULL;
	ClearPage(NOTEBOOK_PAGE_PROP);
	ClearPage(NOTEBOOK_PAGE_IMAGE);
	ClearPage(NOTEBOOK_PAGE_EVENT);
	Refresh();
}

//------------------------------------------------------------------------------
void			WxWidgetPropertySheet::UpdateWidgetSizeAndPos()
{
	const CProperty* pSet = CPropertyConfigMgr::Instance()->GetPropertySet(m_pPropWidget->GetType());
	const CProperty* pPropSize = pSet->GetProperty("WIDGET_SIZE");
	SetPropertyByType( this, pPropSize, NULL, m_pPropWidget );
	const CProperty* pPropPos = pSet->GetProperty("WIDGET_POSITION");
	SetPropertyByType( this, pPropPos, NULL, m_pPropWidget );

	Refresh();
}
//------------------------------------------------------------------------------
int			WxWidgetPropertySheet::OnPropertyChanged( wxPropertyGridEvent& event )
{
	if( !m_pPropWidget )
	{
		return 0;
	}
	wxPGProperty* pPGProperty = event.GetProperty();
	if( !pPGProperty)
	{
		return -1;
	}
	wxPGProperty* pPGTop = event.GetMainParent();
	if( !pPGTop)
	{
		return -1;
	}

	guiex::CGUIProperty* pGuiProperty = GenerateGUIProperty(this, pPGTop);
	wxASSERT( pGuiProperty );

	int nHasError = 0;
	try
	{ 
		m_pPropWidget->SetProperty( *pGuiProperty );
		m_pPropWidget->ProcessProperty(pGuiProperty);
	}
	catch(guiex::CGUIBaseException& rError)
	{
		wxMessageBox( wxConvUTF8.cMB2WC( rError.what()), _T("OnPropertyChanged error"), wxICON_ERROR|wxCENTRE);
		nHasError = -1;
	}
	catch(...)
	{
		wxMessageBox(_T("unknown error"), _T("OnPropertyChanged error"), wxICON_ERROR|wxCENTRE);
		nHasError = -1;
	}

	guiex::CGUIPropertyManager::Instance()->DestroyProperty( pGuiProperty );
	return nHasError;
}
//------------------------------------------------------------------------------
void			WxWidgetPropertySheet::SetPropGridWidget(guiex::CGUIWidget* pWidget)
{
	if( m_pPropWidget == pWidget)
	{
		//ignore it
		return;
	}
	else
	{
		ResetContent();
		m_pPropWidget = pWidget;
		if( m_pPropWidget )
		{
			LoadWidgetConfig(this, m_pPropWidget->GetType(), m_pPropWidget);
		}
	}
}
//------------------------------------------------------------------------------






