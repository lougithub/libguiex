// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
// ---------------------------------------------------------
// General includes
// ---------------------------------------------------------
#include "CORE\ReEditor.h"
#include "UI\ReMainWindow.h"
#include <qt\qapplication.h>
#include <QTimer>
#include <QGLFormat>
#include <QSemaphore>
#include <QSplashScreen>


// ---------------------------------------------------------
// Engine includes
// ---------------------------------------------------------
namespace RE
{


ReEditor::ReEditor( QSemaphore* _lock, int _argc, char* _argv[], QObject* _parent /* = NULL */ )
: m_lock( _lock )
, m_argc( _argc )
, m_argv( _argv )
{
	if( NULL != m_lock )
		m_lock->acquire( 1 );	
}


void ReEditor::run()
{
	// Default to double buffering mode.
	QGLFormat fmt;
	fmt.setDoubleBuffer( true );
	QGLFormat::setDefaultFormat( fmt );

	// Qt resource.
	Q_INIT_RESOURCE( Editor );

	// Qt app.
	m_application = new QApplication( m_argc, m_argv );

	QPixmap pm( ":/image/device_ipad.png" );
	QSplashScreen ss( pm );
	ss.show();
	

	QObject::connect( m_application, SIGNAL( aboutToQuit() ), this, SLOT( Shutdown() ) );
	m_mainWindow = new RE::ReMainWindow();
	m_mainWindow->m_editor = this;
	m_mainWindow->show();

	// Loop based on timer.
	qreal tickDelta = 1.0f / 60.0f;
	QTimer* tickTimer = new QTimer( m_application );
	QObject::connect( tickTimer, SIGNAL( timeout() ), m_mainWindow, SLOT( Tick() ) );
	m_mainWindow->SetUpdateDelta( tickDelta );
	tickTimer->start( tickDelta );

	ss.finish( m_mainWindow );

	QThread::exit( m_application->exec() );

	if( NULL != m_lock )
		m_lock->release( 1 );
}


void ReEditor::Sleep( unsigned long _msec )
{
	msleep( _msec );
}


void ReEditor::Tick()
{
	// [WHY]: When this slot is connected with the timer's timeout signal, 
	// it never gets called!
	if( NULL != m_mainWindow )
	{
		static qreal sDelta = 1.0f / 60.0f;
		m_mainWindow->Tick( sDelta );
		Sleep( 3 );
	}
}


void ReEditor::Shutdown()
{

}


}	// namespace RE
