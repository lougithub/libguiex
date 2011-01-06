// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimEditor.h"
#include "Ui\ReAnimConsoleWidget.h"
#include "Ui\ReAnimGraphicsWidget.h"
#include "Ui\ReAnimGraphicsScene.h"
#include <QMouseEvent>
#include <QPainter>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QMatrix>


namespace RE
{


// ----------------------------------------------------------------------------
// General.
// ----------------------------------------------------------------------------
ReAnimEditor::ReAnimEditor( ReAnimModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
// Widgets.
, m_trackConsole( NULL )
, m_animGraphics( NULL )
, m_lastFocusedWidget( NULL )
// Menu.
, m_animViewMenu( NULL )
// Debug.
, m_isDebugEnabled( false )
{
	// Animation graphics scene.
	m_scene = new ReAnimGraphicsScene( _model, this );

	// Animation console widget.
	m_trackConsole = new ReAnimConsoleWidget( _model, m_scene, this );
	m_trackConsole->setFocusPolicy( Qt::StrongFocus );
	m_trackConsole->setContextMenuPolicy( Qt::CustomContextMenu );
	m_trackConsole->setFocus();
	m_lastFocusedWidget = m_trackConsole;

	// Animation graphics widget.
	m_animGraphics = new ReAnimGraphicsWidget( _model, m_scene, this );
	m_animGraphics->setFocusPolicy( Qt::ClickFocus );

	connect( m_animGraphics, SIGNAL( ItemAdded( ReAnimGraphicsItem* ) ), m_trackConsole, SLOT( OnItemAdded( ReAnimGraphicsItem* ) ) );

	QSplitter* splitter = new QSplitter(  Qt::Vertical, this );
	splitter->addWidget( m_animGraphics );
	splitter->addWidget( m_trackConsole );

	QVBoxLayout* layout = new QVBoxLayout( this );
	layout->addWidget( splitter );
	setLayout( layout );
}


ReAnimEditor::~ReAnimEditor()
{
}


// ----------------------------------------------------------------------------
// Overrides QWidget.
// ----------------------------------------------------------------------------
void ReAnimEditor::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


void ReAnimEditor::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
	}
}


void ReAnimEditor::mouseReleaseEvent( QMouseEvent* _event )
{
}


void ReAnimEditor::mouseMoveEvent( QMouseEvent* _event )
{
}


void ReAnimEditor::wheelEvent( QWheelEvent* _event )
{
	int degrees = _event->delta() / 8;
	int stepCount = degrees / 15;
}


void ReAnimEditor::keyPressEvent( QKeyEvent* _event )
{
	if( Qt::Key_Control == _event->key() )
	{
		OnToggleDebug();
	}
	else if( Qt::Key_Delete == _event->key() )
	{
	}
	else
	{
		TSuper::keyPressEvent( _event );
	}
}


void ReAnimEditor::keyReleaseEvent( QKeyEvent* _event )
{
	TSuper::keyReleaseEvent( _event );
}


// ----------------------------------------------------------------------------
// Override ReBaseWidget.
// ----------------------------------------------------------------------------
void ReAnimEditor::Tick( qreal _delta )
{
	m_animGraphics->Tick( _delta );

	update();
}


QMenu* ReAnimEditor::GetEditMenu() const
{
	if( !m_animGraphics->hasFocus() )
		return m_trackConsole->GetEditMenu();
	else
		return NULL;
}


// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
void ReAnimEditor::OnToggleDebug()
{
	m_isDebugEnabled = !m_isDebugEnabled;
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------



}
