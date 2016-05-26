#version 330

// It was expressed that some driver require this next line to function properly
precision highp float;

in vec4 ex_Color;

void main(void) {
  vec4 in_FragColor = vec4(ex_Color);
}
