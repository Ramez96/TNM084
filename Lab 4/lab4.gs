#version 410 core

layout(triangles) in;
// Use line_strip for visualization and triangle_strip for solids
//layout(triangle_strip, max_vertices = 3) out;
layout(triangle_strip, max_vertices = 3) out;
in vec2 teTexCoord[3];
in vec3 teNormal[3];
out vec2 gsTexCoord;
out vec3 gsNormal;
uniform sampler2D tex;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

uniform float disp;
uniform int texon;

vec2 random2(vec2 st)
{
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
float noise(vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

vec3 random3(vec3 st)
{
    st = vec3( dot(st,vec3(127.1,311.7, 543.21)),
              dot(st,vec3(269.5,183.3, 355.23)),
              dot(st,vec3(846.34,364.45, 123.65)) ); // Haphazard additional numbers by IR
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
// Trivially extended to 3D by Ingemar
float noise(vec3 st)
{
    vec3 i = floor(st);
    vec3 f = fract(st);

    vec3 u = f*f*(3.0-2.0*f);

    return mix(
    			mix( mix( dot( random3(i + vec3(0.0,0.0,0.0) ), f - vec3(0.0,0.0,0.0) ),
                     dot( random3(i + vec3(1.0,0.0,0.0) ), f - vec3(1.0,0.0,0.0) ), u.x),
                mix( dot( random3(i + vec3(0.0,1.0,0.0) ), f - vec3(0.0,1.0,0.0) ),
                     dot( random3(i + vec3(1.0,1.0,0.0) ), f - vec3(1.0,1.0,0.0) ), u.x), u.y),

    			mix( mix( dot( random3(i + vec3(0.0,0.0,1.0) ), f - vec3(0.0,0.0,1.0) ),
                     dot( random3(i + vec3(1.0,0.0,1.0) ), f - vec3(1.0,0.0,1.0) ), u.x),
                mix( dot( random3(i + vec3(0.0,1.0,1.0) ), f - vec3(0.0,1.0,1.0) ),
                     dot( random3(i + vec3(1.0,1.0,1.0) ), f - vec3(1.0,1.0,1.0) ), u.x), u.y), u.z

          	);
}

void computeVertex(int nr)
{
	vec3 p, v1, v2, v3, p1, p2, p3, s1, s2, n;

	p = vec3(gl_in[nr].gl_Position);



	// Add interesting code here
	//Each vertex point has the same distance to center.
	p = normalize(p);
	p = p * (1 + 0.4 * noise(p * 1.5));
	p = p * (1 + 0.2 * noise(p* 3));
	p = p * (1 + 0.1 * noise(p* 6));

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(p, 1.0);

    v1 = cross(vec3(0,1,0),p);
    v2 = cross(v1,p);
    v3 = -v1 + v2;

    v1 = normalize(v1);
    v2 = normalize(v2);
    v3 = normalize(v3);

    p1 = p + v1 * 0.01;
    p1 = p1 * (1 + 0.4 * noise(p1 * 1.5));
	p1 = p1 * (1 + 0.2 * noise(p1* 3));
	p1 = p1 * (1 + 0.1 * noise(p1* 6));


    p2 = p + v2*0.01;
    p2 = p2 * (1 + 0.4 * noise(p2 * 1.5));
	p2 = p2 * (1 + 0.2 * noise(p2 * 3));
	p2 = p2 * (1 + 0.1 * noise(p2 * 6));

    p3 = p + v3 *0.01;
    p3 = p3 * (1 + 0.4 * noise(p3* 1.5));
	p3 = p3 * (1 + 0.2 * noise(p3 * 3));
	p3 = p3 * (1 + 0.1 * noise(p3 * 6));

    s1 = p3-p1;
    s2 = p3-p2;

    n = cross(s2,s1);

    gsTexCoord = teTexCoord[0];

	//n = teNormal[nr]; // This is not the normal you are looking for. Move along!
    gsNormal = mat3(camMatrix * mdlMatrix) * n;
    EmitVertex();
}

void main()
{
	computeVertex(0);
	computeVertex(1);
	computeVertex(2);
}

