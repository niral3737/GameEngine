#version 420 
//fragment01.glsl

// In from a previous stage (vertex shader)
in vec3 color;		// in from the vertex shader
in vec4 vertPosWorld;
in vec3 vertNormal;	// "Model space" (only rotation)

uniform vec4 objectDiffuse;
uniform vec4 objectSpecular;

uniform vec3 eyeLocation;
//uniform vec3 objectColour;
// Set this to true (1), and the vertex colour is used
uniform bool useVertexColour;
uniform bool bDontUseLighting;	

// Good enough for Rock-n-Roll
uniform vec3 lightPos;
uniform float lightBrightness;		

//vec4 gl_FragColor
out vec4 finalOutputColour;		// Any name, but must be vec4

struct sLight
{
	vec4 position;			
	vec4 diffuse;		
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	vec4 param2;	// x = 0 for off, 1 for on
};

const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

void main()
{
	vec4 materialDiffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 materialSpecular = objectSpecular;

	if ( useVertexColour )
	{
		//gl_FragColor = vec4(color, 1.0);
		materialDiffuse = vec4(color, 1.0f );
	}
	else
	{
		//gl_FragColor = vec4(objectColour, 1.0);
		materialDiffuse = objectDiffuse;
	}

	// Is this being lit? 
	if ( bDontUseLighting )
	{
		// Just exit early
		finalOutputColour = objectDiffuse;
		return;
	}

	vec3 norm = normalize(vertNormal);
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );

	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertPosWorld.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, norm);

		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;

		//specular
		vec3 lightSpecularContrib = vec3(0.0f);

		vec3 reflectVector = reflect( -lightVector, normalize(norm) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertPosWorld.xyz);
		
		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = objectSpecular.w; 

		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * objectSpecular.rgb;	//* theLights[lightIndex].Specular.rgb

		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
			
		finalObjectColour.rgb += (materialDiffuse.rgb * lightDiffuseContrib.rgb) 
		                         + (materialSpecular.rgb * lightSpecularContrib.rgb);
		// ********************************************************
	}//for(intindex=0...
	
	finalOutputColour.rgb = finalObjectColour.rgb;
	finalOutputColour.a = 1.0f;
}
