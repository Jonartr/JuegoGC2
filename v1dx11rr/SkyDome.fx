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
    	//Niebla
    float fogFactor : FOG;
};

float4 render(float4 a, float4 b, float t)
{
    //return (a * t) * (b * (1.0 - t));
    return b + t * (a - b);

}

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
    float4 vertexPos;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);
    vertexPos = vsOut.pos;
    
    
	vsOut.tex0 = vertex.tex0;
    
    float fogStart = -10.0f;
    float fogEnd = 50.0f;
	
    float fogFactor = saturate((fogEnd - vertexPos.z) / (fogEnd - fogStart));
    vsOut.fogFactor = fogFactor;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
    //float4 finalColor;
    //float4 finalColor2;
    //if (timer > 0.5)
    //{
       float4 finalColor = textures.Sample(colorSampler, pix.tex0);
    
    

    float4 fogColor = (0.3f, 0.3f, 0.3f, 0.5f);
	
   finalColor += pix.fogFactor * fogColor + (1.0f - pix.fogFactor) * fogColor;
   
    //else if (timer <= 0.5)
    //{
    //    finalColor2 = texture2.Sample(colorSampler, pix.tex0);
    //    return finalColor2;
    //}
        return finalColor;
    
}