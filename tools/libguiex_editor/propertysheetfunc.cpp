/** 
* @file propertysheetfunc.cpp
* @brief used to manipulate the sheet
* @author ken
* @date 2007-09-28
*/


//============================================================================//
// include
//============================================================================// 
#include "propertysheetfunc.h"
#include "editorutility.h"
#include "resourcelist.h"

#include "wxwidgetpropertygridextend.h"
#include "propertysheetfunc.h"
#include "propertyconfigmgr.h"

//============================================================================//
// declare
//============================================================================// 
class wxInvalidWordValidator : public wxValidator
{
public:

	wxInvalidWordValidator( const wxString& invalidWord )
		: wxValidator(), m_invalidWord(invalidWord)
	{
	}

	virtual wxObject* Clone() const
	{
		return new wxInvalidWordValidator(m_invalidWord);
	}
	virtual bool Validate(wxWindow* WXUNUSED(parent))
	{
		wxTextCtrl* tc = wxDynamicCast(GetWindow(), wxTextCtrl);
		wxCHECK_MSG(tc, true, wxT("validator window must be wxTextCtrl"));

		wxString val = tc->GetValue();

		if( val != m_invalidWord)
			return true;

		::wxMessageBox(wxString::Format(wxT("%s is not allowed word"),m_invalidWord.c_str()),
			wxT("Validation Failure"));

		return false;
	}

private:
	wxString    m_invalidWord;
};


//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
static	wxArrayString s_arrayTextAlignmentH;
static	wxArrayString s_arrayTextAlignmentV;


//------------------------------------------------------------------------------
void LoadWidgetConfig( wxPropertyGridManager* pSheetMgr, const std::string& rType, CGUIWidget* pWidget )
{
	//get property set
	const CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet(rType);

	int nPageIdx = pSheetMgr->GetSelectedPage( );

	//process page
	std::map<std::pair<std::string,std::string>, wxPGProperty*>	mapCategory;
	for(unsigned int i=0; i<rSet.GetPropertyNum();++i)
	{
		const CGUIProperty* pProp = rSet.GetProperty(i);
			
		SetPropertyPageByEvent(pSheetMgr, CPropertyData::GetPropertyData(*pProp)->GetPage());
		
		std::string strPage = CPropertyData::GetPropertyData(*pProp)->GetPage();
		std::string strCategory = CPropertyData::GetPropertyData(*pProp)->GetCategory();
		if( mapCategory.find( std::make_pair(strPage,strCategory)) == mapCategory.end())
		{
			//add category
			wxString strLabel = Gui2wxString( CPropertyData::GetPropertyData(*pProp)->GetCategory());
			wxPGProperty* pPGProperty = pSheetMgr->Append(new wxPropertyCategory(strLabel, wxPG_LABEL));
			wxASSERT(pPGProperty);

			mapCategory.insert( std::make_pair(std::make_pair(strPage,strCategory), pPGProperty));
		}

		//add property
		wxPGProperty* pPGProp = mapCategory[std::make_pair(strPage,strCategory)];
		SetPropertyByType( pSheetMgr, pPGProp, pProp, pWidget );
	}
	
	pSheetMgr->SelectPage(nPageIdx);
	pSheetMgr->ExpandAll();
}
//------------------------------------------------------------------------------
void SetPropertyPageByEvent( wxPropertyGridManager* pSheetMgr, const std::string& rEvent )
{
	if( rEvent == NOTEBOOK_EVENT_PAGE_NAME)
	{
		pSheetMgr->SelectPage(NOTEBOOK_PAGE_EVENT);
	}
	else if( rEvent == NOTEBOOK_IMAGE_PAGE_NAME )
	{
		pSheetMgr->SelectPage(NOTEBOOK_PAGE_IMAGE);
	}
	else
	{
		pSheetMgr->SelectPage(NOTEBOOK_PAGE_APPEARANCE);
	}
}
//------------------------------------------------------------------------------
void GenerateGUIProperty( wxPropertyGridManager* pSheetMgr,CGUIProperty& rSet )
{
	for ( wxPGVIterator it = pSheetMgr->GetVIterator(wxPG_ITERATE_PROPERTIES);
		!it.AtEnd();
		it.Next() )
	{
		CGUIProperty aProp;
		GenerateGUIProperty(pSheetMgr, it.GetProperty(), aProp );		
		rSet.AddProperty( aProp );
	}
}
//------------------------------------------------------------------------------
void SetPropertyByType( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, const CGUIProperty* pDefaultProp, CGUIWidget* pWidget )
{
	wxPGProperty* pPGTop = pSheetMgr->GetPropertyByName( Gui2wxString(pDefaultProp->GetName()));
	GUI_ASSERT( pPGTop || pPGProperty, "wrong parameter" );

	CGUIProperty aProp;
	if( pWidget )
	{
		aProp.SetName( pDefaultProp->GetName() );
		aProp.SetType( pDefaultProp->GetTypeAsString() );
		aProp.SetData( pDefaultProp->GetData() );
		try
		{
			if( 0 != pWidget->GenerateProperty( aProp ))
			{
				wxString strError = wxString::Format( _T("Failed to Generate GUI Property <%s :: %s> for widget <%s :: %s>)"),
					Gui2wxString(aProp.GetName()), 
					Gui2wxString(aProp.GetTypeAsString()),
					Gui2wxString(pWidget->GetName()),
					Gui2wxString(pWidget->GetType()));
				wxMessageBox(strError, _T("Error") );
				return;
			}
		}
		catch (std::exception& rError)
		{
			wxMessageBox( Gui2wxString(rError.what()), _T("error") );
			return;
		}
	}
	else
	{
		aProp = *pDefaultProp;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector2
	if( aProp.GetType() == ePropertyType_Vector2 )
	{
		CGUIVector2 aValue;
		PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUIVector2Property( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector3
	else if( aProp.GetType() == ePropertyType_Vector3 )
	{
		CGUIVector3 aValue;
		PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUIVector3Property( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIRect
	else if( aProp.GetType() == ePropertyType_Rect )
	{
		CGUIRect aValue;
		PropertyToValue(aProp, aValue);
			
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUIRectProperty( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIColor
	else if( aProp.GetType() == ePropertyType_Color)
	{
		CGUIColor aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( &aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGuiColorProperty( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUISize
	else if( aProp.GetType() == ePropertyType_Size )
	{
		CGUISize aValue;
		PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUISizeProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIString
	else if( aProp.GetType() == ePropertyType_String )
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxStringProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIEvent
	else if( aProp.GetType() == ePropertyType_Event )
	{
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxStringProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIStringInfo
	else if(  aProp.GetType() == ePropertyType_StringInfo )
	{
		CGUIStringInfo aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUIStringInfoProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetPosition
	else if(  aProp.GetType() == ePropertyType_WidgetPosition )
	{
		CGUIWidgetPosition aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( 
				pPGProperty, 
				-1, 
				new WxGUIWidgetPositionProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetSize
	else if(  aProp.GetType() == ePropertyType_WidgetSize )
	{
		CGUIWidgetSize aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUIWidgetSizeProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//IMAGE
	else if(  aProp.GetType() == ePropertyType_Image )
	{			
		wxString aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new WxGUIImageProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//AS
	else if(  aProp.GetType() == ePropertyType_As )
	{			
		wxString aValue;
		aValue = Gui2wxString(aProp.GetValue());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()),CResourceList::Instance()->GetAsList()));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//ENUM
	else if( aProp.GetType() == ePropertyType_ScreenValue )
	{
		EScreenValue aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	else if( aProp.GetType() == ePropertyType_TextAlignmentHorz )
	{
		ETextAlignmentHorz aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	else if( aProp.GetType() == ePropertyType_TextAlignmentVert )
	{
		ETextAlignmentVert aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	else if( aProp.GetType() == ePropertyType_ImageOrientation )
	{			
		EImageOrientation aValue;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//real
	else if(  aProp.GetType() == ePropertyType_Real )
	{			
		real aValue = 0.f;
		PropertyToValue( aProp, aValue );
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxFloatProperty( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//uint32
	else if(  aProp.GetType() == ePropertyType_UInt32 )
	{			
		uint32 aValue = 0;
		PropertyToValue( aProp, aValue );
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, int(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxIntProperty( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//bool
	else if(  aProp.GetType() == ePropertyType_Bool )
	{			
		bool aValue = false;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxBoolProperty( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()),aValue ) );
			pSheetMgr->SetPropertyAttribute( pPGTop,wxPG_BOOL_USE_CHECKBOX,(long)1 );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//int32
	else if(  aProp.GetType() == ePropertyType_Int32 )
	{			
		int32 aValue = 0;
		PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( int(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxUIntProperty( CPropertyData::GetPropertyLabel(aProp), Gui2wxString(aProp.GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//unknown
	else
	{
		std::string strError = std::string("unsupported property type:  ")+aProp.GetTypeAsString();
		wxMessageBox(Gui2wxString(strError), _T("Error") );
	}

	//set type as client data
	if( pPGTop)
	{
		pSheetMgr->SetPropertyClientData( pPGTop, CPropertyConfigMgr::Instance()->GetTypePtr(aProp.GetTypeAsString()));

		if( CPropertyData::GetPropertyData(aProp)->IsMustExist())
		{
			pSheetMgr->SetPropertyTextColour( pPGTop, wxColour(255,0,0));
		}	
		if( CPropertyData::GetPropertyData(aProp)->IsReadOnly())
		{
			pSheetMgr->SetPropertyReadOnly( pPGTop, true);
		}
	}
}
//------------------------------------------------------------------------------
void GenerateGUIProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty )
{
	std::string  strName = pSheetMgr->GetPropertyName(pPGProperty).char_str(wxConvUTF8).data();
	std::string* pType = (std::string*)pSheetMgr->GetPropertyClientData( pPGProperty );
	wxASSERT( pType );
	
	rProperty.SetName( strName );
	rProperty.SetType( *pType );

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector2
	if( rProperty.GetType() == ePropertyType_Vector2 )
	{
		CGUIVector2 aValue = CGUIVector2RefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector3
	else if( rProperty.GetType() == ePropertyType_Vector3 )
	{
		CGUIVector3 aValue = CGUIVector3RefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIRect
	else if( rProperty.GetType() == ePropertyType_Rect )
	{
		CGUIRect aValue = CGUIRectRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//real
	else if( rProperty.GetType() == ePropertyType_Real )
	{
		real aValue = pSheetMgr->GetPropertyValueAsDouble(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//uint32
	else if( rProperty.GetType() == ePropertyType_UInt32 )
	{
		uint32 aValue = pSheetMgr->GetPropertyValueAsInt(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//bool
	else if( rProperty.GetType() == ePropertyType_Bool )
	{
		bool aValue = pSheetMgr->GetPropertyValueAsBool(pPGProperty);
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUISize
	else if( rProperty.GetType() == ePropertyType_Size )
	{
		CGUISize aValue = CGUISizeRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIEvent
	else if( rProperty.GetType() == ePropertyType_Event )
	{
		CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIEvent
	else if( rProperty.GetType() == ePropertyType_String )
	{
		CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIImage
	else if( rProperty.GetType() == ePropertyType_Image )
	{
		CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIAs
	else if( rProperty.GetType() == ePropertyType_As )
	{
		CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//enum
	else if( rProperty.GetType() == ePropertyType_ImageOrientation ||
		rProperty.GetType() == ePropertyType_ScreenValue ||
		rProperty.GetType() == ePropertyType_TextAlignmentVert ||
		rProperty.GetType() == ePropertyType_TextAlignmentHorz )
	{
		CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIStringInfo
	else if( rProperty.GetType() == ePropertyType_StringInfo )
	{
		CGUIStringInfo aValue= CGUIStringInfoRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetPosition
	else if(  rProperty.GetType() == ePropertyType_WidgetPosition )
	{
		CGUIWidgetPosition aValue = CGUIWidgetPositionRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetSize
	else if( rProperty.GetType() == ePropertyType_WidgetSize )
	{
		CGUIWidgetSize aValue = CGUIWidgetSizeRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIColor
	else if( rProperty.GetType() == ePropertyType_Color )
	{
		//COLOR
		CGUIColor aValue = CGUIColorRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//unknown
	else
	{
		std::string strError = std::string("unsupported property type:  ")+pType->c_str();
		wxMessageBox(Gui2wxString(strError), _T("Error") );
		throw CGUIException("unsupported property type: <%s>", pType->c_str());
	}
}
//------------------------------------------------------------------------------

