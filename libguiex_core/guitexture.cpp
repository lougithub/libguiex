/** 
* @file guitexture.cpp
* @brief wrapper of texture
* @author ken
* @date 2007-06-02
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacerender.h>


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
		,m_nRefCount(0)
	{
	}
	//------------------------------------------------------------------------------
	CGUITexture::~CGUITexture()
	{
		GUI_ASSERT( m_nRefCount==0, "texture is still referenced");

		DestoryTextureImplement();
		FreeMemory();
	}
	//------------------------------------------------------------------------------
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
	uint16	CGUITexture::GetWidth(void) const
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");

		return m_pTextureImp->GetWidth();
	}
	//------------------------------------------------------------------------------
	uint16	CGUITexture::GetHeight(void) const
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");

		return m_pTextureImp->GetHeight();
	}
	//------------------------------------------------------------------------------
	void		CGUITexture::CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight,EGuiPixelFormat ePixelFormat, uint8* pBuffer)
	{
		GUI_ASSERT(m_pTextureImp, "texture implement is NULL");

		return m_pTextureImp->CopySubImage(nX, nY, nWidth, nHeight, ePixelFormat, pBuffer);
	}
	//------------------------------------------------------------------------------
	void	CGUITexture::NotifyDeletedFromImp()
	{
		m_pTextureImp = NULL;
	}
	//------------------------------------------------------------------------------
	uint32	CGUITexture::CreateTextureImplement()
	{
		if( !m_pTextureImp )
		{
			IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
			m_pTextureImp = pRender->CreateTexture();
			m_pTextureImp->SetTexture(this);

			GUI_ASSERT(m_pTextureImp, "failed to create texture");
			return m_pTextureImp?0:-1;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void		CGUITexture::DestoryTextureImplement()
	{
		if( m_pTextureImp)
		{
			m_pTextureImp->SetTexture(NULL);
			m_pTextureImp->Destroy();
			m_pTextureImp = NULL;
		}
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture::CreateTextureByFile(const CGUIString& filename )
	{
		CreateTextureImplement();

		return m_pTextureImp->LoadFromFile(filename);
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture::CreateTextureByMemory(
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat )
	{
		CreateTextureImplement();

		return m_pTextureImp->LoadFromMemory(buffPtr,buffWidth,buffHeight,ePixelFormat);
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture::CreateTextureBySize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		DestoryTextureImplement();
		IGUIInterfaceRender* pRender =  CGUIInterfaceManager::Instance()->GetInterfaceRender();
		m_pTextureImp = pRender->CreateTexture(nWidth, nHeight, ePixelFormat);
		GUI_ASSERT(m_pTextureImp, "failed to create texture");
		return m_pTextureImp?0:-1;
	}
	//------------------------------------------------------------------------------
	void	CGUITexture::FreeMemory()
	{
		if( m_pDumpMemory )
		{
			delete[] m_pDumpMemory;
			m_pDumpMemory = NULL;
		}
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture::SaveToMemory()
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
	int32	CGUITexture::LoadFromMemory()
	{
		GUI_ASSERT(m_pDumpMemory, "no dump memory here");

		return CreateTextureByMemory(m_pDumpMemory, m_aDumpTextureWidth, m_aDumpTextureHeight, m_eDumpPixelFormat);
	}
	//------------------------------------------------------------------------------
	void	CGUITexture::Unreference()
	{
		GUI_ASSERT(m_nRefCount>0, "reference count is zero now, can't be unreference more.");
		--m_nRefCount;
	}
	//------------------------------------------------------------------------------

}//namespaceguiex
