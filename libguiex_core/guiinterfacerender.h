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
#include "guirendertype.h"


//============================================================================//
// declare
//============================================================================// 

namespace guiex
{
	class CGUISize;
	class CGUIIntSize;
	class CGUIRect;
	class CGUITextureImp;
	class CGUITexture;
	class IGUIInterfaceFont;
	class CGUIMatrix4;
	class CGUIVector2;
	class CGUICamera;
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
		IGUIInterfaceRender( const char* szModuleName );

		/**
		* @brief destructor
		*/
		virtual ~IGUIInterfaceRender();

		virtual void ClearDepth (real depth) = 0;
		virtual void ClearColor(real red, real green, real blue, real alpha) = 0;
		virtual void Clear( uint32 uFlag ) = 0;
		virtual void SetDepthTest( bool bEnable ) = 0;
		virtual void SetBlendFunc( const SGUIBlendFunc& rBlendFuncType ) = 0;
		virtual void GetBlendFunc( SGUIBlendFunc& rBlendFuncType ) = 0;
		virtual void SetViewport( int32 x, int32 y, uint32 width, uint32 height) = 0;
		virtual void LookAt(real eyeX, real eyeY, real eyeZ, real lookAtX, real lookAtY, real lookAtZ, real upX, real upY, real upZ) = 0;
		virtual void Perspective(real fovy, real aspect, real zNear, real zFar) = 0;
		//virtual void DrawBuffer( EBufferMode mode ) = 0;
		//virtual void ReadBuffer( EBufferMode mode ) = 0;
		virtual void BindTexture( const CGUITexture* pTexture ) = 0;
		virtual void CopyTexSubImage2D ( int32 level, int32 xoffset, int32 yoffset, int32 x, int32 y, uint32 width, uint32 height) = 0;

		virtual void MatrixMode( EMatrixMode eMode ) = 0;
		virtual void PushMatrix() = 0;
		virtual void PopMatrix() = 0;
		virtual void MultMatrix( const CGUIMatrix4& rMatrix ) = 0;
		virtual void LoadIdentityMatrix( ) = 0;

		virtual void GenFramebuffers( uint32 n, uint32* framebuffers ) = 0;
		virtual void DeleteFramebuffers( uint32 n, const uint32* framebuffers ) = 0;
		virtual void BindFramebuffer( uint32 framebuffer ) = 0;
		virtual void GetBindingFrameBuffer( int32* framebuffer ) = 0;
		virtual void FramebufferTexture2D_Color( const CGUITexture* pTexture, int32 level ) = 0;
		virtual bool CheckFramebufferStatus( ) = 0;

		virtual void GenRenderbuffers(uint32 n, uint32* renderbuffers) = 0;
		virtual void BindRenderbuffer( uint32 renderbuffer) = 0;
		virtual void GetBindingRenderBuffer( int32* renderbuffer ) = 0;
		virtual void RenderbufferStorage_Depth( uint32 width, uint32 height) = 0;
		virtual void FramebufferRenderbuffer_Depth( uint32 renderbuffer ) = 0;
		virtual void DeleteRenderbuffers(uint32 n, const uint32* renderbuffers) = 0;

		virtual void DrawRect(
			const CGUIRect& rDestRect, 
			real fLineWidth,
			real z,
			const CGUIColor& rColor_topleft,
			const CGUIColor& rColor_topright,
			const CGUIColor& rColor_bottomleft,
			const CGUIColor& rColor_bottomright ) = 0;

		virtual void DrawRect(
			const CGUIRect& rDestRect, 
			real fLineWidth,
			real z,
			const CGUIColor& rColor );

		virtual void DrawQuads(
			const CGUITexture* pTexture,
			const SR_V2F_C4F_T2F_Quad* pQuads,
			uint16* pIndices,
			int16 nQuadNum) = 0;

		virtual void DrawGrid(
			const CGUITexture* pTexture,
			const SR_T2F* pTextures,
			const SR_V3F* pVerdices,
			uint16* pIndices,
			int16 nGridNum ) = 0;

		/** 
		* @brief add a texture into render list
		* @param rTextureRect in texture co-ordinates.
		*/
		virtual	void DrawTile(	
			const CGUIRect& rDestRect, 
			real z, 
			const CGUITexture* pTex, 
			const CGUIRect& rTextureRect,
			EImageOrientation eImageOrientation,
			const CGUIColor& rColor_topleft,
			const CGUIColor& rColor_topright,
			const CGUIColor& rColor_bottomleft,
			const CGUIColor& rColor_bottomright
			) = 0;

		virtual	void DrawTile(	
			const CGUIRect& rDestRect, 
			real z, 
			const CGUITexture* pTex, 
			const CGUIRect& rTextureRect,
			EImageOrientation eImageOrientation,
			const CGUIColor& rColor
			);

		virtual void DrawLine(
			const CGUIVector2 &rBegin, 
			const CGUIVector2 &rEnd, 
			real fLineWidth,
			real z,
			const CGUIColor& rColor_begin,
			const CGUIColor& rColor_end) = 0;

		virtual void PushClipRect( const CGUIRect& rClipRect ) = 0;
		virtual void PopClipRect( ) = 0;

		// setup states etc
		virtual void BeginRender(void) = 0;

		// restore states
		virtual void EndRender(void) = 0;

		virtual CGUICamera* ApplyCamera( CGUICamera* pCamera ) = 0;

		/**
		* @brief Destroy all textures
		*/
		virtual	void DestroyAllTexture() = 0;

		/**
		* @brief Return the current width of the display in pixels
		* @return real value equal to the current width of the display in pixels.
		*/
		virtual uint16 GetWidth(void) const	= 0;


		/**
		* @brief Return the current height of the display in pixels
		* @return real value equal to the current height of the display in pixels.
		*/
		virtual uint16 GetHeight(void) const	= 0;


		/**
		* @brief Return the maximum texture size available
		* @return Size of the maximum supported texture in pixels (textures are always assumed to be square)
		*/
		virtual	uint32 GetMaxTextureSize(void) const	= 0;

		/** 
		 * @brief toggle wire frame.
		 */
		virtual void SetWireFrame( bool bWireFrame) = 0;
		virtual bool IsWireFrame( ) const = 0;

		virtual void EnableClip( bool bEnable ) = 0;
		virtual bool IsEnableClip( ) const = 0;

		virtual void OnScreenSizeChange( const CGUIIntSize& rSize ) = 0;

	protected:
		friend class CGUITextureImp;
		friend class CGUITexture;

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
		virtual	void DestroyTexture(CGUITextureImp* texture) = 0;

	public:
		/// set font render
		IGUIInterfaceFont* GetFontRender(){ return m_pFontRender;}
		/// get font render
		void SetFontRender(IGUIInterfaceFont* pRender){ m_pFontRender = pRender;}

	private:
		IGUIInterfaceFont* m_pFontRender; //!< font render, for use it conveniently

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex


#endif __GUI_INTERFACE_RENDER_20060704_H_

