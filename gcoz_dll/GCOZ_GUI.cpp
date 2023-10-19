#include "GCOZ_GUI.h"

LRESULT CALLBACK hkWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
		return true;
	}
	return ::CallWindowProcA(oWndProc, hwnd, uMsg, wParam, lParam);
}

void GUI::init(IDXGISwapChain* pSwapChain) {
	DXGI_SWAP_CHAIN_DESC desc;
	pSwapChain->GetDesc(&desc);

	ID3D11Device* device;
	pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

	ID3D11DeviceContext* context;
	device->GetImmediateContext(&context);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	oWndProc = (WNDPROC) ::SetWindowLongPtr(desc.OutputWindow, GWLP_WNDPROC, (LONG_PTR)hkWindowProc); // changed to (LONG_PTR) from example

	ImGui_ImplWin32_Init(desc.OutputWindow);
	ImGui_ImplDX11_Init(device, context);
}

int getFPS() {

    ImGuiIO& IO = ImGui::GetIO();
    static std::chrono::steady_clock clock;
    static std::chrono::steady_clock::time_point lastCall;
    static float value = 0.;
    static bool firstCall = true;

    if (firstCall) { lastCall = clock.now(); value = IO.Framerate; firstCall = false; }
    if (clock.now() - lastCall > std::chrono::milliseconds(500)) {
        lastCall = clock.now();
        value = IO.Framerate;
    }

    return static_cast<int>(value);
}

void GUI::showGCOZgui(ProfilerStatusManager _man, std::string _debugMsg) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Create a window
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("GCOZ");
    ImGui::SetWindowFontScale(1.1f);

    /// TODO functions to build different windows depending on status

    // Create a tab bar
    if (ImGui::BeginTabBar("")) {

        // Status tab
        if (ImGui::BeginTabItem("Overview")) {
            std::string status = "Current Status: " + profilerStatusString(_man.getStatus());
            std::string method = "Current Method: " + std::to_string(_man.getMethod());
            std::string framerate = "FPS: " + std::to_string(getFPS());
            ImGui::Text(status.c_str());
            ImGui::Text(method.c_str());
            ImGui::Text(framerate.c_str());
            ImGui::EndTabItem();
        }

        // Debug tab
        if (ImGui::BeginTabItem("Debug")) {
            std::string debugMsg = _debugMsg;
            ImGui::Text(debugMsg.c_str());
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End(); // Close the window

    // Render ImGui
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
