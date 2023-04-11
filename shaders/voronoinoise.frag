#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec2 mouse = u_mouse/u_resolution.xy;
    mouse.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(.0);
    float m_dist = 10.;
    vec2 m_point; 

    // Scale
    st *= 3.;
    mouse *= 3.;

    // Tile the space
    vec2 i_st = floor(st); // Integer location of cell
    vec2 f_st = fract(st); // actual cells

    for (int y= -1; y <= 1; y++)
    {
        for (int x= -1; x <= 1; x++)
        {
            vec2 neighbor = vec2(float(x),float(y));
            vec2 point = random2(i_st + neighbor);
            vec2 diff = neighbor + point - f_st;
            float dist = length(diff);
            if( dist < m_dist )
            {
                m_dist = dist;
                m_point = point;
            }
        }
    }

    color += dot(m_point,vec2(.3,.6));

    gl_FragColor = vec4(color,1.0);
}