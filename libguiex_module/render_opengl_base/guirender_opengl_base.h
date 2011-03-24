/** 
* @file guirender_opengl_base.h
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/
#ifndef __GUI_RENDER_OPENGL_BASE_20060706_H__
#define __GUI_RENDER_OPENGL_BASE_20060706_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guimatrix4.h>
#include <vector>
#include <list>
#include <set>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIColor;

#if GUI_DEBUG
	extern void TryThrowOpenglError( const char* info );
# define TRY_THROW_OPENGL_ERROR(info)	TryThrowOpenglError(info)
#else
# define TRY_THROW_OPENGL_ERROR(info)	
#endif
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIRender_opengl_base : public IGUIInterfaceRender
	{
	public:
		virtual ~IGUIRender_opengl_base();

	public:
		//render api 
		virtual void ClearColor(real red, real green, real blue, real alpha);
		virtual void Clear( uint32 uFlag );
		virtual void SetDepthTest( bool bEnable );
		virtual void SetBlendFunc( const SGUIBlendFunc& rBlendFuncType );
		virtual void GetBlendFunc( SGUIBlendFunc& rBlendFuncType );
		virtual void SetViewport( int32 x, int32 y, uint32 width, uint32 height);
		virtual void PushAttrib (uint32 uFlag);
		virtual void PopAttrib ( );
		virtual void DrawBuffer( EBufferMode mode );
		virtual void ReadBuffer( EBufferMode mode );
		virtual void BindTexture( const CGUITexture* pTexture );
		virtual void CopyTexSubImage2D ( int32 level, int32 xoffset, int32 yoffset, int32 x, int32 y, uint32 width, uint32 height);

		virtual void PushMatrix();
		virtual void PopMatrix();
		virtual void MatrixMode( EMatrixMode eMode );
		virtual void LoadIdentityMatrix( );
		virtual void MultMatrix( const CGUIMatrix4& rMatrix );


	public:
		virtual void DrawRect(
			const CGUIRect& rDestRect, 
			real fLineWidth,
			real z,
			const CGUIColor& rColor_topleft,
			const CGUIColor& rColor_topright,
			const CGUIColor& rColor_bottomleft,
			const CGUIColor& rColor_bottomright );

		virtual	void DrawTile(
			const CGUIRect& rDestRect,
			real z, 
			const CGUITexture* pTexture, 
			const CGUIRect& rTextureRect, 
			EImageOrientation eImageOrientation,
			const CGUIColor& rColor_topleft,
			const CGUIColor& rColor_topright,
			const CGUIColor& rColor_bottomleft,
			const CGUIColor& rColor_bottomright);

		virtual void DrawQuads(
			const CGUITexture* pTexture,
			const SR_V2F_C4F_T2F_Quad* pQuads,
			uint16* pIndices,
			int16 nQuadNum);

		virtual void DrawGrid(
			const CGUITexture* pTexture,
			const SR_T2F* pTextures,
			const SR_V3F* pVerdices,
			uint16* pIndices,
			int16 nGridNum );

		virtual void DrawLine(
			const CGUIVector2 &rBegin, 
			const CGUIVector2 &rEnd, 
			real fLineWidth,
			real z,
			const CGUIColor& rColor_begin,
			const CGUIColor& rColor_end);

		virtual void PushClipRect( const CGUIRect& rClipRect );
		virtual void PopClipRect( );

		virtual void BeginRender(void);
		virtual void EndRender(void);

		virtual CGUICamera* ApplyCamera( CGUICamera* pCamera );

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
		IGUIRender_opengl_base( const char* szModuleName );


		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		void UpdateCamera();

		// convert CGUIColor to opengl supported format
		long ColorToOpengl( const CGUIColor& col ) const;
		void makeGLMatrix( real gl_matrix[16], const CGUIMatrix4& m );

		struct SClipRect
		{
			CGUIRect m_aClipRect;
			real m_gl_world_matrix[16];
		};

		void UpdateStencil();
		void RenderRectForStencil( const SClipRect& rRect );

		virtual	void DestroyTexture(CGUITextureImp* texture);

		void AddTexture( CGUITextureImp* pTexture );
		void RemoveTexture( CGUITextureImp* pTexture );

	protected:
		// set the texture's coordinate
		void SetTexCoordinate(SR_T2F_C4UB_V3F* pVertexInfo, CGUIRect tex, const CGUITexture* pTexture, EImageOrientation eImageOrientation);

		bool IsSupportStencil();

	protected:
		int m_maxTextureSize; //!< maximum supported texture size (in pixels).

		//cache for system
		static const int VERTEX_PER_TEXTURE = 4;
		static const int VERTEX_FOR_LINE = 1024;
		SR_T2F_C4UB_V3F m_pVertex[VERTEX_PER_TEXTURE];
		SR_V3F m_pVertexForStencil[VERTEX_PER_TEXTURE];
		SR_C4UB_V3F m_pVertexForLine[VERTEX_FOR_LINE];

		real m_gl_matrix[16];

		//texture list
		typedef std::set<CGUITextureImp*>	TSetTexture;
		TSetTexture	m_setTexture;

		std::vector<SClipRect>	m_arrayClipRects;

		bool m_bEnableClip;

		int m_nStencilBits;
		int m_nMaxStencilRef;
		int m_nCurrentStencilRef;
		SClipRect m_aWholeScreenRect;

		//current camera
		CGUICamera* m_pCamera;
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_RENDER_OPENGL_BASE_20060706_H__


