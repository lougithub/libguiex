/** 
* @file propertysheetfunc.cpp
* @brief used to manipulate the sheet
* @author ken
* @date 2007-09-28
*/


//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"

#include "wxwidgetpropertysheet.h"
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
static	wxArrayString s_arrayImageArray;
static	wxArrayString s_arrayAsArray;
static	wxArrayString s_arrayTextAlignmentH;
static	wxArrayString s_arrayTextAlignmentV;

void	UpdateAsNameList()
{
	s_arrayAsArray.Clear();
	s_arrayAsArray.Add(_T(""));
	const std::map<guiex::CGUIString, guiex::CGUIAs*>& rMapAsList = guiex::CGUIAsManager::Instance()->GetRegisterResourceMap();
	for( std::map<guiex::CGUIString,guiex::CGUIAs*>::const_iterator itor = rMapAsList.begin();
		itor != rMapAsList.end();
		++itor)
	{
		s_arrayAsArray.Add(wxConvUTF8.cMB2WC( itor->first.c_str()).data());
	}
	s_arrayAsArray.Sort();
}
//------------------------------------------------------------------------------
void UpdateImageNameList()
{
	s_arrayImageArray.Clear();
	s_arrayImageArray.Add(_T(""));
	const std::map<guiex::CGUIString, guiex::CGUIImage*>& rMapImageList = guiex::CGUIImageManager::Instance()->GetRegisterResourceMap();
	for( std::map<guiex::CGUIString,guiex::CGUIImage*>::const_iterator itor = rMapImageList.begin();
		itor != rMapImageList.end();
		++itor)
	{
		s_arrayImageArray.Add(wxConvUTF8.cMB2WC( itor->first.c_str()).data());
	}
	s_arrayImageArray.Sort();
}
//------------------------------------------------------------------------------
void LoadWidgetConfig( wxPropertyGridManager* pSheetMgr, const std::string& rType, guiex::CGUIWidget* pWidget )
{
	//get property set
	const guiex::CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet(rType);

	int nPageIdx = pSheetMgr->GetSelectedPage( );

	//process page
	std::map<std::pair<std::string,std::string>, wxPGProperty*>	mapCategory;
	for(unsigned int i=0; i<rSet.GetPropertyNum();++i)
	{
		const guiex::CGUIProperty* pProp = rSet.GetProperty(i);
			
		SetPropertyPageByEvent(pSheetMgr, CPropertyData::GetPropertyData(*pProp)->GetPage());
		
		std::string strPage = CPropertyData::GetPropertyData(*pProp)->GetPage();
		std::string strCategory = CPropertyData::GetPropertyData(*pProp)->GetCategory();
		if( mapCategory.find( std::make_pair(strPage,strCategory)) == mapCategory.end())
		{
			//add category
			wxString strLabel = wxConvUTF8.cMB2WC( CPropertyData::GetPropertyData(*pProp)->GetCategory().c_str());
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
void			SetPropertyPageByEvent( wxPropertyGridManager* pSheetMgr, const std::string& rEvent )
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
		pSheetMgr->SelectPage(NOTEBOOK_PAGE_PROP);
	}
}
//------------------------------------------------------------------------------
void GenerateGUIProperty( wxPropertyGridManager* pSheetMgr,guiex::CGUIProperty& rSet )
{
	for ( wxPGVIterator it = pSheetMgr->GetVIterator(wxPG_ITERATE_PROPERTIES);
		!it.AtEnd();
		it.Next() )
	{
		guiex::CGUIProperty aProp;
		GenerateGUIProperty(pSheetMgr, it.GetProperty(), aProp );		
		rSet.AddProperty( aProp );
	}
}
//------------------------------------------------------------------------------
guiex::CGUIWidget*	CreateWidget( wxPropertyGridManager* pSheetMgr, const char* strType, const char* strName, guiex::CGUIWidget* pParent)
{
	guiex::CGUIWidget* pWidget = NULL;
	try
	{
		pWidget = GUI_CREATE_WIDGET(strType, strName, GetMainFrame()->GetCurrentSceneName());
		guiex::CGUIProperty aSet;
		GenerateGUIProperty( pSheetMgr,aSet);

		pWidget->SetProperty(aSet);
		pWidget->LoadProperty();
		pWidget->Create();
	}
	catch (guiex::CGUIBaseException& rError)
	{
		wxMessageBox( wxConvUTF8.cMB2WC(rError.what()), _T("error"), wxICON_ERROR|wxCENTRE);

		if( pWidget )
		{
			CGUIWidgetManager::Instance()->DestroyWidget(pWidget);
			pWidget = NULL;
		}
	}
	return pWidget;
}


//------------------------------------------------------------------------------
void SetPropertyByType( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, const guiex::CGUIProperty* pDefaultProp, guiex::CGUIWidget* pWidget )
{
	wxPGProperty* pPGTop = pSheetMgr->GetPropertyByName( STRING_M2W(pDefaultProp->GetName()));
	GUI_ASSERT( pPGTop || pPGProperty, "wrong parameter" );

	guiex::CGUIProperty aProp( pDefaultProp->GetName(), pDefaultProp->GetTypeAsString());
	aProp.SetData( pDefaultProp->GetData() );
	try
	{
		if( 0 != pWidget->GenerateProperty( aProp ))
		{
			wxString strError = wxString::Format( _T("Failed to Generate GUI Property <%s :: %s> for widget <%s :: %s>)"),
				STRING_M2W(aProp.GetName()), 
				STRING_M2W(aProp.GetTypeAsString()),
				STRING_M2W(pWidget->GetName()),
				STRING_M2W(pWidget->GetType()));
			wxMessageBox(strError, _T("Error"), wxICON_ERROR|wxCentre);
			return;
		}
	}
	catch (guiex::CGUIBaseException& rError)
	{
		wxMessageBox( wxConvUTF8.cMB2WC(rError.what()), _T("error"), wxICON_ERROR|wxCENTRE);
		return;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector2
	if( aProp.GetType() == guiex::ePropertyType_Vector2 )
	{
		guiex::CGUIVector2 aValue;
		guiex::PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIVector2Property( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector3
	else if( aProp.GetType() == guiex::ePropertyType_Vector3 )
	{
		guiex::CGUIVector3 aValue;
		guiex::PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIVector3Property( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIRect
	else if( aProp.GetType() == guiex::ePropertyType_Rect )
	{
		guiex::CGUIRect aValue;
		guiex::PropertyToValue(aProp, aValue);
			
		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIRectProperty( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIColor
	else if( aProp.GetType() == guiex::ePropertyType_Color)
	{
		guiex::CGUIColor aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( &aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGuiColorProperty( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUISize
	else if( aProp.GetType() == guiex::ePropertyType_Size )
	{
		guiex::CGUISize aValue;
		guiex::PropertyToValue(aProp, aValue);

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUISizeProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIString
	else if( aProp.GetType() == guiex::ePropertyType_String )
	{
		wxString aValue;
		aValue = wxConvUTF8.cMB2WC(aProp.GetValue().c_str());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxStringProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIEvent
	else if( aProp.GetType() == guiex::ePropertyType_Event )
	{
		wxString aValue;
		aValue = wxConvUTF8.cMB2WC(aProp.GetValue().c_str());

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxStringProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIStringInfo
	else if(  aProp.GetType() == guiex::ePropertyType_StringInfo )
	{
		guiex::CGUIStringInfo aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIStringInfoProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetPosition
	else if(  aProp.GetType() == guiex::ePropertyType_WidgetPosition )
	{
		guiex::CGUIWidgetPosition aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( 
				pPGProperty, 
				-1, 
				new wxGUIWidgetPositionProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetSize
	else if(  aProp.GetType() == guiex::ePropertyType_WidgetSize )
	{
		guiex::CGUIWidgetSize aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( WXVARIANT( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIWidgetSizeProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//IMAGE
	else if(  aProp.GetType() == guiex::ePropertyType_Image )
	{			
		wxString aValue;
		aValue = wxConvUTF8.cMB2WC(aProp.GetValue().c_str());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()),s_arrayImageArray));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//AS
	else if(  aProp.GetType() == guiex::ePropertyType_As )
	{			
		wxString aValue;
		aValue = wxConvUTF8.cMB2WC(aProp.GetValue().c_str());

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()),s_arrayAsArray));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//ENUM
	else if( aProp.GetType() == ePropertyType_ScreenValue )
	{
		guiex::EScreenValue aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	else if( aProp.GetType() == ePropertyType_TextAlignmentHorz )
	{
		guiex::ETextAlignmentHorz aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	else if( aProp.GetType() == ePropertyType_TextAlignmentVert )
	{
		guiex::ETextAlignmentVert aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	else if( aProp.GetType() == ePropertyType_ImageOrientation )
	{			
		guiex::EImageOrientation aValue;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			const wxArrayString& arrEnums = CPropertyConfigMgr::Instance()->GetEnumDefine(aProp.GetTypeAsString());
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//real
	else if(  aProp.GetType() == guiex::ePropertyType_Real )
	{			
		guiex::real aValue = 0.f;
		guiex::PropertyToValue( aProp, aValue );
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxFloatProperty( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//uint32
	else if(  aProp.GetType() == guiex::ePropertyType_UInt32 )
	{			
		guiex::uint32 aValue = 0;
		guiex::PropertyToValue( aProp, aValue );
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, int(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxIntProperty( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//bool
	else if(  aProp.GetType() == guiex::ePropertyType_Bool )
	{			
		bool aValue = false;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxBoolProperty( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()),aValue ) );
			pSheetMgr->SetPropertyAttribute( pPGTop,wxPG_BOOL_USE_CHECKBOX,(long)1 );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//int32
	else if(  aProp.GetType() == guiex::ePropertyType_Int32 )
	{			
		guiex::int32 aValue = 0;
		guiex::PropertyToValue( aProp, aValue );

		if( pPGTop )
		{
			pPGTop->SetValue( int(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxUIntProperty( STRING_M2W(CPropertyData::GetPropertyData(aProp)->GetLabel()), STRING_M2W(aProp.GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//unknown
	else
	{
		std::string strError = std::string("unsupported property type:  ")+aProp.GetTypeAsString();
		wxMessageBox(wxConvUTF8.cMB2WC(strError.c_str()), _T("Error"), wxICON_ERROR|wxCentre);
	}

	//set type as client data
	if( pPGTop)
	{
		pSheetMgr->SetPropertyClientData( pPGTop, CPropertyConfigMgr::Instance()->GetTypePtr(aProp.GetTypeAsString()));

		if( CPropertyData::GetPropertyData(aProp)->IsMustExist())
		{
			pSheetMgr->SetPropertyTextColour( pPGTop, wxColour(255,0,0));
		}
	}
}
//------------------------------------------------------------------------------
void GenerateGUIProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, guiex::CGUIProperty& rProperty )
{
	std::string  strName = pSheetMgr->GetPropertyName(pPGProperty).char_str(wxConvUTF8).data();
	std::string* pType = (std::string*)pSheetMgr->GetPropertyClientData( pPGProperty );
	wxASSERT( pType );
	
	rProperty.SetName( strName );
	rProperty.SetType( *pType );

	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector2
	if( rProperty.GetType() == guiex::ePropertyType_Vector2 )
	{
		guiex::CGUIVector2 aValue = CGUIVector2RefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIVector3
	else if( rProperty.GetType() == guiex::ePropertyType_Vector3 )
	{
		guiex::CGUIVector3 aValue = CGUIVector3RefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIRect
	else if( rProperty.GetType() == guiex::ePropertyType_Rect )
	{
		guiex::CGUIRect aValue = CGUIRectRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//real
	else if( rProperty.GetType() == guiex::ePropertyType_Real )
	{
		guiex::real aValue = pSheetMgr->GetPropertyValueAsDouble(pPGProperty);
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//uint32
	else if( rProperty.GetType() == guiex::ePropertyType_UInt32 )
	{
		guiex::uint32 aValue = pSheetMgr->GetPropertyValueAsInt(pPGProperty);
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//bool
	else if( rProperty.GetType() == ePropertyType_Bool )
	{
		bool aValue = pSheetMgr->GetPropertyValueAsBool(pPGProperty);
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUISize
	else if( rProperty.GetType() == guiex::ePropertyType_Size )
	{
		guiex::CGUISize aValue = CGUISizeRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIEvent
	else if( rProperty.GetType() == ePropertyType_Event )
	{
		guiex::CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIEvent
	else if( rProperty.GetType() == ePropertyType_String )
	{
		guiex::CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIImage
	else if( rProperty.GetType() == ePropertyType_Image )
	{
		guiex::CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIAs
	else if( rProperty.GetType() == ePropertyType_As )
	{
		guiex::CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//enum
	else if( rProperty.GetType() == ePropertyType_ImageOrientation ||
		rProperty.GetType() == ePropertyType_ScreenValue ||
		rProperty.GetType() == ePropertyType_TextAlignmentVert ||
		rProperty.GetType() == ePropertyType_TextAlignmentHorz )
	{
		guiex::CGUIString aValue = pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data();
		rProperty.SetValue( aValue );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIStringInfo
	else if( rProperty.GetType() == ePropertyType_StringInfo )
	{
		guiex::CGUIStringInfo aValue= CGUIStringInfoRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetPosition
	else if(  rProperty.GetType() == ePropertyType_WidgetPosition )
	{
		guiex::CGUIWidgetPosition aValue = CGUIWidgetPositionRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIWidgetSize
	else if( rProperty.GetType() == ePropertyType_WidgetSize )
	{
		guiex::CGUIWidgetSize aValue = CGUIWidgetSizeRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//CGUIColor
	else if( rProperty.GetType() == ePropertyType_Color )
	{
		//COLOR
		guiex::CGUIColor aValue = CGUIColorRefFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		guiex::ValueToProperty( aValue, rProperty );
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//unknown
	else
	{
		std::string strError = std::string("unsupported property type:  ")+pType->c_str();
		wxMessageBox(wxConvUTF8.cMB2WC(strError.c_str()), _T("Error"), wxICON_ERROR|wxCENTRE);
		throw guiex::CGUIException("unsupported property type: <%s>", pType->c_str());
	}
}
//------------------------------------------------------------------------------

