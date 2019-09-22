#include "Globals.h"
#include "ModuleUI.h"
#include "Application.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleUI::~ModuleUI()
{}


bool ModuleUI::Start()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	  // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	return true;

}

bool ModuleUI::CleanUp()
{
	LOG("Unloading UI module");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

update_status ModuleUI::Update(float dt)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if(demoWindow)
	ImGui::ShowDemoWindow();

	if(stylewindow)
	ImGui::ShowStyleEditor();


	

		if (ImGui::BeginMainMenuBar())
		{
			//Begin Menu
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Close")) { App->close_app = true; }
				//TODO:/ WE NEED TO PUT HERE THE DIFFERENT OPTIONS OF THE MENU, AS OPEN, SAVE, QUIT...

				if (ImGui::MenuItem("Floating window"))
				{
					if (!show_another_window)
						show_another_window = true;
					else show_another_window = false;
						

					
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Examples"))
			{
				if (ImGui::MenuItem("DemoWindow"))
				{
					if (!demoWindow)
						demoWindow = true;
					else demoWindow = false;
				}
				if (ImGui::MenuItem("StyleEditor"))
				{
					if (!stylewindow)
						stylewindow = true;
					else stylewindow = false;

				}
					ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (show_another_window)
		{
			ImGui::Begin("ImGui Test", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		//Text
			ImGui::Text("ImGui says hello");

			ImGui::SameLine(ImGui::GetWindowSize().x - 30);
			HelpMarker("Here god will\n help you");
			//Headers with options
			if (ImGui::CollapsingHeader("Help"))
			{
				HelpMarker("Here god will\n help you");
			}



			if (ImGui::CollapsingHeader("Window options"))
			{

			}

			if (ImGui::CollapsingHeader("Widgets"))
			{
				//Sub menus
				if (ImGui::TreeNode("Basic"))
				{

					//button
					static int clicked = 0;
					if (ImGui::Button("Button"))
						clicked++;
					if (clicked & 1)
					{
						ImGui::SameLine();
						ImGui::Text("Thanks for clicking me!");
					}

					//checkbox
					static bool checkBox = true;
					ImGui::Checkbox("checkbox", &checkBox);

					//circular checkbox
					static int radio = 0;
					ImGui::RadioButton("radio a", &radio, 0); ImGui::SameLine();
					ImGui::RadioButton("radio b", &radio, 1); ImGui::SameLine();
					ImGui::RadioButton("radio c", &radio, 2);

					//color buttons with text and states
					for (int i = 0; i < 7; i++)
					{
						if (i > 0)
							ImGui::SameLine();
						ImGui::PushID(i);
						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
						ImGui::Button("Click");
						ImGui::PopStyleColor(3);
						ImGui::PopID();
					}

					//Text with tooltips
					ImGui::Text("Hover over me");
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("I am a tooltip");
					ImGui::SameLine();
					ImGui::Text("- or me");
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("I am a fancy tooltip");
						static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
						ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
						ImGui::EndTooltip();
					}

					//text separator
					ImGui::Separator();

					ImGui::LabelText("label", "Value");

					//It shows differents options :)
					{
						const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
						static int item_current = 0;
						ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
					}

					{
						//Input text
						static char str0[128] = "Hello, world!";
						if (ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0)))
						{
							io.WantCaptureKeyboard = false;
							/*ImGui::CaptureKeyboardFromApp(true);
							io.WantCaptureKeyboard = true;*/
							if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
							{
								io.WantCaptureKeyboard = true;
								ImGui::SetKeyboardFocusHere();
							}


						}
						//Input Int
						static int i0 = 123;
						ImGui::InputInt("input int", &i0);

						//Input float
						static float f0 = 0.001f;
						ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

						//3 Input floats
						static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
						ImGui::InputFloat3("input float3", vec4a);

						//Drag Int
						static int i1 = 50, i2 = 42;
						ImGui::DragInt("drag int", &i1, 1);

						ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%");

						//Drag float
						static float f1 = 1.00f, f2 = 0.0067f;
						ImGui::DragFloat("drag float", &f1, 0.005f);
						ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");

						//Slider int
						static int i3 = 0;
						ImGui::SliderInt("slider int", &i3, -1, 3);

						//Slider float
						static float f3 = 0.123f, f4 = 0.0f;
						ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
						ImGui::SliderFloat("slider log float", &f2, -10.0f, 10.0f, "%.4f", 2.0f);

						//Slider angle
						static float angle = 0.0f;
						ImGui::SliderAngle("slider angle", &angle);

						//Color editor
						static float col1[3] = { 1.0f,0.0f,0.2f };
						static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
						ImGui::ColorEdit3("color 1", col1);
						ImGui::ColorEdit4("color 2", col2);
					}



					ImGui::TreePop();
				}


				if (ImGui::TreeNode("Tree"))
				{
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Bullets"))
				{
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Colored Text"))
				{
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Word Wrapping"))
				{
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("UTF-8 Text"))
				{
					ImGui::TreePop();

				}
				if (ImGui::TreeNode("Images"))
				{
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Selectables"))
				{
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Filtered Text Input"))
				{
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Multi-line Text Input"))
				{
					ImGui::TreePop();
				}



			}

			//Button
			if (ImGui::Button("Close Me"))
				show_another_window = false;


			ImGui::End();
		}
	ImGui::Render();

	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}



void ModuleUI::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}