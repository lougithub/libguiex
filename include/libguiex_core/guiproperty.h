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
// declare
//============================================================================// 
namespace guiex
{

}


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
		/**
		* @brief constructor
		*/
		CGUIProperty(const CGUIString& rName=CGUIString(), const CGUIString& rType=CGUIString(), const CGUIString& rValue=CGUIString() );

		/**
		* @brief destructor
		*/
		~CGUIProperty();

		/// copy constructor
		CGUIProperty(const CGUIProperty& other );

		/// assign operator
		void operator=(const CGUIProperty& other );
		bool operator==(const CGUIProperty& other ) const;

		/// clear all sub property
		void Clear();

		/// set name of this property
		void SetName(const CGUIString& rName);

		/// get name of this property
		const CGUIString&	GetName() const;

		/// set value of this property
		void SetValue(const CGUIString& rValue);

		/// get value of this property
		const CGUIString&	GetValue() const;

		/// set type of this property
		void SetType(const CGUIString& rType);

		/// get type of this property
		uint32	GetType() const;
		const CGUIString& GetTypeAsString() const;

		/// add a sub-property
		void AddProperty(  const CGUIProperty& rProperty );
		void RemoveProperty( int32 nIdx );
		void RemoveProperty( const CGUIProperty& rProperty );
		void InsertProperty(  const CGUIProperty& rProperty, int32 nIndex );

		/// get a sub-property num
		uint32 GetPropertyNum( ) const;

		int32 GetPropertyIndex( const CGUIString& rName ) const;

		///get a sub-property by index
		const CGUIProperty*	GetProperty( uint32 nIdx ) const;


		///get a sub-property by name
		const CGUIProperty*	GetProperty( const CGUIString& rName ) const;
		CGUIProperty*	GetProperty( const CGUIString& rName );

		///whether this sub-property contain this property
		bool HasProperty( const CGUIString& rName ) const;

		void SetData( void* pData )
		{
			m_pExtraData = pData;
		}

		void* GetData( ) const
		{
			return m_pExtraData;
		}


	protected:

	protected:
		CGUIString		m_strName;		/// name of this property
		CGUIString		m_strType;		/// type of this property as a string value
		uint32			m_uType;		/// type of this property as a enum value
		CGUIString		m_strValue;		/// value of this property

		typedef std::vector<CGUIProperty>	TSetProperty;
		TSetProperty	m_setProperty;
		//CGUIPropertySet		m_setProperty;	/// sub-property of this property

		void*			m_pExtraData;

	};

}//namespace guiex

#endif //__GUI_PROPERTY_20060621_H__
