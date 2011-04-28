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
#include "guimath.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneEffect::CGUISceneEffect( const CGUISize& rSceneSize )
		:m_pTexture( NULL )
		,m_aSceneSize( rSceneSize )
#if GUI_SCENEEFFECT_USE_VBO
		,m_fbo( 0 )
		,m_oldfbo( 0 )
#if GUI_SCENEEFFECT_USE_RENDERBUFFER
		,m_rbo( 0 )
#endif //#if GUI_SCENEEFFECT_USE_RENDERBUFFER

#endif //#if GUI_SCENEEFFECT_USE_VBO
	{
		m_aBlendFunc.src = eBlendFunc_ONE;
		m_aBlendFunc.dst = eBlendFunc_ZERO;

		if( m_aSceneSize.m_fHeight < 1.0f )
		{
			m_aSceneSize.m_fHeight = 1.0f;
		}
		if( m_aSceneSize.m_fWidth < 1.0f )
		{
			m_aSceneSize.m_fWidth = 1.0f;
		}
	}
	//------------------------------------------------------------------------------
	CGUISceneEffect::~CGUISceneEffect()
	{
		if( m_pTexture )
		{
			CGUIException::ThrowException("[CGUISceneEffect::~CGUISceneEffect]: texture not cleared");
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneEffect::Initialize( )
	{
		IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
		if( !pRender )
		{
			CGUIException::ThrowException("[CGUISceneEffect::Initialize]: failed to get render interface!");
			return -1;
		}

		Release();

		//create texture
		uint32 uTextureWidthUse = CGUITexture::ConvertToTextureSize( GetSceneWidth() );
		uint32 uTextureHeightUse = CGUITexture::ConvertToTextureSize( GetSceneHeight() );
		m_pTexture = CGUITextureManager::Instance()->CreateTexture( uTextureWidthUse, uTextureHeightUse, GUI_PF_RGBA_32 );
		if( !m_pTexture )
		{
			CGUIException::ThrowException("[CGUISceneEffect::Initialize]: failed to create texture!");
			return -1;
		}
		m_pTexture->SetBottomUp( true );

#if GUI_SCENEEFFECT_USE_VBO
		//create frame buffer
		pRender->GenFramebuffers(1, &m_fbo);
		pRender->GetBindingFrameBuffer( &m_oldfbo );
		pRender->BindFramebuffer( m_fbo );

#if GUI_SCENEEFFECT_USE_RENDERBUFFER
		//create render buffer
		pRender->GenRenderbuffers( 1, &m_rbo );
		pRender->BindRenderbuffer( m_rbo );
		pRender->RenderbufferStorage_Depth( GetSceneWidth(), GetSceneHeight() );
		pRender->BindRenderbuffer( 0 );
#endif //#if GUI_SCENEEFFECT_USE_RENDERBUFFER

		// attach a texture to FBO color attachement point
		pRender->FramebufferTexture2D_Color( m_pTexture, 0);
		
#if GUI_SCENEEFFECT_USE_RENDERBUFFER
		// attach a renderbuffer to depth attachment point
		pRender->FramebufferRenderbuffer_Depth( m_rbo );
#endif //#if GUI_SCENEEFFECT_USE_RENDERBUFFER

		// check if it worked
		if( !pRender->CheckFramebufferStatus( ) )
		{
			CGUIException::ThrowException("[CGUISceneEffect::Initialize]: Could not attach texture to framebuffer!");
			return -1;
		}

		//restore fbo
		pRender->BindFramebuffer( m_oldfbo );

#endif //#if GUI_SCENEEFFECT_USE_VBO

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
				CGUIException::ThrowException("[CGUISceneEffect::Release]: failed to get render interface!");
				return;
			}

#if GUI_SCENEEFFECT_USE_VBO
			if( m_fbo != 0 )
			{
				pRender->DeleteFramebuffers(1, &m_fbo);
				m_fbo = 0;
			}

#if GUI_SCENEEFFECT_USE_RENDERBUFFER
			if( m_rbo != 0 )
			{
				pRender->DeleteRenderbuffers(1, &m_rbo);
				m_rbo = 0;
			}
#endif //#if GUI_SCENEEFFECT_USE_RENDERBUFFER

#endif //#if GUI_SCENEEFFECT_USE_VBO
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
	uint32 CGUISceneEffect::GetSceneWidth( ) const
	{
		return GUI_FLOAT2UINT_ROUND( m_aSceneSize.m_fWidth );
	}
	//------------------------------------------------------------------------------
	uint32 CGUISceneEffect::GetSceneHeight( ) const
	{
		return GUI_FLOAT2UINT_ROUND( m_aSceneSize.m_fHeight );
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffect::BeforeRender( IGUIInterfaceRender* pRender )
	{
#if GUI_SCENEEFFECT_USE_VBO
		//set framebuffer
		pRender->GetBindingFrameBuffer( &m_oldfbo );
		pRender->BindFramebuffer( m_fbo );
#else
		// clear buffer
		pRender->PushAttrib(eAttribMask_COLOR_BUFFER_BIT | eAttribMask_PIXEL_MODE_BIT);
		pRender->DrawBuffer(eBufferMode_Back);
		pRender->ReadBuffer(eBufferMode_Back);

#endif //#if GUI_SCENEEFFECT_USE_VBO

		//set viewport
		pRender->SetViewport( 0,m_pTexture->GetHeight() - GetSceneHeight(),GetSceneWidth(), GetSceneHeight() );

		//set matrix
		pRender->LoadIdentityMatrix();

		pRender->ClearColor(0,0,0,0);
		pRender->Clear( eRenderBuffer_COLOR_BIT | eRenderBuffer_DEPTH_BIT );	
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffect::AfterRender( IGUIInterfaceRender* pRender )
	{
#if GUI_SCENEEFFECT_USE_VBO
		//restore fbo
		pRender->BindFramebuffer( m_oldfbo );
#else
		// copy the framebuffer pixels to a texture
		pRender->BindTexture( m_pTexture );
		pRender->CopyTexSubImage2D( 0, 0, 0, 0, 0, GetSceneWidth(), GetSceneHeight());
		pRender->PopAttrib();
#endif //#if GUI_SCENEEFFECT_USE_VBO

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
	}
	//------------------------------------------------------------------------------
}
