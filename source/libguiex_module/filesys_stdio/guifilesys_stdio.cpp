/** 
* @file guifilesys_stdio.cpp
* @brief use standard file io.
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\filesys_stdio\guifilesys_stdio.h>
#include <fstream>
#include <libguiex_core\guiwidgetsystem.h>
#include <io.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	GUI_INTERFACE_IMPLEMENT(IGUIFileSys_stdio);
	//------------------------------------------------------------------------------
	IGUIFileSys_stdio::IGUIFileSys_stdio()
	{
	}
	//------------------------------------------------------------------------------
	IGUIFileSys_stdio::~IGUIFileSys_stdio()
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUIFileSys_stdio::ReadFile( 
		const CGUIString& rFileName, 
		CGUIDataChunk& rData,
		EOpenMode eOpenMode)
	{
		CGUIString	strFileName = CGUIWidgetSystem::Instance()->GetDataPath()+rFileName;

		std::ifstream fin;
		fin.open( strFileName.c_str(), std::ios::binary );
		if( !fin)
		{
			//failed to open file
			return -1;
		}
		fin.seekg(0, std::ios::end);
		std::streamsize nSize = fin.tellg();
		if( nSize == 0 )
		{
			//failed to load file
			return -1;
		}
		fin.seekg(0, std::ios::beg);

		///locate buffer
		char* pBuf = NULL;
		switch( eOpenMode)
		{
		case eOpenMode_Binary:
			pBuf = (char* )rData.Allocate(nSize);
			break;
		case eOpenMode_String:
			pBuf = (char* )rData.Allocate(nSize+1);
			pBuf[nSize] = '\0';
			break;
		default:
			break;
		}
		if( !pBuf )
		{
			//failed to allocate memory
			return -1;
		}

		if( !fin.read(pBuf, nSize ))
		{
			//failed to read file
			return -1;
		}

		fin.close();


		return 0;
	}
	//------------------------------------------------------------------------------
	static bool is_reserved_dir (const char *fn)
    {
        return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
    }
	//------------------------------------------------------------------------------
	void IGUIFileSys_stdio::FindFiles( 
		const CGUIString& rPath,
		const CGUIString& rPatten,
		std::vector<CGUIString>& rArrayStrings )
	{
		//get file list
		long lHandle = 0, res = 0;
		struct _finddata_t tagData;
		CGUIString	strFullPath = CGUIWidgetSystem::Instance()->GetDataPath() + rPath + rPatten;
		lHandle = _findfirst(strFullPath.c_str(), &tagData);
		while (lHandle != -1 && res != -1)
		{
			if ( (tagData.attrib & _A_SUBDIR) == 0)
			{
				//is a file
				CGUIString strFileName = rPath + tagData.name;
				rArrayStrings.push_back(strFileName);
			}
			res = _findnext( lHandle, &tagData );
        }
        // Close if we found any files
        if(lHandle != -1)
		{
            _findclose(lHandle);
		}

		// Now find directories
		strFullPath = CGUIWidgetSystem::Instance()->GetDataPath() + rPath + "*";
		lHandle = _findfirst(strFullPath.c_str (), &tagData);
		res = 0;
		while (lHandle != -1 && res != -1)
		{
			if ((tagData.attrib & _A_SUBDIR) &&
				!is_reserved_dir (tagData.name))
			{
				// recurse
				CGUIString strNewPath = rPath + tagData.name + "\\";
				FindFiles(strNewPath, rPatten, rArrayStrings);
			}
			res = _findnext( lHandle, &tagData );
		}
		// Close if we found any files
		if(lHandle != -1)
		{
			_findclose(lHandle);
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIFileSys_stdio::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int		IGUIFileSys_stdio::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIFileSys_stdio::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
