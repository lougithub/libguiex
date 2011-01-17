// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_ITEMGROUP_H_
#define _RE_EDITOR_ITEMGROUP_H_


#include <list>


namespace RE
{


template< class T >
class ReEqualPredicate
{
public:
	bool IsEqual( const T& _a, const T& _b )
	{
		return _a == _b;
	}
};


template< class T, class E = ReEqualPredicate< T > >
class ReItemGroup
{
public:
	typedef std::list< T >						TItemList;
	typedef typename TItemList::iterator		TItemListItor;
	typedef typename TItemList::const_iterator	TItemListCItor;

	typedef bool ( *SortPredicate )( const T& _a, const T& _b );

	void				Add( T& _item );
	void				Remove( T& _item );
	bool				Find( T& _item ) const;
	int					Size() const;
	int					Index( T& _item ) const;
	T&					GetAt( int _index );
	const T&			GetAt( int _index ) const;
	void				Sort( SortPredicate _predicate );
	virtual void		Clear();
	void				Destroy();

	TItemListItor		Begin()			{ return m_items.begin(); }
	TItemListItor		End()			{ return m_items.end(); }
	TItemListCItor		Begin() const	{ return m_items.begin(); }
	TItemListCItor		End() const		{ return m_items.end(); }

protected:
	TItemList			m_items;
};


template< class T, class E >
void ReItemGroup< T, E >::Add( T& _item )
{
	if( !Find( _item ) )
	{
		m_items.push_back( _item );
	}
}


template< class T, class E >
void ReItemGroup< T, E >::Remove( T& _item )
{
	E equalPredicate;
	TItemListItor itor = m_items.begin();
	TItemListItor itorEnd = m_items.end();
	for( ; itor != itorEnd; ++itor )
	{
		if( equalPredicate.IsEqual( *itor, _item ) )
		{
			m_items.erase( itor );
			break;
		}
	}
}


template< class T, class E >
bool ReItemGroup< T, E >::Find( T& _item ) const
{
	E equalPredicate;
	TItemListCItor itor = m_items.begin();
	TItemListCItor itorEnd = m_items.end();
	for( ; itor != itorEnd; ++itor )
	{
		if( equalPredicate.IsEqual( *itor, _item ) )
		{
			break;
		}
	}

	return itor != itorEnd;
}


template< class T, class E >
int ReItemGroup< T, E >::Size() const
{
	return ( int )m_items.size();
}


template< class T, class E >
int ReItemGroup< T, E >::Index( T& _item ) const
{
	int result = -1;

	E equalPredicate;
	TItemListCItor itor = m_items.begin();
	TItemListCItor itorEnd = m_items.end();
	for( int i = 0; itor != itorEnd; ++itor, ++i )
	{
		if( equalPredicate.IsEqual( *itor, _item ) )
		{
			result = i;
			break;
		}
	}

	return result;
}


template< class T, class E >
T& ReItemGroup< T, E >::GetAt( int _index )
{
	TItemListItor itor = m_items.begin();
	for( int i = 0; i < _index; ++i )
		++itor;
	return *itor;
}


template< class T, class E >
const T& ReItemGroup< T, E >::GetAt( int _index ) const
{
	TItemListCItor itor = m_items.begin();
	for( int i = 0; i < _index; ++i )
		++itor;
	return *itor;
}


template< class T, class E >
void ReItemGroup< T, E >::Sort( typename ReItemGroup< T, E >::SortPredicate _predicate )
{
	m_items.sort( _predicate );
}


template< class T, class E >
void ReItemGroup< T, E >::Clear()
{
	m_items.clear();
}


// Partial specialization to avoid deletion on non-pointer types.
template< class T, class E >
void ReItemGroup< T, E >::Destroy()
{
	TItemListItor itor = m_items.begin();
	TItemListItor itorEnd = m_items.end();
	for( ; itor != itorEnd; ++itor )
	{
		delete *itor;
	}

	m_items.clear();
}


}
#endif	// _RE_EDITOR_ITEMGROUP_H_
