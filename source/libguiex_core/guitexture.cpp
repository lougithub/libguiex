/** 
* @file guitexture.cpp
* @brief wrapper of texture
* @author ken
* @date 2007-06-02
*/


//============================================================================//
// include
//============================================================================// 
#include "guitexture.h"
#include "guitextureimp.h"
#include "guiinterfacemanager.h"
#include "guiinterfacerender.h"
#include "guilogmsGmanager.h"


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITexture::CGUITexture( )
		:m_pTextureImp(NULL)
		,m_pDumpMemory(NULL)
		,m_nType(0)
		,m_bIsBottomUp(false)
	{
	}
	//------------------------------------------------------------------------------
	CGUITexture::~CGUITexture()
	{
		GUI_ASSERT( GetRefCount()==0, "texture is still referenced");

		DestoryTextureImplement();
		FreeMemory();
	}
	//------------------------------------------------------------------------------
	void CGUITexture::PrepareForFont()
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");
		m_pTextureImp->PrepareForFont();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief convert size to texture's standard size
	*/
	uint16	CGUITexture::ConvertToTextureSize(uint16 size)
	{
		if ((size & (size - 1)) || !size)
		{
			int log = 0;

			// get integer log of 'size' to base 2
			while (size >>= 1)
			{
				++log;
			}

			// use log to calculate value to use as size.
			size = (2 << log);
		}
		return size;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Returns the current pixel width of the texture
	*/
	uint16 CGUITexture::GetWidth(void) const
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");

		return m_pTextureImp->GetWidth();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Returns the current pixel height of the texture
	*/
	uint16 CGUITexture::GetHeight(void) const
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");

		return m_pTextureImp->GetHeight();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief copy a sub_image to texture
	*/
	void CGUITexture::CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight,EGuiPixelFormat ePixelFormat, uint8* pBuffer)
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");

		return m_pTextureImp->CopySubImage(nX, nY, nWidth, nHeight, ePixelFormat, pBuffer);
	}
	//------------------------------------------------------------------------------
	//!< notify when texture imp is deleted
	void CGUITexture::NotifyDeletedFromImp()
	{
		m_pTextureImp = NULL;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief create a texture implement from current render
	*/
	uint32 CGUITexture::CreateTextureImplement()
	{
		if( !m_pTextureImp )
		{
			IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
			m_pTextureImp = pRender->CreateTexture();
			m_pTextureImp->SetTexture(this);

			GUI_ASSERT(m_pTextureImp, "failed to create texture");
			return 0;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief destroy a texture implement from current render
	*/
	void CGUITexture::DestoryTextureImplement()
	{
		if( m_pTextureImp)
		{
			m_pTextureImp->SetTexture(NULL);
			m_pTextureImp->Destroy();
			m_pTextureImp = NULL;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Loads the specified image file into the texture.  The texture is resized 
	* as required to hold the image.
	* @exception CGUIException::ThrowException if failed.
	* @return -1 for failed
	*/
	int32 CGUITexture::CreateTextureByFile(const CGUIString& filename )
	{
		CreateTextureImplement();

		return m_pTextureImp->LoadFromFile(filename);
	}
	//------------------------------------------------------------------------------

	/**
	* @brief Loads (copies) an image from memory into the texture.  The texture is resized as 
	required to hold the image.
	* @param ePixelFormat pixel format.
	* @exception CGUIException::ThrowException if failed.
	* @return -1 for failed
	*/
	int32 CGUITexture::CreateTextureByMemory(
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat )
	{
		CreateTextureImplement();

		return m_pTextureImp->LoadFromMemory(buffPtr,buffWidth,buffHeight,ePixelFormat);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create texture by given size and pixelformat
	* @return -1 for failed
	*/
	int32 CGUITexture::CreateTextureBySize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		DestoryTextureImplement();
		IGUIInterfaceRender* pRender =  CGUIInterfaceManager::Instance()->GetInterfaceRender();
		m_pTextureImp = pRender->CreateTexture(nWidth, nHeight, ePixelFormat);
		GUI_ASSERT(m_pTextureImp, "failed to create texture");
		return m_pTextureImp?0:-1;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief free memory which contains the texture data
	*/
	void CGUITexture::FreeMemory()
	{
		if( m_pDumpMemory )
		{
			delete[] m_pDumpMemory;
			m_pDumpMemory = NULL;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief dump texture to memory and release texture implement,
	* usually it will happen when changing render.
	* @return 0 for success, others for failed
	*/
	int32 CGUITexture::SaveToMemory()
	{
		GUI_ASSERT(m_pTextureImp, "texture implement has been released");

		uint32 nBufSize = m_pTextureImp->GetBufferSize();
		GUI_ASSERT(nBufSize>0, "invalid buffer size");

		FreeMemory();
		m_pDumpMemory = new uint8[nBufSize];
		if( 0 != m_pTextureImp->GetBuffer(m_pDumpMemory, nBufSize, m_eDumpPixelFormat))
		{
			GUI_FORCE_ASSERT("failed to get texture buffer");
			return -1;
		}
		m_aDumpTextureWidth = m_pTextureImp->GetWidth();
		m_aDumpTextureHeight = m_pTextureImp->GetHeight();

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a new texture implement and load texture from memory,
	* usually it will happen when changing render.
	* @return 0 for success, others for failed
	*/
	int32 CGUITexture::LoadFromMemory()
	{
		GUI_ASSERT(m_pDumpMemory, "no dump memory here");

		return CreateTextureByMemory(m_pDumpMemory, m_aDumpTextureWidth, m_aDumpTextureHeight, m_eDumpPixelFormat);
	}
	//------------------------------------------------------------------------------
	void CGUITexture::SetBottomUp( bool bIsBottomUp )
	{
		m_bIsBottomUp = bIsBottomUp;
	}
	//------------------------------------------------------------------------------
	bool CGUITexture::IsBottomUp( ) const
	{
		return m_bIsBottomUp;
	}
	//------------------------------------------------------------------------------
	real CGUITexture::UVConvertTopleft2Engine_v( real v ) const
	{
		return (m_bIsBottomUp?1-v:v);
	}
	//------------------------------------------------------------------------------
#if !GUI_TEXTURE_NPOT_SUPPORT
	real CGUITexture::NPOT_Convert_U( real u ) const
	{
		return u * m_pTextureImp->GetWidth() / m_pTextureImp->GetPOTWidth(); 
	}
	//------------------------------------------------------------------------------
	real CGUITexture::NPOT_Convert_V( real v ) const
	{
		return v * m_pTextureImp->GetHeight() / m_pTextureImp->GetPOTHeight();
	}
#endif
	//------------------------------------------------------------------------------
}//namespaceguiex
