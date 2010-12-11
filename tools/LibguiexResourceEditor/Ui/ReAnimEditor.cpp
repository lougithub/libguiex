// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimEditor.h"
#include "Ui\ReTrackPanelWidget.h"
#include "Ui\ReAnimView.h"
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
, m_lastFocusedWidget( NULL )
, m_trackPanelWidget( NULL )
, m_animView( NULL )
// Menu.
, m_animViewMenu( NULL )
// Debug.
, m_isDebugEnabled( false )
{
	// Track panel widget.
	m_trackPanelWidget = new ReTrackPanelWidget( _model, this );
	m_trackPanelWidget->setFocusPolicy( Qt::StrongFocus );
	m_trackPanelWidget->setContextMenuPolicy( Qt::CustomContextMenu );
	//m_trackPanelWidget->installEventFilter( this );
	m_trackPanelWidget->setFocus();
	m_lastFocusedWidget = m_trackPanelWidget;

	// Animation view widget.
	m_animView = new ReAnimView( this );
	m_animView->setFocusPolicy( Qt::ClickFocus );
	//m_animView->installEventFilter( this );

	QSplitter* splitter = new QSplitter(  Qt::Vertical, this );
	splitter->addWidget( m_animView );
	splitter->addWidget( m_trackPanelWidget );

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
	QMatrix translation = m_trackPanelWidget->GetTranslationMatrix();
	QMatrix rotation = m_trackPanelWidget->GetRotationMatrix();
	QMatrix scale = m_trackPanelWidget->GetScaleMatrix();
	QMatrix transform = scale * rotation * translation;
	//QMatrix transform = translation * rotation * scale;

	m_animView->GetMatrixRef() = transform;

	update();
}


QMenu* ReAnimEditor::GetEditMenu() const
{
	if( !m_animView->hasFocus() )
		return m_trackPanelWidget->GetEditMenu();
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
