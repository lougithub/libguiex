#include "StdAfxEditor.h"
#include "Ui\ReAnimGraphicsView.h"
#include "Ui\ReAnimGraphicsItem.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QList>
#include <QLabel>


namespace
{
	int gCounter = 0;
}


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimGraphicsView::ReAnimGraphicsView( QGraphicsScene* _scene, QWidget* _parent )
: TSuper( _scene, _parent )
, m_currentTool( NULL )
, m_editMode( EEditMode_Select )
, m_isInitialized( false )
{
	setCacheMode( QGraphicsView::CacheBackground );
	setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
	setOptimizationFlag( QGraphicsView::DontClipPainter, true );
	setOptimizationFlag( QGraphicsView::DontAdjustForAntialiasing, true );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setFrameShape( QFrame::NoFrame );

	m_modeLabel = new QLabel( this );
	m_modeLabel->move( 120, 120 );
	m_modeLabel->setWindowTitle( gEditModeToString( m_editMode ) );
	m_modeLabel->setVisible( true );

	m_tools[ EEditMode_Select ]		= new ReAnimSelectTool();
	m_tools[ EEditMode_Move ]		= new ReAnimMoveTool();
	m_tools[ EEditMode_Rotate ]		= new ReAnimRotateTool();
	m_tools[ EEditMode_Scale ]		= new ReAnimScaleTool();
	m_tools[ EEditMode_Alpha ]		= new ReAnimAlphaTool();

	m_currentTool = m_tools[ EEditMode_Select ];
}


ReAnimGraphicsView::~ReAnimGraphicsView()
{
	for( int i = 0; i < EEditMode_Count; ++i )
		if( NULL != m_tools[ i ] )
			delete m_tools[ i ];
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimGraphicsView::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragInfo.SetCursorPosBackup( _event->pos() );
		m_dragInfo.SetItemPosBackup( mapToScene( QPoint( width() / 2, height() / 2 ) ) );
		m_dragInfo.StartMove();
		setCursor( Qt::ClosedHandCursor );
	}

	TSuper::mousePressEvent( _event );
}


void ReAnimGraphicsView::mouseReleaseEvent( QMouseEvent* _event )
{
	m_dragInfo.Stop();
	setCursor( Qt::ArrowCursor );
	TSuper::mouseReleaseEvent( _event );
}


void ReAnimGraphicsView::mouseMoveEvent( QMouseEvent* _event )
{
	if( m_dragInfo.IsMoving() )
	{
		QPointF delta = QPointF( _event->pos() ) - m_dragInfo.GetCursorPosBackup();
		QPointF center = m_dragInfo.GetItemPosBackup() - QPointF( delta );
		centerOn( center );

		QPointF origin = mapFromScene( 0.0f, 0.0f );
		emit SceneOriginXChanged( -( int )origin.x() );
		emit SceneOriginYChanged( -( int )origin.y() );
	}

	TSuper::mouseMoveEvent( _event );
}


void ReAnimGraphicsView::wheelEvent( QWheelEvent* _event )
{
	// Override to avoid wheel-scrolling the scene.
}


void ReAnimGraphicsView::keyPressEvent( QKeyEvent* _event )
{
	// Override to avoid scrolling the scene by pressing some
	// keyboard buttons.
	if( Qt::Key_Left != _event->key() &&
		Qt::Key_Up != _event->key() &&
		Qt::Key_Right != _event->key() &&
		Qt::Key_Down != _event->key() &&
		Qt::Key_PageDown != _event->key() &&
		Qt::Key_PageUp != _event->key() )
	{
		eEditMode newMode = EEditMode_Count;
		if( Qt::Key_F1 == _event->key() )
		{
			// Select mode.
			newMode = EEditMode_Select;			
		}
		else if( Qt::Key_F2 == _event->key() )
		{
			// Move mode.
			newMode = EEditMode_Move;
		}
		else if( Qt::Key_F3 == _event->key() )
		{
			// Rotate mode.
			newMode = EEditMode_Rotate;
		}
		else if( Qt::Key_F4 == _event->key() )
		{
			// Scale mode.
			newMode = EEditMode_Scale;
		}
		else if( Qt::Key_F5 == _event->key() )
		{
			// Alpha mode.
			newMode = EEditMode_Alpha;			
		}

		if( EEditMode_Count != newMode )
		{
			ChangeEditMode( newMode );
		}

		TSuper::keyPressEvent( _event );
	}
}


void ReAnimGraphicsView::resizeEvent( QResizeEvent* _event )
{
	if( !m_isInitialized )
	{
		QPointF origin = mapFromScene( 0.0f, 0.0f );
		emit SceneOriginXChanged( -( int )origin.x() );
		emit SceneOriginYChanged( -( int )origin.y() );

		m_isInitialized = true;
	}

	TSuper::resizeEvent( _event );
}


// -----------------------------------------------------------------------------
// Override QGraphicsView.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReAnimGraphicsView::RequestChangeSceneOriginX( int _x )
{
	QPointF originInView = mapFromScene( 0.0f, 0.0f );
	QPointF center = mapToScene( QPoint( width() / 2, height() / 2 ) );
	QPointF deltaInView = QPointF( -_x, 0.0f ) - originInView;
	deltaInView.setY( 0.0f );

	centerOn( center - deltaInView );
}


void ReAnimGraphicsView::RequestChangeSceneOriginY( int _y )
{
	QPointF originInView = mapFromScene( 0.0f, 0.0f );
	QPointF center = mapToScene( QPoint( width() / 2, height() / 2 ) );
	QPointF deltaInView = QPointF( 0.0f, -_y ) - originInView;
	deltaInView.setX( 0.0f );

	centerOn( center - deltaInView );
}


void ReAnimGraphicsView::OnSceneSelectionChanged()
{
	QList< QGraphicsItem* > selection = scene()->selectedItems();
	if( 0 == selection.size() )
	{
		m_currentTool->Release();
	}
	else if( 1 == selection.size() )
	{
		QGraphicsItem* sel = selection.value( 0 );
		ReAnimGraphicsItem* item = qgraphicsitem_cast< ReAnimGraphicsItem* >( sel );

		m_currentTool->Release();
		m_currentTool->Setup( item );
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReAnimGraphicsView::ChangeEditMode( eEditMode _mode )
{
	if( m_editMode != _mode )
	{
		m_editMode = _mode;
		m_modeLabel->setWindowTitle( gEditModeToString( m_editMode ) );
		ReAnimGraphicsItem* item = m_currentTool->GetItem();
		m_currentTool->Release();
		m_currentTool = m_tools[ m_editMode ];
		m_currentTool->Setup( item );

		emit EditModeChanged();
	}
}


}
