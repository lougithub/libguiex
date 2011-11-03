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
#include <libguiex_core/guirendertype.h>
#include <libguiex_core/guicolor.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacerender.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	// This class implements debug drawing callbacks that are invoked
	// inside b2World::Step.
	class CGUIBox2DDebugDraw : public b2Draw
	{
	public:
		CGUIBox2DDebugDraw( );
		void SetRender( IGUIInterfaceRender* pRender );

		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

		virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

		virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

		virtual void DrawTransform(const b2Transform& xf);

		virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

		virtual void DrawString(int x, int y, const char* string, ...); 

		virtual void DrawAABB(b2AABB* aabb, const b2Color& color);

	protected:
		IGUIInterfaceRender* m_pRender;
		SVertexFormat_V3F_C4UB m_aVertexBuf[b2_maxPolygonVertices];
	};

	CGUIBox2DDebugDraw::CGUIBox2DDebugDraw( )
	{
		m_drawFlags = e_shapeBit | e_jointBit | e_aabbBit |  e_pairBit | e_centerOfMassBit;
	}

	void CGUIBox2DDebugDraw::SetRender( IGUIInterfaceRender* pRender )
	{
		m_pRender = pRender;
	}

	void CGUIBox2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		GUI_ASSERT( vertexCount <= b2_maxPolygonVertices, "invalid vertex count" );

		GUIABGR abgr = CGUIColor( color.r, color.g, color.b, 1.0f ).GetAsABGR();
		for( int32 i=0; i<vertexCount; ++i )
		{
			m_aVertexBuf[i].vertices.x = vertices[i].x;
			m_aVertexBuf[i].vertices.y = vertices[i].y;
			m_aVertexBuf[i].vertices.z = 0.0f;
			m_aVertexBuf[i].color.abgr = abgr;
		}
		m_pRender->DrawPolygon( m_aVertexBuf, vertexCount, 2, false );
	}

	void CGUIBox2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		GUI_ASSERT( vertexCount <= b2_maxPolygonVertices, "invalid vertex count" );
		
		for( int32 i=0; i<vertexCount; ++i )
		{
			m_aVertexBuf[i].vertices.x = vertices[i].x;
			m_aVertexBuf[i].vertices.y = vertices[i].y;
			m_aVertexBuf[i].vertices.z = 0.0f;
		}

		GUIABGR abgr = CGUIColor( 0.5f*color.r, 0.5f*color.g, 0.5f*color.b, 0.5f*1.0f ).GetAsABGR();
		for( int32 i=0; i<vertexCount; ++i )
		{
			m_aVertexBuf[i].color.abgr = abgr;

		}
		m_pRender->DrawPolygon( m_aVertexBuf, vertexCount, 1, true );

		abgr = CGUIColor( color.r, color.g, color.b, 1.0f ).GetAsABGR();
		for( int32 i=0; i<vertexCount; ++i )
		{
			m_aVertexBuf[i].color.abgr = abgr;
		}
		m_pRender->DrawPolygon( m_aVertexBuf, vertexCount, 2, false );
	}

	void CGUIBox2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		CGUIColor aColor1( 0.5f*color.r, 0.5f*color.g, 0.5f*color.b, 0.5f*1.0f );
		m_pRender->DrawCircle( CGUIVector2(center.x, center.y), radius, 2, true, 0, aColor1 );

		CGUIColor aColor2( color.r, color.g, color.b, 1.0f );
		m_pRender->DrawCircle( CGUIVector2(center.x, center.y), radius, 2, false, 0, aColor2 );
	}

	void CGUIBox2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		CGUIColor aColor1( color.r, color.g, color.b, 1.0f );
		m_pRender->DrawCircle( CGUIVector2(center.x, center.y), radius, 1, true, 0, aColor1 );
		
		CGUIColor aColor2( color.r, color.g, color.b, 1.0f );
		m_pRender->DrawCircle( CGUIVector2(center.x, center.y), radius, 2, false, 0, aColor2 );
	}

	void CGUIBox2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		CGUIColor aColor( color.r, color.g, color.b, 1.0f );
		m_pRender->DrawLine( CGUIVector2(p1.x, p1.y), CGUIVector2(p2.x, p2.y), 2, 0, aColor, aColor );
	}

	void CGUIBox2DDebugDraw::DrawTransform(const b2Transform& xf)
	{
		b2Vec2 p1 = xf.p, p2;
		const float32 k_axisScale = 0.4f;

		CGUIColor aColor1( 1, 0, 0, 1.0f );
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		m_pRender->DrawLine( CGUIVector2(p1.x, p1.y), CGUIVector2(p2.x, p2.y), 2, 0, aColor1, aColor1 );

		CGUIColor aColor2( 0, 1, 0, 1.0f );
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		m_pRender->DrawLine( CGUIVector2(p1.x, p1.y), CGUIVector2(p2.x, p2.y), 2, 0, aColor2, aColor2 );
	}

	void CGUIBox2DDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		CGUIColor aColor( color.r, color.g, color.b, 1.0f );
		m_pRender->DrawPoint( CGUIVector2(p.x, p.y), size, 0, aColor );
	}

	void CGUIBox2DDebugDraw::DrawString(int x, int y, const char *string, ...)
	{

	}

	void CGUIBox2DDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
	{
		GUIBGRA abgr = CGUIColor( color.r, color.g, color.b, 1.0f ).GetAsABGR();
		for( int32 i=0; i<4; ++i )
		{
			m_aVertexBuf[i].color.abgr = abgr;
			m_aVertexBuf[0].vertices.z = 0;
		}
		m_aVertexBuf[0].vertices.x = aabb->lowerBound.x;
		m_aVertexBuf[0].vertices.y = aabb->lowerBound.y;
		m_aVertexBuf[1].vertices.x = aabb->upperBound.x;
		m_aVertexBuf[1].vertices.y = aabb->lowerBound.y;
		m_aVertexBuf[2].vertices.x = aabb->upperBound.x;
		m_aVertexBuf[2].vertices.y = aabb->upperBound.y;
		m_aVertexBuf[3].vertices.x = aabb->lowerBound.x;
		m_aVertexBuf[4].vertices.y = aabb->upperBound.y;

		m_pRender->DrawPolygon( m_aVertexBuf, 4, 2, false);
	}

}


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
		,m_pDebugDraw(NULL)
		,m_bSimulateWorld(true)
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
		m_pWorld = new b2World( gravity );

		//set debug draw
		m_pDebugDraw = new CGUIBox2DDebugDraw( );
		m_pWorld->SetDebugDraw( m_pDebugDraw );
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::DoDestroy()
	{
		if( m_pDebugDraw )
		{
			delete m_pDebugDraw;
			m_pDebugDraw = NULL;
		}

		if( m_pWorld )
		{
			// By deleting the world, we delete the bomb, mouse joint, etc.
			delete m_pWorld;
			m_pWorld = NULL;
		}
	}
	//------------------------------------------------------------------------------
	bool IGUIPhysics_box2d::IsSimulating() const
	{
		return m_bSimulateWorld;
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::PauseSimulate()
	{
		m_bSimulateWorld = false;
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::BeginSimulate()
	{
		m_bSimulateWorld = true;
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::Update( real fDeltaTime )
	{
		if( m_bSimulateWorld )
		{
			int32 velocityIterations = 10;
			int32 positionIterations = 2;

			m_pWorld->Step( fDeltaTime, velocityIterations, positionIterations );
			m_pWorld->ClearForces();
		}
	}
	//------------------------------------------------------------------------------
	void IGUIPhysics_box2d::RenderExtraInfo( IGUIInterfaceRender* pRender )
	{
		static_cast<CGUIBox2DDebugDraw*>(m_pDebugDraw)->SetRender( pRender );
		m_pWorld->DrawDebugData();
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
