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
	extern void TryThrowOpenglError( const char *file, int line );
#	define TRY_THROW_OPENGL_ERROR( )	TryThrowOpenglError(__FILE__, __LINE__)
#else
#	define TRY_THROW_OPENGL_ERROR( )	
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

		virtual void OnPostRegisterInterface();
	public:
		//render api 
		virtual void SetClearColor(real red, real green, real blue, real alpha);
		virtual void ClearColor(real red, real green, real blue, real alpha);
		virtual void Clear( uint32 uFlag );
		virtual void SetDepthTest( bool bEnable );
		virtual void SetBlendFunc( const SGUIBlendFunc& rBlendFuncType );
		virtual void GetBlendFunc( SGUIBlendFunc& rBlendFuncType );
		virtual void SetViewport( int32 x, int32 y, uint32 width, uint32 height);
		//virtual void DrawBuffer( EBufferMode mode );
		//virtual void ReadBuffer( EBufferMode mode );
		virtual void BindTexture( const CGUITexture* pTexture );
		virtual void CopyTexSubImage2D ( int32 level, int32 xoffset, int32 yoffset, int32 x, int32 y, uint32 width, uint32 height);

		virtual void PushMatrix();
		virtual void PopMatrix();
		virtual void MatrixMode( EMatrixMode eMode );
		virtual void LoadIdentity( );
		virtual void MultMatrix( const CGUIMatrix4& rMatrix );

		virtual void LookAt(real eyeX, real eyeY, real eyeZ, real lookAtX, real lookAtY, real lookAtZ, real upX, real upY, real upZ);
		virtual void Perspective(real fovy, real aspect, real zNear, real zFar);

	public:
		virtual void DrawPoint(
			const CGUIVector2 &rPoint, 
			real fPointSize,
			real z,
			const CGUIColor& rColor);

		virtual void DrawRect(
			const CGUIRect& rDestRect, 
			real fLineWidth,
			bool bSolid,
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
			const SVertexFormat_V2F_T2F_C4UB_Quad* pQuads,
			uint16* pIndices,
			int16 nQuadNum);

		virtual void DrawPolygon(
			const SVertexFormat_V3F_C4UB* pVertex,
			int16 nVertexNum,
			real fLineWidth,
			bool bSolid	);

		virtual void DrawGrid(
			const CGUITexture* pTexture,
			const SVertexFormat_T2F_C4UB* pVerticeInfos,
			const SVertexFormat_V3F* pVerdices,
			uint16* pIndices,
			int16 nGridNum );

		virtual void DrawLine(
			const CGUIVector2 &rBegin, 
			const CGUIVector2 &rEnd, 
			real fLineWidth,
			real z,
			const CGUIColor& rColor_begin,
			const CGUIColor& rColor_end);
		
		virtual void DrawCircle(
			const CGUIVector2& rCenter,
			real fRadius,
			real fLineWidth,
			bool bSolid,
			real z,
			const CGUIColor& rColor );

		virtual void PushClipRect( const CGUIRect& rClipRect );
		virtual void PopClipRect( );

		virtual void BeginRender(void);
		virtual void EndRender(void);

		virtual CGUICamera* ApplyCamera( CGUICamera* pCamera );

		virtual uint16 GetWidth(void) const;
		virtual uint16 GetHeight(void) const;

		virtual	uint32 GetMaxTextureSize(void) const;

		virtual	uint32 GetHorzScreenDPI(void) const;
		virtual	uint32 GetVertScreenDPI(void) const;

		virtual void EnableClip( bool bEnable );
		virtual bool IsEnableClip( ) const;

		virtual void DeleteSelf();

		virtual void OnScreenSizeChange( const CGUIIntSize& rSize );

		virtual void SetWireFrame( bool bWireFrame);
		virtual bool IsWireFrame( ) const;

		virtual uint32 GUIColorToRenderColor( const CGUIColor& col ) const;

		virtual CGUIShader* UseShader( CGUIShader* pShader );

	protected:
		IGUIRender_opengl_base( const char* szModuleName );

		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		virtual	CGUITextureImp*	CreateTexture(void);
		virtual	CGUITextureImp*	CreateTexture(const CGUIString& filename);
		virtual	CGUITextureImp*	CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);
		virtual	void DestroyTexture(CGUITextureImp* texture);
		virtual	void DestroyAllTexture();

		void UpdateCamera();

		void GLMultMatrix( const real* m );
		void GLMultMatrix( const real* a, const real* b, real* out );
		void makeGLMatrix( real gl_matrix[16], const CGUIMatrix4& m );

		struct SClipRect
		{
			CGUIRect m_aClipRect;
			real m_gl_world_matrix[16];
		};
		void UpdateStencil();
		void RenderRectForStencil( const SClipRect& rRect );

		void AddTexture( CGUITextureImp* pTexture );
		void RemoveTexture( CGUITextureImp* pTexture );

		void AddShader( CGUIShaderImp* pShader );
		void RemoveShader( CGUIShaderImp* pShader );
		virtual	void DestroyAllShader();

		void DrawStencil( uint32 uMode, const SVertexFormat_V3F* pVertexBuf, uint32 uVertexNum );
		void DrawPrimitive( uint32 eMode, const SVertexFormat_V3F_C4UB* pVertexBuf, uint32 uVertexNum );
		void DrawPrimitive( uint32 uMode, const SVertexFormat_V3F_T2F_C4UB* pVertexBuf, uint32 uVertexNum );
		void DrawIndexedPrimitive( uint32 uMode, const SVertexFormat_V2F_T2F_C4UB* pVertexBuf, uint16* pIndicesBuf, uint32 uIndexNum );
		void DrawIndexedPrimitive( uint32 uMode, const SVertexFormat_V3F* pVerdiceBuf, const SVertexFormat_T2F_C4UB* pVerticeInfos, uint16* pIndicesBuf, uint32 uIndexNum );
		
		void SetShaderMatrix();
		void DrawStencil_Shader( uint32 uMode, const SVertexFormat_V3F* pVertexBuf, uint32 uVertexNum );
		void DrawPrimitive_Shader( uint32 eMode, const SVertexFormat_V3F_C4UB* pVertexBuf, uint32 uVertexNum );
		void DrawPrimitive_Shader( uint32 uMode, const SVertexFormat_V3F_T2F_C4UB* pVertexBuf, uint32 uVertexNum );
		void DrawIndexedPrimitive_Shader( uint32 uMode, const SVertexFormat_V2F_T2F_C4UB* pVertexBuf, uint16* pIndicesBuf, uint32 uIndexNum );
		void DrawIndexedPrimitive_Shader( uint32 uMode, const SVertexFormat_V3F* pVerdiceBuf, const SVertexFormat_T2F_C4UB* pVerticeInfos, uint16* pIndicesBuf, uint32 uIndexNum );
		
		void SetPipelineMatrix();
		void DrawStencil_Pipeline( uint32 uMode, const SVertexFormat_V3F* pVertexBuf, uint32 uVertexNum );
		void DrawPrimitive_Pipeline( uint32 eMode, const SVertexFormat_V3F_C4UB* pVertexBuf, uint32 uVertexNum );
		void DrawPrimitive_Pipeline( uint32 uMode, const SVertexFormat_V3F_T2F_C4UB* pVertexBuf, uint32 uVertexNum );
		void DrawIndexedPrimitive_Pipeline( uint32 uMode, const SVertexFormat_V2F_T2F_C4UB* pVertexBuf, uint16* pIndicesBuf, uint32 uIndexNum );
		void DrawIndexedPrimitive_Pipeline( uint32 uMode, const SVertexFormat_V3F* pVerdiceBuf, const SVertexFormat_T2F_C4UB* pVerticeInfos, uint16* pIndicesBuf, uint32 uIndexNum );


		virtual	CGUIShaderImp* CreateShader(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName);
		virtual	void DestroyShader(CGUIShaderImp* shader);

protected:
		// set the texture's coordinate
		void SetTexCoordinate(SVertexFormat_V3F_T2F_C4UB* pVertexInfo, CGUIRect tex, const CGUITexture* pTexture, EImageOrientation eImageOrientation);

		bool IsSupportStencil();

	protected:
		CGUIColor m_aClearColor;

		int m_maxTextureSize; //!< maximum supported texture size (in pixels).

		//cache for system
		static const int VERTEX_PER_TEXTURE = 4;
		static const int VERTEX_FOR_LINE = 4;
		static const int VERTEX_FOR_CIRCLE = 360;
		SVertexFormat_V3F_T2F_C4UB m_pVertex[VERTEX_PER_TEXTURE];
		SVertexFormat_V3F m_pVertexForStencil[VERTEX_PER_TEXTURE];
		SVertexFormat_V3F_C4UB m_pVertexForLine[VERTEX_FOR_LINE];
		SVertexFormat_V3F_C4UB m_pVertexForCircle[VERTEX_FOR_CIRCLE];

		//matrix stack
		struct SMatrixInfo
		{
			real m_matrix[2][16];
		};
		std::vector<SMatrixInfo> m_vecMatrixStack;
		EMatrixMode m_eMatrixMode;

		//texture list
		typedef std::set<CGUITextureImp*> TSetTexture;
		TSetTexture	m_setTexture;
		CGUITexture* m_pTextureForLine; //used to draw line in shader

		//shader list
		typedef std::set<CGUIShaderImp*> TSetShader;
		TSetShader	m_setShader;
		class CGUIShader_opengl_base* m_pCurrentShader;

		//stencil buffer for clip
		int m_nStencilBits;
		uint32 m_nMaxStencilRef;
		uint32 m_nCurrentStencilRef;
		bool m_bForceRefreshStencil;
		bool m_bHasClipRectOp;
		std::vector<SClipRect>	m_arrayClipRects;
		SClipRect m_aWholeScreenRect;
		struct SClipRectOp
		{
			SClipRect m_aClipRect;
			enum EOp
			{
				eClipRectOp_Add,
				eClipRectOp_Remove,
			};
			EOp m_eClipOp;
		};
		std::vector<SClipRectOp> m_arrayClipRectOps;
		std::vector<SClipRectOp*> m_arrayClipRectOpsCache;


		bool m_bEnableClip;
		bool m_bDrawWireframe;

		uint32 m_nRenderMode_TRIANGLE_STRIP;
		uint32 m_nRenderMode_TRIANGLES;

		SGUIBlendFunc m_aBlendFunc;

		//current camera
		CGUICamera* m_pCamera;
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_RENDER_OPENGL_BASE_20060706_H__


