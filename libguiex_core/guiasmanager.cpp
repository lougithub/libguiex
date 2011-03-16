/** 
* @file guiasmanager.cpp
* @brief as manager.
* @author ken
* @date 2010-11-16
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiasmanager.h"
#include "guias.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIAsData::CGUIAsData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
		:CGUIResource( rName, rSceneName, "ASDATA", GSystem->GetAsManager() )
		,m_aProperty( rProperty )
	{
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUIAsData::GetAsData() const
	{
		return m_aProperty;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsData::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsData::DoUnload()
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	CGUIAsManager * CGUIAsManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIAsManager::CGUIAsManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIAsManager::CGUIAsManager]:instance has been created" ); 
		m_pSingleton = this; 

		//register as generator to map
#define REGISTER_AS( as ) m_mapAsGenerator[#as] = as::GenerateAs;
		REGISTER_AS( CGUIAsAlpha );
		REGISTER_AS( CGUIAsScale );
		REGISTER_AS( CGUIAsPosition );
		REGISTER_AS( CGUIAsRotation );
		REGISTER_AS( CGUIAsColor );
		REGISTER_AS( CGUIAsContainer );
#undef REGISTER_AS
	}
	//------------------------------------------------------------------------------
	CGUIAsManager::~CGUIAsManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIAsManager* CGUIAsManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsManager::RegisterAs( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAsData* pAsData = new CGUIAsData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResource( pAsData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIAsData* pAsData = CGUIResourceManager<CGUIAsData, CGUIAs>::GetRegisterResource( rResName );
		if( !pAsData )
		{
			throw CGUIException( 
				"[CGUIAsManager::AllocateResource]: failed to get as data by name <%s>",
				rResName.c_str());
			return NULL;
		}

		TMapAsGenerator::iterator itorFind = m_mapAsGenerator.find( pAsData->GetAsData().GetValue() );
		if( itorFind == m_mapAsGenerator.end() )
		{
			throw CGUIException(
				"[CGUIAsManager::AllocateResource] failed to find as generator <%s>",
				pAsData->GetAsData().GetValue().c_str());
			return NULL;
		}

		CGUIAs* pAs = itorFind->second( pAsData->GetAsData().GetName(), pAsData->GetSceneName() );
		if( 0 != pAs->ProcessProperty( pAsData->GetAsData() ))
		{
			throw CGUIException(
				"[CGUIAsManager::AllocateResource]: invalid property: <%s> <%s> <%s>", 
				pAsData->GetAsData().GetName().c_str(), 
				pAsData->GetAsData().GetTypeAsString().c_str(),
				pAsData->GetAsData().GetValue().c_str());
			return NULL;
		}

		pAs->RefRetain();
		AddToAllocatePool( pAs );

		return pAs;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResource( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
	{
		TMapAsGenerator::iterator itorFind = m_mapAsGenerator.find( rAsType );
		if( itorFind == m_mapAsGenerator.end() )
		{
			throw CGUIException(
				"[CGUIAsManager::AllocateResource] failed to find as generator <%s>",
				rAsType.c_str());
			return NULL;
		}

		CGUIAs* pAs = itorFind->second( rAsName, rSceneName );
		pAs->RefRetain();
		AddToAllocatePool( pAs );
		return pAs;
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );
		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			ReleaseFromAllocatePool( pRes );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
