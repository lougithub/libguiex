// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_QRCHELPER_H_
#define _RE_EDITOR_QRCHELPER_H_

#include <QXmlStreamReader>
#include <QStringList>
#include <QList>


namespace RE
{


class ReQrcHelper
{
	// -----------------------------------------------------
	// Class ReQResource & ReQFile
	// -----------------------------------------------------
public:
	enum eResType{ EResourceType_NAN, EResourceType_PNG, EResourceType_BMP, EResourceType_JPG };

	class ReQFile
	{
	public:
		QString		m_filename;
		int			m_type;

		ReQFile( const QString& _filename, int _type ): m_filename( _filename ), m_type( _type ) {}
		void		Clear()	{ m_filename.clear(); m_type = 0; }
	};

	class ReQResource
	{
	public:
		typedef QList< ReQFile >			TFileList;
		typedef TFileList::Iterator			TFileListItor;
		typedef TFileList::const_iterator	TFileListCItor;

		void			AddFile( const QString& _filename, int _type ) { m_fileList.push_back( ReQFile( _filename, _type ) ); }
		void			Clear() { m_fileList.clear(); m_prefix.clear(); }
		void			SetPrefix( const QString& _prefix )	{ m_prefix = _prefix; }

		const TFileList&	GetFileList() const	{ return m_fileList; }
		const QString&		GetPrefix() const	{ return m_prefix; }

	protected:		
		TFileList		m_fileList;
		QString			m_prefix;
	};

	typedef QList< ReQResource >		TResList;
	typedef TResList::Iterator			TResListItor;
	typedef TResList::const_iterator	TResListCItor;

	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
public:
	ReQrcHelper( const QString& _qrcFilename );

	bool				IsValid() const	{ return m_isValid; }
	const TResList&		GetResourceList() const	{ return m_resourceList; }

	// -----------------------------------------------------
	// Utility
	// -----------------------------------------------------
protected:
	void				ReadResourceElement();
	void				ReadFileElement();

	// -----------------------------------------------------
	// Variable
	// -----------------------------------------------------
protected:
	QXmlStreamReader	m_reader;	
	TResList			m_resourceList;
	ReQResource			m_resource;
	ReQFile				m_file;
	bool				m_isValid;
};


}		// namespace RE
#endif	// _RE_EDITOR_QRCHELPER_H_
