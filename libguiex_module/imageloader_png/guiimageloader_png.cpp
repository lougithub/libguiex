/** 
* @file guiimageloader_png.cpp
* @brief interface to load image from file or memory.
* @author ken
* @date 2011-07-06
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_module/imageloader_png/guiimageloader_png.h>
#include <libguiex_module/imageloader_png/guiimagedata_png.h>
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
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIImageLoader_png);
	//------------------------------------------------------------------------------
	const char* IGUIImageLoader_png::StaticGetModuleName()
	{
		return "IGUIImageLoader_png";
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_png::IGUIImageLoader_png()
		:IGUIInterfaceImageLoader( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUIImageLoader_png::~IGUIImageLoader_png()
	{
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief initialize render
	* @return 0 for success
	*/
	int	IGUIImageLoader_png::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIImageLoader_png::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	/**
	* @brief load image from file
	* @return pointer of CGUIImageData, NULL for failed
	*/
	CGUIImageData* IGUIImageLoader_png::LoadFromFile( const CGUIString& rFileName  )
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
	CGUIImageData* IGUIImageLoader_png::LoadFromMemory( uint8* pFileData, size_t nSize )
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

		CGUIImageData_png * pImageData = new CGUIImageData_png(this);
		uint8* pixels = pImageData->SetImageData(width, height, type);

		// since Texture2D loads the image "upside-down", there's no need
		// to flip the image here
		png_byte** row_ptrs = (png_bytep*)malloc(height * sizeof(png_bytep));
		for (png_uint_32 i=0; i<height; i++)
		{
			row_ptrs[i] = pixels + i*width*pImageData->GetBytePerPixel();
		}

		png_read_image(png_ptr, row_ptrs);	
		png_read_end( png_ptr, NULL );
		png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );

		free( row_ptrs );

		return pImageData;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy image data
	*/
	void IGUIImageLoader_png::DestroyImageData(CGUIImageData* pImageData)
	{
		delete pImageData;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief used to delete this object
	*/
	void IGUIImageLoader_png::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------

}//namespace guiex

