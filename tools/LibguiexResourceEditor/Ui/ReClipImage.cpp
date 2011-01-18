// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101115.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReClipUiInfo.h"
#include "Ui\ReClipImage.h"
#include "Ui\ReClipWidget.h"
#include "Core\ReClipModel.h"
#include "Core\ReZoomInfo.h"


namespace RE
{


// -----------------------------------------------------------------------------
// ReClipSelection.
// -----------------------------------------------------------------------------
void ReClipImage::ReClipSelection::Add( QWidget* _widget )
{
	( ( ReClipWidget* )_widget )->ShowOutline( true ); 
	TSuper::Add( _widget ); 
}


void ReClipImage::ReClipSelection::Remove( QWidget* _widget )
{
	( ( ReClipWidget* )_widget )->ShowOutline( false ); 
	TSuper::Remove( _widget ); 
}


void ReClipImage::ReClipSelection::Clear()
{
	TItemListItor itor = Begin();
	TItemListItor itorEnd = End();
	for( ; itor != itorEnd; ++itor )
	{
		ReClipWidget* widget = ( ReClipWidget* )( *itor ).GetWidget();
		widget->ShowOutline( false );
	}

	TSuper::Clear();
}


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReClipImage::ReClipImage( ReZoomInfo* _zoomInfo, QWidget* _parent )
: TSuper( _parent )
, m_modelData( NULL )
, m_zoomScalar( 1 )
{
	//setMouseTracking( true );
	setScaledContents( true );
	setFocusPolicy( Qt::ClickFocus );
	setVisible( true );

	connect( _zoomInfo, SIGNAL( ScalarChanged( int ) ), this, SLOT( OnZoom( int ) ) );

	resize( 64, 64 );
}


ReClipImage::~ReClipImage()
{
}


void ReClipImage::InitFromModelData( ReClipNodeGroup* _modelData )
{
	Reset();

	m_modelData = _modelData;
	setPixmap( *m_modelData->GetImage() );
	setScaledContents( true );
	adjustSize();	

	int clipCount = m_modelData->GetChildrenCount();
	for( int i = 0; i < clipCount; ++i )
	{
		ReClipNode* clipData = ( ReClipNode* )m_modelData->GetChild( i );

		ReClipWidget* clipWidget = new ReClipWidget( this );
		clipWidget->InitFromModelData( clipData );
	}
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReClipImage::paintEvent( QPaintEvent* _event )
{
	if( NULL == m_modelData )
	{
		QPainter painter( this );
		painter.fillRect( _event->rect(), QColor( 168, 170, 100 ) );
		painter.setPen( QColor( 0, 0, 255 ) );
		painter.drawRect( _event->rect().adjusted( 0, 0, -1, -1 ) );
	}
	else
	{
		TSuper::paintEvent( _event );
	}	
}


void ReClipImage::mousePressEvent( QMouseEvent* _event )
{
	QPoint cursor = RoundPoint( _event->pos(), m_zoomScalar );

	if( Qt::LeftButton == _event->button() )
	{
		QWidget* widget = childAt( _event->pos() );
		if( NULL != widget )
		{
			// Clicking on a clip triggers the selection/move/resize routine.
			ReClipWidget* clipWidget = dynamic_cast< ReClipWidget* >( widget );

			if( Qt::ControlModifier & _event->modifiers() )
			{
				if( m_selection.Find( clipWidget ) )
					m_selection.Remove( clipWidget );
				else
					m_selection.Add( clipWidget );

				m_selection.GotoIdleState();
			}
			else
			{
				if( !m_selection.Find( clipWidget ) )
				{
					m_selection.Clear();
					m_selection.Add( clipWidget );
				}

				bool doResize = false;

				if( 1 == m_selection.Size() )
				{
					ReDragInfo::eResize resizeType = clipWidget->CalcResizeType( clipWidget->mapFromParent( cursor ) );
					if( ReDragInfo::EResize_None != resizeType )
					{
						doResize = true;
						m_dragInfo.SetCursorPosBackup( cursor );
						m_dragInfo.SetItemPosBackup( clipWidget->pos() );
						m_dragInfo.SetItemSizeBackup( clipWidget->size() );
						m_dragInfo.SetClip( clipWidget );
						m_dragInfo.StartResize( resizeType );
					}
				}				

				if( !doResize )
				{
					m_selection.GetDragInfoRef().SetCursorPosBackup( cursor );
					m_selection.GotoWaitState();
				}
			}
		}
		else if( !( Qt::ControlModifier & _event->modifiers() ) )
		{
			m_selection.Clear();

			// Clicking on the image triggers the creation of a new clip.
			ReClipWidget* newWidget = CreateClip();
			QPoint pos = cursor;
			newWidget->resize( 1, 1 );
			newWidget->move( pos );
			m_selection.Add( newWidget );

			m_dragInfo.SetCursorPosBackup( cursor );
			m_dragInfo.SetClip( newWidget );
			m_dragInfo.StartCreate();
		}
	}
	else if( Qt::MidButton == _event->button() )
	{
		_event->ignore();
	}
}


void ReClipImage::mouseReleaseEvent( QMouseEvent* _event )
{
	if( m_dragInfo.IsCreating() )
	{
		ReClipWidget* newClip = m_dragInfo.GetClip();
		if( IsClipValid( newClip ) )
		{
			m_clipList.Add( newClip );
			m_selection.Clear();
			m_selection.Add( newClip );
		}
		else
		{
			m_modelData->GetModel()->OnClipDeleted( newClip->GetModelData() );
			m_selection.Clear();
			delete newClip;
		}
	}
	else
	{
		m_selection.GotoIdleState();
		m_selection.GetDragInfoRef().Stop();

		HandleClipping();
	}
	
	m_dragInfo.Stop();
}


void ReClipImage::mouseMoveEvent( QMouseEvent* _event )
{
	QPoint cursor = RoundPoint( _event->pos(), m_zoomScalar );

	if( Qt::MidButton & _event->buttons() )
	{
		_event->ignore();
	}
	else
	{
		if( m_dragInfo.IsCreating() )
		{
			QPoint cursorBackup = m_dragInfo.GetCursorPosBackup();
			QPoint pos(	cursor.x() <= cursorBackup.x() ? cursor.x() : cursorBackup.x(),
				cursor.y() <= cursorBackup.y() ? cursor.y() : cursorBackup.y() );
			pos = RoundPoint( pos, m_zoomScalar );
			QPoint size = AbsPoint( cursor - cursorBackup );

			ReClipWidget* newClip = m_dragInfo.GetClip();
			newClip->resize( PointToSize( size ) );
			newClip->move( pos );
		}
		else if( m_dragInfo.IsResizing() )
		{
			HandleResize( cursor );
		}
		else
		{
			if( m_selection.IsWaitState() )
			{
				m_selection.GotoMoveState();
			}

			if( m_selection.IsMoveState() )
			{
				QPoint delta = RoundPoint( cursor - m_selection.GetDragInfoRef().GetCursorPosBackup(), m_zoomScalar );
				if( !delta.isNull() )
				{
					m_selection.DragMoveBy( delta );
				}
			}
		}
	}
}


void ReClipImage::keyPressEvent( QKeyEvent* _event )
{
	if( Qt::Key_Left == _event->key() )
	{
		m_selection.MoveBy( QPoint( -m_zoomScalar, 0 ) );
		HandleClipping();
	}
	else if( Qt::Key_Right == _event->key() )
	{
		m_selection.MoveBy( QPoint( m_zoomScalar, 0 ) );
		HandleClipping();
	}
	else if( Qt::Key_Up == _event->key() )
	{
		m_selection.MoveBy( QPoint( 0, -m_zoomScalar ) );
		HandleClipping();
	}
	else if( Qt::Key_Down == _event->key() )
	{
		m_selection.MoveBy( QPoint( 0, m_zoomScalar ) );
		HandleClipping();
	}
	else if( Qt::Key_A == _event->key() )
	{
		if( Qt::ControlModifier & _event->modifiers() )
		{
			m_selection.Clear();

			TClipListItor itor = m_clipList.Begin();
			TClipListItor itorEnd = m_clipList.End();
			for( ; itor != itorEnd; ++itor )
			{
				m_selection.Add( *itor );
			}
		}
	}
	else if( Qt::Key_Escape == _event->key() )
	{
		m_selection.Clear();
	}
	else if( Qt::Key_Delete == _event->key() )
	{
		OnDelete();
	}
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReClipImage::OnZoom( int _scalar )
{
	if( m_zoomScalar != _scalar )
	{
		resize( pixmap()->size() * _scalar );

		TClipListItor itor = m_clipList.Begin();
		TClipListItor itorEnd = m_clipList.End();
		for( ; itor != itorEnd; ++itor )
		{
			ReClipWidget* clipWidget = *itor;
			QPoint pos = ZoomPoint( clipWidget->pos(), m_zoomScalar, _scalar );
			QSize size = ZoomSize( clipWidget->size(), m_zoomScalar, _scalar );

			clipWidget->resize( size );
			clipWidget->move( pos );
			ReClipNode* clipData = clipWidget->GetModelData();
			clipData->SetZoomScalar( _scalar );
		}

		m_zoomScalar = _scalar;
		m_modelData->SetZoomScalar( m_zoomScalar );
		update();
	}
}


void ReClipImage::OnDelete()
{
	ReClipSelection buffer = m_selection;
	ReClipSelection::TItemListItor itor = buffer.Begin();
	ReClipSelection::TItemListItor itorEnd = buffer.End();
	for( ; itor != itorEnd; ++itor )
	{
		ReClipWidget* clipWidget = ( ReClipWidget* )( *itor ).GetWidget();
		m_clipList.Remove( clipWidget );
		m_selection.Remove( clipWidget );

		m_modelData->GetModel()->OnClipDeleted( clipWidget->GetModelData() );
		delete clipWidget;
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
ReClipWidget* ReClipImage::CreateClip()
{
	// Model.
	ReClipModel* clipModel = m_modelData->GetModel();
	ReClipNode* clipData = clipModel->CreateClip( m_modelData );
	clipData->SetZoomScalar( m_zoomScalar );

	// Widget.
	ReClipWidget* clipWidget = new ReClipWidget( this );
	clipWidget->InitFromModelData( clipData );
	clipWidget->setVisible( true );

	return clipWidget;
}


bool ReClipImage::IsClipValid( const ReClipWidget* _clip ) const
{
	return ( _clip->width() / m_zoomScalar ) >= CLIP_MIN_WIDTH && ( _clip->height() / m_zoomScalar ) >= CLIP_MIN_HEIGHT;
}


void ReClipImage::HandleResize( const QPoint _pos )
{
	ReClipWidget* clipWidget = m_dragInfo.GetClip();
	QPoint delta = RoundPoint( _pos - m_dragInfo.GetCursorPosBackup(), m_zoomScalar );

	int deltaX = 0;
	int deltaY = 0;
	int deltaW = 0;
	int deltaH = 0;

	ReDragInfo::eResize rt = m_dragInfo.GetResizeType();

	// Size.
	if( ReDragInfo::EResize_Right == rt || 
		ReDragInfo::EResize_RightTop == rt || 
		ReDragInfo::EResize_RightBottom == rt )
	{
		deltaW = delta.x();
	}
	if( ReDragInfo::EResize_Left == rt ||
		ReDragInfo::EResize_LeftTop == rt ||
		ReDragInfo::EResize_LeftBottom == rt )
	{
		deltaW = -delta.x();
	}
	if( ReDragInfo::EResize_Top == rt ||
		ReDragInfo::EResize_LeftTop == rt ||
		ReDragInfo::EResize_RightTop == rt )
	{
		deltaH = -delta.y();
	}
	if( ReDragInfo::EResize_Bottom == rt ||
		ReDragInfo::EResize_LeftBottom == rt ||
		ReDragInfo::EResize_RightBottom == rt )
	{
		deltaH = delta.y();
	}

	// Position.
	if( ReDragInfo::EResize_Left == rt ||
		ReDragInfo::EResize_LeftTop == rt ||
		ReDragInfo::EResize_LeftBottom == rt )
	{
		deltaX = delta.x();
	}
	if( ReDragInfo::EResize_Top == rt ||
		ReDragInfo::EResize_LeftTop == rt ||
		ReDragInfo::EResize_RightTop == rt )
	{
		deltaY = delta.y();
	}

	QPoint pos = m_dragInfo.GetItemPosBackup();
	pos.setX( qMin( pos.x() + deltaX, m_dragInfo.GetItemPosBackup().x() + m_dragInfo.GetItemSizeBackup().width() - CLIP_MIN_WIDTH * m_zoomScalar ) );
	pos.setY( qMin( pos.y() + deltaY, m_dragInfo.GetItemPosBackup().y() + m_dragInfo.GetItemSizeBackup().height() - CLIP_MIN_HEIGHT * m_zoomScalar ) );

	QSize size = m_dragInfo.GetItemSizeBackup();
	size.setWidth( qMax( size.width() + deltaW, CLIP_MIN_WIDTH * m_zoomScalar ) );
	size.setHeight( qMax( size.height() + deltaH, CLIP_MIN_HEIGHT * m_zoomScalar ) );

	clipWidget->resize( size );
	clipWidget->move( pos );
}


void ReClipImage::HandleClipping()
{
	if( m_selection.Size() > 0 )
	{
		TClipList garbage;
		QRect rect( 0, 0, width(), height() );

		ReClipSelection::TItemListItor itor = m_selection.Begin();
		ReClipSelection::TItemListItor itorEnd = m_selection.End();
		for( ; itor != itorEnd; ++itor )
		{
			ReClipWidget* clipWidget = ( ReClipWidget* )( *itor ).GetWidget();
			QRect geo = clipWidget->geometry();

			if( rect.intersects( geo ) )
			{
				geo = rect.intersected( geo );

				if( ( geo.width() / m_zoomScalar >= CLIP_MIN_WIDTH ) && ( geo.height() / m_zoomScalar ) >= CLIP_MIN_HEIGHT )
				{
					clipWidget->setGeometry( geo );
				}
				else
				{
					garbage.Add( clipWidget );
				}
			}
			else
			{
				garbage.Add( clipWidget );
			}
		}

		TClipListItor gItor = garbage.Begin();
		TClipListItor gItorEnd = garbage.End();
		for( ; gItor != gItorEnd; ++gItor )
		{
			ReClipWidget* clipWidget = *gItor;
			m_selection.Remove( clipWidget );
			m_clipList.Remove( clipWidget );

			m_modelData->GetModel()->OnClipDeleted( clipWidget->GetModelData() );
			delete clipWidget;
		}
	}
}


void ReClipImage::Reset()
{
	m_modelData = NULL;
	m_selection.Clear();
	m_clipList.Destroy();
	m_zoomScalar = 1;
}


}
