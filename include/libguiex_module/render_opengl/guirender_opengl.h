/** 
* @file guirender_opengl.h
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/


#ifndef __GUI_RENDER_OPENGL_20060706_H__
#define __GUI_RENDER_OPENGL_20060706_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiinterfacerender.h>
#include <libguiex_core\guirect.h>
#include <vector>
#include <list>
#include <set>


#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#endif

#pragma pack(push,8)
#include <GL/gl.h>
#include <GL/glu.h>
#pragma pack(pop)




//============================================================================//
// define
//============================================================================// 


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITexture_opengl;
	class CGUIColor;
}




//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIRender_opengl : public IGUIInterfaceRender
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIRender_opengl();

		/**
		* @brief destructor
		*/
		virtual ~IGUIRender_opengl();

		/** 
		* @brief add a texture into render list
		*/
		virtual	void	AddRenderTexture(	const CGUIRect& rDestRect, real z, 
			const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
			EImageOperation eImageOperation,
			GUIARGB  rColor_topleft,
			GUIARGB  rColor_topright,
			GUIARGB  rColor_bottomleft,
			GUIARGB  rColor_bottomright);

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
		virtual uint16	GetWidth(void) const;


		/**
		* @brief Return the current height of the display in pixels
		* @return real value equal to the current height of the display in pixels.
		*/
		virtual uint16	GetHeight(void) const;


		/**
		* @brief Return the maximum texture size available
		* @return Size of the maximum supported texture in pixels (textures are always assumed to be square)
		*/
		virtual	uint32	GetMaxTextureSize(void) const;


		/**
		* @brief Return the horizontal display resolution dpi
		* @return horizontal resolution of the display in dpi.
		*/
		virtual	uint32	GetHorzScreenDPI(void) const;


		/**
		* @brief Return the vertical display resolution dpi
		* @return vertical resolution of the display in dpi.
		*/
		virtual	uint32	GetVertScreenDPI(void) const;

		/** 
		* @brief toggle wire frame.
		*/
		virtual void	SetWireFrame( bool bWireFrame);

		virtual void	EnableScissor( bool bEnable );

		/**
		* @brief used to delete this object
		*/
		virtual void	DeleteSelf();

	protected:
		/** 
		* @brief initialize render
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy render
		* @return 0 for success
		*/
		virtual void DoDestroy();

		/**
		* @brief render a texture directly to the display
		*/
		void	RenderTextureDirect(const CGUIRect& rDestRect, real z, 
			const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
			EImageOperation eImageOperation, 
			GUIARGB  rColor_topleft,
			GUIARGB  rColor_topright,
			GUIARGB  rColor_bottomleft,
			GUIARGB  rColor_bottomright);

		// setup states etc
		void	BeginRender(void);

		// restore states
		void	EndRender(void);

		// render buffer
		void	RenderVBuffer(void);

		// convert CGUIColor to opengl supported format
		long	ColorToOpengl(GUIARGB col) const;

		//allocate a node
		struct STextureInfo;
		STextureInfo*	AllocateNode();

	protected:

		struct SVertex
		{
			real tex[2];
			long color;
			real vertex[3];
		};

		enum	ERenderCommand
		{
			eCommand_None = 0,			//
			eCommand_SetTexture,		//<para1:texture id>
			eCommand_SetScissor,		//<para1:x> <para2:y> <para3:width> <para4:height>
			eCommand_DrawVertex,		//<para1:start position>	<para2: count>
		};
		struct SRenderCommand 
		{
			uint32	m_nID;
			uint32	m_nPara1;
			uint32	m_nPara2;
			uint32	m_nPara3;
			uint32	m_nPara4;
		};
		struct SClipRect 
		{
			GLint x;
			GLint y;
			GLint width;
			GLint height;

			SClipRect()
				:x(0),y(0),width(0),height(0)
			{

			}
		};

		// set the texture's coordinate
		void			SetTexCoordinate(SVertex* pTexture, const CGUIRect& tex, EImageOperation eImageOperation);

		//allocate command node
		SRenderCommand* AllocateCommandNode(ERenderCommand eCommand);
		SRenderCommand* GetLastCommandNode();

		//allocate vertex node
		SVertex*		AllocateVertexNode(uint32 nNum);

	protected:
		static const int		VERTEX_PER_TEXTURE = 6;
		static const int		OPENGL_RENDERER_VBUFF_CAPACITY = 1024;

		
		GLint			m_maxTextureSize;		//!< maximum supported texture size (in pixels).
		bool			m_bQueueing;			//!< flag to control whether render texture by queue.

		//command list
		typedef			std::vector<SRenderCommand>	TListCommand;
		TListCommand	m_vecCommand;
		uint32			m_nCommandIdx;

		//vertex list
		typedef			std::vector<SVertex>	TListVertex;
		TListVertex		m_vecVertex;
		uint32			m_nVertexIdx;

		//texture list
		typedef		std::set<CGUITextureImp*>	TSetTexture;
		TSetTexture	m_setTexture;

		ERenderCommand			m_eLastCommand;
		SClipRect				m_aCurrentClipRect;
		uint32					m_nCurrentTexture;
		uint32					m_nCurrentVertexIdx;

		bool					m_bWireFrame;
		bool					m_bEnableScissor;
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_RENDER_OPENGL_20060706_H__


