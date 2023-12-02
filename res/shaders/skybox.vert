#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vec4 pos = projection * view * vec4(aPos, 1.0);

    // Having z equal w will always make the fragment pass the depth test
    gl_Position = vec4(pos.xy, pos.w, pos.w);
    // Flip the z axis to make the fragment pass the depth test
    TexCoords = vec3(aPos.x, aPos.y, -aPos.z);
}
