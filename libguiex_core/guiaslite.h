/** 
* @file guiaslite.h
* @brief action sequence lite for non-widgets, such as camera.
* @author ken
* @date 2010-12-23
*/

#ifndef __GUI_ASLITE_20101223_H__
#define __GUI_ASLITE_20101223_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guivector2.h"
#include "guivector3.h"
#include "guicolor.h"

#include "guisize.h"
#include "guimath.h"
#include "guiinterpolation.h"

#include <list>
#include <vector>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIAsLite
	//*****************************************************************************

	class GUIEXPORT CGUIAsLite
	{
	public:
		CGUIAsLite();
		virtual ~CGUIAsLite();

		void Retire( bool bRetired );
		bool IsRetired();

		void SetLooping( bool bLooping );
		bool IsLooping();

		void SetTotalTime( real fTotalTime );
		real GetTotalTime( ) const;

		void SetElapsedTime( real fElapsedTime );
		real GetElapsedTime( ) const;

		virtual real Update( real fDeltaTime );

	private:
		real m_fTotalTime; //!<delay-time, to control when to process this as, millisecond
		real m_fElapsedTime;			
		bool m_bLooping;
		bool m_bRetired; //!<should this as be retired
	};


	//*****************************************************************************
	//	CGUIAsLite_Interpolation
	//*****************************************************************************
	template< class T >
	class CGUIAsLite_Interpolation : public CGUIAsLite
	{
	public:
		CGUIAsLite_Interpolation( EInterpolationType eInterpolationType = eInterpolationType_Linear )
			:CGUIAsLite( )
			,m_eInterpolationType( eInterpolationType )
		{
		}

		CGUIAsLite_Interpolation( const T& rBeginValue, const T& rEndValue, real fTotalTime, EInterpolationType eInterpolationType = eInterpolationType_Linear )
			:CGUIAsLite( )
			,m_eInterpolationType( eInterpolationType )
		{
			SetInterpolationValue( rBeginValue, rEndValue, fTotalTime );
		}

		EInterpolationType GetInterpolationType( ) const
		{
			return m_eInterpolationType;
		}
		void SetInterpolationType( EInterpolationType eType )
		{
			m_eInterpolationType = eType;
		}

		void SetInterpolationValue( const T& rBeginValue, const T& rEndValue, real fTotalTime )
		{
			m_aBeginValue = rBeginValue;
			m_aEndValue = rEndValue;
			m_aCurValue = m_aBeginValue;
			SetTotalTime( fTotalTime );
		}

		virtual real Update( real fDeltaTime )
		{
			real fLeftTime = CGUIAsLite::Update( fDeltaTime );
			m_aCurValue = LinearTween( GetElapsedTime() / GetTotalTime(), m_aBeginValue, m_aEndValue );
			return fLeftTime;
		}

		void SetBeginValue( const T& rValue )
		{
			m_aBeginValue = rValue;
		}

		const T& GetBeginValue() const
		{
			return m_aBeginValue;
		}

		void SetEndValue( const T& rValue )
		{
			m_aEndValue = rValue;
		}

		const T& GetEndValue() const
		{
			return m_aEndValue;
		}

		void SetCurrentValue( const T& rValue )
		{
			m_aCurValue = rValue;
		}

		const T& GetCurrentValue() const
		{
			return m_aCurValue;
		}

	private:
		EInterpolationType	m_eInterpolationType;

		T m_aBeginValue;
		T m_aEndValue;

		T m_aCurValue;
	};


	//*****************************************************************************
	//	CGUIAsLite_Container
	//*****************************************************************************
	class GUIEXPORT CGUIAsLite_Container : public CGUIAsLite
	{
	public:
		CGUIAsLite_Container( );
		virtual ~CGUIAsLite_Container( );

		virtual real Update( real fDeltaTime );
		void AddItem( const CGUIAsLite& rAsLite, real fBeginTime );
		
	private:
		class CGUIAsLite_ContainItemInfo
		{
		public:
			CGUIAsLite m_rAsLite;
			real m_fBeginTime;

			CGUIAsLite_ContainItemInfo()
				:m_fBeginTime( 0.0f )
			{
			}
		};

		typedef std::vector<CGUIAsLite_ContainItemInfo*> TAsLiteList;
		TAsLiteList m_vAsLiteList; //the as in this list should be sorted by begin time
	};

	//*****************************************************************************
	//	CGUIAsLite_Queue
	//*****************************************************************************
	template< class T >
	class CGUIAsLite_Queue : public CGUIAsLite
	{
	public:
		CGUIAsLite_Queue( );
		virtual ~CGUIAsLite_Queue( );

		virtual real Update( real fDeltaTime );
		void AddItem( const CGUIAsLite_Interpolation<T>& rAsLite );
		const T& GetCurrentValue() const;

	private:
		class CGUIAsLite_QueueItemInfo
		{
		public:
			CGUIAsLite_Interpolation<T> m_rAsLite;
			real m_fBeginTime;

			CGUIAsLite_QueueItemInfo()
				:m_fBeginTime( 0.0f )
			{
			}
		};
		typedef std::vector<CGUIAsLite_QueueItemInfo*> TAsLiteQueue;
		TAsLiteQueue m_vAsLiteQueue;

		T m_aCurValue;
	};

}//namespace guiex

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsLite_Queue<T>::CGUIAsLite_Queue( )
	{
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline CGUIAsLite_Queue<T>::~CGUIAsLite_Queue( )
	{
		for( typename TAsLiteQueue::iterator itor = m_vAsLiteQueue.begin();
			itor != m_vAsLiteQueue.end();
			++itor )
		{
			delete *itor;
		}
		m_vAsLiteQueue.clear();
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline real CGUIAsLite_Queue<T>::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsLite::Update( fDeltaTime );

		for( typename TAsLiteQueue::iterator itor = m_vAsLiteQueue.begin();
			itor != m_vAsLiteQueue.end();
			++itor )
		{
			CGUIAsLite_QueueItemInfo* pInfo = *itor;
			if( pInfo->m_fBeginTime > GetElapsedTime() )
			{
				//update done
				break;
			}

			if( pInfo->m_fBeginTime + pInfo->m_rAsLite.GetTotalTime() < GetElapsedTime() )
			{
				continue;
			}

			pInfo->m_rAsLite.SetElapsedTime( GetElapsedTime() - pInfo->m_fBeginTime );
			pInfo->m_rAsLite.Update( 0.0f );
			m_aCurValue = pInfo->m_rAsLite.GetCurrentValue();
		}

		return fLeftTime;
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline void CGUIAsLite_Queue<T>::AddItem( const CGUIAsLite_Interpolation<T>& rAsLite )
	{
		CGUIAsLite_QueueItemInfo* pNewInfo = new  CGUIAsLite_QueueItemInfo;
		pNewInfo->m_rAsLite = rAsLite;
		if( m_vAsLiteQueue.empty() )
		{
			pNewInfo->m_fBeginTime = 0.0f;
			SetTotalTime( rAsLite.GetTotalTime() );
		}
		else
		{
			pNewInfo->m_fBeginTime = GetTotalTime();
			SetTotalTime( GetTotalTime() + rAsLite.GetTotalTime() );
		}

		//insert
		m_vAsLiteQueue.push_back( pNewInfo );
	}
	//------------------------------------------------------------------------------
	template< class T >
	inline const T& CGUIAsLite_Queue<T>::GetCurrentValue() const
	{
		return m_aCurValue;
	}
	//------------------------------------------------------------------------------
}//namespace guiex


#endif //__GUI_ASLITE_20101223_H__


