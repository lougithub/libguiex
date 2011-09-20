/** 
 * @file guisceneeffecttiledgrid3d.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


//============================================================================//
// include
//============================================================================//
#include "guisceneeffecttiledgrid3d.h"
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
	CGUISceneEffectTiledGrid3D::CGUISceneEffectTiledGrid3D( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize )
		:CGUISceneEffectGridBase( rSceneSize, rGridSize )
		 ,m_pTexCoordinates( NULL )
		 ,m_pVertices( NULL )
		 ,m_pOriginalVertices( NULL )
		 ,m_pIndices( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUISceneEffectTiledGrid3D::~CGUISceneEffectTiledGrid3D()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUISceneEffectTiledGrid3D::Initialize( )
	{
		//call parent function
		if( CGUISceneEffectGridBase::Initialize( ) != 0 )
		{
			return -1;
		}

		real width = m_pTexture->GetWidth();
		real height = m_pTexture->GetHeight();

		// allocating data space	
		uint32 numQuads = m_aGridSize.m_uWidth * m_aGridSize.m_uHeight;
		m_pVertices = new SR_V3F_Quad[numQuads];
		m_pOriginalVertices = new SR_V3F_Quad[numQuads];
		m_pTexCoordinates = new SR_T2F[numQuads * 4];
		m_pIndices = new uint16[ numQuads * 6];

		for( uint32 x = 0; x < m_aGridSize.m_uWidth; x++ )
		{
			for( uint32 y = 0; y < m_aGridSize.m_uHeight; y++ )
			{
				real x1 = x * m_aStep.x;
				real x2 = x1 + m_aStep.x;
				real y1 = y * m_aStep.y;
				real y2 = y1 + m_aStep.y;

				int idx = (x * m_aGridSize.m_uHeight) + y;

				m_pVertices[idx].tl.x = x1; //top left
				m_pVertices[idx].tl.y = y1;
				m_pVertices[idx].tl.z = 0;
				m_pVertices[idx].bl.x = x1; //bottom left
				m_pVertices[idx].bl.y = y2;
				m_pVertices[idx].bl.z = 0;
				m_pVertices[idx].tr.x = x2; //top right
				m_pVertices[idx].tr.y = y1;
				m_pVertices[idx].tr.z = 0;
				m_pVertices[idx].br.x= x2; // bottom right
				m_pVertices[idx].br.y = y2;
				m_pVertices[idx].br.z = 0;

				real newY1 = y1;
				real newY2 = y2;
				if( m_bIsTextureFlipped ) 
				{
					newY1 = m_aSceneSize.m_fHeight - y1;
					newY2 = m_aSceneSize.m_fHeight - y2;
				}

				uint32 nTexIdx = idx*4;
				m_pTexCoordinates[nTexIdx].u = x1 / width; //top left
				m_pTexCoordinates[nTexIdx].v = m_pTexture->UVConvertTopleft2Engine_v( newY1 / height );
				m_pTexCoordinates[nTexIdx+1].u = x1 / width; //bottom left
				m_pTexCoordinates[nTexIdx+1].v = m_pTexture->UVConvertTopleft2Engine_v( newY2 / height );
				m_pTexCoordinates[nTexIdx+2].u = x2 / width; //top right
				m_pTexCoordinates[nTexIdx+2].v = m_pTexture->UVConvertTopleft2Engine_v( newY1 / height );
				m_pTexCoordinates[nTexIdx+3].u = x2 / width; //bottom right
				m_pTexCoordinates[nTexIdx+3].v = m_pTexture->UVConvertTopleft2Engine_v( newY2 / height );
			}
		}

		for( uint32 x = 0; x < numQuads; x++)
		{
			uint32 nIdx = x*6;

			m_pIndices[nIdx+0] = x*4+0; //tl
			m_pIndices[nIdx+1] = x*4+1; //bl
			m_pIndices[nIdx+2] = x*4+2; //tr

			m_pIndices[nIdx+3] = x*4+3; //br
			m_pIndices[nIdx+4] = x*4+2; //tr
			m_pIndices[nIdx+5] = x*4+1; //bl
		}

		memcpy(m_pOriginalVertices, m_pVertices, numQuads*sizeof(SR_V3F_Quad));

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectTiledGrid3D::Release( )
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
	void CGUISceneEffectTiledGrid3D::Reset( )
	{
		memcpy(m_pVertices, m_pOriginalVertices, m_aGridSize.m_uWidth * m_aGridSize.m_uHeight*sizeof(SR_V3F_Quad));
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectTiledGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		uint32 n = m_aGridSize.m_uWidth * m_aGridSize.m_uHeight;
		pRender->DrawGrid( m_pTexture, m_pTexCoordinates, (SR_V3F*)m_pVertices, m_pIndices, n );
	}
	//------------------------------------------------------------------------------
	const SR_V3F_Quad& CGUISceneEffectTiledGrid3D::GetTile( uint32 uX, uint32 uY )
	{
		GUI_ASSERT( uX < m_aGridSize.m_uWidth && uY < m_aGridSize.m_uHeight, "invalid pos" );

		int	index = uX * m_aGridSize.m_uHeight + uY;
		return m_pVertices[index]; 
	}
	//------------------------------------------------------------------------------
	const SR_V3F_Quad& CGUISceneEffectTiledGrid3D::GetOriginalTile( uint32 uX, uint32 uY )
	{
		GUI_ASSERT( uX < m_aGridSize.m_uWidth && uY < m_aGridSize.m_uHeight, "invalid pos" );

		int	index = uX * m_aGridSize.m_uHeight + uY;
		return m_pOriginalVertices[index];
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectTiledGrid3D::SetTile( uint32 uX, uint32 uY, const SR_V3F_Quad& rTile )
	{
		GUI_ASSERT( uX < m_aGridSize.m_uWidth && uY < m_aGridSize.m_uHeight, "invalid pos" );

		int	index = uX * m_aGridSize.m_uHeight + uY;
		m_pVertices[index] = rTile;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
