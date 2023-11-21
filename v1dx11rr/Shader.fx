Texture2D colorMap : register(t0);
Texture2D colorMap2 : register(t1);
Texture2D blendMap : register(t2);
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
//Esta ya no se usa de momento
cbuffer cbDianoche : register(b4)
{
    int opcion;
};
//Intervalo de 0 a 1 para alternar la luz del terreno
cbuffer cbefectodianoche : register(b5)
{
    float tiempo;
};

//COORDENADAS DE LAS POINT LIGHTS
cbuffer cbPosPointlight : register(b6)
{
    float3 CoordPl;
};

//EL COLOR QUE LLEVARA
cbuffer cbcolorPointlight : register(b7)
{
    float3 ColorPl;
};

//RANGO DE ILUMINACION
cbuffer cbRangePointlight : register(b8)
{
    float Range;
};

//SABER SI EL OBJETO LLEVARA UNA LUZ POINT LIGHT O NO
cbuffer cbEnablePointLight : register(b9)
{
    bool Enable;
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
	
	//Niebla
    float fogFactor : FOG;
};

PS_Input VS_Main(VS_Input vertex)
{
	
    float4 vertexPos;
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);
	
	
    vertexPos = vsOut.pos;

	vsOut.tex0 = vertex.tex0;
	vsOut.blendTex = vertex.blendTex;
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangent = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.binorm = normalize(mul(vertex.binormal, worldMatrix));
	
    float fogStart = -10.0f;
    float fogEnd = 60.0f;
	
    float fogFactor = saturate((fogEnd - vertexPos.z) / (fogEnd - fogStart));
    vsOut.fogFactor = fogFactor;
	
	
    float3 toLight;
    float distance;
    float3 lightDirection;
    float attenuation;
    float3 pointLightContribution;
	


	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 fColor = float4(1,0,0,1);

    float3 ambient ;
    float3 DiffuseDirection ;
    float4 DiffuseColor;
	
    //float3 toLight;
    //float distance;
    //float3 lightDirection;
    float attenuation;
    float3 pointLightContribution;
    float diffuseFactor;
    float3 diffuseColor;

        ambient = lerp(float3(0.1f, 0.1f, 0.01f), (0.5f, 0.5f, 0.5f), tiempo);
         DiffuseDirection = float3(0.5f, -1.0f, 0.0f);
	
		 DiffuseColor = float4(ambient, 1.0f);
	

	float4 text = colorMap.Sample(colorSampler, pix.tex0);
	float4 text2 = colorMap2.Sample(colorSampler, pix.tex0);
	float4 alphaBlend = blendMap.Sample(colorSampler, pix.blendTex);
	float4 textf = (text * alphaBlend) + ((1.0 - alphaBlend) * text2);

	

	float3 diffuse = dot(-DiffuseDirection, pix.normal);
	diffuse = saturate(diffuse*DiffuseColor.rgb);
	diffuse = saturate(diffuse + ambient);
	
    float3 colorPl = ColorPl;
	
    float4 pixelPl = float4(CoordPl,1.0) - pix.pos;
	
    float distancia = length(pixelPl);
	
    attenuation = saturate(1.0 - distancia / Range);
    pixelPl /= distancia;
	
    float3 pixel2 = pixelPl;
	
    float iluminacion = dot(pixel2, pix.normal);
	
    if (iluminacion > 0.0f)
    {
        fColor += iluminacion * float4(diffuse,1.0f) * attenuation;

    }

        fColor = float4(textf.rgb * diffuse, 1.0f);
   

    float4 fogColor =  (0.3f, 0.3f, 0.3f,0.5f);
	
    fColor += pix.fogFactor * fogColor + (1.0f - pix.fogFactor) * fogColor;

    return fColor;
}

    //
    //float RangePl : PointLight;
    //float3 lightDirpl : PointLight1;
    //float3 PosPL : PointLight2;



 ////   //if (Enable == true)
 ////   //{
 ////       toLight = CoordPl - pix.pos.xyz;
 ////       distance = length(toLight);
 ////       lightDirection = normalize(toLight);

 ////    //Calcular la atenuación basada en la distancia
 ////       attenuation = saturate(1.0 - distance / Range);

 ////    //Calcular la contribución de la luz puntual
 //   pointLightContribution = ColorPl * pix.RangePl;
	
 //   pix.lightDirpl = normalize(pix.lightDirpl);

 //    //Calcular la componente difusa de la luz puntual
 //   diffuseFactor = saturate(dot(pix.normal, pix.lightDirpl));
 //   diffuseColor = pointLightContribution * DiffuseColor.rgb * diffuseFactor;
 ////   //}
 ////   //else
 ////   //{
 ////   //    diffuseColor = ambient;
 ////   //}
 //   fColor.rgb += diffuseColor;

	
 //   toLight = CoordPl - vertexPos.z;
 //   distance = length(toLight);
	//vsOut.lightDirpl = lightDirection = normalize(toLight);
 //   attenuation = saturate(1.0 - distance / Range);
	
 //   vsOut.PosPL = CoordPl - vertexPos.z;
	
 //   vsOut.RangePl = attenuation;