// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_GLWIDGET_H_
#define _RE_EDITOR_GLWIDGET_H_

#include <QGLWidget>
#include <qnamespace.h>

class QGLFormat;


namespace RE
{


class ReGlWidget : public QGLWidget
{
	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
	Q_OBJECT

	typedef QGLWidget		TSuper;

public:
	ReGlWidget( QWidget* _parent = NULL );
	ReGlWidget( const QGLFormat& _fmt, QWidget* _parent = NULL, const QGLWidget* _shareWidget = NULL, Qt::WindowFlags _flag = 0 );


	void					Tick( float _delta );

	// -----------------------------------------------------
	// Override 
	// -----------------------------------------------------
protected:
	void					initializeGL();
	void					paintGL();
	void					resizeGL( int _width, int _height );

	// -----------------------------------------------------
	// Utility
	// -----------------------------------------------------
protected:
	void					RefreshBackdrop();

	// -----------------------------------------------------
	// Variable
	// -----------------------------------------------------
protected:
	int						m_width;
	int						m_height;

	GLuint					m_backdropId;

	static GLuint			ms_textId;
};


}		// namespace RE
#endif	// _RE_EDITOR_GLWIDGET_H_
