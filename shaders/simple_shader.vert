#version 450
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;
layout (location = 0) out vec3 fragColor;
//layout (location = 1) out vec3 fragPosWorld;
//layout (location = 2) out vec3 fragNormalWorld;
//layout (set = 0, binding = 0) uniform GlabalUbo {
//    mat4 projection;
//    mat4 view;
//    mat4 invView;
//    vec4 ambientLightColor;
//} ubo;
layout (push_constant) uniform Push {
    mat4 modelMatrix;//projection * view * model
    mat4 normalMatrix;
} push;

void main() {
    //    gl_Position = vec4(position, 1.0);
    fragColor = color;
    vec4 positionWorld = push.modelMatrix * vec4(position, 1.0);
    gl_Position = positionWorld;
    //    gl_Position = ubo.projection * ubo.view * positionWorld;
    //
    //    fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
    //    fragPosWorld = positionWorld.xyz;
    //    fragColor = color;
}