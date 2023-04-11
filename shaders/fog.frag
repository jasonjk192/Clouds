#version 330 core

in vec4 clipPos;
in vec3 viewDir;

uniform vec3 cameraPos;

#define STEPS 24

vec4 _FogCenter = vec4(0,0,0,0.5);
vec4 _FogColor = vec4(1,1,1,1);
float _InnerRatio = 0.5;
float _Density = 0.5;

float near = 0.1;
float far  = 100.0;

float saturate(float x) { return clamp(x,0,1); }

float CalculateFogIntensity(float maxDistance)
{
    //maxDistance = 100;

    // Calculating intersection of ray on sphere

    vec3 sphereCenter = _FogCenter.xyz;
    float sphereRadius = _FogCenter.w;
    float innerRatio = _InnerRatio;
    float density = _Density;

    vec3 localCam = cameraPos - sphereCenter;
    float a = dot(viewDir, viewDir);                                    // a = D * D
    float b = 2 * dot(viewDir, localCam);                               // b = 2 * camera * D
    float c = dot(localCam, localCam) - sphereRadius * sphereRadius;    // c = camera * camera - R * R

    float d = b*b - 4*a*c;
    if(d <= 0.0f)
        return 0;

    float DSqrt = sqrt(d);
    float d1 = max((-b - DSqrt) / 2*a, 0);
    float d2 = max((-b + DSqrt) / 2*a, 0);

    float backDepth = min(maxDistance, d2);

    float currentDepth = d1;
    float step_distance = (backDepth - d1) / STEPS;
    float step_contribution = density;

    float centerValue = 1 / (1 - innerRatio);
    float clarity = 1;

    // Ray march into the sphere to determine fog value (clarity)

    for(int segment = 0; segment < STEPS; segment++)
    {
        vec3 position = localCam + viewDir * currentDepth;
        float val = saturate(centerValue * (1 - length(position)/sphereRadius));
        float fog_amount = saturate(val * step_contribution);
        clarity *= 1 - fog_amount;
        currentDepth += step_distance;
    }

    return 1.0 - clarity;
}

float LinearizeDepth()
{
    float ndc_depth = clipPos.z / clipPos.w;
    return (((far-near) * ndc_depth) + near + far) / 2.0;
}

void main()
{
    vec4 color = vec4(_FogColor.rgb, 1);
    float depth = LinearizeDepth();

    color.a = CalculateFogIntensity(depth);
    gl_FragData[0] = vec4(color.rgb * color.a, color.a);
    //gl_FragData[0] = vec4(depth, depth, depth, 1);
}
