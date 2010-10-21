/** 
* @file guiimage.cpp
* @brief image used in widgets
* @author ken
* @date 2006-07-17
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guibase.h>
#include <libguiex_core\guiimage.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guitexture.h>
#include <libguiex_core\guitexturemanager.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guirect.h>
#include <libguiex_core\guiperfmonitor.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiprojectinfo.h>
#include <libguiex_core\guiprojectinfomanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIImage::CGUIImage( 
		const CGUIString& rName, 
		const CGUIString& rProjectName, 
		const CGUIColor& rColor )
		:CGUIResource( rName, rProjectName, "IMAGE" )
		,m_pTexture(NULL)
		,m_eImageOperation(IMAGE_NONE)
		,m_eImageType( eIT_COLOR )
		,m_aColor( rColor )
		,m_aUVRect(CGUIRect(0.f,0.f,1.f,1.f))
	{
	}
	//------------------------------------------------------------------------------

	CGUIImage::CGUIImage( 
		const CGUIString& rName,
		const CGUIString& rProjectName, 
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat /*= GUI_PF_ARGB_32*/ )
		:CGUIResource( rName, rProjectName, "IMAGE" )
		,m_pTexture(NULL)
		,m_eImageOperation(IMAGE_NONE)
		,m_eImageType( eIT_MEM )
		,m_pBuffPtr(buffPtr)
		,m_nBuffWidth(buffWidth)
		,m_nBuffHeight(buffHeight)
		,m_ePixelFormat(ePixelFormat)
		,m_aUVRect(CGUIRect(0.f,0.f,1.f,1.f))
	{
	}
	//------------------------------------------------------------------------------

	CGUIImage::CGUIImage( 		
		const CGUIString& rName,
		const CGUIString& rProjectName, 
		const CGUIString& rPath,
		const CGUIRect& rUVRect,
		EImageOperation	eImageOperation)
		:CGUIResource( rName, rProjectName, "IMAGE" )
		,m_pTexture(NULL)
		,m_eImageOperation(eImageOperation)
		,m_eImageType( eIT_FILE )
		,m_strPath( rPath )
		,m_aUVRect(rUVRect)
	{
	}
	//------------------------------------------------------------------------------
	CGUIImage::~CGUIImage()	
	{
		Unload();
	}
	//------------------------------------------------------------------------------
	int32	CGUIImage::DoLoad()
	{
		GUI_ASSERT( m_pTexture==NULL, "the texture has existed");

		if( m_eImageType == eIT_FILE )
		{
			//get full path
			CGUIString strFullPath = CGUIProjectInfoManager::Instance()->GetProjectFilePath( m_strProjectName ) + m_strPath;
			m_pTexture = CGUITextureManager::Instance()->CreateTexture(strFullPath);

			if( !m_pTexture )
			{
				throw CGUIException("[CGUIImage::DoLoad]: failed to create texture from path <%s>",strFullPath.c_str());
				return -1;
			}
		}
		else if( m_eImageType == eIT_COLOR )
		{
			m_pTexture = CGUITextureManager::Instance()->CreateDefaultTexture();
			if( !m_pTexture )
			{
				throw CGUIException("[CGUIImage::DoLoad]: failed to create texture from color");
				return -1;
			}
		}
		else if( m_eImageType == eIT_MEM )
		{
			m_pTexture = CGUITextureManager::Instance()->CreateTexture(m_pBuffPtr, m_nBuffWidth, m_nBuffHeight, m_ePixelFormat);
			if( !m_pTexture )
			{
				throw CGUIException("[CGUIImage::DoLoad]: failed to create texture from memory");
				return -1;
			}
		}
		else
		{
			throw CGUIException("[CGUIImage::DoLoad]: unknown image data type");
			return -1;
		}

		m_aUVRect.SetRect( m_aUVRect );
		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::DoUnload()
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture(m_pTexture);
			m_pTexture = NULL;
		}
	}
	//------------------------------------------------------------------------------
	CGUISize CGUIImage::GetSize()
	{
		if( !IsLoaded())
		{
			Load();
		}

		CGUISize aTexSize = m_aUVRect.GetSize();
		switch(m_eImageOperation)
		{
		case IMAGE_ROTATE90CW:		///!< rotate image for 90 CW
		case IMAGE_ROTATE90CCW:		///!< rotate image for 90 CCW
			return CGUISize( m_pTexture->GetHeight() * aTexSize.m_fHeight, m_pTexture->GetWidth() * aTexSize.m_fWidth);

		case IMAGE_NONE:			///!< none operation
		case IMAGE_FLIPHORIZON:		///!< flip image horizon
		case IMAGE_FLIPVERTICAL:	///!< flip image vertical
		default:
			return CGUISize( m_pTexture->GetWidth() * aTexSize.m_fWidth, m_pTexture->GetHeight() * aTexSize.m_fHeight);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw(IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColorRect& rColorRect,
		real fAlpha	)
	{
		if( !IsLoaded())
		{
			Load();
		}

		CGUIColorRect aColorRect = rColorRect;
		aColorRect.m_top_left.SetAlpha(aColorRect.m_top_left.GetAlpha()*fAlpha);
		aColorRect.m_top_right.SetAlpha(aColorRect.m_top_right.GetAlpha()*fAlpha);
		aColorRect.m_bottom_left.SetAlpha(aColorRect.m_bottom_left.GetAlpha()*fAlpha);
		aColorRect.m_bottom_right.SetAlpha(aColorRect.m_bottom_right.GetAlpha()*fAlpha);
		pRender->AddRenderTexture( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOperation,
			aColorRect.m_top_left.GetARGB(),aColorRect.m_top_right.GetARGB(),
			aColorRect.m_bottom_left.GetARGB(),aColorRect.m_bottom_right.GetARGB());
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw(
		IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		real fAlpha)
	{
		if( !IsLoaded())
		{
			Load();
		}

		if( m_eImageType == eIT_COLOR )
		{
			CGUIColor aColor = m_aColor;
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->AddRenderTexture( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOperation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
		else
		{
			CGUIColor aColor(0xFFFFFFFF);
			aColor.SetAlpha(fAlpha);
			pRender->AddRenderTexture( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOperation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw(IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColor& rColor,
		real fAlpha)
	{
		if( !IsLoaded())
		{
			Load();
		}

		if( m_eImageType == eIT_COLOR )
		{
			CGUIColor aColor = m_aColor*rColor;
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->AddRenderTexture( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOperation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
		else
		{
			CGUIColor aColor(rColor);
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->AddRenderTexture( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOperation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
	}
	//------------------------------------------------------------------------------


}//namespace guiex




