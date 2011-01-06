// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimDef.h"
#include "Core\ReAnimModel.h"
#include "Core\ReAnimFrame.h"
#include "Ui\ReAnimConsoleWidget.h"
#include "Ui\ReAnimTrackWidget.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReAnimEntityWidget.h"
#include "Ui\ReAnimGraphicsItem.h"
#include "Ui\ReAnimUiInfo.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <QGraphicsScene>


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
, m_scene( _scene )
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
	InitMenus();

	GetVerticalGapRef() = 4;

	connect( m_scene, SIGNAL( selectionChanged() ), this, SLOT( OnSceneSelectionChanged() ) );
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

	// Background.
	painter.fillRect( 0, 0, width(), height(), QColor( 100, 100, 100 ) );

	TSuper::paintEvent( _event );
}


void ReAnimConsoleWidget::resizeEvent( QResizeEvent* _event )
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

	UpdateLayout();
}


// ----------------------------------------------------------------------------
// Override ReBaseWidget.
// ----------------------------------------------------------------------------
QMenu* ReAnimConsoleWidget::GetEditMenu() const
{
	QMenu* result = m_editMenu;

	QWidget* child = focusWidget();
	if( NULL != child )
	{
		ReAnimTrackWidget* track = dynamic_cast< ReAnimTrackWidget* >( child );
		if( NULL != track )
			result = track->GetEditMenu();
	}

	return result;
}



// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
// A ReAnimGraphicsItem object has been created in a Qt Graphics View Architecture.
// It's an indication to create a ReAnimEntityWidget object as a UI representation
// in the console, and a ReAnimEntity object in the animation model as its data
// representation.
// The ReAnimGraphicsItem object and ReAnimEntityWidget object are combined in
// a ReAnimEntityInfo object.
void ReAnimConsoleWidget::OnItemAdded( ReAnimGraphicsItem* _item )
{
	if( NULL != m_animMode && NULL != _item )
	{
		ReAnimEntity* entityData = m_animMode->CreateEntity();

		ReAnimEntityWidget* entityWidget = new ReAnimEntityWidget( entityData, m_rulerWidget, this );
		entityWidget->SetModelData( entityData );
		entityWidget->setContextMenuPolicy( Qt::CustomContextMenu );
		entityWidget->setVisible( true );

		entityWidget->GetLeftPaddingRef() = m_entityWidgetPadding;
		entityWidget->GetRightPaddingRef() = m_entityWidgetPadding;
		entityWidget->GetTopPaddingRef() = m_entityWidgetPadding;
		entityWidget->GetBottomPaddingRef() = m_entityWidgetPadding;
		entityWidget->GetVerticalGapRef() = m_entityWidgetGap;
		entityWidget->GetHorizontalGapRef() = m_entityWidgetGap;

		ReAnimEntityInfo info;
		info.m_graphicsItem = _item;
		info.m_entityWidget = entityWidget;
		m_animEntityList.push_back( info );
		
		connect( _item, SIGNAL( PositionChanged( const QPointF& ) ), entityWidget, SLOT( OnPositionChanged( const QPointF& ) ) );
		connect( _item, SIGNAL( RotationChanged( qreal ) ), entityWidget, SLOT( OnRotationChanged( qreal ) ) );
		connect( _item, SIGNAL( ScaleChanged( const QPointF& ) ), entityWidget, SLOT( OnScaleChanged( const QPointF& ) ) );
		connect( _item, SIGNAL( AlphaChanged( qreal ) ), entityWidget, SLOT( OnAlphaChanged( qreal ) ) );
		entityWidget->OnPositionChanged( _item->pos() );
		entityWidget->OnRotationChanged( 0.0f );
		entityWidget->OnScaleChanged( QPointF( 1.0f, 1.0f ) );
		entityWidget->OnAlphaChanged( 1.0f );

		connect( m_rulerWidget, SIGNAL( CursorChanged( int ) ), this, SLOT( OnRulerCursorChanged( int ) ) );
		connect( entityWidget, SIGNAL( TotalHeightChanged() ), this, SLOT( UpdateLayout() ) );

		UpdateLayout();
	}
}


void ReAnimConsoleWidget::UpdateLayout()
{
	int x = GetLeftPadding();
	int y = GetTopPadding() + m_rulerHeight + GetVerticalGap();
	int w = width() - GetLeftPadding() - GetRightPadding();

	TAnimEntityListItor itor = m_animEntityList.begin();
	TAnimEntityListItor itorEnd = m_animEntityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& item = *itor;
		int h = item.m_entityWidget->GetTotalHeight();
		item.m_entityWidget->setGeometry( x, y, w, h );

		y += h + GetVerticalGap();
	}
}


void ReAnimConsoleWidget::OnSceneSelectionChanged()
{	
	QList< QGraphicsItem* > selection = m_scene->selectedItems();

	TAnimEntityListItor itor = m_animEntityList.begin();
	TAnimEntityListItor itorEnd = m_animEntityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& info = *itor;
		info.m_entityWidget->Highlight( false );

		QList< QGraphicsItem* >::Iterator selItor = selection.begin();
		QList< QGraphicsItem* >::Iterator selItorEnd = selection.end();
		for( ; selItor != selItorEnd; ++selItor )
		{
			QGraphicsItem* sel = *selItor;
			if( info.m_graphicsItem == sel )
			{
				info.m_entityWidget->Highlight( true );
			}
		}
	}
}


void ReAnimConsoleWidget::OnRulerCursorChanged( int _cursor )
{
	TAnimEntityListItor itor = m_animEntityList.begin();
	TAnimEntityListItor itorEnd = m_animEntityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& info = *itor;
		ReAnimEntityWidget* entityWidget = info.m_entityWidget;
		ReAnimGraphicsItem* graphicsItem = info.m_graphicsItem;

		QPointF translation;
		qreal rotation = 0.0f;
		QPointF scale;
		qreal alpha = 1.0f;

		bool hasTranslation = entityWidget->GetTranslationAt( _cursor, translation );
		bool hasRotation = entityWidget->GetRotationAt( _cursor, rotation );
		bool hasScale = entityWidget->GetScaleAt( _cursor, scale );
		bool hasAlpha = entityWidget->GetAlphaAt( _cursor, alpha );
		if( hasTranslation || hasRotation || hasScale )
		{
			graphicsItem->setVisible( true );
			graphicsItem->resetTransform();
			if( hasTranslation )
				//graphicsItem->translate( translation.x(), translation.y() );	// Come back later...
				graphicsItem->setPos( translation.x(), translation.y() );
			if( hasRotation )
				graphicsItem->rotate( rotation );
			if( hasScale )
				graphicsItem->scale( scale.x(), scale.y() );

			if( hasAlpha )
				graphicsItem->SetAlpha( alpha );
		}
		else
		{
			graphicsItem->setVisible( false );
		}
	}
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------
void ReAnimConsoleWidget::InitMenus()
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


void ReAnimConsoleWidget::InitRuler()
{
	// Widgets.
	m_rulerWidget = new ReRulerWidget( this );
	m_rulerWidget->setVisible( true );
	m_rulerWidget->SetUnit( 10 );
	m_rulerWidget->SetSnap( ANIM_RULER_SNAP );
	m_rulerWidget->SetLongMarkDevision( 1 );
	m_rulerWidget->SetMarkOnSizeA( false );

	int rulerX = ANIM_ENTITY_HEADER_WIDTH + m_entityWidgetGap + m_entityWidgetPadding;
	int rulerY = m_topMargin;
	int rulerW = width() - rulerX - m_rightMargin;
	int rulerH = m_rulerHeight;
	m_rulerWidget->setGeometry( QRect( rulerX, rulerY, rulerW, rulerH ) );
}


}
