/** 
* @file guishader.h
* @brief shader used in widgets
* @author ken
* @date 2011-09-23
*/

#ifndef __GUI_SHADER_20110923_H__
#define __GUI_SHADER_20110923_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guiresource.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIShaderImp;
	class IGUIInterfaceRender;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIShader
	*/
	class GUIEXPORT CGUIShader : public CGUIResource
	{	
	protected:
		friend class CGUIShaderManager;
		CGUIShader( const CGUIShader&  );
		const CGUIShader& operator=(const CGUIShader& );

	public:
		virtual ~CGUIShader();

		CGUIShader* Use( IGUIInterfaceRender* pRende );

	protected:
		CGUIShader( const CGUIString& rName, const CGUIString& rSceneName,
			const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName);
		virtual int32 DoLoad();
		virtual void DoUnload();
		void NotifyDeletedFromImp();

	private:
		int32 CreateShaderImplement();
		void DestoryShaderImplement();

	private:
		friend class CGUIShaderImp;
		CGUIShaderImp* m_pShaderImp;
		CGUIString m_strVertexShaderFileName;
		CGUIString m_strFragmentShaderFileName;
	};

}//namespace guiex

#endif //__GUI_SHADER_20110923_H__
