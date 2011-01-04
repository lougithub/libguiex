/** 
* @file guifontdata.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-27
*/


#ifndef	__KEN_GUIFONTDATA_20091027_H__
#define	__KEN_GUIFONTDATA_20091027_H__

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
	* @class CGUIFontData
	*/
	class GUIEXPORT CGUIFontData : public CGUIResource
	{	
	public:
		virtual ~CGUIFontData();

		uint32 GetFontIndex() const;

	protected:
		CGUIFontData( const CGUIString& rName, const CGUIString& rSceneName, uint32 nFontID );

	protected:
		friend class CGUIFontManager;
		//disable =
		CGUIFontData( const CGUIFontData&  );
		const CGUIFontData& operator=(const CGUIFontData& );

	protected:
		uint32 m_nFontIndex;
	};
}


#endif //__KEN_GUIFONTDATA_20091027_H__
