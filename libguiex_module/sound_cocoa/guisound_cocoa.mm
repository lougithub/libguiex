/** 
 * @file guisound_cocoa.cpp
 * @brief sound module, use openal
 * @author ken
 * @date 2006-08-31
 */

//============================================================================//
// include
//============================================================================// 
#include "guisound_cocoa.h"
#include "guisounddata_cocoa.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guisystem.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUISound_cocoa);
	//------------------------------------------------------------------------------
	const char* IGUISound_cocoa::StaticGetModuleName()
	{
		return "IGUISound_cocoa";
	}
	//------------------------------------------------------------------------------
	IGUISound_cocoa::IGUISound_cocoa()
	:IGUIInterfaceSound( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUISound_cocoa::~IGUISound_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int IGUISound_cocoa::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	CGUISoundData* IGUISound_cocoa::CreateSoundData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nSoundID )
	{
		if( m_mapSoundData.find( nSoundID ) != m_mapSoundData.end() )
		{
			throw CGUIException("[IGUISound_cocoa::CreateSoundData]: the sound <%d> has existing", nSoundID );
			return NULL;
		}
		CGUISoundData_cocoa* pSoundData = new CGUISoundData_cocoa( rName, rSceneName, rPath, nSoundID );
		m_mapSoundData.insert( std::make_pair( nSoundID, pSoundData ) );
		return pSoundData;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::DestroySoundData( CGUISoundData* pData )
	{
		GUI_ASSERT( pData, "invalid parameter" );
		for( TMapSoundData::iterator itor = m_mapSoundData.begin();
			itor != m_mapSoundData.end();
			++itor )
		{
			if( itor->second == pData )
			{
				delete pData;
				m_mapSoundData.erase( itor );
				return;
			}
		}
		throw CGUIException(
			"[IGUISound_cocoa::DestroySoundData]: the sound <%s:%s> doesn't existing", 
			pData->GetName().c_str(),
			pData->GetSceneName().c_str());
		return;
	}
	//------------------------------------------------------------------------------
	int32 IGUISound_cocoa::LoadEffect( int32 /*nID*/,  const CGUIString& /*rFileName*/)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUISound_cocoa::UnloadEffect( int32 /*nID*/ )
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUISound_cocoa::PlayEffect( int32 /*nID*/ )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	
}//namespace guiex
