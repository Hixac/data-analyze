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

	protected:
		Shell() = default;
		virtual ~Shell() = default;
	};

	struct ImGuiTalkBuffer
	{
		inline static std::unique_ptr<File::Extracter> file;
		inline static std::unique_ptr<File::Parser> parser;
		
		inline static Database::Intersort data;
						
		inline static Database::Object bufferObject;
		
		inline static std::string bufferName;
		inline static std::string bufferValue;
				
		inline static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchSame;
		inline static int bufferColumns = 1;
		inline static unsigned int currentName = 0;
		inline static float winCheckSize = 100;
	};
	
}
