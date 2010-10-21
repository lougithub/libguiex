/** 
* @file guitexture_dx9.cpp
* @brief use directx 9 to render gui
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include "EnginePrivate.h"

#include <module\render_ue3\guitexture_ue3.h>
#include <module\render_ue3\guirender_ue3.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guidatachunk.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfacefilesys.h>
#include <libguiex_core\guiinterfaceimageloader.h>
#include <libguiex_core\guigarbagecollector.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class FUE3Texture : public FTextureResource
	{
	public:	
		struct SSubImageInfo
		{
			uint32 nX;
			uint32 nY;
			uint32 nWidth;
			uint32 nHeight;
			guiex::EGuiPixelFormat ePixelFormat;
			TArray<BYTE> arrayBuffer;
		};


	public:
		FUE3Texture(UINT inTexSizeX, UINT inTexSizeY, EGuiPixelFormat ePixelFormat, const BYTE* inTexData);

		// FResource interface.
		virtual void InitRHI();

		/** Returns the width of the texture in pixels. */
		virtual UINT GetSizeX() const
		{
			return mTextureSizeX;
		}

		/** Returns the height of the texture in pixels. */
		virtual UINT GetSizeY() const
		{
			return mTextureSizeY;
		}

		UINT GetBytesPerPixel( ) const
		{
			return mBytesPerPixel;
		}

		void		CopySubImage(const SSubImageInfo* pSubImageInfo);



	protected:
		UINT			mTextureSizeX;
		UINT			mTextureSizeY;
		TArray<BYTE>	mTextureData;
		EGuiPixelFormat	mGuiPixelFormat;
		UINT			mBytesPerPixel;
		BYTE			mFormat;
		FTexture2DRHIRef mTexture2D;
	};

	//------------------------------------------------------------------------------
	FUE3Texture::FUE3Texture(UINT inTexSizeX, UINT inTexSizeY, EGuiPixelFormat eGuiPixelFormat, const BYTE* inTexData)
		:mGuiPixelFormat(eGuiPixelFormat)
	{
		check(inTexSizeX != 0 && inTexSizeY != 0);

		bIgnoreGammaConversions = TRUE;

		//get byte per pixel
		switch( eGuiPixelFormat )
		{
		case GUI_PF_ARGB_32:
		case GUI_PF_RGBA_32:
			mFormat = PF_A8R8G8B8;
			mBytesPerPixel = 4;
			break;

		default:
			throw CGUIException("[FUE3Texture::FUE3Texture]: unknown pixel format;");
			mBytesPerPixel = 4;
			break;
		}

		mTextureSizeX = inTexSizeX;
		mTextureSizeY = inTexSizeY;

		// allocate texture space.
		if( inTexData )
		{
			mTextureData.Empty(mTextureSizeX * mTextureSizeY * mBytesPerPixel);
			mTextureData.AddZeroed(mTextureSizeX * mTextureSizeY * mBytesPerPixel);
			appMemcpy( mTextureData.GetData(), inTexData, mTextureSizeX * mTextureSizeY * mBytesPerPixel );
		}
	}

	//------------------------------------------------------------------------------
	// FResource interface.
	void FUE3Texture::InitRHI()
	{
		// Create the texture RHI.  		
		mTexture2D = RHICreateTexture2D(GetSizeX(),GetSizeY(),mFormat,1,TexCreate_Uncooked /*| TexCreate_SRGB*/,NULL);
		TextureRHI = mTexture2D;

		// Write the contents of the texture.
		if( mTextureData.Num() != 0 )
		{
			UINT DestStride;
			BYTE* DestBuffer = (BYTE*)RHILockTexture2D(mTexture2D,0,TRUE,DestStride,FALSE);
			appMemcpy(DestBuffer, mTextureData.GetData(), mTextureSizeX*mTextureSizeY*mBytesPerPixel );
			switch(mGuiPixelFormat)
			{
			case GUI_PF_RGBA_32:
				for (UINT32 i = 0; i < mTextureSizeY; ++i)
				{
					for (UINT32 j = 0; j < mTextureSizeX; ++j)
					{
						INT nStartIdx = i*GetSizeX() * mBytesPerPixel+j*mBytesPerPixel;
						std::swap(DestBuffer[nStartIdx+0], DestBuffer[nStartIdx+2]);
						//std::swap(DestBuffer[nStartIdx+1], DestBuffer[nStartIdx+3]);
						//std::swap(DestBuffer[nStartIdx+1], DestBuffer[nStartIdx+2]);
					}
				}
				break;

			case GUI_PF_ARGB_32:
				break;

			default:
				throw CGUIException("[FUE3Texture::FUE3Texture]: unknown pixel format;");
				mBytesPerPixel = 4;
				break;
			}
			RHIUnlockTexture2D(mTexture2D,0,FALSE);

			// strip data.
			mTextureData.Reset();
		}

		// Create the sampler state RHI resource.
		FSamplerStateInitializerRHI SamplerStateInitializer =
		{
			SF_Point,
			AM_Wrap,
			AM_Wrap,
			AM_Wrap
		};
		SamplerStateRHI = RHICreateSamplerState(SamplerStateInitializer);
	}
	//------------------------------------------------------------------------------
	void		FUE3Texture::CopySubImage(const SSubImageInfo* pSubImageInfo)
	{
		UINT DestStride;
		BYTE* DestBuffer = (BYTE*)RHILockTexture2D(mTexture2D,0,TRUE,DestStride,FALSE);

		DestBuffer += (mTextureSizeX*pSubImageInfo->nY+pSubImageInfo->nX)*mBytesPerPixel;
		const BYTE* SrcBuffer = pSubImageInfo->arrayBuffer.GetData();
		if( pSubImageInfo->ePixelFormat == GUI_PF_ARGB_32 )
		{
			for( uint32 i=0; i<pSubImageInfo->nHeight; ++i)
			{
				memcpy(DestBuffer, SrcBuffer, mBytesPerPixel*pSubImageInfo->nWidth);
				DestBuffer += mBytesPerPixel*pSubImageInfo->nWidth;
				SrcBuffer += mBytesPerPixel*pSubImageInfo->nWidth;
				DestBuffer += (mTextureSizeX-pSubImageInfo->nWidth)*mBytesPerPixel;
			}
		}
		else
		{
			throw CGUIException("[FUE3Texture::CopySubImage]: un-supported pixel format;");
		}

		RHIUnlockTexture2D(mTexture2D,0,FALSE);

		delete pSubImageInfo;
	}
	//------------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
static void CopySubImage(guiex::FUE3Texture* UE3Texture, INT nX, INT nY, UINT nWidth, UINT nHeight,guiex::EGuiPixelFormat ePixelFormat, BYTE* pBuffer)
{
	guiex::FUE3Texture::SSubImageInfo* pSubImageInfo = new guiex::FUE3Texture::SSubImageInfo;
	pSubImageInfo->nX = nX;
	pSubImageInfo->nY = nY;
	pSubImageInfo->nWidth = nWidth;
	pSubImageInfo->nHeight = nHeight;
	pSubImageInfo->ePixelFormat = ePixelFormat;
	pSubImageInfo->arrayBuffer.Empty(nWidth * nHeight * 4);
	pSubImageInfo->arrayBuffer.AddZeroed(nWidth * nHeight * 4);
	appMemcpy( pSubImageInfo->arrayBuffer.GetData(), pBuffer, nWidth * nHeight * 4 );

	// Send the release message.
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		CopySubImageCommand,
		guiex::FUE3Texture*,UE3Texture,UE3Texture,
		guiex::FUE3Texture::SSubImageInfo*,pSubImageInfo,pSubImageInfo,
	{
		UE3Texture->CopySubImage(pSubImageInfo);
	});
}


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITexture_UE3::CGUITexture_UE3(IGUIInterfaceRender* pRender)
		:CGUITextureImp(pRender)
		,m_pUE3Texture(NULL)
	{

	}
	//------------------------------------------------------------------------------
	CGUITexture_UE3::~CGUITexture_UE3()
	{
		FreeUE3Texture();
	}
	//------------------------------------------------------------------------------
	uint16	CGUITexture_UE3::GetWidth(void) const
	{
		check(m_pUE3Texture);
		return m_pUE3Texture->GetSizeX();
	}
	//------------------------------------------------------------------------------
	uint16	CGUITexture_UE3::GetHeight(void) const
	{
		check(m_pUE3Texture);
		return m_pUE3Texture->GetSizeY();
	}
	//------------------------------------------------------------------------------
	uint32 CGUITexture_UE3::GetBufferSize() const
	{
		checkMsg(0, TEXT("[CGUITexture_UE3::GetBufferSize]: not implemented"));
		throw CGUIException("[CGUITexture_UE3::GetBufferSize]: not implemented;");
		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUITexture_UE3::GetBuffer(uint8* pBuffer, uint32 nBufferSize, EGuiPixelFormat& rPixelFormat)
	{
		checkMsg(0, TEXT("[CGUITexture_UE3::GetBuffer]: not implemented"));
		throw CGUIException("[CGUITexture_UE3::GetBuffer]: not implemented;");
		return -1;
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture_UE3::LoadFromFile(const CGUIString& filename )
	{
		IGUIInterfaceImageLoader* pImageLoader = static_cast<IGUIInterfaceImageLoader*>(CGUIInterfaceManager::Instance()->GetInterface("IGUIImageLoader"));
		if( !pImageLoader )
		{
			throw CGUIException("[CGUITexture_UE3::LoadFromFile]: - failed to get image loader!");
			return -1;
		}

		CGUIAutoRelease<CGUIImageData> pImageData( pImageLoader->LoadFromFile( filename ));
		if( !pImageData )
		{
			throw CGUIException("[CGUITexture_UE3::LoadFromFile]: - failed to Load image file <%s>!", filename.c_str());
			return -1;
		}

		int32 ret = LoadFromMemory( pImageData->GetData(), pImageData->GetWidth(), pImageData->GetHeight(), pImageData->GetPixelFormat());

		return ret;
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture_UE3::LoadFromMemory(const void* buffPtr, int32 buffWidth, int32 buffHeight, EGuiPixelFormat ePixelFormat)
	{
		//free old texture
		FreeUE3Texture();

		switch(ePixelFormat)
		{
		case GUI_PF_ARGB_32:
		case GUI_PF_RGBA_32:
		//case GUI_PF_LA_16:
			break;

		default:
			throw CGUIException("[CGUITexture_UE3::LoadFromMemory]: unknown pixel format;");
		}

		m_pUE3Texture = new FUE3Texture( buffWidth, buffHeight, ePixelFormat, (const BYTE *)buffPtr );
		BeginInitResource(m_pUE3Texture);
		
		return 0;
	}
	//------------------------------------------------------------------------------
	void		CGUITexture_UE3::SetUE3TextureSize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		//free old texture
		FreeUE3Texture();

		switch(ePixelFormat)
		{
		case GUI_PF_ARGB_32:
		case GUI_PF_RGBA_32:
		//case GUI_PF_LA_16:
			break;

		default:
			throw CGUIException("[CGUITexture_UE3::SetUE3TextureSize]: unknown pixel format;");
			break;
		}

		m_pUE3Texture = new FUE3Texture( nWidth, nHeight, ePixelFormat, NULL );
		BeginInitResource(m_pUE3Texture);
	}
	//------------------------------------------------------------------------------
	void		CGUITexture_UE3::CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight,EGuiPixelFormat ePixelFormat, uint8* pBuffer)
	{
		check( m_pUE3Texture );
		check( pBuffer );
		if( nX + nWidth > m_pUE3Texture->GetSizeX() ||
			nY + nHeight > m_pUE3Texture->GetSizeY() )
		{
			throw CGUIException("[FUE3Texture::CopySubImage]: invalid range of sub image area");
		}
		if( ePixelFormat != GUI_PF_ARGB_32 )
		{
			throw CGUIException("[FUE3Texture::CopySubImage]: invalid pixel format of sub image area, only support GUI_PF_ARGB_32 now");
		}

		::CopySubImage(m_pUE3Texture, nX, nY, nWidth, nHeight,ePixelFormat, pBuffer);
	}
	//------------------------------------------------------------------------------
	void				CGUITexture_UE3::FreeUE3Texture()
	{
		if (m_pUE3Texture != NULL)
		{
			ReleaseResourceAndFlush(m_pUE3Texture);
			delete m_pUE3Texture;
			m_pUE3Texture = NULL;
		}
	}
	//------------------------------------------------------------------------------


}//namespace guiex
