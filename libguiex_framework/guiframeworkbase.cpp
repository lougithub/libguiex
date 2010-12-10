/** 
 * @file guiframeworkbase.h
 * @brief framework to use this engine
 * @author ken
 * @date 2010-12-10
 */

//============================================================================//
// include 
//============================================================================// 
#include "guiframework.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFrameworkBase::CGUIFrameworkBase( const CGUISize& rScreenSize, const char* pDataPath )
		:m_bIsInitialized( false )
		,m_aScreenSize( rScreenSize )
		,m_strDataPath( pDataPath )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFrameworkBase::~CGUIFrameworkBase()
	{
		Release( );
	}
	//------------------------------------------------------------------------------
	int32 CGUIFrameworkBase::Initialize( )
	{
		try
		{
			if( m_bIsInitialized )
			{
				return 0;
			}
			else
			{
				//initialize system
				if( 0 != InitializeSystem( ))
				{
					ReleaseSystem();
					return -1;
				}

				//initialize game
				if( 0 != InitializeGame( ))
				{
					ReleaseGame();
					ReleaseSystem();
					return -1;
				}

				m_bIsInitialized = true;
				return 0;
			}
		}		
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
			return -1;
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
	}
	//------------------------------------------------------------------------------ 
	void CGUIFrameworkBase::Release()
	{
		try
		{
			if( m_bIsInitialized )
			{
				ReleaseGame();
				ReleaseSystem();
				m_bIsInitialized = false;
			}	
		}
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::Update( real fDeltaTime )
	{
		try
		{
			PreUpdate( fDeltaTime );

			UpdateSystem( fDeltaTime );

			PostUpdate( fDeltaTime );
		}
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::UpdateSystem( real fDeltaTime )
	{
		GSystem->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::Render( )
	{
		try
		{
			GSystem->Render();
		}
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIFrameworkBase::GetScreenSize( ) const
	{
		return m_aScreenSize;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIFrameworkBase::GetDataPath( ) const
	{
		return m_strDataPath;
	}
	//------------------------------------------------------------------------------


}//namespace guiex

