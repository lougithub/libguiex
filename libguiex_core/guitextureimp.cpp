/** 
* @file guitextureimp.cpp
* @brief abstract class for texture
* @author ken
* @date 2006-07-05
*/


//============================================================================//
// include
//============================================================================// 
#include "guitextureimp.h"
#include "guitexture.h"
#include "guiinterfacerender.h"
#include "guilogmsgmanager.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUITextureImp::CGUITextureImp(IGUIInterfaceRender* pRender)
		:m_pRender(pRender)
		,m_pTexture(NULL)
		,m_ePixelFormat(GUI_PF_UNKNOEWN)
	{
		GUI_ASSERT( pRender, "no render" );
	}
	//------------------------------------------------------------------------------
	CGUITextureImp::~CGUITextureImp()
	{
		if( m_pTexture)
		{
			m_pTexture->NotifyDeletedFromImp();
		}
	}
	//------------------------------------------------------------------------------
	void CGUITextureImp::Destroy()
	{
		GUI_ASSERT( m_pRender, "invalid render pointer" );
		m_pRender->DestroyTexture( this );
	}
	//------------------------------------------------------------------------------
	void CGUITextureImp::SetTexture( CGUITexture* pTexture)
	{
		m_pTexture = pTexture;
	}
	//------------------------------------------------------------------------------
	CGUITexture* CGUITextureImp::GetTexture(  ) const
	{
		return m_pTexture;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
