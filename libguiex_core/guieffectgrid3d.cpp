/** 
 * @file guieffectgrid3d.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


//============================================================================//
// include
//============================================================================//
#include "guieffectgrid3d.h"
#include "guiexception.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guitexturemanager.h"
#include "guirendertype.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIEffectGrid3D::CGUIEffectGrid3D( const CGUIIntSize& rSceneSize, const CGUIIntSize& rGridSize )
		:CGUIEffectGridBase( rSceneSize, rGridSize )
		,m_pTexCoordinates( NULL )
		,m_pVertices( NULL )
		,m_pOriginalVertices( NULL )
		,m_pIndices( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUIEffectGrid3D::~CGUIEffectGrid3D()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIEffectGrid3D::Initialize(  )
	{
		//call parent function
		if( CGUIEffectGridBase::Initialize( ) != 0 )
		{
			return -1;
		}

		// allocating data space
		m_pVertices = new SR_V3F[(m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)];
		m_pOriginalVertices = new SR_V3F[(m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)];
		m_pTexCoordinates = new SR_T2F[(m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)];
		m_pIndices = new uint16[ m_aGridSize.m_uWidth * m_aGridSize.m_uHeight * 6];

		real *vertArray = (real*)m_pVertices;
		real *texArray = (real*)m_pTexCoordinates;
		uint16 *idxArray = (uint16 *)m_pIndices;

		for( uint32 x = 0; x < m_aGridSize.m_uWidth; x++ )
		{
			for( uint32 y = 0; y < m_aGridSize.m_uHeight; y++ )
			{
				int idx = (y * m_aGridSize.m_uWidth) + x;

				uint16 a = x * (m_aGridSize.m_uHeight+1) + y;
				uint16 b = (x+1) * (m_aGridSize.m_uHeight+1) + y;
				uint16 c = (x+1) * (m_aGridSize.m_uHeight+1) + (y+1);
				uint16 d = x * (m_aGridSize.m_uHeight+1) + (y+1);

				uint16	tempidx[6] = { a, b, d, b, c, d };
				memcpy( &idxArray[6*idx], tempidx, 6*sizeof(uint16) );

				real x1 = x * m_aStep.x;
				real x2 = x1 + m_aStep.x;
				real y1 = y * m_aStep.y;
				real y2 = y1 + m_aStep.y;

				uint16 l1[4] = { a*3, b*3, c*3, d*3 };
				SR_V3F	e = {x1,y1,0};
				SR_V3F	f = {x2,y1,0};
				SR_V3F	g = {x2,y2,0};
				SR_V3F	h = {x1,y2,0};

				SR_V3F l2[4] = { e, f, g, h };

				uint16 tex1[4] = { a*2, b*2, c*2, d*2 };
				SR_V2F tex2[4] = { {x1, y1}, {x2, y1}, {x2, y2}, {x1, y2} };

				for( int32 i = 0; i < 4; i++ )
				{
					vertArray[ l1[i] ] = l2[i].x;
					vertArray[ l1[i] + 1 ] = l2[i].y;
					vertArray[ l1[i] + 2 ] = l2[i].z;

					texArray[ tex1[i] ] = tex2[i].x / m_pTexture->GetWidth();
					if( m_bIsTextureFlipped )
					{
						texArray[ tex1[i] + 1 ] = (m_aSceneSize.m_fHeight - tex2[i].y) / m_pTexture->GetHeight();
					}
					else
					{
						texArray[ tex1[i] + 1 ] = tex2[i].y / m_pTexture->GetHeight();
					}
				}
			}
		}

		memcpy(m_pOriginalVertices, m_pVertices, (m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)*sizeof(SR_V3F));


		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIEffectGrid3D::Release( )
	{
		if( m_pTexCoordinates )
		{
			delete[] m_pTexCoordinates;
			m_pTexCoordinates = NULL;
		}
		if( m_pVertices )
		{
			delete[] m_pVertices;
			m_pVertices = NULL;
		}
		if( m_pOriginalVertices )
		{
			delete[] m_pOriginalVertices;
			m_pOriginalVertices = NULL;
		}
		if( m_pIndices )
		{
			delete[] m_pIndices;
			m_pIndices = NULL;
		}

		CGUIEffectGridBase::Release();
	}
	//------------------------------------------------------------------------------
	void CGUIEffectGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		CGUIEffectGridBase::ProcessCaptureTexture( pRender );


	}
	//------------------------------------------------------------------------------
}//namespace guiex
