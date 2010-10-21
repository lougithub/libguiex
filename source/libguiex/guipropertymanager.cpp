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
}
//------------------------------------------------------------------------------
CGUIPropertyManager::~CGUIPropertyManager()
{

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
