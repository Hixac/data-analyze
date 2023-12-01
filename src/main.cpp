#include <chrono>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <window.h>
#include <spdlog/spdlog.h>

#define DEBUG
#include <Base.h>

#include <imgui/fastcombo.h>
#include <parser.h>
#include <imgui/dynamictimer.h>
#include <imgui/deltatime.h>

void CreateTableInput(std::string& buffer, size_t id)
{
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::PushID(id);
	ImGui::InputText("##cell", &buffer);
	ImGui::PopID();
}

void LabelOnLeft(const char* name, bool left = false)
{
	ImGui::Text("%s", name);
	if (left) ImGui::SameLine();
}

template<typename __first, typename __last>
std::vector<__first> GetFirstOfPairs(std::vector<std::pair<__first, __last>> data)
{
	std::vector<std::string> firsts;
	for (auto unit : data)
	{
		firsts.push_back(unit.first);
	}

	return firsts;
}

int main(void)
{
	INIT_LOG();

	/* PARKING WINDOW VARIABLES */
	
	ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchSame;
	File::Extracter file("db.rot");
	File::Parser parser(file);
	Database::Intersort data = parser.Process();

	Database::Object bufferObject;
    int bufferColumns = 1;
	
	std::string bufferName;
	std::string bufferValue;
	
	unsigned int current_name = 0;

    float winCheckSize = 100;

  	Window win(1280, 720, "Парковка");

	win.GetWindowIO().Fonts->AddFontFromFileTTF("../res/JetBrainsMonoNLNerdFontMono-Regular.ttf", 25);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

	while (win.StartUpdate())
    {	
		ImGui::Begin("Parking", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
					 | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({win.GetWindowIO().DisplaySize.x / 2, win.GetWindowIO().DisplaySize.y - winCheckSize});
		ImGui::SetWindowPos({0, 0});

	    if (ImGui::BeginMenuBar())
		{
			MyGui::SuicideCommand commando;
			
			if (ImGui::BeginMenu("Add object"))
			{
			    LabelOnLeft("Object");
				ImGui::InputText("##Object", &bufferObject.first);
			    LabelOnLeft("Columns");
				ImGui::SliderInt("##Columns", &bufferColumns, 1, 100);

				if (bufferObject.second.size() > bufferColumns)
					bufferObject.second.clear();
				
				for (size_t i = 0; bufferObject.second.size() < bufferColumns && i < bufferColumns; i++)
					bufferObject.second.push_back({});
				
				if (ImGui::BeginTable("Example", 2, tableFlags))
				{
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Value");
					ImGui::TableHeadersRow();

					ImGui::TableNextColumn();
					for (size_t i = 0; i < bufferColumns; i++)
					{
						CreateTableInput(bufferObject.second[i].name, i);
					}

					ImGui::TableNextColumn();
					for (size_t i = 0; i < bufferColumns; i++)
					{
						CreateTableInput(bufferObject.second[i].value, i+100);
					}
				
					ImGui::EndTable();
				}

				if (ImGui::Button("Commit") && !bufferObject.first.empty() && !data.FindCode(bufferObject.first))
				{
					data.Add(bufferObject.first, bufferObject.second);
				}
				
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Delete object"))
			{
			    const char* preview_val = data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
				LabelOnLeft("Listing");
				MyGui::FastCombo("##Object listing", names, current_name, preview_val);
				if (ImGui::Button("Commit"))
				{
					data.Delete(preview_val);
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Add Change"))
			{
				const char* preview_val = data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
				LabelOnLeft("Listing");
				MyGui::FastCombo("Object listing", names, current_name, preview_val);

				LabelOnLeft("Name");
				ImGui::InputText("##Name", &bufferName);
				LabelOnLeft("Value");
				ImGui::InputText("##Value", &bufferValue);
				if (ImGui::Button("Commit") && !bufferName.empty() && !bufferValue.empty())
				{
					data.AddIntoExisting(data.GetObjects()[current_name].first, {bufferName, bufferValue});
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Delete"))
			{
				const char* preview_val = data.GetObjects()[current_name].first.c_str();
				auto names = GetFirstOfPairs<std::string, std::vector<Database::Dataunit>>(data.GetObjects());
				LabelOnLeft("Listing");
				MyGui::FastCombo("##Object listing", names, current_name, preview_val);

				LabelOnLeft("Name");
				ImGui::InputText("##Name", &bufferName);
				if (ImGui::Button("Commit") && !bufferName.empty())
				{
					data.Delete(data.GetObjects()[current_name].first, bufferName);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		for (auto& object : data.GetObjects())
		{
			ImGui::Text("%s", object.first.c_str());
			if (ImGui::BeginTable("Data", 2, tableFlags))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Value");
				ImGui::TableHeadersRow();

				ImGui::TableNextColumn();
				for (size_t i = 0; i < object.second.size(); i++)
				{
					CreateTableInput(object.second[i].name, i);
				}

				ImGui::TableNextColumn();
				for (size_t i = 0; i < object.second.size(); i++)
				{
					CreateTableInput(object.second[i].value, i+1000);
				}
				
				ImGui::EndTable();
			}
		}
		
		parser.BackProcess(data);
		
		ImGui::End();

		/* EXAMPLE WINDOW WITH TEXT EDITING */		
		ImGui::Begin("Example", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({win.GetWindowIO().DisplaySize.x / 2, win.GetWindowIO().DisplaySize.y - winCheckSize});
		ImGui::SetWindowPos({win.GetWindowIO().DisplaySize.x / 2, 0});

		std::string text = file.GetContent();
		ImGui::InputTextMultiline("##Example", &text, {win.GetWindowIO().DisplaySize.x / 2, win.GetWindowIO().DisplaySize.y - 100});
		file.SetContent(text);

		data = parser.Process();
		
		ImGui::End();

		ImGui::Begin("Check", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize({win.GetWindowIO().DisplaySize.x, winCheckSize });
		ImGui::SetWindowPos({0, win.GetWindowIO().DisplaySize.y - winCheckSize });
		
		static std::string check = "";
		static bool testFlag = false;
		if (ImGui::Button("Check"))
		{
			MyGui::DynamicTimer::Get().start(3);
		}
		
		if (MyGui::DynamicTimer::Get().update())
		{
			testFlag = true;
		}
		
		ImGui::Checkbox("##flag", &testFlag);
		ImGui::SameLine();
		ImGui::Checkbox("##flag1", &testFlag);
		ImGui::SameLine();
		ImGui::Checkbox("##flag2", &testFlag);
		ImGui::SameLine();
		ImGui::Checkbox("##flag3", &testFlag);
		ImGui::SameLine(); 
		
		ImGui::Text("%s", check.c_str()); ImGui::SameLine();
		ImGui::Text("dtime: %f", MyGui::BasicInformation::deltatime);
		
		ImGui::End();
		
        win.EndUpdate();
    }

	Window::StopIMGUI();

    return 0;
}
