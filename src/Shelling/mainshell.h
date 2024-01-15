#pragma once

#include <imgui.h>
#include <extracter.h>
#include <dataunit.h>
#include <parser.h>

#include <memory>

namespace Shell {

	class Shell
	{
	public:
		virtual void OnUpdate() = 0;

		virtual void SetPos(float x, float y) { m_posx = x; m_posy = y; }
		virtual void SetScreen(float width, float height) { m_width = width; m_height = height; }
		virtual void SetActiveWindow(bool showwindow) { m_showWindow = showwindow; }

		virtual bool& GetActive() { return m_showWindow; }
		virtual std::string& GetLabel() { return m_label; }
		
	protected:
		virtual void UpdateWrap() = 0;
		bool m_showWindow;

		unsigned int m_width;
		unsigned int m_height;

		float m_posx, m_posy;

		std::string m_label;
		
		Shell(std::string label, unsigned int width, unsigned int height, bool sw)
			: m_label(label), m_width(width), m_height(height), m_showWindow(sw) {}
		virtual ~Shell() = default;
	};

	struct ImGuiTalkBuffer
	{
		inline static std::unique_ptr<File::Extracter> file;
		inline static std::unique_ptr<File::Parser> parser;
		inline static std::vector<Shell*>* windows;
		
		inline static Database::Intersort data;
				
		inline static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchSame;
	};
	
}
