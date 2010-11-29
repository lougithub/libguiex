// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101120.
// -----------------------------------------------------------------------------
#ifndef _RE_MODEL_BASE_H_
#define _RE_MODEL_BASE_H_
// -----------------------------------------------------------------------------
// A model has two pools: a data pool that holds data that user expects it to
// posses, and a recycle pool where unused data is stored and could be retrieved
// later when new data requests are issued.
// User call CreateData to create a new data item inside this model, and use the
// returned pointer for further operations ( except trying to release it ).
// When a model is being released from memory, it makes sure that all data items
// in both pools are properly released as well.
// A model also provides an interface for garbage collection in case we shall 
// practically run short of memory.
// -----------------------------------------------------------------------------
#include "Core\RePool.h"


namespace RE
{


template< class T >
class ReModelBase
{
	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	typedef T								TData;
	typedef typename RePool< TData >		TPool;
	typedef typename TPool::TItemListItor	TPoolItor;
	typedef typename TPool::TItemListCItor	TPoolCItor;

	virtual ~ReModelBase();

	// ----------------------------------------------------------------------------
	// Interfaces.
	// ----------------------------------------------------------------------------
public:
	virtual T*			CreateData();
	virtual void		RecycleData( T* _data );
	virtual bool		FindData( T* _data ) const;
	virtual int			GetDataCount() const;
	
	virtual void		ClearData();			// Recycle all data.
	virtual void		DestroyData();			// Recycle all data then release the recycle pool.
	virtual void		CollectGarbage();		// Release the recycle pool.

	// ----------------------------------------------------------------------------
	// Variables.
	// ----------------------------------------------------------------------------
protected:
	RePool< T >			m_dataPool;
	RePool< T >			m_recyclePool;
};


template< class T >
ReModelBase< T >::~ReModelBase()
{
	DestroyData();
}


template< class T >
T* ReModelBase< T >::CreateData()
{
	T* result = m_recyclePool.Pop();
	if( NULL == result )
		result = new T();

	if( NULL != result )
		m_dataPool.Push( result );

	return result;	
}


template< class T >
void ReModelBase< T >::RecycleData( T* _data )
{
	// Only recycle data that belongs to this model.
	if( FindData( _data ) )
	{
		m_dataPool.Erase( _data );
		m_recyclePool.Push( _data );
	}
}


template< class T >
bool ReModelBase< T >::FindData( T* _data ) const
{
	bool result = false;

	if( NULL != _data )
	{
		TPoolCItor itor = m_dataPool.Begin();
		TPoolCItor itorEnd = m_dataPool.End();
		for( ; itor != itorEnd; ++itor )
		{
			if( _data == *itor )
			{
				result = true;
				break;
			}
		}
	}

	return result;
}


template< class T >
int ReModelBase< T >::GetDataCount() const
{
	return m_dataPool.Size();
}


template< class T >
void ReModelBase< T >::ClearData()
{
	//T* data = NULL;
	//while( NULL != ( data = m_dataPool.Pop() ) )
	//{
	//	// Here we cannot call RecycleData anymore, since the data
	//	// has been popped off the data pool.
	//	m_recyclePool.Push( data );
	//}
	
	T* data = NULL;
	while( NULL != ( data = m_dataPool.Back() ) )
	{
		RecycleData( data );
		//m_dataPool.Pop();
	}
}


template< class T >
void ReModelBase< T >::DestroyData()
{
	// Do not directly call Destroy on both pools because
	// we might need to log the whole exact process.
	ClearData();
	CollectGarbage();
}


template< class T >
void ReModelBase< T >::CollectGarbage()
{
	m_recyclePool.Destroy();
}


}
#endif	// _RE_MODEL_BASE_H_
