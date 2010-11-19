/** 
* @file guifont.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-27
*/






#ifndef	__KEN_GUIFONT_20091027_H__
#define	__KEN_GUIFONT_20091027_H__





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
	protected:
		friend class CGUIFontManager;
		//disable =
		CGUIFontData( const CGUIFontData&  );
		const CGUIFontData& operator=(const CGUIFontData& );

	public:
		/**
		* @brief destructor;
		*/
		virtual ~CGUIFontData();

	protected:
		/**
		* @brief constructor
		*/
		CGUIFontData( 		
			const CGUIString& rName,
			const CGUIString& rSceneName, 
			const CGUIString& rPath,
			uint32	nFontIndex);

		virtual int32	DoLoad() const;
		virtual void	DoUnload();


	private:
		uint32	m_nFontIndex;
		CGUIString m_strPath;
	};
}




#endif //__KEN_GUIFONT_20091027_H__
