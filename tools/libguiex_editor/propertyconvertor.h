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

	void RegisterConvertor( class CEditorPropertyConvertorBase* pConvertor );

	void GuiProperty2GridProperty( class WxToolsPGManager* pSheetMgr, wxPGProperty* pPGCategory, const CGUIProperty& aProp );
	void GridProperty2GuiProperty( class WxToolsPGManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty );


protected:
	CPropertyConvertorMgr();
	~CPropertyConvertorMgr();

protected:
	typedef std::map<int32, class CEditorPropertyConvertorBase*> TMapConvertor;
	TMapConvertor m_mapConvertor;
};

#endif	//__KEN_PROPERTYCONVERTOR_20110108_H__
