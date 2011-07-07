/** 
* @file guiimagedata_png.h
* @brief image data, used by render and texture
* @author ken
* @date 2011-07-06
*/

#ifndef __GUI_IMAGEDATA_PNG_20110706_H__
#define __GUI_IMAGEDATA_PNG_20110706_H__

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
	* @class CGUIImageData_png
	* @brief the buffer which contain the image data.
	* the format of data is RGBA, four bytes each point.
	*/
	class GUIEXPORT CGUIImageData_png : public CGUIImageData
	{
	public:
		virtual ~CGUIImageData_png();

		virtual uint32 GetWidth();
		virtual uint32 GetHeight();
		virtual uint8* GetData();

		uint8* SetImageData(uint32 uWidth, uint32 uHeight, EGuiPixelFormat ePixelFormat );

	protected:
		CGUIImageData_png(IGUIInterfaceImageLoader* pLoader);

		void DestroyData();

		friend class IGUIImageLoader_png;

	protected:
		uint32 m_uWidth;
		uint32 m_uHeight;
		uint8 *m_pData;
	};

}//namespace guiex



#endif //__GUI_IMAGEDATA_PNG_20110706_H__

