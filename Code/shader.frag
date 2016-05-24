#version 130

// It was expressed that some driver require this next line to function properly
precision highp float;

in vec4 ex_Color;

void main(void) {
  gl_FragColor = vec4(ex_Color);
}
