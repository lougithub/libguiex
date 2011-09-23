/** 
* @file guishaderimp.cpp
* @brief shader used in widgets
* @author ken
* @date 2011-09-23
*/

//============================================================================//
// include 
//============================================================================// 
#include "guishaderimp.h"
#include "guishader.h"
#include "guiinterfacerender.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIShaderImp::CGUIShaderImp(IGUIInterfaceRender* pRender)
		:m_pRender( pRender )
		,m_pShader( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUIShaderImp::~CGUIShaderImp()
	{
		if( m_pShader)
		{
			m_pShader->NotifyDeletedFromImp();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIShaderImp::Destroy()
	{
		GUI_ASSERT( m_pRender, "invalid render pointer" );
		m_pRender->DestroyShader( this );
	}
	//------------------------------------------------------------------------------
	void CGUIShaderImp::SetShader( CGUIShader* pShader)
	{
		m_pShader = pShader;
	}
	//------------------------------------------------------------------------------
	CGUIShader* CGUIShaderImp::GetShader( ) const
	{
		return m_pShader;
	}
	//------------------------------------------------------------------------------
}
