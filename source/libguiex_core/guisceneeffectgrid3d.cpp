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
		,m_pVerticeInfos( NULL )
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
		uint32 uVertexNum = (m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1);
		// allocating data space
		m_pVertices = new SVertexFormat_V3F[uVertexNum];
		m_pOriginalVertices = new SVertexFormat_V3F[uVertexNum];
		m_pVerticeInfos = new SVertexFormat_T2F_C4UB[uVertexNum];
		m_pIndices = new uint16[ m_aGridSize.m_uWidth * m_aGridSize.m_uHeight * 6];
		uint32 uColor = CGUIColor( 1,1,1,1 ).GetAsABGR();

		for( uint32 x = 0; x < m_aGridSize.m_uWidth; x++ )
		{
			for( uint32 y = 0; y < m_aGridSize.m_uHeight; y++ )
			{
				uint16 idxTL = x * (m_aGridSize.m_uHeight+1) + y; //top left
				uint16 idxTR = (x+1) * (m_aGridSize.m_uHeight+1) + y; //top right
				uint16 idxBR = (x+1) * (m_aGridSize.m_uHeight+1) + (y+1); //bottom right
				uint16 idxBL = x * (m_aGridSize.m_uHeight+1) + (y+1); //bottom left

				uint16	tempidx[6] = { idxTL, idxBL, idxTR, idxBR, idxTR, idxBL };
				int idx = (x * m_aGridSize.m_uHeight) + y;
				memcpy( &m_pIndices[6*idx], tempidx, 6*sizeof(uint16) );

				real x1 = x * m_aStep.x;
				real x2 = x1 + m_aStep.x;
				real y1 = y * m_aStep.y;
				real y2 = y1 + m_aStep.y;

				SVertexFormat_V3F	vTL = {x1,y1,0}; //top left
				SVertexFormat_V3F	vTR = {x2,y1,0}; //top right
				SVertexFormat_V3F	vBR = {x2,y2,0}; //bottom right
				SVertexFormat_V3F	vBL = {x1,y2,0}; //bottom left

				uint16 indexs[4] = { idxTL, idxTR, idxBR, idxBL };
				SVertexFormat_V3F vertices[4] = { vTL, vTR, vBR, vBL };

				for( int32 i = 0; i < 4; i++ )
				{
					m_pVertices[ indexs[i] ] = vertices[i];

					m_pVerticeInfos[ indexs[i] ].texCoords.u = vertices[i].x / m_pTexture->GetWidth();
					if( m_bIsTextureFlipped )
					{
						m_pVerticeInfos[ indexs[i] ].texCoords.v = m_pTexture->UVConvertTopleft2Engine_v( (m_aSceneSize.m_fHeight - vertices[i].y) / m_pTexture->GetHeight());
					}
					else
					{
						m_pVerticeInfos[ indexs[i] ].texCoords.v = m_pTexture->UVConvertTopleft2Engine_v(vertices[i].y / m_pTexture->GetHeight());
					}
					m_pVerticeInfos[indexs[i]].color.abgr = uColor;
				}
			}
		}

		memcpy(m_pOriginalVertices, m_pVertices, uVertexNum*sizeof(SVertexFormat_V3F));
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectGrid3D::Release( )
	{
		if( m_pVerticeInfos )
		{
			delete[] m_pVerticeInfos;
			m_pVerticeInfos = NULL;
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
	void CGUISceneEffectGrid3D::Reset( )
	{
		memcpy(m_pVertices, m_pOriginalVertices, (m_aGridSize.m_uWidth+1)*(m_aGridSize.m_uHeight+1)*sizeof(SVertexFormat_V3F));
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		uint32 n = m_aGridSize.m_uWidth * m_aGridSize.m_uHeight;
		pRender->DrawGrid( m_pTexture, m_pVerticeInfos, m_pVertices, m_pIndices, n );
		
		//pRender->DrawTile( 
		//	CGUIRect( CGUIVector2(), m_aSceneSize ), 
		//	0, 
		//	m_pTexture, 
		//	CGUIRect(0,0,m_aSceneSize.GetWidth() / m_pTexture->GetWidth(), m_aSceneSize.GetHeight() / m_pTexture->GetHeight()),  
		//	eImageOrientation_Normal, 
		//	CGUIColor());
	}
	//------------------------------------------------------------------------------
	/** 
	* returns the vertex than belongs to certain position in the grid 
	*/
	const SVertexFormat_V3F& CGUISceneEffectGrid3D::GetVertex( uint32 uX, uint32 uY )
	{
		GUI_ASSERT( uX <= m_aGridSize.m_uWidth && uY <= m_aGridSize.m_uHeight, "invalid pos" );

		int	index = (uX * (m_aGridSize.m_uHeight+1) + uY);
		return m_pVertices[index]; 
	}
	//------------------------------------------------------------------------------
	/** 
	* returns the non-transformed vertex than belongs to certain position in the grid 
	*/
	const SVertexFormat_V3F& CGUISceneEffectGrid3D::GetOriginalVertex( uint32 uX, uint32 uY )
	{
		GUI_ASSERT( uX <= m_aGridSize.m_uWidth && uY <= m_aGridSize.m_uHeight, "invalid pos" );
		
		int	index = (uX * (m_aGridSize.m_uHeight+1) + uY);
		return m_pOriginalVertices[index];
	}
	//------------------------------------------------------------------------------
	/**
	 * sets a new vertex to a certain position of the grid 
	 */
	void CGUISceneEffectGrid3D::SetVertex( uint32 uX, uint32 uY, const SVertexFormat_V3F& rVertex )
	{
		GUI_ASSERT( uX <= m_aGridSize.m_uWidth && uY <= m_aGridSize.m_uHeight, "invalid pos" );

		int	index = (uX * (m_aGridSize.m_uHeight+1) + uY);
		m_pVertices[index] = rVertex;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
