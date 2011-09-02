/** 
* @file tdgametower.cpp
* @brief tower of td game
* @author ken
* @date 2011-08-24
*/


//============================================================================//
// include 
//============================================================================// 
#include "tdgametower.h"
#include "tdgametowerimplement.h"
#include "tdgameworld.h"
#include "tdwgttower.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	static void Panel_OnPageLostFocus( CGUIEventNotification* pEvent )
	{
		GSystem->GetUICanvas()->CloseUIPage(pEvent->GetReceiver());
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	CTDGameTower::CTDGameTower( CTDWgtTower* pWidgetTower, CTDGameWorld * pGameWorld, const CGUIVector2& rAnchorPoint )
		:m_pGameWorld(pGameWorld)
		,m_eTowerType( eTowerType_Base )
		,m_aAnchorPoint( rAnchorPoint )
		,m_aRenderColor(1,1,1,1)
		,m_pWidgetTower(pWidgetTower)
	{
		m_pTowerImplement[eTowerType_Base] = new CTDGameTowerImplement_Base( this );
		m_pTowerImplement[eTowerType_ArcherTower] = new CTDGameTowerImplement_ArcherTower( this );
		m_pTowerImplement[eTowerType_Mages] = new CTDGameTowerImplement_Mages( this );
		m_pTowerImplement[eTowerType_Bombard] = new CTDGameTowerImplement_Bombard( this );
		m_pTowerImplement[eTowerType_Barracks] = new CTDGameTowerImplement_Barracks( this );
		SetTowerType( eTowerType_Base );

		m_aPosition = pWidgetTower->GetPixelPosition();
	}
	//------------------------------------------------------------------------------
	CTDGameTower::~CTDGameTower()
	{
		for( uint32 i=0; i<__eTowerType_MAX__; ++i )
		{
			delete m_pTowerImplement[i];
			m_pTowerImplement[i] = NULL;
		}
	}
	//------------------------------------------------------------------------------
	const CGUIVector2& CTDGameTower::GetPosition() const
	{
		return m_aPosition;
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::OnUpdate( real fDeltaTime )
	{
		m_pTowerImplement[m_eTowerType]->OnUpdate( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::OnRender( IGUIInterfaceRender* pRender )
	{
		m_pTowerImplement[m_eTowerType]->OnRender( pRender );
	}
	//------------------------------------------------------------------------------
	CTDGameWorld* CTDGameTower::GetGameWorld() const
	{
		return m_pGameWorld;
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::SetSize( const CGUISize& rSize )
	{
		m_aSize = rSize;
		m_aRenderRect = CGUIRect( CGUIVector2( -m_aSize.m_fWidth * m_aAnchorPoint.x, -m_aSize.m_fHeight*m_aAnchorPoint.y ), m_aSize );
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CTDGameTower::GetRenderRect() const
	{
		return m_aRenderRect;
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::SetTowerType( ETowerType eType )
	{
		if( m_eTowerType != eType)
		{
			m_pTowerImplement[m_eTowerType]->OnDestruct();
			m_eTowerType = eType;
		}
		m_pTowerImplement[m_eTowerType]->OnBuild();
	}
	//------------------------------------------------------------------------------
	ETowerType CTDGameTower::GetTowerType( ) const
	{
		return m_eTowerType;
	}
	//------------------------------------------------------------------------------
	bool CTDGameTower::CouldBuild( ETowerType eType ) const
	{
		if( GetTowerType() != eTowerType_Base )
		{
			return false;
		}
		if( m_pTowerImplement[eType]->GetTotalLevel() == 0 )
		{
			return false;
		}
		if( GetGameWorld()->GetGold() < m_pTowerImplement[eType]->GetLevelInfo(0)->m_uPrice )
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::BuildTower(ETowerType eType)
	{
		GUI_ASSERT( CouldBuild( eType ), "[CTDGameTower::BuildTower]: couldn't build tower");
		GetGameWorld()->CostGold( m_pTowerImplement[eType]->GetLevelInfo(0)->m_uPrice );
		SetTowerType( eType );
	}
	//------------------------------------------------------------------------------
	bool CTDGameTower::CouldUpgrade( )
	{
		if( GetTowerType() == eTowerType_Base )
		{
			return false;
		}
		CTDGameTowerImplement* pTowerImp = m_pTowerImplement[GetTowerType()];
		if( pTowerImp->GetTotalLevel() == 0 )
		{
			return false;
		}
		if( pTowerImp->GetCurrentLevel() >= pTowerImp->GetTotalLevel()-1 )
		{
			return false;
		}
		if( GetGameWorld()->GetGold() < pTowerImp->GetUpgradeCost() )
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::UpgradeTower( )
	{
		GUI_ASSERT( CouldUpgrade( ), "[CTDGameTower::UpgradeTower]: couldn't upgrade tower");
		GetGameWorld()->CostGold( m_pTowerImplement[GetTowerType()]->GetUpgradeCost() );
		m_pTowerImplement[GetTowerType()]->Upgrade( );
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::SellTower( )
	{

	}
	//------------------------------------------------------------------------------
	void CTDGameTower::CloseTowerSelectPanel()
	{
		GSystem->GetUICanvas()->CloseUIPage(GetGameWorld()->GetTowerSelectPanel());
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::CloseTowerUpgradePanel()
	{
		GSystem->GetUICanvas()->CloseUIPage(GetGameWorld()->GetTowerUpgradePanel());
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::OnMouseEnter()
	{
		m_aRenderColor.SetColor(0.5f,0.5f,0.5f,0.5f);
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::OnMouseLeave()
	{
		m_aRenderColor.SetColor(1.f,1.f,1.f,1.f);
	}
	//------------------------------------------------------------------------------
	void CTDGameTower::OnMouseLeftClick()
	{
		//open tower select panel
		GetGameWorld()->SetCurrentFocusTower( this );

		CGUIWidget* pPanel = NULL;
		if( m_eTowerType == eTowerType_Base )
		{
			pPanel = GetGameWorld()->GetTowerSelectPanel();
		}
		else
		{
			pPanel = GetGameWorld()->GetTowerUpgradePanel();
		}

		pPanel->RegisterNativeCallbackFunc( "OnPageLostFocus", Panel_OnPageLostFocus);
		pPanel->SetGlobalPixelPosition( m_pWidgetTower->GetGlobalPixelPosition() );
		GSystem->GetUICanvas()->SetPopupWidget( pPanel );
		GSystem->GetUICanvas()->OpenUIPage( pPanel );
	}
	//------------------------------------------------------------------------------
	const CGUIColor& CTDGameTower::GetRenderColor() const
	{
		return m_aRenderColor;
	}
	//------------------------------------------------------------------------------
}//namespace guiex


