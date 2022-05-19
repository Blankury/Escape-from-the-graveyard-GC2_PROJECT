Texture2D colorMap : register(t0);
Texture2D colorMap2 : register(t1);
Texture2D blendMap : register(t2);
Texture2D nrmMap1 : register(t3);
Texture2D nrmMap2 : register(t4);

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

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangente : NORMAL1;
	float3 binormal : NORMAL2;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangent : NORMAL1;
	float3 binorm : NORMAL2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	vsOut.blendTex = vertex.blendTex;
	
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangent = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.binorm = normalize(mul(vertex.binormal, worldMatrix));

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	//Lo que será el color final
	float4 fColor = float4(1,0,0,1);
	//Luz ambiental
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//Dirección de la luz
    float3 DiffuseDirection = float3(500.0f, 500.0f, -1000.0f);
	//Color de la luz difusa
    float4 DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//Colores
	float4 text = colorMap.Sample(colorSampler, pix.tex0);
	float4 text2 = colorMap2.Sample(colorSampler, pix.tex0);
	//Normales
    float4 nrm1 = nrmMap1.Sample(colorSampler, pix.tex0);
    float4 nrm2 = nrmMap2.Sample(colorSampler, pix.tex0);
	//BlendMap
	float4 alphaBlend = blendMap.Sample(colorSampler, pix.blendTex);
	
	//Texturas combinadas
    float4 textf = lerp(text, text2, alphaBlend.r);
	//Normales combinadas
    float4 nrmf = lerp(nrm1, nrm2, alphaBlend.r);

	//Para poner el bump de las normales
    float3 bump = 2.0 * nrmf - 1.0; //bump
    float3x3 TBN = { { pix.tangent }, { pix.binorm }, { pix.normal } };
    float3 finalNorm = mul(TBN, bump);
    float FALL = dot(normalize(finalNorm), normalize(DiffuseDirection));

	//Aportación difusa tomando el color de la luz difusa y el resultado del bump de las normalesw
    float4 aportdif = saturate(DiffuseColor * FALL);
	
    fColor = textf * (ambient + aportdif);
	
    //float3 diffuse = dot(-DiffuseDirection, pix.normal);
    //diffuse = saturate(diffuse * DiffuseColor.rgb);
    //diffuse = saturate(diffuse + ambient);

    //fColor = float4(textf.rgb * diffuse, 1.0f);

	return fColor;
}