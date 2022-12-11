#version 450
const float offset = 0.002;
const vec2 OFFSETS[6] = {
vec2(-offset, -offset),
vec2(-offset, offset),
vec2(offset, -offset),
vec2(offset, -offset),
vec2(-offset, offset),
vec2(offset, offset)
};

layout (location = 0) out vec2 fragOffset;

layout (push_constant) uniform Push {
    vec4 position;
    vec4 color;
} push;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 positionWorld = push.position.xyz + vec3(fragOffset, 0);
    gl_Position = vec4(positionWorld, push.position.w);
}