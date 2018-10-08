//#version 330 core
//in vec2 TexCoords1;
//in vec2 TexCoords2;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;
//out vec4 color_1;
//out vec4 color_2;
//void main()
//{
//    color_1 = mix(texture(ourTexture1, TexCoords1), texture(ourTexture2, TexCoords2), 0.8);
////    color_1 = vec4(texture(ourTexture1, TexCoords1).rgb, 1.0f);
//    color_2 = mix(texture(ourTexture1, TexCoords1), texture(ourTexture2, TexCoords2), 0.2);
////    color_2 = vec4(texture(ourTexture1, TexCoords1).rgb, 1.0f);
//}

//#version 330 core
//out vec4 FragColor;
//
//void main()
//{
//    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//}

#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alpha = 0.5;
void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), alpha);
}
