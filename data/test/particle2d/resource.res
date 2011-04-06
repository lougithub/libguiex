<?xml version="1.0" ?>
<guiex>
	<property name="DemoFlower" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="250"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="4"/>
		<property name="particleLifespanVariance" type="real" value="1"/>
		<property name="textureFileName" type="CGUIString" value="image/stars-grayscale.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="360"/>
		<property name="startColor" type="CGUIColor" value="128,128,128,255"/>
		<property name="startColorVariance" type="CGUIColor" value="128,128,128,128"/>
		<property name="finishColor" type="CGUIColor" value="0,0,0,255"/>
		<property name="finishColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="20"/>
		<property name="startParticleSizeVariance" type="real" value="10"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,0"/>
		<property name="speed" type="real" value="80"/>
		<property name="speedVariance" type="real" value="10"/>
		<property name="radialAcceleration" type="real" value="-60"/>
		<property name="radialAccelVariance" type="real" value="0"/>
		<property name="tangentialAcceleration" type="real" value="15"/>
		<property name="tangentialAccelVariance" type="real" value="0"/>
	</property>

	<property name="DemoGalaxy" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="200"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="4"/>
		<property name="particleLifespanVariance" type="real" value="1"/>
		<property name="textureFileName" type="CGUIString" value="image/fire.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="360"/>
		<property name="startColor" type="CGUIColor" value="30,60,193,255"/>
		<property name="startColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="finishColor" type="CGUIColor" value="0,0,0,255"/>
		<property name="finishColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="37"/>
		<property name="startParticleSizeVariance" type="real" value="10"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,0"/>
		<property name="speed" type="real" value="60"/>
		<property name="speedVariance" type="real" value="10"/>
		<property name="radialAcceleration" type="real" value="-80"/>
		<property name="radialAccelVariance" type="real" value="0"/>
		<property name="tangentialAcceleration" type="real" value="80"/>
		<property name="tangentialAccelVariance" type="real" value="0"/>
	</property>


	<property name="DemoFirework" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="1500"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="3.5"/>
		<property name="particleLifespanVariance" type="real" value="1"/>
		<property name="textureFileName" type="CGUIString" value="image/stars.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="20"/>
		<property name="startColor" type="CGUIColor" value="128,128,128,255"/>
		<property name="startColorVariance" type="CGUIColor" value="128,128,128,25"/>
		<property name="finishColor" type="CGUIColor" value="25,25,25,51"/>
		<property name="finishColorVariance" type="CGUIColor" value="25,25,25,51"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="8"/>
		<property name="startParticleSizeVariance" type="real" value="2"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE_MINUS_SRC_ALPHA"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,-90"/>
		<property name="speed" type="real" value="180"/>
		<property name="speedVariance" type="real" value="50"/>
		<property name="radialAcceleration" type="real" value="0"/>
		<property name="radialAccelVariance" type="real" value="0"/>
		<property name="tangentialAcceleration" type="real" value="0"/>
		<property name="tangentialAccelVariance" type="real" value="0"/>
	</property>


	<property name="DemoSpiral" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="500"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="12"/>
		<property name="particleLifespanVariance" type="real" value="0"/>
		<property name="textureFileName" type="CGUIString" value="image/fire.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="0"/>
		<property name="startColor" type="CGUIColor" value="128,128,128,255"/>
		<property name="startColorVariance" type="CGUIColor" value="128,128,128,0"/>
		<property name="finishColor" type="CGUIColor" value="128,128,128,255"/>
		<property name="finishColorVariance" type="CGUIColor" value="128,128,128,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="20"/>
		<property name="startParticleSizeVariance" type="real" value="0"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE_MINUS_SRC_ALPHA"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,0"/>
		<property name="speed" type="real" value="150"/>
		<property name="speedVariance" type="real" value="0"/>
		<property name="radialAcceleration" type="real" value="-380"/>
		<property name="radialAccelVariance" type="real" value="0"/>
		<property name="tangentialAcceleration" type="real" value="45"/>
		<property name="tangentialAccelVariance" type="real" value="0"/>
	</property>


	<property name="DemoSun" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="350"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="1"/>
		<property name="particleLifespanVariance" type="real" value="0.5"/>
		<property name="textureFileName" type="CGUIString" value="image/fire.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="360"/>
		<property name="startColor" type="CGUIColor" value="193,63,30,255"/>
		<property name="startColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="finishColor" type="CGUIColor" value="0,0,0,255"/>
		<property name="finishColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="30"/>
		<property name="startParticleSizeVariance" type="real" value="10"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,0"/>
		<property name="speed" type="real" value="20"/>
		<property name="speedVariance" type="real" value="5"/>
		<property name="radialAcceleration" type="real" value="0"/>
		<property name="radialAccelVariance" type="real" value="0"/>
		<property name="tangentialAcceleration" type="real" value="0"/>
		<property name="tangentialAccelVariance" type="real" value="0"/>
	</property>

	<property name="DemoSnow" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="700"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="3"/>
		<property name="particleLifespanVariance" type="real" value="1"/>
		<property name="textureFileName" type="CGUIString" value="image/snow.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="5"/>
		<property name="startColor" type="CGUIColor" value="230,230,230,255"/>
		<property name="startColorVariance" type="CGUIColor" value="0,25,0,0"/>
		<property name="finishColor" type="CGUIColor" value="255,255,255,0"/>
		<property name="finishColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="10"/>
		<property name="startParticleSizeVariance" type="real" value="5"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="200,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE_MINUS_SRC_ALPHA"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,-10"/>
		<property name="speed" type="real" value="130"/>
		<property name="speedVariance" type="real" value="30"/>
		<property name="radialAcceleration" type="real" value="0"/>
		<property name="radialAccelVariance" type="real" value="-1"/>
		<property name="tangentialAcceleration" type="real" value="0"/>
		<property name="tangentialAccelVariance" type="real" value="1"/>
	</property>


	<property name="DemoExplosion" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="700"/>
		<property name="duration" type="real" value="0.5"/>
		<property name="particleLifespan" type="real" value="5"/>
		<property name="particleLifespanVariance" type="real" value="2"/>
		<property name="textureFileName" type="CGUIString" value="image/stars-grayscale.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="360"/>
		<property name="startColor" type="CGUIColor" value="178,25,51,255"/>
		<property name="startColorVariance" type="CGUIColor" value="128,128,128,0"/>
		<property name="finishColor" type="CGUIColor" value="128,128,128,0"/>
		<property name="finishColorVariance" type="CGUIColor" value="128,128,128,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="15"/>
		<property name="startParticleSizeVariance" type="real" value="10"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="0,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE_MINUS_SRC_ALPHA"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="0,0"/>
		<property name="speed" type="real" value="70"/>
		<property name="speedVariance" type="real" value="40"/>
		<property name="radialAcceleration" type="real" value="0"/>
		<property name="radialAccelVariance" type="real" value="0"/>
		<property name="tangentialAcceleration" type="real" value="0"/>
		<property name="tangentialAccelVariance" type="real" value="0"/>
	</property>


	<property name="DemoRain" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="1000"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="4"/>
		<property name="particleLifespanVariance" type="real" value="0"/>
		<property name="textureFileName" type="CGUIString" value="image/fire.tga"/>
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="5"/>
		<property name="startColor" type="CGUIColor" value="178,204,255,255"/>
		<property name="startColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="finishColor" type="CGUIColor" value="178,204,255,128"/>
		<property name="finishColorVariance" type="CGUIColor" value="0,0,0,0"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="startParticleSize" type="real" value="4"/>
		<property name="startParticleSizeVariance" type="real" value="2"/>
		<property name="finishParticleSize" type="real" value="-1.0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="200,0"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_SRC_ALPHA"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE_MINUS_SRC_ALPHA"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Gravity"/>

		<property name="gravity" type="CGUIVector2" value="10,-10"/>
		<property name="speed" type="real" value="130"/>
		<property name="speedVariance" type="real" value="30"/>
		<property name="radialAcceleration" type="real" value="0"/>
		<property name="radialAccelVariance" type="real" value="1"/>
		<property name="tangentialAcceleration" type="real" value="0"/>
		<property name="tangentialAccelVariance" type="real" value="1"/>
	</property>



	<property name="spinningPeas" type="CGUIParticle2DDefine" >
		<property name="maxParticles" type="uint32" value="1000"/>
		<property name="duration" type="real" value="-1"/>
		<property name="particleLifespan" type="real" value="10"/>
		<property name="particleLifespanVariance" type="real" value="0"/>
		<property name="textureFileName" type="CGUIString" value="image/stars.tga"/><!--SpinningPeas.png-->
		<property name="angle" type="real" value="90"/>
		<property name="angleVariance" type="real" value="360"/>
		<property name="startColor" type="CGUIColor" value="255,255,255,255"/>
		<property name="startColorVariance" type="CGUIColor" value="255,255,255,255"/>
		<property name="finishColor" type="CGUIColor" value="255,255,255,255"/>
		<property name="finishColorVariance" type="CGUIColor" value="255,255,255,255"/>
		<property name="startParticleSize" type="real" value="64"/>
		<property name="startParticleSizeVariance" type="real" value="29"/>
		<property name="startSpin" type="real" value="0"/>
		<property name="startSpinVariance" type="real" value="0"/>
		<property name="endSpin" type="real" value="0"/>
		<property name="endSpinVariance" type="real" value="0"/>
		<property name="finishParticleSize" type="real" value="0"/>
		<property name="finishParticleSizeVariance" type="real" value="0"/>
		<property name="sourcePosition" type="CGUIVector2" value="0,0"/>
		<property name="sourcePositionVariance" type="CGUIVector2" value="7,7"/>
		<property name="blendFunc" type="SGUIBlendFunc">
			<property name="src" type="EBlendFunc" value="eBlendFunc_ONE"/>
			<property name="dst" type="EBlendFunc" value="eBlendFunc_ONE_MINUS_SRC_ALPHA"/>
		</property>
		<property name="emitterType" type="EParticle2DSystemMode" value="eParticle2DSystemMode_Radius"/>

		<!-- mode B -->
		<property name="maxRadius" type="real" value="331"/>
		<property name="maxRadiusVariance" type="real" value="0"/>
		<property name="minRadius" type="real" value="0"/>
		<property name="rotatePerSecond" type="real" value="0"/>
		<property name="rotatePerSecondVariance" type="real" value="68"/>
	</property>

</guiex>



