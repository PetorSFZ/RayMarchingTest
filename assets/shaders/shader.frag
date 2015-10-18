#version 330

// Input & Uniforms
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

in vec2 uvCoord;

uniform int uIntensityRendering;
uniform float uTime;

uniform vec3 uCamPos;
uniform vec3 uCamDir;
uniform vec3 uCamUp;
uniform vec3 uCamRight;
uniform vec2 uCamProjRect; // The rect for the camera 1 distance forward.

out vec4 outFragColor;

// Constants
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

const float EPS = 0.005;
const int MAX_STEPS = 300;
const float MAX_DIST = 60.0;
const vec3 BG_COLOR = vec3(0.85, 0.85, 0.9);

// Primitives (from Iñigo Quílez)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

float sdSphere(vec3 p, float radius)
{
	return length(p) - radius;
}

float udBox(vec3 p, vec3 b)
{
	return length(max(abs(p)-b,0));
}

float sdBox(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float sdPlane(vec3 p, vec4 n)
{
	// n must be normalized
	return dot(p,n.xyz) + n.w;
}

float sdTorus(vec3 p, vec2 t)
{
	vec2 q = vec2(length(p.xz)-t.x,p.y);
	return length(q)-t.y;
}

// Distance operations (from Iñigo Quílez)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

float opUnion(float d1, float d2)
{
	return min(d1,d2);
}

float opSubtract(float d1, float d2) // Order changed compared Inigo's version
{
	return max(d1,-d2);
}

float opIntersect(float d1, float d2)
{
	return max(d1,d2);
}

// Scene defintion
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

float ground(vec3 p)
{
	return sdPlane(p, vec4(0,1,0,0));
}

float sphereHoledCube(vec3 p)
{
	return opSubtract(sdBox(p-vec3(0,3,0),vec3(3,3,3)), sdSphere(p-vec3(0,3,0),3.8));
}

float blocks(vec3 p)
{
	float cosF = cos(uTime*0.5);
	float sinF = sin(uTime*0.5);
	vec3 c = vec3(sinF*15,0,cosF*15);
	vec3 q = mod(p,c)-0.5*c;
	//return sdBox(q-vec3(0,0.5,0), vec3(2, 10, 2));
	return sdBox(q-vec3(0,0.5,0), max(abs(vec3(cosF*2, 10, sinF*2)),vec3(1)));
}

float sceneDistance(vec3 p)
{
	float dist = ground(p);
	dist = opUnion(dist, sphereHoledCube(p));
	dist = opUnion(dist, blocks(p));
	return dist;
}

struct Material {
	vec3 diffuseColor, specularColor;
	float shininess;
};

Material getMaterial(vec3 p)
{
	if (ground(p) < EPS) {
		return Material(vec3(0.4,1,0), vec3(0.5), 4);
	}

	if (sphereHoledCube(p) < EPS) {
		return Material(vec3(0.3,0.3,1.0), vec3(1), 12);
	}

	// Default material
	return Material(vec3(0.75), vec3(0.75), 6);
}

vec3 ambientLight = vec3(0.15);
vec3 globalLightDir = normalize(vec3(1, -1, -0.5));
vec3 globalLightColor = vec3(1, 1, 1);

struct SpotLight {
	vec3 pos, dir, color;
	float reach;	
};

const int NUM_SPOT_LIGHTS = 1;
SpotLight spotLights[NUM_SPOT_LIGHTS] = SpotLight[NUM_SPOT_LIGHTS](
	SpotLight(vec3(0,4,0), normalize(vec3(0,-1,0)), vec3(1,0,0), 2.0)
	//SpotLight(vec3(6,0.25,-4), normalize(vec3(0,-1,0)), vec3(1,0,0), 2.0)
);


// General raymarch functions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

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
		if (dist > MAX_DIST) break;
	}
	return -1;
}

float shadowFactor(vec3 origin, vec3 dir)
{
	const float SHADOW_BIAS = EPS*10;
	const int SHADOW_MAX_STEPS = MAX_STEPS*5;
	const float SHADOW_MAX_DIST = MAX_DIST;
	const float SHADOW_STEP_SIZE = 0.01;
	const float SHADOW_SOFT_FACTOR = 32.0f;

	float res = 1.0;
	float dist = SHADOW_BIAS;//sceneDistance(origin);
	for (int numSteps = 0; numSteps < SHADOW_MAX_STEPS; ++numSteps) {
		float sceneDist = sceneDistance(origin + dir*dist);
		if (sceneDist < EPS) return 0.0;
		dist += SHADOW_STEP_SIZE;
		res = min(res, SHADOW_SOFT_FACTOR*sceneDist/dist);
		if (dist > SHADOW_MAX_DIST) break;
	}
	return res;
}

float ambientOcclusionFactor(vec3 p, vec3 normal)
{
	// TODO: Can be further optimized.

	const float AO_K = 8.0;
	const float AO_STEP_SIZE = 0.075;
	float res = 1.0;
	float dist = 0.0;
	for (int i = 1; i <= 5; ++i) {
		dist += AO_STEP_SIZE;
		res -= (AO_K / pow(2,i)) * (dist - sceneDistance(p + normal*dist));
	}
	return res;
}

vec4 intensityColor(int numSteps)
{
	float numStepsF = float(numSteps);
	float maxNumStepsSqrt = sqrt(float(MAX_STEPS));
	float maxNumStepsSqrtSqrt = sqrt(maxNumStepsSqrt);
	if (numStepsF < maxNumStepsSqrtSqrt) {
		return vec4(vec3(0,1,0) * (numStepsF/maxNumStepsSqrtSqrt),1);
	}
	else if (numStepsF < maxNumStepsSqrt) {
		return vec4(vec3(0,0,1) * (numStepsF/maxNumStepsSqrt),1);
	}
	else {
		return vec4(vec3(1,0,0) * (numStepsF/float(MAX_STEPS)),1);
	}
}

vec3 spotLightContribution(vec3 p, vec3 normal, Material material, vec3 toCam, int i)
{
	SpotLight light = spotLights[i];

	vec3 toLight = normalize(light.pos - p);

	// TODO: Implement proper shadows
	float shadow = dot(-light.dir, toLight);

	vec3 specularMaterial = material.specularColor;
	float diffuseIntensity = max(dot(toLight, normal), 0.0);
	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0) {
		vec3 halfVec = normalize(toLight + toCam);
		float specularAngle = max(dot(normal, halfVec), 0.0);
		specularIntensity = pow(specularAngle, material.shininess);

		// Fresnel effect
		float fresnelBase = max(1.0 - max(dot(normal, toCam), 0.0), 0.0);
		float fresnel = pow(fresnelBase, 5.0);
		specularMaterial = specularMaterial + (1.0-specularMaterial) * fresnel;
	}
	vec3 diffuse = diffuseIntensity * light.color * material.diffuseColor;
	vec3 specular = specularIntensity * light.color * specularMaterial;

	return shadow * (diffuse + specular);
}

vec4 shade(vec3 p, vec3 toCam)
{
	vec3 normal = calculateNormal(p);
	Material material = getMaterial(p);

	vec3 spotLightDiffuseSpecular = vec3(0);
	float amountPerSpotLight = 1.0f/float(NUM_SPOT_LIGHTS);
	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i) {
		spotLightDiffuseSpecular += (spotLightContribution(p, normal, material, toCam, i)*amountPerSpotLight);
	}

	//vec3 toLight = normalize(-globalLightDir);
	vec3 toLight = normalize(vec3(cos(uTime),1,sin(uTime)));
	int numSteps = 0;
	float shadow = shadowFactor(p, toLight);
	float ao = ambientOcclusionFactor(p, normal);

	vec3 specularMaterial = material.specularColor;
	float diffuseIntensity = max(dot(toLight, normal), 0.0);
	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0) {
		vec3 halfVec = normalize(toLight + toCam);
		float specularAngle = max(dot(normal, halfVec), 0.0);
		specularIntensity = pow(specularAngle, material.shininess);

		// Fresnel effect
		float fresnelBase = max(1.0 - max(dot(normal, toCam), 0.0), 0.0);
		float fresnel = pow(fresnelBase, 5.0);
		specularMaterial = specularMaterial + (1.0-specularMaterial) * fresnel;
	}
	vec3 diffuse = shadow * diffuseIntensity * globalLightColor * material.diffuseColor;
	vec3 specular = shadow * specularIntensity * globalLightColor * specularMaterial;

	return vec4((ao * ambientLight) + diffuse + specular + spotLightDiffuseSpecular, 1);
}

// Main
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

void main()
{
	// Calculate ray direction
	vec2 camTargetRectOffs = (uvCoord-vec2(0.5))*uCamProjRect;
	vec3 camTarget = (uCamPos + uCamDir) + camTargetRectOffs.y*uCamUp + camTargetRectOffs.x*uCamRight;
	vec3 rayDir = normalize(camTarget - uCamPos);

	// March until intersection is found or max steps
	int numSteps = 0;
	float dist = march(uCamPos, rayDir, numSteps);
	if (dist < 0) {
		if (uIntensityRendering != 0) {
			outFragColor = intensityColor(numSteps);
		} else {
			outFragColor = vec4(BG_COLOR,1);
		}
		return;
	}

	if (uIntensityRendering != 0) {
		outFragColor = intensityColor(numSteps);
	} else {
		outFragColor = shade(uCamPos + rayDir*dist, -rayDir);
	}
}