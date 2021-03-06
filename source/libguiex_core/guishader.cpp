/** 
* @file guishader.cpp
* @brief shader used in widgets
* @author ken
* @date 2011-09-23
*/

//============================================================================//
// include 
//============================================================================// 
#include "guishader.h"
#include "guishaderimp.h"
#include "guisystem.h"
#include "guishadermanager.h"
#include "guiinterfacemanager.h"
#include "guiinterfacerender.h"
#include "guiscene.h"
#include "guiscenemanager.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIShader::CGUIShader( const CGUIString& rName, const CGUIString& rSceneName,
		const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName )
		:CGUIResource( rName, rSceneName, "SHADER", GSystem->GetShaderManager())
		,m_pShaderImp(NULL)
		,m_strVertexShaderFileName( rVertexShaderFileName )
		,m_strFragmentShaderFileName( rFragmentShaderFileName )
	{

	}
	//------------------------------------------------------------------------------
	CGUIShader::~CGUIShader()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIShader::DoLoad()
	{
		CreateShaderImplement();
		
		//load and compile shader
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIShader::DoUnload()
	{
		DestoryShaderImplement();
	}
	//------------------------------------------------------------------------------
	CGUIShaderImp* CGUIShader::GetShaderImp()
	{
#if defined(GUIEX_RENDER_OPENGL_ES1)
		return NULL;
#endif
		Load();
		return m_pShaderImp;
	}
	//------------------------------------------------------------------------------
	//!< notify when shader imp is deleted
	void CGUIShader::NotifyDeletedFromImp()
	{
		m_pShaderImp = NULL;
		Unload();
	}
	//------------------------------------------------------------------------------
	int32 CGUIShader::CreateShaderImplement()
	{
		if( !m_pShaderImp )
		{
			CGUIScene* pScene = CGUISceneManager::Instance()->GetScene( GetSceneName() );
			if( !pScene )
			{
				return -1;
			}

			IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
			m_pShaderImp = pRender->CreateShader( pScene->GetScenePath() + m_strVertexShaderFileName, pScene->GetScenePath() + m_strFragmentShaderFileName );
			m_pShaderImp->SetShader( this );

			GUI_ASSERT(m_pShaderImp, "failed to create shader");
			return 0;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIShader::DestoryShaderImplement()
	{
		if( m_pShaderImp)
		{
			m_pShaderImp->SetShader(NULL);
			m_pShaderImp->Destroy();
			m_pShaderImp = NULL;
		}
	}
	//------------------------------------------------------------------------------
}
