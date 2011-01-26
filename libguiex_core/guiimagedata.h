/** 
* @file guiimagedata.h
* @brief image data, used by render and texture
* @author ken
* @date 2006-07-13
*/

#ifndef __GUI_IMAGEDATA_20060713_H__
#define __GUI_IMAGEDATA_20060713_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
//============================================================================//
// declare
//============================================================================// 
namespace guiex
{

	class IGUIInterfaceImageLoader;

}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIImageData
	* @brief the buffer which contain the image data.
	* the format of data is RGBA, four bytes each point.
	*/
	class GUIEXPORT CGUIImageData
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUIImageData();

		/**
		* @brief get width of image
		*/
		virtual uint32 GetWidth() = 0;

		/**
		* @brief get height of image
		*/
		virtual uint32 GetHeight() = 0;

		/**
		* @brief get data of image
		*/
		virtual uint8*	GetData() = 0;

		/**
		* @brief set pixel format
		*/
		void	SetPixelFormat(EGuiPixelFormat ePixelFormat)
		{
			m_ePixelFormat = ePixelFormat;
		}

		/**
		* @brief get pixel format
		*/
		EGuiPixelFormat	GetPixelFormat()
		{
			return m_ePixelFormat;
		}

		static uint32 GetBytePerPixel( EGuiPixelFormat ePixelFormat );


		/**
		* @brief release this data
		*/
		void	Release();

	protected:
		/**
		* @brief constructor
		*/
		CGUIImageData(IGUIInterfaceImageLoader* pLoader);

	private:
		IGUIInterfaceImageLoader*		m_pLoader;

		EGuiPixelFormat					m_ePixelFormat;
	};

}//namespace guiex



#endif //__GUI_IMAGEDATA_20060713_H__

