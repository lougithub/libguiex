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
#include "guipropertyset.h"


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
	*
	*
	*-----string-----
	*<property name="PARENT" type="STRING" value="sample_frame1" />
	*
	*-----rect-----
	*<property name="CLIENT_AREA" type="RECT" value="10,10,50,50" />
	*
	*-----size-----
	*<property name="SIZE" type="SIZE" value="800,600" />
	*
	*-----vector2-----
	*<property name="LOCAL_POSITION" type="VECTOR2" value="200,100"/>
	*
	* -----image------
	* <property name="BGIMAGE" type="IMAGE">
	*		<property name="PATH" type="STRING" value="./data/sample.png"/>
	*		<property name="UV" type="RECT" value="0,0,0.5,1" />
	*		<property name="IMAGE_OPERATION" type="ENUM" value="IMAGE_NONE" />
	*	</property>
	*
	*-----text------
	*<property name="TEXT" type="TEXT">
	*		<property name="SIZE"	type="NUMBER" value="22"/>
	*		<property name="FONT_INDEX" type="NUMBER" value="0"/>
	*		<property name="COLOR" type="COLOR" value="0.5,0.0,0.6"/>
	*		<property name="CONTENT"	type="STRING" value="content"/>
	*</property>
	*
	*-----sound-----
	*<property name="OnOpen" type="SOUND" value="3" />
	*
	*-----script event------
	*<property name="OnOpen" type="EVENT" value="btn_ok_onopen" />
	*
	*
	*
	* @exception CGUIException
	*/
	class GUIEXPORT CGUIProperty
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIProperty();

		/**
		* @brief destructor
		*/
		~CGUIProperty();

		/// copy constructor
		CGUIProperty(const CGUIProperty& rProperty );

		/// assign operator
		void operator=(const CGUIProperty& rProperty );

		/// set name of this property
		void				SetName(const CGUIString& rName);

		/// get name of this property
		const CGUIString&	GetName() const;

		/// set value of this property
		void				SetValue(const CGUIString& rValue);

		/// get value of this property
		const CGUIString&	GetValue() const;

		/// set type of this property
		void				SetType(const CGUIString& rType);

		/// get type of this property
		const CGUIString&	GetType() const;

		/// clone a property
		CGUIProperty*		Clone( ) const;

		/// add a sub-property
		void				AddProperty(  CGUIProperty* pProperty );

		/// add a property set as sub-property
		void				AddPropertySet(const CGUIPropertySet& rPropertySet);

		/// get a sub-property num
		uint32				GetPropertyNum( ) const;

		///get a sub-property by index
		const CGUIProperty*	GetProperty( uint32 nIdx ) const;

		///get a sub-property by name
		const CGUIProperty*	GetProperty( const CGUIString& rName ) const;

		///whether this sub-property contain this property
		bool				HasProperty( const CGUIString& rName ) const;


	protected:

	protected:
		CGUIString		m_strName;		/// name of this property
		CGUIString		m_strType;		/// type of this property
		CGUIString		m_strValue;		/// value of this property

		CGUIPropertySet		m_setProperty;	/// sub-property of this property

	};

}//namespace guiex

#endif //__GUI_PROPERTY_20060621_H__
