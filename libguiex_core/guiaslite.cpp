/** 
* @file guiaslite.cpp
* @brief action sequence lite for non-widgets, such as camera.
* @author ken
* @date 2010-12-23
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiaslite.h>
#include <libguiex_core/guimath.h>

#define GUIASLITE_MIN_TOTALTIME (0.01f)

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsLite
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsLite::CGUIAsLite( )
		:m_fTotalTime(GUIASLITE_MIN_TOTALTIME)
		,m_fElapsedTime(0.0f)
		,m_bLooping(false)
		,m_bRetired(false)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destructor
	*/
	CGUIAsLite::~CGUIAsLite()
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the flag after this as has been processed.
	*/
	void CGUIAsLite::Retire( bool bRetired )
	{
		m_bRetired = bRetired;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief whether the as is retired.
	*/
	bool CGUIAsLite::IsRetired()
	{
		return m_bRetired;
	}
	//------------------------------------------------------------------------------
	void CGUIAsLite::SetLooping( bool bLooping )
	{
		m_bLooping = bLooping;
	}
	//------------------------------------------------------------------------------
	bool CGUIAsLite::IsLooping()
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set total time
	*/
	void CGUIAsLite::SetTotalTime(real fTotalTime)
	{
		if( fTotalTime <= 0.0f )
		{
			fTotalTime = GUIASLITE_MIN_TOTALTIME;
		}
		m_fTotalTime = fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAsLite::GetTotalTime( ) const
	{
		return m_fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAsLite::GetElapsedTime( ) const
	{
		return m_fElapsedTime;
	}
	//------------------------------------------------------------------------------
	void CGUIAsLite::SetElapsedTime( real fElapsedTime )
	{
		if( fElapsedTime < 0.0f )
		{
			m_fElapsedTime = 0.0f;
		}
		else
		{
			m_fElapsedTime = fElapsedTime;
		}
	}
	//------------------------------------------------------------------------------
	real CGUIAsLite::Update( real fDeltaTime )
	{
		m_fElapsedTime += fDeltaTime;
		if( m_fElapsedTime >= m_fTotalTime )
		{
			real fLeftTime = m_fElapsedTime - m_fTotalTime;

			if( IsLooping() )
			{
				m_fElapsedTime -= m_fTotalTime;
				while( m_fElapsedTime >= m_fTotalTime )
				{
					fLeftTime = m_fElapsedTime - m_fTotalTime;
					m_fElapsedTime -= m_fTotalTime;	
				}

				return Update( fLeftTime );
			}
			else
			{
				m_fElapsedTime = m_fTotalTime;
				Retire( true );
				return fLeftTime;
			}
		}
		return 0.0f;
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsLite_Container
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsLite_Container::CGUIAsLite_Container( )
		:CGUIAsLite( )
	{
	}
	//------------------------------------------------------------------------------
	CGUIAsLite_Container::~CGUIAsLite_Container( )
	{
		for( TAsLiteList::iterator itor = m_vAsLiteList.begin();
			itor != m_vAsLiteList.end();
			++itor )
		{
			delete *itor;
		}
		m_vAsLiteList.clear();
	}
	//------------------------------------------------------------------------------
	real CGUIAsLite_Container::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsLite::Update( fDeltaTime );

		for( TAsLiteList::iterator itor = m_vAsLiteList.begin();
			itor != m_vAsLiteList.end();
			++itor )
		{
			CGUIAsLite_ContainItemInfo* pInfo = *itor;
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
		}

		return fLeftTime;
	}
	//------------------------------------------------------------------------------
	void CGUIAsLite_Container::AddItem( const CGUIAsLite& rAsLite, real fBeginTime )
	{
		CGUIAsLite_ContainItemInfo* pNewInfo = new  CGUIAsLite_ContainItemInfo;
		pNewInfo->m_rAsLite = rAsLite;
		pNewInfo->m_fBeginTime = fBeginTime;

		//insert
		bool bInserted = false;
		for( TAsLiteList::iterator itor = m_vAsLiteList.begin();
			itor != m_vAsLiteList.end();
			++itor )
		{
			const CGUIAsLite_ContainItemInfo* pInfo = *itor;
			if( pInfo->m_fBeginTime >= pNewInfo->m_fBeginTime )
			{
				m_vAsLiteList.insert( itor, pNewInfo );
				bInserted = true;
				break;
			}
		}
		if( !bInserted )
		{
			//last one
			m_vAsLiteList.push_back( pNewInfo );
		}

		//update time
		if( pNewInfo->m_rAsLite.GetTotalTime() + pNewInfo->m_fBeginTime > GetTotalTime() )
		{
			SetTotalTime( pNewInfo->m_rAsLite.GetTotalTime() + pNewInfo->m_fBeginTime );
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex


