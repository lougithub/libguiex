/** 
* @file guifilesys_android.cpp
* @brief use file io in android.
* @author ken
* @date 2011-05-10
*/


//============================================================================//
// include
//============================================================================// 
#include "guifilesys_android.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <stdio.h>  
#include <dirent.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <sys/param.h>  
#include <sys/types.h>  
#include <unistd.h> 
#include <libgen.h> 

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	GUI_INTERFACE_IMPLEMENT(IGUIFileSys_android);
	//------------------------------------------------------------------------------
	const char* IGUIFileSys_android::StaticGetModuleName()
	{
		return "IGUIFileSys_android";
	}
	//------------------------------------------------------------------------------
	IGUIFileSys_android::IGUIFileSys_android()
		:IGUIInterfaceFileSys(StaticGetModuleName())
		,m_pAPKArchive(NULL)
	{
	}
	//------------------------------------------------------------------------------
	IGUIFileSys_android::~IGUIFileSys_android()
	{
	}
	//------------------------------------------------------------------------------
	int	IGUIFileSys_android::DoInitialize(void* )
	{
		m_pAPKArchive = zip_open(GSystem->GetApkPath().c_str(), 0, NULL);
		if (m_pAPKArchive == NULL) 
		{
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIFileSys_android::DoDestroy()
	{
		if( m_pAPKArchive )
		{
			zip_close( m_pAPKArchive );
		}
		return;
	}
	//------------------------------------------------------------------------------
	int32 IGUIFileSys_android::ReadFile( 
		const CGUIString& rFileName, 
		CGUIDataChunk& rData,
		EOpenMode eOpenMode)
	{
		CGUIString strFullPath;
		GSystem->GenerateFullPath(rFileName, strFullPath );
		zip_file* file = zip_fopen(m_pAPKArchive, strFullPath.c_str(), 0);
		if( !file)
		{
			//failed to open file
			return -1;
		}

		struct zip_stat aZipStat;
		if( 0 != zip_stat( m_pAPKArchive, strFullPath.c_str(), 0, &aZipStat ))
		{
			zip_fclose( file );
			return -1;
		}

		uint32 nSize = aZipStat.size;
		if( nSize == 0 )
		{
			zip_fclose( file );
			return 0;
		}

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
			zip_fclose( file );
			return -1;
		}

		if( nSize != zip_fread( file, pBuf, nSize ) )
		{
			//failed to read file
			zip_fclose( file );
			return -1;
		}

		zip_fclose( file );

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIFileSys_android::FindFiles( const CGUIString& rPath,
									  const CGUIString& rSuffix,
									  std::vector<CGUIString>& rArrayStrings )
	{
		CGUIString strFullPath;
		CGUISystem::Instance()->GenerateFullPath( rPath, strFullPath );

		int numFiles = zip_get_num_files(m_pAPKArchive);
		for (int i=0; i<numFiles; i++) 
		{
			const char* name = zip_get_name(m_pAPKArchive, i, 0);

			if (name != NULL) 
			{
				int nNameLen = strlen( name );
				//check suffix
				if( !rSuffix.empty())
				{
					char* pDest = strstr( name, rSuffix.c_str() );
					if( !pDest || pDest - name != nNameLen - rSuffix.size() )
					{
						continue;
					}
				}
				//check path
				if( strstr( name, strFullPath.c_str() ) != name )
				{
					continue;
				}
				rArrayStrings.push_back( name + strFullPath.size() );
			}
		}
	}
	//------------------------------------------------------------------------------
	CGUIString IGUIFileSys_android::GetFilename( const CGUIString& rPath )
	{
		char* pBuf = new char[rPath.size()+1];
		strcpy( pBuf, rPath.c_str() );
		CGUIString aBaseName(basename( pBuf));
		delete[] pBuf;
		return aBaseName;
	}
	//------------------------------------------------------------------------------
	CGUIString IGUIFileSys_android::GetFileDir( const CGUIString& rPath )
	{
		char* pBuf = new char[rPath.size()+1];
		strcpy( pBuf, rPath.c_str() );
		CGUIString aDirName(dirname( pBuf));
		aDirName += "/";
		delete[] pBuf;
		return aDirName;
	}	
	//------------------------------------------------------------------------------
	void IGUIFileSys_android::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
