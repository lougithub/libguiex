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
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiexception.h>


//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------ 
//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIProperty::CGUIProperty(const CGUIString& rName, const CGUIString& rType, const CGUIString& rValue )
		:m_pExtraData( NULL )
		,m_uType(ePropertyType_Unknown)
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
	bool CGUIProperty::operator==(const CGUIProperty& other ) const
	{
		if( m_strName == other.m_strName &&		/// name of this property
			m_strType == other.m_strType &&		/// type of this property
			m_strValue == other.m_strValue &&		/// value of this property
			m_uType == other.m_uType &&
			m_pExtraData == other.m_pExtraData &&
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
	void CGUIProperty::Clear()
	{
		m_setProperty.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::SetName(const CGUIString& rName)
	{
		m_strName = rName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIProperty::GetName() const
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::SetValue(const CGUIString& rValue)
	{
		m_strValue = rValue;
	}
	//------------------------------------------------------------------------------
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
			throw CGUIException( 
				"[CGUIPropertySet::RemoveProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
				m_setProperty.size(),
				nIdx);
		}

		m_setProperty.erase( m_setProperty.begin() + nIdx );
	}
	//------------------------------------------------------------------------------
	void CGUIProperty::InsertProperty(  const CGUIProperty& rProperty, int32 nIndex )
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
	int32 CGUIProperty::GetPropertyIndex( const CGUIString& rName ) const
	{
		for( uint32 i=0; i<m_setProperty.size(); ++i)
		{
			if( m_setProperty[i].GetName() == rName )
			{
				return int32(i);
			}
		}
		throw CGUIException( 
			"[CGUIProperty::GetPropertyIndex]: failed to get property index by property name <%s>",
			rName.size());
		return -1;
	}
	//------------------------------------------------------------------------------
	const CGUIProperty*	CGUIProperty::GetProperty( uint32 nIdx ) const
	{
		if( nIdx >= m_setProperty.size())
		{
			throw CGUIException( 
				"[CGUIPropertySet::GetProperty]: error!the total size of sub-property is <%d>, you wanted is <%d>",
				m_setProperty.size(),
				nIdx);
			return NULL;
		}

		return &(m_setProperty[nIdx]);
	}
	//------------------------------------------------------------------------------
	CGUIProperty*	CGUIProperty::GetProperty( const CGUIString& rName )
	{
		for( TSetProperty::iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			if( (*itor).GetName() == rName )
			{
				return &(*itor);
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIProperty*	CGUIProperty::GetProperty( const CGUIString& rName ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			if( (*itor).GetName() == rName )
			{
				return &(*itor);
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	bool CGUIProperty::HasProperty( const CGUIString& rName ) const
	{
		for( TSetProperty::const_iterator itor = m_setProperty.begin();
			itor != m_setProperty.end();
			++itor)
		{
			if( (*itor).GetName() == rName )
			{
				return true;
			}
		}
		return false;
	}
}//namespace guiex
