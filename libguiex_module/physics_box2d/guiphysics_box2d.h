/** 
* @file guiphysics_box2d.h
* @brief use box2d as physics engine
* @author ken
* @date 2010-12-21
*/

#ifndef __GUI_PHYSICS_BOX2D_20101221_H__
#define __GUI_PHYSICS_BOX2D_20101221_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacephysics.h>
#include <vector>
#include <Box2D/Box2D.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT IGUIPhysics_box2d : public IGUIInterfacePhysics
	{
	public:
		IGUIPhysics_box2d();
		virtual ~IGUIPhysics_box2d();


	public:
		virtual void DeleteSelf();
		virtual	void Update( real fDeltaTime );

		b2World* GetWorld();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	protected:
		b2World * m_pWorld;

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_PHYSICS_BOX2D_20101221_H__
