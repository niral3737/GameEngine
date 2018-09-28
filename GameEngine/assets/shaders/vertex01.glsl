#version 420 
// vertex01.glsl

//uniform mat4 MVP;		
uniform mat4 matModel;		// M
uniform mat4 matView;		// V
uniform mat4 matProj;		// P

in vec3 vColour;		// rgb   	was "attribute"
in vec3 vPosition;		// xyz		was "attribute"

out vec3 color;			// exit to fragment
out vec4 vertPosWorld;

void main()
{
	vec3 posTemp = vPosition;
	
	// Note these are 'backwards'
	mat4 MVP = matProj * matView * matModel;
    gl_Position = MVP * vec4(posTemp, 1.0);
	
	vertPosWorld = matModel * vec4(posTemp, 1.0);
	
    color = vColour;
}



