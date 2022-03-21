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

cbuffer cbChange : register(b3)
{
    float3 cameraPos;
};

cbuffer cbChangerot : register(b0)
{
    matrix rotationMat;
};
cbuffer cbChangemov : register(b4)
{
    float3 movimiento;
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
    
    float4 espacio : TEXCOORD2;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input) 0;
    vsOut.pos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(vsOut.pos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);

    vsOut.pos = vertex.pos;
    
    ////Guarda el mvp para no cambiar su valor ya que lo usaremos en el pixel shader para el oleaje del agua y movel el especular
    //para no repetir el proceso del valor de pos igualamos espacio = pos
    vsOut.espacio = vsOut.pos;
    
    float tile = 6.0;
    vsOut.tex0 = vertex.tex0 * tile;
    
    return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
    float4 LuzAmbiental = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float FAD = 0.8f;
    float3 DirLuz = float3(500.0f, 500.0f, -1000.0f);
    
   
    //float3 lightDirection = -(float3(0.5f, -1.0f, 0.0f));
    
    const float waveStrenght = 0.5;
	//coordenadas normalizadas que serán las coords de refraccion
    float2 ndc = (pix.espacio.xy / pix.espacio.w) / 2.0 + 0.5;
    float2 refractTexCoords = float2(ndc.x, -ndc.y);
	
	//coordenadas para la distorsion para que parezca que tiene ondas
    float2 distortion, distortion2;
	//Usamos la variable movimiento que moverá la distorsión en x y y
    distortion = (dispMap.Sample(colorSampler, (pix.tex0.x + movimiento.x, pix.tex0.y)).rg * 2.0 - 1.0) * 0.2;
    distortion2 = (dispMap.Sample(colorSampler, (-pix.tex0.x + movimiento.x, pix.tex0.y + movimiento.x)).rg * 2.0 - 1.0) * 0.2;
	//Suma las distorciones

    float2 Total = distortion + distortion2;
	
	//A la refracción le sumamos el total de la distorsión para hacerla una sola coordenada
    refractTexCoords += Total;
    refractTexCoords.x = clamp(refractTexCoords.x, 0.001, 0.999);
    refractTexCoords.y = clamp(refractTexCoords.y, -0.999, -0.001);

	//Color de textura
    float4 textureColor = colorMap.Sample(colorSampler, refractTexCoords);

    //Aporte ambiental
    float4 AportAmb = LuzAmbiental * 0.2;//*FA
    
    //Aporte difuso
    float4 txtnrm = normalMap.Sample(colorSampler, pix.tex0);
    
    //float3 VectorLuz = lightDirection;
    //float4 color = float4(1.0, 1.0, 1.0, 1.0);
    //float4 ColorLuzDifusa = color;

    float3 bump = normalize(2.0 * txtnrm - 1);
    float3 bumpnormal = normalize(mul(rotationMat, float4(bump, 1.0)));
    float FALL = dot(bumpnormal, normalize(DirLuz));
    float diff = saturate(dot(normalize(DirLuz), bumpnormal));
    
    float4 luzdifusafinal = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 AportDif = saturate(luzdifusafinal * FALL * FAD); //FAD
    //float4 luzdifusafinal = ColorLuzDifusa * (1.0f) * diff;


	//SaliDAA DE COLOR
    textureColor.a = 0.5;
    return textureColor * (AportAmb + AportDif);

}