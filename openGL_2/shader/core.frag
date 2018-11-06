#version 330 core
out vec4 FragColor;
in vec3 Normal; // 法向量
in vec3 FragPos; // 片段位置

uniform vec3 lightPos_1;
uniform vec3 lightPos_2;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor_1;
uniform vec3 lightColor_2;

void main()
{
    // ambient 环境光
    float ambientStrength = 0.2f; // 环境光系数
    vec3 ambient_1 = ambientStrength * lightColor_1;
    vec3 ambient_2 = ambientStrength * lightColor_2;
    // diffuse 漫反射
    vec3 norm = normalize(Normal);
    float diffuseStrength = 0.3f; // 亮度系数
    vec3 lightDir_1 = normalize(lightPos_1 - FragPos);
    vec3 lightDir_2 = normalize(lightPos_2 - FragPos);
    float diff_1 = max(dot(norm,lightDir_1),0.0f); // 系数最小为0，即光源没有影响
    float diff_2 = max(dot(norm,lightDir_2),0.0f); // 系数最小为0，即光源没有影响
    vec3 diffuse_1 = diffuseStrength * lightColor_1 * diff_1;
    vec3 diffuse_2 = diffuseStrength * lightColor_2 * diff_2;
    // specular 镜面反射
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir_1 = reflect(-lightDir_1, norm);
    vec3 reflectDir_2 = reflect(-lightDir_2, norm);
    float spec_1 = pow(max(dot(viewDir, reflectDir_1), 0.0), 32);
    float spec_2 = pow(max(dot(viewDir, reflectDir_2), 0.0), 32);
    vec3 specular_1 = specularStrength * spec_1 * lightColor_1;
    vec3 specular_2 = specularStrength * spec_2 * lightColor_2;
    
    
    
    vec3 result = (ambient_1 + diffuse_1 + specular_1 + ambient_2 + diffuse_2 + specular_2) * objectColor;
    FragColor = vec4(result, 1.0f);
}
