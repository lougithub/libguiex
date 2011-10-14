#ifndef GL_ES
#	define lowp
#endif

uniform sampler2D tex;

varying lowp vec2 v_v2TextCoord;
varying lowp vec4 v_v4DestinationColor;

void main(void)
{
	lowp vec3 ct,cf;
	lowp float at,af;
	lowp vec4 texel;
	texel = texture2D(tex, v_v2TextCoord );
	ct = texel.rgb;
	at = texel.a;
	cf = v_v4DestinationColor.rgb;
	af = v_v4DestinationColor.a;
	gl_FragColor = vec4(ct*cf,at*af);

	lowp float gap = 0.004f;
	lowp vec4 texel_right = texture2D(tex, vec2(v_v2TextCoord.x+gap, v_v2TextCoord.y ));
	lowp vec4 texel_left = texture2D(tex, vec2(v_v2TextCoord.x-gap, v_v2TextCoord.y ));
	lowp vec4 texel_up = texture2D(tex, vec2(v_v2TextCoord.x, v_v2TextCoord.y+gap ));
	lowp vec4 texel_low = texture2D(tex, vec2(v_v2TextCoord.x, v_v2TextCoord.y-gap ));
	if( texel.a <= 0.5 && ( texel_right.a >= 0.5 || texel_left.a >= 0.5 || texel_up.a >= 0.5 || texel_low.a >= 0.5))
	{
		gl_FragColor.rgb = 0;
		gl_FragColor.a = 1;
	}

	//vec4 color = vec4(ct*cf,at*af);
	//float intensity = dot(color,vec4(0.299,0.587,0.184,0));
	//gl_FragColor = vec4(intensity.xxx, color.a );
}
