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

#define STRING_M2W(value)	wxConvUTF8.cMB2WC(value.c_str())

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
static	wxArrayString s_arrayTextAlignmentH;
static	wxArrayString s_arrayTextAlignmentV;

void	InitSheet()
{
}
//------------------------------------------------------------------------------
void		UpdateImageNameList()
{
	s_arrayImageArray.Clear();
	s_arrayImageArray.Add(_T(""));
	const std::map<guiex::CGUIString, guiex::CGUIImage*>& rMapImageList = guiex::CGUIImageManager::Instance()->GetResourceMap();
	for( std::map<guiex::CGUIString,guiex::CGUIImage*>::const_iterator itor = rMapImageList.begin();
		itor != rMapImageList.end();
		++itor)
	{
		s_arrayImageArray.Add(wxConvUTF8.cMB2WC( itor->first.c_str()).data());
	}
	s_arrayImageArray.Sort();
}
//------------------------------------------------------------------------------
void	LoadWidgetConfig( wxPropertyGridManager* pSheetMgr, const std::string& rType, guiex::CGUIWidget* pWidget )
{
	//get property set
	const CProperty* pSet = CPropertyConfigMgr::Instance()->GetPropertySet(rType);
	if( !pSet )
	{
		return;
	}

	int nPageIdx = pSheetMgr->GetSelectedPage( );

	//process page
	std::map<std::pair<std::string,std::string>, wxPGProperty*>	mapCategory;
	for(unsigned int i=0; i<pSet->GetPropertyNum();++i)
	{
		const CProperty* pProp = pSet->GetProperty(i);
			
		SetPropertyPageByEvent(pSheetMgr, pProp->GetPage());

		if( mapCategory.find( std::make_pair(pProp->GetPage(),pProp->GetCategory())) == mapCategory.end())
		{
			//add category
			wxPGProperty* pPGProperty = pSheetMgr->Append(new wxPropertyCategory(wxConvUTF8.cMB2WC( pProp->GetCategory().c_str()), wxPG_LABEL));
			wxASSERT(pPGProperty);

			mapCategory.insert( std::make_pair(std::make_pair(pProp->GetPage(),pProp->GetCategory()), pPGProperty));
		}

		//add property
		SetPropertyByType( pSheetMgr, pProp, mapCategory[std::make_pair(pProp->GetPage(),pProp->GetCategory())], pWidget );
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
void GenerateGUIProperty( wxPropertyGridManager* pSheetMgr,guiex::CGUIPropertySet& rSet )
{
	for ( wxPGVIterator it = pSheetMgr->GetVIterator(wxPG_ITERATE_PROPERTIES);
		!it.AtEnd();
		it.Next() )
	{
		guiex::CGUIProperty* pProp = GenerateGUIProperty(pSheetMgr, it.GetProperty());		
		rSet.AddProperty(pProp);
	}
}
//------------------------------------------------------------------------------
guiex::CGUIWidget*	CreateWidget( wxPropertyGridManager* pSheetMgr, const char* strType, const char* strName, guiex::CGUIWidget* pParent)
{
	guiex::CGUIWidget* pWidget = NULL;
	try
	{
		pWidget = GUI_CREATE_WIDGET(strType, strName, GetMainFrame()->GetCurrentProjectName());
		guiex::CGUIPropertySet aSet;
		GenerateGUIProperty( pSheetMgr,aSet);

		//sort property
		guiex::CGUIPropertySet aNewSet;
		if( pParent )
		{
			aSet.AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("PARENT","STRING",pParent->GetName().c_str()));
		}
		//image
		std::vector<guiex::CGUIProperty*>	aImgVector;
		for( unsigned i=0; i<aSet.GetSize(); ++i)
		{
			guiex::CGUIProperty* pProperty = aSet.GetProperty(i);
			if( pProperty->GetType() == "IMAGE")
			{
				aImgVector.push_back(pProperty);
			}
		}
		for( std::vector<guiex::CGUIProperty*>::iterator itor = aImgVector.begin();
			itor != aImgVector.end();
			++itor )
		{
			aNewSet.AddProperty((*itor)->Clone());
			aSet.RemoveProperty(*itor);
		}
		//left
		for( unsigned i=0; i<aSet.GetSize(); ++i)
		{
			guiex::CGUIProperty* pProperty = aSet.GetProperty(i);
			aNewSet.AddProperty(pProperty->Clone());
		}
		//remove all from old set
		aSet.Clear();

		pWidget->SetPropertySet(aNewSet);
		pWidget->LoadProperty();
		pWidget->Create();
	}
	catch (guiex::CGUIBaseException& rError)
	{
		wxMessageBox( wxConvUTF8.cMB2WC(rError.what()), _T("error"), wxICON_ERROR|wxCENTRE);

		if( pWidget )
		{
			GUI_DESTROY_WIDGET(pWidget);
			pWidget = NULL;
		}
	}
	return pWidget;
}



















//------------------------------------------------------------------------------
void			SetPropertyByType( wxPropertyGridManager* pSheetMgr, const CProperty* pProp, wxPGProperty* pPGProperty, guiex::CGUIWidget* pWidget)
{
	wxPGProperty* pPGTop = pSheetMgr->GetPropertyByName( STRING_M2W(pProp->GetName()));
	GUI_ASSERT( pPGTop || pPGProperty, "wrong parameter" );
	
	guiex::CGUIProperty* pGUIProperty = NULL;
	
	if( pWidget )
	{
		pGUIProperty = pWidget->GenerateProperty(pProp->GetName().c_str(), pProp->GetType().c_str());
		if( pGUIProperty )
		{
			pWidget->SetProperty( *pGUIProperty );
		}
		//if( !pGUIProperty && pGUIProperty)
		//{
		//	wxString strError = wxString::Format( _T("Failed to Generate GUI Property <%s :: %s> for widget <%s :: %s>)"),
		//		STRING_M2W(pProp->GetName()), 
		//		wxConvUTF8.cMB2WC(pProp->GetType().c_str()),
		//		wxConvUTF8.cMB2WC(pWidget->GetName().c_str()),
		//		wxConvUTF8.cMB2WC(pWidget->GetType().c_str()));
		//	wxMessageBox(strError, _T("Error"), wxICON_ERROR|wxCentre);
		//}
		//else
		//{
		//	pWidget->SetProperty( *pGUIProperty );
		//}
	}	
	


	//////////////////////////////////////////////////////////////////////////////////////
	//VECTOR2
	if( pProp->GetType() == "VECTOR2" )
	{
		guiex::CGUIVector2 aValue;
		if( pWidget )
		{
			if( pGUIProperty )
			{
				aValue = guiex::CGUIStringConvertor::StringToVector2(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToVector2(pProp->GetValue());
		}
			

		if( pPGTop )
		{
			pPGTop->SetValue( CGUIVector2ToVariant(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIVector2Property( STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//RECT
	else if( pProp->GetType() == "RECT" )
	{
		guiex::CGUIRect aValue;
		if( pWidget )
		{
			if( pGUIProperty )
			{
				aValue = guiex::CGUIStringConvertor::StringToRect(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToRect(pProp->GetValue());
		}
			

		if( pPGTop )
		{
			pPGTop->SetValue( CGUIRectToVariant(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIRectProperty( STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//COLOR
	else if( pProp->GetType() == "COLOR" )
	{
		wxColour aValue;

		if( pWidget )
		{
			if( pGUIProperty )
			{
				guiex::CGUIColor aColor = guiex::CGUIStringConvertor::StringToColor( pGUIProperty->GetValue() );
				aValue = wxColour(aColor.GetRed()*255.f,aColor.GetGreen()*255.f,aColor.GetBlue()*255.f,aColor.GetAlpha()*255.f);
			}
		}
		else
		{
			guiex::CGUIColor aColor = guiex::CGUIStringConvertor::StringToColor(pProp->GetValue());
			aValue = wxColour(aColor.GetRed()*255.f,aColor.GetGreen()*255.f,aColor.GetBlue()*255.f,aColor.GetAlpha()*255.f);
		}


		if( pPGTop )
		{
			pPGTop->SetValue( &aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxColourProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//SIZE
	else if( pProp->GetType() == "SIZE" )
	{
		guiex::CGUISize aValue;
		
		if( pWidget )
		{
			if( pGUIProperty )
			{
				aValue = guiex::CGUIStringConvertor::StringToSize(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToSize(pProp->GetValue());
		}

		if( pPGTop )
		{
			pPGTop->SetValue( CGUISizeToVariant(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUISizeProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//STRING
	else if( pProp->GetType() == "STRING" )
	{
		wxString aValue;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = wxConvUTF8.cMB2WC(pGUIProperty->GetValue().c_str());
			}
		}
		else
		{
			aValue = wxConvUTF8.cMB2WC(pProp->GetValue().c_str());
		}

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxStringProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//EVENT
	else if( pProp->GetType() == "EVENT" )
	{
		wxString aValue;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = wxConvUTF8.cMB2WC(pGUIProperty->GetValue().c_str());
			}
		}
		else
		{
			aValue = wxConvUTF8.cMB2WC(pProp->GetValue().c_str());
		}

		if( pPGTop )
		{
			pPGTop->SetValue( aValue );
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxStringProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//STRING_INFO
	else if(  pProp->GetType() == "STRING_INFO" )
	{
		guiex::CGUIStringExInfo aValue;

		if( pWidget )
		{
			if( pGUIProperty )
			{
				aValue.m_aColor = guiex::CGUIStringConvertor::StringToColor(pGUIProperty->GetProperty("COLOR")->GetValue());
				aValue.m_nFontIdx = guiex::CGUIStringConvertor::StringToInt(pGUIProperty->GetProperty("FONT_INDEX")->GetValue());
				aValue.m_nFontSize = guiex::CGUIStringConvertor::StringToUInt(pGUIProperty->GetProperty("SIZE")->GetValue());
			}
		}
		else
		{
			aValue.m_nFontSize = guiex::CGUIStringConvertor::StringToUInt(pProp->GetProperty("SIZE")->GetValue());
			aValue.m_nFontIdx = guiex::CGUIStringConvertor::StringToUInt(pProp->GetProperty("FONT_INDEX")->GetValue());
			aValue.m_aColor = guiex::CGUIStringConvertor::StringToColor(pProp->GetProperty("COLOR")->GetValue().c_str());
		}

		if( pPGTop )
		{
			pPGTop->SetValue( CGUIStringExInfoToVariant( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIStringInfoProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//WIDGET_POSITION
	else if(  pProp->GetType() == "WIDGET_POSITION" )
	{
		SWidgetPosition aValue;

		if( pWidget )
		{
			if( pGUIProperty )
			{
				aValue.m_bIsRelative = guiex::CGUIStringConvertor::StringToBool(pGUIProperty->GetProperty("RELATIVE")->GetValue());
				aValue.m_aPosition = guiex::CGUIStringConvertor::StringToVector2(pGUIProperty->GetProperty("POSITION")->GetValue());
			}
		}
		else
		{
			aValue.m_bIsRelative = guiex::CGUIStringConvertor::StringToBool(pProp->GetProperty("RELATIVE")->GetValue());
			aValue.m_aPosition = guiex::CGUIStringConvertor::StringToVector2(pProp->GetProperty("POSITION")->GetValue());
		}

		if( pPGTop )
		{
			pPGTop->SetValue( SWidgetPositionToVariant( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIWidgetPositionProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//WIDGET_SIZE
	else if(  pProp->GetType() == "WIDGET_SIZE" )
	{
		SWidgetSize aValue;

		if( pWidget )
		{
			if( pGUIProperty )
			{
				aValue.m_bIsRelative = guiex::CGUIStringConvertor::StringToBool(pGUIProperty->GetProperty("RELATIVE")->GetValue());
				aValue.m_aSize = guiex::CGUIStringConvertor::StringToSize(pGUIProperty->GetProperty("SIZE")->GetValue());
				aValue.m_aMaxSize = guiex::CGUIStringConvertor::StringToSize(pGUIProperty->GetProperty("MAX_SIZE")->GetValue());
				aValue.m_aMinSize = guiex::CGUIStringConvertor::StringToSize(pGUIProperty->GetProperty("MIN_SIZE")->GetValue());
			}
		}
		else
		{
			aValue.m_bIsRelative = guiex::CGUIStringConvertor::StringToBool(pProp->GetProperty("RELATIVE")->GetValue());
			aValue.m_aSize = guiex::CGUIStringConvertor::StringToSize(pProp->GetProperty("SIZE")->GetValue());
			aValue.m_aMaxSize = guiex::CGUIStringConvertor::StringToSize(pProp->GetProperty("MAX_SIZE")->GetValue());
			aValue.m_aMinSize = guiex::CGUIStringConvertor::StringToSize(pProp->GetProperty("MIN_SIZE")->GetValue());
		}

		if( pPGTop )
		{
			pPGTop->SetValue( SWidgetSizeToVariant( aValue ));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxGUIWidgetSizeProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), aValue));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//IMAGE
	else if(  pProp->GetType() == "IMAGE" )
	{			
		wxString aValue;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = wxConvUTF8.cMB2WC(pGUIProperty->GetValue().c_str());
			}
		}
		else
		{
			aValue = wxConvUTF8.cMB2WC(pProp->GetValue().c_str());
		}


		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()),s_arrayImageArray));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//ENUM
	else if(  pProp->GetType() == "ENUM" )
	{			
		wxString aValue;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = wxConvUTF8.cMB2WC(pGUIProperty->GetValue().c_str());
			}
		}
		else
		{
			aValue = wxConvUTF8.cMB2WC(pProp->GetValue().c_str());
		}


		if( pPGTop )
		{
			pPGTop->SetValue(aValue);
		}
		else
		{
			wxArrayString arrEnums;
			for( unsigned i=0; i<pProp->GetEnums().size(); ++i )
			{
				arrEnums.Add( STRING_M2W( pProp->GetEnums()[i] ));
			}
			pPGTop = pSheetMgr->Insert( pPGProperty, -1, new wxEnumProperty(STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()), arrEnums));
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//REAL
	else if(  pProp->GetType() == "REAL" )
	{			
		guiex::real aValue = 0.f;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = guiex::CGUIStringConvertor::StringToReal(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToReal(pProp->GetValue());
		}
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxFloatProperty( STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//UINT
	else if(  pProp->GetType() == "UINT" )
	{			
		guiex::uint32 aValue = 0;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = guiex::CGUIStringConvertor::StringToUInt(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToUInt(pProp->GetValue());
		}
		
		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, int(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxIntProperty( STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//BOOL
	else if(  pProp->GetType() == "BOOL" )
	{			
		bool aValue = false;

		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = guiex::CGUIStringConvertor::StringToBool(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToBool(pProp->GetValue());
		}

		if( pPGTop )
		{
			pSheetMgr->SetPropertyValue(pPGTop, aValue);
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxBoolProperty( STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()),aValue ) );
			pSheetMgr->SetPropertyAttribute( pPGTop,wxPG_BOOL_USE_CHECKBOX,(long)1 );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//UINT
	else if(  pProp->GetType() == "UINT" )
	{			
		guiex::uint32 aValue = 0;
		if( pWidget )
		{
			if( pGUIProperty)
			{
				aValue = guiex::CGUIStringConvertor::StringToUInt(pGUIProperty->GetValue());
			}
		}
		else
		{
			aValue = guiex::CGUIStringConvertor::StringToUInt(pProp->GetValue());
		}
			

		if( pPGTop )
		{
			pPGTop->SetValue( int(aValue));
		}
		else
		{
			pPGTop = pSheetMgr->Insert( pPGProperty,-1, new wxUIntProperty( STRING_M2W(pProp->GetLabel()), STRING_M2W(pProp->GetName()),aValue ) );
		}		
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//unknown
	else
	{
		std::string strError = std::string("unsupported property type:  ")+pProp->GetType();
		wxMessageBox(wxConvUTF8.cMB2WC(strError.c_str()), _T("Error"), wxICON_ERROR|wxCentre);
	}

	//set type as client data
	if( pPGTop)
	{
		pSheetMgr->SetPropertyClientData( pPGTop, CPropertyConfigMgr::Instance()->GetTypePtr(pProp->GetType()));

		if( pProp->IsMustExist())
		{
			pSheetMgr->SetPropertyTextColour( pPGTop, wxColour(255,0,0));
		}
	}

	//release gui property
	if( pGUIProperty )
	{
		guiex::CGUIPropertyManager::Instance()->DestroyProperty(pGUIProperty);
	}
}
//------------------------------------------------------------------------------
guiex::CGUIProperty*	GenerateGUIProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty )
{
	std::string  strName = pSheetMgr->GetPropertyName(pPGProperty).char_str(wxConvUTF8).data();
	std::string* pType = (std::string*)pSheetMgr->GetPropertyClientData( pPGProperty );
	wxASSERT( pType );

	guiex::CGUIProperty* pProperty = NULL;

	//////////////////////////////////////////////////////////////////////////////////////
	//VECTOR2
	if( *pType == "VECTOR2" )
	{
		guiex::CGUIVector2 aVector = CGUIVector2FromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			guiex::CGUIStringConvertor::Vector2ToString(aVector));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//RECT
	else if( *pType == "RECT" )
	{
		guiex::CGUIRect aRect = CGUIRectFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			guiex::CGUIStringConvertor::RectToString(aRect));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//REAL
	else if( *pType == "REAL" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data());
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//UINT
	else if( *pType == "UINT" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			guiex::CGUIStringConvertor::UIntToString(pSheetMgr->GetPropertyValueAsInt(pPGProperty)));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//BOOL
	else if( *pType == "BOOL" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data());
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//SIZE
	else if( *pType == "SIZE" )
	{
		guiex::CGUISize aSize = CGUISizeFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			guiex::CGUIStringConvertor::SizeToString(aSize));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//EVENT
	else if( *pType == "EVENT" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data());
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//STRING
	else if( *pType == "STRING" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data());
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//IMAGE
	else if( *pType == "IMAGE" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data());
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//ENUM
	else if( *pType == "ENUM" )
	{
		pProperty = guiex::CGUIPropertyManager::Instance()->CreateProperty( 
			strName.c_str(), 
			pType->c_str(), 
			pSheetMgr->GetPropertyValueAsString( pPGProperty ).char_str(wxConvUTF8).data());
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//STRING_INFO
	else if(  *pType == "STRING_INFO" )
	{
		guiex::CGUIStringExInfo aStringInfo = CGUIStringExInfoFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		
		pProperty = guiex:: CGUIPropertyManager::Instance()->CreateProperty(strName.c_str(), pType->c_str());
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("SIZE","UINT",guiex::CGUIStringConvertor::UIntToString(aStringInfo.m_nFontSize)));
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("FONT_INDEX","UINT",guiex::CGUIStringConvertor::UIntToString(aStringInfo.m_nFontIdx)));
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("COLOR","COLOR",guiex::CGUIStringConvertor::ColorToString(aStringInfo.m_aColor)));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//WIDGET_POSITION
	else if(  *pType == "WIDGET_POSITION" )
	{
		SWidgetPosition aWidgetPos = SWidgetPositionFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		
		pProperty = guiex:: CGUIPropertyManager::Instance()->CreateProperty(strName.c_str(), pType->c_str());
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("RELATIVE","BOOL",guiex::CGUIStringConvertor::BoolToString(aWidgetPos.m_bIsRelative)));
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("POSITION","VECTOR2",guiex::CGUIStringConvertor::Vector2ToString(aWidgetPos.m_aPosition)));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//WIDGET_SIZE
	else if(  *pType == "WIDGET_SIZE" )
	{
		SWidgetSize aWidgetSize = SWidgetSizeFromVariant( pSheetMgr->GetPropertyValue( pPGProperty ));
		
		pProperty = guiex:: CGUIPropertyManager::Instance()->CreateProperty(strName.c_str(), pType->c_str());
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("RELATIVE","BOOL",guiex::CGUIStringConvertor::BoolToString(aWidgetSize.m_bIsRelative)));
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("SIZE","SIZE",guiex::CGUIStringConvertor::SizeToString(aWidgetSize.m_aSize)));
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("MAX_SIZE","SIZE",guiex::CGUIStringConvertor::SizeToString(aWidgetSize.m_aMaxSize)));
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("MIN_SIZE","SIZE",guiex::CGUIStringConvertor::SizeToString(aWidgetSize.m_aMinSize)));
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//COLOR
	else if(  *pType == "COLOR" )
	{
		//COLOR
		wxColour aWxColor = wxColourPropertyValueFromVariant(pSheetMgr->GetPropertyValue(pPGProperty)).m_colour;
		guiex::CGUIColor aColor(aWxColor.Red() / 255.0, aWxColor.Green() / 255.0, aWxColor.Blue() / 255.0, aWxColor.Alpha() / 255.0 );
		pProperty->AddProperty(guiex::CGUIPropertyManager::Instance()->CreateProperty("COLOR","COLOR",guiex::CGUIStringConvertor::ColorToString(aColor)));
	
	}
	//////////////////////////////////////////////////////////////////////////////////////
	//unknown
	else
	{
		std::string strError = std::string("unsupported property type:  ")+pType->c_str();
		wxMessageBox(wxConvUTF8.cMB2WC(strError.c_str()), _T("Error"), wxICON_ERROR|wxCENTRE);
		throw guiex::CGUIException("unsupported property type: <%s>", pType->c_str());
	}

	return pProperty;
}
//------------------------------------------------------------------------------

