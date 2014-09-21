#version 330 core

// if you remove this, the triangle renders green.
in vec2 texture_position;
flat in int block_type;

out vec4 frag_color;

void main() {
  if(block_type == 0) {
    frag_color = vec4(1, 0, 0, 1);
  } else if(block_type == 1) {
    frag_color = vec4(0, 1, 0, 1);
  } else if(block_type == 2) {
    frag_color = vec4(0, 0, 1, 1);
  } else {
    // if you remove this, the triangle renders green.
    frag_color = vec4(texture_position, 0, 0);
  }
}
