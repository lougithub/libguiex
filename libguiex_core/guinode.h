/** 
* @file guinode.h
* @brief A node contains information about the 
* transformation which will apply to it and all of it's children
* @author ken
* @date 2007-11-23
*/

#ifndef __GUI_NODE_20071123_H__
#define __GUI_NODE_20071123_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guimatrix3.h"
#include "guimatrix4.h"
#include "guiquaternion.h"
#include "guistring.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** 
	Class representing a general-purpose node an articulated scene graph.
	@remarks A node in the scene graph is a node in a structured tree. A node contains
	information about the transformation which will apply to
	it and all of it's children. Child nodes can have transforms of their own, which
	are combined with their parent's transformations.
	*/
	class GUIEXPORT CGUINode
	{
	public:
		/** Enumeration denoting the spaces which a transform can be relative to.
		*/
		enum ETransformSpace
		{
			eTS_LOCAL, //!< Transform is relative to the local space
			eTS_PARENT, //!< Transform is relative to the space of the parent node
			eTS_WORLD, //!< Transform is relative to world space
		};

	protected:
		CGUINode( );
		virtual ~CGUINode();  

		virtual CGUINode* NodeGetParent() = 0;

		const CGUIQuaternion& NodeGetOrientation() const;
		void NodeSetOrientation( const CGUIQuaternion& q );
		void NodeSetOrientation( real w, real x, real y, real z);
		void NodeResetOrientation(void);

		void NodeSetPosition(const CGUIVector3& pos);
		void NodeSetPosition(real x, real y, real z);
		const CGUIVector3& NodeGetPosition(void) const;

		void NodeSetScale(const CGUIVector3& scale);
		void NodeSetScale(real x, real y, real z);
		const CGUIVector3 & NodeGetScale(void) const;

		void NodeSetInheritScale(bool inherit);

		void NodeScale(const CGUIVector3& scale);
		void NodeScale(real x, real y, real z);

		void NodeTranslate(const CGUIVector3& d, ETransformSpace relativeTo = eTS_PARENT);
		void NodeTranslate(real x, real y, real z, ETransformSpace relativeTo = eTS_PARENT);
		void NodeTranslate(const CGUIMatrix3& axes, const CGUIVector3& move, ETransformSpace relativeTo = eTS_PARENT);
		void NodeTranslate(const CGUIMatrix3& axes, real x, real y, real z, ETransformSpace relativeTo = eTS_PARENT);

		void NodeRoll(real angle, ETransformSpace relativeTo = eTS_LOCAL);
		void NodePitch(real angle, ETransformSpace relativeTo = eTS_LOCAL);
		void NodeYaw(real angle, ETransformSpace relativeTo = eTS_LOCAL);

		void NodeRotate(const CGUIVector3& axis, real angle, ETransformSpace relativeTo = eTS_LOCAL);
		void NodeRotate(const CGUIQuaternion& q, ETransformSpace relativeTo = eTS_LOCAL);

		CGUIMatrix3 NodeGetLocalAxes(void) const;

		const CGUIQuaternion& NodeGetDerivedOrientation(void);
		const CGUIVector3& NodeGetDerivedPosition(void);
		const CGUIVector3 & NodeGetDerivedScale(void);

		const CGUIMatrix4& NodeGetFullTransform(void);
		const CGUIMatrix4& NodeGetFullInverseTransform(void);
		//const CGUIMatrix4& NodeGetTransform(void);

		void NodeGetWorldTransforms(CGUIMatrix4* xform);
		const CGUIQuaternion& NodeGetWorldOrientation(void);

		const CGUIVector3& NodeGetWorldPosition(void);

	protected:
		void NodeUpdateFromParent(void);

		void NodeMakeTransform(
			const CGUIVector3& position, 
			const CGUIVector3& scale, 
			const CGUIQuaternion& orientation, 
			CGUIMatrix4& destMatrix ) const;

		void NodeMakeInverseTransform(
			const CGUIVector3& position, 
			const CGUIVector3& scale, 
			const CGUIQuaternion& orientation, 
			CGUIMatrix4& destMatrix );

	protected:
		CGUIQuaternion m_aOrientation; //!< Stores the orientation of the node relative to it's parent.
		CGUIVector3	m_aPosition; //!< Stores the position/translation of the node relative to its parent.
		CGUIVector3	m_aScale; //!< Stores the scaling factor applied to this node

		CGUIQuaternion m_aDerivedOrientation; //Cached combined orientation.
		CGUIVector3 m_aDerivedPosition; //Cached combined position.
		CGUIVector3 m_aDerivedScale; //Cached combined scale.
		
		//CGUIMatrix4 mCachedTransform; //!< Cached transform as a 4x4 matrix
		CGUIMatrix4 mCachedFullTransform; //!< Cached derived transform as a 4x4 matrix
		CGUIMatrix4 mCachedFullInverseTransform; //!< Cached inverse derived transform as a 4x4 matrix
	};

} //namespace

#endif
