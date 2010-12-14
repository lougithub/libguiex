/** 
* @file guifilesys_cocoa.cpp
* @brief use standard file io.
* @author ken
* @date 2010-11-21
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/filesys_cocoa/guifilesys_cocoa.h>
#include <libguiex_core/guisystem.h>
#include <fstream>
#include <Foundation/Foundation.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	GUI_INTERFACE_IMPLEMENT(IGUIFileSys_cocoa);
	//------------------------------------------------------------------------------
	IGUIFileSys_cocoa::IGUIFileSys_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	IGUIFileSys_cocoa::~IGUIFileSys_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUIFileSys_cocoa::ReadFile( 
		const CGUIString& rFileName, 
		CGUIDataChunk& rData,
		EOpenMode eOpenMode)
	{
		CGUIString	strFileName = CGUISystem::Instance()->GetDataPath()+rFileName;

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
	void IGUIFileSys_cocoa::FindFiles( const CGUIString& rPath,
									  const CGUIString& rSuffix,
									  std::vector<CGUIString>& rArrayStrings )
	{
		CGUIString	strFullPath = CGUISystem::Instance()->GetDataPath() + rPath;
		NSString *nsFullPath = [NSString stringWithUTF8String:strFullPath.c_str()];
		
		NSArray *ContentOfDirectory=[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:nsFullPath error:nil];
		int contentcount=[ContentOfDirectory count];
		NSString * strSuffix = [NSString stringWithUTF8String:rSuffix.c_str()];
		for(int i=0;i<contentcount;i++)
		{
			NSString *fileName=[ContentOfDirectory objectAtIndex:i];
			//NSLog( @"file: %@", fileName );
			if( [fileName hasSuffix:strSuffix] )
			{
				CGUIString strFileName = rPath + [fileName UTF8String];
				rArrayStrings.push_back( strFileName );
			}
		}
	}
	//------------------------------------------------------------------------------
	CGUIString IGUIFileSys_cocoa::GetFilename( const CGUIString& rPath )
	{
		char* pBuf = new char[rPath.size()+1];
		strcpy( pBuf, rPath.c_str() );
		CGUIString aBaseName(basename( pBuf));
		delete[] pBuf;
		return aBaseName;
	}
	//------------------------------------------------------------------------------
	CGUIString IGUIFileSys_cocoa::GetFileDir( const CGUIString& rPath )
	{
		char* pBuf = new char[rPath.size()+1];
		strcpy( pBuf, rPath.c_str() );
		CGUIString aDirName(dirname( pBuf));
		aDirName += "/";
		delete[] pBuf;
		return aDirName;
	}	
	//------------------------------------------------------------------------------
	void IGUIFileSys_cocoa::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int		IGUIFileSys_cocoa::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIFileSys_cocoa::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
