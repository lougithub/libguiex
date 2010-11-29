// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "CORE\ReQrcHelper.h"
#include <QFile>
#include <QXmlStreamAttributes>


namespace
{
	static QString sQResource = QObject::tr( "qresource" );
	static QString sFile = QObject::tr( "file" );
	static QString sPrefix = QObject::tr( "prefix" );
}

namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
ReQrcHelper::ReQrcHelper( const QString& _qrcFilename )
: m_file( "", 0 )
, m_isValid( false )
{	
	QFile file( _qrcFilename );
	if( file.open( QFile::ReadOnly | QFile::Text ) )
	{
		m_reader.setDevice( &file );
		m_reader.readNext();
		while( !m_reader.atEnd() )
		{
			if( m_reader.isStartElement() )
			{
				//QString name = m_reader.name().toString();
				if( sQResource == m_reader.name().toString() )
				{
					m_resource.Clear();					
					QXmlStreamAttributes attributes = m_reader.attributes();
					if( attributes.hasAttribute( sPrefix ) )
						m_resource.SetPrefix( attributes.value( sPrefix ).toString() );

					ReadResourceElement();

					m_resourceList.push_back( m_resource );
				}
				else
				{
					m_reader.readNext();
				}
			}
			else
			{
				m_reader.readNext();
			}
		}

		m_isValid = !m_reader.hasError();
	}
}


// -----------------------------------------------------
// Utility
// -----------------------------------------------------
void ReQrcHelper::ReadResourceElement()
{
	m_reader.readNext();
	while( !m_reader.atEnd() )
	{
		if( m_reader.isStartElement() )
		{
			ReadFileElement();
		}
		else if( m_reader.isEndElement() && sQResource == m_reader.name().toString() )
		{
			break;
		}
		else
		{
			m_reader.readNext();
		}
	}
}


void ReQrcHelper::ReadFileElement()
{
	m_file.m_filename = m_reader.readElementText();
	m_resource.AddFile( m_file.m_filename, m_file.m_type );
	m_reader.readNext();
}


}	// namespace RE
