/** 
* @file guitextureimp.cpp
* @brief abstract class for texture
* @author ken
* @date 2006-07-05
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guiinterfacerender.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUITextureImp::CGUITextureImp(IGUIInterfaceRender* pRender)
		:m_pRender(pRender)
		,m_pTexture(NULL)
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
