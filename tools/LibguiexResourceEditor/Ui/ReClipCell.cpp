// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110118.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "UI\ReClipCell.h"
#include "UI\ReClipImage.h"
#include "Core\ReClipModel.h"
#include "Core\ReZoomInfo.h"
#include <QMessageBox>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReClipCell::ReClipCell( QWidget* _parent )
: TSuper( _parent )
, m_clipImage( NULL )
{
	setContextMenuPolicy( Qt::NoContextMenu );
	setFocusPolicy( Qt::ClickFocus );
	
	m_zoomInfo.Init( 1, 1, 6, 1 );

	m_clipImage = new ReClipImage( &m_zoomInfo, this );
}


void ReClipCell::InitFromModelData( ReClipNodeGroup* _modelData )
{
	m_clipImage->InitFromModelData( _modelData );
	m_clipImage->move( 0, 0 );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReClipCell::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );
	painter.fillRect( geometry(), QColor( 200, 200, 180 ) );
}


void ReClipCell::wheelEvent( QWheelEvent* _event )
{
	if( NULL != m_clipImage )
	{
		int degrees = _event->delta() / 8;
		int stepCount = degrees / 15;

		m_zoomInfo.Zoom( stepCount );
	}
}


void ReClipCell::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragInfo.SetCursorPosBackup( _event->pos() );
		m_dragInfo.SetItemPosBackup( m_clipImage->pos() );
		m_dragInfo.StartMove();
	}
}


void ReClipCell::mouseReleaseEvent( QMouseEvent* _event )
{
	m_dragInfo.Stop();
}


void ReClipCell::mouseMoveEvent( QMouseEvent* _event )
{
	if( Qt::MidButton & _event->buttons() )
	{
		if( m_dragInfo.IsMoving() )
		{
			QPoint delta = _event->pos() - m_dragInfo.GetCursorPosBackup();
			m_clipImage->move( m_dragInfo.GetItemPosBackup() + delta );
		}
	}
}


void ReClipCell::mouseDoubleClickEvent( QMouseEvent* _event )
{
	if( NULL != m_modelData )
	{
		m_zoomInfo.Reset( 1 );
		m_clipImage->move( 0, 0 );
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

}
