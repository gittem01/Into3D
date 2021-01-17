#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoords[];
in vec3 theNormal[];
in vec3 fragPos[];

out vec2 texCoordsF;
out vec3 theNormalF;
out vec3 fragPosF;

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        texCoordsF = texCoords[i];
        theNormalF = theNormal[i];
        fragPosF = fragPos[i];
        EmitVertex();
    }
    
    EndPrimitive();
}