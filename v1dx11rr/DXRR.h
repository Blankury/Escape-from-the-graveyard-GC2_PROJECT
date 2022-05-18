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
#include "lago.h"
#include "XACT3Util.h"
#include "GUI.h"


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

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;

	TerrenoRR *terreno;
	SkyDome *skydome;
	BillboardRR *billboard;
	BillboardRR* hojas;
	BillboardRR* esqueleto;
	BillboardRR *arbol, *arbol1, *arbol2, *arbol3, *arbol4, *arbol5, *arbol6, *arbol7;



	LagoRR* lago;
	LagoRR* lago2;

	Camara *camara;
	ModeloRR* puerta;
	ModeloRR* pared;
	ModeloRR* puerta1;
	ModeloRR* puerta2;
	ModeloRR* puertaA;
	ModeloRR* lampara;
	ModeloRR* jarron;
	ModeloRR* lapida_1;
	ModeloRR* lapida_2;
	ModeloRR* lapida_3;
	ModeloRR* lapida_4;
	ModeloRR* tronco; 
	ModeloRR* martillo;
	ModeloRR* pala;
	ModeloRR* tierra[16];
	ModeloRR* vehiculo;
	ModeloRR* Iglesia;
	ModeloRR* Casa;
	ModeloRR* arbolseco;

	ModeloRR* jugador;
	int tierraCorrecta = rand() % 15;

	float izqder;
	float arriaba;
	float vel;
	bool breakpoint;
	float posibilidades[16];

	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;

	float rotCam;
	int tipoCam;

	///////Jugabilidad
	bool Hacha = false;
	bool Martillo = false;
	bool Pala = false;
	bool Abierta_Cerrada = false;


    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		tipoCam = 1;
		rotCam = 0;
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
		billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0, 80, -140), D3DXVECTOR3(-0.65, 80, 0), D3DXVECTOR3(0, 1, 0), Ancho, Alto);
		terreno = new TerrenoRR(900, 900, d3dDevice, d3dContext);

		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"skydome_2.jpg");
		
		//AGUA
		lago = new LagoRR(L"Water_001_COLOR.jpg", L"Water_001_NORM.jpg", L"Water_001_DISP.png", d3dDevice, d3dContext, 4);


		//BILLBOARDS
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 6);
		hojas = new BillboardRR(L"Piso.png", L"Piso_normal.png", d3dDevice, d3dContext, 6);
		esqueleto = new BillboardRR(L"Bill.png", L"Bill_normal.png", d3dDevice, d3dContext, 6);
		arbol = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol1 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol2 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol3 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol4 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol5 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol6 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);
		arbol7 = new BillboardRR(L"Bill1.png", L"Bill1_normal.png", d3dDevice, d3dContext, 6);

		//MODELOS
		puerta = new ModeloRR(d3dDevice, d3dContext, "Assets/Porton/PORTON.obj", L"Assets/Porton/lambert2_Base_Color1.png", L"Assets/Porton/lambert2_Roughness.png", L"Assets/Porton/lambert2_Normal_OpenGL.png", 0, -100);
		puerta1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Porton/PUERTA1.obj", L"Assets/Porton/lambert2_Base_Color1.png", L"Assets/Porton/lambert2_Roughness.png", L"Assets/Porton/lambert2_Normal_OpenGL.png", 0, -100);
		puerta2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Porton/PUERTA2.obj", L"Assets/Porton/lambert2_Base_Color1.png", L"Assets/Porton/lambert2_Roughness.png", L"Assets/Porton/lambert2_Normal_OpenGL.png", 0, -100);
		puertaA = new ModeloRR(d3dDevice, d3dContext, "Assets/Porton/ABIERTA.obj", L"Assets/Porton/lambert2_Base_Color1.png", L"Assets/Porton/lambert2_Roughness.png", L"Assets/Porton/lambert2_Normal_OpenGL.png", 0, -100);
		pared = new ModeloRR(d3dDevice, d3dContext, "Assets/Paredes/Paredes.obj", L"Assets/Paredes/uigmaawg_2K_Albedo.jpg", L"Assets/Paredes/uigmaawg_2K_Roughness.jpg", L"Assets/Paredes/uigmaawg_2K_Normal.jpg", 0, -100);
		lampara = new ModeloRR(d3dDevice, d3dContext, "Assets/Lampara/Lampara.obj", L"Assets/Lampara/Lamp_BaseColor.png", L"Assets/Lampara/Lamp_Metallic.png", L"Assets/Lampara/Lamp_Normal.png", 0, -100);
		jarron = new ModeloRR(d3dDevice, d3dContext, "Assets/Jarron/Jarron.obj", L"Assets/Jarron/lambert1_Base_Color.png", L"Assets/Jarron/lambert1_Roughness.png", L"Assets/Jarron/lambert1_Normal.png", 0, -100);
		tronco = new ModeloRR(d3dDevice, d3dContext, "Assets/Tronco/troncos.obj", L"Assets/Tronco/T_L0001_basecolor.jpg", L"Assets/Tronco/T_L0001_roughness.jpg", L"Assets/Tronco/T_L0001_normal.jpg", 0, -100);
		lapida_1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Lapidas/Lapida1.obj", L"Assets/Lapidas/Lapida1_color.png", L"Assets/Lapidas/Lapida1_roughness.png", L"Assets/Lapidas/Lapida1_normal.png", 0, -100);
		lapida_2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Lapidas/Lapida2.obj", L"Assets/Lapidas/Lapida2_color.png", L"Assets/Lapidas/Lapida2_roughness.png", L"Assets/Lapidas/Lapida2_normal.png", 0, -100);
		lapida_3 = new ModeloRR(d3dDevice, d3dContext, "Assets/Lapidas/Lapida3.obj", L"Assets/Lapidas/Lapida3_color.png", L"Assets/Lapidas/Lapida3_roughness.png", L"Assets/Lapidas/Lapida3_normal.png", 0, -100);
		lapida_4 = new ModeloRR(d3dDevice, d3dContext, "Assets/Lapidas/Lapida4.obj", L"Assets/Lapidas/Lapida4_color.png", L"Assets/Lapidas/Lapida4_roughness.png", L"Assets/Lapidas/Lapida4_normal.png", 0, -100);
		martillo = new ModeloRR(d3dDevice, d3dContext, "Assets/Martillo/Martillo_mov.obj", L"Assets/Martillo/SurvivalKit_Hammer_Diffuse.png", L"Assets/Martillo/SurvivalKit_Hammer_Specular.png", L"Assets/Martillo/SurvivalKit_Hammer_Normal.png", 0, -100);
		pala = new ModeloRR(d3dDevice, d3dContext, "Assets/Pala/Pala.obj", L"Assets/Pala/Shovel_Mat_Base_Color.png", L"Assets/Pala/Shovel_Mat_Base_Color.png", L"Assets/Pala/Shovel_Mat_Normal_OpenGL.png", 0, -100);
		vehiculo = new ModeloRR(d3dDevice, d3dContext, "Assets/Bici/Bici.obj", L"Assets/Bici/Bici_albedo.jpg", L"Assets/Bici/Bici_specular_.jpg", L"Assets/Bici/bici_normal.png", 0, -140);
		Iglesia = new ModeloRR(d3dDevice, d3dContext, "Assets/Iglesia/igl.obj", L"Assets/Iglesia/chapel_diffuse.png", L"Assets/Iglesia/chapel_spec.png", L"Assets/Iglesia/chapel_normal.png", 0, -100);
		arbolseco = new ModeloRR(d3dDevice, d3dContext, "Assets/Arbol/arbol.obj", L"Assets/Arbol/tree_DefaultMaterial_BaseColor.png", L"Assets/Arbol/tree_DefaultMaterial_Roughness.png", L"Assets/Arbol/tree_DefaultMaterial_Normal.png", 0, -100);
		Casa = new ModeloRR(d3dDevice, d3dContext, "Assets/Casita/casa.obj", L"Assets/Casita/Diffuse.png", L"Assets/Casita/Diffuse.png", L"Assets/Casita/normal.png", 0, -100);

		//TIERRAS
		tierra[0] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra0.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[1] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra1.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[2] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra2.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[3] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra3.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[4] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra4.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[5] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra5.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[6] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra6.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[7] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra7.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[8] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra8.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[9] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra9.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[10] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra10.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[11] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra11.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[12] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra12.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[13] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra13.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[14] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra14.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);
		tierra[15] = new ModeloRR(d3dDevice, d3dContext, "Assets/Tierra/Tierra15.obj", L"Assets/Tierra/tierra.jpg", L"Assets/Tierra/tierra_rough.jpg", L"Assets/Tierra/tierra_normal.jpg", 0, -100);

		
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
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
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
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
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
		if(FAILED(result))
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
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);


		//Inicializar XACT para el audio

		bool res = m_XACT3.Initialize();
		if (!res) return false;
		res = m_XACT3.LoadWaveBank(L"Win\\WaveBank.xwb");
		if (!res) return false;
		res = m_XACT3.LoadSoundBank(L"Win\\SoundBank.xsb");
		if (!res) return false;

		//Reproducir

		cueIndex = m_XACT3.m_pSoundBank->GetCueIndex("Fondo");
		m_XACT3.m_pSoundBank->Play(cueIndex, 0, 0, 0);
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
		//Trabaja XACT
		m_XACT3.DoWork();
		
		if (breakpoint) {
			bool x = false;
		
		}

		rotCam += izqder;

		//pared de atras 125.72>

		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if (d3dContext == 0)
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
		d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 16;
		camara->posCam3p.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 20;
		camara->UpdateCam(vel, arriaba, izqder, tipoCam);


		//COLLISION WALL
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, camara->posCam.x, 133.0))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 252, camara->posCam.z))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -252, camara->posCam.z))) {
			camara->posCam = camara->posCampast;
		}
		/*if (isPointInsideSphere(camara->getpos(), getSphere2(5, camara->posCam.x, -100))) {
			camara->posCam = camara->posCampast;
		}*/
		//COLLISIONS
		if (camara->posCam.z < -87.0 && camara->posCam.z > -112.0) {
			if (!(camara->posCam.x < 10.8 && camara->posCam.x > -8)) {
				camara->posCam = camara->posCampast;
			}
		}
		#pragma region COLLISIONS
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 207.21, 103.3))) {
			camara->posCam = camara->posCampast;
		}

		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 215.45, -20.5))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 186.20, -24.4))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 28.1, -88.2))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 216.9, 20.22))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 155.4, -25.5))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 123.1, -24.9))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 216.8, 27.9))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 121.6, 58.5))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 121.1, 24.53))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 152.4, 22.23))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 185.5, 23.1))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 184.2, 90.46))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 218.0, 92.2))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 215.3, 62.6))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 185.9, 62.0))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 154.79, 59.28))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 211.6, -72.5))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 121.5, 89.3))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 152.3, 87.7))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 44.5, -79.15))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -52.5, 34.6))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -60.3, 78.6))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -51.5, 78.6))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -30.1, 114.5))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 28.7, 100.2))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(6, 25.3, 54.7))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 26.6, 1.6))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(6, 46.5, 100.2))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 85.6, -69.1))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -25.9, -85.1))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, 126.0, 22.6))) {
			camara->posCam = camara->posCampast;
		}
		if (isPointInsideSphere(camara->getpos(), getSphere2(5, -44.7, -83.7))) {
			camara->posCam = camara->posCampast;
		}
#pragma endregion 

		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();


		vehiculo->setPosX(camara->hdveo.x);
		vehiculo->setPosZ(camara->hdveo.z);
		vehiculo->Draw(camara->vista, camara->proyeccion,
			terreno->Superficie(vehiculo->getPosX(), vehiculo->getPosZ()) + 2.5,
			camara->posCam, 10.0f, rotCam, 'Y', 1, true, tipoCam);

		terreno->Draw(camara->vista, camara->proyeccion);


		//BILLBOARDS
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11, -78, 4, 5, uv1, uv2, uv3, uv4, frameBillboard, true, 0, 'A');
		
	
		static float zzz = 0;
		zzz += 0.01;
		esqueleto->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11, -78, 6, 12, uv1, uv2, uv3, uv4, 1, false, zzz);

		hojas->Draw(camara->vista, camara->proyeccion, camara->posCam,
			110, -78, 5, 10, uv1, uv2, uv3, uv4, 1, false, 1.57, 'Z');  


		arbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-100, -78, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol1->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-240, -27, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol2->Draw(camara->vista, camara->proyeccion, camara->posCam,
			130, -78, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol3->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-100, -200, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol4->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-170, -300, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol5->Draw(camara->vista, camara->proyeccion, camara->posCam,
			130, 113, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol6->Draw(camara->vista, camara->proyeccion, camara->posCam,
			224, 26, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		arbol7->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-70, 95, 1, 40, uv1, uv2, uv3, uv4, 1, false, 0, 'A');
		//AGUA
		TurnOnAlphaBlending();
		lago->Draw(camara->vista, camara->proyeccion, camara->posCam,
			185.5f, -71, 3.1, 30, uv1, uv2, uv3, uv4, 1.57, 'Z');
		TurnOffAlphaBlending();

		//MODELOS
		pared->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		puerta->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		puerta1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		puerta2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		puertaA->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		lampara->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		jarron->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tronco->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 250.0, 0, 'A', 1, false, tipoCam);
		lapida_1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		lapida_2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		lapida_3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		lapida_4->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		martillo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		pala->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		Iglesia->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 35.0f, 0, 'A', 1, false, tipoCam);
		Casa->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 35.0f, 0, 'A', 1, false, tipoCam);
		arbolseco->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 35.0f, 0, 'A', 1, false, tipoCam);
		//TIERRAS
		tierra[0]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[1]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[2]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[3]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[4]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[5]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[6]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[7]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[8]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[9]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[10]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[11]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[12]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[13]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[14]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[15]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);
		tierra[16]->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, false, tipoCam);

		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2]) {
			collition = true;
		}
			
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

	float sphere[3];
	float* getSphere2(float radio, float posx, float posz) {
		sphere[0] = posx;//pos obj
		sphere[1] = posz;//pos obj
		sphere[2] = radio;

		return sphere;
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