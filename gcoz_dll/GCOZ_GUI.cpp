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

void GUI::showGCOZgui(ProfilerStatusManager _man, std::string _debugMsg) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(250, 150));

    // Create a window
    ImGui::Begin("GCOZ");

    // Create a tab bar
    if (ImGui::BeginTabBar("")) {

        // Status tab
        if (ImGui::BeginTabItem("Overview")) {
            std::string status = "Current Status: " + profilerStatusString(_man.getStatus());
            std::string method = "Current Method: " + std::to_string(_man.getMethod());
            ImGui::Text(status.c_str());
            ImGui::Text(method.c_str());
            ImGui::EndTabItem();
        }

        // Debug tab
        if (ImGui::BeginTabItem("Debug")) {
            std::string debugMsg = _debugMsg;
            ImGui::Text(debugMsg.c_str());
            ImGui::EndTabItem();
        }

  
        // ImGui tab
        //if (ImGui::BeginTabItem("ImGui Settings")) {

        //    // get imgui structs for style and whatnot
        //    ImGuiStyle style = ImGui::GetStyle();
        //    ImGuiIO io = ImGui::GetIO();
        //    ImGui::Text("Default Window Size: %.1f x %.1f", io.DisplaySize.x, io.DisplaySize.y);
        //    //ImGui::Text("Default Window Position: %.1f, %.1f", io.DisplayPos.x, io.DisplayPos.y);
        //    ImGui::Text("Default Font: %s", io.Fonts->Fonts[0]->GetDebugName());
        //    ImGui::Text("Default Font Size: %.1f", io.FontGlobalScale);
        //    ImGui::Text("Default Style WindowRounding: %.1f", style.WindowRounding);
        //    ImGui::Text("Default Style FrameRounding: %.1f", style.FrameRounding);
        //    //ImGui::Text("Default Tooltip Display Time: %.2f", io.ConfigTooltipDisplayTime);

        //    ImGui::EndTabItem();
        //}

        ImGui::EndTabBar();
    }

    ImGui::End(); // Close the window

    // Render ImGui
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
