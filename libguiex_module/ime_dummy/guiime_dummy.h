/** 
* @file guiime_dummy.h
* @brief use dummy ime
* @author ken
* @date 2011-03-29
*/

#ifndef __GUI_IME_DUMMY_20110329_H__
#define __GUI_IME_DUMMY_20110329_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfaceime.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIIme_dummy
	* @brief use dummy to implement ime.
	*/
	class GUIEXPORT IGUIIme_dummy : public IGUIInterfaceIme
	{
	public:
		IGUIIme_dummy();
		virtual ~IGUIIme_dummy();

		virtual void DeleteSelf();

	public:
		virtual void OpenIme();
		virtual void CloseIme();
		virtual const wchar_t* GetResultString() const;
		virtual void ClearResultString();
		virtual void SetCursorPos( const CGUIVector2& rPos );
		virtual const CGUIVector2& GetCursorPos() const;

	protected:
		virtual int DoInitialize(void* pData);
		virtual void DoDestroy();
	
	protected:
		CGUIVector2 m_aPos; /// position of cursor

	public: 
		static const char* StaticGetModuleName();
	};



	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_IME_DUMMY_20110329_H__

