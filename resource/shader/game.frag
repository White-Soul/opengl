#version 330 core

in vec3 fColor;

void main(){
    FragColor = vec4(fColor, 1.0);
}