attribute vec4 a_v4Position;

uniform mat4 u_m4ModelViewProjectionMatrix;

void main()
{	
	// the following three lines provide the same result
	gl_Position = u_m4ModelViewProjectionMatrix * a_v4Position;
}
