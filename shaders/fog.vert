#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec4 clipPos;
out vec3 viewDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 screenSize;
uniform vec3 cameraPos;


void main()
{
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    clipPos = projection * view * vec4(worldPos, 1.0);
    viewDir = normalize(worldPos - cameraPos);

	float inFrontOf = (clipPos.z/clipPos.w) > 0 ? 1.0 : 0.0;
    clipPos.z *= inFrontOf;

    gl_Position = clipPos;
}