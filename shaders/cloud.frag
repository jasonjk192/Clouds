#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

float near = 0.1;
float far  = 100.0;

float _MinHeight = 0;
float _MaxHeight = 5;
float _FadeDistance = 0.1;
float _Scale = 1.0;
float _StepScale = 1.0;
float _Steps = 40;
vec4 _SunDir = vec4(1,0,0,0);

uniform vec3 cameraPos;
uniform vec3 viewDir;   // remember to send in normalized value
uniform vec2 screenSize;

uniform sampler2D noiseImage;

vec4 integrate(vec4 sum, float diffuse, float density, vec4 bgcol, float t)
{
    vec3 lighting = vec3(0.51, 0.53, 0.63) * 1.3 + 0.5 * vec3(0.85, 0.57, 0.3) * diffuse;
    vec3 colrgb = mix( vec3(1.0, 0.95, 0.8), vec3(0.65, 0.65, 0.65), density);
    vec4 col = vec4(colrgb, density);
    col.rgb *= lighting;
    col.rgb = mix(col.rgb, bgcol.rgb, 1.0 - exp(-0.003*t*t));
    col.a *= 0.35;
    col.rgb *= col.a;
    return sum + col*(1.0 - sum.a);
}

#define MARCH(noiseMap) { \
    for(int i=0; i<_Steps; i++) { \
        if(t > far) break; \
        vec3 pos = ro + t * rd; \
        if( pos.y<_MinHeight || pos.y>_MaxHeight || sum.a>0.99 )  {\
            t += max(0.1, 0.02*t); \
            continue; \
        }\
        float density = noiseMap( pos ); \
        if( density > 0.01 ) {  \
            float diffuse = clamp((density - noiseMap(pos + 0.6 * _SunDir.xyz)) / 0.6, 0.0, 1.0);\
            sum = integrate(sum, diffuse, density, bgcol, t); \
        }  \
        t += max(0.06,0.05*t);  \
    } \
}

#define NOISEPROC(N, P) 1.75 * N * clamp((_MaxHeight - P.y)/_FadeDistance, 0, 1) 

float noiseFromImage(vec3 x)
{
    x *= _Scale;
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = smoothstep(0,1,f);
    
    vec2 uv = (p.xy + vec2(37.0, -17.0) * p.z) + f.xy;
    vec2 rg = texture(noiseImage, uv/256).rg;
    return 2.0 * mix(rg.g, rg.r, f.z) - 1.0;
} 

float map5(vec3 q)
{
    vec3 p = q;
    float f;
    f = 0.5 * noiseFromImage(q);
    q = q * 2;
    f += 0.25 * noiseFromImage(q);
    q = q * 3;
    f += 0.125 * noiseFromImage(q);
    q = q * 4;
    f += 0.06250 * noiseFromImage(q);
    q = q * 5;
    f += 0.03125 * noiseFromImage(q);
    q = q * 6;
    f += 0.015625 * noiseFromImage(q);
    return NOISEPROC(f, p);
} 
            
float map4(vec3 q)
{
    vec3 p = q;
    float f;
    f = 0.5 * noiseFromImage(q);
    q = q * 2;
    f += 0.25 * noiseFromImage(q);
    q = q * 3;
    f += 0.125 * noiseFromImage(q);
    q = q * 4;
    f += 0.06250 * noiseFromImage(q);
    q = q * 5;
    f += 0.03125 * noiseFromImage(q);
    return NOISEPROC(f, p);
} 
            
float map3(vec3 q)
{
    vec3 p = q;
    float f;
    f = 0.5 * noiseFromImage(q);
    q = q * 2;
    f += 0.25 * noiseFromImage(q);
    q = q * 3;
    f += 0.125 * noiseFromImage(q);
    q = q * 4;
    f += 0.06250 * noiseFromImage(q);
    return NOISEPROC(f, p);
} 
            
float map2(vec3 q)
{
    vec3 p = q;
    float f;
    f = 0.5 * noiseFromImage(q);
    q = q * 2;
    f += 0.25 * noiseFromImage(q);
    q = q * 3;
    f += 0.125 * noiseFromImage(q);
    return NOISEPROC(f, p);
} 

float map1(vec3 q)
{    
    vec3 p = q;
    float f;
    f = 0.5 * noiseFromImage(q);
    q = q * 2;
    f += 0.25 * noiseFromImage(q);
    return NOISEPROC(f, p);
}

vec4 raymarch( vec3 ro, vec3 rd, vec4 bgcol)
{    
    vec4 sum = vec4(0);    
    float t = 0;
    MARCH(map1);
    //MARCH(map2);
    //MARCH(map3);
    //MARCH(map4);
    //MARCH(map5);
    return clamp( sum, 0.0, 1.0 );
}

vec3 sky( vec3 rd )
{
    vec3 col = vec3(0.);
    
    // Horizon and Background
    float hort = 1. - clamp(abs(rd.y), 0., 1.);
    col += 0.5*vec3(.5,.5,.99)*exp2(hort*8.-8.); // Horizon Main Halo
    col += 0.1*vec3(.9,.5,1.)*exp2(hort*3.-3.); // Horizon Halo light scatter (Secondary)
    col += 0.25*vec3(.2,.4,.9); // Main Background
    
    // Sun
    float sun = clamp( dot(_SunDir.xyz, rd), 0.0, 1.0 );
    col += .5*vec3(1.,.9,.9)*exp2(sun*650.-650.); // sun center
    col += .2*vec3(1.0,0.3,0.2)*pow( sun, 2.0 ); // 1st halo
    col += .1*vec3(1.,1.,0.1)*exp2(sun*100.-100.); // 2nd halo
    col += .3*vec3(1.,.7,0.)*exp2(sun*50.-50.); // 3rd halo
    col += .5*vec3(1.,0.3,0.05)*exp2(sun*10.-10.); // 4th halo
    
    // Stars
    float ax = atan(rd.y, length(rd.xz))/1.;
    float ay = atan(rd.z, rd.x)/2.;
    float st1 = texture(noiseImage, vec2(ax,ay)).x;
    float st2 = texture(noiseImage, .25 * vec2(ax,ay)).x;
    st1 *= st2;
    st1 = smoothstep(0.7,0.9,st1); // Tighter dropoff (not fuzzy stars)
    col = mix(col, col + 1.8 * st1, clamp(1. - 1.1 * length(col), 0., 1.));

    return col;
}

vec4 cloud( vec3 camPos, vec3 rd, vec4 bgcolor )
{ 
    return raymarch( camPos, rd * _StepScale, bgcolor );
}

void main()
{
    // TODO: Fix circular lines
    vec2 p = 2.0 * TexCoords - 1.0;
    p.x *= screenSize.x/ screenSize.y;

    // Camera's Forward Ray Direction (Defined in : https://www.shadertoy.com/view/XdfXzn)
    vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), viewDir ));
    vec3 vv = normalize(cross(viewDir,uu));
    vec3 rd = normalize( p.x*uu + 1.2*p.y*vv + 1.5*viewDir );

    vec3 skyColor = sky(rd);
    vec4 cloudColor = cloud(cameraPos, rd, vec4(skyColor,1));

    vec3 color = mix( skyColor, cloudColor.rgb, cloudColor.a );
    
    FragColor = vec4(color,1);
}