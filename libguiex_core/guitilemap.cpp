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
#include "guiproperty.h"
#include "guiscenemanager.h"
#include "guiexception.h"
#include "guitilemapmanager.h"
#include "guisystem.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITileMap::CGUITileMap( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "TILEMAP", GSystem->GetTileMapManager())
		,m_pMapInfo( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUITileMap::~CGUITileMap()
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	void CGUITileMap::Reset()
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
	int32 CGUITileMap::LoadValueFromProperty( const class CGUIProperty& rProperty )
	{
		/*
		<property name="tilemap_0" type="CGUITileMapDefine" >
			<property name="working_dir" type="CGUIString" value="tilemap/"/>
			<property name="config_file" type="CGUIString" value="tilemap_0.tmx"/>
		</property>
		*/
		const CGUIProperty* pPpt_WorkingDir = rProperty.GetPropertyChecked("working_dir", "CGUIString");
		m_strFullWorkingDir = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + pPpt_WorkingDir->GetValue();
		const CGUIProperty* pPpt_ConfigFile = rProperty.GetPropertyChecked("config_file", "CGUIString");
		m_strConfigFile = pPpt_ConfigFile->GetValue();

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMap::DoLoad()
	{
		if( 0 != ParseTMXFile(m_strFullWorkingDir + m_strConfigFile) )
		{
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUITileMap::DoUnload()
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	int32 CGUITileMap::ParseTMXFile( const CGUIString& rFileName )
	{
		Reset();

		m_pMapInfo = new CGUITileMapInfo;
		if( 0 != m_pMapInfo->InitWithTMXFile( rFileName ) )
		{
			throw CGUIException("[CGUITileMap::ParseTMXFile]: failed parse tmx file %s", rFileName.c_str() );
			return -1;
		}

		for( uint32 i=0; i<m_pMapInfo->layers.size(); ++i ) 
		{
			CGUITileMapLayer* pLayer = new CGUITileMapLayer( this, i );
			m_arrayLayer.push_back( pLayer );
		}		

		return 0;
	}
	//------------------------------------------------------------------------------
	CGUITileMapLayer* CGUITileMap::GetLayer( const CGUIString& rLayerName )
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
	const CGUITileMapObjectGroup* CGUITileMap::GetObjectGroup( const CGUIString& rGroupName ) const
	{
		for( std::vector<CGUITileMapObjectGroup>::const_iterator itor = m_pMapInfo->objectGroups.begin();
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
	const CGUIString* CGUITileMap::GetProperty( const CGUIString& rPropertyName ) const
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
	const std::map<CGUIString, CGUIString>* CGUITileMap::GetTileProperties( uint32 gid )
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
	void CGUITileMap::Render( IGUIInterfaceRender* pRender, const CGUIMatrix4& rWorldMatrix )
	{
		for( TLayerArray::iterator itor = m_arrayLayer.begin();
			itor != m_arrayLayer.end();
			++itor )
		{
			(*itor)->Render( pRender );
		}

	}
	//------------------------------------------------------------------------------
}//namespace guiex
