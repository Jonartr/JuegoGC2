#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"
#include <random>

#include "GUI.h"
#include "Text.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-200.0f, 200.0f);
float posrand[100];

float tiempo=0.001 ;

class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;
	ID3D11Query* timer;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;

	TerrenoRR *terreno;
	SkyDome *skydome;
	BillboardRR *billboard;
	BillboardRR* arbol;
	BillboardRR* Fondo;
	Camara *camara;
	ModeloRR* model;
	ModeloRR* edificio;
	ModeloRR* edificio2;
	ModeloRR* bicicleta;
	ModeloRR* basura;
	ModeloRR* mesa;
	ModeloRR* cruz;
	ModeloRR* enemigo;
	ModeloRR* fogata;

	GUI* vida;
	Text* texto;
	Text* texto_x;
	Text* texto_z;
	float izqder;
	float arriaba;
	float rotacioncamera;
	float vel, vel2;
	float rotacioncruz;
	bool subir_bici = false, sobre_bici = false, mostrar_mensaje = false,entra=false;
	bool breakpoint;
	bool is_Walking = false, on_Music = false;
	int cruces_recogidas = 0;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	vector2 uvfija;



	XACTINDEX cueIndex;
	XACTINDEX walking;
	CXACT3Util m_XACT3;



	void seguir_jugador(float* jugadorPos, float* enemigoPos, ModeloRR * enemyPos) {
			float posicionInicial = enemigoPos[0];
			float posicionInicial2 = enemigoPos[1];
			float newPosx = 0, newPosz = 0;

			// Actualizamos la coordenada del enemigoPos
			newPosx = posicionInicial + (jugadorPos[0] - posicionInicial) * tiempo;
			newPosz = posicionInicial2 + (jugadorPos[1] - posicionInicial2) * tiempo;

			// Actualizamos la posición del enemigoPos
			enemyPos->setPosX(newPosx);
			enemyPos->setPosZ(newPosz);

			// Incrementamos el tiempo
			tiempo += 0.000001;
		
		}




	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		rotacioncruz = 0;
		vel2 = 0;
		rotacioncamera = 0;
		billCargaFuego();
		uvfija.u = 255.0;
		uvfija.v = 255.0;
		for (int i = 0; i <= 99; i++) {

			posrand[i] = dis(gen);
		}
		camara = new Camara(D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,1,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(512, 512, d3dDevice, d3dContext);
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"cielito.jpg");
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		model = new ModeloRR(d3dDevice, d3dContext, "MODELOS/ArbolA/ArbolA.obj", L"MODELOS/Grupo_arboles/tronco2.jpg", L"MODELOS/Grupo_arboles/Tronco2N (3).png", 0, 0);
		arbol = new BillboardRR(L"arbol.png", L"NormalMap.png", d3dDevice, d3dContext, 5);
		Fondo = new BillboardRR(L"Assets/Materiales/bosquecito.jpg", L"NormalMap.png", d3dDevice, d3dContext, 100);

		//========================MODELOS ========================================
		//COLOCAR COORDENADAS, PORQUE AL CARGAR POR PRIMERA VEZ LA COLISION SERA VERDADERA
		//Y AL QUERER CARGAR LA POSICION ANTERIOR COMO ESTA ES NULA MARCARA ERROR
		edificio = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Casitas/cottage_obj.obj", L"MODELOS/Casitas/cottage_diffuse.png", L"MODELOS/Casitas/cottage_normal.png", 0, 0);
		edificio2 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Casitas/woodenhouse.obj", L"MODELOS/Grupo_arboles/tronco2.jpg", L"MODELOS/Grupo_arboles/Tronco2N (3).png", 0, 0);
		bicicleta= new ModeloRR(d3dDevice, d3dContext, "MODELOS/Vehiculo/bike2.obj",L"Rojo.png", L"NormalMap.png", 20, 20);
		basura = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Basura/TRASH1.obj", L"Gris.png", L"NormalMap.png", 80, 60);
		mesa = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Mesa/wood_bench.obj", L"MODELOS/Mesa/texture_pino.jpg", L"MODELOS/Bosque/ArbolCafeSpec.png", 40, 15);
		cruz = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", 30, 0);
		enemigo = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Enemigo/kodama.obj", L"Gris.png", L"NormalMap.png", 40, 40);
		fogata = new ModeloRR(d3dDevice, d3dContext, "MODELOS/campfire/campfire.obj", L"MODELOS/Grupo_arboles/tronco2.jpg", L"MODELOS/Grupo_arboles/Tronco2N (3).png", 0,60);
		vida = new GUI(d3dDevice, d3dContext, 0.55, 0.35, L"Assets/Materiales/Crucecita.png");
		
		texto = new Text(d3dDevice, d3dContext, 9, 3, L"Assets/Materiales/font_2.png",XMFLOAT4(0.0f,0.0f,0.0f,1.0f));
		//========================================================================
		

		
	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for (driver = 0; driver < totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels,
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if (SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if (FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}

		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if (FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if (backBufferTexture)
			backBufferTexture->Release();

		if (FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if (FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);


		//Cargar audio
		bool resultado = m_XACT3.Initialize();
	
		if (!resultado) {
			return resultado;
		}

		resultado = m_XACT3.LoadWaveBank(L"Audios\\Win\\uwu.xwb");

		if (!resultado) {
			return resultado;
		}

		resultado = m_XACT3.LoadSoundBank(L"Audios\\Win\\Sound Bank.xsb");

		//Reproducir audio
		// //1-05-Fight-Against-Koopa
		cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("1-28-Beware-the-Forest_s-Mushrooms");
		walking = m_XACT3.m_pSoundBank->GetCueIndex("StepGrass");
		//cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("1-05-Fight-Against-Koopa");
		//m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);
		//m_XACT3.m_pSoundBank->Play(walking, 0, 0, 0);

		if (is_Walking == true ) {
			on_Music = true;
			entra = true;
			m_XACT3.m_pSoundBank->Play(walking, 0, 0, 0);
			
		}
		else {
		//	m_XACT3.m_pSoundBank->Stop(walking, 0);
		//	on_Music = false;
		}


		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		rotacioncamera += izqder ;
		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.01;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;
	


		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 7 ;
		bool chocan = isPointInsideSphere(camara->getpoint(), fogata->getSphere(7.5));
		bool enemy = isPointInsideSphere(camara->getpoint(), enemigo->getSphere(10.5));
		bool bici = isPointInsideSphere(camara->getpoint(), bicicleta->getSphere(10.5));
		bool crossi = isPointInsideSphere(camara->getpoint(), cruz->getSphere(5.5));
		//Colisiones antes del updatecam
			//Al chocar con colision
		//	m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);


		if (!chocan) {
			camara->UpdateCam(vel, vel2, arriaba, izqder);
	

		}
		else {
			camara->posCam = camara->posCam2;
		}

		if (!enemy) {
			camara->UpdateCam(vel, vel2, arriaba, izqder);

		}
		else {
	
		}

		if (!bici) {
			camara->UpdateCam(vel, vel2, arriaba, izqder);
			subir_bici = false;
			//Al chocar con colision
		//	m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);

		}
		else {
			camara->posCam = camara->posCam2;
			subir_bici = true;
			mostrar_mensaje = true;
		}

		if (!crossi) {
			camara->UpdateCam(vel, vel2, arriaba, izqder);

		}
		else {
			camara->posCam = camara->posCam2;
		//	cruz->
			cruces_recogidas++;
		}

		
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);



		//=====================================ARBOLES============================================================
		//int i = 0;
		//while (i <= 99) {
		//	model->setPosX(posrand[i]);
		//	i++;
		//	model->setPosZ(posrand[i]);
		//	i++;
		//	model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) - 5, camara->posCam, 10.0f, 0, 'Z', .25);
		//	
		//}
		//
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			0, 60, 0, 2.5, false ,uv1, uv2, uv3, uv4, frameBillboard);

		arbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
		-40, -20, terreno->Superficie(-40,-20), 20, true);


		//Fondo->Draw(camara->vista, camara->proyeccion, camara->posCam,
		//	-128, -128,0, 1000, true);
		//Fondo->Draw(camara->vista, camara->proyeccion, camara->posCam,
		//	-128, 128,0, 1000, true);
		//Fondo->Draw(camara->vista, camara->proyeccion, camara->posCam,
		//	 128, -128, 0, 1000, true);
		//Fondo->Draw(camara->vista, camara->proyeccion, camara->posCam,
		//	-128, -128, 0, 1000, true);

		//==========================================================================================================
		//================CARGAR LOS MODELOS============================
		model->setPosX(30.0f);
		model->setPosZ(30.0f);
		model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0), camara->posCam, 10.0f, 0, 'Z', .25);


		edificio->setPosX(-40.0f);
		edificio->setPosZ(0.0f);
	 	edificio->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0)  , camara->posCam, 10.0f, 0, 'Z', 1);

		edificio2->setPosX(-5.0f);
		edificio2->setPosZ(0.0f);
		edificio2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0), camara->posCam, 10.0f, 0, 'Z', 2.5);



		//bicicleta->setPosX(-10.0f);
		//bicicleta->setPosZ(-10.0f);

		if (sobre_bici) {
			bicicleta->setPosX(camara->posCam.x);
			bicicleta->setPosZ(camara->posCam.z);
			mostrar_mensaje = false;
		}
		else {
			camara->posCam = camara->posCam2;
		
		}



		if (sobre_bici) {
			bicicleta->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) + .25, camara->posCam, 10.0f, XM_PIDIV2 + rotacioncamera, 'Y', 0.030,true);
		}
		else {
	
			bicicleta->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) + .25, camara->posCam, 10.0f, XM_PIDIV2 + 90, 'Y', 0.030);

		}
		
	
		
		basura->setPosX(-25.0f);
		basura->setPosZ(-25.0f);
		basura->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) , camara->posCam, 10.0f, 0, 'X', 0.05);

		mesa->setPosX(20.0f);
		mesa->setPosZ(0.0f);
		mesa->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) , camara->posCam, 10.0f, 0, 'Z', 0.0045);

		cruz->setPosX(30.0f);
		cruz->setPosZ(0.0f);
		cruz->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0) + 5 , camara->posCam, 10.0f, XM_PIDIV2 + angle, 'X', 0.005);

		//enemigo->setPosX(0.0f);
		//enemigo->setPosZ(40.0f);
		
		//seguir_jugador(camara->getpoint(), enemigo->getposition(),enemigo);
		float x = camara->posCam.x;
		float z = camara->posCam.z;
		enemigo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(enemigo->getPosX(), enemigo->getPosZ()), camara->posCam, 10.0f, XM_PIDIV2 + rotacioncamera, 'Y', 1.0, true);

		fogata->setPosX(0.0f);
		fogata->setPosZ(60.0f);
		fogata->Draw(camara->vista, camara->proyeccion, terreno->Superficie(0, 0), camara->posCam, 10.0f, 0, 'Z', 2.0);

		//==============================================================

		//GUI

       vida->Draw(0.4f, 0.7f);
       TurnOnAlphaBlending();


	   if (subir_bici && mostrar_mensaje) {
		   texto->DrawText(-0.75f, -0.10f, "Pulsa E para subirte", 0.03);

		  // texto->DrawText(-0.75f, -0.30f, "True", 0.03);
		}

	   texto->DrawText(-0.75f, -0.20f, to_string(is_Walking), 0.03);
	   texto->DrawText(-0.75f, -0.40f, to_string(entra), 0.03);
	   texto->DrawText(0.55f, 0.7f, to_string(cruces_recogidas), 0.03);
	   if (is_Walking == true  && entra == true) {
		   texto->DrawText(-0.75f, -0.10f, "Musica reproducida", 0.03);
	   }
		  
      TurnOffAlphaBlending();

		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]) 
		);

		if (distance < sphere[2]) 
			collition = true;
		

		return collition;
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

};


#endif

//FUNCIONES COMENTADAS
/*   texto->DrawText(0.55f, 0.7f, "0", 0.015);
   texto->DrawText(-0.75f, 0.7f, to_string(x), 0.03);
   texto->DrawText(-0.75f, 0.3f, to_string(z), 0.03);*/


   //if (is_Walking) {
	  // texto->DrawText(-0.75f, 0.7f, "Si se presiona", 0.03);
	  // if (entra) {
		 //  texto->DrawText(-0.75f, 0.5f, "Si entra aqui", 0.03);
	  // }
   //}
   //else  {
	  // texto->DrawText(-0.75f, 0.7f, "No se presiona", 0.03);
	  // if (entra == false) {
		 //  texto->DrawText(-0.75f, 0.5f, "No entra aqui", 0.03);
	  // }
   //}