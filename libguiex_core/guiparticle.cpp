/** 
* @file guiparticle.cpp
* @brief 
* @author ken
* @date 2011-01-18
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticle.h"
#include "guiparticlesystem.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIParticle::CGUIParticle()
		:mParentSystem(0), mOwnDimensions(false), rotation(0), 
		position(CGUIVector3::ZERO), direction(CGUIVector3::ZERO), 
		colour(CGUIColor::White), timeToLive(10), totalTimeToLive(10), 
		rotationSpeed(0), particleType(Visual)
	{
	}
	//-----------------------------------------------------------------------
	CGUIParticle::~CGUIParticle()
	{
	}
	//-----------------------------------------------------------------------
    void CGUIParticle::setRotation(const CGUIRadian& rot)
    {
        rotation = rot;
        if (rotation != CGUIRadian(0))
		{
            mParentSystem->_notifyParticleRotated();
		}
    }
	//-----------------------------------------------------------------------
	const CGUIRadian& CGUIParticle::getRotation(void) const 
	{
		return rotation; 
	}
	//-----------------------------------------------------------------------
	real CGUIParticle::getOwnWidth(void) const
	{
		return mWidth; 
	}
	//-----------------------------------------------------------------------
	/** Retrieves the particle's personal width, if hasOwnDimensions is true. */
	real CGUIParticle::getOwnHeight(void) const
	{
		return mHeight; 
	}
	//-----------------------------------------------------------------------
	/** Sets the width and height for this particle.
	@remarks
	Note that it is most efficient for every particle in a CGUIParticleSystem to have the same dimensions. If you
	choose to alter the dimensions of an individual particle the set will be less efficient. Do not call
	this method unless you really need to have different particle dimensions within the same set. Otherwise
	just call the CGUIParticleSystem::setDefaultDimensions method instead.
	*/
	void CGUIParticle::setDimensions(real width, real height)
	{
		mOwnDimensions = true;
		mWidth = width;
		mHeight = height;
		mParentSystem->_notifyParticleResized();
	}
	//-----------------------------------------------------------------------
	/** Returns true if this particle deviates from the CGUIParticleSystem's default dimensions (i.e. if the
	particle::setDimensions method has been called for this instance).
	@see
	particle::setDimensions
	*/
	bool CGUIParticle::hasOwnDimensions(void) const
	{
		return mOwnDimensions; 
	}
	//-----------------------------------------------------------------------
	/** Internal method for notifying the particle of it's owner.
	*/
	void CGUIParticle::_notifyOwner(CGUIParticleSystem* owner)
    {
        mParentSystem = owner;
    }
    //-----------------------------------------------------------------------
	/// Utility method to reset this particle
	void CGUIParticle::resetDimensions(void)
    {
        mOwnDimensions = false;
    }
	//-----------------------------------------------------------------------
}
