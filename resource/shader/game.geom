#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

out vec3 fColor;

void main(){
    gl_Position = gl_in[0].gl_Position + vec4(-1.0-gl_in[0].gl_Position.x, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 1.0-gl_in[0].gl_Position.x, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
