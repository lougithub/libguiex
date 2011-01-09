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
, m_animConsole( NULL )
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
	m_animConsole = new ReAnimConsoleWidget( _model, m_scene, this );
	m_animConsole->setFocusPolicy( Qt::StrongFocus );
	m_animConsole->setContextMenuPolicy( Qt::CustomContextMenu );
	m_animConsole->setFocus();
	m_lastFocusedWidget = m_animConsole;

	// Animation graphics widget.
	m_animGraphics = new ReAnimGraphicsWidget( _model, m_scene, this );
	m_animGraphics->setFocusPolicy( Qt::ClickFocus );

	connect( m_animGraphics, SIGNAL( ItemAdded( ReAnimGraphicsItem* ) ), m_animConsole, SLOT( OnItemAdded( ReAnimGraphicsItem* ) ) );
	connect( m_animGraphics, SIGNAL( ItemDeleted( ReAnimGraphicsItem* ) ), m_animConsole, SLOT( OnItemDeleted( ReAnimGraphicsItem* ) ) );

	QSplitter* splitter = new QSplitter(  Qt::Vertical, this );
	splitter->addWidget( m_animGraphics );
	splitter->addWidget( m_animConsole );
	splitter->setStretchFactor( 0, 1 );
	splitter->setStretchFactor( 1, 0 );

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
	//else if( Qt::Key_Space == _event->key() )
	//{
	//	m_animConsole->OnTogglePlay();
	//}
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
QMenu* ReAnimEditor::GetEditMenu() const
{
	if( !m_animGraphics->hasFocus() )
		return m_animConsole->GetEditMenu();
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
