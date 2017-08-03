#version 330 core

//--- Uniform Data -----------------------------------------------------------//

uniform float tile_repeat; // Number of times to repeat a texture per face
uniform float fuzz_factor; // Fuzzes normals

uniform vec4 material; // { ambient, diffuse, specular, power } eg. { 0.1, 0.4, 0.1, 8 }

uniform vec3 light_position; // eg  { 2, 1, 0 }
uniform vec3 light_color;    // eg. { 1, 1, 1 }
uniform float light_power;   // eg. 0.7
uniform float texture_displacement;

uniform vec3 view_position;
uniform vec3 view_direction;

uniform sampler2D texture;
uniform samplerCube skybox_texture;

// Mode
uniform bool skybox;
uniform bool sun;
uniform bool water;
uniform bool cloud;

// Texture sampling mode
uniform bool sample_1d;
uniform bool sample_2d;
uniform bool sample_blend; // One day :)

// Debugging options
uniform bool debug_solid;
uniform bool debug_uv;
uniform bool debug_normal;
uniform bool debug_height_map;
uniform bool debug_no_light;

//--- Input Data -------------------------------------------------------------//

in float height;
in vec2 uv;
in vec3 frag_position;
in vec3 sphere_normal;
in vec3 vertex_normal;

//--- Output Data ------------------------------------------------------------//

out vec4 color;

//--- Functions --------------------------------------------------------------//

// Borrowed from vertex shader
float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

vec3 hash(vec3 v) {
    return vec3(hash(v.x), hash(v.y), hash(v.z));
}

// Fuzzes a normal vector according to weight
vec3 fuzz(vec3 n, float weight) {
    return normalize(n + normalize(hash(n)) * weight);
}

// Calculate ambient light
vec3 ambient(vec3 sample) {
    return material.x * sample;
}

// Calculate diffuse light
vec3 diffuse(vec3 sample, vec3 normal, vec3 light_dir, vec3 light_position) {

    float cos_vertex = clamp(dot(normal, light_dir), 0, 1);
    float cos_sphere = clamp(dot(sphere_normal, light_position), 0, 1);

    float cos_theta  = cos_vertex * cos_sphere + cos_sphere / 2;

    return material.y * sample * light_color * light_power * cos_theta;
}

// Calculate specular light
vec3 specular(vec3 sample, vec3 normal, vec3 light_dir, vec3 light_position) {

    vec3 view_dir    = normalize(view_position - frag_position);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float cos_sphere = clamp(dot(sphere_normal, light_position), 0, 1);
    float specular   = pow(clamp(dot(view_dir, reflect_dir) * cos_sphere, 0.0, 1.0), material.w);

    return material.z * specular * light_color;
}

//--- Program ----------------------------------------------------------------//

void main() {

    vec3 sample;
    float alpha = 1;

    if(skybox) {
        float bloom = clamp(1 - distance(
            normalize(normalize(frag_position)  - view_position),
            normalize(normalize(view_direction) - view_position)
        ), 0, 1);

        // Drop off
        bloom = pow(bloom, 32);

        bloom = bloom * clamp(1 - distance(normalize(view_direction), normalize(light_position)), 0, 1);

        float cs = clamp(dot(normalize(view_position), light_position), 0, 1);
        float d  = length(view_position - normalize(view_position));
        float k  = (1 / (d * 50 * d)) * cs;
        color    = clamp(texture(skybox_texture, frag_position) + bloom * light_color + k * vec4(0, 0.5, 1, 1), 0, 1);
        return;
    }
    if(cloud) {
        color   = clamp(vec4(height * height), 0, 1);
        color   = (vec4(1) - color) * color;
        color.w = color.w / 0.3;
        sample  = color.xyz;
        alpha   = color.w;
    }
    if(sample_1d) {
        sample = texture(texture, vec2(0, height)).rgb;
    }
    if(sample_2d) {
        float adjusted_dist = texture_displacement / 32;
        sample = texture(texture, fract(uv + vec2(adjusted_dist, adjusted_dist)) * tile_repeat).rgb;
    }
    if(sample_blend) {
        // TODO!
    }

    vec3 normal    = fuzz(vertex_normal, 0.4);
    vec3 light_dir = normalize(light_position - frag_position);
    vec3 light_pos = light_position;

    if(sun) {
        light_dir = normal;
        light_pos = sphere_normal;
    }

    color = clamp(vec4((
        ambient(sample) +
        diffuse(sample, normal, light_dir, light_pos) +
        specular(sample, normal, light_dir, light_pos)
    ), alpha), 0, 1);

    if(water) {
        color = vec4(color.xyz, 0.5);
    }
    if(debug_solid) {
        color = vec4(0, 1, 0, 1);
    }
    if(debug_uv) {
        color = vec4(uv * tile_repeat, 0, 1);
    }
    if(debug_normal) {
        color = vec4((normal + 1) / 2, 1);
    }
    if(debug_height_map) {
        color = vec4(height, height, height, 1);
    }
    if(debug_no_light) {
        color = vec4(sample, alpha);
    }
}