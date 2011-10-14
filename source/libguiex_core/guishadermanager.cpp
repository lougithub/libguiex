/** 
* @file guishadermanager.cpp
* @brief shader manager.
* @author ken
* @date 2011-09-23
*/

//============================================================================//
// include 
//============================================================================// 
#include "guishadermanager.h"
#include "guishader.h"
#include "guiproperty.h"
#include "guistringconvertor.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guiexception.h"
#include "guipropertyconvertor.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIShaderManager * CGUIShaderManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIShaderManager::CGUIShaderManager()
		:CGUIResourceManager <CGUIShader, CGUIShader>("Shader")
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIShaderManager::CGUIShaderManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIShaderManager::~CGUIShaderManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIShaderManager* CGUIShaderManager::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	int32 CGUIShaderManager::RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		/*
		<property name="shader_default" type="CGUIShaderDefine" >
			<property name="vertex" type="CGUIString" value="shader/default.vert" />
			<property name="fragment" type="CGUIString" value="shader/default/frag" />
		</property>
		*/
		CGUIString strVertexShader = rProperty.GetProperty("vertex")->GetValue();
		CGUIString strFragmentShader = rProperty.GetProperty("fragment")->GetValue();
		
		CGUIShader* pShader = new CGUIShader( rProperty.GetName(), rSceneName, strVertexShader, strFragmentShader );
		RegisterResourceImp( pShader );

		//set default system shader.
		if( pShader->GetName() == "default_stencil" )
		{
			GSystem->SetDefaultShader_Stencil( pShader );
		}
		else if( pShader->GetName() == "default_render" )
		{
			GSystem->SetDefaultShader_Render( pShader );
		}
		//else if( pShader->GetName() == "default_font" )
		//{
		//	GSystem->SetDefaultShader_Font( pShader );
		//}
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIShader* CGUIShaderManager::AllocateResource( const CGUIString& rResName ) const
	{
		CGUIShader* pShader = CGUIResourceManager<CGUIShader, CGUIShader>::GetRegisterResource( rResName );
		if( !pShader )
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIShaderManager::AllocateResource]: failed to get shader by name <%s>",
				rResName.c_str()));
			return NULL;
		}
		pShader->RefRetain();
		return pShader;
	}
	//------------------------------------------------------------------------------
	void CGUIShaderManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			if( pRes->GetName() == "" &&
				pRes->GetSceneName() == "" )
			{
				//this shader is not registered as a named shader, free it
				ReleaseFromAllocatePool( pRes );
			}
			else
			{
				//named shader's reference count shouldn't be zero, which is retained by register function
				GUI_THROW( GUI_FORMAT(
					"[CGUIShaderManager::DeallocateResource]: invalid reference count [%d] for resource: <%s:%s:%s>", 
					pRes->GetRefCount(),
					pRes->GetName().c_str(), 
					pRes->GetResourceType().c_str(),
					pRes->GetSceneName().c_str()));
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIShaderManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUIShaderManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
