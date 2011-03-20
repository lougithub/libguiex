/** 
* @file guiimage.cpp
* @brief image used in widgets
* @author ken
* @date 2006-07-17
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiimage.h"
#include "guiinterfacemanager.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guitexturemanager.h"
#include "guiexception.h"
#include "guirect.h"
#include "guiperfmonitor.h"
#include "guisystem.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guirenderrect.h"
#include "guiimagemanager.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIImage::CGUIImage( 
		const CGUIString& rName, 
		const CGUIString& rSceneName, 
		const CGUIColor& rColor,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "IMAGE", GSystem->GetImageManager() )
		,m_pTexture(NULL)
		,m_aUVRect(CGUIRect(0.f,0.f,1.f,1.f))
		,m_eImageOrientation(eImageOrientation_Normal)
		,m_eImageType( eIT_COLOR )
		,m_strPath()	
		,m_aColor( rColor )
		,m_aImageSize( rSize )
	{
	}
	//------------------------------------------------------------------------------
	CGUIImage::CGUIImage( 
		const CGUIString& rName,
		const CGUIString& rSceneName, 
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "IMAGE", GSystem->GetImageManager() )
		,m_pTexture(NULL)
		,m_aUVRect(CGUIRect(0.f,0.f,1.f,1.f))
		,m_eImageOrientation(eImageOrientation_Normal)
		,m_eImageType( eIT_MEM )
		,m_strPath()
		,m_aColor()
		,m_pBuffPtr(buffPtr)
		,m_nBuffWidth(buffWidth)
		,m_nBuffHeight(buffHeight)
		,m_ePixelFormat(ePixelFormat)
		,m_aImageSize( rSize )
	{
	}
	//------------------------------------------------------------------------------
	CGUIImage::CGUIImage( 		
		const CGUIString& rName,
		const CGUIString& rSceneName, 
		const CGUIString& rPath,
		const CGUIRect& rUVRect,
		EImageOrientation	eImageOrientation,
		const CGUISize& rSize )
		:CGUIResource( rName, rSceneName, "IMAGE", GSystem->GetImageManager() )
		,m_pTexture(NULL)
		,m_aUVRect( rUVRect )
		,m_eImageOrientation(eImageOrientation)
		,m_eImageType( eIT_FILE )
		,m_strPath( rPath )
		,m_aColor()
		,m_aImageSize( rSize )
	{
	}
	//------------------------------------------------------------------------------
	CGUIImage::~CGUIImage()	
	{
	}
	//------------------------------------------------------------------------------
	int32	CGUIImage::DoLoad()
	{
		GUI_ASSERT( m_pTexture==NULL, "the texture has existed");

		if( m_eImageType == eIT_FILE )
		{
			//get full path
			CGUIString strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;
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

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIImage::DoUnload()
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture(m_pTexture);
			m_pTexture = NULL;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get image size
	*/
	const CGUISize& CGUIImage::GetSize()
	{
		if( m_aImageSize.IsEqualZero())
		{
			Load();

			CGUISize aTexSize = m_aUVRect.GetSize();
			switch(m_eImageOrientation)
			{
			case eImageOrientation_90CW:		///!< rotate image for 90 CW
			case eImageOrientation_90CCW:		///!< rotate image for 90 CCW
				m_aImageSize.SetValue( 
					m_pTexture->GetHeight() * aTexSize.m_fHeight, 
					m_pTexture->GetWidth() * aTexSize.m_fWidth);
				break;

			case eImageOrientation_Normal:			///!< none Orientation
			case eImageOrientation_FlipHorizon:		///!< flip image horizon
			case eImageOrientation_FlipVertical:	///!< flip image vertical
			default:
				m_aImageSize.SetValue( 
					m_pTexture->GetWidth() * aTexSize.m_fWidth, 
					m_pTexture->GetHeight() * aTexSize.m_fHeight);
				break;
			}
		}

		return m_aImageSize;
	}
	//------------------------------------------------------------------------------
	CGUITexture* CGUIImage::GetTexture()
	{
		Load();
		return m_pTexture;
	}
	//------------------------------------------------------------------------------
	void CGUIImage::Draw(
		IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColorRect& rColorRect,
		real fAlpha	)
	{
		Load();

		CGUIColorRect aColorRect = rColorRect;
		aColorRect.m_top_left.SetAlpha(aColorRect.m_top_left.GetAlpha()*fAlpha);
		aColorRect.m_top_right.SetAlpha(aColorRect.m_top_right.GetAlpha()*fAlpha);
		aColorRect.m_bottom_left.SetAlpha(aColorRect.m_bottom_left.GetAlpha()*fAlpha);
		aColorRect.m_bottom_right.SetAlpha(aColorRect.m_bottom_right.GetAlpha()*fAlpha);
		pRender->DrawTile( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,
			aColorRect.m_top_left,aColorRect.m_top_right,
			aColorRect.m_bottom_left,aColorRect.m_bottom_right);
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw(
		IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		real fAlpha)
	{
		Load();

		if( m_eImageType == eIT_COLOR )
		{
			CGUIColor aColor = m_aColor;
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,aColor);
		}
		else
		{
			CGUIColor aColor(1.0f,1.0f,1.0f,1.0f);
			aColor.SetAlpha(fAlpha);
			pRender->DrawTile( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,aColor);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw(
		IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColor& rColor,
		real fAlpha)
	{
		Load();

		if( m_eImageType == eIT_COLOR )
		{
			CGUIColor aColor = m_aColor*rColor;
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,aColor);
		}
		else
		{
			CGUIColor aColor(rColor);
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,aColor);
		}
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIImage::GetFullFilePath() const
	{
		CGUIString strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;
		return strFullPath;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIImage::GetFilePath() const
	{
		return m_strPath;
	}
	//------------------------------------------------------------------------------
	const CGUIRect& CGUIImage::GetUVRect() const
	{
		return m_aUVRect;
	}
	//------------------------------------------------------------------------------
	EImageOrientation CGUIImage::GetOrientation() const
	{
		return m_eImageOrientation;
	}
	//------------------------------------------------------------------------------

}//namespace guiex




