/** 
* @file guirender_ue3.h
* @brief use unreal 3 to render gui
* @author ken
* @date 2009-09-22
*/

#ifndef __GUI_RENDER_UE3_20090922_H__
#define __GUI_RENDER_UE3_20090922_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guirect.h>
#include <vector>
#include <list>
#include <set>





//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITexture_UE3;
	class CGUIColor;
}


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class IGUIRender_UE3 
	* @brief the parameter of this interface is LPDIRECT3DDEVICE9
	*/
	class GUIEXPORT IGUIRender_UE3 : public IGUIInterfaceRender
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIRender_UE3();

		/**
		* @brief destructor
		*/
		virtual ~IGUIRender_UE3();

		/** 
		* @brief add a texture into render list
		*/
		virtual	void	AddRenderTexture(	const CGUIRect& rDestRect, 
											real z, 
											const CGUITextureImp* pTexture, 
											const CGUIRect& rTextureRect, 
											EImageOperation eImageOperation,
											GUIARGB rColor_topleft,
											GUIARGB rColor_topright,
											GUIARGB rColor_bottomleft,
											GUIARGB rColor_bottomright);

		/** 
		* @brief set a clip rect
		*/
		virtual	void	AddScissor( const CGUIRect& rClipRect);

		/**
		* @brief do final render for all texture in the render list
		*/
		virtual	void	DoRender(void);

		/**
		* @brief clear render queue
		*/
		virtual	void	ClearRenderList(void);

		/**
		* @brief Enable or disable the queueing of quads from this point on.
		*	This only affects queueing.  If queueing is turned off, any calls to AddRenderTexture 
		* will cause the quad to be rendered directly.  Note that disabling queueing will not cause 
		* currently queued texture to be rendered, nor is the texture cleared - at any time the queue 
		* can still be drawn by calling doRender, and the list can be cleared by calling clearRenderList.  
		* Re-enabling the queue causes subsequent quads to be added as if queueing had never been disabled.
		*/
		virtual void	EnableRenderQueue(bool bEnable);


		/**
		* @brief Return whether queueing is enabled.
		* @return true if queueing is enabled, false if queueing is disabled.
		*/
		virtual bool	IsRenderQueueEnabled(void) const;


		/**
		* @brief Creates a 'null' Texture object.
		* @return a newly created Texture object.  The returned Texture object has no size or imagery 
		* associated with it, and is generally of little or no use.
		*/
		virtual	CGUITextureImp*	CreateTexture(void);


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
		virtual	CGUITextureImp*	CreateTexture(const CGUIString& filename);


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
		virtual	CGUITextureImp*	CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);


		/**
		* @brief Destroy the given Texture object.
		* @param texture pointer to the Texture object to be destroyed
		*/
		virtual	void		DestroyTexture(CGUITextureImp* texture);

		/**
		* @brief Destroy all textures
		*/
		virtual	void		DestroyAllTexture();

		/**
		* @brief Return the current width of the display in pixels
		* @return real value equal to the current width of the display in pixels.
		*/
		virtual uint16		GetWidth(void) const;


		/**
		* @brief Return the current height of the display in pixels
		* @return real value equal to the current height of the display in pixels.
		*/
		virtual uint16		GetHeight(void) const;


		/**
		* @brief Return the maximum texture size available
		* @return Size of the maximum supported texture in pixels (textures are always assumed to be square)
		*/
		virtual	uint32	GetMaxTextureSize(void) const;

		/** 
		* @brief toggle wire frame.
		*/
		virtual void	SetWireFrame( bool bWireFrame);

		virtual void	EnableScissor( bool bEnable );

		/**
		* @brief used to delete this object
		*/
		virtual void	DeleteSelf();

	public:
		//set canvas of unreal 3 for render everything.
		void	SetCanvas( FCanvas* pCanvas );


	protected:
		/** 
		* @brief initialize render
		* @param pData LPDIRECT3DDEVICE9, device of dx9
		* @return 0 for success
		*/
		virtual int DoInitialize(void* pData);

		/** 
		* @brief destroy render
		* @return 0 for success
		*/
		virtual void DoDestroy();


	protected:

	protected:
		bool			m_bQueueing;			//!< flag to control whether render texture by queue.
		class FCanvas*	m_pUE3Canvas;

		//texture list
		typedef		std::set<CGUITextureImp*>	TSetTexture;
		TSetTexture	m_setTexture;
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex


#endif //__GUI_RENDER_DX9_20060706_H__
