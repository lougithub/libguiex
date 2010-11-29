// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101121.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReClipModel.h"
#include "UI\ReClipModelWidget.h"
#include "UI\ReClipWidget.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReClipModelWidget::ReClipModelWidget( ReClipModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_model( _model )
, m_currentClip( NULL )
{
	setMouseTracking( true );
}


// -----------------------------------------------------------------------------
// Override QLabel.
// -----------------------------------------------------------------------------
void ReClipModelWidget::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );

	QPainter painter( this );
	painter.drawText( 0, 20, m_debugInfo );
}


void ReClipModelWidget::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		_event->ignore();
	}
	else if( IsReadyForEdit() )
	{
		if( Qt::LeftButton == _event->button() )
		{
			QWidget* child = childAt( _event->pos() );
			ReClipWidget* clip = qobject_cast< ReClipWidget* >( child );

			if( NULL == clip )
			{
				m_currentClip = CreateClip();
				m_currentClip->move( _event->pos() );
				m_currentClip->resize( 0, 0 );
				m_currentClip->PushData();
				m_currentClip->ShowOutline( true );

				m_currentClip->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
				m_currentClip->GetDragInfoRef().SetItemPosBackup( m_currentClip->pos() );
				m_currentClip->GetDragInfoRef().SetItemSizeBackup( m_currentClip->size() );
				m_currentClip->GetDragInfoRef().StartResize( ReDragInfo::EResize_RightBottom );
			}
			else
			{
				m_currentClip = clip;
				m_currentClip->ShowOutline( true );
				ReDragInfo::eResize resizeDir = m_currentClip->CalcResizeType( _event->pos() - m_currentClip->pos() );
				if( ReDragInfo::EResize_None != resizeDir )
				{
					m_currentClip->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
					m_currentClip->GetDragInfoRef().SetItemPosBackup( m_currentClip->pos() );
					m_currentClip->GetDragInfoRef().SetItemSizeBackup( m_currentClip->size() );
					m_currentClip->GetDragInfoRef().StartResize( resizeDir );
				}
				else
				{
					m_currentClip->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
					m_currentClip->GetDragInfoRef().SetItemPosBackup( m_currentClip->pos() );
					m_currentClip->GetDragInfoRef().StartMove();
				}
			}
		}
	}
}


void ReClipModelWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		_event->ignore();
	}
	else if( IsReadyForEdit() )
	{
		if( NULL != m_currentClip  )
		{
			m_currentClip->GetDragInfoRef().Stop();

			TrimClip( m_currentClip );
			if( !ValidateClip( m_currentClip ) )
			{				
				RecycleData( m_currentClip );
			}

			m_currentClip->ShowOutline( false );
			m_currentClip = NULL;
		}

		setCursor( QCursor( Qt::ArrowCursor ) );
	}
}


void ReClipModelWidget::mouseMoveEvent( QMouseEvent* _event )
{
	m_cursor = _event->pos();

	if( Qt::MidButton & _event->buttons() )
	{
		_event->ignore();
	}
	else if( IsReadyForEdit() )
	{
		if( NULL != m_currentClip )
		{
			if( m_currentClip->GetDragInfo().IsMoving() )
			{
				QPoint delta = _event->pos() - m_currentClip->GetDragInfo().GetCursorPosBackup();
				m_currentClip->move( m_currentClip->GetDragInfo().GetItemPosBackup() + delta );
				m_currentClip->PushData();
			}
			else if( m_currentClip->GetDragInfo().IsResizing() )
			{
				QRect rect = geometry();
				QRect geo = m_currentClip->geometry();

				ReDragInfo& dragInfo = m_currentClip->GetDragInfoRef();
				QPoint point =_event->pos();
				QPoint delta = point - dragInfo.GetCursorPosBackup();				
				QSize minSize = ReClipWidget::ms_minSize;

				int newX = dragInfo.GetItemPosBackup().x();//m_currentClip->pos().x();
				int newY = dragInfo.GetItemPosBackup().y();//m_currentClip->pos().y();
				int newW = dragInfo.GetItemSizeBackup().width();//m_currentClip->size().width();
				int newH = dragInfo.GetItemSizeBackup().height();//m_currentClip->size().height();

				QPoint pointBackup = point;
				if( pointBackup.x() < 0 )
					pointBackup.setX( 0 );
				if( pointBackup.y() < 0 )
					pointBackup.setY( 0 );		

				if( dragInfo.IsResizeL() || dragInfo.IsResizeLT() || dragInfo.IsResizeLB() )
				{
					QRect geoProbe = geo;
					geoProbe.setLeft( geoProbe.left() + delta.x() );
					
					//if( geoProbe.width() >= minSize.width() )
					{
						newX += delta.x();
						newW -= delta.x();
					}
				}
				else if( dragInfo.IsResizeR() || dragInfo.IsResizeRB() || dragInfo.IsResizeRT() )
				{
					QRect geoProbe = geo;
					geoProbe.setRight( geoProbe.right() + delta.x() );

					//if( geoProbe.width() >= minSize.width() )
					{
						newW += delta.x();
					}
				}

				if( dragInfo.IsResizeT() || dragInfo.IsResizeLT() || dragInfo.IsResizeRT() )
				{
					QRect geoProbe = geo;
					geoProbe.setTop( geoProbe.top() + delta.y() );

					//if( geoProbe.height() >= minSize.height() )
					{
						newY += delta.y();
						newH -= delta.y();
					}
				}
				else if( dragInfo.IsResizeB() || dragInfo.IsResizeRB() || dragInfo.IsResizeLB() )
				{
					QRect geoProbe = geo;
					geoProbe.setBottom( geoProbe.bottom() + delta.y() );

					//if( geoProbe.height() >= minSize.height() )
					{
						newH += delta.y();
					}
				}

				m_currentClip->resize( newW, newH );
				m_currentClip->move( newX, newY );
				m_currentClip->PushData();
			}
		}
		else
		{
			// Check on cursor only when we are not drag-moving or drag-resizing.

			ReClipWidget* clipWidget = dynamic_cast< ReClipWidget* >( childAt( _event->pos() ) );
			if( NULL != clipWidget )
			{
				ReDragInfo::eResize resizeDir = clipWidget->CalcResizeType( _event->pos() - clipWidget->pos() );

				if( ReDragInfo::EResize_LeftTop == resizeDir || ReDragInfo::EResize_RightBottom == resizeDir )
					setCursor( QCursor( Qt::SizeFDiagCursor ) );
				else if( ReDragInfo::EResize_RightTop == resizeDir || ReDragInfo::EResize_LeftBottom == resizeDir )
					setCursor( QCursor( Qt::SizeBDiagCursor ) );
				else if( ReDragInfo::EResize_Left == resizeDir || ReDragInfo::EResize_Right == resizeDir )
					setCursor( QCursor( Qt::SizeHorCursor ) );
				else if( ReDragInfo::EResize_Top == resizeDir || ReDragInfo::EResize_Bottom == resizeDir )
					setCursor( QCursor( Qt::SizeVerCursor ) );
				else
					setCursor( QCursor( Qt::ArrowCursor ) );
			}
			else
			{
				setCursor( QCursor( Qt::ArrowCursor ) );
			}
		}
	}
}


// -----------------------------------------------------------------------------
// Override ReModelBase.
// -----------------------------------------------------------------------------
void ReClipModelWidget::RecycleData( ReClipWidget* _clip )
{
	if( IsReadyForEdit() )
	{
		if( NULL != _clip )
		{
			// Recycle the clip data associated with this clip widget.
			m_model->RecycleData( _clip->GetModelData() );

			// Recycle the clip widget.
			_clip->resize( 0, 0 );
			_clip->setParent( NULL );
			m_clipList.Erase( _clip );
			TSuperB::RecycleData( _clip );
		}
	}
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReClipModelWidget::OnZoom( qreal _zoomFactor )
{
	if( IsReadyForEdit() )
	{
		resize( pixmap()->size() * _zoomFactor );

		TClipPool::TItemListItor itor = m_clipList.Begin();
		TClipPool::TItemListItor itorEnd = m_clipList.End();
		for( ; itor != itorEnd; ++itor )
		{
			ReClipWidget* clipWidget = *itor;
			ReClipData* clipData = clipWidget->GetModelData();

			QPointF offset = clipData->GetOffset();
			QSizeF size = clipData->GetSize();

			QRect rect = geometry();
			int x = ( int )( 0.5f + ( qreal )rect.width() * offset.x() );
			int y = ( int )( 0.5f + ( qreal )rect.height() * offset.y() );
			int w = ( int )( 0.5f + ( qreal )rect.width() * size.width() );
			int h = ( int )( 0.5f + ( qreal )rect.height() * size.height() );

			clipWidget->move( x, y );
			clipWidget->resize( w, h );
		}
	}
}


void ReClipModelWidget::OnDelete()
{
	if( NULL == m_currentClip )
	{
		ReClipWidget* clipWidget = qobject_cast< ReClipWidget* >( childAt( m_cursor ) );
		RecycleData( clipWidget );
	}
}


bool ReClipModelWidget::OnImportImage()
{
	bool result = false;
	QString path = QFileDialog::getOpenFileName( this, tr( "Open Image File" ), 
		tr( "." ), tr( "Image File ( *.png *.bmp *.tga *.jpg )" ) );

	if( !path.isNull() )
	{
		if( !DoImportImage( path ) )
		{
			QMessageBox::critical( this, tr( "Failure" ), tr( "Failed to import image" ), QMessageBox::Ok );
		}
		else
		{
			result = true;
		}
	}

	return result;
}


// -------------------------------------------------------------------------
// Specific.
// -------------------------------------------------------------------------
void ReClipModelWidget::Tick( qreal _delta )
{
	if( NULL == m_currentClip )
	{
		TClipPoolItor itor = m_clipList.Begin();
		TClipPoolItor itorEnd = m_clipList.End();
		for( ; itor != itorEnd; ++itor )
		{
			ReClipWidget* clipWidget = *itor;
			clipWidget->PullData();
			clipWidget->update();
		}
	}

	//m_debugInfo = QString( tr( "DataCount: %1, ClipCount: %2, PoolSize: %3" ) )
	//	.arg( GetDataCount() )
	//	.arg( m_clipList.Size() )
	//	.arg( m_recyclePool.Size() );
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
bool ReClipModelWidget::IsReadyForEdit() const
{
	return NULL != pixmap();
}


ReClipWidget* ReClipModelWidget::CreateClip()
{
	ReClipWidget* result = NULL;

	if( IsReadyForEdit() )
	{
		// Create widget.
		result = CreateData();
		result->setParent( this );

		// Create model data and associate it to the widget.
		ReClipData* modelData = m_model->CreateData();
		result->SetModelData( modelData );

		// Cache all the clip widgets ( children of the image clip ).
		m_clipList.Push( result );

		result->setVisible( true );
	}

	return result;
}


bool ReClipModelWidget::ValidateClip( ReClipWidget* _clip ) const
{
	bool result = false;

	if( IsReadyForEdit() )
	{
		if( NULL != _clip )
		{
			// Should be at least 8 x 8 big.
			QRect geo = _clip->geometry();
			if( geo.width() >= ReClipWidget::ms_minSize.width() && geo.height() >= ReClipWidget::ms_minSize.height() )
			{
				bool isOutside = geo.left() >= width() || geo.right() <= 0 ||
					geo.top() >= height() || geo.bottom() <= 0;
				result = !isOutside;
			}
		}
	}

	return result;
}


bool ReClipModelWidget::DoImportImage( const QString& _path )
{
	bool result = false;

	QImage image( _path );
	if( !image.isNull() )
	{
		// TODO: Prompt to save.

		// Recycle data.
		m_clipList.Clear();
		TSuperB::ClearData();
		m_model->ClearData();

		m_model->SetImageHandle( _path );
		setPixmap( QPixmap::fromImage( image ) );
		move( 0, 0 );
		adjustSize();
		update();

		result = true;
	}

	return result;
}


void ReClipModelWidget::TrimClip( ReClipWidget* _clip )
{
	QRect imgRect = geometry();
	imgRect.translate( -imgRect.topLeft() );
	QRect clipRect = _clip->geometry();

	// If the clip is totally outside of the parent, leave the processing
	// to ValidateClip.
	if( imgRect.intersects( clipRect ) )
	{
		QRect newRect = imgRect.intersected( clipRect );
		_clip->setGeometry( newRect );
		_clip->PushData();
	}
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

}
