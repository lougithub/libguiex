/** 
* @file guiimage.cpp
* @brief image used in widgets
* @author ken
* @date 2006-07-17
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guibase.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guiperfmonitor.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>
#include <libguiex_core/guirenderrect.h>

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
		:CGUIResource( rName, rSceneName, "IMAGE" )
		,m_pTexture(NULL)
		,m_eImageOrientation(eImageOrientation_Normal)
		,m_eImageType( eIT_COLOR )
		,m_aColor( rColor )
		,m_aUVRect(CGUIRect(0.f,0.f,1.f,1.f))
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
		:CGUIResource( rName, rSceneName, "IMAGE" )
		,m_pTexture(NULL)
		,m_eImageOrientation(eImageOrientation_Normal)
		,m_eImageType( eIT_MEM )
		,m_pBuffPtr(buffPtr)
		,m_nBuffWidth(buffWidth)
		,m_nBuffHeight(buffHeight)
		,m_ePixelFormat(ePixelFormat)
		,m_aUVRect(CGUIRect(0.f,0.f,1.f,1.f))
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
		:CGUIResource( rName, rSceneName, "IMAGE" )
		,m_pTexture(NULL)
		,m_eImageOrientation(eImageOrientation)
		,m_eImageType( eIT_FILE )
		,m_strPath( rPath )
		,m_aUVRect( rUVRect )
		,m_aImageSize( rSize )
	{
	}
	//------------------------------------------------------------------------------
	CGUIImage::~CGUIImage()	
	{
		Unload();
	}
	//------------------------------------------------------------------------------
	int32	CGUIImage::DoLoad() const
	{
		GUI_ASSERT( m_pTexture==NULL, "the texture has existed");

		if( m_eImageType == eIT_FILE )
		{
			//get full path
			CGUIString strFullPath = CGUISceneInfoManager::Instance()->GetSceneFileRootPath( m_strSceneName ) + m_strPath;
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
	void	CGUIImage::DoUnload()
	{
		if( m_pTexture )
		{
			CGUITextureManager::Instance()->DestroyTexture(m_pTexture);
			m_pTexture = NULL;
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIImage::GetSize() const
	{
		if( !m_aImageSize.IsEqualZero())
		{
			if( !IsLoaded())
			{
				Load();
			}

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
	void	CGUIImage::Draw(IGUIInterfaceRender* pRender,
		const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColorRect& rColorRect,
		real fAlpha	) const
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
		pRender->DrawTile( rWorldMatrix, rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,
			aColorRect.m_top_left.GetARGB(),aColorRect.m_top_right.GetARGB(),
			aColorRect.m_bottom_left.GetARGB(),aColorRect.m_bottom_right.GetARGB());
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw( IGUIInterfaceRender* pRender,
		const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect,
		real z, 
		real fAlpha) const
	{
		if( !IsLoaded())
		{
			Load();
		}

		if( m_eImageType == eIT_COLOR )
		{
			CGUIColor aColor = m_aColor;
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( rWorldMatrix, rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
		else
		{
			CGUIColor aColor(0xFFFFFFFF);
			aColor.SetAlpha(fAlpha);
			pRender->DrawTile( rWorldMatrix, rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIImage::Draw(IGUIInterfaceRender* pRender,
		const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect,
		real z, 
		const CGUIColor& rColor,
		real fAlpha) const
	{
		if( !IsLoaded())
		{
			Load();
		}

		if( m_eImageType == eIT_COLOR )
		{
			CGUIColor aColor = m_aColor*rColor;
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( rWorldMatrix, rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
		else
		{
			CGUIColor aColor(rColor);
			aColor.SetAlpha(aColor.GetAlpha()*fAlpha);
			pRender->DrawTile( rWorldMatrix, rDestRect, z, m_pTexture->GetTextureImplement(),m_aUVRect, m_eImageOrientation,
				aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB(),aColor.GetARGB());
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex




