/** 
* @file guiimageloader_png_tga.cpp
* @brief interface to load image from file or memory.
* @author ken
* @date 2011-07-06
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module/imageloader_png_tga/guiimageloader_png_tga.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <png.h>

//============================================================================//
// declare
//============================================================================// 
#define PNG_SIG_BYTES 8
namespace guiex
{
	struct SPngFileData
	{
		uint8* m_pData;
		uint32 m_uSize;
	};

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


	static uint8 uTGAcompare1[12] = {0,0,2, 0,0,0,0,0,0,0,0,0}; // Uncompressed, RGB images
	static uint8 uTGAcompare2[12] = {0,0,3, 0,0,0,0,0,0,0,0,0}; // Uncompressed, black and white images
	static uint8 cTGAcompare1[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed, RGB images.
	static uint8 cTGAcompare2[12] = {0,0,11,0,0,0,0,0,0,0,0,0};	// Compressed, black and white images.
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIImageLoader_png_tga);
	//------------------------------------------------------------------------------
	const char* IGUIImageLoader_png_tga::StaticGetModuleName()
	{
		return "IGUIImageLoader_png_tga";
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_png_tga::IGUIImageLoader_png_tga()
		:IGUIInterfaceImageLoader( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_png_tga::~IGUIImageLoader_png_tga()
	{
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief initialize render
	* @return 0 for success
	*/
	int	IGUIImageLoader_png_tga::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIImageLoader_png_tga::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief load image from file
	* @return pointer of CGUIImageData, NULL for failed
	*/
	CGUIImageData* IGUIImageLoader_png_tga::LoadFromFile( const CGUIString& rFileName  )
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
	static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		png_voidp a = png_get_io_ptr(png_ptr);
		SPngFileData* pFileData = (SPngFileData*)a;
		if (length > pFileData->m_uSize)
		{
			png_error(png_ptr, "EOF");
			return;
		}
		
		memcpy( data, pFileData->m_pData, length );
		pFileData->m_pData += length;
		pFileData->m_uSize -= length;
	}
	/**
	* @brief load image from memory
	* @return pointer of CGUIImageData, NULL for failed
	*/
	CGUIImageData* IGUIImageLoader_png_tga::LoadFromMemory( uint8* pFileData, size_t nSize )
	{
		if( !pFileData )
		{
			return NULL;
		}

		if( nSize >= PNG_SIG_BYTES )
		{
			if( 0 == png_sig_cmp(pFileData, 0, PNG_SIG_BYTES) )
			{
				return LoadPng(pFileData, nSize);
			}
		}
		return LoadTga( pFileData, nSize );
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_png_tga::LoadPng( uint8* pFileData, size_t nSize )
	{
		if( !pFileData || nSize <= PNG_SIG_BYTES )
		{
			return NULL;
		}

		if( 0 != png_sig_cmp(pFileData, 0, PNG_SIG_BYTES) )
		{
			GUI_THROW( "[IGUIImageLoader_png::LoadFromMemory] - not a png image file!");
			return NULL;
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr)
		{
			return NULL;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			return NULL;
		}

		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info)
		{
			png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
			return NULL;
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			return NULL;
		}

		SPngFileData aFileData;
		aFileData.m_pData = pFileData + PNG_SIG_BYTES;
		aFileData.m_uSize = nSize - PNG_SIG_BYTES;
		png_set_read_fn(png_ptr, &aFileData, user_read_data);

		png_set_sig_bytes(png_ptr, PNG_SIG_BYTES);

		png_read_info(png_ptr, info_ptr);

		png_uint_32 width, height;
		width = png_get_image_width(png_ptr, info_ptr);
		height = png_get_image_height(png_ptr, info_ptr);

		int bit_depth, color_type;
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);

		if( color_type == PNG_COLOR_TYPE_PALETTE )
		{
			png_set_palette_to_rgb( png_ptr );
		}

		if( color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
		{
			png_set_expand_gray_1_2_4_to_8( png_ptr );
		}

		if( png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS ) )
		{
			png_set_tRNS_to_alpha (png_ptr);
		}

		if( bit_depth == 16 )
		{
			png_set_strip_16( png_ptr );
		}

		else if( bit_depth < 8 )
		{
			png_set_packing( png_ptr );
		}

		png_read_update_info(png_ptr, info_ptr);

		png_get_IHDR( png_ptr, info_ptr,&width, &height, &bit_depth, &color_type,NULL, NULL, NULL );

		EGuiPixelFormat type = GUI_PF_UNKNOEWN;
		switch( color_type )
		{
		case PNG_COLOR_TYPE_GRAY:
			type = GUI_PF_ALPHA_8;
			break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			type = GUI_PF_LUMINANCE_ALPHA_16;
			break;

		case PNG_COLOR_TYPE_RGB:
			type = GUI_PF_RGB_24;
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			type = GUI_PF_RGBA_32;
			break;
		}

		CGUIImageData * pImageData = new CGUIImageData(this);
		uint8* pixels = pImageData->SetImageData(width, height, type);

		// since Texture2D loads the image "upside-down", there's no need
		// to flip the image here
		png_byte** row_ptrs = (png_bytep*)malloc(height * sizeof(png_bytep));
		for (png_uint_32 i=0; i<height; i++)
		{
			row_ptrs[i] = pixels + i*pImageData->GetWidth()*pImageData->GetBytePerPixel();
		}

		png_read_image(png_ptr, row_ptrs);	
		png_read_end( png_ptr, NULL );
		png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );

		free( row_ptrs );

		return pImageData;
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_png_tga::LoadTga( uint8* pFileData, size_t nSize )
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

		if(memcmp(uTGAcompare1, &tgaheader, uHeadSize) == 0 ||
			memcmp(uTGAcompare2, &tgaheader, uHeadSize) == 0)	
		{														
			return LoadUncompressedTGA(pFileData, nSize );
		}
		else if(memcmp(cTGAcompare1, &tgaheader, uHeadSize) == 0 ||
			memcmp(cTGAcompare2, &tgaheader, uHeadSize) == 0 )
		{																
			return LoadCompressedTGA(pFileData, nSize);				
		}
		else															
		{
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadFromMemory] - Failed to load tga image!");
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_png_tga::LoadUncompressedTGA( uint8* pFileData, size_t nSize )
	{
		if( !pFileData || nSize <= 0 )
		{
			return NULL;
		}

		TGA tga;
		uint32 uHeadSize = sizeof(tga.header);
		if( nSize < uHeadSize )
		{
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}
		memcpy( tga.header, pFileData, uHeadSize );
		pFileData += uHeadSize;
		nSize -= uHeadSize;

		tga.Width = tga.header[1] * 256 + tga.header[0];
		tga.Height = tga.header[3] * 256 + tga.header[2];
		tga.Bpp		= tga.header[4];

		if(tga.Width <= 0 || tga.Height <= 0)
		{
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}

		switch( tga.Bpp )
		{
		case 24:
			tga.type = GUI_PF_RGB_24;
			break;
		case 32:
			tga.type = GUI_PF_RGBA_32;
			break;		
		case 8:
			tga.type = GUI_PF_ALPHA_8;
			break;
		default:
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}

		tga.bytesPerPixel = CGUIImageData::GetBytePerPixel( tga.type );					
		tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);	
		if( nSize < tga.imageSize )
		{
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}
		//create image data
		CGUIImageData * pImageData = new CGUIImageData(this);
		uint8* tmpBuff = pImageData->SetImageData(tga.Width, tga.Height, tga.type);
		//memcpy( tmpBuff, pFileData, tga.imageSize);
		uint32 uDataLineBytes = pImageData->GetWidth()*pImageData->GetBytePerPixel();
		uint32 uFileLineBytes = tga.Width*pImageData->GetBytePerPixel();
		for( uint32 i=0; i<tga.Height; ++i )
		{
			memcpy( tmpBuff + i*uDataLineBytes, pFileData+i*uFileLineBytes, uFileLineBytes );
		}

		if( tga.type == GUI_PF_RGBA_32 || tga.type == GUI_PF_RGB_24 )
		{
			for( uint32 i=0; i<tga.Height; ++i )
			{
				uint32 uWidthStart = i*uDataLineBytes;
				for( uint32 j=0; j<tga.Width; ++j )
				{
					uint32 uStart = uWidthStart + j*pImageData->GetBytePerPixel();
					uint8 temp = tmpBuff[uStart];
					tmpBuff[uStart] = tmpBuff[uStart+2];
					tmpBuff[uStart+2] = temp;
				}
			}
		}
		return pImageData;
	}
	//------------------------------------------------------------------------------
	CGUIImageData* IGUIImageLoader_png_tga::LoadCompressedTGA( uint8* pFileData, size_t nSize )
	{
		TGA tga;
		uint32 uHeadSize = sizeof(tga.header);
		if( nSize < uHeadSize )
		{
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
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
			GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
			return NULL;
		}

		if(tga.Bpp == 24)		
			tga.type = GUI_PF_RGB_24;
		else					
			tga.type = GUI_PF_RGBA_32;

		tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
		tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
		//create image data
		CGUIImageData * pImageData = new CGUIImageData(this);
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
				GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
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
						GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
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
						GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
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
					GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
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
						GUI_THROW( "[IGUIImageLoader_png_tga::LoadUncompressedTGA] - Invalid texture information!");
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
	/**
	* @brief destroy image data
	*/
	void IGUIImageLoader_png_tga::DestroyImageData(CGUIImageData* pImageData)
	{
		delete pImageData;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief used to delete this object
	*/
	void IGUIImageLoader_png_tga::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------

}//namespace guiex

