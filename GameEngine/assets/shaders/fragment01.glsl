#version 420 
//fragment01.glsl

// In from a previous stage (vertex shader)
in vec3 color;		// in from the vertex shader
in vec4 vertPosWorld;

uniform vec3 objectColour;
// Set this to true (1), and the vertex colour is used
uniform bool useVertexColour;

// Good enough for Rock-n-Roll
uniform vec3 lightPos;
uniform float lightBrightness;		

//vec4 gl_FragColor
out vec4 finalOutputColour;		// Any name, but must be vec4


void main()
{
	if ( useVertexColour )
	{
		//gl_FragColor = vec4(color, 1.0);
		finalOutputColour = vec4(color, 1.0);
	}
	else
	{
		//gl_FragColor = vec4(objectColour, 1.0);
		finalOutputColour = vec4(objectColour, 1.0);
	}
	
	// Calculate the distance between the "light" 
	// and THIS vertex (in 'world space')
	float distToLight = distance(vertPosWorld.xyz, lightPos.xyz);
	
	
	distToLight = abs(distToLight);
	
	float atten = (1.0f/distToLight) * lightBrightness;
	// Clamp output to 1.0f
	atten = min( 1.0f, atten );
	
	// Drop the brightness of the light based on distance
	finalOutputColour *= atten;
	
//	finalOutputColour *= 0.001f;
//	finalOutputColour.r += distToLight;
}
