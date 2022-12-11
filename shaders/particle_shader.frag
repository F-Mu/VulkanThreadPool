#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

layout (push_constant) uniform Push {
    vec4 position;
    vec4 color;
} push;

const float M_PI = 3.14159265358;

void main() {
    float dis = sqrt(dot(fragOffset, fragOffset));
    if (dis >= 0.002) {
        discard;
    }
    outColor = vec4(push.color.xyz*push.color.w,push.color.w);
//    outColor=vec4(0.,1.,1.,0.);
}