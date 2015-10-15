#version 330

precision highp float; // required by GLSL spec Sect 4.5.3

// Input & Uniforms
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

in vec2 uvCoord;

uniform vec2 uCamProjRect; // The rect for the camera 1 distance forward.

// Constants
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// Camera
const vec3 camOrigin = vec3(0, 0, -2);
const vec3 camUp = vec3(0, 1, 0);
const vec3 camRight = vec3(1, 0, 0);
const vec3 camForward = vec3(0, 0, 1);

const float EPS = 0.001;
const float MAX_STEPS = 20;

// Primitives (from Iñigo Quílez)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

float sdSphere(vec3 p, float radius)
{
	return length(p) - radius;
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

// General raymarch functions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

float sceneDistance(vec3 p)
{
	return sdTorus(p, vec2(0.2, 0.2));
	//return sdSphere(p, 0.5);
}

vec3 calculateNormal(vec3 p)
{
	// https://en.wikipedia.org/wiki/Finite_difference
	const float NORMAL_EPS = EPS;
	vec3 normal;
	normal.x = sceneDistance(p + vec3(NORMAL_EPS,0,0)) - sceneDistance(p - vec3(NORMAL_EPS,0,0));
	normal.y = sceneDistance(p + vec3(0,NORMAL_EPS,0)) - sceneDistance(p - vec3(0,NORMAL_EPS,0));
	normal.z = sceneDistance(p + vec3(0,0,NORMAL_EPS)) - sceneDistance(p - vec3(0,0,NORMAL_EPS));
	return normalize(normal);
}

float march(vec3 origin, vec3 dir, out int numSteps)
{
	float dist = 0;
	for (numSteps = 0; numSteps < MAX_STEPS; ++numSteps) {
		float sceneDist = sceneDistance(origin + dir*dist);
		if (sceneDist < EPS) {
			return dist;
		}
		dist += sceneDist;
	}
	return -1;
}

// Main
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

void main()
{
	vec2 camTargetRectOffs = (uvCoord-vec2(0.5))*uCamProjRect;
	vec3 camTarget = (camOrigin + camForward) + camTargetRectOffs.y*camUp + camTargetRectOffs.x*camRight;
	vec3 rayDir = normalize(camTarget - camOrigin);

	int numSteps = 0;
	float dist = march(camOrigin, rayDir, numSteps);
	if (dist < 0) {
		gl_FragColor = vec4(0, 0, 0, 1);
		return;
	}



	vec3 pos = camOrigin + rayDir*dist;
	vec3 normal = calculateNormal(pos);
	gl_FragColor = vec4(dot((-rayDir), normal)*vec3(1,0,0), 1);
	//gl_FragColor = vec4(normal, 1);
	//gl_FragColor = vec4(1, 0, 0, 1) * (float(numSteps) / float(MAX_STEPS)); // Calculate rendering intensity
}