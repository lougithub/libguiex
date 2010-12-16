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

#include <map>
#include <vector>
#include <set>

//============================================================================//
// declare
//============================================================================// 
//class CProperty;
class TiXmlElement;


class CPropertyData
{
public:
	static const CPropertyData* GetPropertyData( const guiex::CGUIProperty& rProperty );

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

	void SetPage(const std::string& rPage)
	{
		m_strPage = rPage;
	}
	const std::string& GetPage() const
	{
		return m_strPage;
	}

protected:
	std::string		m_strLabel;		/// label of this property
	std::string		m_strPage;		/// page of property
	std::string		m_strCategories;/// categories of property
	bool			m_bMustExist;	/// whether this property must exist
};


//============================================================================//
// class
//============================================================================// 

class CPropertyConfigMgr
{
public:
	typedef std::map<std::string, guiex::CGUIProperty>		TMapPropertySet;

public:
	~CPropertyConfigMgr();

	/** 
	* @brief get singleton of Scene config object
	*/
	static CPropertyConfigMgr* Instance();

	/** 
	 * @brief clear all property
	 */
	void	Clear();

	/** 
	 * @brief read property config from file
	 * @return 0 for success, vice versa
	 */
	int		ReadPropertyConfig(const std::string& rFileName);

	/**
	* @brief get a property set
	* @return NULL for failed to find this kind of property
	*/
	const guiex::CGUIProperty&	GetPropertySet(const std::string& rSetName ) const;
	
	const wxArrayString&	GetEnumDefine(const guiex::CGUIString& rEnumName ) const;

	/** 
	 * @brief get property set map
	 */
	const TMapPropertySet& 	GetPropertySetMap( ) const;

	//add a page
	void	AddType( const std::string& rType );

	//get type ptr
	std::string*	GetTypePtr( const std::string& rType );

protected:
	/** 
	 * @brief process widget node
	 * @return 0 for success, vice versa
	 */
	int		ProcessWidgetNode(TiXmlElement* pWidgetNode);
	int		ProcessEnumNode(TiXmlElement* pWidgetNode);

	int		ProcessPropertyNode(const std::string& rPage, guiex::CGUIProperty& rPropertySet, TiXmlElement* pNode);

	/**
	* @brief register a property set
	*/
	void	RegisterSet( const std::string& rSetName, const guiex::CGUIProperty& rProperty );

	void	RegisterEnumDefine( const guiex::CGUIString& rEnumName, const wxArrayString& rEnumValue );

protected:
	CPropertyConfigMgr();
	CPropertyConfigMgr& operator=(CPropertyConfigMgr&);

protected:
	TMapPropertySet		m_mapPropertySet;

	typedef	std::map<std::string, std::string*>		TSetType;
	TSetType		m_mapType;

	std::vector<CPropertyData*>	m_arrayPropertyDataCache;

	typedef	std::map<guiex::CGUIString, wxArrayString>		TEnumMap;
	TEnumMap m_mapEnums;
};


#endif //__KEN_PROPERTYCONFIGMANAGER_20070919_H__
