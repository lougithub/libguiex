// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimConsoleWidget.h"
#include "Ui\ReAnimPlayerPanel.h"
#include "Ui\ReAnimEntityPanel.h"
#include "Ui\ReAnimUiInfo.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QSizePolicy>


namespace
{
	static int DEFAULT_MARGIN = 5;
	const int m_entityWidgetPadding = 2;
	const int m_entityWidgetGap = 2;
}


namespace RE
{


// ----------------------------------------------------------------------------
// General.
// ----------------------------------------------------------------------------
ReAnimConsoleWidget::ReAnimConsoleWidget( ReAnimModel* _model, QGraphicsScene* _scene, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
// Widgets.
, m_playerPanel( NULL )
// Menu.
, m_editMenu( NULL )
// Debug.
, m_isDebugEnabled( false )
{
	GetVerticalGapRef() = 8;

	setMinimumHeight( ANIM_PLAYER_PANEL_HEIGHT + 2 );

	// Player panel.
	m_playerPanel = new ReAnimPlayerPanel( _model, this );
	m_playerPanel->setVisible( true );

	// Entity panel.
	m_entityPanel = new ReAnimEntityPanel( _model, m_playerPanel->GetRuler(), _scene, this );
	m_entityPanel->setVisible( true );	
}


ReAnimConsoleWidget::~ReAnimConsoleWidget()
{
}


// ----------------------------------------------------------------------------
// Overrides QWidget.
// ----------------------------------------------------------------------------
void ReAnimConsoleWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );
	painter.fillRect( 0, 0, width(), height(), QColor( 123, 145, 230 ) );
}


void ReAnimConsoleWidget::keyReleaseEvent( QKeyEvent* _event )
{
	if( Qt::Key_Space == _event->key() )
		m_playerPanel->OnTogglePlay();
	else
		_event->ignore();
}


void ReAnimConsoleWidget::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );

	// Player panel.
	int x = 0;
	int y = 0;
	int w = _event->size().width();
	int h = ANIM_PLAYER_PANEL_HEIGHT;

	m_playerPanel->setGeometry( x, y, w, h );

	// Entity panel.
	x = 0;
	y = y + h + GetVerticalGap();
	h = height() - ANIM_PLAYER_PANEL_HEIGHT;
	m_entityPanel->setGeometry( x, y, w, h );
}


// ----------------------------------------------------------------------------
// Override ReBaseWidget.
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
void ReAnimConsoleWidget::OnItemAdded( ReAnimGraphicsItem* _item )
{
	m_entityPanel->OnItemAdded( _item );
}


void ReAnimConsoleWidget::OnItemDeleted( ReAnimGraphicsItem* _item )
{
	m_entityPanel->OnItemDeleted( _item );
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------


}
