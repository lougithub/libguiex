/** 
* @file guiparticle2dsystem.cpp
* @brief 
* @author ken
* @date 2011-01-12
*/


//============================================================================//
// include
//============================================================================// 
#include "guiparticle2dsystem.h"
#include "guiexception.h"
#include "guitexturemanager.h"
#include "guimath.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const SParticle2DVector2& SParticle2DVector2::operator=( const CGUIVector2& rOther )
	{
		x = rOther.x;
		y = rOther.y;
		return *this;
	}
	//------------------------------------------------------------------------------
	const SParticle2DVector2& SParticle2DVector2::operator*( real fScale )
	{
		x *= fScale;
		y *= fScale;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 operator+ ( const CGUIVector2& rVec1, const SParticle2DVector2& rVec2 )
	{
		return CGUIVector2(
			rVec1.x + rVec2.x,
			rVec1.y + rVec2.y
			);
	}
	//------------------------------------------------------------------------------
	SParticle2DVector2 operator+ ( const SParticle2DVector2& rVec1, const CGUIVector2& rVec2 )
	{
		SParticle2DVector2 aVec;
		aVec.x = rVec1.x + rVec2.x;
		aVec.y = rVec1.y + rVec2.y;
		return aVec;
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	CGUIParticle2DSystem::CGUIParticle2DSystem( int numberOfParticles )
		:emissionRate( 0.0f )
		,emitCounter( 0.0f )
		,totalParticles( numberOfParticles )
		,particleCount( 0 )
		,particleIdx( 0 )
		,autoRemoveOnFinish( false )
		,texture( NULL )
		,particles( NULL)
		,endSpinVar( 0.0f )
		,endSpin( 0.0f )
		,startSpinVar( 0.0f )
		,startSpin( 0.0f )
		,lifeVar( 0.0f )
		,life( 0.0f )
		,endSizeVar( 0.0f )
		,endSize( 0.0f )
		,startSizeVar( 0.0f )
		,startSize( 0.0f )
		,angle( 0.0f )
		,angleVar( 0.0f )
		,duration( -1.0f )
		,elapsed( 0.0f )
	{
		particles = (CGUIParticle2D*)calloc( totalParticles, sizeof(CGUIParticle2D) );

		if( !particles ) 
		{
			throw CGUIException( "[CGUIParticle2DSystem::CGUIParticle2DSystem]: not enough memory" );
			return;
		}


		// default, active
		active = true;

		// default blend function
		blendFunc.src = eBlendFuncType_ONE;
		blendFunc.dst = eBlendFuncType_SRC_ALPHA ;

		// default movement type;
		positionType = kCCPositionTypeFree;

		// by default be in mode A:
		emitterMode = eParticle2DSystemMode_Gravity;

		autoRemoveOnFinish = false;

		texture = NULL;
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystem::~CGUIParticle2DSystem()
	{
		free( particles );
		particles = NULL;

		if( texture )
		{
			CGUITextureManager::Instance()->DestroyTexture(texture);
			texture = NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::InitParticle( CGUIParticle2D* particle )
	{
		// timeToLive
		// no negative life. prevent division by 0
		particle->timeToLive = GUIMax(0.0f, life + lifeVar * CGUIMath::RangeRandom(-1.0f,1.0f) );

		// position
		particle->pos.x = sourcePosition.x + posVar.x * CGUIMath::RangeRandom(-1.0f,1.0f);
		particle->pos.y = sourcePosition.y + posVar.y * CGUIMath::RangeRandom(-1.0f,1.0f);

		// Color
		CGUIColor start( 
			GUIMin(1.0f, GUIMax(0.0f, startColor.GetRed() + startColorVar.GetRed() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ),
			GUIMin(1.0f, GUIMax(0.0f, startColor.GetGreen() + startColorVar.GetGreen() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ),
			GUIMin(1.0f, GUIMax(0.0f, startColor.GetBlue() + startColorVar.GetBlue() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ),
			GUIMin(1.0f, GUIMax(0.0f, startColor.GetAlpha() + startColorVar.GetAlpha() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ) );

		CGUIColor end(
			GUIMin(1.0f, GUIMax(0.0f, endColor.GetRed() + endColorVar.GetRed() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ),
			GUIMin(1.0f, GUIMax(0.0f, endColor.GetGreen() + endColorVar.GetGreen() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ),
			GUIMin(1.0f, GUIMax(0.0f, endColor.GetBlue() + endColorVar.GetBlue() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ),
			GUIMin(1.0f, GUIMax(0.0f, endColor.GetAlpha() + endColorVar.GetAlpha() * CGUIMath::RangeRandom(-1.0f,1.0f) ) ) );

		particle->color = start;
		particle->deltaColor = ( end - start ) / particle->timeToLive;

		// size
		real startS = GUIMax(0.0f, startSize + startSizeVar * CGUIMath::RangeRandom(-1.0f,1.0f) ); // no negative size

		particle->size = startS;
		if( GUI_REAL_EQUAL( endSize, -1.0f ))
			particle->deltaSize = 0;
		else
		{
			real endS = endSize + endSizeVar * CGUIMath::RangeRandom(-1.0f,1.0f);
			endS = GUIMax(0.0f, endS);
			particle->deltaSize = (endS - startS) / particle->timeToLive;
		}

		// rotation
		real startA = startSpin + startSpinVar * CGUIMath::RangeRandom(-1.0f,1.0f);
		real endA = endSpin + endSpinVar * CGUIMath::RangeRandom(-1.0f,1.0f);
		particle->rotation = startA;
		particle->deltaRotation = (endA - startA) / particle->timeToLive;

		// position
		particle->startPos = position;
		//if( positionType == kCCPositionTypeFree )
		//{
		//	particle->startPos = [self convertToWorldSpace:CGPointZero];
		//}
		//else if( positionType == kCCPositionTypeRelative ) {
		//	particle->startPos = position;
		//}

		// direction
		real a = CGUIMath::DegreesToRadians( angle + angleVar * CGUIMath::RangeRandom(-1.0f,1.0f) );	

		// Mode Gravity: A
		if( emitterMode == eParticle2DSystemMode_Gravity )
		{

			CGUIVector2 v(cosf( a ), sinf( a ));
			real s = mode.A.speed + mode.A.speedVar * CGUIMath::RangeRandom(-1.0f,1.0f);

			// direction
			particle->mode.A.dir = v * s;

			// radial accel
			particle->mode.A.radialAccel = mode.A.radialAccel + mode.A.radialAccelVar * CGUIMath::RangeRandom(-1.0f,1.0f);

			// tangential accel
			particle->mode.A.tangentialAccel = mode.A.tangentialAccel + mode.A.tangentialAccelVar * CGUIMath::RangeRandom(-1.0f,1.0f);

		}

		// Mode Radius: B
		else 
		{
			// Set the default diameter of the particle from the source position
			real startRadius = mode.B.startRadius + mode.B.startRadiusVar * CGUIMath::RangeRandom(-1.0f,1.0f);
			real endRadius = mode.B.endRadius + mode.B.endRadiusVar * CGUIMath::RangeRandom(-1.0f,1.0f);


			particle->mode.B.radius = startRadius;

			if( GUI_REAL_EQUAL( mode.B.endRadius, -1.0f ))
			{
				particle->mode.B.deltaRadius = 0;
			}
			else
			{
				particle->mode.B.deltaRadius = (endRadius - startRadius) / particle->timeToLive;
			}

			particle->mode.B.angle = a;
			particle->mode.B.degreesPerSecond = CGUIMath::DegreesToRadians(mode.B.rotatePerSecond + mode.B.rotatePerSecondVar * CGUIMath::RangeRandom(-1.0f,1.0f));
		}	
	}
	//------------------------------------------------------------------------------
	bool CGUIParticle2DSystem::AddParticle()
	{
		if( IsFull() )
		{
			return false;
		}

		CGUIParticle2D * particle = &particles[ particleCount ];

		InitParticle( particle );		
		particleCount++;

		return true;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::StopSystem()
	{
		active = false;
		elapsed = duration;
		emitCounter = 0;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::ResetSystem()
	{
		active = true;
		elapsed = 0;
		for(particleIdx = 0; particleIdx < particleCount; ++particleIdx)
		{
			CGUIParticle2D *p = &particles[particleIdx];
			p->timeToLive = 0;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::Update( real rDeltaTime )
	{
		if( active && emissionRate > 0.0f ) 
		{
			real rate = 1.0f / emissionRate;
			emitCounter += rDeltaTime;
			while( particleCount < totalParticles && emitCounter > rate ) 
			{
				AddParticle();
				emitCounter -= rate;
			}

			elapsed += rDeltaTime;
			if( duration >= 0.0f && duration < elapsed)
			{
				StopSystem();
			}
		}


		//position
		CGUIVector2 currentPosition = position;
		//if( positionType == kCCPositionTypeFree ) 
		//{
		//	currentPosition = [self convertToWorldSpace:CGPointZero];
		//}
		//else if( positionType == kCCPositionTypeRelative ) 
		//{
		//	currentPosition = position;
		//}

		particleIdx = 0;
		while( particleIdx < particleCount )
		{
			CGUIParticle2D *p = &particles[particleIdx];

			// life
			p->timeToLive -= rDeltaTime;

			if( p->timeToLive > 0 ) 
			{
				// Mode A: gravity, direction, tangential accel & radial accel
				if( emitterMode == eParticle2DSystemMode_Gravity )
				{
					CGUIVector2 tmp, radial, tangential;

					radial = CGUIVector2::ZERO;
					// radial acceleration
					if(p->pos.x || p->pos.y)
					{
						radial = p->pos.NormalisedCopy();
					}

					tangential = radial;
					radial = radial * p->mode.A.radialAccel;

					// tangential acceleration
					real newy = tangential.x;
					tangential.x = -tangential.y;
					tangential.y = newy;
					tangential = tangential * p->mode.A.tangentialAccel;

					// (gravity + radial + tangential) * rDeltaTime
					tmp = (radial + tangential + mode.A.gravity) * rDeltaTime;
					p->mode.A.dir = p->mode.A.dir + tmp;
					p->pos = p->pos + p->mode.A.dir * rDeltaTime;
				}

				// Mode B: radius movement
				else 
				{				
					// Update the angle and radius of the particle.
					p->mode.B.angle += p->mode.B.degreesPerSecond * rDeltaTime;
					p->mode.B.radius += p->mode.B.deltaRadius * rDeltaTime;

					p->pos.x = - cosf(p->mode.B.angle) * p->mode.B.radius;
					p->pos.y = - sinf(p->mode.B.angle) * p->mode.B.radius;
				}

				// color
				p->color.SetRed( p->color.GetRed() + p->deltaColor.GetRed() * rDeltaTime );
				p->color.SetGreen( p->color.GetGreen() + p->deltaColor.GetGreen() * rDeltaTime );
				p->color.SetBlue( p->color.GetBlue() + p->deltaColor.GetBlue() * rDeltaTime );
				p->color.SetAlpha( p->color.GetAlpha() + p->deltaColor.GetAlpha() * rDeltaTime );

				// size
				p->size += (p->deltaSize * rDeltaTime);
				p->size = GUIMax( 0.0f, p->size );

				// angle
				p->rotation += (p->deltaRotation * rDeltaTime);

				//
				// update values in quad
				//

				CGUIVector2	newPos;

				if( positionType == kCCPositionTypeFree || positionType == kCCPositionTypeRelative )
				{
					CGUIVector2 diff = currentPosition - p->startPos;
					newPos = p->pos - diff;
				}
				else
				{
					newPos = p->pos;
				}


				UpdateQuadWithParticle( p, newPos );

				// update particle counter
				particleIdx++;
			}
			else
			{
				// life < 0
				if( particleIdx != particleCount-1 )
				{
					particles[particleIdx] = particles[particleCount-1];
				}
				particleCount--;

				if( particleCount == 0 && autoRemoveOnFinish )
				{
					return;
				}
			}
		}

		PostStep();
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::PostStep()
	{
		// should be override
	}
	void CGUIParticle2DSystem::Render( class IGUIInterfaceRender* /*pRender*/, const class CGUIMatrix4& /*rWorldMatrix*/ )
	{
		// should be override
	}
	void CGUIParticle2DSystem::UpdateQuadWithParticle( CGUIParticle2D* /*particle*/, const CGUIVector2& /*rNewPos*/ )
	{
		// should be override
	}
	//------------------------------------------------------------------------------
	bool CGUIParticle2DSystem::IsFull()
	{
		return (particleCount == totalParticles);
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetAngle( real fAngle )
	{
		angle = fAngle;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetAngle(  ) const
	{
		return angle;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetAngleVar( real fAngle )
	{
		angleVar = fAngle;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetAngleVar(  ) const
	{
		return angleVar;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetDuration( real fDuration )
	{
		duration = fDuration;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetDuration(  ) const
	{
		return duration;
	}
	//------------------------------------------------------------------------------

//	-(id) initWithDictionary:(NSDictionary *)dictionary
//	{
//		NSUInteger maxParticles = [[dictionary valueForKey:"maxParticles"] intValue];
//		// self, not super
//		if ((self=[self initWithTotalParticles:maxParticles] ) ) {
//
//			// angle
//			angle = [[dictionary valueForKey:"angle"] floatValue];
//			angleVar = [[dictionary valueForKey:"angleVariance"] floatValue];
//
//			// duration
//			duration = [[dictionary valueForKey:"duration"] floatValue];
//
//			// blend function 
//			blendFunc.src = [[dictionary valueForKey:"blendFuncSource"] intValue];
//			blendFunc.dst = [[dictionary valueForKey:"blendFuncDestination"] intValue];
//
//			// color
//			real r,g,b,a;
//
//			r = [[dictionary valueForKey:"startColorRed"] floatValue];
//			g = [[dictionary valueForKey:"startColorGreen"] floatValue];
//			b = [[dictionary valueForKey:"startColorBlue"] floatValue];
//			a = [[dictionary valueForKey:"startColorAlpha"] floatValue];
//			startColor = (ccColor4F) {r,g,b,a};
//
//			r = [[dictionary valueForKey:"startColorVarianceRed"] floatValue];
//			g = [[dictionary valueForKey:"startColorVarianceGreen"] floatValue];
//			b = [[dictionary valueForKey:"startColorVarianceBlue"] floatValue];
//			a = [[dictionary valueForKey:"startColorVarianceAlpha"] floatValue];
//			startColorVar = (ccColor4F) {r,g,b,a};
//
//			r = [[dictionary valueForKey:"finishColorRed"] floatValue];
//			g = [[dictionary valueForKey:"finishColorGreen"] floatValue];
//			b = [[dictionary valueForKey:"finishColorBlue"] floatValue];
//			a = [[dictionary valueForKey:"finishColorAlpha"] floatValue];
//			endColor = (ccColor4F) {r,g,b,a};
//
//			r = [[dictionary valueForKey:"finishColorVarianceRed"] floatValue];
//			g = [[dictionary valueForKey:"finishColorVarianceGreen"] floatValue];
//			b = [[dictionary valueForKey:"finishColorVarianceBlue"] floatValue];
//			a = [[dictionary valueForKey:"finishColorVarianceAlpha"] floatValue];
//			endColorVar = (ccColor4F) {r,g,b,a};
//
//			// particle size
//			startSize = [[dictionary valueForKey:"startParticleSize"] floatValue];
//			startSizeVar = [[dictionary valueForKey:"startParticleSizeVariance"] floatValue];
//			endSize = [[dictionary valueForKey:"finishParticleSize"] floatValue];
//			endSizeVar = [[dictionary valueForKey:"finishParticleSizeVariance"] floatValue];
//
//
//			// position
//			real x = [[dictionary valueForKey:"sourcePositionx"] floatValue];
//			real y = [[dictionary valueForKey:"sourcePositiony"] floatValue];
//			self.position = ccp(x,y);
//			posVar.x = [[dictionary valueForKey:"sourcePositionVariancex"] floatValue];
//			posVar.y = [[dictionary valueForKey:"sourcePositionVariancey"] floatValue];
//
//
//			emitterMode = [[dictionary valueForKey:"emitterType"] intValue];
//
//			// Mode A: Gravity + tangential accel + radial accel
//			if( emitterMode == eParticle2DSystemMode_Gravity ) {
//				// gravity
//				mode.A.gravity.x = [[dictionary valueForKey:"gravityx"] floatValue];
//				mode.A.gravity.y = [[dictionary valueForKey:"gravityy"] floatValue];
//
//				//
//				// speed
//				mode.A.speed = [[dictionary valueForKey:"speed"] floatValue];
//				mode.A.speedVar = [[dictionary valueForKey:"speedVariance"] floatValue];
//
//				// radial acceleration			
//				NSString *tmp = [dictionary valueForKey:"radialAcceleration"];
//				mode.A.radialAccel = tmp ? [tmp floatValue] : 0;
//
//				tmp = [dictionary valueForKey:"radialAccelVariance"];
//				mode.A.radialAccelVar = tmp ? [tmp floatValue] : 0;
//
//				// tangential acceleration
//				tmp = [dictionary valueForKey:"tangentialAcceleration"];
//				mode.A.tangentialAccel = tmp ? [tmp floatValue] : 0;
//
//				tmp = [dictionary valueForKey:"tangentialAccelVariance"];
//				mode.A.tangentialAccelVar = tmp ? [tmp floatValue] : 0;
//			}
//
//
//			// or Mode B: radius movement
//			else if( emitterMode == eParticle2DSystemMode_Radius ) {
//				real maxRadius = [[dictionary valueForKey:"maxRadius"] floatValue];
//				real maxRadiusVar = [[dictionary valueForKey:"maxRadiusVariance"] floatValue];
//				real minRadius = [[dictionary valueForKey:"minRadius"] floatValue];
//
//				mode.B.startRadius = maxRadius;
//				mode.B.startRadiusVar = maxRadiusVar;
//				mode.B.endRadius = minRadius;
//				mode.B.endRadiusVar = 0;
//				mode.B.rotatePerSecond = [[dictionary valueForKey:"rotatePerSecond"] floatValue];
//				mode.B.rotatePerSecondVar = [[dictionary valueForKey:"rotatePerSecondVariance"] floatValue];
//
//			} else {
//				GUI_ASSERT( NO, "Invalid emitterType in config file");
//			}
//
//			// life span
//			life = [[dictionary valueForKey:"particleLifespan"] floatValue];
//			lifeVar = [[dictionary valueForKey:"particleLifespanVariance"] floatValue];				
//
//			// emission Rate
//			emissionRate = totalParticles/life;
//
//			// texture		
//			// Try to get the texture from the cache
//			NSString *textureName = [dictionary valueForKey:"textureFileName"];
//
//			self.texture = [[CCTextureCache sharedTextureCache] addImage:textureName];
//
//			NSString *textureData = [dictionary valueForKey:"textureImageData"];
//
//			if ( ! texture && textureData) {
//
//				// if it fails, try to get it from the base64-gzipped data			
//				unsigned char *buffer = NULL;
//				NSUInteger len = base64Decode((unsigned char*)[textureData UTF8String], [textureData length], &buffer);
//				GUI_ASSERT( buffer != NULL, "CCParticleSystem: error decoding textureImageData");
//
//				unsigned char *deflated = NULL;
//				NSUInteger deflatedLen = ccInflateMemory(buffer, len, &deflated);
//				free( buffer );
//
//				GUI_ASSERT( deflated != NULL, "CCParticleSystem: error ungzipping textureImageData");
//				NSData *data = [[NSData alloc] initWithBytes:deflated length:deflatedLen];
//
//#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
//				UIImage *image = [[UIImage alloc] initWithData:data];
//#elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
//				NSBitmapImageRep *image = [[NSBitmapImageRep alloc] initWithData:data];
//#endif
//
//				free(deflated); deflated = NULL;
//
//				self.texture = [[CCTextureCache sharedTextureCache] addCGImage:[image CGImage] forKey:textureName];
//				[data release];
//				[image release];
//			}
//
//			GUI_ASSERT( [self texture] != NULL, "CCParticleSystem: error loading the texture");
//
//		}
//
//		return self;
//	}
//------------------------------------------------------------------------------
void CGUIParticle2DSystem::SetTexture( const CGUIString& rTexturePath )
{
	if( texture )
	{
		CGUITextureManager::Instance()->DestroyTexture(texture);
		texture = NULL;
	}

	//CGUIString strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;
	texture = CGUITextureManager::Instance()->CreateTexture(rTexturePath);
	if( !texture )
	{
		throw CGUIException("[CGUIParticle2DSystem::SetTexture]: failed to create texture from path <%s>", rTexturePath.c_str());
		return;
	}
}
//------------------------------------------------------------------------------
void CGUIParticle2DSystem::SetBlendAdditive( bool additive )
{
	if( additive )
	{
		blendFunc.src = eBlendFuncType_SRC_ALPHA;
		blendFunc.dst = eBlendFuncType_ONE;

	}
	else 
	{
		blendFunc.src = eBlendFuncType_ONE;
		blendFunc.dst = eBlendFuncType_SRC_ALPHA;
	}
}
//------------------------------------------------------------------------------
bool CGUIParticle2DSystem::IsBlendAdditive()
{
	return( blendFunc.src == eBlendFuncType_SRC_ALPHA && blendFunc.dst == eBlendFuncType_ONE);
}
//------------------------------------------------------------------------------
void CGUIParticle2DSystem::SetTangentialAccel( real t )
{
	GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity" );
	mode.A.tangentialAccel = t;
}
//------------------------------------------------------------------------------
real CGUIParticle2DSystem::GetTangentialAccel()
{
	GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
	return mode.A.tangentialAccel;
}
//------------------------------------------------------------------------------
void CGUIParticle2DSystem::SetTangentialAccelVar( real t )
{
	GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
	mode.A.tangentialAccelVar = t;
}
//------------------------------------------------------------------------------
real CGUIParticle2DSystem::GetTangentialAccelVar()
{
	GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
	return mode.A.tangentialAccelVar;
}
//------------------------------------------------------------------------------
void CGUIParticle2DSystem::SetRadialAccel( real t )
{
	GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
	mode.A.radialAccel = t;
}
//------------------------------------------------------------------------------
real CGUIParticle2DSystem::GetRadialAccel()
{
	GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
	return mode.A.radialAccel;
}
//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetRadialAccelVar( real t )
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		mode.A.radialAccelVar = t;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetRadialAccelVar()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		return mode.A.radialAccelVar;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetGravity( const CGUIVector2& g )
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		mode.A.gravity = g;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIParticle2DSystem::GetGravity()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		CGUIVector2 aVec;
		aVec.x = mode.A.gravity.x;
		aVec.y = mode.A.gravity.y;
		return aVec;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetSpeed(real speed)
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		mode.A.speed = speed;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetSpeed()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		return mode.A.speed;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetSpeedVar(real speedVar)
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		mode.A.speedVar = speedVar;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetSpeedVar()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Gravity, "Particle Mode should be Gravity");
		return mode.A.speedVar;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetStartRadius(real startRadius)
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		mode.B.startRadius = startRadius;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetStartRadius()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		return mode.B.startRadius;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetStartRadiusVar(real startRadiusVar)
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		mode.B.startRadiusVar = startRadiusVar;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetStartRadiusVar()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		return mode.B.startRadiusVar;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetEndRadius(real endRadius )
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		mode.B.endRadius = endRadius;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetEndRadius()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		return mode.B.endRadius;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetEndRadiusVar(real endRadiusVar)
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		mode.B.endRadiusVar = endRadiusVar;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetEndRadiusVar()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		return mode.B.endRadiusVar;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetRotatePerSecond(real degrees )
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		mode.B.rotatePerSecond = degrees;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetRotatePerSecond()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		return mode.B.rotatePerSecond;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetRotatePerSecondVar(real degrees)
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		mode.B.rotatePerSecondVar = degrees;
	}
	//------------------------------------------------------------------------------
	real CGUIParticle2DSystem::GetRotatePerSecondVar()
	{
		GUI_ASSERT( emitterMode == eParticle2DSystemMode_Radius, "Particle Mode should be Radius");
		return mode.B.rotatePerSecondVar;
	}
	//------------------------------------------------------------------------------
	const SBlendFuncType& CGUIParticle2DSystem::GetBlendFuncType() const
	{
		return blendFunc;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetEmissionRate( real rRate )
	{
		emissionRate = rRate;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetLife( real rLife )
	{
		life = rLife;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetStartColor( const CGUIColor& rColor )
	{
		startColor = rColor;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetEndColor( const CGUIColor& rColor )
	{
		endColor = rColor;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetStartSize( real fSize)
	{
		startSize = fSize;
	}
	//------------------------------------------------------------------------------
}

