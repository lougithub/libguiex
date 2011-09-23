/** 
* @file guishaderimp.h
* @brief shader used in widgets
* @author ken
* @date 2011-09-23
*/

#ifndef __GUI_SHADERIMP_20110923_H__
#define __GUI_SHADERIMP_20110923_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIShader;
	class IGUIInterfaceRender;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIShaderImp
	*/
	class GUIEXPORT CGUIShaderImp
	{	
	public:
		virtual ~CGUIShaderImp();

		/**
		* @brief destroy this shader
		*/
		void Destroy();

		//!< save pointer of shader
		void SetShader( CGUIShader* pTexture);

		//!< get pointer of shader
		CGUIShader* GetShader( ) const;

	protected:
		CGUIShaderImp(IGUIInterfaceRender* pRender);

	protected:
		IGUIInterfaceRender* m_pRender;	//!< the render which create this texture
		CGUIShader* m_pShader; //!< pointer of shader class
	};

}//namespace guiex

#endif //__GUI_SHADERIMP_20110923_H__
