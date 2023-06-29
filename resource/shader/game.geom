#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in vec3 gColor;
out vec3 fColor;

void main(){
    fColor = gColor;
    gl_Position = gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, -1.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
