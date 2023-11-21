Texture2D textures : register(t0);
//Texture2D texture2 : register(t1);
SamplerState colorSampler : register(s0);

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};


cbuffer Opcion : register(b1)
{
    int dianoche;
};

cbuffer Opcion : register(b2)
{
    float timer;
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
};

float4 render(float4 a, float4 b, float t)
{
    //return (a * t) * (b * (1.0 - t));
    return b + t * (a - b);

}

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
    //float4 finalColor;
    //float4 finalColor2;
    //if (timer > 0.5)
    //{
       float4 finalColor = textures.Sample(colorSampler, pix.tex0);
   
    //else if (timer <= 0.5)
    //{
    //    finalColor2 = texture2.Sample(colorSampler, pix.tex0);
    //    return finalColor2;
    //}
        return finalColor;
    
}