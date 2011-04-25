/** 
* @file savefilebase.h
* @brief save this to file, base class
* @author ken
* @date 2007-09-25
*/

#ifndef __KEN_SAVE_FILE_BASE_20070925_H__
#define __KEN_SAVE_FILE_BASE_20070925_H__


//============================================================================//
// include
//============================================================================// 
#include <string>

//============================================================================//
// declare
//============================================================================// 
enum ESaveFileMode
{
	ESaveFileMode_Canvas = 0,
	ESaveFileMode_Text
};


//============================================================================//
// class
//============================================================================// 
class CSaveFileBase
{
public:
	CSaveFileBase(const std::string& rFilename, ESaveFileMode eFileMode )
		:m_bSaveFlag(false)
		,m_strFileName(rFilename)
		,m_eSaveFileMode( eFileMode )
	{

	}

	virtual ~CSaveFileBase()
	{
		//wxASSERT( m_bSaveFlag == false );
		if(m_bSaveFlag )
		{
			//wxString strInfo = wxString::Format("WARNING: the file %s is closed but not saved!", m_strFileName.c_str());
			//GetMainFrame()->OutputString(strInfo);
		}
	}

	ESaveFileMode GetSaveFileMode() const
	{
		return m_eSaveFileMode;
	}

	void SetSaveFlag( bool bFlag )
	{
		m_bSaveFlag = bFlag;
	}

	bool ShouldSaveFile() const
	{
		return m_bSaveFlag;
	}

	const std::string& GetFileName() const
	{
		return m_strFileName;
	}

	int SaveFile()
	{
		int ret = SaveFileAs(m_strFileName);
		if( 0 == ret )
		{
			SetSaveFlag(false);
		}
		return ret;
	}

	virtual int SaveFileAs(const std::string& rNewFileName) = 0;


private:
	bool m_bSaveFlag;
	ESaveFileMode m_eSaveFileMode;
	std::string	m_strFileName;
};




#endif //__KEN_SAVE_FILE_BASE_20070925_H__
