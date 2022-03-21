#version 330

in vec4 fPosition;
in vec4 fColor;
in vec4 fLightPosition;
in vec3 fNormal;

// output
out vec4 color;

void main(void) {
  color = vec4(0.9, 0.9, 0.9, 0.5);

  /*color = vec3(1,0,0);*/
}
