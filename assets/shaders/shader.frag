#version 330

precision highp float; // required by GLSL spec Sect 4.5.3

// Input
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

in vec2 uvCoord;

// Constants
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// Camera
const vec3 camOrigin = vec3(0, 0, -2);
const vec3 camUp = vec3(0, 1, 0);
const vec3 camRight = vec3(1, 0, 0);
//const vec3 camDir = vec3(0, 0, -1);

const int maxMarchingSteps = 20;
const float rayMarchEps = 0.01;

// Primitives (from Iñigo Quílez)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

float sdSphere(vec3 p, float radius)
{
	return length(p) - radius;
}

// General raymarch functions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

/*bool march(vec3 origin, vec3 dir)
{
	float dist = 0;
	for (int i = 0; i < maxMarchingSteps; ++i) {
		float sceneDist = sdSphere(origin + dir*dist, 0.5);//10.0f; //sceneDist(origin + dir*dist);
		if (sceneDist < rayMarchEps) {
			// Do something with p
			return true;
		}
		dist += sceneDist;
	}
	return false;
}*/

// Main
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

void main()
{
	vec2 extUV = (uvCoord-vec2(0.5)) * 2; // Range: [-1,1]x[-1,1]

	gl_FragColor = vec4(0, 0, 0, 1);

	vec3 origin = camOrigin + camRight*extUV.x + camUp*extUV.y;
	vec3 dir = vec3(0, 0, 1);

	float dist = 0;
	for (int i = 0; i < maxMarchingSteps; ++i) {
		float sceneDist = sdSphere(origin + dir*dist, 0.5);//10.0f; //sceneDist(origin + dir*dist);
		if (sceneDist < rayMarchEps) {
			

			gl_FragColor = vec4(1, 1, 1, 1);
		}
		dist += sceneDist;
	}	
}