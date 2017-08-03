#version 330 core

//--- Vertex Data ------------------------------------------------------------//

layout(location = 0) in vec3 vertex_position; // cube coordinates
layout(location = 1) in vec2 vertex_uv;       // texture mapping

//--- Uniform Data -----------------------------------------------------------//

// Mode
uniform bool skybox;
uniform bool water;
uniform bool cloud;

// Octables            // Reasonable values
uniform vec2 octave_1; // { 1.0,  2.0  }
uniform vec2 octave_2; // { 0.5,  8.0  }
uniform vec2 octave_3; // { 0.25, 32.0 }

// Displacement magnitude
uniform float magnitude; // 0.2
uniform float texture_displacement;

// Matricies
uniform mat4 proj_view;
uniform mat4 model;

// Debugging
uniform bool debug_cube;
uniform bool debug_sphere;
uniform bool debug_sphere_normal;

//-- Output Data -------------------------------------------------------------//

out float height;
out vec2 uv;
out vec3 frag_position;
out vec3 sphere_normal;
out vec3 vertex_normal;

//--- Functions --------------------------------------------------------------//

// Simple fast shader hash generator (uniform dist.)
float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}
// overloaded for hashed vector
vec3 hash(vec3 v) {
    return vec3(hash(v.x), hash(v.y), hash(v.z));
}

// I experimented with a number of different simplex noise algorithms for the
// hpu. This proved to be the most robust.
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83

vec4 permute(vec4 x)       { return mod(((x * 34.0) + 1.0) * x, 289.0);      }
vec4 taylor_inv_sqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

// AKA prepare to swivel
float simplex_noise(vec3 v) {

    if(water) {
        v = v + texture_displacement;
    }
    if(cloud) {
        v = v + texture_displacement / 1.75;
    }

    const vec2  C = vec2(1.0 / 6.0, 1.0 / 3.0);
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;

    // Other corners
    vec3 g  = step(x0.yzx, x0.xyz);
    vec3 l  = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy);
    vec3 i2 = max(g.xyz, l.zxy);

    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;

    // Permutations
    i = mod(i, 289.0 );
    vec4 p = permute(permute(permute(
            i.z + vec4(0.0, i1.z, i2.z, 1.0 )) +
            i.y + vec4(0.0, i1.y, i2.y, 1.0 )) +
            i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

    // Gradients
    // ( N * N points uniformly over a square, mapped onto an octahedron.)
    float n_ = 1.0 / 7.0; // N=7
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z * ns.z); //  mod(p, N * N)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_); // mod(j,N)

    vec4 x = x_ * ns.x + ns.yyyy;
    vec4 y = y_ * ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);

    vec4 s0 =  floor(b0) * 2.0 + 1.0;
    vec4 s1 =  floor(b1) * 2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww ;

    vec3 p0 = vec3(a0.xy, h.x);
    vec3 p1 = vec3(a0.zw, h.y);
    vec3 p2 = vec3(a1.xy, h.z);
    vec3 p3 = vec3(a1.zw, h.w);

    //Normalise gradients
    vec4 norm = taylor_inv_sqrt(vec4(
        dot(p0, p0),
        dot(p1, p1),
        dot(p2, p2),
        dot(p3, p3)
    ));

    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(
        dot(x0,x0),
        dot(x1,x1),
        dot(x2,x2),
        dot(x3,x3)
    ), 0.0);

    m = m * m;
    return 42.0 * dot(m * m, vec4(
        dot(p0,x0),
        dot(p1,x1),
        dot(p2,x2),
        dot(p3,x3)
    ));
}

// Maps cubic coordinates to spgere
vec3 sphereize(vec3 v) {
    float x2 = v.x * v.x, y2 = v.y * v.y, z2 = v.z * v.z;
    return vec3(
        v.x * sqrt(1.0 - (y2 / 2.0) - (z2 / 2.0) + (y2 * z2 / 3.0)),
        v.y * sqrt(1.0 - (z2 / 2.0) - (x2 / 2.0) + (z2 * x2 / 3.0)),
        v.z * sqrt(1.0 - (x2 / 2.0) - (y2 / 2.0) + (x2 * y2 / 3.0))
    );
}

// Get the height displacement
float displace(vec3 v) {
    float height =
        octave_1.x * simplex_noise(v * octave_1.y) +
        octave_2.x * simplex_noise(v * octave_2.y) +
        octave_3.x * simplex_noise(v * octave_3.y);
    // Normalise and clamp
    height = height / (octave_1.x + octave_2.x + octave_3.x);
    height = clamp((height + 1) / 2, 0, 1);
    return height * magnitude;
}

// Adjust vector by height displacement
vec3 position(vec3 v) {
    return v + displace(v) * normalize(v);
}

//--- Program ----------------------------------------------------------------//

void main() {

    if(skybox) {
        frag_position = vertex_position;
        gl_Position   = proj_view * model * vec4(vertex_position, 1.0);
        return;
    }

    sphere_normal = sphereize(vertex_position);

    // compute manually so that we can use later when estimating the gradient
    float displacement = displace(sphere_normal);
    height = displacement / magnitude;
    // computing the normalization here might seem like a wase, but its needed
    // for properly shading a cube in debug mode
    frag_position = sphere_normal + displacement * normalize(sphere_normal);
    gl_Position   = proj_view * model * vec4(frag_position, 1);
    uv            = vertex_uv;

    // Took forever to find a gradient sampling algorithm that actually worked.
    // Most had a bias, for example axis bias, where along the x,y,z planes
    // the normals would diverge or converge causing severe artiphacts. This
    // algorithm also only needs three additional samples.
    // http://mrl.nyu.edu/~perlin/homepage2006/bumpy/Sphere.java

    const float epsilon = 0.0001;

    vertex_normal = sphere_normal;

    // We don't actually (and shouldn't!) have to sphereize the samples! I
    // tested with that before and it creates worse results, because of fp
    // issues

    float sx = displace(vec3(sphere_normal.x + epsilon, sphere_normal.yz).xyz);
    float sy = displace(vec3(sphere_normal.y + epsilon, sphere_normal.xz).yxz);
    float sz = displace(vec3(sphere_normal.z + epsilon, sphere_normal.xy).yzx);

    vertex_normal.x = vertex_normal.x - (sx - displacement) / epsilon;
    vertex_normal.y = vertex_normal.y - (sy - displacement) / epsilon;
    vertex_normal.z = vertex_normal.z - (sz - displacement) / epsilon;

    vertex_normal = normalize(vertex_normal);

    // Debug options

    if(debug_cube) {
        gl_Position = proj_view * model * vec4(vertex_position, 1);
    }
    if(debug_sphere) {
        gl_Position = proj_view * model * vec4(sphere_normal, 1);
    }
    if(debug_sphere_normal) {
        vertex_normal = sphere_normal;
    }
}