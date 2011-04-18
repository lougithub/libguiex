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
		,m_aProperty( rProperty )
	{
	}
	//------------------------------------------------------------------------------
	const CGUIProperty& CGUIAnimationData::GetAnimationData() const
	{
		return m_aProperty;
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
	int32 CGUIAnimationManager::RegisterAnimation(
		const CGUIString& rSceneName, 
		const CGUIProperty& rProperty )
	{
		CGUIAnimationData* pAnimationData = new CGUIAnimationData( rProperty.GetName(), rSceneName, rProperty );
		RegisterResource( pAnimationData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIAnimationManager::AllocateResource( const CGUIString& rResName )
	{
		CGUIAnimationData* pAnimationData = CGUIResourceManager<CGUIAnimationData, CGUIAnimation>::GetRegisterResource( rResName );
		if( !pAnimationData )
		{
			throw CGUIException( 
				"[CGUIAnimationManager::AllocateResource]: failed to get as data by name <%s>",
				rResName.c_str());
			return NULL;
		}

		/**
		<property name="mole_laugh" type="CGUIImageDefine">
			<property name="size" type="CGUISize" value="178,200"/>
			<property name="interval" type="real" value="0.5"/>
			<property name="loop" type="bool" value="false"/>
			<property name="path" type="CGUIString" value="image/anim/mole_laugh1.tga" />
			<property name="uv" type="CGUIRect" value="0,0,1,1" />
			<property name="path" type="CGUIString" value="image/anim/mole_laugh2.tga" />
			<property name="uv" type="CGUIRect" value="0,0,1,1" />
			<property name="path" type="CGUIString" value="image/anim/mole_laugh3.tga" />
			<property name="uv" type="CGUIRect" value="0,0,1,1" />
		</property>
		*/

		const CGUIProperty& rRootProperty = pAnimationData->GetAnimationData();

		//size
		CGUISize aAnimationSize;
		{
			const CGUIProperty* pPropertySize = rRootProperty.GetProperty("size", "CGUISize");
			if( pPropertySize )
			{
				PropertyToValue( *pPropertySize, aAnimationSize );
			}
		}

		//looping
		bool bLoop = false;
		{
			const CGUIProperty* pPropertyLoop = rRootProperty.GetProperty("loop", "bool");
			if( pPropertyLoop )
			{
				PropertyToValue( *pPropertyLoop, bLoop );
			}
		}

		//interval
		real fInterval = 0.033f;
		{
			const CGUIProperty* pPropertyInterval = rRootProperty.GetProperty("interval", "real");
			if( pPropertyInterval )
			{
				PropertyToValue( *pPropertyInterval, fInterval );
			}
		}

		//images
		std::vector<CGUIString> vecFilenames;
		std::vector<CGUIRect> vecRects;
		{
			const CGUIProperty* pPropertyImages = rRootProperty.GetProperty("images", "folder" );
			if( pPropertyImages )
			{
				for( uint32 i=0; i<pPropertyImages->GetPropertyNum(); ++i )
				{
					const CGUIProperty* pProperty = pPropertyImages->GetProperty( i );
					if( pProperty->GetType() == ePropertyType_String && pProperty->GetName() == "path" )
					{
						vecFilenames.push_back( pProperty->GetValue() );
					}
					else if( pProperty->GetType() == ePropertyType_Rect && pProperty->GetName() == "uv" )
					{
						CGUIRect aRect;
						PropertyToValue( *pProperty, aRect );
						vecRects.push_back( aRect );
					}
					else
					{
						throw CGUIException( 
							"[CGUIAnimationManager::AllocateResource]: failed to parse animation data <%s>",
							rRootProperty.GetName().c_str());
					}
				}
			}
		}

		CGUIAnimation* pAnimation = new CGUIAnimation( rRootProperty.GetName(), pAnimationData->GetSceneName(), vecFilenames, vecRects, fInterval, bLoop, aAnimationSize );
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
