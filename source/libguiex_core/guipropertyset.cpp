/** 
 * @file guipropertyset.cpp
 * @brief the property set, contain the config of the widget
 * @author ken
 * @date 2006-09-12
 */

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guilogmsgmanager.h>
#include <libguiex_core\guipropertyset.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guipropertymanager.h>
#include <libguiex_core\guiexception.h>
#include <algorithm>


//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------ 

namespace guiex
{
////============================================================================//
//// function
////============================================================================// 
////------------------------------------------------------------------------------
//CGUIPropertySet::CGUIPropertySet()
//:m_pExtraData(NULL)
//{
//}
////------------------------------------------------------------------------------
//CGUIPropertySet::~CGUIPropertySet()
//{
//	Clear();
//}
////------------------------------------------------------------------------------
//CGUIPropertySet::CGUIPropertySet(const CGUIPropertySet& rPropertySet )
//{
//	*this = rPropertySet;
//}
////------------------------------------------------------------------------------
//void CGUIPropertySet::operator=(const CGUIPropertySet& other )
//{
//	if( this != &other )
//	{
//		Clear();
//		
//		uint32 nSize = other.GetSize();
//		for( uint32 i=0; i<nSize; ++i)
//		{
//			CGUIProperty* pProperty = CGUIPropertyManager::Instance()->CreateProperty();
//			*pProperty = *other.GetProperty(i);
//			AddProperty(pProperty);
//		}
//
//		m_pExtraData = other.m_pExtraData;
//	}
//}
////------------------------------------------------------------------------------
//uint32	CGUIPropertySet::GetSize() const
//{
//	return static_cast<uint32>(m_setProperty.size());
//}
////------------------------------------------------------------------------------
//void			CGUIPropertySet::Clear()
//{
//	/// destroy all sub property
//	for( TSetProperty::iterator itor = m_setProperty.begin();
//		itor != m_setProperty.end();
//		itor++)
//	{
//		CGUIPropertyManager::Instance()->DestroyProperty(*itor);
//	}
//	m_setProperty.clear();
//}
////------------------------------------------------------------------------------
//CGUIProperty*	CGUIPropertySet::GetProperty(uint32 nIdx)
//{
//	if( nIdx >= m_setProperty.size())
//	{
//		throw CGUIException( 
//			"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
//			m_setProperty.size(),
//			nIdx);
//	}
//
//	return m_setProperty[nIdx];
//}
////------------------------------------------------------------------------------
//const CGUIProperty*	CGUIPropertySet::GetProperty(uint32 nIdx) const
//{
//	if( nIdx >= m_setProperty.size())
//	{
//		throw CGUIException( 
//			"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
//			m_setProperty.size(),
//			nIdx);
//	}
//
//	return m_setProperty[nIdx];
//}
////------------------------------------------------------------------------------
//int32		CGUIPropertySet::GetPropertyIndex( const CGUIString& rName ) const
//{
//	for( uint32 i=0; i<m_setProperty.size(); ++i)
//	{
//		if( m_setProperty[i].GetName() == rName )
//		{
//			return int32(i);
//		}
//	}
//	throw CGUIException( 
//		"[CGUIPropertySet::GetPropertyIndex]: failed to get property index by property name <%s>",
//		rName.size());
//	return -1;
//}
////------------------------------------------------------------------------------
//bool		CGUIPropertySet::HasProperty(const CGUIString& rName) const
//{
//	for( TSetProperty::const_iterator itor = m_setProperty.begin();
//		itor != m_setProperty.end();
//		++itor)
//	{
//		if( (*itor)->GetName() == rName )
//		{
//			return true;
//		}
//	}
//	return false;
//}
////------------------------------------------------------------------------------
//const CGUIProperty*	CGUIPropertySet::GetProperty(const CGUIString& rName) const
//{
//	for( TSetProperty::const_iterator itor = m_setProperty.begin();
//		itor != m_setProperty.end();
//		++itor)
//	{
//		if( (*itor)->GetName() == rName )
//		{
//			return *itor;
//		}
//	}
//
//	return NULL;
//}
////------------------------------------------------------------------------------
//CGUIProperty*	CGUIPropertySet::GetProperty(const CGUIString& rName)
//{
//	for( TSetProperty::const_iterator itor = m_setProperty.begin();
//		itor != m_setProperty.end();
//		++itor)
//	{
//		if( (*itor)->GetName() == rName )
//		{
//			return *itor;
//		}
//	}
//
//	return NULL;
//}
////------------------------------------------------------------------------------
//void		CGUIPropertySet::AddProperty(CGUIProperty* pProperty)
//{
//	GUI_ASSERT(pProperty,"wrong parameter");
//	m_setProperty.push_back(pProperty);
//}
////------------------------------------------------------------------------------
//void		CGUIPropertySet::AddProperty(const CGUIString& rName, const CGUIString& rType, const CGUIString& rValue)
//{
//	CGUIProperty *pProperty = CGUIPropertyManager::Instance()->CreateProperty(rName, rType, rValue);
//	m_setProperty.push_back(pProperty);
//}
////------------------------------------------------------------------------------
//void		CGUIPropertySet::AddPropertySet(const CGUIPropertySet& rPropertySet)
//{
//	uint32 nSize = rPropertySet.GetSize();
//	for( uint32 nIdx = 0; nIdx < nSize; ++nIdx)
//	{
//		m_setProperty.push_back(rPropertySet.GetProperty(nIdx)->Clone());
//	}
//}
////------------------------------------------------------------------------------
//void		CGUIPropertySet::RemoveProperty(const CGUIProperty* pProperty)
//{
//	TSetProperty::iterator itor = std::find(m_setProperty.begin(), m_setProperty.end(),pProperty);
//	if( itor == m_setProperty.end() )
//	{
//		throw CGUIException( 
//			"[CGUIPropertySet::RemoveProperty]: failed to remove property whose name is <%s>",
//			pProperty->GetName().c_str());
//	}
//	CGUIPropertyManager::Instance()->DestroyProperty(*itor);
//	m_setProperty.erase(itor);
//}
////------------------------------------------------------------------------------

}//namespace guiex
