#version 330 core

in vec3 a_position;

void main(void)
{
    gl_Position = vec4(a_position.x, a_position.y, a_position.z, 1.0);
}
