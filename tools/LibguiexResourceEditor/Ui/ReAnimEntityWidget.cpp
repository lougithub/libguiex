// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101127.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimEntity.h"
#include "Core\ReAnimFrame.h"
#include "Core\ReAnimTrack.h"
#include "Ui\ReAnimEntityWidget.h"
#include "Ui\ReAnimTrackWidget.h"
#include "Ui\ReAnimUiInfo.h"
#include <QPainter>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QTransform>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimEntityWidget::ReAnimEntityWidget( ReAnimEntity* _model, ReRulerWidget* _ruler, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_modelData( _model )
, m_ruler( _ruler )
, m_editMenu( NULL )
, m_isHighlighted( false )
{
	InitMenu();

	m_toggleButton = new QPushButton( tr( "Collapse" ), this );
	m_toggleButton->setCheckable( true );
	m_toggleButton->setChecked( true );
	m_toggleButton->setVisible( true );
	connect( m_toggleButton, SIGNAL( toggled( bool ) ), this, SLOT( OnToggled( bool ) ) );
	
	m_label = new QLabel( tr( "Item" ), this );
	m_label->setVisible( true );

	UpdateLayout();

	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );

	// Create all types of tracks at startup.
	OnNewTranslationTrack();
	OnNewRotationTrack();
	OnNewScaleTrack();
	OnNewAlphaTrack();
}


int ReAnimEntityWidget::GetTotalHeight() const
{
	int result = 0;

	if( m_toggleButton->isChecked() )
	{
		result = GetTopPadding() + ( 1 + GetTrackCount() ) * ( ANIM_ENTITY_SUITE_HEIGHT + GetVerticalGap() );
	}
	else
	{
		result = GetTopPadding() + GetBottomPadding() + ANIM_ENTITY_SUITE_HEIGHT;
	}

	return result;
}


int ReAnimEntityWidget::GetTrackCount() const
{
	int result = 0;
	for( int i = 0; i < ETrackType_Count; ++i )
		if( NULL != m_suiteArray[ i ].m_track )
			++result;

	return result;
}


void ReAnimEntityWidget::Highlight( bool _isHighlight )
{
	if( m_isHighlighted != _isHighlight )
	{
		m_isHighlighted = _isHighlight;
	}
}


bool ReAnimEntityWidget::GetTranslationAt( int _cursor, QPointF& _result )
{
	bool isOk = false;

	ReSuite& suite = m_suiteArray[ ETrackType_Translation ];
	if( NULL != suite.m_track &&
		suite.m_buttons[ ESuiteButton_Enable ]->isChecked() )
	{
		ReAnimTrack* trackData = suite.m_track->GetModelData();
		qreal time = suite.m_track->GetValueAt( _cursor );
		QVariant data;
		if( trackData->Interpolate( time, data, false ) )
		{
			_result = data.toPointF();
			isOk = true;
		}
	}

	return isOk;
}


bool ReAnimEntityWidget::GetRotationAt( int _cursor, qreal& _result )
{
	bool isOk = false;

	ReSuite& suite = m_suiteArray[ ETrackType_Rotation ];
	if( NULL != suite.m_track &&
		suite.m_buttons[ ESuiteButton_Enable ]->isChecked() )
	{
		ReAnimTrack* trackData = suite.m_track->GetModelData();
		qreal time = suite.m_track->GetValueAt( _cursor );
		QVariant data;
		if( trackData->Interpolate( time, data, false ) )
		{
			_result = data.toDouble();
			isOk = true;
		}
	}

	return isOk;
}


bool ReAnimEntityWidget::GetScaleAt( int _cursor, QPointF& _result )
{
	bool isOk = false;

	ReSuite& suite = m_suiteArray[ ETrackType_Scale ];
	if( NULL != suite.m_track &&
		suite.m_buttons[ ESuiteButton_Enable ]->isChecked() )
	{
		ReAnimTrack* trackData = suite.m_track->GetModelData();
		qreal time = suite.m_track->GetValueAt( _cursor );
		QVariant data;
		if( trackData->Interpolate( time, data, false ) )
		{
			_result = data.toPointF();
			isOk = true;
		}
	}

	return isOk;
}


bool ReAnimEntityWidget::GetAlphaAt( int _cursor, qreal& _result )
{
	bool isOk = false;

	ReSuite& suite = m_suiteArray[ ETrackType_Alpha ];
	if( NULL != suite.m_track &&
		suite.m_buttons[ ESuiteButton_Enable ]->isChecked() )
	{
		ReAnimTrack* trackData = suite.m_track->GetModelData();
		qreal time = suite.m_track->GetValueAt( _cursor );
		QVariant data;
		if( trackData->Interpolate( time, data, false ) )
		{
			_result = data.toDouble();
			isOk = true;
		}
	}

	return isOk;
}



bool ReAnimEntityWidget::GetTransformAt( int _cursor, QTransform& _result )
{
	bool isOk = false;

	QPointF translation;
	qreal rotation = 0.0f;
	QPointF scale;

	bool hasTranslation = GetTranslationAt( _cursor, translation );
	bool hasRotation = GetRotationAt( _cursor, rotation );
	bool hasScale = GetScaleAt( _cursor, scale );

	if( hasTranslation || hasRotation || hasScale )
	{		
		_result.reset();

		if( hasScale )
			_result.scale( scale.x(), scale.y() );

		if( hasRotation )
			_result.rotate( rotation );

		if( hasTranslation )
			_result.translate( translation.x(), translation.y() );

		isOk = true;
	}

	return isOk;
}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void ReAnimEntityWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	if( IsHighlighted() )
		painter.fillRect( 0, 0, width(), height(), QColor( 0, 255, 0 ) );
	else
		painter.fillRect( 0, 0, width(), height(), QColor( 0, 0, 0 ) );

	painter.setPen( QColor( 0, 0, 0 ) );
	painter.drawText( width() - 100, 0, QString( "%1" ).arg( GetTotalHeight() ) );
	TSuper::paintEvent( _event );
}


void ReAnimEntityWidget::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );
	UpdateLayout();
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReAnimEntityWidget::OnContextMenu( const QPoint& _point )
{
	//m_editMenu->exec( mapToGlobal( _point ) );
}


void ReAnimEntityWidget::OnToggled( bool _isChecked )
{
	if( _isChecked )
		m_toggleButton->setText( tr( "Collapse" ) );
	else
		m_toggleButton->setText( tr( "Expand" ) );

	emit TotalHeightChanged();
}


void ReAnimEntityWidget::OnNewTranslationTrack()
{
	CreateTrack( ETrackType_Translation );
}


void ReAnimEntityWidget::OnNewRotationTrack()
{
	CreateTrack( ETrackType_Rotation );
}


void ReAnimEntityWidget::OnNewScaleTrack()
{
	CreateTrack( ETrackType_Scale );
}


void ReAnimEntityWidget::OnNewAlphaTrack()
{
	CreateTrack( ETrackType_Alpha );
}


void ReAnimEntityWidget::OnPositionChanged( const QPointF& _pos )
{
	ReAnimTrackWidget* trackWidget = m_suiteArray[ ETrackType_Translation ].m_track;
	if( NULL != trackWidget )
	{
		bool isDirty = false;
		ReAnimFrameWidget* frameWidget = trackWidget->GetFrameAtCursor( trackWidget->GetCursor() );
		if( NULL == frameWidget )
		{
			frameWidget = trackWidget->CreateFrameAtCursor( trackWidget->GetCursor() );
			isDirty = true;
		}

		ReAnimFrame* frameData = frameWidget->GetModelData();
		frameData->SetFrameValue( _pos.x(), 0 );
		frameData->SetFrameValue( _pos.y(), 1 );

		if( isDirty )
			update();
	}
}


void ReAnimEntityWidget::OnRotationChanged( qreal _delta )
{
	ReAnimTrackWidget* trackWidget = m_suiteArray[ ETrackType_Rotation ].m_track;
	if( NULL != trackWidget )
	{
		bool isDirty = false;
		ReAnimFrameWidget* frameWidget = trackWidget->GetFrameAtCursor( trackWidget->GetCursor() );
		if( NULL == frameWidget )
		{
			frameWidget = trackWidget->CreateFrameAtCursor( trackWidget->GetCursor() );
			isDirty = true;
		}

		ReAnimFrame* frameData = frameWidget->GetModelData();
		frameData->SetFrameValue( _delta, 0 );

		if( isDirty )
			update();
	}
}


void ReAnimEntityWidget::OnScaleChanged( const QPointF& _scale )
{
	ReAnimTrackWidget* trackWidget = m_suiteArray[ ETrackType_Scale ].m_track;
	if( NULL != trackWidget )
	{
		bool isDirty = false;
		ReAnimFrameWidget* frameWidget = trackWidget->GetFrameAtCursor( trackWidget->GetCursor() );
		if( NULL == frameWidget )
		{
			frameWidget = trackWidget->CreateFrameAtCursor( trackWidget->GetCursor() );
			isDirty = true;
		}

		ReAnimFrame* frameData = frameWidget->GetModelData();
		frameData->SetFrameValue( _scale.x(), 0 );
		frameData->SetFrameValue( _scale.y(), 1 );

		if( isDirty )
			update();
	}
}


void ReAnimEntityWidget::OnAlphaChanged( qreal _delta )
{
	ReAnimTrackWidget* trackWidget = m_suiteArray[ ETrackType_Alpha ].m_track;
	if( NULL != trackWidget )
	{
		bool isDirty = false;
		ReAnimFrameWidget* frameWidget = trackWidget->GetFrameAtCursor( trackWidget->GetCursor() );
		if( NULL == frameWidget )
		{
			frameWidget = trackWidget->CreateFrameAtCursor( trackWidget->GetCursor() );
			isDirty = true;
		}

		ReAnimFrame* frameData = frameWidget->GetModelData();
		frameData->SetFrameValue( _delta, 0 );

		if( isDirty )
			update();
	}
}


// -----------------------------------------------------------------------------
// Utilities
// -----------------------------------------------------------------------------
void ReAnimEntityWidget::InitMenu()
{
	//QAction* action = NULL;
	//m_editMenu = new QMenu( tr( "&Edit" ) );

	//action = m_editMenu->addAction( tr( "New &Translation Track" ) );
	//connect( action, SIGNAL( triggered() ), this, SLOT( OnNewTranslationTrack() ) );

	//action = m_editMenu->addAction( tr( "New &Rotation Track" ) );
	//connect( action, SIGNAL( triggered() ), this, SLOT( OnNewRotationTrack() ) );

	//action = m_editMenu->addAction( tr ( "New &Scale Track" ) );
	//connect( action, SIGNAL( triggered() ), this, SLOT( OnNewScaleTrack() ) );

	//action = m_editMenu->addAction( tr ( "New &Alpha Track" ) );
	//connect( action, SIGNAL( triggered() ), this, SLOT( OnNewAlphaTrack() ) );
}


void ReAnimEntityWidget::CreateTrack( eTrackType _type )
{
	if( NULL == m_suiteArray[ _type ].m_track )
	{
		// Model.
		ReAnimTrack* trackData = m_modelData->CreateTrack( _type );

		// UI.
		ReAnimTrackWidget* trackWidget = new ReAnimTrackWidget( trackData, _type, this );

		trackWidget->setContextMenuPolicy( Qt::CustomContextMenu );
		trackWidget->setFocusPolicy( Qt::ClickFocus );
		trackWidget->setVisible( true );
		trackWidget->SetUnit( m_ruler->GetUnit() );
		trackWidget->SetUnitValue( m_ruler->GetUnitValue() );
		trackWidget->SetSnap( m_ruler->GetSnap() );
		trackWidget->OnViewportChanged( m_ruler->GetViewport() );
		trackWidget->OnCursorChanged( m_ruler->GetCursor() );
		trackWidget->EnableDragViewport( false );
		//trackWidget->ShowCursorValue( false );
		trackWidget->EnableDragCursor( false );

		QObject::connect( m_ruler, SIGNAL( CursorChanged( int ) ), trackWidget, SLOT( OnCursorChanged( int ) ) );
		QObject::connect( m_ruler, SIGNAL( ViewportChanged( int ) ), trackWidget, SLOT( OnViewportChanged( int ) ) );

		ReSuite& suite = m_suiteArray[ _type ];
		suite.m_type = _type;
		suite.m_track = trackWidget;
		suite.m_buttons[ ESuiteButton_Enable ] = new QPushButton( tr( "E" ), this );
		suite.m_buttons[ ESuiteButton_Enable ]->setVisible( true );
		suite.m_buttons[ ESuiteButton_Enable ]->setCheckable( true );
		suite.m_buttons[ ESuiteButton_Enable ]->setChecked( true );
		suite.m_buttons[ ESuiteButton_Enable ]->setStyleSheet( "QPushButton{ text-align: center; }" );
		suite.m_buttons[ ESuiteButton_Delete ] = new QPushButton( tr( "D" ), this );
		suite.m_buttons[ ESuiteButton_Delete ]->setVisible( true );
		suite.m_buttons[ ESuiteButton_Delete ]->setCheckable( true );
		suite.m_buttons[ ESuiteButton_Delete ]->setStyleSheet( "QPushButton{ text-align: center; }" );

		UpdateLayout();
	}
}


void ReAnimEntityWidget::UpdateLayout()
{
	// Toggle button.
	int toggleButtonX = GetLeftPadding();
	int toggleButtonY = GetTopPadding();
	int toggleButtonW = ANIM_ENTITY_HEADER_WIDTH;
	int toggleButtonH = ANIM_ENTITY_SUITE_HEIGHT;
	m_toggleButton->setGeometry( toggleButtonX, toggleButtonY, toggleButtonW, toggleButtonH );

	// Label.
	int labelX = GetLeftPadding() + ANIM_ENTITY_HEADER_WIDTH + GetHorizontalGap();
	int labelY = toggleButtonY;
	m_label->move( labelX, labelY );

	// Suites.
	int suiteX = GetLeftPadding();
	int suiteY = toggleButtonY + toggleButtonH + GetVerticalGap();
	int suiteButtonHorizontalGap = 4;
	int suiteButtonWidth = ANIM_ENTITY_SUITE_HEIGHT;
	int suiteButtonHeight = ANIM_ENTITY_SUITE_HEIGHT;
	int trackX = labelX;
	int trackY = labelY + ANIM_ENTITY_SUITE_HEIGHT + GetVerticalGap();
	int trackW = width() - GetLeftPadding() - GetRightPadding() - ANIM_ENTITY_HEADER_WIDTH - GetHorizontalGap();
	int trackH = ANIM_ENTITY_SUITE_HEIGHT;

	for( int i = 0, validSuiteCount = 0; i < ETrackType_Count; ++i )
	{
		ReSuite& suite = m_suiteArray[ i ];
		if( NULL != suite.m_track )
		{
			// Suite buttons.
			for( int j = 0; j < ESuiteButton_Count; ++j )
			{
				if( NULL != suite.m_buttons[ j ] )
				{
					int buttonX = suiteX + j * ( suiteButtonWidth + GetHorizontalGap() );
					int buttonY = suiteY + validSuiteCount * ( suiteButtonHeight + GetVerticalGap() );
					int buttonW = suiteButtonWidth;
					int buttonH = suiteButtonHeight;

					suite.m_buttons[ j ]->setGeometry( buttonX, buttonY, buttonW, buttonH );
				}
			}

			// Track.
			suite.m_track->setGeometry( trackX, trackY + validSuiteCount * ( trackH + GetVerticalGap() ), trackW, trackH );

			// Keep count of valid suites.
			++validSuiteCount;
		}
	}

	// Notify change of total height.
	emit TotalHeightChanged();
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
