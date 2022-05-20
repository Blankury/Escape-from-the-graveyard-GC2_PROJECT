Texture2D colorMap : register(t0);
Texture2D specMap : register(t1);
Texture2D bumpMap : register(t2);

SamplerState colorSampler : register(s0);

cbuffer cbChangerEveryFrame : register(b0)
{
	matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
	matrix projMatrix;
};

cbuffer cbChangesOccasionally : register(b3)
{
	float3 cameraPos;
};

cbuffer cbChangesOccasionally : register(b4)
{
	float specForce;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;	
	float3 normal : NORMAL0;
	
    
    float3 tangente : NORMAL1;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;	
    float3 normal : TEXCOORD1;
	float3 campos : TEXCOORD2;
	float specForce : TEXCOORD3;
	
    float3 tangent : TEXCOORD4;
    float3 binorm : TEXCOORD5;
	
};

PS_Input VS_Main(VS_Input vertex)
{
	
    float4 worldPosition;

    PS_Input vsOut = (PS_Input) 0;
    
    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);

    vsOut.tex0 = vertex.tex0;
    vsOut.specForce = specForce;
  
    vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
    
    // Tangente
    float3 tangent;
    float3 c1 = cross(vsOut.normal, float3(0.0, 0.0, 1.0));
    float3 c2 = cross(vsOut.normal, float3(0.0, 1.0, 0.0));
    
    if (length(c1) > length(c2))
    {
        tangent = c1;
    }
    else
    {
        tangent = c2;
    }
    
    vsOut.tangent = normalize(tangent);
    
    // Binormal
    float3 binormal;
    binormal = cross(vsOut.normal, vsOut.tangent);
    //vsOut.binormal = normalize(binormal) * -1; //bump hacia adentro
    vsOut.binorm = normalize(binormal); //bump hacia afuera
    
    
    worldPosition = mul(vertex.pos, worldMatrix);
    
    vsOut.campos = cameraPos.xyz - worldPosition.xyz;
    vsOut.campos = normalize(vsOut.campos);

    return vsOut;
}
float4 PS_Main(PS_Input pix) : SV_TARGET
{
    float3 reflection;
    float4 finalSpec;
    float4 aportdif;
    float diffuseTerm;
	
	//Color del modelo
    float4 textureColor = colorMap.Sample(colorSampler, pix.tex0);
    float4 color = float4(0.2, 0.2, 0.2, 1);
	//Ambiente
    float3 ambientColor = float3(0.0f, 0.0f, 0.2f);
    float3 diffuseColor = float3(1.0f, 1.0f, 1.0f);
	//Color especular
    float4 specular = float4(0.0, 0.0, 0.0, 1.0);
	//Mapa de especular
    float4 specularMap = specMap.Sample(colorSampler, pix.tex0);

	//Mapa de bump
    float4 BumpMap = bumpMap.Sample(colorSampler, pix.tex0);
    BumpMap = (BumpMap * 2.0f) - 1.0f;
    float3 bumpNormal = (BumpMap.x * pix.tangent) + (BumpMap.y * pix.binorm) + (BumpMap.z * pix.normal);
    bumpNormal = normalize(bumpNormal);
	//DIRECCIÓN de la luz
    float3 lightDir = -(float3(0.5f, -1.0f, 0.0f));
	
	//Saca la intensidad de la luz usando la dirección y las normales
    float lightIntensity = saturate(dot(pix.normal, lightDir));
	
    // Get parameters
    float3 normal = normalize(pix.normal);
    float3 cameraVec = normalize(pix.campos);

    if (lightIntensity > 0)
    {
		// Determina la luz DIFUSA basada en el color difuso y la intensida de la luz.
        color += (float4(1.0f, 1.f, 1.f, 1.0f) /*color difuso  otra op (0.8f, 0.3f, 0.0f);*/ * lightIntensity);
		
		// Saturate the ambient and diffuse color.
        color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * pix.normal - lightDir);

		// Determina la luz especular basada en reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, pix.campos)), pix.specForce);
        finalSpec = specular * specularMap;
		
    }
    // Calculate diffuse lighting USING the BumpMap
    diffuseTerm = saturate(dot(bumpNormal, lightDir));
    // Calculate specular lighting
    float3 R = normalize(lightDir + cameraVec);
    float specularTerm = pow(saturate(dot(normal, R)), pix.specForce);
    if (diffuseTerm > 0.0f)
    {
        R = normalize(lightDir + (2 * diffuseTerm * bumpNormal));
        specularTerm = pow(saturate(dot(cameraVec, R)), pix.specForce);
        specularMap = specularTerm * specularMap;
    }
    float3 finalColor = saturate(ambientColor + diffuseColor * diffuseTerm);
    
    return textureColor * float4(finalColor, 1.0f) + finalSpec;
    
    //return color;
}