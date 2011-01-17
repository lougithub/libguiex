// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_ROOT_H_
#define _RE_ANIM_ROOT_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include <QVariant>


namespace RE
{


class ReAnimModel;


class ReAnimNode
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimNode( ReAnimModel* _model ): m_model( _model ), m_parent( NULL ) {}
	virtual ~ReAnimNode() {}

	ReAnimModel*		GetModel() const					{ return m_model; }
	ReAnimNode*			GetParent() const					{ return m_parent; }
	void				SetParent( ReAnimNode* _p )			{ m_parent = _p; }

	QString				GetName() const						{ return m_name; }
	void				SetName( const QString& _name )		{ m_name = _name; }

	virtual QVariant	GetNameVariant() const				{ return m_name; }
	virtual QVariant	GetDataVariant() const				{ return QVariant(); }

	virtual ReAnimNode*	CreateChild( const QVariant& _arg )	{ return NULL; }
	virtual void		DestroyChild( ReAnimNode* _child )	{}
	virtual int			GetChildrenCount() const			{ return 0; }
	virtual ReAnimNode*	GetChild( int _index )				{ return NULL; }
	virtual int			IndexOfChild( ReAnimNode* )	const	{ return -1; }

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimModel*		m_model;
	ReAnimNode*			m_parent;
	QString				m_name;
};


class ReAnimValue : public ReAnimNode
{
public:
	typedef ReAnimNode	TSuper;

	ReAnimValue( ReAnimModel* _model = NULL, qreal _value = 0.0f ): TSuper( _model ), m_value( _value ) {}

	virtual QVariant	GetDataVariant() const				{ return m_value; }
	void				SetValue( qreal _value )			{ m_value = _value; }

protected:
	qreal				m_value;
};


}
#endif	// _RE_ANIM_ROOT_H_
