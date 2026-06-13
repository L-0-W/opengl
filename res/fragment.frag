#version 330 core
#define NR_POINT_LIGHTS 3

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 lightColor;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutOff;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    float shininess;
};

struct FlashLight {
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // vec3 norm = normalize(Normal);
    // vec3 lightDir = vec3(0.0f, 0.0f, 0.0f);

    // if (light.source.w == 0.0) // IS DIRECTIONAL
    // {
    //     lightDir = normalize(-light.source.xyz);
    // } else if (light.source.w == 1.0)
    // {
    //     lightDir = normalize(light.source.xyz - FragPos);
    // }

    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);

    // float diff = max(dot(norm, lightDir), 0.0);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // vec3 emission = vec3(0.5f) * texture(material.emission, TexCoords).rgb;

    // if (light.source.w == 1.0)
    // {
    //     float distance = length(light.source.xyz - FragPos);
    //     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //     ambient *= attenuation;
    //     diffuse *= attenuation;
    //     specular *= attenuation;
    // }

    // float theta = dot(lightDir, normalize(-flashlight.direction));
    // float epsilon = flashlight.cutoff - flashlight.outerCutOff;
    // float intensity = clamp((theta - flashlight.outerCutOff) / epsilon, 0.0, 1.0);

    // diffuse *= intensity * (2.0 * texture(material.emission, TexCoords).rgb);
    // specular *= intensity;

    // vec3 result = ambient + diffuse + specular;

    // FragColor = vec4(result, 1.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.lightColor * light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
