/** 
 * @file guiproperty.cpp
 * @brief the property of the widget, each kind of widget has
 * it's own property
 * @author ken
 * @date 2006-06-21
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guiexception.h>


//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------ 

namespace guiex
{
//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
CGUIProperty::CGUIProperty()
{
}
//------------------------------------------------------------------------------
CGUIProperty::~CGUIProperty()
{
}
//------------------------------------------------------------------------------
CGUIProperty::CGUIProperty(const CGUIProperty& rProperty )
{
	*this = rProperty;
}
//------------------------------------------------------------------------------
void CGUIProperty::operator=(const CGUIProperty& rProperty )
{
	if( this != &rProperty )
	{
		m_strName = rProperty.m_strName;		/// name of this property
		m_strType = rProperty.m_strType;		/// type of this property
		m_strValue = rProperty.m_strValue;		/// value of this property

		m_setProperty = rProperty.m_setProperty;	/// sub-property of this property
	}
}
//------------------------------------------------------------------------------
void					CGUIProperty::SetName(const CGUIString& rName)
{
	m_strName = rName;
}
//------------------------------------------------------------------------------
const CGUIString&	CGUIProperty::GetName() const
{
	return m_strName;
}
//------------------------------------------------------------------------------
void					CGUIProperty::SetValue(const CGUIString& rValue)
{
	m_strValue = rValue;
}
//------------------------------------------------------------------------------
const CGUIString&	CGUIProperty::GetValue() const
{
	return m_strValue;
}
//------------------------------------------------------------------------------
void					CGUIProperty::SetType(const CGUIString& rType)
{
	m_strType = rType;
}
//------------------------------------------------------------------------------
const CGUIString&	CGUIProperty::GetType() const
{
	return m_strType;
}
//------------------------------------------------------------------------------
CGUIProperty*	CGUIProperty::Clone() const
{
	CGUIProperty* pProperty = CGUIPropertyManager::Instance()->CreateProperty();
	*pProperty = *this;
	return pProperty;
}
//------------------------------------------------------------------------------
void				CGUIProperty::AddProperty( CGUIProperty* pProperty )
{
	m_setProperty.AddProperty(pProperty);
}
//------------------------------------------------------------------------------
void					CGUIProperty::AddPropertySet(const CGUIPropertySet& rPropertySet)
{
	m_setProperty.AddPropertySet(rPropertySet);
}
//------------------------------------------------------------------------------
uint32			CGUIProperty::GetPropertyNum( ) const
{
	return m_setProperty.GetSize();
}
//------------------------------------------------------------------------------
const CGUIProperty*	CGUIProperty::GetProperty( uint32 nIdx ) const
{
	return m_setProperty.GetProperty(nIdx);
}
//------------------------------------------------------------------------------
const CGUIProperty*	CGUIProperty::GetProperty( const CGUIString& rName ) const
{
	return m_setProperty.GetProperty(rName);
}
//------------------------------------------------------------------------------
bool			CGUIProperty::HasProperty( const CGUIString& rName ) const
{
	return m_setProperty.HasProperty(rName);
}

}//namespace guiex
