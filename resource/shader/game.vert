#version 330 core
layout (location = 0) in float aPos;

out VS_OUT{
    vec3 gColor;
}vs_out;

void main(){
    gl_Position = vec4(aPos, 0.5, 0.0, 1.0);
    vs_out.gColor = vec3(0.5,0.5,0.5);
}