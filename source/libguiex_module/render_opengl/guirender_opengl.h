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
#include <libguiex_module/render_opengl_base/guirender_opengl_base.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guimatrix4.h>
#include <vector>
#include <list>
#include <set>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIRender_opengl : public IGUIRender_opengl_base
	{
	public:
		IGUIRender_opengl();
		virtual ~IGUIRender_opengl();

	public:	//api
		virtual void ClearDepth (real depth);
		virtual void LookAt(real eyeX, real eyeY, real eyeZ, real lookAtX, real lookAtY, real lookAtZ, real upX, real upY, real upZ);
		virtual void Perspective(real fovy, real aspect, real zNear, real zFar);

		virtual void GenFramebuffers( uint32 n, uint32* framebuffers );
		virtual void DeleteFramebuffers( uint32 n, const uint32* framebuffers );
		virtual void BindFramebuffer( uint32 framebuffer );
		virtual void GetBindingFrameBuffer( int32* framebuffer );
		virtual void FramebufferTexture2D_Color( const CGUITexture* pTexture, int32 level );
		virtual bool CheckFramebufferStatus( );

		virtual void GenRenderbuffers(uint32 n, uint32* renderbuffers);
		virtual void BindRenderbuffer( uint32 renderbuffer);
		virtual void GetBindingRenderBuffer( int32* renderbuffer );
		virtual void RenderbufferStorage_Depth( uint32 width, uint32 height);
		virtual void FramebufferRenderbuffer_Depth( uint32 renderbuffer );
		virtual void DeleteRenderbuffers(uint32 n, const uint32* renderbuffers);

		virtual void BeginRender(void);
		virtual void EndRender(void);
		
		virtual void SetWireFrame( bool bWireFrame);
		virtual bool IsWireFrame( ) const;
		
	protected:
		virtual	CGUITextureImp*	CreateTexture(void);
		virtual	CGUITextureImp*	CreateTexture(const CGUIString& filename);
		virtual	CGUITextureImp*	CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);


	protected:
		virtual int DoInitialize(void* );

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_RENDER_OPENGL_20060706_H__
