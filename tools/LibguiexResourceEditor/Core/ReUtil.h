// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110116.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_UTIL_H_
#define _RE_EDITOR_UTIL_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QPoint>
#include <QSize>


namespace RE
{


inline QPoint SizeToPoint( const QSize& _size )
{
	return QPoint( _size.width(), _size.height() );
}


inline QSize PointToSize( const QPoint& _point )
{
	return QSize( _point.x(), _point.y() );
}


inline QPoint RoundPoint( const QPoint& _point, int _alignment )
{
	return QPoint( _point.x() / _alignment * _alignment, _point.y() / _alignment * _alignment );
}


inline QSize RoundSize( const QSize& _size, int _alignment )
{
	return QSize( _size.width() / _alignment * _alignment, _size.height() / _alignment * _alignment );
}


inline QPoint AbsPoint( const QPoint& _point )
{
	return QPoint( qAbs( _point.x() ), qAbs( _point.y() ) );
}


inline QSize AbsSize( const QSize& _size )
{
	return QSize( qAbs( _size.width() ), qAbs( _size.height() ) );
}


inline QPoint ZoomPoint( const QPoint& _point, int _oldFactor, int _newFactor )
{
	return _point / _oldFactor * _newFactor;
}


inline QSize ZoomSize( const QSize& _size, int _oldFactor, int _newFactor )
{
	return _size / _oldFactor * _newFactor;
}


}
#endif	// _RE_EDITOR_UTIL_H_
