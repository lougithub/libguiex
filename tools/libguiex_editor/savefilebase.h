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


//============================================================================//
// declare
//============================================================================// 



//============================================================================//
// class
//============================================================================// 
class CSaveFileBase
{
public:
	CSaveFileBase(const std::string& rFilename)
		:m_bSaveFlag(false)
		,m_strFileName(rFilename)
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

	void	SetSaveFlag( bool bFlag )
	{
		m_bSaveFlag = bFlag;
	}

	bool	ShouldSaveFile() const
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
	bool		m_bSaveFlag;
	std::string	m_strFileName;
};




#endif //__KEN_SAVE_FILE_BASE_20070925_H__
