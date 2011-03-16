/** 
* @file guiparticle2dsystem.h
* @brief 
* @author ken
* @date 2011-01-12
*/


#ifndef __GUI_PARTICLE2DSYSTEM_20110112_H__
#define __GUI_PARTICLE2DSYSTEM_20110112_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector2.h"
#include "guicolor.h"
#include "guirendertype.h"
#include "guiresource.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	struct SParticle2DVector2
	{
		real x, y;        
		
		const SParticle2DVector2& operator=( const CGUIVector2& rOther );
		SParticle2DVector2 operator*( real fScale );
	};
	extern CGUIVector2 operator+ ( const CGUIVector2& rVec1, const SParticle2DVector2& rVec2 );
	extern SParticle2DVector2 operator+ ( const SParticle2DVector2& rVec1, const CGUIVector2& rVec2 );


	class CGUIParticle2D
	{
	public:
		CGUIVector2 pos;
		CGUIVector2 startPos;

		CGUIColor color;
		CGUIColor deltaColor;

		real size;
		real deltaSize;

		real rotation;
		real deltaRotation;

		real timeToLive;

		union 
		{
			// Mode A: gravity, direction, radial accel, tangential accel
			struct
			{
			public:
				SParticle2DVector2 dir;
				real radialAccel;
				real tangentialAccel;
			} A;

			// Mode B: radius mode
			struct
			{
				real angle;
				real degreesPerSecond;
				real radius;
				real deltaRadius;
			} B;
		} mode;
	};


	class CGUIParticle2DSystem : public CGUIResource
	{	
	public:
		virtual ~CGUIParticle2DSystem();

		int32 LoadValueFromProperty( const class CGUIProperty& rProperty );

		bool AddParticle();

		virtual void Render( class IGUIInterfaceRender* pRender );

		void StopSystem();
		void ResetSystem();
		bool IsFull();

		void Update( real rDeltaTime );

		void SetTexture( const CGUIString& rTexturePath );

		void SetBlendAdditive( bool additive );
		bool IsBlendAdditive();

		void SetTangentialAccel( real t );
		real GetTangentialAccel( );

		void SetTangentialAccelVar( real t );
		real GetTangentialAccelVar( );

		void SetRadialAccel( real t );
		real GetRadialAccel( );

		void SetRadialAccelVar( real t );
		real GetRadialAccelVar();

		void SetGravity( const CGUIVector2& g );
		CGUIVector2 GetGravity();

		void SetSpeed(real speed);
		real GetSpeed();

		void SetSpeedVar(real speedVar);
		real GetSpeedVar();

		void SetStartRadius(real startRadius);
		real GetStartRadius();

		void SetStartRadiusVar(real startRadiusVar);
		real GetStartRadiusVar();

		void SetEndRadius(real endRadius );
		real GetEndRadius();

		void SetEndRadiusVar(real endRadiusVar);
		real GetEndRadiusVar();

		void SetRotatePerSecond(real degrees );
		real GetRotatePerSecond();

		void SetRotatePerSecondVar(real degrees);
		real GetRotatePerSecondVar();

		const SGUIBlendFunc& GetBlendFuncType() const;

		void SetEmissionRate( real rRate );
		void SetLife( real rLife );

		void SetStartColor( const CGUIColor& rColor );
		void SetEndColor( const CGUIColor& rColor );

		void SetStartSize( real fSize);

		void SetAngle( real fAngle );
		real GetAngle(  ) const;

		void SetAngleVar( real fAngle );
		real GetAngleVar(  ) const;

		void SetDuration( real fDuration );
		real GetDuration(  ) const;

		void SetTotalParticles( uint32 nTotalParticle );

	protected:
		CGUIParticle2DSystem( const CGUIString& rName, const CGUIString& rSceneName );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

		virtual void UpdateQuadWithParticle( CGUIParticle2D* particle, const CGUIVector2& rNewPos );
		virtual void PostStep();

		void InitParticle( CGUIParticle2D* particle );

	protected:
		// is the particle system active ?
		bool active;
		// duration in seconds of the system. -1 is infinity
		real duration;
		// time elapsed since the start of the system (in seconds)
		real elapsed;

		CGUIVector2 sourcePosition;
		// Position variance
		CGUIVector2 posVar;

		// The angle (direction) of the particles measured in degrees
		real angle;
		// Angle variance measured in degrees;
		real angleVar;

		// Different modes

		EParticle2DSystemMode emitterMode;
		union 
		{
			// Mode A:Gravity + Tangential Accel + Radial Accel
			struct
			{
				// gravity of the particles
				SParticle2DVector2 gravity;

				// The speed the particles will have.
				real speed;
				// The speed variance
				real speedVar;

				// Tangential acceleration
				real tangentialAccel;
				// Tangential acceleration variance
				real tangentialAccelVar;

				// Radial acceleration
				real radialAccel;
				// Radial acceleration variance
				real radialAccelVar;
			} A;


			// Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
			struct
			{
				// The starting radius of the particles
				real startRadius;
				// The starting radius variance of the particles
				real startRadiusVar;
				// The ending radius of the particles
				real endRadius;
				// The ending radius variance of the particles
				real endRadiusVar;			
				// Number of degrees to rotate a particle around the source pos per second
				real rotatePerSecond;
				// Variance in degrees for rotatePerSecond
				real rotatePerSecondVar;
			} B;
		} mode;

		// start size of the particles
		real startSize;
		// start Size variance
		real startSizeVar;
		// End size of the particle
		real endSize;
		// end size of variance
		real endSizeVar;

		// How many seconds will the particle live
		real life;
		// Life variance
		real lifeVar;

		// Start color of the particles
		CGUIColor startColor;
		// Start color variance
		CGUIColor startColorVar;
		// End color of the particles
		CGUIColor endColor;
		// End color variance
		CGUIColor endColorVar;

		// start angle of the particles
		real startSpin;
		// start angle variance
		real startSpinVar;
		// End angle of the particle
		real endSpin;
		// end angle variance
		real endSpinVar;


		// Array of particles
		mutable CGUIParticle2D *particles;
		// Maximum particles
		uint32 totalParticles;
		// Count of active particles
		uint32 particleCount;

		// How many particles can be emitted per second
		real emissionRate;
		real emitCounter;

		// Texture of the particles
		CGUIString m_strFullTexturePath;
		mutable class CGUITexture *texture;
		// blend type
		SGUIBlendFunc blendFunc;

		// Whether or not the node will be auto-removed when there are not particles
		bool autoRemoveOnFinish;

		//  particle idx
		uint32 particleIdx;
	};

}

#endif //__GUI_PARTICLE2DSYSTEM_20110112_H__
