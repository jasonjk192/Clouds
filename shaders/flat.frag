#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D colorTexture;

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    gl_FragData[0] = texture(colorTexture, TexCoord);

    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    gl_FragData[1]  = vec4(vec3(1 - depth), 1.0);
}