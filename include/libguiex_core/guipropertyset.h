/** 
* @file guipropertyset.h
* @brief the property set, contain the config of the widget
* @author ken
* @date 2006-09-12
*/

#ifndef __GUI_PROPERTYSET_20060912_H__
#define __GUI_PROPERTYSET_20060912_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIProperty;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	///**
	//* @class CGUIPropertySet
	//* @brief a property set
	//* @exception CGUIException
	//*/
	//class GUIEXPORT CGUIPropertySet
	//{
	//public:
	//	/**
	//	* @brief constructor
	//	*/
	//	CGUIPropertySet();

	//	/**
	//	* @brief destructor
	//	*/
	//	~CGUIPropertySet();

	//	/// copy constructor
	//	CGUIPropertySet(const CGUIPropertySet& rPropertySet );

	//	/// assign operator
	//	void operator=(const CGUIPropertySet& rPropertySet );

	//	/// get property size
	//	uint32	GetSize() const;

	//	/// clear all property
	//	void			Clear();

	//	/// get property by index
	//	const CGUIProperty*	GetProperty(uint32 nIdx) const;

	//	/// get property by index
	//	CGUIProperty*	GetProperty(uint32 nIdx);

	//	/// get property by name
	//	const CGUIProperty*	GetProperty(const CGUIString& rName) const;

	//	/// get property by name
	//	CGUIProperty*	GetProperty(const CGUIString& rName);

	//	int32		GetPropertyIndex( const CGUIString& rName ) const;


	//	/// whether this set contains the property which has this name
	//	bool		HasProperty(const CGUIString& rName) const;

	//	/// add a property to list
	//	void		AddProperty(CGUIProperty* pProperty);
	//	
	//	void		AddProperty(const CGUIString& rName, const CGUIString& rType, const CGUIString& rValue);

	//	/// add a property set to list
	//	void		AddPropertySet(const CGUIPropertySet& rPropertySet);

	//	/// remove a property from list
	//	void		RemoveProperty(const CGUIProperty* pProperty);

	//	void				SetData( void* pData )
	//	{
	//		m_pExtraData = pData;
	//	}

	//	void*				GetData( )
	//	{
	//		return m_pExtraData;
	//	}

	//protected:

	//protected:
	//	typedef std::vector<CGUIProperty*>	TSetProperty;
	//	TSetProperty	m_setProperty;	
	//	void*			m_pExtraData;
	//};

}//namespace guiex

#endif //__GUI_PROPERTYSET_20060912_H__
