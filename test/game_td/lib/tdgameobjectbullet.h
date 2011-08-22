/** 
* @file tdgameobjectbullet.h
* @brief bullet
* @author ken
* @date 2011-08-22
*/
#ifndef __TD_GAMEOBJECTBULLET_20110822_H__
#define __TD_GAMEOBJECTBULLET_20110822_H__

//============================================================================//
// include
//============================================================================// 
#include "tdgameobject.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CTDGameObjectBullet : public CTDGameObject
	{
	public:
		CTDGameObjectBullet( class CTDGameWorld* pGameWorld );
		virtual ~CTDGameObjectBullet();

	protected:
		friend class CTDGameWorld;

		virtual void OnActive();
		virtual void OnDeactive();

		virtual void OnRender( IGUIInterfaceRender* pRender );
		virtual void OnUpdate( real fDeltaTime );

	};
}

#endif //__TD_GAMEOBJECTBULLET_20110822_H__
