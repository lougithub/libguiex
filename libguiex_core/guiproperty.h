/** 
 * @file guiproperty.h
 * @brief the property of the widget, each kind of widget has
 * it's own property
 * @author ken
 * @date 2006-06-21
 */

#ifndef __GUI_PROPERTY_20060621_H__
#define __GUI_PROPERTY_20060621_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include <vector>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{


	/**
	* @class CGUIProperty
	* @brief store property used for create widget.
	* widget could load property and store them.
	* the format of property is describe below
	* @exception CGUIException
	*/
	class GUIEXPORT CGUIProperty
	{
	public:
		CGUIProperty(const CGUIString& rName=CGUIString(), const CGUIString& rType=CGUIString(), const CGUIString& rValue=CGUIString() );
		CGUIProperty(const CGUIProperty& other );
		~CGUIProperty();

		void Clear();

		void SetName(const CGUIString& rName);
		const CGUIString& GetName() const;

		void SetValue(const CGUIString& rValue);
		const CGUIString& GetValue() const;

		void SetType(const CGUIString& rType);
		uint32 GetType() const;
		const CGUIString& GetTypeAsString() const;

		void AddProperty(  const CGUIProperty& rProperty );
		void RemoveProperty( int32 nIdx );
		void RemoveProperty( const CGUIProperty& rProperty );
		void InsertProperty(  const CGUIProperty& rProperty, int32 nIndex );
		uint32 GetPropertyNum( ) const;
		int32 GetPropertyIndex( const CGUIString& rName ) const;
		int32 GetPropertyIndex( const CGUIString& rName, const CGUIString& rType ) const;
		const CGUIProperty*	GetProperty( uint32 nIdx ) const;
		CGUIProperty* GetProperty( uint32 nIdx );
		//const CGUIProperty* GetProperty( const CGUIString& rName ) const;
		//CGUIProperty* GetProperty( const CGUIString& rName );
		const CGUIProperty*	GetProperty( const CGUIString& rName, uint32 nType ) const;
		CGUIProperty* GetProperty( const CGUIString& rName, uint32 nType );
		const CGUIProperty*	GetProperty( const CGUIString& rName, const CGUIString& rType ) const;
		CGUIProperty* GetProperty( const CGUIString& rName, const CGUIString& rType );
		//bool HasProperty( const CGUIString& rName ) const;
		bool HasProperty( const CGUIString& rName, uint32 nType ) const;
		bool HasProperty( const CGUIString& rName, const CGUIString& rType ) const;

		//set extra data
		void SetData( void* pData );
		void* GetData( ) const;

		void operator=(const CGUIProperty& other );
		bool operator==(const CGUIProperty& other ) const;

		bool HasDuplicatedNames( );

	protected:
		CGUIString m_strName; /// name of this property
		CGUIString m_strType; /// type of this property as a string value
		uint32 m_uType; /// type of this property as a enum value
		CGUIString m_strValue; /// value of this property

		typedef std::vector<CGUIProperty> TSetProperty;
		TSetProperty m_setProperty;
		void* m_pExtraData;
	};

}//namespace guiex

#endif //__GUI_PROPERTY_20060621_H__
