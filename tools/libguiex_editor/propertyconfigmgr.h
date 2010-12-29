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

using namespace guiex;

//============================================================================//
// declare
//============================================================================// 
class CPropertyData
{
public:
	static const CPropertyData* GetPropertyData( const CGUIProperty& rProperty );

	static wxString GetPropertyLabel( const CGUIProperty& rProperty );

	CPropertyData()
		:m_bMustExist(false)
	{

	}

	void SetLabel(const std::string& rType)
	{
		m_strLabel = rType;
	}
	const std::string& GetLabel() const
	{
		return m_strLabel;
	}

	void SetCategory(const std::string& rCategory)
	{
		m_strCategories = rCategory;
	}
	const std::string&	GetCategory() const
	{
		return m_strCategories;
	}

	void SetMustExist(bool bMustExist)
	{
		m_bMustExist = bMustExist;
	}
	bool IsMustExist() const
	{
		return m_bMustExist;
	}
	void SetReadOnly(bool bReadOnly)
	{
		m_bReadOnly = bReadOnly;
	}
	bool IsReadOnly() const
	{
		return m_bReadOnly;
	}	

	void SetPage(const std::string& rPage)
	{
		m_strPage = rPage;
	}
	const std::string& GetPage() const
	{
		return m_strPage;
	}

protected:
	std::string m_strLabel; /// label of this property
	std::string m_strPage; /// page of property
	std::string m_strCategories;/// categories of property
	bool m_bMustExist; /// whether this property must exist
	bool m_bReadOnly;
};


//============================================================================//
// class
//============================================================================// 

class CPropertyConfigMgr
{
public:
	typedef std::map<std::string, CGUIProperty> TMapPropertySet;
	typedef	std::vector<std::string> TSetType;

public:
	~CPropertyConfigMgr();
	static CPropertyConfigMgr* Instance();

	/** 
	 * @brief clear all property
	 */
	void Clear();

	/** 
	 * @brief read property config from file
	 * @return 0 for success, vice versa
	 */
	int	ReadPropertyConfig(const std::string& rFileName);

	/**
	* @brief get a property set
	* @return NULL for failed to find this kind of property
	*/
	const CGUIProperty&	GetPropertySet(const std::string& rSetName ) const;
	
	const wxArrayString& GetEnumDefine(const CGUIString& rEnumName ) const;

	/** 
	 * @brief get property set map
	 */
	const TMapPropertySet& 	GetPropertySetMap( ) const;
	const TSetType& GetWidgetTypes( ) const
	{
		return m_setWidgetTyps;
	}

	//add a page
	void AddType( const std::string& rType );

	//get type ptr
	std::string* GetTypePtr( const std::string& rType );

protected:
	/** 
	 * @brief process widget node
	 * @return 0 for success, vice versa
	 */
	int	ProcessWidgetNode(TiXmlElement* pWidgetNode);
	int	ProcessEnumNode(TiXmlElement* pWidgetNode);

	int	ProcessPropertyNode(const std::string& rPage, CGUIProperty& rPropertySet, TiXmlElement* pNode);

	/**
	* @brief register a property set
	*/
	void RegisterSet( const std::string& rSetName, const CGUIProperty& rProperty, bool bIsTemplate );

	void RegisterEnumDefine( const CGUIString& rEnumName, const wxArrayString& rEnumValue );

protected:
	CPropertyConfigMgr();
	CPropertyConfigMgr& operator=(CPropertyConfigMgr&);

protected:
	TMapPropertySet m_mapPropertySet;

	typedef	std::map<std::string, std::string*> TMapType;
	TMapType m_mapType;

	TSetType m_setWidgetTyps;

	std::vector<CPropertyData*>	m_arrayPropertyDataCache;

	typedef	std::map<CGUIString, wxArrayString> TEnumMap;
	TEnumMap m_mapEnums;
};

#endif //__KEN_PROPERTYCONFIGMANAGER_20070919_H__
