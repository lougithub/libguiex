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


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneCapture::CGUISceneCapture( )
		:m_pTexture( NULL )
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
	void CGUISceneCapture::Initialize( uint32 uTextureWidth, uint32 uTextureHeight )
	{
		Release();

		//create texture
		uint32 uTextureWidthUse = 1;
		while( uTextureWidthUse < uTextureWidth )
		{
			uTextureWidthUse <<= 1;
		}
		uint32 uTextureHeightUse = 1;
		while( uTextureHeightUse < uTextureHeight )
		{
			uTextureHeightUse <<= 1;
		}
		m_pTexture = CGUITextureManager::Instance()->CreateTexture( uTextureWidthUse, uTextureHeightUse, GUI_PF_RGBA_32 );
		if( !m_pTexture )
		{
			throw CGUIException("[CGUISceneCapture::Initialize]: failed to create texture!");
		}

		//create frame buffer
		
	}
	//------------------------------------------------------------------------------
	void CGUISceneCapture::Release( )
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture( m_pTexture );
			m_pTexture = NULL;
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

		pRender->PushMatrix();
		pRender->LoadIdentityMatrix();
	}
	//------------------------------------------------------------------------------
	void CGUISceneCapture::AfterRender( IGUIInterfaceRender* pRender )
	{
		if( !m_pTexture )
		{
			return;
		}

		pRender->PopMatrix();

		pRender->PushMatrix();
		pRender->LoadIdentityMatrix();
		ProcessCaptureTexture( pRender );
		pRender->PopMatrix();
	}
	//------------------------------------------------------------------------------
}
