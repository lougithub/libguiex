/** 
* @file guirender_opengles.h
* @brief use opengl to render gui
* @author ken
* @date 2010-11-09
*/


#ifndef __GUI_RENDER_OPENGLES_20101109_H__
#define __GUI_RENDER_OPENGLES_20101109_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guimatrix4.h>
#include <vector>
#include <list>
#include <set>


#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>


//============================================================================//
// define
//============================================================================// 


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITexture_opengles;
	class CGUIColor;
}




//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIRender_opengles : public IGUIInterfaceRender
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIRender_opengles();

		/**
		* @brief destructor
		*/
		virtual ~IGUIRender_opengles();

		virtual void DrawRect(const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect, 
			real fLineWidth,
			real z,
			GUIARGB rColor_topleft,
			GUIARGB rColor_topright,
			GUIARGB rColor_bottomleft,
			GUIARGB rColor_bottomright );

		/** 
		* @brief add a texture into render list
		*/
		virtual	void	DrawTile(	const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect, real z, 
			const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
			EImageOperation eImageOperation,
			GUIARGB  rColor_topleft,
			GUIARGB  rColor_topright,
			GUIARGB  rColor_bottomleft,
			GUIARGB  rColor_bottomright);

		/** 
		* @brief add a texture into render list
		*/
		virtual void	PushClipRect( const CGUIMatrix4& rMatrix, const CGUIRect& rClipRect );
		virtual void	PopClipRect( );

		// setup states etc
		virtual void	BeginRender(void);

		// restore states
		virtual void	EndRender(void);

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

		// convert CGUIColor to opengl supported format
		long	ColorToOpengl(GUIARGB col) const;

		void	makeGLMatrix(real gl_matrix[16], const CGUIMatrix4& m);


		struct SClipRect
		{
			CGUIRect m_aClipRect;
			real	m_gl_world_matrix[16];
		};

		void	UpdateStencil();
		void	RenderRectForStencil( const SClipRect& rRect );

	protected:

		struct SVertexForTile
		{
			real vertex[3];
			long color;
			real tex[2];
		};
		struct SVertexForStencil
		{
			real vertex[3];
		};
		struct SVertexForLine
		{
			real vertex[3];		
			long color;
		};

		// set the texture's coordinate
		void			SetTexCoordinate(SVertexForTile* pTexture, const CGUIRect& tex, EImageOperation eImageOperation);

	protected:
		GLint			m_maxTextureSize;		//!< maximum supported texture size (in pixels).

		//cache for system
		static const int		VERTEX_PER_TEXTURE = 4;
		static const int		VERTEX_FOR_LINE = 1024;
		
		SVertexForTile	m_pVertex[VERTEX_PER_TEXTURE];
		SVertexForStencil m_pVertexForStencil[VERTEX_PER_TEXTURE];
		SVertexForLine	m_pVertexForLine[VERTEX_FOR_LINE];
		
		real			m_gl_matrix[16];

		//texture list
		typedef		std::set<CGUITextureImp*>	TSetTexture;
		TSetTexture	m_setTexture;

		uint32					m_nCurrentTexture;

		std::vector<SClipRect>	m_arrayClipRects;

		bool					m_bWireFrame;
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_RENDER_OPENGLES_20101109_H__


