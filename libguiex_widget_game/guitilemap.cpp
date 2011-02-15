/** 
* @file guitilemap.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
//============================================================================//
// include
//============================================================================//
#include "guitilemap.h"
#include "guitilemaplayer.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CCTMXTiledMap::CCTMXTiledMap()
		:m_pMapInfo( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CCTMXTiledMap::~CCTMXTiledMap()
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	void CCTMXTiledMap::Reset()
	{
		//clear layer
		for( uint32 i=0; i<m_arrayLayer.size(); ++i )
		{
			delete m_arrayLayer[i];
		}
		m_arrayLayer.clear();

		//clear map info
		if( m_pMapInfo )
		{
			delete m_pMapInfo;
			m_pMapInfo = NULL;
		}
	}
	//------------------------------------------------------------------------------
	int32 CCTMXTiledMap::ParseTMXFile( const CGUIString& rFileName )
	{
		Reset();

		m_pMapInfo = new CCTMXMapInfo;
		if( 0 != m_pMapInfo->InitWithTMXFile( rFileName ) )
		{
			throw CGUIException("[CCTMXTiledMap::ParseTMXFile]: failed parse tmx file %s", rFileName.c_str() );
			return -1;
		}

		for( uint32 i=0; i<m_pMapInfo->layers.size(); ++i ) 
		{
			CCTMXLayer* pLayer = new CCTMXLayer( this, i );
			m_arrayLayer.push_back( pLayer );
		}		

		return 0;
	}
	//------------------------------------------------------------------------------
	CCTMXLayer* CCTMXTiledMap::GetLayer( const CGUIString& rLayerName )
	{
		for( TLayerArray::iterator itor = m_arrayLayer.begin();
			itor != m_arrayLayer.end();
			++itor )
		{
			if( (*itor)->GetLayerName() == rLayerName )
			{
				return *itor;
			}
		}
		return NULL;
	}

	//------------------------------------------------------------------------------
	const CCTMXObjectGroup* CCTMXTiledMap::GetObjectGroup( const CGUIString& rGroupName ) const
	{
		for( std::vector<CCTMXObjectGroup>::const_iterator itor = m_pMapInfo->objectGroups.begin();
			itor != m_pMapInfo->objectGroups.end();
			++itor ) 
		{
			if( (*itor).groupName == rGroupName )
			{
				return &(*itor);
			}
		}

		// objectGroup not found
		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIString* CCTMXTiledMap::GetProperty( const CGUIString& rPropertyName ) const
	{
		std::map<CGUIString, CGUIString>::const_iterator itorFind = m_pMapInfo->properties.find( rPropertyName );
		if( itorFind == m_pMapInfo->properties.end() )
		{
			return NULL;
		}
		else
		{
			return &itorFind->second;
		}
	}
	//------------------------------------------------------------------------------
	const std::map<CGUIString, CGUIString>* CCTMXTiledMap::GetTileProperties( uint32 gid )
	{
		std::map<uint32, std::map<CGUIString, CGUIString> >::const_iterator itorFind = m_pMapInfo->tileProperties.find( gid );
		if( itorFind == m_pMapInfo->tileProperties.end() )
		{
			return NULL;
		}
		else
		{
			return &itorFind->second;
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
