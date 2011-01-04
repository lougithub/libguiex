/** 
* @file guisounddata_dummy.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUISOUNDDATA_DUMMY_20110104_H__
#define	__KEN_GUISOUNDDATA_DUMMY_20110104_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisounddata.h>


//============================================================================//
// declare
//============================================================================//



//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUISoundData_dummy
	*/
	class GUIEXPORT CGUISoundData_dummy : public CGUISoundData
	{	
	public:
		virtual ~CGUISoundData_dummy();

	protected:
		friend class IGUISound_dummy;
		CGUISoundData_dummy( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nSoundID );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		CGUIString m_strPath; //sound file path
	};
}


#endif //__KEN_GUISOUNDDATA_DUMMY_20110104_H__
