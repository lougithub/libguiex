/** 
 * @file guisceneeffect.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


//============================================================================//
// include
//============================================================================//
#include "guisceneeffect.h"
#include "guiexception.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guitexturemanager.h"
#include "guiinterfacemanager.h"
#include "guisystem.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneEffect::CGUISceneEffect( const CGUISize& rSceneSize )
		:m_pTexture( NULL )
		,m_fbo( 0 )
		,m_oldfbo( 0 )
		,m_aSceneSize( rSceneSize )
	{
		m_aBlendFunc.src = eBlendFunc_ONE;
		m_aBlendFunc.dst = eBlendFunc_ZERO;
	}
	//------------------------------------------------------------------------------
	CGUISceneEffect::~CGUISceneEffect()
	{
		if( m_pTexture )
		{
			throw CGUIException("[CGUISceneEffect::~CGUISceneEffect]: texture not cleared");
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneEffect::Initialize( )
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
		if( !pRender )
		{
			throw CGUIException("[CGUISceneEffect::Initialize]: failed to get render interface!");
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
			throw CGUIException("[CGUISceneEffect::Initialize]: failed to create texture!");
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
			throw CGUIException("[CGUISceneEffect::Initialize]: Could not attach texture to framebuffer!");
			return -1;
		}

		//restore fbo
		pRender->BindFramebuffer( m_oldfbo );

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffect::Release( )
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture( m_pTexture );
			m_pTexture = NULL;

			IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
			if( !pRender )
			{
				throw CGUIException("[CGUISceneEffect::Release]: failed to get render interface!");
				return;
			}
			pRender->DeleteFramebuffers(1, &m_fbo);
			m_fbo = 0;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffect::RefRelease()
	{
		DoDecreaseReference();

		if( GetRefCount() == 0 )
		{
			Release();
			delete this;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffect::BeforeRender( IGUIInterfaceRender* pRender )
	{
		if( !m_pTexture )
		{
			return;
		}

		//set matrix
		pRender->PushMatrix();
		pRender->LoadIdentityMatrix();

		//set viewport
		pRender->SetViewport(
			0,0,
			GUI_FLOAT2UINT_ROUND(m_aSceneSize.m_fWidth), 
			GUI_FLOAT2UINT_ROUND(m_aSceneSize.m_fHeight));

		//set framebuffer
		pRender->GetCurrentBindingFrameBuffer( &m_oldfbo );
		pRender->BindFramebuffer( m_fbo );

		pRender->ClearColor(0,0,0,0);
		pRender->Clear( eRenderBuffer_COLOR_BIT | eRenderBuffer_DEPTH_BIT );	
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffect::AfterRender( IGUIInterfaceRender* pRender )
	{
		if( !m_pTexture )
		{
			return;
		}

		//restore fbo
		pRender->BindFramebuffer( m_oldfbo );

		//restore view port
		const CGUIIntSize& rSize = GSystem->GetScreenSize();
		pRender->SetViewport(0,0,rSize.GetWidth(),rSize.GetHeight());

		//reset matrix
		pRender->LoadIdentityMatrix();

		//set blend func
		SGUIBlendFunc oldBlendFunc;
		pRender->GetBlendFunc( oldBlendFunc );
		pRender->SetBlendFunc( m_aBlendFunc );

		//process texture
		ProcessCaptureTexture( pRender );

		//restore blend func
		pRender->SetBlendFunc( oldBlendFunc );

		//reset matrix
		pRender->PopMatrix();
	}
	//------------------------------------------------------------------------------
}
