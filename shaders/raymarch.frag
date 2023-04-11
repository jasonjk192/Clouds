#version 330 core

in vec4 clipPos;
in vec3 viewDir;
in vec4 projPos;
in vec3 worldPos;

#define STEPS 128
#define STEP_SIZE 0.01

vec3 sphereCenter = vec3(0,0,0);
vec3 sphereColor = vec3(1,0.2,0.7);
vec3 lightPos = vec3(0,1,1);
vec3 lightColor = vec3(0.1,0.9,1);

bool SphereHit(vec3 pos, vec3 center, float radius)
{
    return distance(pos, center) < radius;
}

vec3 RaymarchHit(vec3 position, vec3 direction)
{
    for(int i = 0; i < STEPS; i++)
    {
        if(SphereHit(position, sphereCenter, 0.5))
            return position;
        position += direction * STEP_SIZE;
    }
    return vec3(0,0,0);
}

void main()
{
    vec3 depth = RaymarchHit(worldPos, viewDir);
    vec3 worldNormal = depth - sphereCenter;
    float nl = max(0, dot(worldNormal, lightPos)); 

    if(depth.x != 0 || depth.y != 0 || depth.z != 0)
    {
        sphereColor *= nl * lightColor;
        gl_FragData[0] = vec4(sphereColor, 1);
    } 
    else
        gl_FragData[0] = vec4(0, 0, 0, 0);
}