#pragma once

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

class Window
{
public:
	Window(unsigned int width, unsigned int height, const char* title);
	~Window();
	
	static void StopIMGUI();

	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }

	inline ImGuiIO& GetWindowIO() { return ImGui::GetIO(); }
	
	bool StartUpdate();
    void EndUpdate();
	bool IsClosed();
private:
    GLFWwindow* m_pWindow;
	
	unsigned int m_Height;
	unsigned int m_Width;
};
