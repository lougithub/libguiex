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
class CProperty;
class TiXmlElement;




//============================================================================//
// class
//============================================================================// 
class CProperty
{
public:
	/**
	* @brief constructor
	*/
	CProperty();

	/**
	* @brief destructor
	*/
	~CProperty();

	void	Clear( );

	/// set name of this property
	void				SetName(const std::string& rName);

	/// get name of this property
	const std::string&		GetName() const;

	/// set value of this property
	void				SetValue(const std::string& rValue);

	/// get value of this property
	const std::string&		GetValue() const;

	/// set type of this property
	void				SetType(const std::string& rType);

	/// get type of this property
	const std::string&		GetType() const;

	/// set label of this property
	void				SetLabel(const std::string& rType);

	/// get label of this property
	const std::string&		GetLabel() const;

	/// set category of this property
	void				SetCategory(const std::string& rCategory);

	/// get category of this property
	const std::string&	GetCategory() const;

	/// set this property must exist
	void				SetMustExist(bool bMustExist);

	/// whether this property must exist
	bool				IsMustExist() const;

	/// set page of this property
	void				SetPage(const std::string& rPage);

	/// get page of this property
	const std::string&		GetPage() const;

	void	SetEnums(const std::vector<std::string>& rEnum);
	const std::vector<std::string>&		GetEnums() const;


	/// add a sub-property
	void				AddProperty(  const CProperty& rProperty );

	void				InsertProperty(  const CProperty& rProperty, guiex::int32 nIndex );

	/// get a sub-property num
	unsigned int				GetPropertyNum( ) const;

	///get a sub-property by index
	const CProperty*	GetProperty( unsigned int nIdx ) const;

	///get a sub-property by name
	const CProperty*	GetProperty( const std::string& rName ) const;

	///whether this sub-property contain this property
	bool				HasProperty( const std::string& rName ) const;

	guiex::int32		GetPropertyIndex( const std::string& rName ) const;

	void				RemoveProperty( const std::string& rName );


protected:
	std::string		m_strName;		/// name of this property
	std::string		m_strType;		/// type of this property
	std::string		m_strLabel;		/// label of this property
	std::string		m_strValue;		/// value of this property
	std::string		m_strPage;		/// page of property
	std::string		m_strCategories;/// categories of property
	bool			m_bMustExist;	/// whether this property must exist
	std::vector<std::string>	m_vecEnums;

	typedef std::vector<CProperty>	TSetProperty;
	TSetProperty	m_setProperty;	/// sub-property of this property
};



//============================================================================//
// class
//============================================================================// 

class CPropertyConfigMgr
{
public:
	~CPropertyConfigMgr();

	/** 
	* @brief get singleton of project config object
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
	* @brief register a property set
	*/
	void	RegisterSet( const std::string& rSetName, const CProperty& rProperty );


	/**
	* @brief get a property set
	* @return NULL for failed to find this kind of property
	*/
	const CProperty*	GetPropertySet(const std::string& rSetName ) const;

	/** 
	 * @brief get property set map
	 */
	const std::map<std::string, CProperty>& 	GetPropertySetMap( ) const;

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

	int		ProcessPropertyNode(const std::string& rPage, CProperty* pPropertySet, TiXmlElement* pNode);

protected:
	CPropertyConfigMgr();
	CPropertyConfigMgr& operator=(CPropertyConfigMgr&);

protected:
	typedef std::map<std::string, CProperty>		TMapPropertySet;
	TMapPropertySet		m_mapPropertySet;

	typedef	std::map<std::string, std::string*>		TSetType;
	TSetType		m_mapType;
};


#endif //__KEN_PROPERTYCONFIGMANAGER_20070919_H__
