/** 
* @file guisounddata.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/


#ifndef	__KEN_GUISOUNDDATA_20110104_H__
#define	__KEN_GUISOUNDDATA_20110104_H__

//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guistring.h"
#include "guiresource.h"


//============================================================================//
// declare
//============================================================================//



//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @class CGUISoundData
	*/
	class GUIEXPORT CGUISoundData : public CGUIResource
	{	
	public:
		virtual ~CGUISoundData();
		uint32 GetSoundID() const;

	protected:
		CGUISoundData( const CGUIString& rName, const CGUIString& rSceneName, uint32 nSoundID );

	protected:
		friend class CGUISoundManager;
		//disable =
		CGUISoundData( const CGUISoundData&  );
		const CGUISoundData& operator=(const CGUISoundData& );

	protected:
		uint32 m_nSoundID;
	};
}


#endif //__KEN_GUISOUNDDATA_20110104_H__
