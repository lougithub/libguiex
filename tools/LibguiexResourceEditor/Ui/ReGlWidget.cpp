// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include <QColor>
#include "UI\ReGlWidget.h"


namespace
{
	GLuint gObject = 0;

	GLuint gMakeObject( GLuint _oldId = -1, GLsizei _range = 1 )
	{
		if( _oldId > 0 )
		{
			glDeleteLists( _oldId, _range );
		}

		GLuint newId = glGenLists( _range );
		glNewList( newId, GL_COMPILE );
		glBegin( GL_TRIANGLES );

		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex2d( 0, 0 );
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex2d( 30, 8 );
		glColor3f( 0.0f, 0.0f, 1.0f );
		glVertex2d( 8, 30 );

		glEnd();
		glEndList();
		return newId;
	}
}


namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
GLuint ReGlWidget::ms_textId = -1;


ReGlWidget::ReGlWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_width( 0 )
, m_height( 0 )
, m_backdropId( 0 )
{
}


ReGlWidget::ReGlWidget( const QGLFormat& _fmt, 
					   QWidget* _parent /* = NULL */, 
					   const QGLWidget* _shareWidget /* = NULL */, 
					   Qt::WindowFlags _flag /* = 0 */ )
: TSuper( _fmt, _parent, _shareWidget, _flag )
{

}


void ReGlWidget::Tick( float _delta )
{
}


// -----------------------------------------------------
// Override QGLWidget
// -----------------------------------------------------
void ReGlWidget::initializeGL()
{
	qglClearColor( QColor( 100, 100, 100 ) );
	glShadeModel( GL_SMOOTH );
	glDisable( GL_DEPTH_TEST );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
}


void ReGlWidget::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT );

	if( m_backdropId > 0 )
		glCallList( m_backdropId );
}


void ReGlWidget::resizeGL( int _width, int _height )
{
	m_width = _width;
	m_height = _height;

	RefreshBackdrop();

	glViewport( 0, 0, m_width, m_height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, m_width, 0, m_height, -0.1f, 100.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();	
}


// -----------------------------------------------------
// Utility
// -----------------------------------------------------
void ReGlWidget::RefreshBackdrop()
{
	if( m_width > 0 && m_height > 0 )
	{
		if( m_backdropId > 0 )
			glDeleteLists( m_backdropId, 1 );

		m_backdropId = glGenLists( 1 );
		glNewList( m_backdropId, GL_COMPILE );
		glBegin( GL_QUADS );

		float c0 = 0.1f;
		float c1 = 0.6f;
		float c2 = 1.0f;

		glColor3f( c0, c0, c0 );
		glVertex2d( 0, 0 );
		glColor3f( c1, c1, c1 );
		glVertex2d( m_width, 0 );
		glColor3f( c2, c2, c2 );
		glVertex2d( m_width, m_height );
		glColor3f( c1, c1, c1 );
		glVertex2d( 0, m_height );

		glEnd();
		glEndList();
	}
}


}	// namespace RE
