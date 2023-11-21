Texture2D colorMap : register(t0);
Texture2D specMap : register(t1);

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

cbuffer cbOpcion : register(b6)
{
    float timer;
};

////COORDENADAS DE LAS POINT LIGHS
//cbuffer cbPosPointlight : register(b7)
//{
//    float3 CoordPl;
//};

////EL COLOR QUE LLEVARA
//cbuffer cbcolorPointlight : register(b8)
//{
//    float3 ColorPl;
//};

////RANGO DE ILUMINACION
//cbuffer cbRangePointlight : register(b9)
//{
//    float Range;
//};

////SABER SI EL OBJETO LLEVARA UNA LUZ POINT LIGHT O NO
//cbuffer cbEnablePointLight : register(b5)
//{
//    bool Enable;
//};



struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;	
	float3 normal : NORMAL0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;	
	float3 normal : TEXCOORD1;
	float3 campos : TEXCOORD2;
	float specForce : TEXCOORD3;
    //float3 pointLight : TEXCOORD4;
    //float range : TEXCOORD5;
	
		//Niebla
    float fogFactor : FOG;
};

PS_Input VS_Main(VS_Input vertex)
{
	
	float4 worldPosition;
	
    float4 vertexPos;

		PS_Input vsOut = (PS_Input)0;

		vsOut.pos = mul(vertex.pos, worldMatrix);
		vsOut.pos = mul(vsOut.pos, viewMatrix);
		vsOut.pos = mul(vsOut.pos, projMatrix);
	
		vertexPos = vsOut.pos;

		vsOut.tex0 = vertex.tex0;
		vsOut.normal = normalize(mul(vertex.normal, worldMatrix));

		worldPosition = mul(vertex.pos, worldMatrix);

		vsOut.campos = cameraPos.xyz - worldPosition.xyz;

		vsOut.campos = normalize(vsOut.campos);

		vsOut.specForce = specForce;
	
    float fogStart = -10.0f;
    float fogEnd = 50.0f;
	
    float fogFactor = saturate((fogEnd - vertexPos.z) / (fogEnd - fogStart));
    vsOut.fogFactor = fogFactor;
	
    //vsOut.pointLight = CoordPl.xyz - worldPosition.xyz;
	
    //vsOut.pointLight = normalize(vsOut.pointLight);
	
    //vsOut.range = Range;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
    float3 daycolor;
	float3 reflection;
	float4 specular;
	float4 specularMap;
	float4 finalSpec;
    float3 dayspecular;
	
	//Parametros si es verdadero el point light
    float3 toLight;
    float distance;
    float3 lightDirection;
    float attenuation;
    float3 pointLightContribution;
    float diffuseFactor;
    float3 diffuseColor;
	
	
        textureColor = colorMap.Sample(colorSampler, pix.tex0);
		 daycolor = lerp(float3(0.1f, 0.1f, 0.1f), (0.5f, 0.5f, 0.5f), timer);
		color = float4(daycolor, 1.10); // ambient color

		dayspecular = lerp(float3(0.1f, 0.1f, 0.1f), (0.5f, 0.5f, 0.5f), timer);	
		specular = float4(dayspecular, 1.0); //specular color
        specularMap = specMap.Sample(colorSampler, pix.tex0);
	
	lightDir = -(float3(0.5f, -1.0f, 0.0f)); // lightDirection

	lightIntensity = saturate(dot(pix.normal, lightDir));

	if (lightIntensity > 0) {
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color += (float4(0.0f, 0.0f, 0.2f, 1.0f)/*diffuse color*/ * lightIntensity);

		// Saturate the ambient and diffuse color.
		color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection = normalize(2 * lightIntensity * pix.normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(dot(reflection, pix.campos)), pix.specForce);
		finalSpec = specular * specularMap;
	}
	
    //if (Enable == true)
    //{
    //     toLight = CoordPl - pix.pos.xyz;
    //     distance = length(toLight);
    //     lightDirection = normalize(toLight);

    // //Calcular la atenuación basada en la distancia
    //     attenuation = saturate(1.0 - distance / Range);

    // //Calcular la contribución de la luz puntual
    //     pointLightContribution = ColorPl * attenuation;

    // //Calcular la componente difusa de la luz puntual
    //     diffuseFactor = saturate(dot(pix.normal, lightDirection));
    //    diffuseColor = pointLightContribution /** color.rgb*/ * diffuseFactor;
    //}
    //else
    //{
    //    diffuseColor = daycolor;
    //}
	

    color = color * textureColor;
    color = saturate(color + finalSpec);
	
    float4 fogColor = (0.3f, 0.3f, 0.3f, 0.5f);
	
    color += pix.fogFactor * fogColor + (1.0f - pix.fogFactor) * fogColor;

    return color;
}