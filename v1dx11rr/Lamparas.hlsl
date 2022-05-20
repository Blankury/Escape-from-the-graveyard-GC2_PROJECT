
Texture2D colorMap : register(t0);

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

cbuffer cbparpadea : register(b4)
{
    float parpadea;
};


struct VS_Input
{
    float4 pos : POSITION;
    float2 tex0 : TEXCOORD0;
};


struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float3 campos : TEXCOORD2;
    float parpadea : TEXCOORD3;
};



PS_Input VS_Main(VS_Input vertex)
{
	
    float4 worldPosition;

    PS_Input vsOut = (PS_Input) 0;

    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);

    vsOut.tex0 = vertex.tex0;

    worldPosition = mul(vertex.pos, worldMatrix);

    vsOut.campos = cameraPos.xyz - worldPosition.xyz;

    vsOut.campos = normalize(vsOut.campos);


    return vsOut;
}


float4 PS_Main(PS_Input pix) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;

    textureColor = colorMap.Sample(colorSampler, pix.tex0);
    color = float4(0.2, 0.2, 0.2, 1); // ambient color

    lightDir = -(float3(0.5f, -1.0f, 0.0f)); // lightDirection

    lightIntensity = saturate(lightDir);

   
	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    color += (float4(1.0f, 1.f, 1.f, 1.0f) /*diffuse color*/ * lightIntensity);

	// Saturate the ambient and diffuse color.
    color = saturate(color);

    color = color * textureColor;

    color = saturate(color);
    float4 finalColor = (0.0f, 0.0f, 0.0f, 1.0f);
    return finalColor;
    //return float4(color.x, color.y, color.z, 1.0);
}
        

