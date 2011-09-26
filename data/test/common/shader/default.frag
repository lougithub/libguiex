uniform sampler2D tex;
void main(void)
{
	vec3 ct,cf;
	float at,af;
	vec4 texel;
	texel = texture(tex,gl_TexCoord[0].st );
	ct = texel.rgb;
	at = texel.a;
	cf = gl_Color.rgb;
	af = gl_Color.a;
	gl_FragColor = vec4(ct*cf,at*af);
}
