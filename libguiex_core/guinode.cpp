/** 
* @file guinode.cpp
* @brief A node contains information about the 
* transformation which will apply to it and all of it's children
* @author ken
* @date 2007-11-23
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guinode.h>
#include <libguiex_core/guimath.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistring.h>


namespace guiex 
{
	//-----------------------------------------------------------------------
	CGUINode::CGUINode( )
		:m_pParent(NULL)
		,m_pChild(NULL)
		,m_pNextSibling(NULL)
	{
		m_pParent = 0;
		m_aOrientation = m_aDerivedOrientation = CGUIQuaternion::IDENTITY;
		m_aPosition = m_aDerivedPosition = CGUIVector3::ZERO;
		m_aScale = m_aDerivedScale = CGUIVector3::UNIT_SCALE;
		m_bInheritScale = true;
	}

	//-----------------------------------------------------------------------
	CGUINode::~CGUINode()
	{
	}    
	//-----------------------------------------------------------------------
	CGUINode* CGUINode::GetParent(void) const
	{
		return m_pParent;
	}

	//-----------------------------------------------------------------------
	void CGUINode::SetParent(CGUINode* parent)
	{
		m_pParent = parent;
	}

	//-----------------------------------------------------------------------
	const CGUIMatrix4& CGUINode::getTransform(void)
	{
		return mCachedTransform;
	}
	//-----------------------------------------------------------------------
	const CGUIMatrix4& CGUINode::getFullTransform(void)
	{
		return mCachedFullTransform;
	}
	//-----------------------------------------------------------------------
	const CGUIMatrix4& CGUINode::getFullInverseTransform(void)
	{
		return mCachedFullInverseTransform;
	}
	//-----------------------------------------------------------------------
	void CGUINode::updateFromParent(void)
	{
		if (m_pParent)
		{
			// Combine orientation with that of parent
			CGUIQuaternion parentOrientation = m_pParent->getDerivedOrientation();
			m_aDerivedOrientation = parentOrientation * m_aOrientation;

			// Update scale
			CGUIVector3 parentScale = m_pParent->getDerivedScale();
			if (m_bInheritScale)
			{
				// Scale own position by parent scale
				m_aDerivedScale = m_aScale * parentScale;
			}
			else
			{
				// No inheritence
				m_aDerivedScale = m_aScale;
			}

			// Change position vector based on parent's orientation & scale
			m_aDerivedPosition = parentOrientation * (m_aPosition * parentScale);

			// Add altered position vector to parents
			m_aDerivedPosition += m_pParent->getDerivedPosition();
		}
		else
		{
			// Root node, no parent
			m_aDerivedOrientation = m_aOrientation;
			m_aDerivedPosition = m_aPosition;
			m_aDerivedScale = m_aScale;
		}

		
		makeTransform( getPosition(), getScale(), getOrientation(), mCachedTransform);
		makeTransform( getDerivedPosition(), getDerivedScale(), getDerivedOrientation(), mCachedFullTransform);
		mCachedFullInverseTransform = mCachedFullTransform.inverse();
	}
	//-----------------------------------------------------------------------
	const CGUIQuaternion& CGUINode::getOrientation() const
	{
		return m_aOrientation;
	}

	//-----------------------------------------------------------------------
	void CGUINode::setOrientation( const CGUIQuaternion & q )
	{
		m_aOrientation = q;
	}
	//-----------------------------------------------------------------------
	void CGUINode::setOrientation( real w, real x, real y, real z)
	{
		m_aOrientation.w = w;
		m_aOrientation.x = x;
		m_aOrientation.y = y;
		m_aOrientation.z = z;
	}
	//-----------------------------------------------------------------------
	void CGUINode::resetOrientation(void)
	{
		m_aOrientation = CGUIQuaternion::IDENTITY;
	}

	//-----------------------------------------------------------------------
	void CGUINode::setPosition(const CGUIVector3& pos)
	{
		m_aPosition = pos;
	}


	//-----------------------------------------------------------------------
	void CGUINode::setPosition(real x, real y, real z)
	{
		CGUIVector3 v(x,y,z);
		setPosition(v);
	}

	//-----------------------------------------------------------------------
	const CGUIVector3 & CGUINode::getPosition(void) const
	{
		return m_aPosition;
	}
	//-----------------------------------------------------------------------
	CGUIMatrix3 CGUINode::getLocalAxes(void) const
	{
		CGUIVector3 axisX = CGUIVector3::UNIT_X;
		CGUIVector3 axisY = CGUIVector3::UNIT_Y;
		CGUIVector3 axisZ = CGUIVector3::UNIT_Z;

		axisX = m_aOrientation * axisX;
		axisY = m_aOrientation * axisY;
		axisZ = m_aOrientation * axisZ;

		return CGUIMatrix3(axisX.x, axisY.x, axisZ.x,
			axisX.y, axisY.y, axisZ.y,
			axisX.z, axisY.z, axisZ.z);
	}

	//-----------------------------------------------------------------------
	void CGUINode::translate(const CGUIVector3& d, ETransformSpace relativeTo)
	{
		CGUIVector3 adjusted;
		switch(relativeTo) 
		{
		case eTS_LOCAL:
			// position is relative to local so transform downwards
			m_aPosition += m_aOrientation * d;
			break;
		case eTS_WORLD:
			// position is relative to parent so transform upwards
			if (m_pParent)
			{
				m_aPosition += m_pParent->getDerivedOrientation().Inverse() * d; 
			}
			else
			{
				m_aPosition += d;
			}
			break;
		case eTS_PARENT:
			m_aPosition += d;
			break;
		}
	}
	//-----------------------------------------------------------------------
	void CGUINode::translate(real x, real y, real z, ETransformSpace relativeTo)
	{
		CGUIVector3 v(x,y,z);
		translate(v, relativeTo);
	}
	//-----------------------------------------------------------------------
	void CGUINode::translate(const CGUIMatrix3& axes, const CGUIVector3& move, ETransformSpace relativeTo)
	{
		CGUIVector3 derived = axes * move;
		translate(derived, relativeTo);
	}
	//-----------------------------------------------------------------------
	void CGUINode::translate(const CGUIMatrix3& axes, real x, real y, real z, ETransformSpace relativeTo)
	{
		CGUIVector3 d(x,y,z);
		translate(axes,d,relativeTo);
	}
	//-----------------------------------------------------------------------
	void CGUINode::roll(const CGUIRadian& angle, ETransformSpace relativeTo)
	{
		rotate(CGUIVector3::UNIT_Z, angle, relativeTo);
	}
	//-----------------------------------------------------------------------
	void CGUINode::pitch(const CGUIRadian& angle, ETransformSpace relativeTo)
	{
		rotate(CGUIVector3::UNIT_X, angle, relativeTo);
	}
	//-----------------------------------------------------------------------
	void CGUINode::yaw(const CGUIRadian& angle, ETransformSpace relativeTo)
	{
		rotate(CGUIVector3::UNIT_Y, angle, relativeTo);

	}
	//-----------------------------------------------------------------------
	void CGUINode::rotate(const CGUIVector3& axis, const CGUIRadian& angle, ETransformSpace relativeTo)
	{
		CGUIQuaternion q;
		q.FromAngleAxis(angle,axis);
		rotate(q, relativeTo);
	}

	//-----------------------------------------------------------------------
	void CGUINode::rotate(const CGUIQuaternion& q, ETransformSpace relativeTo)
	{
		switch(relativeTo) 
		{
		case eTS_PARENT:
			// Rotations are normally relative to local axes, transform up
			m_aOrientation = q * m_aOrientation;
			break;
		case eTS_WORLD:
			// Rotations are normally relative to world axes, transform up
			m_aOrientation = m_aOrientation * getDerivedOrientation().Inverse() * q * getDerivedOrientation();
			break;
		case eTS_LOCAL:
			// Note the order of the mult, i.e. q comes after
			m_aOrientation = m_aOrientation * q;
			break;
		}
	}
	//-----------------------------------------------------------------------
	const CGUIQuaternion & CGUINode::getDerivedOrientation(void)
	{
		return m_aDerivedOrientation;
	}
	//-----------------------------------------------------------------------
	const CGUIVector3 & CGUINode::getDerivedPosition(void)
	{
		return m_aDerivedPosition;
	}
	//-----------------------------------------------------------------------
	const CGUIVector3 & CGUINode::getDerivedScale(void)
	{
		return m_aDerivedScale;
	}
	//-----------------------------------------------------------------------
	void CGUINode::setScale(const CGUIVector3& scale)
	{
		m_aScale = scale;
	}
	//-----------------------------------------------------------------------
	void CGUINode::setScale(real x, real y, real z)
	{
		m_aScale.x = x;
		m_aScale.y = y;
		m_aScale.z = z;
	}
	//-----------------------------------------------------------------------
	const CGUIVector3 & CGUINode::getScale(void) const
	{
		return m_aScale;
	}
	//-----------------------------------------------------------------------
	void CGUINode::setInheritScale(bool inherit)
	{
		m_bInheritScale = inherit;
	}
	//-----------------------------------------------------------------------
	bool CGUINode::getInheritScale(void) const
	{
		return m_bInheritScale;
	}
	//-----------------------------------------------------------------------
	void CGUINode::scale(const CGUIVector3& scale)
	{
		m_aScale = m_aScale * scale;
	}
	//-----------------------------------------------------------------------
	void CGUINode::scale(real x, real y, real z)
	{
		m_aScale.x *= x;
		m_aScale.y *= y;
		m_aScale.z *= z;
	}
	//-----------------------------------------------------------------------
	void CGUINode::makeTransform(const CGUIVector3& position, const CGUIVector3& scale, 
		const CGUIQuaternion& orientation, CGUIMatrix4& destMatrix) const
	{
		destMatrix = CGUIMatrix4::IDENTITY;
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		// Parent scaling is already applied to derived position
		// Own scale is applied before rotation
		CGUIMatrix3 rot3x3, scale3x3;
		orientation.ToRotationMatrix(rot3x3);
		scale3x3 = CGUIMatrix3::ZERO;
		scale3x3[0][0] = scale.x;
		scale3x3[1][1] = scale.y;
		scale3x3[2][2] = scale.z;

		destMatrix = rot3x3 * scale3x3;
		destMatrix.setTrans(position);
	}
	//-----------------------------------------------------------------------
	void CGUINode::getWorldTransforms(CGUIMatrix4* xform)
	{
		// Assumes up to date
		*xform = getFullTransform();
	}
	//-----------------------------------------------------------------------
	const CGUIQuaternion& CGUINode::getWorldOrientation(void)
	{
		return getDerivedOrientation();
	}
	//-----------------------------------------------------------------------
	const CGUIVector3& CGUINode::getWorldPosition(void)
	{
		return getDerivedPosition();
	}
	//------------------------------------------------------------------------------
	void	CGUINode::setChild( const CGUINode* pNode)
	{
		m_pChild = (CGUINode*)(pNode);
	}
	//-----------------------------------------------------------------------	
	void	CGUINode::SetNextSibling( const CGUINode* pNode )
	{
		m_pNextSibling = (CGUINode*)(pNode);
	}
	//------------------------------------------------------------------------------
}

