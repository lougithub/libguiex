/** 
* @file guiinterfacerender.h
* @brief interface for render
* @author ken
* @date 2006-07-04
*/


#ifndef __GUI_INTERFACE_RENDER_20060704_H_
#define __GUI_INTERFACE_RENDER_20060704_H_

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"
#include "guicolor.h"


//============================================================================//
// declare
//============================================================================// 

namespace guiex
{
	class CGUISize;
	class CGUIRect;
	class CGUITextureImp;
	class IGUIInterfaceFont;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIInterfaceRender 
	* @brief interface of render
	*/
	class GUIEXPORT IGUIInterfaceRender : public IGUIInterface
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIInterfaceRender();

		/**
		* @brief destructor
		*/
		virtual ~IGUIInterfaceRender();

		/** 
		* @brief add a texture into render list
		* @param rTextureRect in texture co-ordinates.
		*/
		virtual	void	AddRenderTexture(	const CGUIRect& rDestRect, 
			real z, 
			const CGUITextureImp* pTex, 
			const CGUIRect& rTextureRect,
			EImageOperation eImageOperation,
			GUIARGB rColor_topleft,
			GUIARGB rColor_topright,
			GUIARGB rColor_bottomleft,
			GUIARGB rColor_bottomright
			) = 0;

		/** 
		* @brief set a clip rect
		*/
		virtual	void	AddScissor( const CGUIRect& rClipRect) = 0;

		/**
		* @brief do final render for all texture in the render list
		*/
		virtual	void	DoRender(void) = 0;

		/**
		* @brief clear render queue
		*/
		virtual	void	ClearRenderList(void) = 0;

		/**
		* @brief Enable or disable the queueing of quads from this point on.
		*	This only affects queueing.  If queueing is turned off, any calls to AddRenderTexture 
		* will cause the quad to be rendered directly.  Note that disabling queueing will not cause 
		* currently queued texture to be rendered, nor is the texture cleared - at any time the queue 
		* can still be drawn by calling doRender, and the list can be cleared by calling clearRenderList.  
		* Re-enabling the queue causes subsequent quads to be added as if queueing had never been disabled.
		*/
		virtual void	EnableRenderQueue(bool bEnable) = 0;


		/**
		* @brief Return whether queueing is enabled.
		* @return true if queueing is enabled, false if queueing is disabled.
		*/
		virtual bool	IsRenderQueueEnabled(void) const = 0;


		/**
		* @brief Creates a 'null' Texture object.
		* @return a newly created Texture object.  The returned Texture object has no size or imagery 
		* associated with it, and is generally of little or no use.
		*/
		virtual	CGUITextureImp*	CreateTexture(void) = 0;


		/**
		* @brief Create a Texture object using the given image file.
		* @param filename String object that specifies the path and filename of the image file to use 
		* when creating the texture.
		* return a newly created Texture object.  The initial contents of the texture memory is the 
		* requested image file.
		* @note Textures are always created with a size that is a power of 2.  If the file you specify 
		* is of a size that is not a power of two, the final size will be rounded up.  Additionally, 
		* textures are always square, so the ultimate size is governed by the larger of the width and 
		* height of the specified file.  You can check the ultimate sizes by querying the texture after creation.
		*/
		virtual	CGUITextureImp*	CreateTexture(const CGUIString& filename) = 0;


		/** 
		* @brief Create a Texture object with the given pixel dimensions as specified by \a size.  
		* NB: Textures are always square.
		* param size real value that specifies the size used for the width and height when creating 
		* the new texture.
		* @return a newly created Texture object.  The initial contents of the texture memory is 
		* undefined / random.
		* note Textures are always created with a size that is a power of 2.  If you specify a size that 
		* is not a power of two, the final size will be rounded up.  So if you specify a size of 1024, the 
		* texture will be (1024 x 1024), however, if you specify a size of 1025, the texture will be 
		* (2048 x 2048).  You can check the ultimate size by querying the texture after creation.
		*/	
		virtual	CGUITextureImp*	CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat) = 0;


		/**
		* @brief Destroy the given Texture object.
		* @param texture pointer to the Texture object to be destroyed
		*/
		virtual	void		DestroyTexture(CGUITextureImp* texture) = 0;

		/**
		* @brief Destroy all textures
		*/
		virtual	void		DestroyAllTexture() = 0;

		/**
		* @brief Return the current width of the display in pixels
		* @return real value equal to the current width of the display in pixels.
		*/
		virtual uint16		GetWidth(void) const	= 0;


		/**
		* @brief Return the current height of the display in pixels
		* @return real value equal to the current height of the display in pixels.
		*/
		virtual uint16		GetHeight(void) const	= 0;


		/**
		* @brief Return the maximum texture size available
		* @return Size of the maximum supported texture in pixels (textures are always assumed to be square)
		*/
		virtual	uint32	GetMaxTextureSize(void) const	= 0;

		/** 
		 * @brief toggle wire frame.
		 */
		virtual void	SetWireFrame( bool bWireFrame) = 0;

		virtual void	EnableScissor( bool bEnable ) = 0;

	public:
		/**
		* @brief Reset the z co-ordinate for rendering.
		*/
		void	ResetZValue(void);

		/**
		* @brief Update the z co-ordinate for the next major UI element (window).
		*/
		void	IncreaseZValue(void);

		/**
		* @brief return the current Z value to use (equates to layer 0 for this UI element).
		* @return real value that specifies the z co-ordinate to be used for layer 0 on the current GUI element.
		*/
		real	GetCurrentZ(void) const			{return m_current_z;}

		/**
		* @brief return the current Z value to use and then increase it.
		*/
		real	GetAndIncZ();


	public:
		/// set font render
		IGUIInterfaceFont*		GetFontRender(){ return m_pFontRender;}
		/// get font render
		void	SetFontRender(IGUIInterfaceFont* pRender){ m_pFontRender = pRender;}

	private:
		IGUIInterfaceFont*		m_pFontRender;			//!< font render, for use it conveniently

		static const real	ms_ZInitialValue;		//!< Initial value to use for 'z' each frame.
		static const real	ms_ZElementStep;		//!< Value to step 'z' for each GUI element.


		real				m_current_z;		//!< The current z co-ordinate value.

	};



}//namespace guiex


#endif __GUI_INTERFACE_RENDER_20060704_H_

