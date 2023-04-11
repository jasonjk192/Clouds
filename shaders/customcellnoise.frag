#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec4 permute(vec4 x) {
  return mod((34.0 * x + 1.0) * x, 289.0);
}

vec2 cellular2x2(vec2 P) {
#define K 0.142857142857 // 1/7
#define K2 0.0714285714285 // K/2
#define jitter 0.8 // jitter 1.0 makes F1 wrong more often
	vec2 Pi = mod(floor(P), 289.0);
 	vec2 Pf = fract(P);
	vec4 Pfx = Pf.x + vec4(-0.5, -1.5, -0.5, -1.5);
	vec4 Pfy = Pf.y + vec4(-0.5, -0.5, -1.5, -1.5);
	vec4 p = permute(Pi.x + vec4(0.0, 1.0, 0.0, 1.0));
	p = permute(p + Pi.y + vec4(0.0, 0.0, 1.0, 1.0));
	vec4 ox = mod(p, 7.0)*K+K2;
	vec4 oy = mod(floor(p*K),7.0)*K+K2;
	vec4 dx = Pfx + jitter*ox;
	vec4 dy = Pfy + jitter*oy;
	vec4 d = dx * dx + dy * dy; // d11, d12, d21 and d22, squared
	// Sort out the two smallest distances
#if 0
	// Cheat and pick only F1
	d.xy = min(d.xy, d.zw);
	d.x = min(d.x, d.y);
	return d.xx; // F1 duplicated, F2 not computed
#else
	// Do it right and find both F1 and F2
	d.xy = (d.x < d.y) ? d.xy : d.yx; // Swap if smaller
	d.xz = (d.x < d.z) ? d.xz : d.zx;
	d.xw = (d.x < d.w) ? d.xw : d.wx;
	d.y = min(d.y, d.z);
	d.y = min(d.y, d.w);
	return sqrt(d.xy);
#endif
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

    //color += dot(m_point,vec2(.3,.6));

    vec2 F = cellular2x2(st*3.);

	vec2 pos = st - mouse;
    float a = dot(pos,pos) - u_time * 0.1;
    float n = step(abs(sin(a*3.1415*5.)),F.x*2.);

    color = vec3(n);

    gl_FragColor = vec4(color,1.0);
}