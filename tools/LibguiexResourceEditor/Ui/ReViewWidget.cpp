// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include <QPainter>
#include <QResizeEvent>
#include "UI\ReViewWidget.h"
#include "UI\ReGlWidget.h"


namespace RE
{


// -------------------------------------------------------------------------
// General
// -------------------------------------------------------------------------
ReViewWidget::ReViewWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_glWidget( NULL )
{
	m_glWidget = new ReGlWidget( this );
}


// -------------------------------------------------------------------------
// Override QWidget.
// -------------------------------------------------------------------------
void ReViewWidget::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


void ReViewWidget::resizeEvent( QResizeEvent* _event )
{
	m_glWidget->setGeometry( QRect( QPoint( 0, 0 ), _event->size() ) );
}


// -------------------------------------------------------------------------
// Override ReBaseWidget.
// -------------------------------------------------------------------------
void ReViewWidget::Tick( qreal _delta )
{
	m_glWidget->Tick( _delta );
}


}	// namespace RE
