#version 330

// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in vec3 in_Position;
in vec4 in_Color;

// Output the ex_Color variable to the next shader in the chain
out vec4 color;

void main(void) {
  // Since there's flat lines, the input only had two points: x and y.
  // Set the Z coordinate to 0 and W coordinate to 1
  gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0f);

  // Pass the color on to the fragment shader
  color = in_Color;
}
