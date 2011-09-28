uniform sampler2D tex;

varying vec2 v_v2TextCoord;

void main(void)
{
	vec3 ct,cf;
	float at,af;
	vec4 texel;
	texel = texture(tex, v_v2TextCoord );
	ct = texel.rgb;
	at = texel.a;
	cf = gl_Color.rgb;
	af = gl_Color.a;
	gl_FragColor = vec4(ct*cf,at*af);

	//vec4 color = vec4(ct*cf,at*af);
	//float intensity = dot(color,vec4(0.299,0.587,0.184,0));
	//gl_FragColor = vec4(intensity.xxx, color.a );
}
