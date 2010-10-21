/** 
* @file guiprojectinfo.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/






//============================================================================//
// include
//============================================================================//
#include <libguiex_core\guiprojectinfo.h>
#include <libguiex_core\guiinterfacefilesys.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guistring.h>
#include <libguiex_core\guiexception.h>
#include "tinyxml.h"




//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIProjectInfo::CGUIProjectInfo()
		:m_bDependenciesLoaded(false)
		,m_bResourceLoaded(false)
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	CGUIProjectInfo::~CGUIProjectInfo()
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfo::Reset()
	{
		m_strProjectFilename.empty();
		m_strProjectFilePath.empty();

		m_vecWidgetFiles.clear();
		m_vecScriptFiles.clear();
		m_vecResourceFiles.clear();
		m_vecFontInfos.clear();

		m_vecDependencies.clear();
		m_strTitle.clear();
	}
	//------------------------------------------------------------------------------
	bool	CGUIProjectInfo::IsDependenciesLoaded() const
	{
		return m_bDependenciesLoaded;
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfo::SetDependenciesLoaded( bool bLoaded )
	{
		m_bDependenciesLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	bool	CGUIProjectInfo::IsResourceLoaded() const
	{
		return m_bResourceLoaded;
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfo::SetResourceLoaded( bool bLoaded )
	{
		m_bResourceLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	const CGUIString&		CGUIProjectInfo::GetProjectFilePath() const
	{
		return m_strProjectFilePath;
	}
	//------------------------------------------------------------------------------
	const CGUIString&		CGUIProjectInfo::GetProjectFilename() const
	{
		return m_strProjectFilename;
	}
	//------------------------------------------------------------------------------
	static CGUIString	DoGetFilename( const CGUIString& rPath ) 
	{
		char fname[_MAX_FNAME];
		char fext[_MAX_EXT];
		_splitpath( rPath.c_str(), NULL, NULL, fname, fext ); 
		return CGUIString( fname ) + fext;
	}
	//------------------------------------------------------------------------------
	static CGUIString	DoGetFileDir( const CGUIString& rPath ) 
	{
		char fdir[_MAX_DIR];
		_splitpath( rPath.c_str(), NULL, fdir, NULL, NULL ); 
		return CGUIString( fdir );
	}
	//------------------------------------------------------------------------------
	int32		CGUIProjectInfo::ReadProjectFile( const CGUIString& rFileName)
	{
		Reset();

		m_strProjectFilename = DoGetFilename(rFileName);
		m_strProjectFilePath = DoGetFileDir(rFileName);

#if 1
		///read file
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			throw CGUIException("[CGUIProjectInfo::ReadProjectFile]: failed to read file <%s>!", rFileName.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[CGUIProjectInfo::ReadProjectFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}
#else
		///parse file
		TiXmlDocument aDoc;
		aDoc.LoadFile( rFileName.c_str() );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[CGUIProjectInfo::ReadProjectFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}
#endif
		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[CGUIProjectInfo::ReadProjectFile], failed to get root node from file <%s>!", rFileName.c_str());
			return -1;
		}

		///get node that contain config information
		TiXmlElement* pNode = pRootNode->FirstChildElement();
		while( pNode )
		{
			//=====================================================================================
			//<files>
			//=====================================================================================
			if( CGUIString("files") == pNode->Value())
			{
				TiXmlElement* pChildNode = pNode->FirstChildElement();
				while( pChildNode )
				{
					//add to set
					if( CGUIString("widget") == pChildNode->Value())
					{	
						//file path
						//<file path="script/configfile_tinyxml.xml" />
						CGUIString strPath = pChildNode->Attribute( "path" );
						m_vecWidgetFiles.push_back(strPath);
					}
					else if( CGUIString("script") == pChildNode->Value())
					{
						CGUIString strPath = pChildNode->Attribute( "path" );
						m_vecScriptFiles.push_back(strPath);
					}
					else if( CGUIString("resource") == pChildNode->Value())
					{
						CGUIString strPath = pChildNode->Attribute( "path" );
						m_vecResourceFiles.push_back(strPath);
					}
					else
					{
						throw guiex::CGUIException("[CGUIProjectInfo::ReadProjectFile], Failed to parse project file! <%s>!", rFileName.c_str());
						return -1;
					}

					pChildNode = pChildNode->NextSiblingElement();

				}
			}
			//=====================================================================================
			//<info>
			//=====================================================================================
			else if( CGUIString("info") == pNode->Value())
			{
				TiXmlElement* pChildNode = pNode->FirstChildElement();
				while( pChildNode )
				{
					//add to set
					if( CGUIString("title") == pChildNode->Value())
					{	
						CGUIString strValue = pChildNode->Attribute( "value" );
						m_strTitle = strValue;
					}
					else
					{
						throw guiex::CGUIException("[CGUIProjectInfo::ReadProjectFile], Failed to parse project file! <%s>!", rFileName.c_str());
						return -1;
					}
					pChildNode = pChildNode->NextSiblingElement();
				}
			}
			//=====================================================================================
			//<dependencies>
			//=====================================================================================
			else if( CGUIString("dependencies") == pNode->Value())
			{
				TiXmlElement* pChildNode = pNode->FirstChildElement();
				while( pChildNode )
				{
					//add to set
					if( CGUIString("project") == pChildNode->Value())
					{	
						CGUIString strPath = pChildNode->Attribute( "path" );
						m_vecDependencies.push_back( strPath );
					}
					else
					{
						throw guiex::CGUIException("[CGUIProjectInfo::ReadProjectFile], Failed to parse project file! <%s>!", rFileName.c_str());
						return -1;
					}
					pChildNode = pChildNode->NextSiblingElement();
				}
			}
			else
			{
				throw guiex::CGUIException("[CGUIProjectInfo::ReadProjectFile], Failed to parse project file! <%s>!", rFileName.c_str());
				return -1;
			}
			pNode = pNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetWidgetFiles() const
	{
		return m_vecWidgetFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetScriptFiles() const
	{
		return m_vecScriptFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetResourceFiles() const
	{
		return m_vecResourceFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIProjectInfo::SFontInfo>&	CGUIProjectInfo::GetFontInfos() const
	{
		return m_vecFontInfos;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetDependencies() const
	{
		return m_vecDependencies;
	}
	//------------------------------------------------------------------------------
	const CGUIString&		CGUIProjectInfo::GetTitle() const
	{
		return m_strTitle;
	}
	//------------------------------------------------------------------------------
}
