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
#include "guiproperty.h"
#include "guipropertymanager.h"
#include "guiexception.h"

//------------------------------------------------------------------------------ 

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIProperty::CGUIProperty(const CGUIString& rName, const CGUIString& rType, const CGUIString& rValue )
		:m_pExtraData( NULL )
		,m_uType( ePropertyType_Unknown )
	{
		if( !rName.empty() )
		{
			SetName( rName );
		}
		if( !rType.empty() )
		{
			SetType( rType );
		}
		if( !rValue.empty() )
		{
			SetValue( rValue );
		}
	}
	//------------------------------------------------------------------------------
	CGUIProperty::~CGUIProperty()
	{
	}
	//------------------------------------------------------------------------------
	CGUIProperty::CGUIProperty(const CGUIProperty& other )
	{
		*this = other;
	}
	//------------------------------------------------------------------------------
	bool CGUIProperty::operator!=(const CGUIProperty& other ) const
	{
		return !(*this == other);
	}
	//------------------------------------------------------------------------------
	bool CGUIProperty::operator==(const CGUIProperty& other ) const
	{
		if( m_strName == other.m_strName &&		/// name of this property
			m_strType == other.m_strType &&		/// type of this property
			m_strValue == other.m_strValue &&		/// value of this property
			m_uType == other.m_uType &&
			m_setProperty == other.m_setProperty )	/// sub-property of this property
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	// assign operator
	void CGUIProperty::operator=(const CGUIProperty& other )
	{
		if( this != &other )
		{
			m_strName = other.m_strName;		/// name of this property
			m_strType = other.m_strType;		/// type of this property
			m_strValue = other.m_strValue;		/// value of this property
			m_uType = other.m_uType;
			m_pExtraData = other.m_pExtraData;
			m_setProperty = other.m_setProperty;	/// sub-property of this property
		}
	}
	//------------------------------------------------------------------------------
	/// clear all sub property
	void CGUIProperty::Clear()
	{
		m_setProperty.clear();
	}
	//------------------------------------------------------------------------------
	// set name of this property
	void CGUIProperty::SetName(const CGUIString& rName)
	{
		m_strName = rName;
	}
	//------------------------------------------------------------------------------
	// get name of this property
	const CGUIString& CGUIProperty::GetName() const
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------
	/// set value of this property
	void CGUIProperty::SetValue(const CGUIString& rValue)
	{
		m_strValue = rValue;
	}
	//------------------------------------------------------------------------------
	/// get value of this property
	const CGUIString& CGUIProperty::GetValue() const
	{
		return m_strValue;
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::SetType(const CGUIString& rType)
	{
		m_strType = rType;

		m_uType = CGUIPropertyManager::Instance()->StringToPropertyType( rType );
	}
	//------------------------------------------------------------------------------
	uint32 CGUIProperty::GetType() const
	{
		return m_uType;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIProperty::GetTypeAsString() const
	{
		return m_strType;
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::AddProperty( const CGUIProperty& pProperty )
	{
		m_setProperty.push_back(pProperty);
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::RemoveProperty( const CGUIProperty& rProperty )
	{
		for( TSetProperty::iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			if( (*itor) == rProperty )
			{
				m_setProperty.erase( itor );
				return;
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::RemoveProperty( int32 nIdx )
	{
		if( uint32(nIdx) >= m_setProperty.size())
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIPropertySet::RemoveProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
				m_setProperty.size(),
				nIdx));
		}

		m_setProperty.erase( m_setProperty.begin() + nIdx );
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::InsertProperty( const CGUIProperty& rProperty, int32 nIndex )
	{
		if( nIndex < 0 || guiex::uint32(nIndex) >= m_setProperty.size())
		{
			m_setProperty.push_back(rProperty);
		}
		else
		{
			m_setProperty.insert(m_setProperty.begin() + nIndex, rProperty);
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIProperty::GetPropertyNum( ) const
	{
		return static_cast<uint32>(m_setProperty.size());
	}
	//------------------------------------------------------------------------------
	int32 CGUIProperty::GetPropertyIndex( const CGUIString& rName, const CGUIString& rType ) const
	{
		for( uint32 i=0; i<m_setProperty.size(); ++i)
		{
			if( m_setProperty[i].GetName() == rName &&
				m_setProperty[i].GetTypeAsString() == rType )
			{
				return int32(i);
			}
		}
		GUI_THROW( GUI_FORMAT( 
			"[CGUIProperty::GetPropertyIndex]: failed to get property index by property name <%s:%s>",
			rName.c_str(),
			rType.c_str()));
		return -1;
	}
	//------------------------------------------------------------------------------
	int32 CGUIProperty::GetPropertyIndex( const CGUIString& rName ) const
	{
		for( uint32 i=0; i<m_setProperty.size(); ++i)
		{
			if( m_setProperty[i].GetName() == rName )
			{
				return int32(i);
			}
		}
		GUI_THROW( GUI_FORMAT( 
			"[CGUIProperty::GetPropertyIndex]: failed to get property index by property name <%s>",
			rName.c_str()));
		return -1;
	}
	//------------------------------------------------------------------------------
	CGUIProperty* CGUIProperty::GetProperty( uint32 nIdx )
	{
		if( nIdx >= m_setProperty.size())
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
				m_setProperty.size(),
				nIdx));
			return NULL;
		}

		return &(m_setProperty[nIdx]);
	}
	//------------------------------------------------------------------------------
	//get a sub-property by index
	const CGUIProperty*	CGUIProperty::GetProperty( uint32 nIdx ) const
	{
		if( nIdx >= m_setProperty.size())
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
				m_setProperty.size(),
				nIdx));
			return NULL;
		}

		return &(m_setProperty[nIdx]);
	}
	//------------------------------------------------------------------------------
	///get a sub-property by name
	CGUIProperty* CGUIProperty::GetProperty( const CGUIString& rName, uint32 nType )
	{
		for( TSetProperty::iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			CGUIProperty& rProperty = *itor;
			if( rProperty.GetName() == rName &&
				rProperty.GetType() == nType )
			{
				return &(*itor);
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIProperty*	CGUIProperty::GetProperty( const CGUIString& rName, const CGUIString& rType ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			const CGUIProperty& rProperty = *itor;

			if( rType.empty() )
			{
				if( rProperty.GetName() == rName )
				{
					return &(*itor);
				}
			}
			else
			{
				if( rProperty.GetName() == rName &&
					rProperty.GetTypeAsString() == rType )
				{
					return &(*itor);
				}
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIProperty* CGUIProperty::GetProperty( const CGUIString& rName, const CGUIString& rType )
	{
		for( TSetProperty::iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			CGUIProperty& rProperty = *itor;

			if( rType.empty() )
			{
				if( rProperty.GetName() == rName )
				{
					return &(*itor);
				}
			}
			else
			{
				if( rProperty.GetName() == rName &&
					rProperty.GetTypeAsString() == rType )
				{
					return &(*itor);
				}
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIProperty* CGUIProperty::GetPropertyChecked( const CGUIString& rName, const CGUIString& rType ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			const CGUIProperty& rProperty = *itor;

			if( rType.empty() )
			{
				if( rProperty.GetName() == rName )
				{
					return &(*itor);
				}
			}
			else
			{
				if( rProperty.GetName() == rName &&
					rProperty.GetTypeAsString() == rType )
				{
					return &(*itor);
				}
			}
		}

		GUI_THROW( GUI_FORMAT( 
			"[CGUIPropertySet::GetPropertyChecked]: failed to get property by name <%s> and type <%s>",
			rName.c_str(),
			rType.c_str()));
		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIProperty*	CGUIProperty::GetProperty( const CGUIString& rName, uint32 nType ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			const CGUIProperty& rProperty = *itor;

			if( rProperty.GetName() == rName &&
				rProperty.GetType() == nType )
			{
				return &(*itor);
			}
		}

		return NULL;
	}
	////------------------------------------------------------------------------------
	/////get a sub-property by name
	//CGUIProperty* CGUIProperty::GetProperty( const CGUIString& rName )
	//{
	//	for( TSetProperty::iterator itor = m_setProperty.begin();
	//		itor != m_setProperty.end();
	//		++itor)
	//	{
	//		if( (*itor).GetName() == rName )
	//		{
	//			return &(*itor);
	//		}
	//	}

	//	return NULL;
	//}
	////------------------------------------------------------------------------------
	//const CGUIProperty*	CGUIProperty::GetProperty( const CGUIString& rName ) const
	//{
	//	for( TSetProperty::const_iterator itor = m_setProperty.begin();
	//		itor != m_setProperty.end();
	//		++itor)
	//	{
	//		if( (*itor).GetName() == rName )
	//		{
	//			return &(*itor);
	//		}
	//	}

	//	return NULL;
	//}
	//------------------------------------------------------------------------------
	bool CGUIProperty::HasProperty( const CGUIString& rName, const CGUIString& rType ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			const CGUIProperty& rProperty = *itor;

			if( rProperty.GetName() == rName && rProperty.GetTypeAsString() == rType )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	bool CGUIProperty::HasProperty( const CGUIString& rName, uint32 nType ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			const CGUIProperty& rProperty = *itor;

			if( rProperty.GetName() == rName && rProperty.GetType() == nType )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	///whether this sub-property contain this property
	//bool CGUIProperty::HasProperty( const CGUIString& rName ) const
	//{
	//	for( TSetProperty::const_iterator itor = m_setProperty.begin();
	//		itor != m_setProperty.end();
	//		++itor)
	//	{
	//		if( (*itor).GetName() == rName )
	//		{
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	//------------------------------------------------------------------------------
	void CGUIProperty::SetData( void* pData )
	{
		m_pExtraData = pData;
	}
	//------------------------------------------------------------------------------
	void* CGUIProperty::GetData( ) const
	{
		return m_pExtraData;
	}
	//------------------------------------------------------------------------------
	bool CGUIProperty::HasDuplicatedNames( CGUIString* pName )
	{
		if( m_setProperty.size() < 2 )
		{
			return false;
		}

		for( uint32 i=0; i<m_setProperty.size()-1; ++i )
		{
			for( uint32 j=i+1; j<m_setProperty.size(); ++j )
			{
				if( m_setProperty[i].GetName() == m_setProperty[j].GetName() &&
					m_setProperty[i].GetType() == m_setProperty[j].GetType() )
				{
					if( pName )
					{
						*pName = m_setProperty[j].GetName();
					}
					return true;
				}
			}
			if( true == m_setProperty[i].HasDuplicatedNames( pName ))
			{
				return true;
			}
		}

		return false;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
