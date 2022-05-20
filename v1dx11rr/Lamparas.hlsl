
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
    
    vsOut.parpadea = parpadea;
    
    worldPosition = mul(vertex.pos, worldMatrix);

    vsOut.campos = cameraPos.xyz - worldPosition.xyz;

    vsOut.campos = normalize(vsOut.campos);


    return vsOut;
}


float4 PS_Main(PS_Input pix) : SV_TARGET
{
    float4 textureColor;
    float4 color;

    textureColor = colorMap.Sample(colorSampler, pix.tex0);

    color = color * textureColor;

    color = saturate(color);
    
    //float4 finalColor = (pix.parpadea, pix.parpadea, pix.parpadea, 1.0f);
    float4 finalColor = (0.0, 0.0, 0.0, pix.parpadea);
    return finalColor;
    
}
        

