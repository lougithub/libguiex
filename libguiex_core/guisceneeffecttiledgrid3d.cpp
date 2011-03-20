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
	CGUISceneEffectTiledGrid3D::CGUISceneEffectTiledGrid3D( const CGUIIntSize& rSceneSize, const CGUIIntSize& rGridSize )
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
		real width = m_pTexture->GetWidth();
		real height = m_pTexture->GetHeight();
		
		//call parent function
		if( CGUISceneEffectGridBase::Initialize( ) != 0 )
		{
			return -1;
		}

		// allocating data space	
		uint32 numQuads = m_aGridSize.m_uWidth * m_aGridSize.m_uHeight;
		m_pVertices = new SR_V3F[numQuads * 4];
		m_pOriginalVertices = new SR_V3F[numQuads * 4];
		m_pTexCoordinates = new SR_T2F[numQuads * 4];
		m_pIndices = new uint16[ numQuads * 6];

		SR_V3F *vertArray = m_pVertices;
		SR_T2F *texArray = m_pTexCoordinates;
		uint16 *idxArray = m_pIndices;

		for( uint32 x = 0; x < m_aGridSize.m_uWidth; x++ )
		{
			for( uint32 y = 0; y < m_aGridSize.m_uHeight; y++ )
			{
				real x1 = x * m_aStep.x;
				real x2 = x1 + m_aStep.x;
				real y1 = y * m_aStep.y;
				real y2 = y1 + m_aStep.y;

				vertArray->x = x1;
				vertArray->y = y1;
				vertArray->z = 0;
				++vertArray;
				vertArray->x = x2;
				vertArray->y = y1;
				vertArray->z = 0;
				++vertArray;
				vertArray->x = x1;
				vertArray->y = y2;
				vertArray->z = 0;
				++vertArray;
				vertArray->x = x2;
				vertArray->y = y2;
				vertArray->z = 0;
				++vertArray;

				real newY1 = y1;
				real newY2 = y2;

				if( m_bIsTextureFlipped ) 
				{
					newY1 = m_aSceneSize.m_fHeight - y1;
					newY2 = m_aSceneSize.m_fHeight - y2;
				}

				texArray->u = x1 / width;
				texArray->v = newY1 / height;
				++texArray;
				texArray->u = x2 / width;
				texArray->v = newY1 / height;
				++texArray;
				texArray->u = x1 / width;
				texArray->v = newY2 / height;
				++texArray;
				texArray->u = x2 / width;
				texArray->v = newY2 / height;
				++texArray;
			}
		}

		for( uint32 x = 0; x < numQuads; x++)
		{
			idxArray[x*6+0] = x*4+0;
			idxArray[x*6+1] = x*4+1;
			idxArray[x*6+2] = x*4+2;

			idxArray[x*6+3] = x*4+1;
			idxArray[x*6+4] = x*4+2;
			idxArray[x*6+5] = x*4+3;
		}

		memcpy(m_pOriginalVertices, m_pVertices, numQuads*4*sizeof(SR_V3F));

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
	void CGUISceneEffectTiledGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		CGUISceneEffectGridBase::ProcessCaptureTexture( pRender );

	}
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
}//namespace guiex
