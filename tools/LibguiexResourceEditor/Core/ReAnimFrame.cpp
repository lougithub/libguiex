#include "StdAfxEditor.h"
#include "Core\ReAnimFrame.h"
#include "Core\ReAnimTrack.h"
#include <QPointF>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimFrame::ReAnimFrame( qreal _time )
: m_time( _time )
{
	static QString sValueNames[ VALUE_COUNT ] = 
	{
		QObject::tr( "X" ),
		QObject::tr( "Y" ),
	};

	for( int i = 0; i < VALUE_COUNT; ++i )
	{
		m_values[ i ] = ReAnimValueItem();
		m_values[ i ].GetParentRef() = this;
		m_values[ i ].GetNameRef() = sValueNames[ i ];
	}
}


bool ReAnimFrame::operator<( ReAnimFrame* _other ) const
{
	return m_time < _other->GetTime();
}


// -----------------------------------------------------------------------------
// Override ReAnimItem.
// -----------------------------------------------------------------------------
QVariant ReAnimFrame::ToKeyVariant() const
{
	ReAnimTrack* track = dynamic_cast< ReAnimTrack* >( GetParent() );
	if( NULL != track )
	{
		return  QString( QObject::tr( "%1_%2_%3" ) ).arg( TSuper::ToKeyVariant().toString() ).arg( track->IndexOfChild( this ) ).arg( m_time );
	}
	else
	{
		return TSuper::ToKeyVariant();
	}
}


QVariant ReAnimFrame::ToValueVariant() const
{
	QVariant result = QVariant();

	int count = gDataValueCount( GetType() );
	if( 1 == count )
		result = QString( QObject::tr( "%1" ) ).arg( GetValue( 0 ).GetData() );
	else if( 2 == count )
		result = QString( QObject::tr( "%1,%2" ) ).arg( GetValue( 0 ).GetData() ).arg( GetValue( 1 ).GetData() );
	
	return result;
}


QVariant ReAnimFrame::ToVariant() const
{
	QPointF point( GetValue( 0 ).GetData(), GetValue( 1 ).GetData() );
	return point;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------




}
