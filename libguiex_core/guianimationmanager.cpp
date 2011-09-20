/** 
* @file guianimationmanager.cpp
* @brief image manager.
* @author ken
* @date 2009-10-27
*/

//============================================================================//
// include 
//============================================================================// 
#include "guianimationmanager.h"
#include "guianimation.h"
#include "guiproperty.h"
#include "guistringconvertor.h"
#include "guiexception.h"
#include "guipropertyconvertor.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIAnimationData::CGUIAnimationData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIProperty& rProperty )
		:CGUIResource( rName, rSceneName, "ANIMATIONDATA", GSystem->GetAnimationManager() )
	{
		/**
		<property name="mole_laugh" type="CGUIAnimationDefine">
			<property name="size" type="CGUISize" value="178,200"/>
			<property name="interval" type="real" value="0.2"/>
			<property name="loop" type="bool" value="false"/>
			<property name="images" type="array" >
				<property name="image" type="CGUIImage" value="mole_laugh_1" />
				<property name="image" type="CGUIImage" value="mole_laugh_2" />
				<property name="image" type="CGUIImage" value="mole_laugh_3" />
			</property>
		</property>
		*/
		//size
		{
			const CGUIProperty* pPropertySize = rProperty.GetProperty("size", "CGUISize");
			if( pPropertySize )
			{
				PropertyToValue( *pPropertySize, m_aSize );
			}
		}

		//looping
		m_bLoop = false;
		{
			const CGUIProperty* pPropertyLoop = rProperty.GetProperty("loop", "bool");
			if( pPropertyLoop )
			{
				PropertyToValue( *pPropertyLoop, m_bLoop );
			}
		}

		//interval
		m_fInterval = 0.033f;
		{
			const CGUIProperty* pPropertyInterval = rProperty.GetProperty("interval", "real");
			if( pPropertyInterval )
			{
				PropertyToValue( *pPropertyInterval, m_fInterval );
			}
		}

		//images
		{
			const CGUIProperty* pPropertyImages = rProperty.GetProperty("images", "array" );
			if( pPropertyImages )
			{
				for( uint32 i=0; i<pPropertyImages->GetPropertyCount(); ++i )
				{
					const CGUIProperty* pProperty = pPropertyImages->GetProperty( i );
					if( pProperty->GetType() == ePropertyType_Image && pProperty->GetName() == "image" )
					{
						m_vecImageName.push_back( pProperty->GetValue() );
					}
					else
					{
						GUI_THROW( GUI_FORMAT( 
							"[CGUIAnimationManager::AllocateResource]: failed to parse animation data <%s>",
							rProperty.GetName().c_str()));
					}
				}
			}
		}
		if( m_vecImageName.empty() )
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIAnimationManager::AllocateResource]: failed to parse animation data <%s>",
				rProperty.GetName().c_str()));
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimationData::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimationData::DoUnload()
	{
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	CGUIAnimationManager * CGUIAnimationManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIAnimationManager::CGUIAnimationManager()
		:CGUIResourceManager<CGUIAnimationData, CGUIAnimation>("Animation")
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIAnimationManager::CGUIAnimationManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIAnimationManager::~CGUIAnimationManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIAnimationManager* CGUIAnimationManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIAnimationManager::RegisterResource(const CGUIString& rSceneName, const CGUIProperty& rProperty )
	{
		CGUIAnimationData* pAnimationData = new CGUIAnimationData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResourceImp( pAnimationData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIAnimationData* pAnimationData = CGUIResourceManager<CGUIAnimationData, CGUIAnimation>::GetRegisterResource( rResName );
		if( !pAnimationData )
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIAnimationManager::AllocateResource]: failed to get as data by name <%s>",
				rResName.c_str()));
			return NULL;
		}

		CGUIAnimation* pAnimation = new CGUIAnimation(
			pAnimationData->GetName(), 
			pAnimationData->GetSceneName(),
			pAnimationData->GetImages(), 
			pAnimationData->GetInterval(), 
			pAnimationData->IsLoop(), 
			pAnimationData->GetSize());

		pAnimation->RefRetain();
		AddToAllocatePool( pAnimation );
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimationManager::DeallocateResource( CGUIResource* pRes )
	{
		GUI_ASSERT( pRes, "invalid parameter" );

		DoRefRelease( pRes );
		if( pRes->GetRefCount() == 0 )
		{
			ReleaseFromAllocatePool( pRes );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAnimationManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
	void CGUIAnimationManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		delete pRes;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
