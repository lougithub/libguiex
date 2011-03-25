/** 
* @file guiasgridbase.cpp
* @brief
* @author ken
* @date 2011-03-16
*/

//============================================================================//
// include
//============================================================================// 
#include "guiasgridbase.h"
#include "guisceneeffectgridbase.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsGridBase
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsGridBase::CGUIAsGridBase( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs( rAsType, rAsName, rSceneName )
		,m_aGridSize( 1,1 )
		,m_pEffectGrid( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUIAsGridBase::~CGUIAsGridBase( )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::Reset( )
	{
		CGUIAs::Reset();
		if( m_pEffectGrid )
		{
			m_pEffectGrid->Reset();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::OnUpdate( )
	{
		CGUIAs::OnUpdate( );
		
		if( !IsGridInit() )
		{
			InitGrid();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::SetGridSize( const CGUIIntSize& rGridSize )
	{
		m_aGridSize = rGridSize;
	}
	//------------------------------------------------------------------------------
	const CGUIIntSize& CGUIAsGridBase::GetGridSize() const
	{
		return m_aGridSize;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsGridBase::ProcessProperty( const CGUIProperty& rProperty )
	{
		int32 ret = CGUIAs::ProcessProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsGridBase::GenerateProperty( CGUIProperty& rProperty )
	{
		int32 ret = CGUIAs::GenerateProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}



		return 0;
	}
	//------------------------------------------------------------------------------
	bool CGUIAsGridBase::IsGridInit() const
	{
		return m_pEffectGrid != NULL;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsGridBase::OnInitGrid()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::OnFiniGrid()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIAsGridBase::InitGrid( )
	{
		//check grid
		if( IsGridInit() )
		{
			throw CGUIException("[CGUIAsGridBase::InitGrid]: grid has been initialized!" );
			return -1;
		}

		//check receiver
		CGUIWidget* pReceiver = GetReceiver();
		if( !pReceiver )
		{
			throw CGUIException("[CGUIAsGridBase::InitGrid]: no receiver!" );
			return -1;
		}

		m_pEffectGrid = GenerateGrid( GetReceiver()->GetPixelSize(), m_aGridSize );
		if( !m_pEffectGrid )
		{
			throw CGUIException("[CGUIAsGridBase::InitGrid]: failed to create grid!" );
			return -1;
		}

		pReceiver->SetSceneEffect( m_pEffectGrid ); 

		if( 0 != OnInitGrid() )
		{
			throw CGUIException("[CGUIAsGridBase::InitGrid]: failed in OnInitGrid!" );
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::FiniGrid( )
	{
		if( !IsGridInit() )
		{
			return;
		}

		OnFiniGrid();

		if( GetReceiver() && GetReceiver()->GetSceneEffect() == m_pEffectGrid )
		{
			GetReceiver()->SetSceneEffect( NULL );
		}

		m_pEffectGrid->RefRelease();
		m_pEffectGrid = 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::OnRetired()
	{
		FiniGrid();
		CGUIAs::OnRetired();
	}
	//------------------------------------------------------------------------------
	void CGUIAsGridBase::OnDestory()
	{
		FiniGrid();
		CGUIAs::OnDestory();
	}
	//------------------------------------------------------------------------------
}
