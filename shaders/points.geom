#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

flat in int type[1];

out vec2 texture_position;
flat out int block_type;

void main() {
  gl_Position = vec4(-1, -1, 0, 1);
  texture_position = vec2(0);
  block_type = type[0];
  EmitVertex();
  gl_Position = vec4(0, 0, 0, 1);
  texture_position = vec2(0);
  block_type = type[0];
  EmitVertex();
  gl_Position = vec4(-1, 0, 0, 1);
  texture_position = vec2(0);
  block_type = type[0];
  EmitVertex();
  EndPrimitive();
}
