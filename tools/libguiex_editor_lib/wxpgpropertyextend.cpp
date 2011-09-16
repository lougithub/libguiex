/** 
* @file widgetpropertysheet.cpp
* @brief property sheet used in editor, for widget's config
* @author ken
* @date 2007-09-20
*/

//============================================================================//
// include
//============================================================================// 
#include "wxpgpropertyextend.h"
#include "propertyconfigmgr.h"
#include "toolsmisc.h"
#include "guiresourcepool.h"
#include "wxresourceselectdlg.h"

#include <wx/colordlg.h>

//============================================================================//
// function
//============================================================================// 
int GetTypeIndexInScreenValueEnum( EScreenValue eValue, const wxArrayString& arrEnums )
{
	CGUIString strValue;
	ValueToString( eValue, strValue );
	for( int i=0; i<int(arrEnums.size()); ++i )
	{
		if( arrEnums[i] == Gui2wxString( strValue ))
		{
			return i;
		}
	}

	throw CGUIException("unknown EScreenValue : <%d>", eValue);
	return 0;
}

wxString GetUniquePropName()
{
	static int i = 0;
	return wxString::Format( L"propperty_%d", i++ );
}

// -----------------------------------------------------------------------
// WxGUIPropertyBase
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIProperty );
WxGUIPropertyBase::WxGUIPropertyBase( )
: wxPGProperty( )
{
}
WxGUIPropertyBase::WxGUIPropertyBase( const wxString& label, const CGUIProperty& rValue )
	: wxPGProperty(label,GetUniquePropName())
{
	SetValue( WXVARIANT(rValue) );
}



// -----------------------------------------------------------------------
// WxGUISizeProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUISizeProperty, WxGUIPropertyBase)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN( WxGUISizeProperty,CGUIProperty,TextCtrl);
WxGUISizeProperty::WxGUISizeProperty( )
	: WxGUIPropertyBase( )
{
}
WxGUISizeProperty::WxGUISizeProperty( const wxString& label, const CGUIProperty& rValue )
: WxGUIPropertyBase(label,rValue)
{
	CGUISize aValue = rValue.GetCommonValue<CGUISize>();

	AddPrivateChild( new wxFloatProperty(wxT("width"), wxT("width"), aValue.m_fWidth));
	AddPrivateChild( new wxFloatProperty(wxT("height"), wxT("height"), aValue.m_fHeight));
}
// -----------------------------------------------------------------------
void WxGUISizeProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(m_value);
	CGUISize aValue = rValue.GetCommonValue<CGUISize>();
	Item(0)->SetValue( aValue.m_fWidth );
	Item(1)->SetValue( aValue.m_fHeight );
}
// -----------------------------------------------------------------------
wxVariant WxGUISizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(thisValue);
	CGUISize aValue = rValue.GetCommonValue<CGUISize>();

	switch ( childIndex )
	{
	case 0: aValue.m_fWidth = childValue.GetDouble(); break;
	case 1: aValue.m_fHeight = childValue.GetDouble(); break;
	}
	rValue.SetCommonValue( aValue );

	return rValue;
}
// -----------------------------------------------------------------------



// -----------------------------------------------------------------------
// WxGUIVector2Property
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIVector2Property, wxPGProperty)
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIVector2 );
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIVector2Property,CGUIVector2,TextCtrl);
WxGUIVector2Property::WxGUIVector2Property( const wxString& label,const wxString& name,const CGUIVector2& value)
: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("x"), wxT("X"),value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("y"), wxT("Y"),value.y) );
}
// -----------------------------------------------------------------------
void WxGUIVector2Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIVector2& vector = CGUIVector2RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y);
}
// -----------------------------------------------------------------------
wxVariant WxGUIVector2Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
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


// -----------------------------------------------------------------------
// WxGUIVector3Property
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIVector3Property, wxPGProperty)
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIVector3 );
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIVector3Property,CGUIVector3,TextCtrl);
WxGUIVector3Property::WxGUIVector3Property( 
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
// -----------------------------------------------------------------------
void WxGUIVector3Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIVector3& vector = CGUIVector3RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y);
	Item(2)->SetValue( vector.z);
}
// -----------------------------------------------------------------------
wxVariant WxGUIVector3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
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



// -----------------------------------------------------------------------
// WxGUIRotatorProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIRotatorProperty, wxPGProperty)
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIRotator );
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIRotatorProperty,CGUIRotator,TextCtrl);
WxGUIRotatorProperty::WxGUIRotatorProperty( 
	const wxString& label,
	const wxString& name,
	const CGUIRotator& value)
	: wxPGProperty(label,name)
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("Pitch"), wxT("Pitch"),value.Pitch) );
	AddPrivateChild( new wxFloatProperty(wxT("Yaw"), wxT("Yaw"),value.Yaw) );
	AddPrivateChild( new wxFloatProperty(wxT("Roll"), wxT("Roll"),value.Roll) );
}
// -----------------------------------------------------------------------
void WxGUIRotatorProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIRotator& rotator = CGUIRotatorRefFromVariant(m_value);
	Item(0)->SetValue( rotator.Pitch );
	Item(1)->SetValue( rotator.Yaw);
	Item(2)->SetValue( rotator.Roll);
}
// -----------------------------------------------------------------------
wxVariant WxGUIRotatorProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIRotator& rotator = CGUIRotatorRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: rotator.Pitch = childValue.GetDouble(); break;
	case 1: rotator.Yaw = childValue.GetDouble(); break;
	case 2: rotator.Roll = childValue.GetDouble(); break;
	}

	wxVariant newVariant;
	newVariant << rotator;
	return newVariant;
}
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// WxGUIRectProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIRectProperty, wxPGProperty)
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIRect );
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIRectProperty,CGUIVector2,TextCtrl);
WxGUIRectProperty::WxGUIRectProperty( const wxString& label,
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
// -----------------------------------------------------------------------
void WxGUIRectProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIRect& rect = CGUIRectRefFromVariant(m_value);
	Item(0)->SetValue( rect.m_fLeft );
	Item(1)->SetValue( rect.m_fTop);
	Item(2)->SetValue( rect.m_fRight);
	Item(3)->SetValue( rect.m_fBottom);
}
// -----------------------------------------------------------------------
wxVariant WxGUIRectProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
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


// -----------------------------------------------------------------------
// WxGUIStringInfoProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIStringInfoProperty, WxGUIPropertyBase);
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIStringInfoProperty,CGUIProperty,TextCtrl);
WxGUIStringInfoProperty::WxGUIStringInfoProperty()
	:WxGUIPropertyBase()
{
}
WxGUIStringInfoProperty::WxGUIStringInfoProperty( const wxString& label, const CGUIProperty& rValue )
	:WxGUIPropertyBase( label, rValue)
{
	CGUIStringRenderInfo aValue = rValue.GetCommonValue<CGUIStringRenderInfo>();
	AddPrivateChild( new wxFloatProperty(wxT("scale"), wxT("scale"), aValue.m_fFontScale ));
	AddPrivateChild( new WxGUIFontProperty(wxT("font"), *rValue["font"] ));
	AddPrivateChild( new WxGuiColorProperty(wxT("color"), *rValue["color"] ));
}

void WxGUIStringInfoProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(m_value);
	CGUIStringRenderInfo aValue = rValue.GetCommonValue<CGUIStringRenderInfo>();
	Item(0)->SetValue( aValue.m_fFontScale );
	Item(1)->SetValue( wxVariant(*rValue["font"] ));
	Item(2)->SetValue( wxVariant(*rValue["color"] ));
}
// -----------------------------------------------------------------------
wxVariant WxGUIStringInfoProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(thisValue);
	CGUIStringRenderInfo aValue = rValue.GetCommonValue<CGUIStringRenderInfo>();

	switch ( childIndex )
	{
	case 0: aValue.m_fFontScale = childValue.GetDouble(); break;
	case 1: *rValue["font"] = CGUIPropertyRefFromVariant(childValue); break;
	case 2: *rValue["color"] = CGUIPropertyRefFromVariant(childValue); break;
	}

	return rValue;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// WxGUIWidgetPositionProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIWidgetPositionProperty, WxGUIPropertyBase);
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIWidgetPositionProperty,CGUIProperty,TextCtrl);
WxGUIWidgetPositionProperty::WxGUIWidgetPositionProperty()
	: WxGUIPropertyBase( )
{
}
WxGUIWidgetPositionProperty::WxGUIWidgetPositionProperty(const wxString& label, const CGUIProperty& rValue)
	: WxGUIPropertyBase(label,rValue)
{
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums) );
	AddPrivateChild( new WxGUISizeProperty(wxT("value"), *rValue["value"] ));

	RefreshChildren();
}
// -----------------------------------------------------------------------
void WxGUIWidgetPositionProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(m_value);
	CGUIWidgetPosition aValue = rValue.GetCommonValue<CGUIWidgetPosition>();

	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( aValue.m_eType, m_arrEnums ) );
	Item(1)->SetValue( wxVariant(*rValue["value"]));
}
// -----------------------------------------------------------------------
wxVariant WxGUIWidgetPositionProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(thisValue);
	CGUIWidgetPosition aValue = rValue.GetCommonValue<CGUIWidgetPosition>();

	switch ( childIndex )
	{
	case 0: 
		guiex::StringToValue( wx2GuiString( m_arrEnums[childValue.GetLong()]), aValue.m_eType );
		break;
	case 1: 
		*rValue["value"] = CGUIPropertyRefFromVariant(childValue);
		break;
	}

	return rValue;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// wxGUIColorProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGuiColorProperty, WxGUIPropertyBase)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGuiColorProperty,CGUIProperty,TextCtrlAndButton);
WxGuiColorProperty::WxGuiColorProperty()
	:WxGUIPropertyBase()
{

}
WxGuiColorProperty::WxGuiColorProperty( const wxString& label, const CGUIProperty& rValue )
	:WxGUIPropertyBase(label, rValue)
{
}
// -----------------------------------------------------------------------
wxString WxGuiColorProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(value);
	return Gui2wxString( rValue.GetValue() );
}
// -----------------------------------------------------------------------
bool WxGuiColorProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	try
	{
		CGUIColor color;
		if( 0 != guiex::StringToValue( wx2GuiString( text ), color ) )
		{
			return false;
		}
	}
	catch(...)
	{
		return false;
	}
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(variant);
	rValue.SetValue( wx2GuiString( text ) );
	return true;
}
// -----------------------------------------------------------------------
bool WxGuiColorProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		const CGUIProperty& rValue = CGUIPropertyRefFromVariant(GetValue());
		CGUIColor color = rValue.GetCommonValue<CGUIColor>();
		wxColour aWxColor( 
			GUI_FLOAT2UINT_ROUND( color.GetRed()*255 ),
			GUI_FLOAT2UINT_ROUND( color.GetGreen()*255 ), 
			GUI_FLOAT2UINT_ROUND( color.GetBlue()*255 ), 
			GUI_FLOAT2UINT_ROUND( color.GetAlpha()*255 ));
		
		wxColourData data;
		data.SetChooseFull(true);
		data.SetColour( aWxColor );
		wxColourDialog dialog(propgrid, &data);
		if ( dialog.ShowModal() == wxID_OK )
		{
			wxColourData retData = dialog.GetColourData();
			aWxColor = retData.GetColour();
			color.SetColor( 
				aWxColor.Red() / 255.0f,
				aWxColor.Green() / 255.0f,
				aWxColor.Blue() / 255.0f,
				aWxColor.Alpha() / 255.0f );

			CGUIProperty aValue = rValue;
			aValue.SetCommonValue(color);

			SetValueInEvent( wxVariant( aValue ));

			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
wxSize WxGuiColorProperty::OnMeasureImage( int item ) const
{
	return wxPG_DEFAULT_IMAGE_SIZE;
}
// -----------------------------------------------------------------------
void WxGuiColorProperty::OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata )
{
	const CGUIProperty& rValue = CGUIPropertyRefFromVariant(GetValue());
	CGUIColor color = rValue.GetCommonValue<CGUIColor>();
	wxColour aWxColor( 
		GUI_FLOAT2UINT_ROUND( color.GetRed()*255 ),
		GUI_FLOAT2UINT_ROUND( color.GetGreen()*255 ), 
		GUI_FLOAT2UINT_ROUND( color.GetBlue()*255 ), 
		GUI_FLOAT2UINT_ROUND( color.GetAlpha()*255 ));

	if ( aWxColor.Ok() )
	{
		dc.SetBrush(aWxColor);
		dc.DrawRectangle(rect);
	}
}
// -----------------------------------------------------------------------



// -----------------------------------------------------------------------
// WxGUIImageProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIImageProperty, wxPGProperty)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIImageProperty,CGUIString,ChoiceAndButton);
WxGUIImageProperty::WxGUIImageProperty( const wxString& label, const wxString& name,const wxString& rImage )
: wxPGProperty(label,name)
{
	SetValue( wxVariant( wx2GuiString( rImage )) );
	m_choices.Set(CGUIResourcePool::Instance()->GetImageList(), 0);
}
// -----------------------------------------------------------------------
void WxGUIImageProperty::OnSetValue()
{
	wxString variantType = m_value.GetType();
	if ( variantType == wxPG_VARIANT_TYPE_LONG )
	{
		//index of choice
		wxString strImageName = m_choices.GetLabel( m_value.GetInteger() );

		m_value = strImageName;
	}
}
// -----------------------------------------------------------------------
wxString WxGUIImageProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	wxString s = value.GetString();
	return s;
}
// -----------------------------------------------------------------------
bool WxGUIImageProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	if ( variant != text )
	{
		variant = text;
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------
bool WxGUIImageProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		WxImageSelectDialog dialog( propgrid );
		if ( dialog.ShowModal() == wxID_OK )
		{
			SetValueInEvent( dialog.GetResourceName() );
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
wxSize WxGUIImageProperty::OnMeasureImage( int item ) const
{
	return wxSize(PREF_THUMBNAIL_HEIGHT,PREF_THUMBNAIL_HEIGHT);
}
// -----------------------------------------------------------------------
void WxGUIImageProperty::OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata )
{
	wxString strImageName;

	if ( paintdata.m_choiceItem >= 0 &&
		paintdata.m_choiceItem < (int)m_choices.GetCount())
	{
		int colInd = m_choices[paintdata.m_choiceItem].GetValue();
		strImageName = m_choices.GetLabel( colInd );
	}
	else
	{
		strImageName = GetValueAsString();
	}

	const wxBitmap* pBitmap = CGUIResourcePool::Instance()->GetImageThumbnail( strImageName );
	if( pBitmap )
	{
		dc.DrawBitmap ( *pBitmap, rect.x, rect.y, FALSE );
		// Tell the caller how wide we drew.
		paintdata.m_drawnWidth = pBitmap->GetWidth();
	}
	else
	{
		dc.SetBrush ( *wxWHITE_BRUSH );
		dc.DrawRectangle ( rect );
	}
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// WxGUIAnimationProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIAnimationProperty, wxPGProperty)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIAnimationProperty,CGUIString,ChoiceAndButton);
WxGUIAnimationProperty::WxGUIAnimationProperty( const wxString& label, const wxString& name,const wxString& rAnimation )
	: wxPGProperty(label,name)
{
	SetValue( wxVariant( wx2GuiString( rAnimation )) );
	m_choices.Set(CGUIResourcePool::Instance()->GetAnimationList(), 0);
}
// -----------------------------------------------------------------------
void WxGUIAnimationProperty::OnSetValue()
{
	wxString variantType = m_value.GetType();
	if ( variantType == wxPG_VARIANT_TYPE_LONG )
	{
		//index of choice
		wxString strAnimationName = m_choices.GetLabel( m_value.GetInteger() );

		m_value = strAnimationName;
	}
}
// -----------------------------------------------------------------------
wxString WxGUIAnimationProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	wxString s = value.GetString();
	return s;
}
// -----------------------------------------------------------------------
bool WxGUIAnimationProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	if ( variant != text )
	{
		variant = text;
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------
bool WxGUIAnimationProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		WxAnimationSelectDialog dialog( propgrid );
		if ( dialog.ShowModal() == wxID_OK )
		{
			SetValueInEvent( dialog.GetResourceName() );
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
wxSize WxGUIAnimationProperty::OnMeasureImage( int item ) const
{
	return wxSize(PREF_THUMBNAIL_HEIGHT,PREF_THUMBNAIL_HEIGHT);
}
// -----------------------------------------------------------------------
void WxGUIAnimationProperty::OnCustomPaint( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata )
{
	wxString strAnimationName;

	if ( paintdata.m_choiceItem >= 0 &&
		paintdata.m_choiceItem < (int)m_choices.GetCount())
	{
		int colInd = m_choices[paintdata.m_choiceItem].GetValue();
		strAnimationName = m_choices.GetLabel( colInd );
	}
	else
	{
		strAnimationName = GetValueAsString();
	}

	const wxBitmap* pBitmap = CGUIResourcePool::Instance()->GetAnimationThumbnail( strAnimationName );
	if( pBitmap )
	{
		dc.DrawBitmap ( *pBitmap, rect.x, rect.y, FALSE );
		// Tell the caller how wide we drew.
		paintdata.m_drawnWidth = pBitmap->GetWidth();
	}
	else
	{
		dc.SetBrush ( *wxWHITE_BRUSH );
		dc.DrawRectangle ( rect );
	}
}
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// WxGUILocalizedStringProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUILocalizedStringProperty, wxPGProperty)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUILocalizedStringProperty,CGUIString,TextCtrlAndButton);
WxGUILocalizedStringProperty::WxGUILocalizedStringProperty( const wxString& label, const wxString& name,const wxString& rLocalizedString )
: wxPGProperty(label,name)
{
	SetValue( wxVariant( wx2GuiString( rLocalizedString )) );
}
// -----------------------------------------------------------------------
wxString WxGUILocalizedStringProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	return value;
}
// -----------------------------------------------------------------------
bool WxGUILocalizedStringProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	if ( variant != text )
	{
		variant = text;
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------
bool WxGUILocalizedStringProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		WxLocalizedstringSelectDialog dialog( propgrid );
		if ( dialog.ShowModal() == wxID_OK )
		{
			SetValueInEvent( dialog.GetResourceName() );
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// WxGUISoundProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUISoundProperty, wxPGProperty)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUISoundProperty,CGUIString,ChoiceAndButton);
WxGUISoundProperty::WxGUISoundProperty( const wxString& label, const wxString& name,const wxString& rSound )
: wxPGProperty(label,name)
{
	SetValue( wxVariant( wx2GuiString( rSound )) );
	m_choices.Set(CGUIResourcePool::Instance()->GetSoundList(), 0);
}
// -----------------------------------------------------------------------
void WxGUISoundProperty::OnSetValue()
{
	wxString variantType = m_value.GetType();
	if ( variantType == wxPG_VARIANT_TYPE_LONG )
	{
		//index of choice
		wxString strSound = m_choices.GetLabel( m_value.GetInteger() );

		m_value = strSound;
	}
}
// -----------------------------------------------------------------------
wxString WxGUISoundProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	wxString s = value.GetString();
	return s;
}
// -----------------------------------------------------------------------
bool WxGUISoundProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	if ( variant != text )
	{
		variant = text;
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------
bool WxGUISoundProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		WxSoundSelectDialog dialog( propgrid );
		if ( dialog.ShowModal() == wxID_OK )
		{
			SetValueInEvent( dialog.GetResourceName() );
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------



// -----------------------------------------------------------------------
// WxGUIWidgetSizeProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIWidgetSizeProperty, WxGUIPropertyBase)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIWidgetSizeProperty,CGUIProperty,TextCtrl);
WxGUIWidgetSizeProperty::WxGUIWidgetSizeProperty()
	:WxGUIPropertyBase()
{
}
WxGUIWidgetSizeProperty::WxGUIWidgetSizeProperty( const wxString& label, const CGUIProperty& rValue )
:WxGUIPropertyBase(label, rValue)
{
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums) );
	AddPrivateChild( new WxGUISizeProperty(wxT("value"), *rValue["value"] ));

	RefreshChildren();
}
// -----------------------------------------------------------------------
void WxGUIWidgetSizeProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(m_value);
	CGUIWidgetSize aValue = rValue.GetCommonValue<CGUIWidgetSize>();

	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( aValue.m_eType, m_arrEnums ) );
	Item(1)->SetValue( wxVariant(*rValue["value"]));
}
// -----------------------------------------------------------------------
wxVariant WxGUIWidgetSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(thisValue);
	CGUIWidgetSize aValue = rValue.GetCommonValue<CGUIWidgetSize>();

	switch ( childIndex )
	{
	case 0: 
		guiex::StringToValue( wx2GuiString( m_arrEnums[childValue.GetLong()]), aValue.m_eType );
		break;
	case 1: 
		*rValue["value"] = CGUIPropertyRefFromVariant(childValue);
		break;
	}

	return rValue;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// WxGUIFontProperty
// -----------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(WxGUIFontProperty, WxGUIPropertyBase)
WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(WxGUIFontProperty,CGUIProperty,ChoiceAndButton );
WxGUIFontProperty::WxGUIFontProperty()
	:WxGUIPropertyBase()
{
}
WxGUIFontProperty::WxGUIFontProperty( const wxString& label, const CGUIProperty& rValue )
:WxGUIPropertyBase(label, rValue)
{
	m_aFontProp = rValue;

	m_choices.Set(CGUIResourcePool::Instance()->GetFontList(), 0);
}
// -----------------------------------------------------------------------
void WxGUIFontProperty::OnSetValue()
{
	wxString variantType = m_value.GetType();
	if ( variantType == wxPG_VARIANT_TYPE_LONG )
	{
		//index of choice
		if( m_choices.GetCount() > 0 )
		{
			wxString rFont = m_choices.GetLabel( m_value.GetInteger() );
			m_aFontProp.SetValue( wx2GuiString( rFont ));
			m_value = wxVariant(m_aFontProp);
		}
	}
}
// -----------------------------------------------------------------------
wxString WxGUIFontProperty::ValueToString( wxVariant& WXUNUSED(value), int argFlags ) const
{
	return Gui2wxString(m_aFontProp.GetValue());
}
// -----------------------------------------------------------------------
bool WxGUIFontProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	CGUIProperty& rValue = CGUIPropertyRefFromVariant(variant);
	rValue.SetValue( wx2GuiString( text ) );
	return true;
}
// -----------------------------------------------------------------------
bool WxGUIFontProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		WxFontSelectDialog dialog( propgrid );
		if ( dialog.ShowModal() == wxID_OK )
		{
			const CGUIProperty& rValue = CGUIPropertyRefFromVariant(GetValue());
			CGUIProperty aValue = rValue;
			aValue.SetValue( wx2GuiString(dialog.GetResourceName() ));
			SetValueInEvent( wxVariant(aValue) );
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
