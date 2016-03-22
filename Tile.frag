
/*************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:	ground.frag

This is the fragment shader for transitioning the ground
between seasons
*************************************************************/

uniform sampler2D Texture;
uniform float Alpha;
void main(void)
{
	vec3 lightColor = vec3(texture2D(Texture, gl_TexCoord[0].st));
	
	gl_FragColor = vec4(lightColor, Alpha);
}

