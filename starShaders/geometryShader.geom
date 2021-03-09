#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in float brightnessVG[];
out float brightnessGF;
out vec2 fragPos;

uniform float screenRatio[];
uniform float zoom[];

float m = brightnessVG[0]/150;
highp float n = 1;

void main()
{
    if (brightnessVG[0] < 0.2){
        m *= zoom[0];
    }
    if (m<0.003){ return; };

    gl_Position = gl_in[0].gl_Position+vec4(m, 0, 0, 0);
    fragPos = vec2(n, -n);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(m, m*screenRatio[0], 0, 0);
    fragPos = vec2(n, n);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(0.0, 0.0, 0, 0);
    fragPos = vec2(-n, -n);
    EmitVertex();
    EndPrimitive();
    gl_Position = gl_in[0].gl_Position+vec4(m, m*screenRatio[0], 0, 0);
    fragPos = vec2(n, n);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(0.0, m*screenRatio[0], 0, 0);
    fragPos = vec2(-n, n);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position+vec4(0.0, 0.0, 0, 0);
    fragPos = vec2(-n, -n);
    EmitVertex();
    brightnessGF = brightnessVG[0];
    EndPrimitive();
}