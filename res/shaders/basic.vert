#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std330, binding = 0) buffer instanceModels {
    mat4 models[];
};

layout (std330, binding = 1) buffer instanceColors {
    vec4 colors[];
};

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 Color;

uniform mat4 projection;
uniform mat4 view;
uniform int id = 0;

void main() {
    int index;

    if (id < 7) {
        index = id + gl_InstanceID;
    } else {
        index = id + 2 * gl_InstanceID;
    }

    FragPos = vec3(models[index] * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(models[index]))) * aNormal;
    TexCoord = aTexCoord;
    Color = colors[index];

    gl_Position = projection * view * models[index] * vec4(aPos, 1.0f);
}
