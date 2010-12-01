/** 
* @file guiimageloader_tga.cpp
* @brief interface to load image from file or memory.
* @author ken
* @date 2006-07-13
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module/imageloader_tga/guiimageloader_tga.h>
#include <libguiex_module/imageloader_tga/guiimagedata_tga.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	typedef struct
	{
		uint8 Header[12];									// TGA File Header
	} TGAHeader;

	typedef struct
	{
		uint8		header[6];								// First 6 Useful Bytes From The Header
		uint32		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
		uint32		imageSize;								// Used To Store The Image Size When Setting Aside Ram
		uint32		temp;									// Temporary Variable
		EGuiPixelFormat	type;	
		uint32		Height;									//Height of Image
		uint32		Width;									//Width ofImage
		uint32		Bpp;									// Bits Per Pixel
	} TGA;


	uint8 uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	uint8 cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header
}


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIImageLoader_tga);
	//------------------------------------------------------------------------------
	IGUIImageLoader_tga::IGUIImageLoader_tga()
	{
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_tga::~IGUIImageLoader_tga()
	{
	}
	//------------------------------------------------------------------------------
	int		IGUIImageLoader_tga::DoInitialize(void* )
	{

		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIImageLoader_tga::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_tga::LoadFromFile( const CGUIString& rFileName  )
	{
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();;
		CGUIDataChunk aDataChunk;
		if( 0 == pFileSys->ReadFile( rFileName, aDataChunk ))
		{
			return LoadFromMemory(aDataChunk.GetDataPtr(), aDataChunk.GetSize());
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_tga::LoadFromMemory( uint8* pFileData, size_t nSize )
	{
		if( !pFileData || nSize <= sizeof( TGAHeader) )
		{
			return NULL;
		}

		TGAHeader tgaheader;
		uint32 uHeadSize = sizeof( TGAHeader);
		memcpy( &tgaheader, pFileData, uHeadSize );
		pFileData += uHeadSize;
		nSize -= uHeadSize;

		if(memcmp(uTGAcompare, &tgaheader, uHeadSize) == 0)	
		{														
			return LoadUncompressedTGA(pFileData, nSize );
		}
		else if(memcmp(cTGAcompare, &tgaheader, uHeadSize) == 0)		
		{																
			return LoadCompressedTGA(pFileData, nSize);				
		}
		else															
		{
			throw CGUIException("[IGUIImageLoader_tga::LoadFromMemory] - Failed to load tga image!");
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_tga::LoadUncompressedTGA( uint8* pFileData, size_t nSize )
	{
		if( !pFileData || nSize <= 0 )
		{
			return NULL;
		}

		TGA tga;
		uint32 uHeadSize = sizeof(tga.header);
		if( nSize < uHeadSize )
		{
			throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}
		memcpy( tga.header, pFileData, uHeadSize );
		pFileData += uHeadSize;
		nSize -= uHeadSize;

		tga.Width = tga.header[1] * 256 + tga.header[0];
		tga.Height = tga.header[3] * 256 + tga.header[2];
		tga.Bpp		= tga.header[4];

		if((tga.Width <= 0) || (tga.Height <= 0) || ((tga.Bpp != 24) && (tga.Bpp !=32)))
		{
			throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}

		if(tga.Bpp == 24)		
			tga.type = GUI_PF_RGB_24;
		else					
			tga.type = GUI_PF_RGBA_32;

		tga.bytesPerPixel	= (tga.Bpp / 8);					
		tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);	
		if( nSize < tga.imageSize )
		{
			throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}
		//create image data
		CGUIImageData_tga * pImageData = new CGUIImageData_tga(this);
		uint8* tmpBuff = pImageData->SetImageData(tga.Width, tga.Height, tga.type);
		memcpy( tmpBuff, pFileData, tga.imageSize);

		// Byte Swapping Optimized By Steve Thomas
		//for(uint32 cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
		//{
		//	tmpBuff[cswap] ^= tmpBuff[cswap+2] ^= tmpBuff[cswap] ^= tmpBuff[cswap+2];
		//}
		for(uint32 cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
		{
			uint8 temp = tmpBuff[cswap+0];
			tmpBuff[cswap+0] = tmpBuff[cswap+2];
			tmpBuff[cswap+2] = temp;
		}
		return pImageData;
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_tga::LoadCompressedTGA( uint8* pFileData, size_t nSize )
	{
		TGA tga;
		uint32 uHeadSize = sizeof(tga.header);
		if( nSize < uHeadSize )
		{
			throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}
		memcpy( tga.header, pFileData, uHeadSize );
		pFileData += uHeadSize;
		nSize -= uHeadSize;

		tga.Width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
		tga.Height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
		tga.Bpp	= tga.header[4];										// Determine Bits Per Pixel

		if((tga.Width <= 0) || (tga.Height <= 0) || ((tga.Bpp != 24) && (tga.Bpp !=32)))
		{
			throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}

		if(tga.Bpp == 24)		
			tga.type = GUI_PF_RGB_24;
		else					
			tga.type = GUI_PF_RGBA_32;

		tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
		tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
		//create image data
		CGUIImageData_tga * pImageData = new CGUIImageData_tga(this);
		uint8* tmpBuff = pImageData->SetImageData(tga.Width, tga.Height, tga.type);

		uint32 pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
		uint32 currentpixel	= 0;												// Current pixel being read
		uint32 currentbyte	= 0;												// Current byte 
		uint8 * colorbuffer = (uint8 *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

		do
		{
			uint8 chunkheader = 0;											// Storage for "chunk" header

			uint32 uChunkHeadSize = sizeof(uint8);
			if( nSize < uChunkHeadSize )
			{
				delete pImageData;
				free( colorbuffer );
				throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
				return NULL;
			}
			memcpy( &chunkheader, pFileData, uChunkHeadSize );
			pFileData += uChunkHeadSize;
			nSize -= uChunkHeadSize;

			if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
			{																	// that follow the header
				chunkheader++;													// add 1 to get number of following color values
				for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
				{
					if( nSize < tga.bytesPerPixel )
					{
						delete pImageData;
						free( colorbuffer );
						throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
						return NULL;
					}
					memcpy( colorbuffer, pFileData, tga.bytesPerPixel );
					pFileData += tga.bytesPerPixel;
					nSize -= tga.bytesPerPixel;

					// write to memory
					tmpBuff[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
					tmpBuff[currentbyte + 1	] = colorbuffer[1];
					tmpBuff[currentbyte + 2	] = colorbuffer[0];

					if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
					{
						tmpBuff[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
					}

					currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
					currentpixel++;															// Increase current pixel by 1

					if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
					{
						delete pImageData;
						free( colorbuffer );
						throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
						return NULL;													// Return failed
					}
				}
			}
			else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
			{
				chunkheader -= 127;		
				if( nSize < tga.bytesPerPixel )
				{
					delete pImageData;
					free( colorbuffer );
					throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
					return NULL;
				}
				memcpy( colorbuffer, pFileData, tga.bytesPerPixel );
				pFileData += tga.bytesPerPixel;
				nSize -= tga.bytesPerPixel;

				for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
				{																			// by the header
					tmpBuff[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
					tmpBuff[currentbyte + 1	] = colorbuffer[1];
					tmpBuff[currentbyte + 2	] = colorbuffer[0];

					if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
					{
						tmpBuff[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
					}

					currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
					currentpixel++;															// Increase pixel count by 1

					if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
					{
						delete pImageData;
						free( colorbuffer );
						throw CGUIException("[IGUIImageLoader_tga::LoadUncompressedTGA] - Invalid texture information!");
						return NULL;													// Return failed
					}
				}
			}
		}
		while(currentpixel < pixelcount);	
		
		free( colorbuffer );
		return pImageData;
	}
	//------------------------------------------------------------------------------
	void	IGUIImageLoader_tga::DestroyImageData(CGUIImageData* pImageData)
	{
		delete pImageData;
	}
	//------------------------------------------------------------------------------
	void	IGUIImageLoader_tga::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------


}//namespace guiex

