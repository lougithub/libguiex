/** 
 * @file guiscenecapture.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


//============================================================================//
// include
//============================================================================//
#include "guiscenecapture.h"
#include "guiexception.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guitexturemanager.h"
#include "guiinterfacemanager.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneCapture::CGUISceneCapture( const CGUISize& rSceneSize )
		:m_pTexture( NULL )
		,m_fbo( 0 )
		,m_oldfbo( 0 )
		,m_aSceneSize( rSceneSize )
	{
	}
	//------------------------------------------------------------------------------
	CGUISceneCapture::~CGUISceneCapture()
	{
		if( m_pTexture )
		{
			throw CGUIException("[CGUISceneCapture::~CGUISceneCapture]: texture not cleared");
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneCapture::Initialize( )
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
		if( !pRender )
		{
			throw CGUIException("[CGUISceneCapture::Initialize]: failed to get render interface!");
			return -1;
		}

		Release();

		//create texture
		uint32 uTextureWidthUse = 1;
		while( uTextureWidthUse < m_aSceneSize.m_fWidth )
		{
			uTextureWidthUse <<= 1;
		}
		uint32 uTextureHeightUse = 1;
		while( uTextureHeightUse < m_aSceneSize.m_fHeight )
		{
			uTextureHeightUse <<= 1;
		}
		m_pTexture = CGUITextureManager::Instance()->CreateTexture( uTextureWidthUse, uTextureHeightUse, GUI_PF_RGBA_32 );
		if( !m_pTexture )
		{
			throw CGUIException("[CGUISceneCapture::Initialize]: failed to create texture!");
			return -1;
		}

		//create frame buffer
		pRender->GenFramebuffers(1, &m_fbo);
		pRender->GetCurrentBindingFrameBuffer( &m_oldfbo );

		// bind
		pRender->BindFramebuffer( m_fbo );

		// associate texture with FBO
		pRender->FramebufferTexture2D_Color( m_pTexture->GetTextureImplement(), 0);

		// check if it worked
		if( !pRender->CheckFramebufferStatus( ) )
		{
			throw CGUIException("[CGUISceneCapture::Initialize]: Could not attach texture to framebuffer!");
			return -1;
		}

		//restore fbo
		pRender->BindFramebuffer( m_oldfbo );

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneCapture::Release( )
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture( m_pTexture );
			m_pTexture = NULL;

			IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
			if( !pRender )
			{
				throw CGUIException("[CGUISceneCapture::Release]: failed to get render interface!");
				return;
			}
			pRender->DeleteFramebuffers(1, &m_fbo);
			m_fbo = 0;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneCapture::RefRelease()
	{
		DoDecreaseReference();

		if( GetRefCount() == 0 )
		{
			Release();
			delete this;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneCapture::BeforeRender( IGUIInterfaceRender* pRender )
	{
		if( !m_pTexture )
		{
			return;
		}

		//set matrix
		pRender->PushMatrix();
		pRender->LoadIdentityMatrix();

		//set framebuffer
		pRender->GetCurrentBindingFrameBuffer( &m_oldfbo );
		pRender->BindFramebuffer( m_fbo );

		pRender->ClearColor(0,0,0,0);
		pRender->Clear( eRenderBuffer_COLOR_BIT | eRenderBuffer_DEPTH_BIT );	
	}
	//------------------------------------------------------------------------------
	void CGUISceneCapture::AfterRender( IGUIInterfaceRender* pRender )
	{
		if( !m_pTexture )
		{
			return;
		}

		//restore fbo
		pRender->BindFramebuffer( m_oldfbo );

		ProcessCaptureTexture( pRender );

		pRender->PopMatrix();
	}
	//------------------------------------------------------------------------------
}
