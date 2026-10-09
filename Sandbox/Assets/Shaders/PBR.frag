#version 450

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragUV;
layout(location = 2) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;


layout(std140, binding = 1) uniform Material {
    uint albedoMap;
    uint normalMap;
    uint metallicMap;
    uint roughnessMap;
    uint aoMap;
} material;

layout(binding = 2) uniform sampler2D u_Textures[16];

const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  



void main() {
    vec3 albedo = texture(u_Textures[material.albedoMap], fragUV.xy).xyz;
    vec3 normals = normalize(texture(u_Textures[material.normalMap], fragUV.xy).rgb);
    float metallic = texture(u_Textures[material.metallicMap], fragUV.xy).b;
    float roughness = texture(u_Textures[material.roughnessMap], fragUV.xy).g; 
    float ao = texture(u_Textures[material.aoMap], fragUV.xy).r;

    

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);   
    

    vec3 N = normalize(normals);
    //                   LightPos
    vec3 L = normalize(vec3(0.0, 5.0, 0.0) - fragPosition);
    vec3 V = normalize(vec3(0.0, 0.0, 0.0) - fragPosition);
    vec3 H = normalize(V + L);    
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + specular) * NdotL; 
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    outColor = vec4(color, 1.0);
}
