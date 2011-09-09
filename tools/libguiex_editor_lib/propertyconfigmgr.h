/** 
* @file propertyconfigmgr.h
* @brief used to manage property
* @author ken
* @date 2007-09-19
*/

#ifndef __KEN_PROPERTYCONFIGMANAGER_20070919_H__
#define __KEN_PROPERTYCONFIGMANAGER_20070919_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <libguiex_core/guiex.h>
#include <tinyxml.h>

#include <map>
#include <vector>
#include <set>
#include <string>

using namespace guiex;

//============================================================================//
// class
//============================================================================// 
class CPropertyData
{
public:
	CPropertyData();

	static const CPropertyData* GetPropertyData( const CGUIProperty& rProperty );
	static wxString GetPropertyLabel( const CGUIProperty& rProperty );

	void SetLabel(const std::string& rType);
	const std::string& GetLabel() const;
	void SetCategory(const std::string& rCategory);
	const std::string&	GetCategory() const;
	void SetMustExist(bool bMustExist);
	bool IsMustExist() const;
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;
	void SetAlternaitiveSave(bool bAlternativeSave );
	bool IsAlternaitiveSave() const;
	void SetPage(const std::string& rPage);
	const std::string& GetPage() const;

protected:
	std::string m_strLabel; /// label of this property
	std::string m_strPage; /// page of property
	std::string m_strCategories;/// categories of property
	bool m_bMustExist; /// whether this property must exist
	bool m_bReadOnly;
	bool m_bAlternativeSave;
};



class CPropertyConfigMgr
{
public:
	typedef std::map<std::string, CGUIProperty> TMapPropertySet;
	typedef	std::vector<std::string> TSetType;

public:
	~CPropertyConfigMgr();
	static CPropertyConfigMgr* Instance();

	void Clear();

	int	ReadPropertyConfig(const wxString& rFileName);
	int	ReadLocalizationConfig(const std::string& rFileName);

	const CGUIProperty&	GetPropertySet(const std::string& rSetName ) const;
	bool HasPropertySet(const std::string& rSetName ) const;

	const wxArrayString& GetEnumDefine(const CGUIString& rEnumName ) const;

	const TMapPropertySet& GetPropertySetMap( ) const;
	const TSetType& GetWidgetTypes( ) const;
	const std::vector<wxString>& GetLocalizations() const;

	void AddType( const std::string& rType );
	const std::string* GetTypePtr( const std::string& rType );

protected:
	int	ProcessWidgetNode(TiXmlElement* pWidgetNode);
	int	ProcessEnumNode(TiXmlElement* pWidgetNode);

	int	ProcessPropertyNode(const std::string& rPage, CGUIProperty& rPropertySet, TiXmlElement* pNode);

	void RegisterSet( const std::string& rSetName, const CGUIProperty& rProperty, bool bIsTemplate );

	void RegisterEnumDefine( const CGUIString& rEnumName, const wxArrayString& rEnumValue );

protected:
	CPropertyConfigMgr();
	CPropertyConfigMgr& operator=(CPropertyConfigMgr&);

protected:
	TMapPropertySet m_mapPropertySet;

	std::set<std::string> m_setType;
	TSetType m_setWidgetTyps;
	std::vector<CPropertyData*>	m_arrayPropertyDataCache;
	std::vector<wxString> m_arrayLocalizations;

	typedef	std::map<CGUIString, wxArrayString> TEnumMap;
	TEnumMap m_mapEnums;

};

#endif //__KEN_PROPERTYCONFIGMANAGER_20070919_H__
