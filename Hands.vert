//#version 120

varying vec3 N;
varying vec4 v;

void main(void)
{
	v = gl_ModelViewMatrix * gl_Vertex;
	N = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;	
}