Texture2D noche1 : register(t0);
Texture2D noche2 : register(t1);

SamplerState colorSampler : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};

cbuffer cbdegra : register(b1)
{
    float degradado;
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
    float degradado : TEXCOORD1;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

    vsOut.degradado = degradado;
	
    vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{

    float4 text1 = noche1.Sample(colorSampler, pix.tex0);
    float4 text2 = noche2.Sample(colorSampler, pix.tex0);
	

    float4 finalColor = (0.0f, 0.0f, 0.0f, 1.0f);
    finalColor = lerp(text1, text2, pix.degradado);


	return finalColor;
}