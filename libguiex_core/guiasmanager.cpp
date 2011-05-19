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
#include "guiaswidget.h"
#include "guiascontainer.h"
#include "guiasgrid3d.h"
#include "guiastiledgrid3d.h"

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
		:CGUIResourceManager <CGUIAsData, CGUIAs>("As")
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIAsManager::CGUIAsManager]:instance has been created" ); 
		m_pSingleton = this; 

		//register as generator to map
		RegisterAs();
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
	int32 CGUIAsManager::RegisterResource( const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAsData* pAsData = new CGUIAsData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResourceImp( pAsData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAsManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIAsData* pAsData = CGUIResourceManager<CGUIAsData, CGUIAs>::GetRegisterResource( rResName );
		if( !pAsData )
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIAsManager::AllocateResource]: failed to get as data by name <%s>",
				rResName.c_str()));
			return NULL;
		}

		TMapAsGenerator::iterator itorFind = m_mapAsGenerator.find( pAsData->GetAsData().GetValue() );
		if( itorFind == m_mapAsGenerator.end() )
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIAsManager::AllocateResource] failed to find as generator <%s>",
				pAsData->GetAsData().GetValue().c_str()));
			return NULL;
		}

		CGUIAs* pAs = itorFind->second( pAsData->GetAsData().GetName(), pAsData->GetSceneName() );
		if( 0 != pAs->ProcessProperty( pAsData->GetAsData() ))
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIAsManager::AllocateResource]: invalid property: <%s> <%s> <%s>", 
				pAsData->GetAsData().GetName().c_str(), 
				pAsData->GetAsData().GetTypeAsString().c_str(),
				pAsData->GetAsData().GetValue().c_str()));
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
			GUI_THROW( GUI_FORMAT(
				"[CGUIAsManager::AllocateResource] failed to find as generator <%s>",
				rAsType.c_str()));
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
		CGUIAs* pAs = static_cast<CGUIAs*>( pRes );
		pAs->OnDestory();
		delete pAs;
	}
	//------------------------------------------------------------------------------
	void CGUIAsManager::RegisterAs()
	{
#define REGISTER_AS( as ) m_mapAsGenerator[#as] = as::GenerateAs;

		REGISTER_AS( CGUIAsWidgetAlpha );
		REGISTER_AS( CGUIAsWidgetScale );
		REGISTER_AS( CGUIAsWidgetPosition );
		REGISTER_AS( CGUIAsWidgetSize );
		REGISTER_AS( CGUIAsWidgetRotation );
		REGISTER_AS( CGUIAsWidgetColor );
		REGISTER_AS( CGUIAsContainer );
		REGISTER_AS( CGUIAsCallFunc );
		REGISTER_AS( CGUIAsWidgetMoveTo );
		

		//grid 3d
		REGISTER_AS( CGUIAsWaves3D );
		REGISTER_AS( CGUIAsPageTurn3D );
		REGISTER_AS( CGUIAsShaky3D );
		REGISTER_AS( CGUIAsFlipX3D );
		REGISTER_AS( CGUIAsFlipY3D );
		REGISTER_AS( CGUIAsLens3D );
		REGISTER_AS( CGUIAsRipple3D );
		REGISTER_AS( CGUIAsLiquid );
		REGISTER_AS( CGUIAsWaves );
		REGISTER_AS( CGUIAsTwirl );
		

		//tiled grid 3d
		REGISTER_AS( CGUIAsShakyTiles3D );
		REGISTER_AS( CGUIAsShatteredTiles3D );
		REGISTER_AS( CGUIAsShuffleTiles );
		REGISTER_AS( CGUIAsFadeOutTRTiles );
		REGISTER_AS( CGUIAsFadeOutBLTiles );
		REGISTER_AS( CGUIAsFadeOutUpTiles );
		REGISTER_AS( CGUIAsFadeOutDownTiles );
		REGISTER_AS( CGUIAsTurnOffTiles );
		REGISTER_AS( CGUIAsWavesTiles3D );
		REGISTER_AS( CGUIAsJumpTiles3D );
		REGISTER_AS( CGUIAsSplitRows );
		REGISTER_AS( CGUIAsSplitCols );
		
#undef REGISTER_AS
	}
	//------------------------------------------------------------------------------

}//namespace guiex
