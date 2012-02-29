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
		CGUIImageData(IGUIInterfaceImageLoader* pLoader);
		~CGUIImageData();
		
		uint8* SetImageData(uint32 uWidth, uint32 uHeight, EGuiPixelFormat ePixelFormat );
		uint32 GetDataWidth();
		uint32 GetDataHeight();
		uint32 GetImageWidth();
		uint32 GetImageHeight();
		uint8* GetData();
		uint32 GetBytePerPixel() const;
		EGuiPixelFormat	GetPixelFormat();
		static uint32 GetBytePerPixel( EGuiPixelFormat ePixelFormat );

		void Release();


	protected:
		void DestroyData();

	private:
		IGUIInterfaceImageLoader* m_pLoader;

		uint32 m_uImageWidth;
		uint32 m_uImageHeight;
		uint32 m_uDataWidth;
		uint32 m_uDataHeight;
		uint8 *m_pData;
		EGuiPixelFormat m_ePixelFormat;
	};

}//namespace guiex



#endif //__GUI_IMAGEDATA_20060713_H__

