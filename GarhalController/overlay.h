#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "InputSystem.h"
#include <d3d11.h>
#include <d3dx11tex.h>
#pragma comment(lib, "D3DX11.lib")
#include <dinput.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <map>
#include <dwmapi.h>
#include <fstream>
#include <ShlObj.h>
#include "icons.h"

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "d3d11.lib") 
#pragma comment(lib, "dwmapi.lib")

class OverlayRender
{
public:
	virtual void display() {}
	virtual BOOL useVSync() { return FALSE; }

	/* DirectX Pointers */
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
	ID3D11BlendState* g_pBlendState = NULL;
	InputSystem* g_pInputSystem = new InputSystem();
	

	/* Menu Colors */
	ImVec4 TabSlider = { 0.929f, 0.290f, 0.290f, 1.00f };
	ImVec4 TabTitleText = { 0.929f, 0.290f, 0.290f, 1.00f };
	ImVec4 RedText = { 1.f, 0.0f, 0.0f, .95f };
	ImVec4 ActiveButton = { 0.917f, 0.223f, 0.223f, 1.f };
	ImVec4 GreenText = { 0.f, 1.0f, 0.0f, .95f };
	ImVec4 Underline = { 0.f, 0.749f, 1.f, 1.00f };
	ImVec4 ActiveBut = { 0.2f, 0.2f, 0.231f, 1.00f };
	ImVec4 RegButton = { 0.137f, 0.137f, 0.160f, 1.00f };

	/* Fonts*/
	std::map<int, ImFont*> mapFont;
	ImFont* Icons = NULL;

	/* Handle*/
	WNDCLASSEX wc = { NULL };

	int xScreen, yScreen;
	D3DX11_IMAGE_LOAD_INFO info;
	ID3D11ShaderResourceView* LogoTexture = NULL;

	/* Keyboard */
	SHORT KeyStates[256];
	

	void render(const std::vector<RenderData>& renderData)
	{
		beginDraw();
		renderAll(renderData);
		renderMenu();
		endDraw();	
	}

	float getFactorFromDistance(float distance, bool scale)
	{
		if (distance > 400.f || !scale)
			return 0.f;

		return powf(1 - pow(distance / 400.f, 0.3f), 3.f);
	}

	void PollSystem()
	{
		ImGuiIO& io = ImGui::GetIO();
		int mousex, mousey;

		g_pInputSystem->UpdateMousePos(0, 0);
		g_pInputSystem->GetMousePos(mousex, mousey);
		g_pInputSystem->PollInputState(true);

		io.MousePos.x = (float)mousex;
		io.MousePos.y = (float)mousey;

		for (int i = 0; i < 256; ++i)
		{
			KeyStates[i] = GetAsyncKeyState(i);			
		}

		bool shift_held = false; 
		for (int i = 0; i < 256; i++)
		{
			if ((KeyStates[i] & 0x8000)) //key is down
			{
				io.KeysDown[i] = true;
			}
			else if (io.KeysDown[i])
				io.KeysDown[i] = false;

			if (KeyStates[0xA0] & 0x8000)
				shift_held = true;

			if ((KeyStates[i] & 0x1)) // key was pressed
			{
				io.KeysPressed[i] = true;				
				/*
					This is a bad way of input text
					Ideally need to write a function that
					1) Checks if control keys are pressed (arrows left/right/up/down, enter, backspace, etc) and exclude them
					2) Check if shift is pressed (0xA0 & 0x8000) and input corresponding characters (like "shift + 8" = "*")
				*/
				io.InputQueueCharacters.push_back(i);

			}
			else
				io.KeysPressed[i] = false;
		}
		
		if (g_pInputSystem->InputStates[LBUTTON_DOWN] == true)
		{
			io.MouseDown[0] = true;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		
		if (g_pInputSystem->InputStates[LBUTTON_CLICKED] == true)
		{
			io.MouseClicked[0] = true;
		}
		else
		{
			io.MouseClicked[0] = false;
		}
	}

	void CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
		pBackBuffer->Release();
	}

	bool CreateDeviceD3D(HWND hWnd)
	{
		// setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 300;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Here is a very nice example to select an adapter. Not necessarily needed, but
		// I provide a commented version.
		// Prepare a factory
		/*HRESULT result = E_FAIL;
		IDXGIFactory* factory = nullptr;
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			MessageBox(0, "Failed to create factory", "", MB_OK | MB_ICONHAND);
			return false;
		}
		
		// Find our primary GPU
		struct GPUData
		{
			DXGI_ADAPTER_DESC dxgiAdapterDesc;
			IDXGIAdapter* dxgiAdapter;
		};
		
		IDXGIAdapter* chosenAdapter = nullptr;
		int adapterIndex = 0;
		std::vector<GPUData> adapters;
		
		while (factory->EnumAdapters(adapterIndex, &chosenAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC adapterDesc;
			chosenAdapter->GetDesc(&adapterDesc);
			GPUData data = {.dxgiAdapterDesc = adapterDesc, .dxgiAdapter = chosenAdapter};
			adapters.push_back(data);
			++adapterIndex;
		}

		// Release the factory.
		factory->Release();

		DXGI_ADAPTER_DESC chosenAdapterDesc = adapters[0].dxgiAdapterDesc;
		chosenAdapter = adapters[0].dxgiAdapter;

		const std::wstring nvidia = L"NVIDIA";
		const std::wstring ati = L"ATI";

		int memory = (int)(chosenAdapterDesc.DedicatedVideoMemory / 1024 / 1024);
		int mostMem = 0;

		// Then after finding the correct data, you use chosenAdapter according to the
		// MS docs, utilizing D3D11CreateDeviceAndSwapChain with the current arguments.
		for (GPUData& adapter : adapters)
		{
			DXGI_ADAPTER_DESC desc = adapter.dxgiAdapterDesc;
			memory = (int)(desc.DedicatedVideoMemory / 1024 / 1024);
			std::wstring name = desc.Description;
			if (name.find(nvidia) != std::wstring::npos || name.find(ati) != std::wstring::npos)
			{
				if (memory > mostMem)
				{
					mostMem = memory;
					chosenAdapterDesc = desc;
					chosenAdapter = adapter.dxgiAdapter;
				}
			}
		}*/

		UINT createDeviceFlags = 0;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
			return false;

		CreateRenderTarget();
 		return true;
	}

	/* Functions */
	void beginDraw()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	}

	void endDraw()
	{
		// Rendering
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		ImGui::Render();			
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		g_pSwapChain->Present(useVSync(), 0);

		// A little bit dirty, but D3DX11CreateShaderResourceVievFromMemory() only works after ImGui::Render() in this project
		// Reason: Debug imgui.cpp, line 10917, ImeSetInputScreenPosFn_DefaultImpl()
		// Ideally you would call it in void init()
		if (LogoTexture == nullptr)
			D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, &GarhalLogo, sizeof(GarhalLogo), &info, NULL, &LogoTexture, 0);
	}

	bool isInScreen(const ImVec2& vec)
	{
		return fabs(vec.x) <= xScreen && fabs(vec.y) <= yScreen;
	}

	void renderMenu()
	{
		ImGui::PushFont(mapFont[12]);
		display();
		ImGui::PopFont();
	}

	virtual void renderAll(const std::vector<RenderData>& ds)
	{
		if (ds.empty())
			return;
		
		// Style
		auto& style = ImGui::GetStyle();

		//style.AntiAliasedFill = FALSE;
		//style.AntiAliasedLines = TRUE;

		// Menu
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(xScreen, yScreen));
		static const auto dwFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoClipping;
		ImGui::Begin("o", 0, dwFlags);
		{
			ImDrawList* draw = ImGui::GetWindowDrawList();

			for (RenderData p : ds)
			{
				if (p.inGameDistance == 0)
				{
					continue;
				}
				
				if (p.x == p.y && p.x == 0)
				{
					continue;
				}
				if (p.x < 0 || p.y < 0)
				{
					continue;
				}

				if (p.render)
				{
					float radius = 8;
					float minX = p.x - 5;
					float maxX = p.x;
					float minY = p.y - 10;
					float maxY = p.y + 10;
					ImU32 color = ImGui::GetColorU32(p.color);
					
					// TODO: Implement name for yourself
					//ImVec2 textSize = ImGui::CalcTextSize(p.name);
					int fontSize = (int)(12 + 6 * getFactorFromDistance(p.inGameDistance, p.scale));

					//ImGui::PushFont(mapFont[fontSize]);
					ImVec2 textSize = ImGui::CalcTextSize(p.name);
					//ImGui::PopFont();
					
					draw->AddText(mapFont[fontSize], fontSize, ImVec2(p.x - textSize.x / 2,
						p.y - 1.5 * textSize.y - 2 * radius * getFactorFromDistance(p.inGameDistance, p.scale)), color, p.name);

					
					// Bone render
					for (const std::pair<ImVec2, ImVec2>& bonePair : p.bones)
					{
						const ImVec2& firstBone = bonePair.first;
						const ImVec2& secondBone = bonePair.second;										
						if (!isInScreen(firstBone) || !isInScreen(secondBone))
							continue;
						if (firstBone.x == xScreen / 2 || firstBone.y == yScreen / 2)
							continue;
						if (secondBone.x == xScreen / 2 || secondBone.y == yScreen / 2)
							continue;
						if (firstBone.x < 0 || firstBone.y < 0)
							continue;
						if (secondBone.x < 0 || secondBone.y < 0)
							continue;

						for (const auto& bone : { firstBone, secondBone })
						{
							minX = min(bone.x, minX);
							minY = min(bone.y, minY);
							maxX = max(bone.x, maxX);
							maxY = max(bone.y, maxY);
						}

						draw->AddLine(
							ImVec2(firstBone.x, firstBone.y),
							ImVec2(secondBone.x, secondBone.y),
							color,
							1.f);


					}
				}
			}
		}
		ImGui::End();
	}

	virtual void init()
	{
		/* Colors */
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);


		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		TCHAR szPath[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_FONTS, NULL, NULL, szPath);

		std::string Path = szPath;
		std::string pArialBD{Path + "\\cambriab.ttf"};
		
		for (auto size = 11; size < 31; ++size)
		{
			float SCALE = 2.0f;
			ImFontConfig cfg;
			cfg.SizePixels = size * SCALE;
			//cfg.GlyphOffset.y = SCALE;

			ImFont* font = io.Fonts->AddFontFromFileTTF(pArialBD.c_str(), size, &cfg, io.Fonts->GetGlyphRangesDefault());
			mapFont[size] = font;
		}

		// * setup style * //
		ImGui::InitStyle();

		// * setup colors * //
		ImGui::InitColors();
	}
	
	bool setHWNDAttributes(HWND win)
	{
		MARGINS margin = { -1 };

		RECT rect;
		GetWindowRect(win, &rect);
		xScreen = rect.right;
		yScreen = rect.bottom;

		SetMenu(win, NULL);
		SetWindowLongPtr(win, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS);
		SetWindowLongPtr(win, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
		ShowWindow(win, SW_SHOW);

		SetLayeredWindowAttributes(win, 0, 255, LWA_ALPHA);
		DwmExtendFrameIntoClientArea(win, &margin);

		if (!CreateDeviceD3D(win))
		{
			MessageBox(0, "Failed to create CreateDeviceD3D.", "", MB_OK | MB_ICONHAND);
			return false;
		}

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(win);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
		init();

		ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.00f);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
		SetWindowPos(win, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		return true;
	}

	BOOL createWindow()
	{
		ImmDisableIME(0);

		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX), NULL, DefWindowProc, 0L, 0L, GetModuleHandle(NULL),
			NULL, NULL, NULL, NULL, _T("IMM"), NULL
		};

		RegisterClassEx(&wc);

		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		
		HWND hwnd = CreateWindowEx(WS_EX_LAYERED, wc.lpszClassName, _T("Default IME"),
			WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT, rect.left, rect.top,
			rect.right, rect.bottom, NULL, NULL, GetModuleHandle(NULL), NULL);

		if (hwnd == NULL)
			return false;
		
		if (!this->setHWNDAttributes(hwnd))
		{
			return false;
		}
		
		Sleep(1000);

#ifndef _DEBUG
		ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
		
		return true;
	}
};