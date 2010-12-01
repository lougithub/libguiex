/** 
* @file guiimagedata_tga.h
* @brief image data, used by render and texture
* @author ken
* @date 2010-11-10
*/

#ifndef __GUI_IMAGEDATA_TGA_20101110_H__
#define __GUI_IMAGEDATA_TGA_20101110_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiimagedata.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIImageData_tga
	* @brief the buffer which contain the image data.
	* the format of data is RGBA, four bytes each point.
	*/
	class GUIEXPORT CGUIImageData_tga : public CGUIImageData
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUIImageData_tga();

		/**
		* @brief get width of image
		*/
		virtual uint32 GetWidth();

		/**
		* @brief get height of image
		*/
		virtual uint32 GetHeight();

		/**
		* @brief get data of image
		*/
		virtual uint8* GetData();

		/**
		* @brief set image data
		* @return data pointer, NULL for failed
		*/
		uint8* SetImageData(uint32 uWidth, uint32 uHeight, EGuiPixelFormat ePixelFormat );

	protected:
		/**
		* @brief constructor
		*/
		CGUIImageData_tga(IGUIInterfaceImageLoader* pLoader);

		/**
		* @brief destroy data
		*/
		void DestroyData();

		friend class IGUIImageLoader_tga;

	protected:
		uint32 m_uWidth;
		uint32 m_uHeight;
		uint8 *m_pData;
	};

}//namespace guiex



#endif //__GUI_IMAGEDATA_TGA_20101110_H__

