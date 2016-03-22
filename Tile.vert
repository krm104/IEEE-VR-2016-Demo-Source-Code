
/*************************************************************
Name: Kenny Moser	krm104@gmail.com
Course: ECE8990
File:	ground.vert

This is the vertex shader for transitioning the ground
between seasons
*************************************************************/

void main(void)
{
    gl_TexCoord[0]  = gl_MultiTexCoord0;
    gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
}