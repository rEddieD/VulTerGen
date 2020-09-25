#version 450 

layout( location = 0 ) in vec3 FragColor;
layout( location = 0 ) out vec4 frag_color; 

void main() { 
  frag_color = vec4( FragColor, 1.0 ); 
}