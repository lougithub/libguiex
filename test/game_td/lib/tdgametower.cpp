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
#include "tdgameworld.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	// CTDGameTowerImplement
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement( CTDGameTower* pGameTower, const CGUIString& rTowerType );
		virtual ~CTDGameTowerImplement();

		virtual void OnUpdate( real fDeltaTime );
		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnBuild(){}
		virtual void OnDestruct(){}

		CTDGameTower* GetGameTower() const { return m_pGameTower; }
		CTDGameWorld* GetGameWorld() const { return m_pGameTower->GetGameWorld(); }

		const CGUIRect& GetRenderRect() const {return m_pGameTower->GetRenderRect();}
		const CGUIColor& GetRenderColor() const {return m_pGameTower->GetRenderColor();}

	protected:
		CGUIAnimation* GetAnimation( const CGUIString& rAnimName );
		void SetCurrentAnimation( CGUIAnimation* pAnim ){m_pCurrentAnim = pAnim;}

	protected:
		CGUIAnimation* m_pCurrentAnim;

	private:
		CTDGameTower* m_pGameTower;
		CGUIString m_strTowerType;
		std::map<CGUIString, CGUIAnimation*> m_mapAnimations;
	};
	//------------------------------------------------------------------------------
	CTDGameTowerImplement::CTDGameTowerImplement( CTDGameTower* pGameTower, const CGUIString& rTowerType )
		:m_pGameTower( pGameTower )
		,m_strTowerType( rTowerType )
		,m_pCurrentAnim( NULL )
	{
		const CGUIProperty* pAllTowerProperty = GetGameWorld()->GetDataProperty("tower.xml");
		const CGUIProperty* pMonsterProp = pAllTowerProperty->GetProperty(m_strTowerType);

		for( uint32 i=0; i<pMonsterProp->GetPropertyNum(); ++i )
		{
			const CGUIProperty* pProp = pMonsterProp->GetProperty( i );
			if( pProp->GetType() == ePropertyType_Animation )
			{
				GUI_ASSERT( m_mapAnimations.find(pProp->GetName()) == m_mapAnimations.end(), "[CTDGameTowerImplement::]: duplicate animation name");
				m_mapAnimations[pProp->GetName()] = CGUIAnimationManager::Instance()->AllocateResource( pProp->GetValue() );
			}
		}
	}
	//------------------------------------------------------------------------------
	CTDGameTowerImplement::~CTDGameTowerImplement()
	{
		for( std::map<CGUIString, CGUIAnimation*>::iterator itor = m_mapAnimations.begin();
			itor != m_mapAnimations.end();
			++itor )
		{
			itor->second->RefRelease();
		}
		m_mapAnimations.clear();
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CTDGameTowerImplement::GetAnimation( const CGUIString& rAnimName )
	{
		std::map<CGUIString, CGUIAnimation*>::iterator itor = m_mapAnimations.find( rAnimName );
		if( itor == m_mapAnimations.end() )
		{
			GUI_THROW( GUI_FORMAT("[CTDGameTowerImplement::GetAnimation]: can't find animation by name: %s", rAnimName.c_str()) );
			return NULL;
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement::OnUpdate( real fDeltaTime )
	{
		if( m_pCurrentAnim )
		{
			m_pCurrentAnim->Update( fDeltaTime );
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement::OnRender( IGUIInterfaceRender* pRender )
	{
		if( m_pCurrentAnim )
		{
			m_pCurrentAnim->Draw( pRender, GetRenderRect(), 0, GetRenderColor(), 1 );
		}
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Base
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Base : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_Base( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_Base(){}

		virtual void OnUpdate( real fDeltaTime ){ CTDGameTowerImplement::OnUpdate( fDeltaTime );}
		virtual void OnRender( IGUIInterfaceRender* pRender );

		virtual void OnBuild();

	protected:
		CGUIAnimation* m_pAnimTowerBase;
	};
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Base::CTDGameTowerImplement_Base( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_base")
	{
		m_pAnimTowerBase = GetAnimation( "AnimIdle" );
		GetGameTower()->SetSize( m_pAnimTowerBase->GetSize());
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Base::OnBuild()
	{
		SetCurrentAnimation( m_pAnimTowerBase );
	}
	//------------------------------------------------------------------------------
	void CTDGameTowerImplement_Base::OnRender( IGUIInterfaceRender* pRender )
	{
		CTDGameTowerImplement::OnRender( pRender );
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_ArcherTower
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_ArcherTower : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_ArcherTower( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_ArcherTower(){}

		virtual void OnUpdate( real fDeltaTime ){ CTDGameTowerImplement::OnUpdate( fDeltaTime );}
		virtual void OnRender( IGUIInterfaceRender* pRender ){ CTDGameTowerImplement::OnRender( pRender );}
	};
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_ArcherTower::CTDGameTowerImplement_ArcherTower( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_archertower")
	{

	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Mages
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Mages : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_Mages( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_Mages(){}

		virtual void OnUpdate( real fDeltaTime ){ CTDGameTowerImplement::OnUpdate( fDeltaTime );}
		virtual void OnRender( IGUIInterfaceRender* pRender ){ CTDGameTowerImplement::OnRender( pRender );}
	};
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Mages::CTDGameTowerImplement_Mages( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_mages")
	{

	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_Barracks
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_Barracks : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_Barracks( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_Barracks(){}

		virtual void OnUpdate( real fDeltaTime ){ CTDGameTowerImplement::OnUpdate( fDeltaTime );}
		virtual void OnRender( IGUIInterfaceRender* pRender ){ CTDGameTowerImplement::OnRender( pRender );}
	};
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_Barracks::CTDGameTowerImplement_Barracks( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_barracks")
	{

	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// CTDGameTowerImplement_DwarvenBoombard
	//------------------------------------------------------------------------------
	class CTDGameTowerImplement_DwarvenBoombard : public CTDGameTowerImplement
	{
	public:
		CTDGameTowerImplement_DwarvenBoombard( CTDGameTower* pGameTower );
		virtual ~CTDGameTowerImplement_DwarvenBoombard(){}

		virtual void OnUpdate( real fDeltaTime ){ CTDGameTowerImplement::OnUpdate( fDeltaTime );}
		virtual void OnRender( IGUIInterfaceRender* pRender ){ CTDGameTowerImplement::OnRender( pRender );}
	};
	//------------------------------------------------------------------------------
	CTDGameTowerImplement_DwarvenBoombard::CTDGameTowerImplement_DwarvenBoombard( CTDGameTower* pGameTower )
		:CTDGameTowerImplement( pGameTower, "tower_dwarvenboombard")
	{

	}
	//------------------------------------------------------------------------------
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameTower::CTDGameTower( CTDGameWorld * pGameWorld, const CGUIVector2& rAnchorPoint )
		:m_pGameWorld(pGameWorld)
		,m_eTowerType( eTowerType_Base )
		,m_aAnchorPoint( rAnchorPoint )
		,m_aRenderColor(1,1,1,1)
	{
		m_pTowerImplement[eTowerType_Base] = new CTDGameTowerImplement_Base( this );
		m_pTowerImplement[eTowerType_ArcherTower] = new CTDGameTowerImplement_ArcherTower( this );
		m_pTowerImplement[eTowerType_Mages] = new CTDGameTowerImplement_Mages( this );
		m_pTowerImplement[eTowerType_DwarvenBoombard] = new CTDGameTowerImplement_DwarvenBoombard( this );
		m_pTowerImplement[eTowerType_Barracks] = new CTDGameTowerImplement_Barracks( this );

		SetTowerType( eTowerType_Base );
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
	const CGUIColor& CTDGameTower::GetRenderColor() const
	{
		return m_aRenderColor;
	}
	//------------------------------------------------------------------------------
}//namespace guiex


