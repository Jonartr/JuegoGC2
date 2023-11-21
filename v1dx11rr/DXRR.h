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
#include <cmath>

#include "GUI.h"
#include "Text.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-150.0f, 150.0f);
float posrand[100];



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
	SkyDome* Noche;
	BillboardRR *billboard;
	BillboardRR* arbol;
	BillboardRR* fantasma;
	BillboardRR* Fondo;
	Camara *camara;
	ModeloRR* model;
	ModeloRR* model2;
	ModeloRR* model3;
	ModeloRR* model4;
	ModeloRR* model5;
	ModeloRR* edificio;
	ModeloRR* edificio2;
	ModeloRR* bicicleta;
	ModeloRR* bicicleta2;
	ModeloRR* basura;
	ModeloRR* mesa;

	ModeloRR* cruz;
	ModeloRR* cruz2;
	ModeloRR* cruz3;
	ModeloRR* cruz4;
	ModeloRR* cruz5;
	ModeloRR* cruz6;
	ModeloRR* cruz7;
	ModeloRR* cruz8;
	ModeloRR* cruz9;
	ModeloRR* cruz10;


	ModeloRR* enemigo;
	ModeloRR* fogata;
	ModeloRR* Montaña;
	ModeloRR* Tumba1;
	ModeloRR* Tumba2;
	ModeloRR* Tumba3;
	ModeloRR* Tumba4;
	ModeloRR* Tumba5;

	ModeloRR* settiempo;

	GUI* vida;
	GUI* gogui;
	GUI* Wingui;
	Text* texto;
	Text* inicio_gaming;
	Text* gameovertext;
	float izqder;
	float arriaba;
	float rotacioncamera;
	float vel, vel2;
	float rotacioncruz;
	bool subir_bici = false, sobre_bici = false, mostrar_mensaje = false,reproduciendo=false;
	bool breakpoint, sobre_colision = false, near_campfire;
	bool is_Walking = false, on_Sound = false, colisiona = false, sobrecruz = false;
	bool on_Bike = false, reproduciendobike = false;
	bool enemysound = false,  near_enemy = false;
	bool gameover = false, screamer = false;
	int cruces_recogidas = 0;
	bool camaratipo = false;
	//Si se agarra una cruz se activa la bandera para que ya no se dibuje
	bool grabcruz = false, grabcruz2 = false, grabcruz3 = false, grabcruz4 = false, grabcruz5 = false;

	bool wingame = false;



	float bici2_x = 0, bici2_z = 0;
	float tiempo = 0.001;
	float tiempo_dia = 0.01;
	float distanciaglobal = 0;
	float tiempo_inicio = 0.06;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];
	vector2 uvfija;

	float movX = 0, movZ = 0, movY = 0;

	XACTINDEX cueIndex;
	XACTINDEX walking;
	XACTINDEX holySound;
	XACTINDEX campFire;
	XACTINDEX risa;
	XACTINDEX grito;
	XACTINDEX onbiki;

	CXACT3Util m_XACT3;

	bool dianoche = false;
	bool reinicio_juego = false;

	void reiniciar_juego() {
		//reiniciamos los parametros del juego 
		subir_bici = false;  sobre_bici = false; mostrar_mensaje = false; reproduciendo = false;
		breakpoint, sobre_colision = false, near_campfire; is_Walking = false; on_Sound = false; colisiona = false;
		sobrecruz = false; on_Bike = false; reproduciendobike = false; enemysound = false; near_enemy = false;
		gameover = false; screamer = false;
		grabcruz = false, grabcruz2 = false, grabcruz3 = false, grabcruz4 = false, grabcruz5 = false;
		cruces_recogidas = 0;
		tiempo = 0.001;
		tiempo_dia = 0.01;
		distanciaglobal = 0;
	    tiempo_inicio = 0.06;
		enemigo->setPosX(260);
		enemigo->setPosZ(260);

		reinicio_juego = false;

		camara->posCam.x = 0;
		camara->posCam.z = 0;

	}

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
			tiempo += 0.000002;
		
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
		camara = new Camara(D3DXVECTOR3(0,80,6), D3DXVECTOR3(0,80,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(512, 512, d3dDevice, d3dContext);
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Cielo.jpg");
		Noche = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Noche.png");

		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		model = new ModeloRR(d3dDevice, d3dContext, "MODELOS/ArbolCafe/ArbolCafe.obj", L"MODELOS/ArbolA/ArbolCafeTextura.jpg", L"MODELOS/ArbolA/ArbolCafeTexturaSPEC.jpg", 50, 80);

		model2 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/ArbolCafe/ArbolCafe.obj", L"MODELOS/ArbolCafe/ArbolCafeTextura.jpg", L"MODELOS/ArbolCafe/ArbolCafeAO.png", 40, -140);
		model3 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/ArbolCafe/ArbolCafe.obj", L"MODELOS/ArbolCafe/ArbolCafeTextura.jpg", L"MODELOS/ArbolCafe/ArbolCafeAO.png", -120, 30);
		model4 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/ArbolCafe/ArbolCafe.obj", L"MODELOS/ArbolCafe/ArbolCafeTextura.jpg", L"MODELOS/ArbolCafe/ArbolCafeAO.png", 40, 150);
		model5 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/ArbolCafe/ArbolCafe.obj", L"MODELOS/ArbolCafe/ArbolCafeTextura.jpg", L"MODELOS/ArbolCafe/ArbolCafeAO.png", 60, -160);
		
		Tumba1 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Tumba/Gravestone_Original.obj", L"MODELOS/Tumba/Grave.png", L"MODELOS/Tumba/Grave_AO.png", 190, -180);
		Tumba2 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Tumba/Gravestone_Original.obj", L"MODELOS/Tumba/Grave.png", L"MODELOS/Tumba/Grave_AO.png", 90, -180);
		Tumba3 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Tumba/Gravestone_Original.obj", L"MODELOS/Tumba/Grave.png", L"MODELOS/Tumba/Grave_AO.png", -10,- 180);
		Tumba4 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Tumba/Gravestone_Original.obj", L"MODELOS/Tumba/Grave.png", L"MODELOS/Tumba/Grave_AO.png", -90, -180);
		Tumba5 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Tumba/Gravestone_Original.obj", L"MODELOS/Tumba/Grave.png", L"MODELOS/Tumba/Grave_AO.png", -150, -180);

		arbol = new BillboardRR(L"Assets/Billboards/fuego-anim.png", L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		Fondo = new BillboardRR(L"Assets/Materiales/bosquecito.jpg", L"NormalMap.png", d3dDevice, d3dContext, 100);
		fantasma = new BillboardRR(L"Assets/Billboards/preview.png", L"Assets/Billboards/previewNormal.png", d3dDevice, d3dContext, 5);


		//========================MODELOS ========================================
		//COLOCAR COORDENADAS, PORQUE AL CARGAR POR PRIMERA VEZ LA COLISION SERA VERDADERA
		//Y AL QUERER CARGAR LA POSICION ANTERIOR COMO ESTA ES NULA MARCARA ERROR
		
		edificio = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Casitas/woodenhouse.obj", L"MODELOS/ArbolA/ArbolCafeTextura.jpg", L"MODELOS/ArbolA/ArbolCafeNM.png", 120, 110);
		edificio2 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Casitas/woodenhouse.obj", L"MODELOS/ArbolA/ArbolCafeTextura.jpg", L"MODELOS/ArbolA/ArbolCafeNM.png", -140, 85);

		bicicleta= new ModeloRR(d3dDevice, d3dContext, "MODELOS/Vehiculo/bike21.obj",L"MODELOS/Vehiculo/blinn1SG_Base_color.jpg", L"MODELOS/Vehiculo/blinn1SG_SC.jpg", 20, 20);
		bicicleta2 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Vehiculo/biciconmono.obj", L"MODELOS/Vehiculo/blinn1SG_Base_color.jpg", L"MODELOS/Vehiculo/blinn1SG_SC.jpg", 20, 20);

		basura = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Basura/TRASH1.obj", L"Gris.png", L"NormalMap.png", 80, 60);

		mesa = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Mesa/wood_bench.obj", L"MODELOS/Mesa/texture_pino.jpg", L"MODELOS/Mesa/texture_pinoSC.png", -140, 55);
		mesa = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Mesa/wood_bench.obj", L"MODELOS/Mesa/texture_pino.jpg", L"MODELOS/Mesa/texture_pinoSC.png", -140, 55);
		mesa = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Mesa/wood_bench.obj", L"MODELOS/Mesa/texture_pino.jpg", L"MODELOS/Mesa/texture_pinoSC.png", -140, 55);
		mesa = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Mesa/wood_bench.obj", L"MODELOS/Mesa/texture_pino.jpg", L"MODELOS/Mesa/texture_pinoSC.png", -140, 55);
		mesa = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Mesa/wood_bench.obj", L"MODELOS/Mesa/texture_pino.jpg", L"MODELOS/Mesa/texture_pinoSC.png", -140, 55);

	    cruz = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", -50, -140);
		 cruz2  = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", 140, 160);
		 cruz3 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", -180, 99);
		 cruz4  = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", -125, -200);
		cruz5 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", 10, 10);
		cruz6 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", -140, -140);
		cruz7 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", 140, 160);
		cruz8 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", -72, 80);
		cruz9 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", -30, -110);
		cruz10 = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Cruz/Cross2.obj", L"MODELOS/Cruz/tex/CIMG0212 tiles.jpg", L"MODELOS/Cruz/tex/cross specular color.png", 100, 100);




		enemigo = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Enemigo/kodama.obj", L"MODELOS/Enemigo/Enemigo-textura.png", L"MODELOS/Enemigo/Enemigo-texturaSC.png", 260, 260);
		fogata = new ModeloRR(d3dDevice, d3dContext, "MODELOS/campfire/campfire.obj", L"MODELOS/ArbolA/ArbolCafeTextura.jpg", L"MODELOS/ArbolA/ArbolCafeNM.png", 80,110);
		Montaña = new ModeloRR(d3dDevice, d3dContext, "MODELOS/Montaña/ltb.obj", L"MODELOS/Montaña/Mountain_COL_4096.jpg", L"MODELOS/Montaña/Mountain_REFL_4096.jpg",128 ,240);
		
		vida = new GUI(d3dDevice, d3dContext, 0.55, 0.35, L"Assets/Materiales/Crucecita.png");
		gogui = new GUI(d3dDevice, d3dContext, 3, 3, L"fondogameover.png");
		Wingui = new GUI(d3dDevice, d3dContext, 3, 3, L"Win.jpg");


		texto = new Text(d3dDevice, d3dContext, 9, 3, L"Assets/Materiales/font_2.png",XMFLOAT4(1.0f,1.0f,1.0f,1.0f));
		gameovertext = new Text(d3dDevice, d3dContext, 8, 2.65, L"Assets/Materiales/font_2.png", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		inicio_gaming = new Text(d3dDevice, d3dContext, 4.5, 1.5, L"Assets/Materiales/font_2.png", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

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
		holySound = m_XACT3.m_pSoundBank->GetCueIndex("HolySound");
		campFire = m_XACT3.m_pSoundBank->GetCueIndex("Sonido-De-Fogata");

		onbiki = m_XACT3.m_pSoundBank->GetCueIndex("Bici");
		risa = m_XACT3.m_pSoundBank->GetCueIndex("Risas");
		grito = m_XACT3.m_pSoundBank->GetCueIndex("Grito");
	//	m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, &pCue1);
	//	m_XACT3.m_pSoundBank->Prepare(walking, 0, 0, &pCue2);
	//	pCue1->Play();
		//cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("1-05-Fight-Against-Koopa");
		//m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);
		//m_XACT3.m_pSoundBank->Play(walking, 0, 0, 0);
		//m_XACT3.m_pSoundBank->Play(campFire, 0, 0, 0);

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

		if (reinicio_juego) {
			reiniciar_juego();
		}
		rotacioncamera += izqder;

		if (rotacioncamera >= 6.0 || rotacioncamera <= -6.0) {
			rotacioncamera = 0.0;
		}
		float sphere[3] = { 0,0,0 };
		// X   Z   R
		float limite1[3] = { 100,520,300 };
		float limite2[3] = { -100, 520 ,300 };


		float limite5[3] = { 100, -520,300 };
		float limite6[3] = { -100, -520 ,300 };

		float limite3[3] = { -520,100,300 };
		float limite4[3] = { -520,-100,300 };

		float limite7[3] = { 520, 100,300 };
		float limite8[3] = { 520,-100,300 };

		float limite9[3] = { -256, 256 ,50 };
		float limite10[3] = { 256, 256 ,50 };
		float limite11[3] = { -256,256,50 };
		float limite12[3] = { -256,-256,50 };

		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.01;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if (d3dContext == 0)
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
		d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 7;
		camara->posCam3P.y = terreno->Superficie(camara->posCam3P.x, camara->posCam3P.z) + 7;


		bool chocan = isPointInsideSphere(camara->getpoint(), fogata->getSphere(5.5));

		bool arbol1 = isPointInsideSphere(camara->getpoint(), model->getSphere(5.5));
		bool arbol2 = isPointInsideSphere(camara->getpoint(), model2->getSphere(5.5));
		bool arbol3 = isPointInsideSphere(camara->getpoint(), model3->getSphere(5.5));
		bool arbol4 = isPointInsideSphere(camara->getpoint(), model4->getSphere(5.5));
		bool arbol5 = isPointInsideSphere(camara->getpoint(), model5->getSphere(5.5));

		bool grave1 = isPointInsideSphere(camara->getpoint(), Tumba1->getSphere(5.5));
		bool grave2 = isPointInsideSphere(camara->getpoint(), Tumba2->getSphere(5.5));
		bool grave3 = isPointInsideSphere(camara->getpoint(), Tumba3->getSphere(5.5));
		bool grave4 = isPointInsideSphere(camara->getpoint(), Tumba4->getSphere(5.5));
		bool grave5 = isPointInsideSphere(camara->getpoint(), Tumba5->getSphere(5.5));

		bool mesaa = isPointInsideSphere(camara->getpoint(), mesa->getSphere(7.5));

		bool enemy = isPointInsideSphere(camara->getpoint(), enemigo->getSphere(5.5));
		//Si esta cerca del personaje empezara a emitir audio
		bool enemy_sound = isPointInsideSphere(camara->getpoint(), enemigo->getSphere(50.0));
		bool bici = isPointInsideSphere(camara->getpoint(), bicicleta->getSphere(5.5));
		bool crossi = false, crossi2 = false,  crossi3 = false,  crossi4 = false,  crossi5 =false;

		if(!grabcruz)
		 crossi = isPointInsideSphere(camara->getpoint(), cruz->getSphere(5.5));
		if (!grabcruz2)
		 crossi2 = isPointInsideSphere(camara->getpoint(), cruz2->getSphere(5.5));
		if (!grabcruz3)
		 crossi3 = isPointInsideSphere(camara->getpoint(), cruz3->getSphere(5.5));
		if (!grabcruz4)
		 crossi4 = isPointInsideSphere(camara->getpoint(), cruz4->getSphere(5.5));
		if (!grabcruz5)
		 crossi5 = isPointInsideSphere(camara->getpoint(), cruz5->getSphere(5.5));

		bool casa2 = isPointInsideSphere(camara->getpoint(), edificio->getSphere(30.0));

		bool casa = isPointInsideSphere(camara->getpoint(), edificio2->getSphere(30.0));

		bool limites = isPointInsideSphere(camara->getpoint(), limite1);
		bool limites2 = isPointInsideSphere(camara->getpoint(), limite2);
		bool limites3 = isPointInsideSphere(camara->getpoint(), limite3);
		bool limites4 = isPointInsideSphere(camara->getpoint(), limite4);
		bool limites5 = isPointInsideSphere(camara->getpoint(), limite5);
		bool limites6 = isPointInsideSphere(camara->getpoint(), limite6);
		bool limites7 = isPointInsideSphere(camara->getpoint(), limite7);
		bool limites8 = isPointInsideSphere(camara->getpoint(), limite8);

		bool limites9 = isPointInsideSphere(camara->getpoint(), limite9);
		bool limites10 = isPointInsideSphere(camara->getpoint(), limite10);
		bool limites11 = isPointInsideSphere(camara->getpoint(), limite11);
		bool limites12 = isPointInsideSphere(camara->getpoint(), limite12);

		//Colisiones antes del updatecam
			//Al chocar con colision
		//	m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);


		if (chocan) {
			//	camara->UpdateCam(vel, vel2, arriaba, izqder);
			colisiona = true;
		}
		else  if (enemy) {
			//	camara->UpdateCam(vel, vel2, arriaba, izqder);
				//colisiona = true;
			gameover = true;

			if (!screamer) {
				m_XACT3.m_pSoundBank->Play(grito, 0, 0, 0);
			}
			screamer = true;

			
		}
		else if (casa) {
			//		camara->UpdateCam(vel, vel2, arriaba, izqder);
			colisiona = true;

		}
		else if (casa2) {
			//		camara->UpdateCam(vel, vel2, arriaba, izqder);
			colisiona = true;

		}
		else if (limites) {
			colisiona = true;
		}
		else if (limites2) {
			colisiona = true;
		}
		else if (limites3) {
			colisiona = true;
		}
		else if (limites4) {
			colisiona = true;
		}
		else if (limites5) {
			colisiona = true;
		}
		else if (limites6) {
			colisiona = true;
		}
		else if (limites7) {
			colisiona = true;
		}
		else if (limites8) {
			colisiona = true;
		}
		else if (limites9) {
			colisiona = true;
		}
		else if (limites10) {
			colisiona = true;
		}
		else if (limites11) {
			colisiona = true;
		}
		else if (limites12) {
			colisiona = true;
		}
		else if (arbol1) {
			colisiona = true;
		}
		else if (arbol2) {
			colisiona = true;
		}
		else if (arbol3) {
			colisiona = true;
		}
		else if (arbol4) {
			colisiona = true;
		}
		else if (arbol5) {
			colisiona = true;
		}
		else if (mesaa) {
			colisiona = true;
		}
		else if (bici) {
			//camara->UpdateCam(vel, vel2, arriaba, izqder);
			subir_bici = true;
			mostrar_mensaje = true;
		}
		else if (crossi) {
			sobre_colision = true;
			grabcruz = true;
		}
		else if (crossi2) {
			sobre_colision = true;
			grabcruz2 = true;
		}
		else if (crossi3) {
			sobre_colision = true;
			grabcruz3 = true;
		}
		else if (crossi4) {
			sobre_colision = true;
			grabcruz4 = true;
		}
		else if (crossi5) {
			sobre_colision = true; grabcruz5 = true;
		}
		else if (grave1) {
		colisiona = true;
		}
		else if (grave2) {
		colisiona = true;
		}
		else if (grave3) {
		colisiona = true;
		}
		else if (grave4) {
		colisiona = true;
		}
		else if (grave5) {
		colisiona = true;
		}



		if (!colisiona) {
			if (sobre_bici) {
				camara->UpdateCam(vel, vel2, arriaba, izqder, camaratipo);
			}
			else {
				camara->UpdateCam(vel, vel2, arriaba, izqder, camaratipo);
			}

			colisiona = false;

		}
		else {
			camara->posCam = camara->posCam2;
			camara->posCam3P = camara->posCam4;
			colisiona = false;

		}

		if (!bici) {
			subir_bici = false;
			mostrar_mensaje = false;
		}


		if (crossi || crossi2 || crossi3 || crossi4||crossi5) {
			if (!sobrecruz) {
				cruces_recogidas++;
				m_XACT3.m_pSoundBank->Play(holySound, 0, 0, 0);
				sobrecruz = true;
			}

			if (cruces_recogidas == 5) {
				wingame = true;
				
			}

		}
		else {
			sobrecruz = false;
		}

		if (enemy_sound) {
			near_enemy = true;
		}
		else {
			near_enemy = false;
		}

		if (is_Walking == true && gameover == false && wingame == false) {

			if (!reproduciendo) {
				m_XACT3.m_pSoundBank->Play(walking, 0, 0, 0);
				reproduciendo = true;
			}

		}
		else {
			m_XACT3.m_pSoundBank->Stop(walking, 0);
			reproduciendo = false;
		}


		if (on_Bike == true && gameover == false && wingame == false) {

			if (!reproduciendobike) {
				m_XACT3.m_pSoundBank->Play(onbiki, 0, 0, 0);
				reproduciendobike = true;
			}

		}
		else {
			m_XACT3.m_pSoundBank->Stop(onbiki, 0);
			reproduciendobike = false;
		}


		if (near_enemy == true && gameover == false && wingame == false) {
			if (!enemysound) {
				m_XACT3.m_pSoundBank->Play(risa, 0, 0, 0);
				enemysound = true;
			}
		}
		else {
			m_XACT3.m_pSoundBank->Stop(risa, 0);
			enemysound = false;
		}

		if (tiempo_dia <= 1.00 && !dianoche) {
			tiempo_dia += 0.001;
			TurnOnDepth();
			Noche->Update(camara->vista, camara->proyeccion);
			TurnOffDepth();
			float camPosXZ[2] = { camara->posCam.x, camara->posCam.z, };

			TurnOffDepth();
			Noche->Render(camara->posCam);
			TurnOnDepth();


		}
		else {
			dianoche = true;
		}

		if (tiempo_dia >= 0.00 && dianoche) {
			tiempo_dia -= 0.001;

			TurnOnDepth();
			skydome->Update(camara->vista, camara->proyeccion);
			TurnOffDepth();
			float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

			TurnOffDepth();

			skydome->Render(camara->posCam);
			TurnOnDepth();
		}
		else {
			dianoche = false;
		}


		float* PosPl = new float[3]{ cruz->getPosX(),10,cruz->getPosZ() };
		float* cPl = new float[3]{ 1.0,1.0,1.0 };
		terreno->setPospLight(PosPl);
		terreno->setColorpLight(cPl);
		terreno->setRangepLight(1.0);
		terreno->setpointLight(true);
		terreno->Draw(camara->vista, camara->proyeccion, 1, tiempo_dia);

		//=====================================ARBOLES============================================================

	

		
		

		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			fogata->getPosX(), fogata->getPosZ(), terreno->Superficie(fogata->getPosX(), fogata->getPosZ()), 2.5, false, uv1, uv2, uv3, uv4, frameBillboard);


		fantasma->Draw(camara->vista, camara->proyeccion, camara->posCam,
			Tumba3->getPosX(), Tumba3->getPosZ(), terreno->Superficie(Tumba3->getPosX(), Tumba3->getPosZ()) + 5, 5, true);

		//arbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
		//	-40, -20, terreno->Superficie(-40, -20), 20, true);
		//==========================================================================================================
		//================CARGAR LOS MODELOS============================
		

		edificio->setTimer(tiempo_dia);
		edificio->Draw(camara->vista, camara->proyeccion, terreno->Superficie(edificio->getPosX(), edificio->getPosZ()), camara->posCam, 10.0f, XM_PI + 90, 'Y', 2.5);

		edificio2->setTimer(tiempo_dia);
		edificio2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(edificio2->getPosX(), edificio2->getPosZ()), camara->posCam, 10.0f, XM_PI - 90, 'Y', 2.5);



		if (sobre_bici) {
			bicicleta2->setPosX(camara->posCam.x);
			bicicleta2->setPosZ(camara->posCam.z);

			bicicleta->setPosX(camara->posCam.x);
			bicicleta->setPosZ(camara->posCam.z);

			mostrar_mensaje = false;
		}

		if (sobre_bici) {
			bicicleta2->setTimer(tiempo_dia);
			bicicleta2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(bicicleta->getPosX(), bicicleta->getPosZ()), camara->posCam, 10.0f, XM_PI + 0, 'Y', 0.50, true, true);
		}
		else {
			bicicleta->setTimer(tiempo_dia);
			bicicleta->Draw(camara->vista, camara->proyeccion, terreno->Superficie(bicicleta->getPosX(), bicicleta->getPosZ()), camara->posCam, 10.0f, XM_PI + 45, 'Y', 0.70);

		}

		model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(model->getPosX(), model->getPosZ()) - 5, camara->posCam, 10.0f, 0, 'Z', .5);
		model2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(model2->getPosX(), model2->getPosZ()) - 5, camara->posCam, 10.0f, 0, 'Z', .5);
		model3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(model3->getPosX(), model3->getPosZ()) - 5, camara->posCam, 10.0f, 0, 'Z', .5);
		model4->Draw(camara->vista, camara->proyeccion, terreno->Superficie(model4->getPosX(), model4->getPosZ()) - 5, camara->posCam, 10.0f, 0, 'Z', .5);
		model5->Draw(camara->vista, camara->proyeccion, terreno->Superficie(model5->getPosX(), model5->getPosZ()) - 5, camara->posCam, 10.0f, 0, 'Z', .5);


		Tumba1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(Tumba1->getPosX(), Tumba1->getPosZ()), camara->posCam, 10.0f, XM_PI + 90, 'Y', 0.5);

		Tumba2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(Tumba2->getPosX(), Tumba2->getPosZ()), camara->posCam, 10.0f, XM_PI + 90, 'Y', 0.5);
		Tumba3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(Tumba3->getPosX(), Tumba3->getPosZ()), camara->posCam, 10.0f, XM_PI + 90, 'Y', 0.5);
		Tumba4->Draw(camara->vista, camara->proyeccion, terreno->Superficie(Tumba4->getPosX(), Tumba4->getPosZ()), camara->posCam, 10.0f, XM_PI + 90, 'Y', 0.5);
		Tumba5->Draw(camara->vista, camara->proyeccion, terreno->Superficie(Tumba5->getPosX(), Tumba5->getPosZ()), camara->posCam, 10.0f, XM_PI + 90, 'Y', 0.5);

		basura->setTimer(tiempo_dia);
		basura->Draw(camara->vista, camara->proyeccion, terreno->Superficie(basura->getPosX(), basura->getPosZ()), camara->posCam, 10.0f, 0, 'X', 0.05);

		mesa->setTimer(tiempo_dia);
		mesa->Draw(camara->vista, camara->proyeccion, terreno->Superficie(mesa->getPosX(), mesa->getPosZ()), camara->posCam, 10.0f, 0, 'Z', 0.0045);


		if (!grabcruz) {
			cruz->setTimer(tiempo_dia);
			cruz->Draw(camara->vista, camara->proyeccion, terreno->Superficie(cruz->getPosX(), cruz->getPosZ()) + 5, camara->posCam, 10.0f, XM_PIDIV2 + angle, 'X', 0.005);
		}
		if (!grabcruz2) {
			cruz2->setTimer(tiempo_dia);
			cruz2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(cruz2->getPosX(), cruz2->getPosZ()) + 5, camara->posCam, 10.0f, XM_PIDIV2 + angle, 'X', 0.005);
		}
		if (!grabcruz3) {
			cruz3->setTimer(tiempo_dia);
			cruz3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(cruz3->getPosX(), cruz3->getPosZ()) + 5, camara->posCam, 10.0f, XM_PIDIV2 + angle, 'X', 0.005);
		}
		if (!grabcruz4) {
			cruz4->setTimer(tiempo_dia);
			cruz4->Draw(camara->vista, camara->proyeccion, terreno->Superficie(cruz4->getPosX(), cruz4->getPosZ()) + 5, camara->posCam, 10.0f, XM_PIDIV2 + angle, 'X', 0.005);
		}
		if (!grabcruz5) {
			cruz5->setTimer(tiempo_dia);
			cruz5->Draw(camara->vista, camara->proyeccion, terreno->Superficie(cruz5->getPosX(), cruz5->getPosZ()) + 5, camara->posCam, 10.0f, XM_PIDIV2 + angle, 'X', 0.005);
		}
		enemigo->setTimer(tiempo_dia);
		if (tiempo_inicio == 0.00) {
			if (!wingame) {
	//			seguir_jugador(camara->getpoint(), enemigo->getposition(), enemigo);
			}
	//		
		}
		enemigo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(enemigo->getPosX(), enemigo->getPosZ()), camara->posCam, 10.0f, XM_PIDIV2 + 90, 'Y', 1.0, true);

		fogata->setTimer(tiempo_dia);
		fogata->Draw(camara->vista, camara->proyeccion, terreno->Superficie(fogata->getPosX(), fogata->getPosZ()), camara->posCam, 10.0f, 0, 'Z', 2.0);;

		//==============================================================

		//GUI
		if (gameover) {
		//	TurnOnAlphaBlending();
			gameovertext->DrawText(-0.50f, 0.40f, "Haz muerto", 0.03);
			gameovertext->DrawText(-0.70f, -0.10f, "P para reintentar", 0.03);
			gameovertext->DrawText(-0.90f, -0.40f, "Escape para cerrar juego", 0.03);
		//	TurnOnAlphaBlending();
			gogui->Draw(-0.0, -0.0);
		}

		if (wingame) {
			gameovertext->DrawText(-0.50f, 0.40f, "Haz ganado", 0.03);
			gameovertext->DrawText(-0.70f, -0.10f, "P para reintentar", 0.03);
			gameovertext->DrawText(-0.90f, -0.40f, "Escape para cerrar juego", 0.03);
			Wingui->Draw(0.0, 0.0);

		}
		vida->Draw(0.4f, 0.7f);
		TurnOnAlphaBlending();

		if (tiempo_inicio > 0.00) {
			inicio_gaming->DrawText(-0.85f, -0.0f, "Debes recolectar 5 cruces", 0.03);
			inicio_gaming->DrawText(-0.85f, -0.20f, "Pero cuidado", 0.03);
			inicio_gaming->DrawText(-0.85f, -0.40f, "Hay alguien acechandote", 0.03);
			tiempo_inicio -= 0.0001;
		}
		else {
			tiempo_inicio = 0.0;
		}

		if (subir_bici && mostrar_mensaje) {
			texto->DrawText(-0.75f, -0.10f, "Pulsa E para subirte", 0.03);
		}

		texto->DrawText(0.55f, 0.7f, to_string(cruces_recogidas), 0.03);

		texto->DrawText(0.55f, 0.7f, to_string(arbol1), 0.03);
		texto->DrawText(0.55f, 0.5f, to_string(arbol2), 0.03);
		texto->DrawText(0.55f, 0.3f, to_string(arbol3), 0.03);
		texto->DrawText(0.55f, 0.1f, to_string(arbol4), 0.03);
		texto->DrawText(0.55f, -0.1f, to_string(arbol5), 0.03);




		TurnOffAlphaBlending();



		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]) 
		);

		distanciaglobal = distance;

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

