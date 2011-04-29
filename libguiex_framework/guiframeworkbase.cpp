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
	CGUIFrameworkBase::CGUIFrameworkBase( )
		:m_bIsInitialized( false )
	{
	}
	//------------------------------------------------------------------------------
	CGUIFrameworkBase::~CGUIFrameworkBase()
	{
		if( m_bIsInitialized )
		{
			CGUIException::ThrowException("[CGUIFrameworkBase::~CGUIFrameworkBase]: CGUIFrameworkBase is deleted but not released" );
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIFrameworkBase::Initialize( const CGUIIntSize& rScreenSize, const char* pDataPath )
	{
#if GUI_USE_EXCEPTION
		try
#endif
		{
			if( m_bIsInitialized )
			{
				return 0;
			}
			else
			{
				//initialize system
				if( 0 != InitializeSystem( rScreenSize, pDataPath ))
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
#if GUI_USE_EXCEPTION
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
			return -1;
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
			return -1;
		}
#endif
	}
	//------------------------------------------------------------------------------ 
	void CGUIFrameworkBase::Release()
	{
#if GUI_USE_EXCEPTION
		try
#endif
		{
			if( m_bIsInitialized )
			{
				ReleaseGame();
				ReleaseSystem();
				m_bIsInitialized = false;
			}	
		}
#if GUI_USE_EXCEPTION
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::Update( real fDeltaTime )
	{
#if GUI_USE_EXCEPTION
		try
#endif
		{
			PreUpdate( fDeltaTime );
			UpdateSystem( fDeltaTime );
			PostUpdate( fDeltaTime );
		}
#if GUI_USE_EXCEPTION
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::UpdateSystem( real fDeltaTime )
	{
		GSystem->Update( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::PreRender( )
	{
		GSystem->BeginRender();
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::PostRender( )
	{
		GSystem->EndRender();
	}
	//------------------------------------------------------------------------------
	void CGUIFrameworkBase::Render( )
	{
#if GUI_USE_EXCEPTION
		try
#endif
		{
			PreRender( );
			GSystem->Render();
			PostRender( );
		}
#if GUI_USE_EXCEPTION
		catch (std::exception& rError)
		{
			OutputFatalError( rError.what() );
		}
		catch (...)
		{
			OutputFatalError( "unknown error" );
		}
#endif
	}
	//------------------------------------------------------------------------------


}//namespace guiex

