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
		IGUIRender_opengles( );
		virtual ~IGUIRender_opengles();

		virtual void DrawRect(const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect, 
			real fLineWidth,
			real z,
			GUIARGB rColor_topleft,
			GUIARGB rColor_topright,
			GUIARGB rColor_bottomleft,
			GUIARGB rColor_bottomright );


		virtual	void DrawTile( const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect,
			real z, 
			const CGUITextureImp* pTexture, 
			const CGUIRect& rTextureRect, 
			EImageOrientation eImageOrientation,
			GUIARGB rColor_topleft,
			GUIARGB rColor_topright,
			GUIARGB rColor_bottomleft,
			GUIARGB rColor_bottomright);

		virtual void DrawLine(const CGUIMatrix4& rWorldMatrix,
			const CGUIVector2 &rBegin, 
			const CGUIVector2 &rEnd, 
			real fLineWidth,
			real z,
			GUIARGB rColor_begin,
			GUIARGB rColor_end);

		virtual void PushClipRect( const CGUIMatrix4& rMatrix, const CGUIRect& rClipRect );
		virtual void PopClipRect( );

		virtual void BeginRender(void);
		virtual void EndRender(void);

		virtual CGUICamera* ApplyCamera( CGUICamera* pCamera );

		virtual	CGUITextureImp*	CreateTexture(void);
		virtual	CGUITextureImp*	CreateTexture(const CGUIString& filename);
		virtual	CGUITextureImp*	CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);
		virtual	void DestroyTexture(CGUITextureImp* texture);
		virtual	void DestroyAllTexture();

		virtual uint16 GetWidth(void) const;
		virtual uint16 GetHeight(void) const;

		virtual	uint32 GetMaxTextureSize(void) const;

		virtual	uint32 GetHorzScreenDPI(void) const;
		virtual	uint32 GetVertScreenDPI(void) const;

		virtual void SetWireFrame( bool bWireFrame);
		virtual bool IsWireFrame( ) const;
		virtual void EnableClip( bool bEnable );
		virtual bool IsEnableClip( ) const;

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		void UpdateCamera();

		/**
		* @brief render a texture directly to the display
		*/
		void RenderTextureDirect(
			const CGUIRect& rDestRect, real z, 
			const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
			EImageOrientation eImageOrientation, 
			GUIARGB  rColor_topleft,
			GUIARGB  rColor_topright,
			GUIARGB  rColor_bottomleft,
			GUIARGB  rColor_bottomright);

		// convert CGUIColor to opengl supported format
		long ColorToOpengl(GUIARGB col) const;

		void makeGLMatrix(real gl_matrix[16], const CGUIMatrix4& m);

		struct SClipRect
		{
			CGUIRect m_aClipRect;
			real	m_gl_world_matrix[16];
		};

		void UpdateStencil();
		void RenderRectForStencil( const SClipRect& rRect );
		
		/** OpenGL gluLookAt implementation */
		void gluLookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
		/** OpenGL gluPerspective implementation */
		void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
		

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
		void			SetTexCoordinate(SVertexForTile* pTexture, const CGUIRect& tex, EImageOrientation eImageOrientation);

		bool IsSupportStencil();

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

		bool m_bWireFrame;
		bool m_bEnableClip;

		int m_nStencilBits;
		int m_nMaxStencilRef;
		int m_nCurrentStencilRef;
		SClipRect m_aWholeScreenRect;
		
		CGUICamera* m_pCamera;
		
	public: 
		static const char* StaticGetModuleName();		
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_RENDER_OPENGLES_20101109_H__


