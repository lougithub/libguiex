/** 
* @file guiimagedata_devil.h
* @brief image data, used by render and texture
* @author ken
* @date 2006-07-13
*/

#ifndef __GUI_IMAGEDATA_DEVIL_20060713_H__
#define __GUI_IMAGEDATA_DEVIL_20060713_H__

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
	* @class CGUIImageData_devil
	* @brief the buffer which contain the image data.
	* the format of data is RGBA, four bytes each point.
	*/
	class GUIEXPORT CGUIImageData_devil : public CGUIImageData
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUIImageData_devil();

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
		virtual uint8*	GetData();

		/**
		* @brief set image data
		* @return data pointer, NULL for failed
		*/
		uint8*	SetImageData(uint32 uWidth, uint32 uHeight );

	protected:
		/**
		* @brief constructor
		*/
		CGUIImageData_devil(IGUIInterfaceImageLoader* pLoader);

		/**
		* @brief destroy data
		*/
		void	DestroyData();

		friend class IGUIImageLoader_devil;

	protected:
		uint32		m_uWidth;
		uint32		m_uHeight;
		uint8*	m_pData;
	};

}//namespace guiex



#endif //__GUI_IMAGEDATA_DEVIL_20060713_H__

