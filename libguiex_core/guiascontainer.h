/** 
* @file guiascontainer.h
* @brief widget action sequence
* @author ken
* @date 2011-04-22
*/

#ifndef __GUI_ASCONTAINER_20110422_H__
#define __GUI_ASCONTAINER_20110422_H__

//============================================================================//
// include
//============================================================================// 
#include "guias.h"
#include <vector>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIAsInterpolationQueue
	//*****************************************************************************
	template< class T >
	class CGUIAsInterpolationQueue : public CGUIAs
	{
	protected:
		CGUIAsInterpolationQueue(const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsInterpolationQueue( );

	public:
		virtual void Reset( );

		void AddItem( CGUIAsInterpolation<T>* pAs );
		const T& GetCurrentValue() const;

	protected:
		virtual void OnUpdate();

	private:
		class CGUIQueueItemInfo
		{
		public:
			CGUIAsInterpolation<T>* m_pAs;
			real m_fBeginTime;
			CGUIQueueItemInfo()
				:m_fBeginTime( 0.0f )
				,m_pAs(NULL)
			{
			}
		};
		typedef std::vector<CGUIQueueItemInfo> TAsQueue;
		TAsQueue m_vAsQueue;

		T m_aCurValue;

		GUI_AS_GENERATOR_DECLARE( CGUIAsInterpolationQueue );
	};


	//*****************************************************************************
	//	CGUIAsContainer
	//*****************************************************************************
	class CGUIAsContainItemInfo
	{
	public:
		CGUIAs* m_pAs;
		real m_fBeginTime;

		CGUIAsContainItemInfo()
			:m_pAs( NULL )
			,m_fBeginTime( 0.0f )
		{
		}
	};

	//*****************************************************************************
	//	CGUIAsContainer
	//*****************************************************************************
	class GUIEXPORT CGUIAsContainer : public CGUIAs
	{
	protected:
		CGUIAsContainer(const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsContainer( );

	public:
		virtual void Reset( );

		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		virtual void SetReceiver(CGUIWidget* pReceiver);

		void AddItem( CGUIAsContainItemInfo& rItemInfo );
		void AddItem( CGUIAs* pAs, real fBeginTime );

	protected:
		virtual void OnUpdate();

	private:
		typedef std::vector<CGUIAsContainItemInfo> TAsList;
		TAsList m_vAsList;//the as in this list should be sorted by begin time

		GUI_AS_GENERATOR_DECLARE( CGUIAsContainer);
	};
}//namespace guiex




//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CGUIAsInterpolationQueue
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsInterpolationQueue<T>::CGUIAsInterpolationQueue( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAs("CGUIAsInterpolationQueue", rAsName, rSceneName)
	{
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsInterpolationQueue<T>::~CGUIAsInterpolationQueue( )
	{
		for( typename TAsQueue::iterator itor = m_vAsQueue.begin();
			itor != m_vAsQueue.end();
			++itor )
		{
			(*itor).m_pAs->RefRelease();
		}
		m_vAsQueue.clear();
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolationQueue<T>::Reset( )
	{
		CGUIAs::Reset();
		for( typename TAsQueue::iterator itor = m_vAsQueue.begin();
			itor != m_vAsQueue.end();
			++itor )
		{
			(*itor).m_pAs->Reset();
		}
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolationQueue<T>::OnUpdate( )
	{
		CGUIAs::OnUpdate( );

		for( typename TAsQueue::iterator itor = m_vAsQueue.begin();
			itor != m_vAsQueue.end();
			++itor )
		{
			CGUIQueueItemInfo &rInfo = *itor;
			if( rInfo.m_fBeginTime > GetElapsedTime() )
			{
				//update done
				break;
			}

			if( rInfo.m_fBeginTime + rInfo.m_pAs->GetTotalTime() < GetElapsedTime() )
			{
				continue;
			}

			rInfo.m_pAs->SetElapsedTime( GetElapsedTime() - rInfo.m_fBeginTime );
			rInfo.m_pAs->Update( 0.0f );
			m_aCurValue = rInfo.m_pAs->GetCurrentValue();
		}
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsInterpolationQueue<T>::AddItem( CGUIAsInterpolation<T>* pAs )
	{
		CGUIQueueItemInfo aNewInfo;
		aNewInfo.m_pAs = pAs;

		//retain
		aNewInfo.m_pAs->RefRetain();

		if( m_vAsQueue.empty() )
		{
			aNewInfo.m_fBeginTime = 0.0f;
			SetTotalTime( pAs->GetTotalTime() );
		}
		else
		{
			aNewInfo.m_fBeginTime = GetTotalTime();
			SetTotalTime( GetTotalTime() + pAs->GetTotalTime() );
		}

		//insert
		m_vAsQueue.push_back( aNewInfo );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline const T& CGUIAsInterpolationQueue<T>::GetCurrentValue() const
	{
		return m_aCurValue;
	}
	//------------------------------------------------------------------------------

}

#endif //__GUI_ASCONTAINER_20110422_H__


