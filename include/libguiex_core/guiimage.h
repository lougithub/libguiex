/** 
* @file guiimage.h
* @brief image used in widgets
* @author ken
* @date 2006-07-17
*/

#ifndef __GUI_IMAGE_20060717_H__
#define __GUI_IMAGE_20060717_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guicolorrect.h"
#include "guisize.h"
#include "guirect.h"
#include "guiresource.h"

//============================================================================//
// declare
//============================================================================// 

namespace guiex
{
	class CGUITexture;
	class IGUIInterfaceRender;
	class CGUIRect;
	class CGUIRenderRect;
	class CGUIMatrix4;
}

//============================================================================//
// class
//============================================================================// 


namespace guiex
{
	/**
	* @class CGUIImage
	* @brief image used by widgets
	* one image correspond to one texture
	*/
	class GUIEXPORT CGUIImage : public CGUIResource
	{	
	protected:
		friend class CGUIImageManager;
		//disable =
		CGUIImage( const CGUIImage&  );
		const CGUIImage& operator=(const CGUIImage& );

	public:
		/**
		* @brief destructor;
		*/
		virtual ~CGUIImage();

		void	Draw( IGUIInterfaceRender* pRender,
			const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect,
			real z, 
			const CGUIColorRect& rColorRect,
			real fAlpha);

		void	Draw(IGUIInterfaceRender* pRender,
			const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect,
			real z, 
			real fAlpha	);

		void	Draw( IGUIInterfaceRender* pRender,
			const CGUIMatrix4& rWorldMatrix,
			const CGUIRect& rDestRect,
			real z, 
			const CGUIColor& rColor,
			real fAlpha	);

		//void	Draw(IGUIInterfaceRender* pRender,
		//	const CGUIRenderRect& rRenderRect,
		//	real z, 
		//	real fAlpha	);

		/**
		* @brief get image size
		*/
		CGUISize		GetSize();

	protected:
		/**
		* @brief constructor
		*/
		CGUIImage( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIColor& rColor );

		CGUIImage( 
			const CGUIString& rName,
			const CGUIString& rSceneName, 
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat );

		CGUIImage( 		
			const CGUIString& rName,
			const CGUIString& rSceneName, 
			const CGUIString& rPath,
			const CGUIRect& rUVRect,
			EImageOperation	eImageOperation);

		virtual int32	DoLoad();
		virtual void	DoUnload();


	private:
		//parameter about texture
		CGUITexture*	m_pTexture;		//!< texture
		CGUIRect		m_aUVRect;		//!< UV of texture
		EImageOperation	m_eImageOperation;	//!<

		/** 
		* @brief the image type, means where the image load from
		*/
		enum	EImageType
		{
			eIT_FILE,
			eIT_COLOR,
			eIT_MEM,
		};
		EImageType		m_eImageType;

		CGUIString		m_strPath;		//!< for eIT_FILE

		CGUIColor		m_aColor;		//!< for eIT_COLOR

		const void*		m_pBuffPtr;		//!< for eIT_MEM
		int32			m_nBuffWidth;
		int32			m_nBuffHeight; 
		EGuiPixelFormat m_ePixelFormat;

		//uint16			m_nRefCount;	//!< reference count
	};

}//namespace guiex

#endif		//__GUI_IMAGE_20060717_H__
