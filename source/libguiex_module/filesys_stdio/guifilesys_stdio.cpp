/** 
* @file guifilesys_stdio.cpp
* @brief use standard file io.
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/filesys_stdio/guifilesys_stdio.h>
#include <fstream>
#include <libguiex_core/guiwidgetsystem.h>

#if defined(GUIEX_PLATFORM_WIN32)
#include <io.h>
#elif defined(GUIEX_PLATFORM_MAC)
#include <stdio.h>  
#include <dirent.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <sys/param.h>  
#include <sys/types.h>  
#include <unistd.h> 
#else
#error "unknown platform"	
#endif
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
	
#if defined(GUIEX_PLATFORM_WIN32)	
	static bool is_reserved_dir (const char *fn)
    {
        return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
    }
	//------------------------------------------------------------------------------
	void IGUIFileSys_stdio::FindFiles( 
		const CGUIString& rPath,
		const CGUIString& rSuffix,
		std::vector<CGUIString>& rArrayStrings )
	{
		//get file list
		long lHandle = 0, res = 0;
		struct _finddata_t tagData;
		CGUIString	strFullPath = CGUIWidgetSystem::Instance()->GetDataPath() + rPath + "*" + rSuffix;
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
				FindFiles(strNewPath, rSuffix, rArrayStrings);
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
#elif defined(GUIEX_PLATFORM_MAC)
	void IGUIFileSys_stdio::FindFiles( const CGUIString& rPath,
									  const CGUIString& rSuffix,
									  std::vector<CGUIString>& rArrayStrings )
	{	
		DIR *dp = NULL; 
		struct dirent *entry = NULL; 
		struct stat statbuf; 
		
		CGUIString	strFullPath = CGUIWidgetSystem::Instance()->GetDataPath() + rPath;
		if((dp = opendir(strFullPath.c_str())) == NULL) 
		{  
			return; 
		}  
		chdir(strFullPath.c_str());
		while((entry = readdir(dp)) != NULL)
		{ 
			stat(entry->d_name,&statbuf); 
			if(S_ISDIR(statbuf.st_mode)) 
			{ 
				if(strcmp( ".",entry->d_name) == 0 ||strcmp( "..",entry->d_name) == 0) 
				{
					continue; 
				}
				
				FindFiles( rPath + entry->d_name + "/", rSuffix, rArrayStrings );
			}
			else
			{ 
				//is a file 
				CGUIString strFilename(entry->d_name);
				if( strFilename.size() > rSuffix.size() )
				{
					if( strFilename.rfind( rSuffix ) == strFilename.size() - rSuffix.size() )
					{
						//is a file
						CGUIString strResult = rPath + entry->d_name;
						rArrayStrings.push_back(strResult);	
					}
				}
			} 
		} 
		chdir("..");
        closedir(dp); 
	}
#else
	#	error "unknown platform"	
#endif
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
