/** 
* @file guiphysics_box2d.cpp
* @brief use box2d as physics engine
* @author ken
* @date 2010-12-21
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <libguiex_core/guisystem.h>
#include <Box2D/Box2D.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIPhysics_box2d);
	//------------------------------------------------------------------------------
	real IGUIPhysics_box2d::ms_fScale = 10.0f;
	//------------------------------------------------------------------------------
	const char* IGUIPhysics_box2d::StaticGetModuleName()
	{
		return "IGUIPhysics_box2d";
	}
	//------------------------------------------------------------------------------
	IGUIPhysics_box2d::IGUIPhysics_box2d()
		:IGUIInterfacePhysics( StaticGetModuleName() )
		,m_pWorld(NULL)
	{
	}
	//------------------------------------------------------------------------------
	IGUIPhysics_box2d::~IGUIPhysics_box2d()
	{
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int	IGUIPhysics_box2d::DoInitialize(void* )
	{
		//create world
		b2Vec2 gravity( 0.0f, 10.0f );
		bool bDoSleep = true;
		m_pWorld = new b2World( gravity, bDoSleep );

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::DoDestroy()
	{
		if( m_pWorld )
		{
			// By deleting the world, we delete the bomb, mouse joint, etc.
			delete m_pWorld;
			m_pWorld = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::Update( real fDeltaTime )
	{
		guiex::int32 velocityIterations = 10;
		guiex::int32 positionIterations = 2;

		m_pWorld->Step( fDeltaTime, velocityIterations, positionIterations );
		m_pWorld->ClearForces();
	}
	//------------------------------------------------------------------------------
	b2World* IGUIPhysics_box2d::GetWorld()
	{
		return m_pWorld;
	}
	//------------------------------------------------------------------------------
	real IGUIPhysics_box2d::Meter2Pixel( real fMeter )
	{
		return fMeter * ms_fScale;
	}
	//------------------------------------------------------------------------------
	real IGUIPhysics_box2d::Pixel2Meter( real fPixel )
	{
		return fPixel / ms_fScale;
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::SetMeterPixelScale( real fScale )
	{
		ms_fScale = fScale;
	}
	//------------------------------------------------------------------------------
	real IGUIPhysics_box2d::GetMeterPixelScale( )
	{
		return ms_fScale;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
