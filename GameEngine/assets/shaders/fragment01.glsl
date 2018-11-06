#version 420 
//fragment01.glsl

// In from a previous stage (vertex shader)
in vec3 color;		// in from the vertex shader
in vec4 vertPosWorld;
in vec3 vertNormal;	// "Model space" (only rotation)
in vec4 vertUV_x2;		// Texture coordinates

uniform vec4 objectDiffuse;
uniform vec4 objectSpecular;

uniform vec3 eyeLocation;
//uniform vec3 objectColour;
// Set this to true (1), and the vertex colour is used
uniform bool useVertexColour;
uniform bool bDontUseLighting;	

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

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

// CAN'T put texture samplers into an array (sadly)
//uniform sampler textures[10];

uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D texture04;
uniform sampler2D texture05;
uniform sampler2D texture06;
uniform sampler2D texture07;

// This is 4 x 2 floats or 8 floats
uniform vec4 texBlendWeights[2];	// x is 0, y is 1, z is 2


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
		vec4 tex0Col = texture( texture00, vertUV_x2.st ).rgba;
		vec4 tex1Col = texture( texture01, vertUV_x2.st ).rgba;
		vec4 tex2Col = texture( texture02, vertUV_x2.st ).rgba;
		vec4 tex3Col = texture( texture03, vertUV_x2.st ).rgba;
		vec4 tex4Col = texture( texture04, vertUV_x2.st ).rgba;
		vec4 tex5Col = texture( texture05, vertUV_x2.st ).rgba;
		vec4 tex6Col = texture( texture06, vertUV_x2.st ).rgba;
		vec4 tex7Col = texture( texture07, vertUV_x2.st ).rgba;
		
		materialDiffuse =  objectDiffuse
						  + (tex0Col * texBlendWeights[0].x) 	 // 0
		                  + (tex1Col * texBlendWeights[0].y)     // 1
						  + (tex2Col * texBlendWeights[0].z)     // 2
		                  + (tex3Col * texBlendWeights[0].w)     // 3
		                  + (tex4Col * texBlendWeights[1].x)     // 4
		                  + (tex5Col * texBlendWeights[1].y)     // 5
		                  + (tex6Col * texBlendWeights[1].z)     // 6
		                  + (tex7Col * texBlendWeights[1].w);    // 7
		//gl_FragColor = vec4(objectColour, 1.0);
//		materialDiffuse = objectDiffuse;
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

		int intLightType = int(theLights[index].param1.x);

		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
			
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (materialDiffuse.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)

			// TODO: Still need to do specular, but this gives you an idea
			finalOutputColour.rgb = finalObjectColour.rgb;
			finalOutputColour.a = 1.0f;

			return;		
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

		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertPosWorld.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
			
		finalObjectColour.rgb += (materialDiffuse.rgb * lightDiffuseContrib.rgb) 
		                         + (materialSpecular.rgb * lightSpecularContrib.rgb);
		// ********************************************************
	}//for(intindex=0...
	
//	if(vertPosWorld.y > 0.2f){
//		finalOutputColour.rgb = finalObjectColour.rgb;
//	}else{
//		finalObjectColour.r = 0.0f;
//		finalOutputColour.g = 0.0f;
//		finalOutputColour.b = 1.0f;
//	}
//	finalOutputColour.a = 1.0f;

	finalOutputColour.rgb = finalObjectColour.rgb;
	finalOutputColour.a = 1.0f;

	
}
