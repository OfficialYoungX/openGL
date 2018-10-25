#version 330 core
out vec4 FragColor;
in vec3 Normal; // 法向量
in vec3 FragPos; // 片段位置

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // ambient 环境光
    float ambientStrength = 0.1f; // 环境光系数
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 漫反射
    vec3 norm = normalize(Normal);
    float diffuseStrength = 0.3f; // 亮度系数
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0f); // 系数最小为0，即光源没有影响
    vec3 diffuse = diffuseStrength * lightColor * diff;
    // specular 镜面反射
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}
