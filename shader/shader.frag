#version 330

in vec4 fPosition;
in vec4 fLightPosition;
in vec3 fNormal;

// output
out vec4 color;

void main(void) {
    color = vec4(fNormal, 1.0);
    /*color = vec3(1,0,0);*/
}
