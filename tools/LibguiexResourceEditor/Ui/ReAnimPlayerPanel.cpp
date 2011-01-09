// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimModel.h"
#include "Ui\ReAnimPlayerPanel.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReAnimUiInfo.h"
#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QAction>


namespace
{
	const int BUTTON_SIZE = 16;
}


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimPlayerPanel::ReAnimPlayerPanel( ReAnimModel* _model, QWidget* _parent )
: TSuper( _parent )
, m_animModel( _model )
, m_rulerWidget( NULL )
, m_modeComboBox( NULL )
{
	GetTopPaddingRef() = 2;
	GetLeftPaddingRef() = 2;

	// Buttons.
	m_buttons[ EButton_Play ]		= new QPushButton( QIcon( ":/res/image/editor_anim_play.png" ), tr( "" ), this );
	m_buttons[ EButton_Stop ]		= new QPushButton( QIcon( ":/res/image/editor_anim_stop.png" ), tr( "" ), this );
	m_buttons[ EButton_GotoBegin ]	= new QPushButton( QIcon( ":/res/image/editor_anim_gotobegin.png" ), tr( "" ), this );
	m_buttons[ EButton_GotoEnd ]	= new QPushButton( QIcon( ":/res/image/editor_anim_gotoend.png" ), tr( "" ), this );
	m_buttons[ EButton_More ]		= new QPushButton( QIcon( ":/res/image/editor_anim_more.png" ), tr( "" ), this );
	for( int i = 0; i < EButton_Count; ++i )
		m_buttons[ i ]->setDisabled( true );

	// Play mode combo box.
	m_modeComboBox = new QComboBox( this );
	m_modeComboBox->setVisible( true );
	m_modeComboBox->addItem( tr( "Loop" ) );
	m_modeComboBox->addItem( tr( "Once" ) );
	m_modeComboBox->addItem( tr( "BounceOnce" ) );
	m_modeComboBox->addItem( tr( "BounceLoop" ) );
	m_modeComboBox->setCurrentIndex( 0 );

	// Ruler.
	m_rulerWidget = new ReRulerWidget( this );
	m_rulerWidget->setVisible( true );
	m_rulerWidget->SetRulerHeight( ANIM_RULER_HEIGHT );
	m_rulerWidget->SetUnit( 10 );
	m_rulerWidget->SetUnitValue( 1.0f / ( qreal )ANIM_FPS_DEFAULT );
	m_rulerWidget->SetSnap( ANIM_RULER_SNAP );
	m_rulerWidget->SetLongMarkDevision( 1 );
	m_rulerWidget->SetMarkOnSizeA( false );

	// Player.
	m_player.Setup( m_rulerWidget );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimPlayerPanel::paintEvent( QPaintEvent* _event )
{
	//QPainter painter( this );
	//painter.fillRect( _event->rect(), QColor( 121, 232, 232 ) );
}


void ReAnimPlayerPanel::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );

	UpdateLayout();
}


// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
void ReAnimPlayerPanel::OnTogglePlay()
{
	if( m_player.IsPlaying() )
	{
		m_player.Stop();
	}
	else
	{
		qreal startValue = m_rulerWidget->GetValueAt( m_rulerWidget->GetCursor() );
		qreal endValue = m_animModel->GetTotalLength();
		if( startValue < endValue )
		{
			int mode = m_modeComboBox->currentIndex();
			m_player.Play( startValue, endValue, ( ReAnimPlayer::ePlayMode )mode );
		}
	}
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------
void ReAnimPlayerPanel::UpdateLayout()
{
	// Player buttons.
	int x = GetLeftPadding();
	int y = GetTopPadding();
	int w = ANIM_PLAYER_BUTTON_SIZE;
	int h = ANIM_PLAYER_BUTTON_SIZE;

	for( int i = 0; i < EButton_Count; ++i )
	{
		m_buttons[ i ]->setGeometry( x, y, w, h );
		x += w;
	}

	// Play mode combo box.
	x = GetLeftPadding();
	y = y + ANIM_PLAYER_BUTTON_SIZE;
	w = ANIM_HEADER_WIDTH;
	h = ANIM_PLAYER_BUTTON_SIZE;
	m_modeComboBox->setGeometry( x, y, w, h );

	// Player track.
	x = GetLeftPadding() + ANIM_HEADER_WIDTH + ANIM_CONSOLE_H_GAP;
	y = GetTopPadding();
	w = width() - x;
	h = ANIM_PLAYER_BUTTON_SIZE * 2;
	m_rulerWidget->setGeometry( x, y, w, h );
	m_rulerWidget->SetRulerHeight( h );
}


}
