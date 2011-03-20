/** 
 * @file guisceneeffectgrid3d.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


//============================================================================//
// include
//============================================================================//
#include "guisceneeffectgrid3d.h"
#include "guiexception.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guitexturemanager.h"
#include "guirendertype.h"
#include "guirect.h"

//TEMP
#include "guiimage.h"
#include "guiimagemanager.h"

//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneEffectGrid3D::CGUISceneEffectGrid3D( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize )
		:CGUISceneEffectGridBase( rSceneSize, rGridSize )
		,m_pTexCoordinates( NULL )
		,m_pVertices( NULL )
		,m_pOriginalVertices( NULL )
		,m_pIndices( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUISceneEffectGrid3D::~CGUISceneEffectGrid3D()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUISceneEffectGrid3D::Initialize(  )
	{
		//call parent function
		if( CGUISceneEffectGridBase::Initialize( ) != 0 )
		{
			return -1;
		}

		// allocating data space
		m_pVertices = new SR_V3F[(m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)];
		m_pOriginalVertices = new SR_V3F[(m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)];
		m_pTexCoordinates = new SR_T2F[(m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)];
		m_pIndices = new uint16[ m_aGridSize.m_uWidth * m_aGridSize.m_uHeight * 6];

		//real *vertArray = (real*)m_pVertices;
		//real *texArray = (real*)m_pTexCoordinates;
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

				uint16 l1[4] = { a, b, c, d };
				SR_V3F	e = {x1,y1,0};
				SR_V3F	f = {x2,y1,0};
				SR_V3F	g = {x2,y2,0};
				SR_V3F	h = {x1,y2,0};

				SR_V3F l2[4] = { e, f, g, h };

				uint16 tex1[4] = { a, b, c, d };
				SR_V2F tex2[4] = { {x1, y1}, {x2, y1}, {x2, y2}, {x1, y2} };

				for( int32 i = 0; i < 4; i++ )
				{
					m_pVertices[ l1[i] ] = l2[i];

					m_pTexCoordinates[ tex1[i] ].u = tex2[i].x / m_pTexture->GetWidth();
					if( m_bIsTextureFlipped )
					{
						m_pTexCoordinates[ tex1[i] ].v = (m_aSceneSize.m_fHeight - tex2[i].y) / m_pTexture->GetHeight();
					}
					else
					{
						m_pTexCoordinates[ tex1[i] ].v = tex2[i].y / m_pTexture->GetHeight();
					}
				}
			}
		}

		memcpy(m_pOriginalVertices, m_pVertices, (m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)*sizeof(SR_V3F));


		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectGrid3D::Release( )
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

		CGUISceneEffectGridBase::Release();
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		//uint32 n = m_aGridSize.m_uWidth * m_aGridSize.m_uHeight;
		//pRender->DrawGrid( m_pTexture->GetTextureImplement(), m_pTexCoordinates, m_pVertices, m_pIndices, n );

		pRender->DrawTile( CGUIRect( 0,0, m_pTexture->GetWidth(), m_pTexture->GetHeight() ), 0, m_pTexture->GetTextureImplement(), CGUIRect(0,0,1,1),  eImageOrientation_Normal, CGUIColor());
		
		CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource("scrollbar_downbutton_highlight" );
		pRender->DrawTile( CGUIRect( CGUIVector2(100,100), pImage->GetSize()*4 ), 0, pImage->GetTexture()->GetTextureImplement(), CGUIRect(0,0,1,1),  eImageOrientation_Normal, CGUIColor());
		pImage->RefRelease();
 
	}
	//------------------------------------------------------------------------------
	/** 
	* returns the vertex than belongs to certain position in the grid 
	*/
	const SR_V3F& CGUISceneEffectGrid3D::GetVertex( const CGUIIntSize& rPos )
	{
		GUI_ASSERT( rPos.m_uWidth <= m_aGridSize.m_uWidth && rPos.m_uHeight <= m_aGridSize.m_uHeight, "invalid pos" );

		int	index = (rPos.m_uWidth * (m_aGridSize.m_uHeight+1) + rPos.m_uHeight);
		return m_pVertices[index]; 
	}
	//------------------------------------------------------------------------------
	/** 
	* returns the non-transformed vertex than belongs to certain position in the grid 
	*/
	const SR_V3F& CGUISceneEffectGrid3D::GetOriginalVertex( const CGUIIntSize& rPos )
	{
		GUI_ASSERT( rPos.m_uWidth <= m_aGridSize.m_uWidth && rPos.m_uHeight <= m_aGridSize.m_uHeight, "invalid pos" );
		
		int	index = (rPos.m_uWidth * (m_aGridSize.m_uHeight+1) + rPos.m_uHeight);
		return m_pOriginalVertices[index];
	}
	//------------------------------------------------------------------------------
	/**
	 * sets a new vertex to a certain position of the grid 
	 */
	void CGUISceneEffectGrid3D::SetVertex( const CGUIIntSize& rPos, const SR_V3F& rVertex )
	{
		GUI_ASSERT( rPos.m_uWidth <= m_aGridSize.m_uWidth && rPos.m_uHeight <= m_aGridSize.m_uHeight, "invalid pos" );

		int	index = (rPos.m_uWidth * (m_aGridSize.m_uHeight+1) + rPos.m_uHeight);
		m_pOriginalVertices[index] = rVertex;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
