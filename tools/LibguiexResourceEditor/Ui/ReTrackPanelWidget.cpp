// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReTrackPanelWidget.h"
#include "Ui\ReTrackWidget.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReTrackHeadWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>


namespace
{
	static int DEFAULT_MARGIN = 5;
}


namespace RE
{


// ----------------------------------------------------------------------------
// General.
// ----------------------------------------------------------------------------
ReTrackPanelWidget::ReTrackPanelWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
// Widgets.
, m_rulerWidget( NULL )
// Menu.
, m_editMenu( NULL )
// UI stats.
, m_leftMargin( DEFAULT_MARGIN )
, m_rightMargin( DEFAULT_MARGIN )
, m_topMargin( DEFAULT_MARGIN )
, m_bttomMargin( DEFAULT_MARGIN )
, m_verticalGap( 3 )
, m_horizontalGap( 3 )
, m_rulerHeight( 40 )
, m_trackHeight( 25 )
, m_headWidth( 20 )
// Debug.
, m_isDebugEnabled( false )
{
	InitMenus();

	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );
}


ReTrackPanelWidget::~ReTrackPanelWidget()
{
}


// ----------------------------------------------------------------------------
// Overrides QWidget.
// ----------------------------------------------------------------------------
void ReTrackPanelWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	// Background.
	painter.fillRect( 0, 0, width(), height(), QColor( 100, 100, 100 ) );
}


void ReTrackPanelWidget::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
	}
}


void ReTrackPanelWidget::mouseReleaseEvent( QMouseEvent* _event )
{
}


void ReTrackPanelWidget::mouseMoveEvent( QMouseEvent* _event )
{
}


void ReTrackPanelWidget::wheelEvent( QWheelEvent* _event )
{
	int degrees = _event->delta() / 8;
	int stepCount = degrees / 15;
}


void ReTrackPanelWidget::keyPressEvent( QKeyEvent* _event )
{
	if( Qt::Key_Control == _event->key() )
	{
	}
	else if( Qt::Key_Delete == _event->key() )
	{
	}
	else
	{
		TSuper::keyPressEvent( _event );
	}
}


void ReTrackPanelWidget::keyReleaseEvent( QKeyEvent* _event )
{
	TSuper::keyReleaseEvent( _event );
}


void ReTrackPanelWidget::resizeEvent( QResizeEvent* _event )
{
	// When the panel is being resized, we just shorten or lengthen 
	// the ruler and the tracks. Their height will not be affected.
	TSuper::resizeEvent( _event );

	QSize oldSize = _event->oldSize();
	QSize newSize = _event->size();
	int widthDelta = newSize.width() - oldSize.width();

	// Ruler.
	if( NULL != m_rulerWidget )
	{
		QRect rulerGeo = m_rulerWidget->geometry();
		rulerGeo.setWidth( rulerGeo.width() + widthDelta );
		m_rulerWidget->setGeometry( rulerGeo );
	}
	else
	{
		InitRuler();
	}

	// Tracks.
	TTrackListItor itor = m_trackList.begin();
	TTrackListItor itorEnd = m_trackList.end();
	for( int i = 0; itor != itorEnd; ++itor, ++i )
	{
		ReTrackSuite& suite = *itor;
		QRect trackGeo = suite.m_trackWidget->geometry();
		trackGeo.setWidth( trackGeo.width() + widthDelta );
		suite.m_trackWidget->setGeometry( trackGeo );
	}
}


// ----------------------------------------------------------------------------
// Override ReBaseWidget.
// ----------------------------------------------------------------------------
void ReTrackPanelWidget::Tick( qreal _delta )
{
	update();
}


QMenu* ReTrackPanelWidget::GetEditMenu() const
{
	QMenu* result = m_editMenu;

	QWidget* child = focusWidget();
	if( NULL != child )
	{
		ReTrackWidget* track = dynamic_cast< ReTrackWidget* >( child );
		if( NULL != track )
			result = track->GetEditMenu();
	}

	return result;
}



// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
ReTrackPanelWidget::ReTrackSuite* ReTrackPanelWidget::OnNewTrack()
{
	int y = m_topMargin + m_rulerHeight + m_verticalGap + 
		( m_verticalGap + m_trackHeight ) * ( int )m_trackList.size();

	ReTrackHeadWidget* head = new ReTrackHeadWidget( this );
	head->setFocusPolicy( Qt::ClickFocus );
	head->setScaledContents( true );
	head->setPixmap( QPixmap( ":/image/editor_track_frame.png" ) );
	head->setVisible( true );

	ReTrackWidget* track = new ReTrackWidget( this );
	track->setContextMenuPolicy( Qt::CustomContextMenu );
	track->setFocusPolicy( Qt::ClickFocus );
	track->setVisible( true );
	track->SetFrameSize( 10, m_trackHeight - 2 );

	connect( m_rulerWidget, SIGNAL( CursorChanged( int ) ), track, SLOT( OnCursorChanged( int ) ) );
	connect( m_rulerWidget, SIGNAL( ViewportChanged( int ) ), track, SLOT( OnViewportChanged( int ) ) );

	int headX = m_leftMargin;
	int headY = y;
	int headW = m_headWidth;
	int headH = m_trackHeight;
	head->setGeometry( QRect( headX, headY, headW, headH ) );

	int trackX = m_leftMargin + headW + m_horizontalGap;
	int trackY = y;
	int trackW = width() - trackX - m_rightMargin;
	int trackH = m_trackHeight;
	track->setGeometry( QRect( trackX, trackY, trackW, trackH ) );

	m_trackList.push_back( ReTrackSuite( head, track ) );
	return &m_trackList.back();
}


void ReTrackPanelWidget::OnContextMenu( const QPoint& _point )
{
	m_editMenu->exec( mapToGlobal( _point ) );
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------
void ReTrackPanelWidget::InitMenus()
{
	QMenu* menu = NULL;
	QAction* action = NULL;

	QActionGroup* trackGroup = new QActionGroup( this );
	m_editMenu = new QMenu( tr( "&Edit" ) );

	action = m_editMenu->addAction( tr( "New &Track" ) );
	action->setActionGroup( trackGroup );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnNewTrack() ) );
}


void ReTrackPanelWidget::InitRuler()
{
	// Widgets.
	m_rulerWidget = new ReRulerWidget( this );
	m_rulerWidget->setVisible( true );

	int rulerX = m_leftMargin + m_headWidth + m_horizontalGap;
	int rulerY = m_topMargin;
	int rulerW = width() - rulerX - m_rightMargin;
	int rulerH = m_rulerHeight;
	m_rulerWidget->setGeometry( QRect( rulerX, rulerY, rulerW, rulerH ) );
}


}
