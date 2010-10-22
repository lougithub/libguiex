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
//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
CGUIPropertySet::CGUIPropertySet()
{
}
//------------------------------------------------------------------------------
CGUIPropertySet::~CGUIPropertySet()
{
	Clear();
}
//------------------------------------------------------------------------------
CGUIPropertySet::CGUIPropertySet(const CGUIPropertySet& rPropertySet )
{
	*this = rPropertySet;
}
//------------------------------------------------------------------------------
void CGUIPropertySet::operator=(const CGUIPropertySet& rPropertySet )
{
	if( this != &rPropertySet )
	{
		Clear();
		
		uint32 nSize = rPropertySet.GetSize();
		for( uint32 i=0; i<nSize; ++i)
		{
			CGUIProperty* pProperty = CGUIPropertyManager::Instance()->CreateProperty();
			*pProperty = *rPropertySet.GetProperty(i);
			AddProperty(pProperty);
		}
	}
}
//------------------------------------------------------------------------------
uint32	CGUIPropertySet::GetSize() const
{
	return static_cast<uint32>(m_setProperty.size());
}
//------------------------------------------------------------------------------
void			CGUIPropertySet::Clear()
{
	/// destroy all sub property
	for( TSetProperty::iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		itor++)
	{
		CGUIPropertyManager::Instance()->DestroyProperty(*itor);
	}
	m_setProperty.clear();
}
//------------------------------------------------------------------------------
CGUIProperty*	CGUIPropertySet::GetProperty(uint32 nIdx)
{
	if( nIdx >= m_setProperty.size())
	{
		throw CGUIException( 
			"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
			m_setProperty.size(),
			nIdx);
	}

	return m_setProperty[nIdx];
}
//------------------------------------------------------------------------------
const CGUIProperty*	CGUIPropertySet::GetProperty(uint32 nIdx) const
{
	if( nIdx >= m_setProperty.size())
	{
		throw CGUIException( 
			"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
			m_setProperty.size(),
			nIdx);
	}

	return m_setProperty[nIdx];
}
//------------------------------------------------------------------------------
bool		CGUIPropertySet::HasProperty(const CGUIString& rName) const
{
	for( TSetProperty::const_iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		++itor)
	{
		if( (*itor)->GetName() == rName )
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
const CGUIProperty*	CGUIPropertySet::GetProperty(const CGUIString& rName) const
{
	for( TSetProperty::const_iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		++itor)
	{
		if( (*itor)->GetName() == rName )
		{
			return *itor;
		}
	}

	throw CGUIException( 
		"[CGUIPropertySet::GetProperty]: failed to find property whose name is <%s>",
		rName.c_str());
}
//------------------------------------------------------------------------------
CGUIProperty*	CGUIPropertySet::GetProperty(const CGUIString& rName)
{
	for( TSetProperty::const_iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		++itor)
	{
		if( (*itor)->GetName() == rName )
		{
			return *itor;
		}
	}

	return NULL;
}
//------------------------------------------------------------------------------
void		CGUIPropertySet::AddProperty(CGUIProperty* pProperty)
{
	GUI_ASSERT(pProperty,"wrong parameter");


	//for( TSetProperty::iterator itor = m_setProperty.begin();
	//	itor != m_setProperty.end();
	//	++itor)
	//{
	//	if( (*itor)->GetName() == pProperty->GetName())
	//	{
	//		GUI_TRACE( GUI_FORMAT("[CGUIPropertySet::AddProperty]: the new property replace the old property!<%s %s>", 
	//			pProperty->GetName().c_str(), pProperty->GetType().c_str()));

	//		CGUIPropertyManager::Instance()->DestroyProperty(*itor);
	//		(*itor) = pProperty;
	//		return;
	//	}
	//}
	m_setProperty.push_back(pProperty);
}
//------------------------------------------------------------------------------
void		CGUIPropertySet::AddPropertySet(const CGUIPropertySet& rPropertySet)
{
	uint32 nSize = rPropertySet.GetSize();
	for( uint32 nIdx = 0; nIdx < nSize; ++nIdx)
	{
		m_setProperty.push_back(rPropertySet.GetProperty(nIdx)->Clone());
	}
}
//------------------------------------------------------------------------------
void		CGUIPropertySet::RemoveProperty(const CGUIProperty* pProperty)
{
	TSetProperty::iterator itor = std::find(m_setProperty.begin(), m_setProperty.end(),pProperty);
	if( itor == m_setProperty.end() )
	{
		throw CGUIException( 
			"[CGUIPropertySet::RemoveProperty]: failed to remove property whose name is <%s>",
			pProperty->GetName().c_str());
	}
	CGUIPropertyManager::Instance()->DestroyProperty(*itor);
	m_setProperty.erase(itor);
}
//------------------------------------------------------------------------------

}//namespace guiex
