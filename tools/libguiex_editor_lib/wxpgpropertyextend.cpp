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
#include "propertyconvertor.h"

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

// -----------------------------------------------------------------------
// WxGUISizeProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUISize );
WX_PG_IMPLEMENT_PROPERTY_CLASS( WxGUISizeProperty,wxPGProperty,CGUISize,const CGUISize&,TextCtrl);
WxGUISizeProperty::WxGUISizeProperty( const wxString& label, const CGUISize& value)
	: wxPGProperty(label,GetUniquePropName())
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("width"), wxT("WIDTH"),value.m_fWidth) );
	AddPrivateChild( new wxFloatProperty(wxT("height"), wxT("HEIGHT"),value.m_fHeight) );
}
// -----------------------------------------------------------------------
void WxGUISizeProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	CGUISize& size = CGUISizeRefFromVariant(m_value);
	Item(0)->SetValue( size.m_fWidth );
	Item(1)->SetValue( size.m_fHeight );
}
// -----------------------------------------------------------------------
wxVariant WxGUISizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
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



// -----------------------------------------------------------------------
// WxGUIVector2Property
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIVector2 );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIVector2Property,wxPGProperty,CGUIVector2,const CGUIVector2&,TextCtrl);
WxGUIVector2Property::WxGUIVector2Property( const wxString& label,const CGUIVector2& value)
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIVector3 );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIVector3Property,wxPGProperty,CGUIVector3,const CGUIVector3&,TextCtrl);
WxGUIVector3Property::WxGUIVector3Property( const wxString& label, const CGUIVector3& value)
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIRotator );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIRotatorProperty,wxPGProperty,CGUIRotator,const CGUIRotator&,TextCtrl);
WxGUIRotatorProperty::WxGUIRotatorProperty( const wxString& label,const CGUIRotator& value)
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIRect );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIRectProperty,wxPGProperty,CGUIVector2,const CGUIVector2&,TextCtrl);
WxGUIRectProperty::WxGUIRectProperty( const wxString& label,const CGUIRect& value)
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIStringRenderInfo );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIStringInfoProperty,wxPGProperty,CGUIStringRenderInfo,const CGUIStringRenderInfo&,TextCtrl);
WxGUIStringInfoProperty::WxGUIStringInfoProperty( const wxString& label,const CGUIStringRenderInfo& value)
	: wxPGProperty(label,GetUniquePropName())
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("scale"),wxT("scale"), value.m_fFontScale) );
	AddPrivateChild( new WxGUIFontProperty(wxT("font"),value.m_uFontID) );
	AddPrivateChild( new WxGuiColorProperty(wxT("color"), value.m_aColor) );
}

void WxGUIStringInfoProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIStringRenderInfo& string_info = CGUIStringRenderInfoRefFromVariant(m_value);
	Item(0)->SetValue( string_info.m_fFontScale );
	Item(1)->SetValue( string_info.m_uFontID);
	Item(2)->SetValue( WXVARIANT( string_info.m_aColor ));
}
// -----------------------------------------------------------------------
wxVariant WxGUIStringInfoProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIStringRenderInfo& string_info = CGUIStringRenderInfoRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: string_info.m_fFontScale = childValue.GetDouble(); break;
	case 1: string_info.m_uFontID = childValue.GetInteger(); break;
	case 2: string_info.m_aColor << childValue; break;
	}

	wxVariant newVariant;
	newVariant << string_info;
	return newVariant;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// WxGUIWidgetPositionProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIWidgetPosition );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIWidgetPositionProperty,wxPGProperty,CGUIWidgetPosition,const CGUIWidgetPosition&,TextCtrl);
WxGUIWidgetPositionProperty::WxGUIWidgetPositionProperty( const wxString& label,const CGUIWidgetPosition& value)
	: wxPGProperty(label,GetUniquePropName())
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums ) );
	AddPrivateChild( new WxGUIVector2Property(wxT("value"), value.m_aValue) );

	RefreshChildren();
}
// -----------------------------------------------------------------------
void WxGUIWidgetPositionProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIWidgetPosition& widget_pos = CGUIWidgetPositionRefFromVariant(m_value);
	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( widget_pos.m_eType, m_arrEnums ) );
	Item(1)->SetValue( WXVARIANT(widget_pos.m_aValue));
}
// -----------------------------------------------------------------------
wxVariant WxGUIWidgetPositionProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIWidgetPosition& widget_pos = CGUIWidgetPositionRefFromVariant( thisValue );

	switch ( childIndex )
	{
	case 0:
		guiex::StringToValue( wx2GuiString( m_arrEnums[childValue.GetLong()]), widget_pos.m_eType );
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


// -----------------------------------------------------------------------
// wxGUIColorProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIColor );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGuiColorProperty,wxPGProperty,CGUIColor,const CGUIColor&,TextCtrlAndButton);
WxGuiColorProperty::WxGuiColorProperty( const wxString& label, const CGUIColor& value)
	: wxPGProperty(label,GetUniquePropName())
{
	SetValue( WXVARIANT(value) );
}
// -----------------------------------------------------------------------
wxString WxGuiColorProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	wxString test = value.GetType();
	CGUIColor color = CGUIColorRefFromVariant(value);
	CGUIString strValue;
	guiex::ValueToString( color, strValue );
	return Gui2wxString( strValue );
}
// -----------------------------------------------------------------------
bool WxGuiColorProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	CGUIColor color;
	CGUIString strValue = wx2GuiString( text );
	try
	{
		if( 0 != guiex::StringToValue( strValue, color ) )
		{
			return false;
		}
	}
	catch(...)
	{
		return false;
	}
	variant = WXVARIANT(color);
	return true;
}
// -----------------------------------------------------------------------
bool WxGuiColorProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		CGUIColor color = CGUIColorRefFromVariant(GetValue());
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

			SetValueInEvent( WXVARIANT(color) );

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
	CGUIColor color = CGUIColorRefFromVariant(GetValue());
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
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIImageProperty,wxPGProperty,CGUIString,const CGUIString&,ChoiceAndButton);
WxGUIImageProperty::WxGUIImageProperty( const wxString& label, const wxString& rImage )
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIAnimationProperty,wxPGProperty,CGUIString,const CGUIString&,ChoiceAndButton);
WxGUIAnimationProperty::WxGUIAnimationProperty( const wxString& label, const wxString& rAnimation )
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUILocalizedStringProperty,wxPGProperty,CGUIString,const CGUIString&,TextCtrlAndButton);
WxGUILocalizedStringProperty::WxGUILocalizedStringProperty( const wxString& label, const wxString& rLocalizedString )
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUISoundProperty,wxPGProperty,CGUIString,const CGUIString&,ChoiceAndButton);
WxGUISoundProperty::WxGUISoundProperty( const wxString& label, const wxString& rSound )
	: wxPGProperty(label,GetUniquePropName())
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
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ( CGUIWidgetSize );
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIWidgetSizeProperty,wxPGProperty,CGUIWidgetSize,const CGUIWidgetSize&,TextCtrl);
WxGUIWidgetSizeProperty::WxGUIWidgetSizeProperty( const wxString& label,const CGUIWidgetSize& value)
	: wxPGProperty(label,GetUniquePropName())
{
	//ChangeFlag(wxPG_PROP_READONLY, true);
	SetValue( WXVARIANT(value) );
	m_arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine("EScreenValue");
	AddPrivateChild( new wxEnumProperty(wxT("type"), wxT("type"), m_arrEnums) );
	AddPrivateChild( new WxGUISizeProperty(wxT("size"), value.m_aValue) );

	RefreshChildren();
}
// -----------------------------------------------------------------------
void WxGUIWidgetSizeProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	CGUIWidgetSize& widget_size = CGUIWidgetSizeRefFromVariant(m_value);
	Item(0)->SetValue( GetTypeIndexInScreenValueEnum( widget_size.m_eType, m_arrEnums ) );
	Item(1)->SetValue( WXVARIANT(widget_size.m_aValue));
}
// -----------------------------------------------------------------------
wxVariant WxGUIWidgetSizeProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	CGUIWidgetSize& widget_size = CGUIWidgetSizeRefFromVariant(thisValue);

	switch ( childIndex )
	{
	case 0: 
		guiex::StringToValue( wx2GuiString( m_arrEnums[childValue.GetLong()]), widget_size.m_eType );
		break;
	case 1: 
		widget_size.m_aValue << childValue; 
		break;
	}

	wxVariant newVariant;
	newVariant << widget_size;
	return newVariant;
}
// -----------------------------------------------------------------------


// -----------------------------------------------------------------------
// WxGUIFontProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(WxGUIFontProperty,wxPGProperty,CGUIString,const CGUIString&,ChoiceAndButton );
WxGUIFontProperty::WxGUIFontProperty( const wxString& label, guiex::uint16 uFontID )
	: wxPGProperty(label,GetUniquePropName())
{
	wxString strFont;
	strFont<<uFontID;
	SetValue( wxVariant( strFont) );
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
			m_value = rFont;
		}
	}
}
// -----------------------------------------------------------------------
wxString WxGUIFontProperty::ValueToString( wxVariant& value, int argFlags ) const
{
	wxString s = value.GetString();
	return s;
}
// -----------------------------------------------------------------------
bool WxGUIFontProperty::StringToValue( wxVariant& variant, const wxString& text, int argFlags ) const
{
	if ( variant != text )
	{
		variant = text;
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------
bool WxGUIFontProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
	if ( propgrid->IsMainButtonEvent(event) )
	{
		WxFontSelectDialog dialog( propgrid );
		if ( dialog.ShowModal() == wxID_OK )
		{
			SetValueInEvent( dialog.GetResourceName() );
			return true;
		}
	}
	return false;
}
// -----------------------------------------------------------------------
