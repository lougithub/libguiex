// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101116.
// -----------------------------------------------------------------------------
#ifndef _RE_POOL_H_
#define _RE_POOL_H_
// -----------------------------------------------------------------------------
// This class can serve as a normal container, but it won't assume any ownership
// of the items it holds. Any memory related operations should be conducted
// by user classes by calling either Clear ( memory not release ) or Destroy
// ( memory released ).
// It also guarantees that no duplicated data should exist ( pretty much what
// std::set does ).
// -----------------------------------------------------------------------------

#include <list>


namespace RE
{


template< class T >
class RePool
{
public:
	typedef std::list< T* >						TItemList;
	typedef typename TItemList::iterator		TItemListItor;
	typedef typename TItemList::const_iterator	TItemListCItor;

public:
	T*				Pop();
	T*				Back();
	void			Push( T* _item );
	void			Erase( T* _item );
	void			Erase( TItemListItor _itor );
	void			Destroy();
	void			Clear();

	TItemListItor	Begin()			{ return m_itemList.begin(); }
	TItemListItor	End()			{ return m_itemList.end(); }
	TItemListCItor	Begin() const	{ return m_itemList.begin(); }
	TItemListCItor	End() const		{ return m_itemList.end(); }	
	int				Size() const	{ return ( int )m_itemList.size(); }

protected:
	TItemList		m_itemList;
};


template< class T >
T* RePool< T >::Pop()
{
	T* result = NULL;

	if( ( int )m_itemList.size() > 0 )
	{
		result = m_itemList.back();
		m_itemList.pop_back();
	}

	return result;
}


template< class T >
T* RePool< T >::Back()
{
	T* result = NULL;

	if( ( int )m_itemList.size() > 0 )
	{
		result = m_itemList.back();
	}

	return result;
}


template< class T >
void RePool< T >::Push( T* _item )
{
	if( NULL != _item )
	{
		bool isFound = false;
		if( ( int )m_itemList.size() > 0 )
		{
			TItemListItor itor = m_itemList.begin();
			TItemListItor itorEnd = m_itemList.end();
			for( ; itor != itorEnd; ++itor )
			{
				if( _item == *itor )
				{
					isFound = true;
					break;
				}
			}
		}

		if( !isFound )
			m_itemList.push_back( _item );
	}
}


template< class T >
void RePool< T >::Erase( T* _item )
{
	if( NULL != _item )
	{
		if( ( int )m_itemList.size() > 0 )
		{
			TItemListItor itor = m_itemList.begin();
			TItemListItor itorEnd = m_itemList.end();
			for( ; itor != itorEnd; ++itor )
			{
				if( _item == *itor )
				{
					m_itemList.erase( itor );
					break;
				}
			}
		}
	}
}


template< class T >
void RePool< T >::Erase( TItemListItor _itor )
{
	if( m_itemList.end() != _itor )
		m_itemList.erase( _itor );
}


template< class T >
void RePool< T >::Destroy()
{
	if( ( int )m_itemList.size() > 0 )
	{
		TItemListItor itor = m_itemList.begin();
		TItemListItor itorEnd = m_itemList.end();
		for( ; itor != itorEnd; ++itor )
			if( NULL != *itor )
				delete *itor;
		m_itemList.clear();
	}
}


template< class T >
void RePool< T >::Clear()
{
	m_itemList.clear();
}


}
#endif	// _RE_POOL_H_
