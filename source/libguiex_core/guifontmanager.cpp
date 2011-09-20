/** 
* @file guifontmanager.cpp
* @brief font manager.
* @author ken
* @date 2009-10-27
*/

//============================================================================//
// include 
//============================================================================// 
#include "guifontmanager.h"
#include "guiinterfacemanager.h"
#include "guiinterfacefont.h"
#include "guifontdata.h"
#include "guipropertyconvertor.h"
#include "guistringconvertor.h"
#include "guiexception.h"
#include "guiproperty.h"


//============================================================================//
// define
//============================================================================// 
#define GUI_FONT_TEXTURE_SIZE	512

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIFontManager * CGUIFontManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIFontManager::CGUIFontManager()
		:CGUIResourceManager<CGUIFontData, CGUIFontData>("Font")
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIFontManager::CGUIFontManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIFontManager::~CGUIFontManager()
	{
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIFontManager* CGUIFontManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32 CGUIFontManager::RegisterResource( 
		const CGUIString& rSceneName,
		const CGUIProperty& rProperty)
	{
		/*
		<property name="font_ltypeb" type="CGUIFontDefine" >
			<property name="path" type="CGUIString" value="font/LTYPEB.TTF"/>
			<property name="id" type="uint16" value="1"/>
			<property name="size" type="uint16" value="12"/>
			<property name="desc" type="CGUIString" value="desc"/>
			<property name="texture_size" type="CGUIIntSize" value="512,512"/>
		</property>
		*/

		SFontInfo aFontInfo;

		//path
		{
			const CGUIProperty* pPropPath = rProperty.GetProperty("path", "CGUIString");
			if( !pPropPath )
			{
				GUI_THROW( GUI_FORMAT(
					"[CGUIFontManager::RegisterResource], invalid property <%s:%s:%s>!", 
					rProperty.GetName().c_str(),
					rProperty.GetTypeAsString().c_str(),
					rProperty.GetValue().c_str()));
				return -1;
			}
			aFontInfo.m_strPath = pPropPath->GetValue();
		}

		//font id
		{
			const CGUIProperty* pPropId = rProperty.GetProperty("id", "uint16");
			if( !pPropId )
			{
				GUI_THROW( GUI_FORMAT(
					"[CGUIFontManager::RegisterResource], invalid property <%s:%s:%s>!", 
					rProperty.GetName().c_str(),
					rProperty.GetTypeAsString().c_str(),
					rProperty.GetValue().c_str()));
				return -1;
			}
			PropertyToValue( *pPropId, aFontInfo.m_uID);
		}

		//size
		{
			const CGUIProperty* pPropSize = rProperty.GetProperty("size", "uint16");
			if( !pPropSize )
			{
				GUI_THROW( GUI_FORMAT(
					"[CGUIFontManager::RegisterResource], invalid property <%s:%s:%s>!", 
					rProperty.GetName().c_str(),
					rProperty.GetTypeAsString().c_str(),
					rProperty.GetValue().c_str()));
				return -1;
			}
			PropertyToValue( *pPropSize, aFontInfo.m_uSize);
		}

		//desc
		{
			const CGUIProperty* pPropDesc = rProperty.GetProperty("desc", "CGUIString");
			if( pPropDesc )
			{
				aFontInfo.m_strDesc = pPropDesc->GetValue();
			}
		}

		//texture size
		{
			const CGUIProperty* pPropTextureSize = rProperty.GetProperty("texture_size", "CGUIIntSize");
			if( pPropTextureSize )
			{
				PropertyToValue( *pPropTextureSize, aFontInfo.m_aTextureSize);
			}
			else
			{
				aFontInfo.m_aTextureSize.SetValue( GUI_FONT_TEXTURE_SIZE, GUI_FONT_TEXTURE_SIZE);
			}
		}

		CGUIFontData* pFontData = DoCreateFont( rProperty.GetName(), rSceneName, aFontInfo );
		RegisterResourceImp( pFontData );
		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIFontData* CGUIFontManager::DoCreateFont(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const SFontInfo& rFontInfo)
	{
		if( rFontInfo.m_uID >= GUI_FONT_MAX_NUM )
		{
			GUI_THROW( GUI_FORMAT("[CGUIFontManager::CreateGUIFont]: nFontID should be smaller than %d", GUI_FONT_MAX_NUM ));
			return NULL;
		}

		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			GUI_THROW( "[CGUIFontManager::CreateGUIFont]: failed to get font interface");
			return NULL;
		}
		CGUIFontData* pFontData = pFont->CreateFontData( rName, rSceneName, rFontInfo );
		return pFontData;
	}
	//------------------------------------------------------------------------------
	void CGUIFontManager::DestroyRegisterResourceImp( CGUIResource* pRes )
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			GUI_THROW( "[CGUIFontManager::DestroyResourceImp]: failed to get font interface");
		}
		pFont->DestroyFontData( (CGUIFontData*)pRes );
	}
	//------------------------------------------------------------------------------
	void CGUIFontManager::DestroyAllocateResourceImp( CGUIResource* pRes )
	{
		IGUIInterfaceFont* pFont = CGUIInterfaceManager::Instance()->GetInterfaceFont();
		if( !pFont )
		{
			GUI_THROW( "[CGUIFontManager::DestroyResourceImp]: failed to get font interface");
		}
		pFont->DestroyFontData( (CGUIFontData*)pRes );
	}
	//------------------------------------------------------------------------------
	void CGUIFontManager::DeallocateResource( CGUIResource* /*pRes*/ )
	{
		GUI_THROW( "[CGUIFontManager::DeallocateResource]: shouldn't call here");
	}	
	//------------------------------------------------------------------------------
}//namespace guiex
