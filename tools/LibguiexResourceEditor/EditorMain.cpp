// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
// ---------------------------------------------------------
// General includes
// ---------------------------------------------------------
#include "StdAfxEditor.h"
#include "CORE\ReEditor.h"
#include <QSemaphore>

#include <QApplication>
#include <QMap>
#include <QList>
#include <QPointF>


int main( int _argc, char* _argv[] )
{
	if( true )
	{
		QSemaphore* lock = new QSemaphore( 1 );

		RE::ReEditor* editor = new RE::ReEditor( lock, _argc, _argv );
		editor->start();

		lock->acquire( 1 );
		lock->release( 1 );

		return 1;
	}
}
