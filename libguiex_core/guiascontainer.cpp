/** 
* @file guiascontainer.cpp
* @brief widget action sequence
* @author ken
* @date 2011-04-22
*/
//============================================================================//
// include
//============================================================================// 
#include "guiascontainer.h"
#include "guiwidget.h"
#include "guisystem.h"
#include "guimath.h"
#include "guiasmanager.h"

#define GUIAS_MIN_TOTALTIME (0.01f)

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsContainer
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsContainer::CGUIAsContainer(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAs("CGUIAsContainer", rAsName, rSceneName)
	{
	}
	//------------------------------------------------------------------------------
	CGUIAsContainer::~CGUIAsContainer( )
	{
		for( TAsList::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			(*itor).m_pAs->RefRelease();
		}
		m_vAsList.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::Reset( )
	{
		CGUIAs::Reset();

		for( TAsList::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			(*itor).m_pAs->Reset();
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsContainer::ProcessProperty( const CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsContainer">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="10" />
		*		<property name="elapsed_time" type="real" value="5" />
		*
		*		<property name="item_info" type="CGUIAsContainItemInfo"/>
		*			<property name="begin_time" type="real" value="5"/>
		*			<property name="item" type="CGUIAs" value="temp_as"/>
		*		</property>
		*		
		*		<property name="item_info" type="CGUIAsContainItemInfo"/>
		*			<property name="begin_time" type="real" value="5"/>
		*			<property name="item" type="CGUIAsDefine" value="CGUIAsWidgetColor""/>
		*				<property name="loop" type="bool" value="false"/>
		*				<property name="total_time" type="real" value="10" />
		*				<property name="elapsed_time" type="real" value="5" />
		*				<property name="interpolation" type="EInterpolationType" value="0,0,0" />
		*				<property name="begin_value" type="CGUIVector3" value="0,0,0" />
		*				<property name="end_value" type="CGUIVector3" value="0,0,0" />
		*		</property>
		*</property>
		*/
		int32 ret = CGUIAs::ProcessProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		for( uint32 i=0; i<rProperty.GetPropertyNum(); ++i )
		{
			const CGUIProperty* pProperty = rProperty.GetProperty( i );
			if( pProperty->GetType() == ePropertyType_AsContainerItemInfo )
			{
				CGUIAsContainItemInfo aItemInfo;

				//begin time
				{
					real fBeginTime = 0.0f;
					const CGUIProperty* pPptBeginTime = pProperty->GetProperty("begin_time", "real");
					if( pPptBeginTime )
					{
						PropertyToValue( *pPptBeginTime, fBeginTime );
					}
					aItemInfo.m_fBeginTime = fBeginTime;
				}

				//item
				{
					const CGUIProperty* pPptAs = pProperty->GetProperty("item", "CGUIAsDefine");
					if( !pPptAs )
					{
						throw CGUIException(
							"[CGUIAsContainer::ProcessProperty]: invalid property: <%s> <%s>", 
							pProperty->GetName().c_str(), 
							pProperty->GetTypeAsString().c_str());
						return -1;
					}

					if( pPptAs->GetType() == ePropertyType_As )
					{
						//allocate as by name
						CGUIAs* pAs = CGUIAsManager::Instance()->AllocateResource( pPptAs->GetValue() );
						if( !pAs )
						{
							throw CGUIException(
								"[CGUIAsContainer::ProcessProperty]: invalid property: <%s> <%s>", 
								pProperty->GetName().c_str(), 
								pProperty->GetTypeAsString().c_str());
							return -1;
						}
						aItemInfo.m_pAs = pAs;
					}
					else if( pPptAs->GetType() == ePropertyType_AsDefine )
					{
						//allocate as by type
						CGUIAs* pAs = CGUIAsManager::Instance()->AllocateResource( pPptAs->GetValue(), "", "" );
						if( !pAs )
						{
							throw CGUIException(
								"[CGUIAsContainer::ProcessProperty]: invalid property: <%s> <%s>", 
								pProperty->GetName().c_str(), 
								pProperty->GetTypeAsString().c_str());
							return -1;
						}
						int32 ret = pAs->ProcessProperty( *pPptAs );
						if( 0 != ret )
						{
							throw CGUIException(
								"[CGUIAsContainer::ProcessProperty]: invalid property: <%s> <%s>", 
								pProperty->GetName().c_str(), 
								pProperty->GetTypeAsString().c_str());
							return ret;
						}
						aItemInfo.m_pAs = pAs;
					}
					else
					{
						throw CGUIException(
							"[CGUIAsContainer::ProcessProperty]: invalid property: <%s> <%s> <%s>", 
							pProperty->GetName().c_str(), 
							pProperty->GetTypeAsString().c_str(),
							pProperty->GetValue().c_str());
						return -1;
					}
				}
				AddItem( aItemInfo );
				aItemInfo.m_pAs->RefRelease();
			}
			else
			{
				continue;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsContainer::GenerateProperty( CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsContainer">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="10" />
		*		<property name="elapsed_time" type="real" value="5" />
		*
		*		<property name="item_info" type="CGUIAsContainerItemInfo"/>
		*			<property name="begin_time" type="real" value="5"/>
		*			<property name="item" type="CGUIAs" value="temp_as"/>
		*		</property>
		*		
		*		<property name="item_info" type="CGUIAsContainerItemInfo">
		*			<property name="begin_time" type="real" value="5"/>
		*			<property name="item" type="CGUIAsDefine" value="CGUIAsColorDefine">
		*				<property name="loop" type="bool" value="false"/>
		*				<property name="total_time" type="real" value="10" />
		*				<property name="elapsed_time" type="real" value="5" />
		*				<property name="interpolation" type="EInterpolationType" value="eInterpolationType_EaseInOut" />
		*				<property name="begin_value" type="CGUIVector3" value="0,0,0" />
		*				<property name="end_value" type="CGUIVector3" value="0,0,0" />
		*		</property>
		*</property>
		*/
		int32 ret = CGUIAs::GenerateProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		for( TAsList::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			const CGUIAsContainItemInfo& rItemInfo = *itor;

			//item info
			CGUIProperty aItemInfoProperty( "item_info", "CGUIAsContainItemInfo" );
			rProperty.AddProperty( aItemInfoProperty );

			//begin time
			{
				CGUIProperty aProperty( "begin_time", "real" );
				ValueToProperty( rItemInfo.m_fBeginTime, aProperty );
				aItemInfoProperty.AddProperty( aProperty );
			}

			//item
			{
				if( rItemInfo.m_pAs->GetName() != "" )
				{
					//named as
					CGUIProperty aProperty( "item", "CGUIAs", rItemInfo.m_pAs->GetName() );
					aItemInfoProperty.AddProperty( aProperty );
				}
				else
				{
					CGUIProperty aProperty( "item", "CGUIAsDefine", rItemInfo.m_pAs->GetType() );
					if( 0 != rItemInfo.m_pAs->GenerateProperty( aProperty ) )
					{
						return -1;
					}
					aItemInfoProperty.AddProperty( aProperty );
				}
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::SetReceiver(CGUIWidget* pReceiver)
	{
		CGUIAs::SetReceiver( pReceiver );

		for( TAsList::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			CGUIAsContainItemInfo& rInfo = *itor;
			rInfo.m_pAs->SetReceiver( GetReceiver() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::OnUpdate( )
	{
		CGUIAs::OnUpdate( );

		for( TAsList::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			const CGUIAsContainItemInfo& rInfo = *itor;
			if( rInfo.m_fBeginTime > GetElapsedTime() )
			{
				//update done
				break;
			}

			if( rInfo.m_fBeginTime + rInfo.m_pAs->GetTotalTime() < GetElapsedTime() )
			{
				continue;
			}

			rInfo.m_pAs->SetElapsedTime( GetElapsedTime() - rInfo.m_fBeginTime );
			rInfo.m_pAs->Update( 0.0f );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::AddItem( CGUIAs* pAs, real fBeginTime )
	{
		CGUIAsContainItemInfo aInfo;
		aInfo.m_pAs = pAs;
		aInfo.m_fBeginTime = fBeginTime;
		AddItem( aInfo );
	}
	//------------------------------------------------------------------------------
	void CGUIAsContainer::AddItem( CGUIAsContainItemInfo& rItemInfo )
	{
		rItemInfo.m_pAs->SetReceiver( GetReceiver() );

		//insert
		bool bInserted = false;
		for( TAsList::iterator itor = m_vAsList.begin();
			itor != m_vAsList.end();
			++itor )
		{
			const CGUIAsContainItemInfo& rInfo = *itor;
			if( rInfo.m_fBeginTime >= rItemInfo.m_fBeginTime )
			{
				m_vAsList.insert( itor, rItemInfo );
				bInserted = true;
				break;
			}
		}
		if( !bInserted )
		{
			//last one
			m_vAsList.push_back( rItemInfo );
		}

		//retain
		rItemInfo.m_pAs->RefRetain();

		//update time
		if( rItemInfo.m_pAs->GetTotalTime() + rItemInfo.m_fBeginTime > GetTotalTime() )
		{
			SetTotalTime( rItemInfo.m_pAs->GetTotalTime() + rItemInfo.m_fBeginTime );
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex


