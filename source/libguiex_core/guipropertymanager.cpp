/** 
 * @file guipropertymanager.cpp
 * @brief used to manage property
 * @author ken
 * @date 2006-06-21
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guistring.h>
#include <libguiex_core\guiexception.h>


//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------ 

namespace guiex
{
//============================================================================//
// function
//============================================================================// 
GUI_SINGLETON_IMPLEMENT_EX(CGUIPropertyManager);
//------------------------------------------------------------------------------
CGUIPropertyManager::CGUIPropertyManager()
{
	RegisterPropertyType( ePropertyType_Size, "SIZE" );
}
//------------------------------------------------------------------------------
CGUIPropertyManager::~CGUIPropertyManager()
{

}
//------------------------------------------------------------------------------
void CGUIPropertyManager::RegisterPropertyType( uint32 uPropertyType, const CGUIString& rStringValue )
{
	TMapPropertyType2String::iterator itorFindType2String = m_mapPropertyType2String.find( uPropertyType );
	if( itorFindType2String  != m_mapPropertyType2String.end() )
	{
		throw CGUIException( "[CGUIPropertyManager::RegisterPropertyType]: the property <%s:%d> has existed!",
			rStringValue.c_str(), 
			uPropertyType);
	}
	TMapString2PropertyType::iterator itorFindString2Type = m_mapString2PropertyType.find( rStringValue );
	if( itorFindString2Type  != m_mapString2PropertyType.end() )
	{
		throw CGUIException( "[CGUIPropertyManager::RegisterPropertyType]: the property <%s:%d> has existed!",
			rStringValue.c_str(), 
			uPropertyType);
	}
	m_mapPropertyType2String.insert( std::make_pair( uPropertyType, rStringValue ));
	m_mapString2PropertyType.insert( std::make_pair( rStringValue, uPropertyType ));
}
//------------------------------------------------------------------------------
uint32 CGUIPropertyManager::StringToPropertyType( const CGUIString& rStringValue )
{
	TMapString2PropertyType::iterator itorFindString2Type = m_mapString2PropertyType.find( rStringValue );
	if( itorFindString2Type  != m_mapString2PropertyType.end() )
	{
		throw CGUIException( "[CGUIPropertyManager::StringToPropertyType]: failed to find property type by string value <%s>",
			rStringValue.c_str());
	}
	return itorFindString2Type->second;
}
//------------------------------------------------------------------------------
const CGUIString& CGUIPropertyManager::PropertyTypeToString( uint32 uPropertyType )
{
	TMapPropertyType2String::iterator itorFindType2String = m_mapPropertyType2String.find( uPropertyType );
	if( itorFindType2String  != m_mapPropertyType2String.end() )
	{
		throw CGUIException( "[CGUIPropertyManager::PropertyTypeToString]: failed to find string value by property type <%d>",
			uPropertyType);
	}
	return itorFindType2String->second;
}
//------------------------------------------------------------------------------
CGUIProperty*	CGUIPropertyManager::CreateProperty()
{
	return new CGUIProperty;
}
//------------------------------------------------------------------------------
CGUIProperty*	CGUIPropertyManager::CreateProperty(	
	const CGUIString& rName, 
	const CGUIString& rType)
{
	CGUIProperty* pProperty = new CGUIProperty;
	pProperty->SetName(rName);
	pProperty->SetType(rType);
	return pProperty;
}
//------------------------------------------------------------------------------
CGUIProperty*	CGUIPropertyManager::CreateProperty(
	const CGUIString& rName, 
	const CGUIString& rType,
	const CGUIString& rValue)
{
	CGUIProperty* pProperty = new CGUIProperty;
	pProperty->SetName(rName);
	pProperty->SetType(rType);
	pProperty->SetValue(rValue);
	return pProperty;
}
//------------------------------------------------------------------------------
void						CGUIPropertyManager::DestroyProperty(CGUIProperty* pProperty)
{
	delete pProperty;
}
//------------------------------------------------------------------------------
void						CGUIPropertyManager::RegisterSet( const CGUIString& rSetName, const CGUIPropertySet& rPropertySet )
{
	if( m_mapPropertySet.find(rSetName) != m_mapPropertySet.end())
	{
		throw CGUIException( "[CGUIPropertyManager::RegisterSet]: the property <%s> has existed!",rSetName.c_str());
	}
	m_mapPropertySet.insert( std::make_pair(rSetName,rPropertySet) );
}
//------------------------------------------------------------------------------
void						CGUIPropertyManager::UnregisterSet( const CGUIString& rSetName )
{
	TMapPropertySet::iterator itor = m_mapPropertySet.find(rSetName);
	if( itor == m_mapPropertySet.end())
	{
		throw CGUIException( "[CGUIPropertyManager::UnregisterSet]: the property <%s> doesn't existed!",rSetName.c_str());
	}
	m_mapPropertySet.erase( itor );
}
//------------------------------------------------------------------------------
const  CGUIPropertySet&	CGUIPropertyManager::GetSet(const CGUIString& rSetName ) const
{
	TMapPropertySet::const_iterator itor = m_mapPropertySet.find(rSetName);
	if( itor == m_mapPropertySet.end())
	{
		throw CGUIException( "[CGUIPropertyManager::GetSet]: the property <%s> doesn't existed!",rSetName.c_str());
	}
	return itor->second;
}
//------------------------------------------------------------------------------
}//namespace guiex
