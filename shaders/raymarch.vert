#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec4 clipPos;
out vec3 viewDir;
out vec4 projPos;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 screenSize;
uniform vec3 cameraPos;

vec4 ComputeScreenPos (vec4 pos)
{
    vec4 o = pos * 0.5f;
    o.xy = vec2(o.x * screenSize.x, o.y * screenSize.y) + o.w;
    o.zw = pos.zw;
    return o;
}

void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    clipPos = projection * view * vec4(worldPos, 1.0);
    projPos = ComputeScreenPos(clipPos);
    viewDir = worldPos - cameraPos;

    gl_Position = clipPos;
}