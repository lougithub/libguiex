// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimDef.h"
#include "Core\ReAnimModel.h"
#include "Core\ReAnimFrame.h"
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
ReTrackPanelWidget::ReTrackPanelWidget( ReAnimModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_animMode( _model )
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
	for( int i = 0; i < ETrackType_Count; ++i )
	{
		m_trackSuites[ i ] = NULL;
	}

	InitMenus();

	// Since this class derives from QStandardItemModel and QWidget both of which has Q_OBJECT declared,
	// we need to use some trick to avoid compiling errors.
	QObject* pThis = ( QWidget* )this;
	//QObject* pThis = ( QStandardItemModel* )this;
	pThis->connect( pThis, SIGNAL( customContextMenuRequested( const QPoint& ) ), pThis, SLOT( OnContextMenu( const QPoint& ) ) );
}


ReTrackPanelWidget::~ReTrackPanelWidget()
{
}


QMatrix ReTrackPanelWidget::GetTranslationMatrix() const
{
	QMatrix matrix;

	QVariant left;
	QVariant right;
	qreal factor = 0.0f;

	if( PrepareTransform( ETrackType_Translation, left, right, factor ) )
	{
		QPointF valueLeft = left.toPointF();
		QPointF valueRight = right.toPointF();

		QPointF value;
		if( factor <= 0.0f )
		{
			value = valueRight;
		}
		else if( factor >= 1.0f )
		{
			value = valueLeft;
		}
		else
		{
			value = valueLeft * factor + valueRight * ( 1.0f - factor );
		}

		matrix.translate( value.x(), value.y() );
	}

	return matrix;
}


QMatrix ReTrackPanelWidget::GetRotationMatrix() const
{
	QMatrix matrix;

	QVariant left;
	QVariant right;
	qreal factor = 0.0f;

	if( PrepareTransform( ETrackType_Rotation, left, right, factor ) )
	{
		qreal valueLeft = left.toPointF().x();
		qreal valueRight = right.toPointF().x();

		qreal value;
		if( factor <= 0.0f )
		{
			value = valueRight;
		}
		else if( factor >= 1.0f )
		{
			value = valueLeft;
		}
		else
		{
			value = valueLeft * factor + valueRight * ( 1.0f - factor );
		}

		matrix.rotate( value );
	}

	return matrix;
}


QMatrix ReTrackPanelWidget::GetScaleMatrix() const
{
	QMatrix matrix;

	QVariant left;
	QVariant right;
	qreal factor = 0.0f;

	if( PrepareTransform( ETrackType_Scale, left, right, factor ) )
	{
		QPointF valueLeft = left.toPointF();
		QPointF valueRight = right.toPointF();

		QPointF value;
		if( factor <= 0.0f )
		{
			value = valueRight;
		}
		else if( factor >= 1.0f )
		{
			value = valueLeft;
		}
		else
		{
			value = valueLeft * factor + valueRight * ( 1.0f - factor );
		}

		matrix.scale( value.x(), value.y() );
	}

	return matrix;
}


bool ReTrackPanelWidget::PrepareTransform( eTrackType _type, QVariant& _left, QVariant& _right, qreal& _factor ) const
{
	bool result = false;

	ReTrackSuite* suite = m_trackSuites[ _type ];
	if( NULL != suite )
	{
		ReTrackWidget* track = suite->m_trackWidget;

		ReTrackFrameWidget* left = NULL;
		ReTrackFrameWidget* right = NULL;
		track->GetNearestFrames( track->GetCursor(), left, right );

		result = NULL != left || NULL != right;

		if( NULL != left && NULL != right )
		{
			if( left == right )
			{
				ReAnimFrame* frameData = left->GetModelData();
				_left = frameData->ToVariant();
				_right = _left;
				_factor = 1.0f;
			}
			else
			{
				int leftCursor = track->CalcCursorAtFrame( left );
				int rightCursor = track->CalcCursorAtFrame( right );
				_factor = 1.0f - ( qreal )( track->GetCursor() - leftCursor ) / ( qreal )( rightCursor - leftCursor );
				_left = left->GetModelData()->ToVariant();
				_right = right->GetModelData()->ToVariant();
			}
		}
		else if( NULL != left )
		{
			_left = left->GetModelData()->ToVariant();
			_right = QVariant();
			_factor = 1.0f;
		}
		else if( NULL != right )
		{
			int rightCursor = track->CalcCursorAtFrame( right );
			_factor = 1.0f - ( qreal )track->GetCursor() / ( qreal )rightCursor;
			_left = 0;
			_right = right->GetModelData()->ToVariant();
		}

	}

	return result;
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
ReTrackPanelWidget::ReTrackSuite* ReTrackPanelWidget::OnNewTranslationTrack()
{
	return OnNewTrack( ETrackType_Translation );
}


ReTrackPanelWidget::ReTrackSuite* ReTrackPanelWidget::OnNewRotationTrack()
{
	return OnNewTrack( ETrackType_Rotation );
}


ReTrackPanelWidget::ReTrackSuite* ReTrackPanelWidget::OnNewScaleTrack()
{
	return OnNewTrack( ETrackType_Scale );
}


ReTrackPanelWidget::ReTrackSuite* ReTrackPanelWidget::OnNewAlphaTrack()
{
	return OnNewTrack( ETrackType_Alpha );
}


ReTrackPanelWidget::ReTrackSuite* ReTrackPanelWidget::OnNewTrack( eTrackType _type )
{
	ReTrackPanelWidget::ReTrackSuite* result = NULL;

	if( NULL != m_animMode )
	{
		if( NULL == m_animMode->GetTrackByType( _type ) )
		{
			// Create model data.
			m_animMode->CreateTrack( _type );

			// Create widget ( suite = head + track ).
			ReTrackHeadWidget* head = new ReTrackHeadWidget( this );
			head->setFocusPolicy( Qt::ClickFocus );
			head->setScaledContents( true );
			head->setPixmap( QPixmap( ":/image/editor_track_frame.png" ) );
			head->setVisible( true );

			//ReTrackWidget* track = NULL;
			//int type = ( int )_type;
			//if( ETrackType_Rotation == _type )
			//	track = new ReRotationTrackWidget( this );
			//else if( ETrackType_Scale == _type )
			//	track = new ReScaleTrackWidget( this );
			//else if( ETrackType_Alpha == _type )
			//	track = new ReAlphaTrackWidget( this );
			//else
			//	track = new ReTranslationTrackWidget( this );
			ReTrackWidget* track = new ReTrackWidget( this );

			track->setContextMenuPolicy( Qt::CustomContextMenu );
			track->setFocusPolicy( Qt::ClickFocus );
			track->setVisible( true );
			track->SetFrameSize( 10, m_trackHeight - 2 );

			QObject::connect( track, SIGNAL( CreateFrameRequested( ReTrackWidget* ) ), this, SLOT( OnCreateFrameRequested( ReTrackWidget* ) ) );
			QObject::connect( track, SIGNAL( DeleteFrameRequested( ReTrackWidget* ) ), this, SLOT( OnDeleteFrameRequested( ReTrackWidget* ) ) );
			QObject::connect( track, SIGNAL( FrameMoved( ReTrackWidget*, ReTrackFrameWidget*, qreal ) ), this, SLOT( OnFrameMoved( ReTrackWidget*, ReTrackFrameWidget*, qreal ) ) );
			QObject::connect( m_rulerWidget, SIGNAL( CursorChanged( int ) ), track, SLOT( OnCursorChanged( int ) ) );
			QObject::connect( m_rulerWidget, SIGNAL( ViewportChanged( int ) ), track, SLOT( OnViewportChanged( int ) ) );

			m_trackList.push_back( ReTrackSuite( head, track, ( int )_type ) );			
			result = &m_trackList.back();
			m_trackSuites[ _type ] = result;

			// Tracks layout.
			LayoutTracks();
		}
	}

	return result;
}


void ReTrackPanelWidget::OnContextMenu( const QPoint& _point )
{
	m_editMenu->exec( mapToGlobal( _point ) );
}


void ReTrackPanelWidget::OnCreateFrameRequested( ReTrackWidget* _track )
{	
	eTrackType trackType = GetTrackType( _track );
	if( trackType < ETrackType_Count )
	{
		// Update widget data.
		ReTrackFrameWidget* frameWidget = _track->CreateFrameAtCurrentCursor();

		// Update model data.
		int pos = _track->GetViewport() + _track->GetCursor();
		qreal time = ( ( qreal )pos / ( qreal )_track->GetUnit() ) * _track->GetUnitValue();
		ReAnimFrame* frameData = m_animMode->CreateFrame( trackType, time );

		// Associate the frame widget and the frame data.
		//frameData->GetIdRef() = frameWidget->GetId();
		frameWidget->GetModelDataRef() = frameData;
	}
}


void ReTrackPanelWidget::OnDeleteFrameRequested( ReTrackWidget* _track )
{
	eTrackType trackType = GetTrackType( _track );
	if( trackType < ETrackType_Count )
	{
		ReTrackFrameWidget* frame = _track->GetCurrentFrame();
		if( NULL != frame )
		{
			// Update model data.
			m_animMode->DeleteFrame( trackType, frame->GetModelData() );

			// Update widget data.
			_track->DeleteCurrentFrame();
		}
	}
}


void ReTrackPanelWidget::OnFrameMoved( ReTrackWidget* _track, ReTrackFrameWidget* _frame, qreal _time )
{
	if( NULL != _track && NULL != _frame )
	{
		_frame->GetModelData()->GetTimeRef() = _time;
		m_animMode->OnFrameChanged( GetTrackType( _track ), _frame->GetModelData() );
	}
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------
void ReTrackPanelWidget::InitMenus()
{
	QObject* pThis = ( QWidget* )this;

	QMenu* menu = NULL;
	QAction* action = NULL;

	m_editMenu = new QMenu( tr( "&Edit" ) );

	action = m_editMenu->addAction( tr( "New &Translation Track" ) );
	pThis->connect( action, SIGNAL( triggered() ), pThis, SLOT( OnNewTranslationTrack() ) );
	
	action = m_editMenu->addAction( tr( "New &Rotation Track" ) );
	pThis->connect( action, SIGNAL( triggered() ), pThis, SLOT( OnNewRotationTrack() ) );

	action = m_editMenu->addAction( tr ( "New &Scale Track" ) );
	pThis->connect( action, SIGNAL( triggered() ), pThis, SLOT( OnNewScaleTrack() ) );

	action = m_editMenu->addAction( tr ( "New &Alpha Track" ) );
	pThis->connect( action, SIGNAL( triggered() ), pThis, SLOT( OnNewAlphaTrack() ) );
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


void ReTrackPanelWidget::LayoutTracks()
{
	int y = m_topMargin + m_rulerHeight + m_verticalGap;

	for( int i = 0; i < ETrackType_Count; ++i )
	{
		if( NULL != m_trackSuites[ i ] )
		{
			int headX = m_leftMargin;
			int headY = y;
			int headW = m_headWidth;
			int headH = m_trackHeight;
			m_trackSuites[ i ]->m_headWidget->setGeometry( QRect( headX, headY, headW, headH ) );

			int trackX = m_leftMargin + headW + m_horizontalGap;
			int trackY = y;
			int trackW = width() - trackX - m_rightMargin;
			int trackH = m_trackHeight;
			m_trackSuites[ i ]->m_trackWidget->setGeometry( QRect( trackX, trackY, trackW, trackH ) );

			y += ( m_verticalGap + m_trackHeight );
		}
	}
}


eTrackType ReTrackPanelWidget::GetTrackType( ReTrackWidget* _track ) const
{
	eTrackType result = ETrackType_Count;

	if( NULL != _track )
	{

		for( int i = 0; i < ETrackType_Count; ++i )
		{
			ReTrackSuite* suite = m_trackSuites[ i ];
			if( NULL != suite )
			{
				if( suite->m_trackWidget == _track )
				{
					result = ( eTrackType )i;
					break;
				}
			}
		}
	}

	return result;
}


}
