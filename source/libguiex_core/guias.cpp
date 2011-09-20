/** 
* @file guias.cpp
* @brief widget action sequence
* @author ken
* @date 2007-11-21
*/

//============================================================================//
// include
//============================================================================// 
#include "guias.h"
#include "guiwidget.h"
#include "guisystem.h"
#include "guimath.h"
#include "guiasmanager.h"

#define GUIAS_MIN_TOTALTIME (0.01f)

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAs
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAs::CGUIAs( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIResource( rAsName, rSceneName, "AS", GSystem->GetAsManager() )
		,m_fTotalTime(GUIAS_MIN_TOTALTIME)
		,m_fElapsedTime(0.0f)
		,m_bLooping(false)
		,m_bReverse(false)
		,m_bRetired(false)
		,m_pReceiver(NULL)
		,m_strAsType(rAsType)
	{
		Load();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destructor
	*/
	CGUIAs::~CGUIAs()
	{
		ClearSuccessor();
	}
	//------------------------------------------------------------------------------
	void CGUIAs::Reset( )
	{
		m_bRetired = false;
		m_fElapsedTime = 0.0f;

		for(TArraySuccessor::iterator itor = m_arraySuccessor.begin();
			itor != m_arraySuccessor.end();
			++itor)
		{
			( *itor )->Reset();
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIAs::ProcessProperty( const CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsWidgetColor">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="10" />
		*		<property name="elapsed_time" type="real" value="5" />
		*		<property name="reverse" type="bool" value="false" />
		*</property>
		*/
		{
			bool bLoop = false;
			const CGUIProperty* pPptLoop = rProperty.GetProperty("loop", "bool");
			if( pPptLoop )
			{
				PropertyToValue( *pPptLoop, bLoop );
			}
			SetLooping( bLoop );
		}	
		
		{
			bool bReverse = false;
			const CGUIProperty* pPptReverse = rProperty.GetProperty("reverse", "bool");
			if( pPptReverse )
			{
				PropertyToValue( *pPptReverse, bReverse );
			}
			SetReverse( bReverse );
		}

		{
			real fTotalTime = GUIAS_MIN_TOTALTIME;
			const CGUIProperty* pPptTotalTime = rProperty.GetProperty("total_time", "real");
			if( pPptTotalTime )
			{
				PropertyToValue( *pPptTotalTime, fTotalTime );
			}
			SetTotalTime( fTotalTime );
		}

		{
			real fElapsedTime = 0.0;
			const CGUIProperty* pPPtElapsedTime = rProperty.GetProperty("elapsed_time", "real");
			if( pPPtElapsedTime )
			{
				PropertyToValue( *pPPtElapsedTime, fElapsedTime );
			}
			SetElapsedTime( fElapsedTime );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAs::GenerateProperty( CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsWidgetColor">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="10" />
		*		<property name="elapsed_time" type="real" value="5" />
		*</property>
		*/
		{
			CGUIProperty aProperty("loop", "bool");
			ValueToProperty( IsLooping(), aProperty );
			rProperty.AddProperty( aProperty );
		}


		{
			CGUIProperty aProperty( "total_time", "real" );
			ValueToProperty( GetTotalTime(), aProperty );
			rProperty.AddProperty( aProperty );
		}

		{
			CGUIProperty aProperty( "elapsed_time", "real" );
			ValueToProperty( GetElapsedTime(), aProperty );
			rProperty.AddProperty( aProperty );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get as type
	*/
	const CGUIString& CGUIAs::GetType() const
	{
		return m_strAsType;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the flag after this as has been processed.
	*/
	void CGUIAs::Retire( bool bRetired )
	{
		if( !m_bRetired && bRetired )
		{
			m_bRetired = bRetired;
			OnRetired();
		}
		else
		{
			m_bRetired = bRetired;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief whether the as is retired.
	*/
	bool CGUIAs::IsRetired()
	{
		return m_bRetired;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetLooping( bool bLooping )
	{
		m_bLooping = bLooping;
	}
	//------------------------------------------------------------------------------
	bool CGUIAs::IsLooping()
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetReverse( bool bReverse )
	{
		m_bReverse = bReverse;
	}
	//------------------------------------------------------------------------------
	bool CGUIAs::IsReverse() const
	{
		return m_bReverse;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set widget which receives this as
	*/
	void CGUIAs::SetReceiver(CGUIWidget* pReceiver)
	{
		m_pReceiver = pReceiver;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Get widget which receives this as
	*/
	CGUIWidget*	CGUIAs::GetReceiver()
	{
		return m_pReceiver;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set total time
	*/
	void CGUIAs::SetTotalTime(real fTotalTime)
	{
		if( fTotalTime <= 0.0f )
		{
			fTotalTime = GUIAS_MIN_TOTALTIME;
		}
		m_fTotalTime = fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAs::GetTotalTime( ) const
	{
		return m_fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAs::GetElapsedTime( ) const
	{
		return m_fElapsedTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAs::GetPercent() const
	{
		if( IsReverse() )
		{
			return 1.0f - GetElapsedTime() / GetTotalTime();
		}
		else
		{
			return GetElapsedTime() / GetTotalTime();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetElapsedTime( real fElapsedTime )
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
	real CGUIAs::Update( real fDeltaTime )
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
				OnUpdate();
				Retire( true );
				return fLeftTime;
			}
		}

		OnUpdate();
		return 0.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::OnUpdate()
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAs::OnDestory()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAs::OnRetired()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAs::AddSuccessor( CGUIAs* pAs)
	{
		GUI_ASSERT(pAs, "wrong parameter");

		pAs->RefRetain();
		m_arraySuccessor.push_back(pAs);
	}
	//------------------------------------------------------------------------------
	uint32 CGUIAs::GetSuccessorNum( ) const
	{
		return uint32(m_arraySuccessor.size());
	}
	//------------------------------------------------------------------------------
	CGUIAs*	CGUIAs::GetSuccessor( uint32 nIndex )
	{
		GUI_ASSERT( nIndex<uint32(m_arraySuccessor.size()), "wrong parameter");
		return m_arraySuccessor[nIndex];
	}
	//------------------------------------------------------------------------------
	void CGUIAs::RemoveSuccessor( uint32 nIndex )
	{
		GUI_ASSERT( nIndex<uint32(m_arraySuccessor.size()), "wrong parameter");
		m_arraySuccessor[nIndex]->RefRelease();
		m_arraySuccessor.erase( m_arraySuccessor.begin() + nIndex );
	}
	//------------------------------------------------------------------------------
	void CGUIAs::ClearSuccessor( )
	{
		for(TArraySuccessor::iterator itor = m_arraySuccessor.begin();
			itor != m_arraySuccessor.end();
			++itor)
		{
			( *itor )->RefRelease();
		}
		m_arraySuccessor.clear();
	}
	//------------------------------------------------------------------------------
	int32 CGUIAs::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::DoUnload()
	{

	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUICustomAs
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUICustomAs::CGUICustomAs( const CGUIString& rAsType )
		:CGUIAs( rAsType, "", "" )
	{
	}
	//------------------------------------------------------------------------------
	void CGUICustomAs::RefRelease()
	{
		DoDecreaseReference();
		if( GetRefCount() == 0 )
		{
			Unload();
			DestroySelf();
		}
	}
	//------------------------------------------------------------------------------
	void CGUICustomAs::DestroySelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsCallFunc
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsCallFunc::CGUIAsCallFunc(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs("CGUIAsCallFunc", rAsName, rSceneName)
		,m_funCallback( NULL )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsCallFunc::SetFuncCallback( FunOnAsCallback funCallback )
	{
		m_funCallback = funCallback;
	}
	//------------------------------------------------------------------------------
	real CGUIAsCallFunc::Update( real fDeltaTime )
	{
		if( !IsRetired() )
		{
			if( m_funCallback )
			{
				m_funCallback( this );
			}
			Retire( true );
		}

		return 0.0f;
	}
	//------------------------------------------------------------------------------


}//namespace guiex


