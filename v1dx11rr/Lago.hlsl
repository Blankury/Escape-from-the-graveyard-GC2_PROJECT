Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D dispMap : register(t2);

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

cbuffer cbChangerotation : register(b3)
{
    matrix rotationMat;
};


cbuffer cbChangemov : register(b4)
{
    float movimiento;
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
    float4 espacio : TEXCOORD1;
    float movimiento : TEXCOORD2;
};

PS_Input VS_Main(VS_Input vertex)
{  
    PS_Input vsOut = (PS_Input) 0;
    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.espacio = vsOut.pos;
    vsOut.movimiento = movimiento;

    
    vsOut.tex0 = float2(vertex.tex0.x / 2.0 + 0.5, vertex.tex0.y / 2.0 + 0.5) * 8.0;
    


    
    return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
    const float waveStrenght = 0.5;
    float4 LuzAmbiental = float4(0.1, 0.1, 0.1, 1);
    float FA = 0.3;
    
    //coordenadas normalizadas que serán las coords de refraccion
    float2 ndc = (pix.espacio.xy / pix.espacio.w) / 2.0 + 0.5;
    float2 refractTexCoords = float2(ndc.x, -ndc.y);
	////Usamos la variable movimiento que moverá la distorsión en x y y
    float2 distortion = (dispMap.Sample(colorSampler, float2 (pix.tex0.x + pix.movimiento, pix.tex0.y)).rg * 2.0 - 1.0) * waveStrenght;
    float2 distortion2 = (dispMap.Sample(colorSampler, float2(-pix.tex0.x + pix.movimiento, pix.tex0.y + pix.movimiento)).rg * 2.0 - 1.0) * waveStrenght;
	    
    float2 Total = distortion + distortion2;
    //A la refracción le sumamos el total de la distorsión para hacerla una sola coordenada
    refractTexCoords += Total;
    refractTexCoords.x = clamp(refractTexCoords.x, 0.001, 0.999);
    refractTexCoords.y = clamp(refractTexCoords.y, -0.999, -0.001);    
    
    float4 text = colorMap.Sample(colorSampler, refractTexCoords);
   
	///////////////////////////////////////////////
	// aqui se desarrolla el elemento ambiental
	///////////////////////////////////////////////
    float4 AportAmb = LuzAmbiental * FA;
	//////////////////////////////////////////
	//aqui se desarrolla el elemento difuso //
	//////////////////////////////////////////
    float4 textnorm = normalMap.Sample(colorSampler, pix.tex0);

    float3 DirLuz = float3(30, 10, 30);
    float4 LuzDifusa = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float FAD = 1;
	
    float3 bump = normalize(2.0 * textnorm - 1);   
    float3 bumpnrm = normalize(mul(rotationMat, float4(bump, 1.0)));
    float FALL = dot(bump, normalize(DirLuz));
    float4 AportLuzDif = saturate(LuzDifusa * FALL * FAD);
        
    //Salida de color
    text = text * (AportAmb + AportLuzDif);
    text.a = 0.0;
    return text;
    
}
        
