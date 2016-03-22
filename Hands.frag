varying vec3 N;
varying vec4 v;

uniform vec3 Color;

void main(void)
{
	vec4 LightDiffuse = vec4(Color, 1.0);

	vec3 LightPos = vec3(0.0, 0.0, 0.0);
	vec3 L = normalize(LightPos - vec3(v));   
	vec4 Idiff = LightDiffuse * max(dot(N,L), 0.0);  
	Idiff = clamp(Idiff, 0.0, 1.0); 

	gl_FragColor = Idiff;
}