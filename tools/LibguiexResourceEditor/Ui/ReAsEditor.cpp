// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAsEditor.h"
#include "Ui\ReTrackPanelWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>


namespace RE
{


// ----------------------------------------------------------------------------
// General.
// ----------------------------------------------------------------------------
ReAsEditor::ReAsEditor( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
// Widgets.
, m_lastFocusedWidget( NULL )
, m_trackPanelWidget( NULL )
, m_asViewWidget( NULL )
// Menu.
, m_asViewMenu( NULL )
// Debug.
, m_isDebugEnabled( false )
{
	// Track panel widget.
	m_trackPanelWidget = new ReTrackPanelWidget();
	m_trackPanelWidget->setFocusPolicy( Qt::StrongFocus );
	m_trackPanelWidget->setContextMenuPolicy( Qt::CustomContextMenu );
	m_trackPanelWidget->installEventFilter( this );
	m_trackPanelWidget->setFocus();
	m_lastFocusedWidget = m_trackPanelWidget;

	// As view widget.
	m_asViewWidget = new QLabel();
	m_asViewWidget->setFocusPolicy( Qt::ClickFocus );
	m_asViewWidget->installEventFilter( this );

	QSplitter* splitter = new QSplitter(  Qt::Vertical, this );
	splitter->addWidget( m_asViewWidget );
	splitter->addWidget( m_trackPanelWidget );

	QVBoxLayout* layout = new QVBoxLayout( this );
	layout->addWidget( splitter );
	setLayout( layout );
}


ReAsEditor::~ReAsEditor()
{
}


// ----------------------------------------------------------------------------
// Overrides QWidget.
// ----------------------------------------------------------------------------
void ReAsEditor::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


void ReAsEditor::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
	}
}


void ReAsEditor::mouseReleaseEvent( QMouseEvent* _event )
{
}


void ReAsEditor::mouseMoveEvent( QMouseEvent* _event )
{
}


void ReAsEditor::wheelEvent( QWheelEvent* _event )
{
	int degrees = _event->delta() / 8;
	int stepCount = degrees / 15;
}


void ReAsEditor::keyPressEvent( QKeyEvent* _event )
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


void ReAsEditor::keyReleaseEvent( QKeyEvent* _event )
{
	TSuper::keyReleaseEvent( _event );
}


// ----------------------------------------------------------------------------
// Override ReBaseWidget.
// ----------------------------------------------------------------------------
void ReAsEditor::Tick( qreal _delta )
{
	if( m_isDebugEnabled )
	{
	}

	update();
}


QMenu* ReAsEditor::GetEditMenu() const
{
	if( !m_asViewWidget->hasFocus() )
		return m_trackPanelWidget->GetEditMenu();
	else
		return NULL;
}


// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
void ReAsEditor::OnToggleDebug()
{
	m_isDebugEnabled = !m_isDebugEnabled;
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------



}
