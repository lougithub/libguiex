/** 
* @file propertyconvertor.h
* @brief 
* @author ken
* @date 2011-01-08
*/

#ifndef __KEN_PROPERTYCONVERTOR_20110108_H__
#define __KEN_PROPERTYCONVERTOR_20110108_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include <libguiex_core/guiex.h>

using namespace guiex;

//============================================================================//
// class
//============================================================================// 
class CPropertyConvertorMgr
{
public:
	static CPropertyConvertorMgr* Instance();

	void GuiProperty2GridProperty( class wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, const CGUIProperty& aProp, bool bEnableIdenticalName = false );
	void GridProperty2GuiProperty( class wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty );
	wxPGProperty* CreateGridProperty( const CGUIProperty& aProp );

	void GuiProperty2GridPropertyRow( class wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGCategory, const CGUIProperty& aProp, bool bEnableIdenticalName = false );
	wxPGProperty* CreateGridPropertyRow( const CGUIProperty& aProp );

protected:
	CPropertyConvertorMgr();
	~CPropertyConvertorMgr();

	void RegisterConvertor( class CEditorPropertyConvertorBase* pConvertor );

protected:
	typedef std::map<int32, class CEditorPropertyConvertorBase*> TMapConvertor;
	TMapConvertor m_mapConvertor;
};

wxString GetUniquePropName();

#endif	//__KEN_PROPERTYCONVERTOR_20110108_H__
