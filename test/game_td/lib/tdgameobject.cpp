/** 
* @file tdgameobject.cpp
* @brief game object, could be monster, bullet or other dynamic actor in game.
* @author ken
* @date 2011-08-22
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameobject.h"
#include "tdgameworld.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameObject::CTDGameObject( uint32 uObjectType, CTDGameWorld* pGameWorld )
		:m_uObjectType( uObjectType )
		,m_pGameWorld( pGameWorld )
		,m_bIsAlive( true )
	{
	}
	//------------------------------------------------------------------------------
	CTDGameObject::~CTDGameObject()
	{
	}
	//------------------------------------------------------------------------------
	uint32 CTDGameObject::GetType() const
	{
		return m_uObjectType;
	}
	//------------------------------------------------------------------------------
	CTDGameWorld* CTDGameObject::GetGameWorld() const
	{
		return m_pGameWorld;
	}
	//------------------------------------------------------------------------------
	void CTDGameObject::OnActive()
	{
		m_bIsAlive = true;
	}
	//------------------------------------------------------------------------------
	void CTDGameObject::OnDeactive()
	{
		m_bIsAlive = false;
	}
	//------------------------------------------------------------------------------
	bool CTDGameObject::IsAlive() const
	{
		return m_bIsAlive;
	}
	//------------------------------------------------------------------------------
	void CTDGameObject::KillObject()
	{
		m_bIsAlive = false;
	}
	//------------------------------------------------------------------------------
}

