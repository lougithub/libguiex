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
#include "guiintsize.h"
#include "guicolor.h"


//============================================================================//
// declare
//============================================================================//


//============================================================================//
// class
//============================================================================//

namespace guiex
{
	/**
	* @desc font info
	*/
	struct SFontInfo
	{
		uint16 m_uID;
		CGUIString m_strPath;
		uint16 m_uSize;
		CGUIString m_strDesc;
		CGUIIntSize m_aTextureSize;
		
		bool m_bUseMono;

		bool m_bHasOutline;
		uint16 m_uOutlineWidth;
		CGUIColor m_aOutlineColor;
		CGUIColor m_aFontColor;

		SFontInfo()
			:m_uID(0)
			,m_uSize(16)
			,m_aTextureSize(256,256)
			,m_bUseMono(false)
			,m_bHasOutline(false)
			,m_uOutlineWidth(3)
			,m_aOutlineColor(0,0,0,1)
			,m_aFontColor(1,1,1,1)
		{
		}
	};


	/**
	* @class CGUIFontData
	*/
	class GUIEXPORT CGUIFontData : public CGUIResource
	{	
	public:
		virtual ~CGUIFontData();

		uint16 GetFontID() const;
		uint16 GetFontSize() const;
		const CGUIString& GetFontDesc() const;
		const CGUIString& GetFontPath() const;
		const CGUIIntSize& GetTextureSize() const;

	protected:
		CGUIFontData( const CGUIString& rName,
			const CGUIString& rSceneName, 
			const SFontInfo& rFontInfo );

	protected:
		friend class CGUIFontManager;
		//disable =
		CGUIFontData( const CGUIFontData&  );
		const CGUIFontData& operator=(const CGUIFontData& );

	protected:
		SFontInfo m_aFontInfo;
	};
}


#endif //__KEN_GUIFONTDATA_20091027_H__
