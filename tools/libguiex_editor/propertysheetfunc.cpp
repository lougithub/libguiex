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
#include "wxtoolspgmanager.h"
#include "propertyconvertor.h"

//============================================================================//
// declare
//============================================================================// 


//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
std::vector<CGUIString> g_vecPropertyPages;

//------------------------------------------------------------------------------
void UpdateGridProperties( WxToolsPGManager* pSheetMgr, const std::string& rType, CGUIWidget* pWidget )
{
	//get property set
	if( pWidget )
	{
		pWidget->DumpToProperty();
	}
	const CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet(rType);

	//save current page index
	int nOldPageIdx = pSheetMgr->GetSelectedPage( );

	//process page
	std::map<std::pair<std::string,std::string>, wxPGProperty*>	mapCategory;
	for(unsigned int i=0; i<rSet.GetPropertyNum();++i)
	{
		const CGUIProperty* pProp = rSet.GetProperty(i);
		CGUIProperty aGUIProp = *pProp;
		if( pWidget )
		{
			//load property from widget
			const CGUIProperty* pGUIProp = pWidget->GetProperty().GetProperty( pProp->GetName(), pProp->GetType() );
			if( pGUIProp )
			{
				aGUIProp = *pGUIProp;
				aGUIProp.SetData( pProp->GetData() );
			}
		}
		
		//select page
		pSheetMgr->ToolsSelectPage(CPropertyData::GetPropertyData(*pProp)->GetPage());
		
		//add category
		std::string strPage = CPropertyData::GetPropertyData(*pProp)->GetPage();
		std::string strCategory = CPropertyData::GetPropertyData(*pProp)->GetCategory();
		if( mapCategory.find( std::make_pair(strPage,strCategory)) == mapCategory.end())
		{
			wxString strLabel = Gui2wxString( CPropertyData::GetPropertyData(*pProp)->GetCategory());
			wxPGProperty* pPGProperty = pSheetMgr->Append(new wxPropertyCategory(strLabel, wxPG_LABEL));
			wxASSERT(pPGProperty);

			mapCategory.insert( std::make_pair(std::make_pair(strPage,strCategory), pPGProperty));
		}

		//add property
		wxPGProperty* pPGCategory = mapCategory[std::make_pair(strPage,strCategory)];
		CPropertyConvertorMgr::Instance()->GuiProperty2GridProperty( pSheetMgr, pPGCategory, aGUIProp );
	}
	
	//restore old page index
	pSheetMgr->SelectPage(nOldPageIdx);
	
	//pSheetMgr->ExpandAll();
}
//------------------------------------------------------------------------------
void GenerateGUIProperties( WxToolsPGManager* pSheetMgr, CGUIProperty& rSet )
{
	for ( wxPGVIterator it = pSheetMgr->GetVIterator(wxPG_ITERATE_PROPERTIES);
		!it.AtEnd();
		it.Next() )
	{
		CGUIProperty aProp;
		CPropertyConvertorMgr::Instance()->GridProperty2GuiProperty(pSheetMgr, it.GetProperty(), aProp );		
		rSet.AddProperty( aProp );
	}
}
//------------------------------------------------------------------------------
void UpdateGridAndGuiProperty( WxToolsPGManager* pSheetMgr, CGUIWidget* pWidget, const CGUIString& rPropertyName, const CGUIString& rPropertyType )
{
	const CGUIProperty& rSet = CPropertyConfigMgr::Instance()->GetPropertySet(pWidget->GetType());

	const CGUIProperty* pDefaultProp = rSet.GetProperty(rPropertyName, rPropertyType);
	if( !pDefaultProp )
	{
		throw CGUIException( "[UpdateGridAndGuiProperty]: failed to get default property by name <%s>", rPropertyName.c_str() );
	}

	const CGUIProperty* pProp = pWidget->GetProperty().GetProperty(rPropertyName, rPropertyType);
	if( !pProp )
	{
		throw CGUIException( "[UpdateGridAndGuiProperty]: failed to get property by name <%s> from widget <%s>", rPropertyName.c_str(), pWidget->GetName().c_str() );
	}

	CGUIProperty aProp = *pProp;
	pWidget->GenerateProperty( aProp );
	pWidget->InsertProperty( aProp );

	aProp.SetData( pDefaultProp->GetData() );
	CPropertyConvertorMgr::Instance()->GuiProperty2GridProperty( pSheetMgr, NULL, aProp );
}
//------------------------------------------------------------------------------

