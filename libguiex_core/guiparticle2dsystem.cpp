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
#include "guiscenemanager.h"
#include "guimath.h"
#include "guiproperty.h"
#include "guipropertyconvertor.h"
#include "guiparticle2dmanager.h"


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
	SParticle2DVector2 SParticle2DVector2::operator*( real fScale )
	{
		SParticle2DVector2 aVec;
		aVec.x = x*fScale;
		aVec.y = y*fScale;
		return aVec;
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
	CGUIParticle2DSystem::CGUIParticle2DSystem( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "PARTICLE2D", GSystem->GetParticle2DManager())
		,emissionRate( 0.0f )
		,emitCounter( 0.0f )
		,totalParticles( 0 )
		,particleCount( 0 )
		,particleIdx( 0 )
		,autoRemoveOnFinish( false )
		,texture( NULL )
		,particles( NULL)
		,endSpin( 0.0f )
		,endSpinVar( 0.0f )
		,startSpin( 0.0f )
		,startSpinVar( 0.0f )
		,life( 0.0f )
		,lifeVar( 0.0f )
		,endSize( 0.0f )
		,endSizeVar( 0.0f )
		,startSize( 0.0f )
		,startSizeVar( 0.0f )
		,angle( 0.0f )
		,angleVar( 0.0f )
		,duration( -1.0f )
		,elapsed( 0.0f )
		,active( true )
		,emitterMode( eParticle2DSystemMode_Gravity )
	{
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystem::~CGUIParticle2DSystem()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIParticle2DSystem::LoadValueFromProperty( const class CGUIProperty& rProperty )
	{
		/*
		<property name="Galaxy" type="CGUIParticle2DDefine" >
			<property name="maxParticles" type="uint32" value="934"/>
			<property name="duration" type="real" value="-1"/>
			<property name="particleLifespan" type="real" value="4"/>
			<property name="particleLifespanVariance" type="real" value="1"/>
			<property name="textureFileName" type="CGUIString" value="Galaxy.png"/>
			<property name="angle" type="real" value="90"/>
			<property name="angleVariance" type="real" value="360"/>
			<property name="startColor" type="CGUIColor" value="30,64,192,255"/>
			<property name="startColorVariance" type="CGUIColor" value="0,0,0,0"/>
			<property name="finishColor" type="CGUIColor" value="0,0,0,255"/>
			<property name="finishColorVariance" type="CGUIColor" value="0,0,0,0"/>
			<property name="startParticleSize" type="real" value="37"/>
			<property name="startParticleSizeVariance" type="real" value="10"/>
			<property name="finishParticleSize" type="real" value="37"/>
			<property name="finishParticleSizeVariance" type="real" value="0"/>
			<property name="sourcePosition" type="CGUIVector2" value="170,228"/>
			<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
			<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>
		</property>
		*/
		//maxParticles
		{
			const CGUIProperty* pPpt_maxParticles = rProperty.GetPropertyChecked("maxParticles", "uint32");
			PropertyToValue( *pPpt_maxParticles, totalParticles );
		}


		//angle
		{
			const CGUIProperty* pPpt_angle = rProperty.GetPropertyChecked("angle", "real");
			PropertyToValue( *pPpt_angle, angle );
			const CGUIProperty* pPpt_angleVar = rProperty.GetPropertyChecked("angleVariance", "real");
			PropertyToValue( *pPpt_angleVar, angleVar );
		}

		//duration
		{
			const CGUIProperty* pPpt_duration = rProperty.GetPropertyChecked("duration", "real");
			PropertyToValue( *pPpt_duration, duration );
		}

		//blendfunc
		{
			const CGUIProperty* pPpt_blendFunc = rProperty.GetPropertyChecked("blendFunc", "SGUIBlendFunc");
			PropertyToValue( *pPpt_blendFunc, blendFunc );
		}

		//color
		{
			const CGUIProperty* pPpt_startColor = rProperty.GetPropertyChecked("startColor", "CGUIColor");
			PropertyToValue( *pPpt_startColor, startColor );
			const CGUIProperty* pPpt_startColorVar = rProperty.GetPropertyChecked("startColorVariance", "CGUIColor");
			PropertyToValue( *pPpt_startColorVar, startColorVar );
			const CGUIProperty* pPpt_finishColor = rProperty.GetPropertyChecked("finishColor", "CGUIColor");
			PropertyToValue( *pPpt_finishColor, endColor );
			const CGUIProperty* pPpt_finishColorVar = rProperty.GetPropertyChecked("finishColorVariance", "CGUIColor");
			PropertyToValue( *pPpt_finishColorVar, endColorVar );
		}

		//size
		{
			const CGUIProperty* pPpt_startSize = rProperty.GetPropertyChecked("startParticleSize", "real");
			PropertyToValue( *pPpt_startSize, startSize );
			const CGUIProperty* pPpt_startSizeVar = rProperty.GetPropertyChecked("startParticleSizeVariance", "real");
			PropertyToValue( *pPpt_startSizeVar, startSizeVar );
			const CGUIProperty* pPpt_finishSize = rProperty.GetPropertyChecked("finishParticleSize", "real");
			PropertyToValue( *pPpt_finishSize, endSize );
			const CGUIProperty* pPpt_finishSizeVar = rProperty.GetPropertyChecked("finishParticleSizeVariance", "real");
			PropertyToValue( *pPpt_finishSizeVar, endSizeVar );
		}

		//spin
		{
			const CGUIProperty* pPpt_startSpin = rProperty.GetPropertyChecked("startSpin", "real");
			PropertyToValue( *pPpt_startSpin, startSpin );
			const CGUIProperty* pPpt_startSpinVar = rProperty.GetPropertyChecked("startSpinVariance", "real");
			PropertyToValue( *pPpt_startSpinVar, startSpinVar );
			const CGUIProperty* pPpt_endSpin = rProperty.GetPropertyChecked("endSpin", "real");
			PropertyToValue( *pPpt_endSpin, endSpin );
			const CGUIProperty* pPpt_endSpinVar = rProperty.GetPropertyChecked("endSpinVariance", "real");
			PropertyToValue( *pPpt_endSpinVar, endSpinVar );
		}

		//position
		{
			const CGUIProperty* pPpt_sourcePosition = rProperty.GetPropertyChecked("sourcePosition", "CGUIVector2");
			PropertyToValue( *pPpt_sourcePosition, sourcePosition );
			const CGUIProperty* pPpt_sourcePositionVar = rProperty.GetPropertyChecked("sourcePositionVariance", "CGUIVector2");
			PropertyToValue( *pPpt_sourcePositionVar, posVar );
		}

		//textureFileName
		{
			const CGUIProperty* pPpt_textureFileName = rProperty.GetPropertyChecked("textureFileName", "CGUIString");
			m_strFullTexturePath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + pPpt_textureFileName->GetValue();
		}

		//emitter mode
		{
			const CGUIProperty* pPpt_emitterType = rProperty.GetPropertyChecked("emitterType", "EParticle2DSystemMode");
			PropertyToValue( *pPpt_emitterType, emitterMode );
		}

		//life span
		{
			const CGUIProperty* pPpt_particleLifespan = rProperty.GetPropertyChecked("particleLifespan", "real");
			PropertyToValue( *pPpt_particleLifespan, life );
			const CGUIProperty* pPpt_particleLifespanVariance = rProperty.GetPropertyChecked("particleLifespanVariance", "real");
			PropertyToValue( *pPpt_particleLifespanVariance, lifeVar );
		}

		emissionRate = totalParticles/life;

		// Mode A: Gravity + tangential accel + radial accel
		if( emitterMode == eParticle2DSystemMode_Gravity )
		{
			//gravity
			{
				const CGUIProperty* pPpt_gravity = rProperty.GetPropertyChecked("gravity", "CGUIVector2");
				CGUIVector2 aValue;
				PropertyToValue( *pPpt_gravity, aValue );
				mode.A.gravity = aValue;
			}

			//speed
			{
				const CGUIProperty* pPpt_speed = rProperty.GetPropertyChecked("speed", "real");
				PropertyToValue( *pPpt_speed, mode.A.speed );
				const CGUIProperty* pPpt_speedVar = rProperty.GetPropertyChecked("speedVariance", "real");
				PropertyToValue( *pPpt_speedVar, mode.A.speedVar );
			}

			//radial acceleration
			{
				const CGUIProperty* pPpt_radialAccel = rProperty.GetPropertyChecked("radialAcceleration", "real");
				PropertyToValue( *pPpt_radialAccel, mode.A.radialAccel );
				const CGUIProperty* pPpt_radialAccelVar = rProperty.GetPropertyChecked("radialAccelVariance", "real");
				PropertyToValue( *pPpt_radialAccelVar, mode.A.radialAccelVar );
			}

			//tangential acceleration
			{
				const CGUIProperty* pPpt_tangentialAccel = rProperty.GetPropertyChecked("tangentialAcceleration", "real");
				PropertyToValue( *pPpt_tangentialAccel, mode.A.tangentialAccel );
				const CGUIProperty* pPpt_tangentialAccelVar = rProperty.GetPropertyChecked("tangentialAccelVariance", "real");
				PropertyToValue( *pPpt_tangentialAccelVar, mode.A.tangentialAccelVar );
			}
		}
		// or Mode B: radius movement
		else if( emitterMode == eParticle2DSystemMode_Radius ) 
		{
			//radius
			{
				const CGUIProperty* pPpt_maxRadius = rProperty.GetPropertyChecked("maxRadius", "real");
				PropertyToValue( *pPpt_maxRadius, mode.B.startRadius );
				const CGUIProperty* pPpt_maxRadiusVar = rProperty.GetPropertyChecked("maxRadiusVariance", "real");
				PropertyToValue( *pPpt_maxRadiusVar, mode.B.startRadiusVar );
				const CGUIProperty* pPpt_minRadius = rProperty.GetPropertyChecked("minRadius", "real");
				PropertyToValue( *pPpt_minRadius, mode.B.endRadius );
				mode.B.endRadiusVar = 0;
			}

			//rotate
			{
				const CGUIProperty* pPpt_rotatePerSecond = rProperty.GetPropertyChecked("rotatePerSecond", "real");
				PropertyToValue( *pPpt_rotatePerSecond, mode.B.rotatePerSecond );
				const CGUIProperty* pPpt_rotatePerSecondVar = rProperty.GetPropertyChecked("rotatePerSecondVariance", "real");
				PropertyToValue( *pPpt_rotatePerSecondVar, mode.B.rotatePerSecondVar );
			}
		}
		else
		{
			throw CGUIException( "[CGUIParticle2DSystem::LoadValueFromProperty]: unknown emitter mode!" );
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIParticle2DSystem::DoLoad()
	{
		GUI_ASSERT( particles==NULL, "invalid pointer");
		if( totalParticles <= 0 )
		{
			throw CGUIException( "CGUIParticle2DSystem::DoLoad: totalparticles shouldn't be zero" );
			return -1;
		}
		particles = (CGUIParticle2D*)calloc( totalParticles, sizeof(CGUIParticle2D) );
		if( !particles ) 
		{
			throw CGUIException( "[CGUIParticle2DSystem::CGUIParticle2DSystem]: not enough memory" );
			return -1;
		}

		GUI_ASSERT( texture == NULL, "invalid pointer" );
		texture = CGUITextureManager::Instance()->CreateTexture( m_strFullTexturePath );
		if( !texture )
		{
			throw CGUIException("[CGUIParticle2DSystem::DoLoad]: failed to create texture from path <%s>", m_strFullTexturePath.c_str());
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::DoUnload()
	{
		if( particles )
		{
			free( particles );
			particles = NULL;
		}

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
		{
			particle->deltaSize = 0;
		}
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
		particle->startPos = CGUIVector2::ZERO;

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
		Load();

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
		CGUIVector2 currentPosition = CGUIVector2::ZERO;

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
					CGUIVector2 radial = CGUIVector2::ZERO;
					// radial acceleration
					if(p->pos.x || p->pos.y)
					{
						radial = p->pos.NormalisedCopy();
					}

					CGUIVector2 tangential = radial;
					radial = radial * p->mode.A.radialAccel;

					// tangential acceleration
					real newy = tangential.x;
					tangential.x = -tangential.y;
					tangential.y = newy;
					tangential = tangential * p->mode.A.tangentialAccel;

					// (gravity + radial + tangential) * rDeltaTime
					CGUIVector2 tmp = ( radial + tangential + mode.A.gravity ) * rDeltaTime;
					p->mode.A.dir = p->mode.A.dir + tmp;
					SParticle2DVector2 tmp2 = p->mode.A.dir * rDeltaTime;
					p->pos = p->pos + tmp2;
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

				//position
				CGUIVector2 diff = currentPosition - p->startPos;
				CGUIVector2 newPos = p->pos - diff;

				//
				// update values in quad
				//

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
	void CGUIParticle2DSystem::Render( class IGUIInterfaceRender* /*pRender*/ )
	{
		// should be override
		Load();
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
	void CGUIParticle2DSystem::SetTexture( const CGUIString& rTexturePath )
	{
		m_strFullTexturePath = rTexturePath;
		if( IsLoaded() )
		{
			throw CGUIException( "[CGUIParticle2DSystem::SetTexture]: system has been loaded.");
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystem::SetBlendAdditive( bool additive )
	{
		if( additive )
		{
			blendFunc.src = eBlendFunc_SRC_ALPHA;
			blendFunc.dst = eBlendFunc_ONE;

		}
		else 
		{
			blendFunc.src = eBlendFunc_ONE;
			blendFunc.dst = eBlendFunc_SRC_ALPHA;
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIParticle2DSystem::IsBlendAdditive()
	{
		return( blendFunc.src == eBlendFunc_SRC_ALPHA && blendFunc.dst == eBlendFunc_ONE);
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
	const SGUIBlendFunc& CGUIParticle2DSystem::GetBlendFuncType() const
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
	void CGUIParticle2DSystem::SetTotalParticles( uint32 nTotalParticle )
	{
		totalParticles = nTotalParticle;
	}
	//------------------------------------------------------------------------------
}

