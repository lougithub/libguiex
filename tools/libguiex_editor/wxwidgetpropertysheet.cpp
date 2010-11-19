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

int GetTypeIndexInScreenValueEnum( guiex::EScreenValue eValue, const wxArrayString& arrEnums )
{
	guiex::CGUIString strValue;
	guiex::ValueToString( eValue, strValue );
	for( int i=0; i<int(arrEnums.size()); ++i )
	{
		if( arrEnums[i] == STRING_M2W( strValue ))
		{
			return i;
		}
	}

	throw guiex::CGUIException("unknown EScreenValue : <%d>", eValue);
	return 0;
}

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
// wxGUIVector3Property
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIVector3VariantData, CGUIVector3)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIVector3Property,wxPGProperty,
							   CGUIVector3,const CGUIVector3&,TextCtrl)


wxGUIVector3Property::wxGUIVector3Property( const wxString& label,
							   const wxString& name,
							   const CGUIVector3& value)
							   : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( CGUIVector3ToVariant(value) );
	AddPrivateChild( new wxFloatProperty(wxT("x"), wxT("X"),value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("y"), wxT("Y"),value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("z"), wxT("z"),value.z) );
}

wxGUIVector3Property::~wxGUIVector3Property() { }

void wxGUIVector3Property::RefreshChildren()
{
	if ( !GetCount() )
	{
		return;
	}

	CGUIVector3& vector = CGUIVector3FromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y);
	Item(2)->SetValue( vector.z);
}

void wxGUIVector3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIVector3& vector = CGUIVector3FromVariant(thisValue);
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	case 2: vector.z = childValue.GetDouble(); break;
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

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIStringInfoVariantData, CGUIStringInfo)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIStringInfoProperty,wxPGProperty,
                               CGUIStringInfo,const CGUIStringInfo&,TextCtrl)


wxGUIStringInfoProperty::wxGUIStringInfoProperty( const wxString& label,
                                                  const wxString& name,
                                                  const CGUIStringInfo& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUIStringInfoToVariant(value) );
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

    CGUIStringInfo& string_info = CGUIStringInfoFromVariant(m_value);
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
    CGUIStringInfo& string_info = CGUIStringInfoFromVariant(thisValue);
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

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIWidgetPositionVariantData, CGUIWidgetPosition)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIWidgetPositionProperty,wxPGProperty,
                               CGUIWidgetPosition,const CGUIWidgetPosition&,TextCtrl)


wxGUIWidgetPositionProperty::wxGUIWidgetPositionProperty( const wxString& label,
                                                  const wxString& name,
                                                  const CGUIWidgetPosition& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUIWidgetPositionToVariant(value) );
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums ) );
	AddPrivateChild( new wxGUIVector2Property(wxT("value"), wxT("value"),value.m_aValue) );

	RefreshChildren();
}

wxGUIWidgetPositionProperty::~wxGUIWidgetPositionProperty() { }


void wxGUIWidgetPositionProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    CGUIWidgetPosition& widget_pos = CGUIWidgetPositionFromVariant(m_value);
	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( widget_pos.m_eType, m_arrEnums ) );
	Item(1)->SetValue( CGUIVector2ToVariant(widget_pos.m_aValue));
}

void wxGUIWidgetPositionProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    CGUIWidgetPosition& widget_pos = CGUIWidgetPositionFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0:
		guiex::StringToValue( wxConvUTF8.cWC2MB( m_arrEnums[childValue.GetLong()].c_str()).data(), widget_pos.m_eType );
		break;
	case 1: 
		widget_pos.m_aValue= CGUIVector2FromVariant( childValue ); 
		break;
    }
}






// -----------------------------------------------------------------------
// wxGUIWidgetSizeProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_VARIANT_DATA(wxGUIWidgetSizeVariantData, CGUIWidgetSize)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIWidgetSizeProperty,wxPGProperty,
                               CGUIWidgetSize,const CGUIWidgetSize&,TextCtrl)


wxGUIWidgetSizeProperty::wxGUIWidgetSizeProperty( const wxString& label,
                                                  const wxString& name,
												  const CGUIWidgetSize& value)
    : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
    SetValue( CGUIWidgetSizeToVariant(value) );
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums) );
	AddPrivateChild( new wxGUISizeProperty(wxT("size"), wxT("size"),value.m_aValue) );

	RefreshChildren();
}

wxGUIWidgetSizeProperty::~wxGUIWidgetSizeProperty() { }

void wxGUIWidgetSizeProperty::RefreshChildren()
{
    if ( !GetCount() )
	{
		return;
	}

    CGUIWidgetSize& widget_size = CGUIWidgetSizeFromVariant(m_value);
	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( widget_size.m_eType, m_arrEnums ) );
	Item(1)->SetValue( CGUISizeToVariant(widget_size.m_aValue));
}

void wxGUIWidgetSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
    CGUIWidgetSize& widget_size = CGUIWidgetSizeFromVariant(thisValue);
    switch ( childIndex )
    {
	case 0: 
		guiex::StringToValue( wxConvUTF8.cWC2MB( m_arrEnums[childValue.GetLong()].c_str()).data(), widget_size.m_eType );
		break;
	case 1: 
		widget_size.m_aValue = CGUISizeFromVariant( childValue ); 
		break;
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
	const guiex::CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet(m_pPropWidget->GetType());
	const guiex::CGUIProperty* pPropSize = rSet.GetProperty("size");
	SetPropertyByType( this, NULL, pPropSize, m_pPropWidget );
	const guiex::CGUIProperty* pPropPos = rSet.GetProperty("position");
	SetPropertyByType( this, NULL, pPropPos, m_pPropWidget );

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

	guiex::CGUIProperty aGuiProperty;
	GenerateGUIProperty(this, pPGTop, aGuiProperty);

	int nHasError = 0;
	try
	{ 
		m_pPropWidget->InsertProperty( aGuiProperty );
		m_pPropWidget->ProcessProperty(aGuiProperty);
		m_pPropWidget->Refresh();
		LoadWidgetConfig( this, m_pPropWidget->GetType(), m_pPropWidget );
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


