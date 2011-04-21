/** 
* @file guiparticle2dmanager.cpp
* @brief particle2d manager.
* @author ken
* @date 2011-01-17
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiparticle2dmanager.h"
#include "guiinterfacemanager.h"
#include "guiinterfacesound.h"
#include "guisounddata.h"
#include "guipropertyconvertor.h"
#include "guistringconvertor.h"
#include "guiexception.h"
#include "guiproperty.h"
#include "guiparticle2dsystemquad.h"
#include "guisystem.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIParticle2DData::CGUIParticle2DData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
		:CGUIResource( rName, rSceneName, "PARTICLE2DDATA", GSystem->GetParticle2DManager() )
		,m_aProperty( rProperty )
	{
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUIParticle2DData::GetParticle2DData() const
	{
		return m_aProperty;
	}
	//------------------------------------------------------------------------------
	int32 CGUIParticle2DData::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DData::DoUnload()
	{
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	CGUIParticle2DManager * CGUIParticle2DManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIParticle2DManager::CGUIParticle2DManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIParticle2DManager::CGUIParticle2DManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DManager::~CGUIParticle2DManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DManager* CGUIParticle2DManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIParticle2DManager::RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty)
	{
		CGUIParticle2DData* pParticle2Data = new CGUIParticle2DData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResourceImp( pParticle2Data );
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystem* CGUIParticle2DManager::DoCreateParticle2D( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIParticle2DSystem* pParticle2D = new CGUIParticle2DSystemQuad( rProperty.GetName(), rSceneName );
		if( pParticle2D->LoadValueFromProperty( rProperty ) != 0 )
		{
			delete pParticle2D;
			throw CGUIException( 
				"[CGUIParticle2DManager::DoCreateParticle2D]: failed to load particle <%s> from property",
				rProperty.GetName().c_str() );
			return NULL;
		}
		return pParticle2D;
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystem* CGUIParticle2DManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIParticle2DData* pParticle2DData = CGUIResourceManager<CGUIParticle2DData, CGUIParticle2DSystem>::GetRegisterResource( rResName );
		if( !pParticle2DData )
		{
			throw CGUIException( 
				"[CGUIParticle2DManager::AllocateResource]: failed to get sound by name <%s>",
				rResName.c_str());
			return NULL;
		}

		CGUIParticle2DSystem* pParticle2D = DoCreateParticle2D( pParticle2DData->GetSceneName(), pParticle2DData->GetParticle2DData() );
		pParticle2D->RefRetain();
		AddToAllocatePool( pParticle2D );

		return pParticle2D;
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystem* CGUIParticle2DManager::AllocateResource( const CGUIString& rName, const CGUIString& rSceneName )
	{
		CGUIParticle2DSystem* pParticle2D = new CGUIParticle2DSystemQuad( rName, rSceneName );
		pParticle2D->RefRetain();
		AddToAllocatePool( pParticle2D );

		return pParticle2D;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			ReleaseFromAllocatePool( pRes );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
