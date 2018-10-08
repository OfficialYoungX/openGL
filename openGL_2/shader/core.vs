//#version 330 core
//layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 texcoords_1;
//layout(location = 2) in vec2 texcoords_2;
//out vec2 TexCoords1;
//out vec2 TexCoords2;
//void main()
//{
//    gl_Position = vec4(position.x, position.y, position.z, 1.0f);
//    TexCoords1 = vec2(texcoords_1.x, 1.0-texcoords_1.y);
//    TexCoords2 = vec2(texcoords_2.x, 1.0-texcoords_2.y);
//}


//#version 330 core
//layout (location = 0) in vec3 aPos;
//
//void main()
//{
//    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
