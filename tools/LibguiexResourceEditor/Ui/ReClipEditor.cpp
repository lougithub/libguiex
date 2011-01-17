// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101111.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "ReClipEditor.h"
#include "UI\ReClipWorkshop.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>


namespace RE
{


// ----------------------------------------------------------------------------
// General.
// ----------------------------------------------------------------------------
ReClipEditor::ReClipEditor( ReClipModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_isDebugEnabled( false )
{
	setFocusPolicy( Qt::ClickFocus );

	m_workshop = new ReClipWorkshop( _model, this );
}


ReClipEditor::~ReClipEditor()
{
}


// ----------------------------------------------------------------------------
// Overrides QWidget.
// ----------------------------------------------------------------------------
void ReClipEditor::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );

	m_workshop->move( 0, 0 );
	m_workshop->resize( _event->size() );
}


void ReClipEditor::OnToggleDebug()
{
	m_isDebugEnabled = !m_isDebugEnabled;
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------


}
