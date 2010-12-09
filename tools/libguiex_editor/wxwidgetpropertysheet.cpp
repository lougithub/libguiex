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
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUISize );
WX_PG_IMPLEMENT_PROPERTY_CLASS( wxGUISizeProperty,wxPGProperty,CGUISize,const CGUISize&,TextCtrl);
wxGUISizeProperty::wxGUISizeProperty( const wxString& label, const wxString& name,const CGUISize& value)
: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("width"), wxT("WIDTH"),value.m_fWidth) );
	AddPrivateChild( new wxFloatProperty(wxT("height"), wxT("HEIGHT"),value.m_fHeight) );
}

void wxGUISizeProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUISize& size = CGUISizeRefFromVariant(m_value);
	Item(0)->SetValue( size.m_fWidth );
	Item(1)->SetValue( size.m_fHeight );
}

wxVariant wxGUISizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUISize& size = CGUISizeRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: size.m_fWidth = childValue.GetDouble(); break;
	case 1: size.m_fHeight = childValue.GetDouble(); break;
	}

	wxVariant newVariant;
	newVariant << size;
	return newVariant;
}



// -----------------------------------------------------------------------
// wxGUIVector2Property
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIVector2 );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIVector2Property,wxPGProperty,CGUIVector2,const CGUIVector2&,TextCtrl);
wxGUIVector2Property::wxGUIVector2Property( const wxString& label,const wxString& name,const CGUIVector2& value)
: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("x"), wxT("X"),value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("y"), wxT("Y"),value.y) );
}

void wxGUIVector2Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIVector2& vector = CGUIVector2RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y);
}

wxVariant wxGUIVector2Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIVector2& vector = CGUIVector2RefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	}

	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}


// -----------------------------------------------------------------------
// wxGUIVector3Property
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIVector3 );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIVector3Property,wxPGProperty,CGUIVector3,const CGUIVector3&,TextCtrl);
wxGUIVector3Property::wxGUIVector3Property( 
	const wxString& label,
	const wxString& name,
	const CGUIVector3& value)
	: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("x"), wxT("X"),value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("y"), wxT("Y"),value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("z"), wxT("z"),value.z) );
}

void wxGUIVector3Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIVector3& vector = CGUIVector3RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y);
	Item(2)->SetValue( vector.z);
}

wxVariant wxGUIVector3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIVector3& vector = CGUIVector3RefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	case 2: vector.z = childValue.GetDouble(); break;
	}

	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}

// -----------------------------------------------------------------------
// wxGUIRectProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIRect );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIRectProperty,wxPGProperty,CGUIVector2,const CGUIVector2&,TextCtrl);
wxGUIRectProperty::wxGUIRectProperty( const wxString& label,
									 const wxString& name,
									 const CGUIRect& value)
									 : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("left"), wxT("LEFT"),value.m_fLeft) );
	AddPrivateChild( new wxFloatProperty(wxT("top"), wxT("TOP"),value.m_fTop) );
	AddPrivateChild( new wxFloatProperty(wxT("right"), wxT("RIGHT"),value.m_fRight) );
	AddPrivateChild( new wxFloatProperty(wxT("bottom"), wxT("BOTTOM"),value.m_fBottom) );
}

void wxGUIRectProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIRect& rect = CGUIRectRefFromVariant(m_value);
	Item(0)->SetValue( rect.m_fLeft );
	Item(1)->SetValue( rect.m_fTop);
	Item(2)->SetValue( rect.m_fRight);
	Item(3)->SetValue( rect.m_fBottom);
}

wxVariant wxGUIRectProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIRect& rect = CGUIRectRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: rect.m_fLeft = childValue.GetDouble(); break;
	case 1: rect.m_fTop = childValue.GetDouble(); break;
	case 2: rect.m_fRight = childValue.GetDouble(); break;
	case 3: rect.m_fBottom = childValue.GetDouble(); break;
	}

	wxVariant newVariant;
	newVariant << rect;
	return newVariant;
}





// -----------------------------------------------------------------------
// wxGUIStringInfoProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIStringInfo );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIStringInfoProperty,wxPGProperty,CGUIStringInfo,const CGUIStringInfo&,TextCtrl);


wxGUIStringInfoProperty::wxGUIStringInfoProperty( 
	const wxString& label,
	const wxString& name,
	const CGUIStringInfo& value)
	: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxUIntProperty(wxT("size"), wxT("SIZE"), value.m_nFontSize) );
	AddPrivateChild( new wxUIntProperty(wxT("index"), wxT("FONT_INDEX"),value.m_nFontIdx) );
	AddPrivateChild( new wxGuiColorProperty(wxT("color"), wxT("COLOR"), value.m_aColor) );
}

void wxGUIStringInfoProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIStringInfo& string_info = CGUIStringInfoRefFromVariant(m_value);
	Item(0)->SetValue( string_info.m_nFontSize );
	Item(1)->SetValue( string_info.m_nFontIdx);
	Item(2)->SetValue( WXVARIANT( string_info.m_aColor ));
}

wxVariant wxGUIStringInfoProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIStringInfo& string_info = CGUIStringInfoRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: string_info.m_nFontSize = childValue.GetInteger(); break;
	case 1: string_info.m_nFontIdx = childValue.GetInteger(); break;
	case 2: string_info.m_aColor << childValue; break;
	}

	wxVariant newVariant;
	newVariant << string_info;
	return newVariant;
}

// -----------------------------------------------------------------------
// wxGUIWidgetPositionProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIWidgetPosition );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIWidgetPositionProperty,wxPGProperty,CGUIWidgetPosition,const CGUIWidgetPosition&,TextCtrl);
wxGUIWidgetPositionProperty::wxGUIWidgetPositionProperty( 
	const wxString& label,
	const wxString& name,
	const CGUIWidgetPosition& value)
	: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums ) );
	AddPrivateChild( new wxGUIVector2Property(wxT("value"), wxT("value"),value.m_aValue) );

	RefreshChildren();
}


void wxGUIWidgetPositionProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIWidgetPosition& widget_pos = CGUIWidgetPositionRefFromVariant(m_value);
	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( widget_pos.m_eType, m_arrEnums ) );
	Item(1)->SetValue( WXVARIANT(widget_pos.m_aValue));
}

wxVariant wxGUIWidgetPositionProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIWidgetPosition& widget_pos = CGUIWidgetPositionRefFromVariant( thisValue );

	switch ( childIndex )
	{
	case 0:
		guiex::StringToValue( wxConvUTF8.cWC2MB( m_arrEnums[childValue.GetLong()].c_str()).data(), widget_pos.m_eType );
		break;
	case 1: 
		widget_pos.m_aValue << childValue; 
		break;
	}

	wxVariant newVariant;
	newVariant << widget_pos;
	return newVariant;
}


// -----------------------------------------------------------------------
// wxGUIColorProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIColor );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGuiColorProperty,wxPGProperty,CGUIColor,const CGUIColor&,TextCtrlAndButton);
wxGuiColorProperty::wxGuiColorProperty( const wxString& label, const wxString& name,const CGUIColor& value)
: wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxIntProperty(wxT("red"), wxT("RED"),value.GetRed()*255) );
	AddPrivateChild( new wxIntProperty(wxT("green"), wxT("GREEN"),value.GetGreen()*255) );
	AddPrivateChild( new wxIntProperty(wxT("blue"), wxT("BLUE"),value.GetBlue()*255) );
	AddPrivateChild( new wxIntProperty(wxT("alpha"), wxT("ALPHA"),value.GetAlpha()*255) );
}

void wxGuiColorProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUIColor& color = CGUIColorRefFromVariant(m_value);

	Item(0)->SetValue( color.GetRed()*255 );
	Item(1)->SetValue( color.GetGreen()*255 );
	Item(2)->SetValue( color.GetBlue()*255 );
	Item(3)->SetValue( color.GetAlpha()*255 );
}

wxVariant wxGuiColorProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIColor& color = CGUIColorRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: color.SetRed( childValue.GetInteger() / 255.0f ); break;
	case 1: color.SetGreen( childValue.GetInteger() / 255.0f ); break;
	case 2: color.SetBlue( childValue.GetInteger() / 255.0f ); break;
	case 3: color.SetAlpha( childValue.GetInteger() / 255.0f ); break;
	}

	wxVariant newVariant;
	newVariant << color;
	return newVariant;
}



// -----------------------------------------------------------------------
// wxGUIWidgetSizeProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIWidgetSize );
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxGUIWidgetSizeProperty,wxPGProperty,CGUIWidgetSize,const CGUIWidgetSize&,TextCtrl);
wxGUIWidgetSizeProperty::wxGUIWidgetSizeProperty( const wxString& label,
												 const wxString& name,
												 const CGUIWidgetSize& value)
												 : wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums) );
	AddPrivateChild( new wxGUISizeProperty(wxT("size"), wxT("size"),value.m_aValue) );

	RefreshChildren();
}

void wxGUIWidgetSizeProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIWidgetSize& widget_size = CGUIWidgetSizeRefFromVariant(m_value);
	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( widget_size.m_eType, m_arrEnums ) );
	Item(1)->SetValue( WXVARIANT(widget_size.m_aValue));
}

wxVariant wxGUIWidgetSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIWidgetSize& widget_size = CGUIWidgetSizeRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: 
		guiex::StringToValue( wxConvUTF8.cWC2MB( m_arrEnums[childValue.GetLong()].c_str()).data(), widget_size.m_eType );
		break;
	case 1: 
		widget_size.m_aValue << childValue; 
		break;
	}

	wxVariant newVariant;
	newVariant << widget_size;
	return newVariant;
}


//------------------------------------------------------------------------------
WxWidgetPropertySheet::WxWidgetPropertySheet( 
	wxWindow * parent, 
	wxWindowID id, 
	const wxPoint& pos,
	const wxSize& size, 
	long style )
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
void WxWidgetPropertySheet::ResetContent()
{
	m_pPropWidget = NULL;
	ClearPage(NOTEBOOK_PAGE_PROP);
	ClearPage(NOTEBOOK_PAGE_IMAGE);
	ClearPage(NOTEBOOK_PAGE_EVENT);
	Refresh();
}

//------------------------------------------------------------------------------
void WxWidgetPropertySheet::UpdateWidgetSizeAndPos()
{
	const guiex::CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet(m_pPropWidget->GetType());
	const guiex::CGUIProperty* pPropSize = rSet.GetProperty("size");
	SetPropertyByType( this, NULL, pPropSize, m_pPropWidget );
	const guiex::CGUIProperty* pPropPos = rSet.GetProperty("position");
	SetPropertyByType( this, NULL, pPropPos, m_pPropWidget );

	Refresh();
}
//------------------------------------------------------------------------------
int WxWidgetPropertySheet::OnPropertyChanged( wxPropertyGridEvent& event )
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
void WxWidgetPropertySheet::SetPropGridWidget(guiex::CGUIWidget* pWidget)
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


