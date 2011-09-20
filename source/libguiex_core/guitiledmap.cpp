/** 
* @file guitiledmap.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-27
*/
//============================================================================//
// include
//============================================================================//
#include "guitiledmap.h"
#include "guitiledmaplayer.h"
#include "guiproperty.h"
#include "guiscenemanager.h"
#include "guiexception.h"
#include "guitiledmapmanager.h"
#include "guisystem.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITiledMap::CGUITiledMap( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "TILEDMAP", GSystem->GetTiledMapManager())
		,m_pMapInfo( NULL )
	{

	}
	//------------------------------------------------------------------------------
	CGUITiledMap::~CGUITiledMap()
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	void CGUITiledMap::Reset()
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
	int32 CGUITiledMap::LoadValueFromProperty( const class CGUIProperty& rProperty )
	{
		/*
		<property name="tiledmap_0" type="CGUITiledMapDefine" >
			<property name="working_dir" type="CGUIString" value="tiledmap/"/>
			<property name="config_file" type="CGUIString" value="tiledmap_0.tmx"/>
		</property>
		*/
		const CGUIProperty* pPpt_WorkingDir = rProperty.GetPropertyChecked("working_dir", "CGUIString");
		m_strFullWorkingDir = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + pPpt_WorkingDir->GetValue();
		const CGUIProperty* pPpt_ConfigFile = rProperty.GetPropertyChecked("config_file", "CGUIString");
		m_strConfigFile = pPpt_ConfigFile->GetValue();

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMap::DoLoad()
	{
		if( 0 != ParseTMXFile(m_strFullWorkingDir + m_strConfigFile) )
		{
			return -1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUITiledMap::DoUnload()
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	int32 CGUITiledMap::ParseTMXFile( const CGUIString& rFileName )
	{
		Reset();

		m_pMapInfo = new CGUITiledMapInfo;
		if( 0 != m_pMapInfo->InitWithTMXFile( rFileName ) )
		{
			GUI_THROW( GUI_FORMAT("[CGUITiledMap::ParseTMXFile]: failed parse tmx file %s", rFileName.c_str() ));
			return -1;
		}

		for( uint32 i=0; i<m_pMapInfo->GetLayerInfos().size(); ++i ) 
		{
			CGUITiledMapLayer* pLayer = new CGUITiledMapLayer( this, i );
			m_arrayLayer.push_back( pLayer );
		}		

		return 0;
	}
	//------------------------------------------------------------------------------
	CGUITiledMapLayer* CGUITiledMap::GetLayer( const CGUIString& rLayerName )
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
	const CGUITiledMapObjectGroup* CGUITiledMap::GetObjectGroup( const CGUIString& rGroupName ) const
	{
		for( std::vector<CGUITiledMapObjectGroup>::const_iterator itor = m_pMapInfo->GetObjectGroups().begin();
			itor != m_pMapInfo->GetObjectGroups().end();
			++itor ) 
		{
			if( (*itor).GetName() == rGroupName )
			{
				return &(*itor);
			}
		}

		// objectGroup not found
		return NULL;
	}
	//------------------------------------------------------------------------------
	const CGUIString* CGUITiledMap::GetProperty( const CGUIString& rPropertyName ) const
	{
		std::map<CGUIString, CGUIString>::const_iterator itorFind = m_pMapInfo->GetProperties().find( rPropertyName );
		if( itorFind == m_pMapInfo->GetProperties().end() )
		{
			return NULL;
		}
		else
		{
			return &itorFind->second;
		}
	}
	//------------------------------------------------------------------------------
	const std::map<CGUIString, CGUIString>* CGUITiledMap::GetTileProperties( uint32 gid )
	{
		std::map<uint32, std::map<CGUIString, CGUIString> >::const_iterator itorFind = m_pMapInfo->GetAllTileProperties().find( gid );
		if( itorFind == m_pMapInfo->GetAllTileProperties().end() )
		{
			return NULL;
		}
		else
		{
			return &itorFind->second;
		}
	}
	//------------------------------------------------------------------------------
	void CGUITiledMap::Render( IGUIInterfaceRender* pRender )
	{
		for( TLayerArray::iterator itor = m_arrayLayer.begin();
			itor != m_arrayLayer.end();
			++itor )
		{
			(*itor)->Render( pRender );
		}

	}
	//------------------------------------------------------------------------------
	CGUITiledMapInfo* CGUITiledMap::GetMapInfo()
	{
		return m_pMapInfo;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUITiledMap::GetWorkingDir() const
	{
		return m_strFullWorkingDir;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
