/** 
* @file guias.cpp
* @brief widget action sequence
* @author ken
* @date 2007-11-21
*/

//============================================================================//
// include
//============================================================================// 
#include "guias.h"
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
	//	CGUIAs
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAs::CGUIAs( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIResource( rAsName, rSceneName, "AS", GSystem->GetAsManager() )
		,m_fTotalTime(GUIAS_MIN_TOTALTIME)
		,m_fElapsedTime(0.0f)
		,m_bLooping(false)
		,m_bRetired(false)
		,m_pReceiver(NULL)
		,m_strAsType(rAsType)
	{
		Load();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destructor
	*/
	CGUIAs::~CGUIAs()
	{
		for(TListSuccessor::iterator itor = m_listSuccessor.begin();
			itor != m_listSuccessor.end();
			++itor)
		{
			( *itor )->RefRelease();
		}
		m_listSuccessor.clear();
	}
	//------------------------------------------------------------------------------
	int32 CGUIAs::ProcessProperty( const CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsColor">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="10" />
		*		<property name="elapsed_time" type="real" value="5" />
		*</property>
		*/
		{
			bool bLoop = false;
			const CGUIProperty* pPptLoop = rProperty.GetProperty("loop", "bool");
			if( pPptLoop )
			{
				PropertyToValue( *pPptLoop, bLoop );
			}
			SetLooping( bLoop );
		}

		{
			real fTotalTime = GUIAS_MIN_TOTALTIME;
			const CGUIProperty* pPptTotalTime = rProperty.GetProperty("total_time", "real");
			if( pPptTotalTime )
			{
				PropertyToValue( *pPptTotalTime, fTotalTime );
			}
			SetTotalTime( fTotalTime );
		}

		{
			real fElapsedTime = 0.0;
			const CGUIProperty* pPPtElapsedTime = rProperty.GetProperty("elapsed_time", "real");
			if( pPPtElapsedTime )
			{
				PropertyToValue( *pPPtElapsedTime, fElapsedTime );
			}
			SetElapsedTime( fElapsedTime );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAs::GenerateProperty( CGUIProperty& rProperty )
	{
		/*
		*<property name="asname" type="CGUIAsColor">
		*		<property name="loop" type="bool" value="false"/>
		*		<property name="total_time" type="real" value="10" />
		*		<property name="elapsed_time" type="real" value="5" />
		*</property>
		*/
		{
			CGUIProperty aProperty("loop", "bool");
			ValueToProperty( IsLooping(), aProperty );
			rProperty.AddProperty( aProperty );
		}


		{
			CGUIProperty aProperty( "total_time", "real" );
			ValueToProperty( GetTotalTime(), aProperty );
			rProperty.AddProperty( aProperty );
		}

		{
			CGUIProperty aProperty( "elapsed_time", "real" );
			ValueToProperty( GetElapsedTime(), aProperty );
			rProperty.AddProperty( aProperty );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get as type
	*/
	const CGUIString& CGUIAs::GetType() const
	{
		return m_strAsType;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set the flag after this as has been processed.
	*/
	void CGUIAs::Retire( bool bRetired )
	{
		m_bRetired = bRetired;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief whether the as is retired.
	*/
	bool CGUIAs::IsRetired()
	{
		return m_bRetired;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetLooping( bool bLooping )
	{
		m_bLooping = bLooping;
	}
	//------------------------------------------------------------------------------
	bool CGUIAs::IsLooping()
	{
		return m_bLooping;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set widget which receives this as
	*/
	void CGUIAs::SetReceiver(CGUIWidget* pReceiver)
	{
		m_pReceiver = pReceiver;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Get widget which receives this as
	*/
	CGUIWidget*	CGUIAs::GetReceiver()
	{
		return m_pReceiver;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set total time
	*/
	void CGUIAs::SetTotalTime(real fTotalTime)
	{
		if( fTotalTime <= 0.0f )
		{
			fTotalTime = GUIAS_MIN_TOTALTIME;
		}
		m_fTotalTime = fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAs::GetTotalTime( ) const
	{
		return m_fTotalTime;
	}
	//------------------------------------------------------------------------------
	real CGUIAs::GetElapsedTime( ) const
	{
		return m_fElapsedTime;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::SetElapsedTime( real fElapsedTime )
	{
		if( fElapsedTime < 0.0f )
		{
			m_fElapsedTime = 0.0f;
		}
		else
		{
			m_fElapsedTime = fElapsedTime;
		}
	}
	//------------------------------------------------------------------------------
	real CGUIAs::Update( real fDeltaTime )
	{
		m_fElapsedTime += fDeltaTime;
		if( m_fElapsedTime >= m_fTotalTime )
		{
			real fLeftTime = m_fElapsedTime - m_fTotalTime;

			if( IsLooping() )
			{
				m_fElapsedTime -= m_fTotalTime;
				while( m_fElapsedTime >= m_fTotalTime )
				{
					fLeftTime = m_fElapsedTime - m_fTotalTime;
					m_fElapsedTime -= m_fTotalTime;	
				}

				return Update( fLeftTime );
			}
			else
			{
				m_fElapsedTime = m_fTotalTime;
				Retire( true );
				return fLeftTime;
			}
		}
		return 0.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::OnDestory()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAs::PushSuccessor( CGUIAs* pAs)
	{
		GUI_ASSERT(pAs, "wrong parameter");

		pAs->RefRetain();
		m_listSuccessor.push_back(pAs);
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIAs::PopSuccessor()
	{
		if( m_listSuccessor.empty())
		{
			return NULL;
		}
		else
		{
			CGUIAs* pAs = m_listSuccessor.front();
			pAs->RefRelease();
			m_listSuccessor.pop_front();
			return pAs;
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIAs::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAs::DoUnload()
	{

	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsAlpha
	//*****************************************************************************
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	CGUIAsAlpha::CGUIAsAlpha( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsInterpolation<real>("CGUIAsAlpha" , rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsAlpha::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsInterpolation<real>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetAlpha(GetCurrentValue());
		return fLeftTime;
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsRotation
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsRotation::CGUIAsRotation(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUIVector3>("CGUIAsRotation", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsRotation::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsInterpolation<CGUIVector3>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetRotation(GetCurrentValue());
		return fLeftTime;
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsScale
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsScale::CGUIAsScale(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUISize>("CGUIAsScale", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsScale::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsInterpolation<CGUISize>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetScale(GetCurrentValue());
		return fLeftTime;
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsPosition
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsPosition::CGUIAsPosition(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUIVector2>("CGUIAsPosition", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsPosition::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsInterpolation<CGUIVector2>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetPixelPosition(GetCurrentValue());
		return fLeftTime;
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsColor
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsColor::CGUIAsColor(const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsInterpolation<CGUIColor>("CGUIAsColor", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsColor::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAsInterpolation<CGUIColor>::Update( fDeltaTime );
		GUI_ASSERT( GetReceiver(), "no receiver");
		GetReceiver()->SetColor(GetCurrentValue());
		return fLeftTime;
	}
	//------------------------------------------------------------------------------



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
		*			<property name="item" type="CGUIAsDefine" value="CGUIAsColor""/>
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
	real CGUIAsContainer::Update( real fDeltaTime )
	{
		real fLeftTime = CGUIAs::Update( fDeltaTime );

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

		return fLeftTime;
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


