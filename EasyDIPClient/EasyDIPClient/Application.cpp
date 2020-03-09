#include "Application.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <WinBase.h>
#include <Commdlg.h>
#include <locale>
#include <codecvt>

using std::string;


std::string openfilename()
	{
		OPENFILENAME ofn;
		wchar_t fileName[MAX_PATH] = L"";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = L"All Files\0*.OBJ;*.OFF";
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = L"";
		std::string fileNameStr;

		if (GetOpenFileName(&ofn)){
			std::wstring aux (fileName);
			std::string str(aux.begin(), aux.end());
			fileNameStr = str;
		}

		return fileNameStr;
	}

//
//std::wstring openfilename(HWND owner = NULL) {
//	OPENFILENAME ofn;       // common dialog box structure
//	wchar_t szFile[260];       // buffer for file name
//	// HWND hwnd;              // owner window
//	HANDLE hf;              // file handle
//	// Initialize OPENFILENAME
//	ZeroMemory(&ofn, sizeof(ofn));
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = owner;
//	ofn.lpstrFile = szFile;
//	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
//	// use the contents of szFile to initialize itself.
//	ofn.lpstrFile[0] = '\0';
//	ofn.nMaxFile = sizeof(szFile);
//	//ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = NULL;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	// Display the Open dialog box. 
//
//	if (GetOpenFileName(&ofn) == TRUE)
//		hf = CreateFile(ofn.lpstrFile,
//			GENERIC_READ,
//			0,
//			(LPSECURITY_ATTRIBUTES)NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL,
//			(HANDLE)NULL);
//	return szFile;
//}

//"../../EasyDIPAPI/EasyDIPAPI/shaders/shader.frag"
//"C:/Users/heide/Desktop/ICG/[ICG] Tarea #3 - 24981800/ICGTarea3/EasyDIPAPI/EasyDIPAPI/shaders/shader.geom"
//"./../EasyDIPAPI/EasyDIPAPI/shaders/shader.geom"

extern Shader *bwShader;

const char* vertexPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader.vert";
const char* fragmentPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader.frag";
const char* geometryPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader.geom";

quat qRot = quat(1.f, 0.f, 0.f, 0.f);
mat4 modelMatrix;
static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
static float col1[4] = { 0.2f,0.3f,0.3f,1.0f};
static float col4[4] = { 0.0f,0.0f,0.0f,0.0f };
static float col3[4] = { 1.0f,1.0f,1.0f,1.0f };

static float vec4fs[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
static float vec4ft[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

bool Iwant_torotate = 0;
bool orthos;
glm::mat4 proj;
glm::mat4 orthogonal;
static int picked = -1;

std::vector <Mesh *> model;

Application::Application() {

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		__debugbreak();
		return;
	}

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	 window = glfwCreateWindow(800, 600, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	 if (window == NULL) {
		__debugbreak();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	bool err = gladLoadGL() == 0;

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		__debugbreak();
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	//CG::Model model = CG::Load("../Models/modelo.obj", );
	//CG::Load("C:/Users/heide/Desktop/ICG/cube.off");
	//CG::Load("./../Modelo/modelo.obj");
	//CG::Load("./../Modelo/modelo.off");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/Apple.off");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/dragon.off");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/teapot.off");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/seashell.off");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/LegoHead.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/icosahedron.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/Umbrella.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/mono.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/Oso.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/Batman.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/sphere.obj");
	//CG::Load("C:/Users/heide/Desktop/ICG/CasosDePrueba/files/Porsche_911_GT2.obj");
	//models.push_back(model);
	//	Scene.LoadModel("pelota.obj")

	//	Scene.add("pelota");

	//Scene.Save("escena_de_prueba");
	//Scene.Load("escena_de_prueba.graficas");
	//
	//void Scene::Load(std::string path) {
	//	LoadScene(path);
	//}

	// (optional) set browser properties
	//fileDialog.SetTitle("title");
	//fileDialog.SetTypeFilters({ ".jpg", ".png", ".jpeg" });
	Init();
}

Application::~Application() {

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	//delete img;
	//delete composite;
	//delete bw;
}


void Application::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(col1[0],col1[1],col1[2],col1[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		/*glLoadIdentity();
		if (orthos) {
			glOrtho(0.0f, windowWidth, windowHeight, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
		}
		glMatrixMode(GL_MODELVIEW);*/
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		
		

		ImGui();

		// Rendering
		ImGui::Render();
		Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}


void Application::Render()
{
	if (!orthos)
	{
		//Perspective
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-(float)windowWidth / 800.0f, (float)windowWidth / 800.0f, -(float)windowHeight / 800.0f, (float)windowHeight / 800.0f, NCP, 1000.0f);
	}
	else
	{
		//Orthogonal
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, NCP, 1000.0f);
	}
	if (model.size() > 0) {
		for (int i = 0; i < model.size(); i++)
		{
			if (bwShader) {
				bwShader->use();
				//glActiveTexture(0);
				//glBindTexture(GL_TEXTURE_2D, texId);
				//bwShader->setInt("tex", 0);
				//bwShader->setFloat("test", test);
				model[i]->Bind();
				model[i]->Draw();
				model[i]->setproj(proj);
				//model[i]->colormesh = glm::vec4(col3[0], col3[1], col3[2], col3[3]);
				//model[i]->colorpoints = glm::vec4(col4[0], col4[1], col4[2], col4[3]);
				//mesh->DrawNormals();
				//model[i]->colorrelleno = glm::vec4(col2[0], col2[1], col2[2], col2[3]);
				//bwShader->setVec4("my_color", glm::vec4(col2[0], col2[1], col2[2], col2[3]));
				//bwShader->setMat4("mModelView", modelMatrix);
				//bwShader->setMat4("projection", proj);
			}
		}
	}
	//Quad *quad = Quad::Instance();
	//if (bwShader) {
	//	bwShader->use();
	//	//glActiveTexture(0);
	//	//glBindTexture(GL_TEXTURE_2D, texId);
	//	//wShader->setInt("tex", 0);
	//	//bwShader->setFloat("test", test);
	//	quad->Bind();
	//	quad->Draw();
	//}
}


std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

void Application::ImGui()
{

	//ImGui::SliderFloat("test", &test, 0, 1);

	ImGui::Begin("Convolution Editor");
	if (ImGui::Button("Load"))
	{
		string a = openfilename();
		Mesh* mesh = new Mesh();
		mesh = CG::Load(a);
		model.push_back(mesh);
		picked = model.size() - 1;
		Iwant_torotate = false;
	}

	ImGui::Text("Backgound color button with Picker:");
	ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");
	ImGui::ColorEdit4("color 1", col1);

	ImGui::InputInt("Figured Picked", &picked);
	//ImGui::Text("Color button with Picker:");
	//ImGui::SameLine(); HelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");

	//static std::vector<const char*> names = {
	//	"0,0",
	//	"1,0",
	//	"2,0",
	//};

	//for (size_t yy = 0, nn = 0; yy < heightConv; yy++)
	//{
	//	for (size_t xx = 0; xx < widthConv; xx++, nn++)
	//	{
	//		ImGui::ColorEdit4(names[nn], (float*)&color[yy][xx], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	//		ImGui::SameLine();
	//	}
	//	ImGui::NewLine();
	//}


	ImGui::PushItemWidth(100);
	if (ImGui::DragFloat("Near Clipping Plane", &NCP, 0.01f));
	ImGui::PopItemWidth();

	if (texOGImg)
	{
		//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	}
	if (!model.empty() && picked<model.size())
	{
		//colors
		col2[0] = model[picked]->colorrelleno[0];
		col2[1] = model[picked]->colorrelleno[1];
		col2[2] = model[picked]->colorrelleno[2];
		col2[3] = model[picked]->colorrelleno[3];
		ImGui::Text("Color fill button with Picker:");
		ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");
		ImGui::ColorEdit4("color 2", col2);

		col4[0] = model[picked]->colorpoints[0];
		col4[1] = model[picked]->colorpoints[1];
		col4[2] = model[picked]->colorpoints[2];
		col4[3] = model[picked]->colorpoints[3];
		ImGui::Text("Color points button with Picker:");
		ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");
		ImGui::ColorEdit4("color 4", col4);

		col3[0] = model[picked]->colormesh[0];
		col3[1] = model[picked]->colormesh[1];
		col3[2] = model[picked]->colormesh[2];
		col3[3] = model[picked]->colormesh[3];
		ImGui::Text("Color wire-frame with Picker:");
		ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");
		ImGui::ColorEdit4("color 3", col3);

		model[picked]->colormesh = glm::vec4(col3[0], col3[1], col3[2], col3[3]);
		model[picked]->colorpoints = glm::vec4(col4[0], col4[1], col4[2], col4[3]);
		model[picked]->colorrelleno = glm::vec4(col2[0], col2[1], col2[2], col2[3]);

		//rotate
		modelMatrix = model[picked]->modelMatrix;
		ImGui::Text("Red is X, Green is Y, Blue is Z");
		ImGui::Checkbox("Rotate", &Iwant_torotate);
		if (Iwant_torotate)
		{
			qRot = model[picked]->Qrotacion;
			ImGui::gizmo3D("##gizmo1", qRot /*, size,  mode */);
		}
		model[picked]->Qrotacion = qRot;
		modelMatrix = mat4_cast(qRot);
		//Traslate, scale
		ImGui::Text("X, Y, Z");
		vec4fs[0] = model[picked]->vec4fscale[0];
		vec4fs[1] = model[picked]->vec4fscale[1];
		vec4fs[2] = model[picked]->vec4fscale[2];
		ImGui::DragFloat("SCALE X", &vec4fs[0], 0.01f);
		ImGui::DragFloat("SCALE Y", &vec4fs[1], 0.01f);
		ImGui::DragFloat("SCALE Z", &vec4fs[2], 0.01f);
		model[picked]->vec4fscale[0] = vec4fs[0];
		model[picked]->vec4fscale[1] = vec4fs[1];
		model[picked]->vec4fscale[2] = vec4fs[2];
		if (ImGui::DragFloat("SCALE ALL", &vec4fs[0], 0.01f, 0.0f))
		{
			vec4fs[1] = vec4fs[0];
			vec4fs[2] = vec4fs[0];
		}
		model[picked]->vec4fscale[0] = vec4fs[0];
		model[picked]->vec4fscale[1] = vec4fs[1];
		model[picked]->vec4fscale[2] = vec4fs[2];

		//ImGui::SliderFloat3("SCALE", vec4fs, 0.0f, 3.0f);
		ImGui::Text("X, Y, Z");
		vec4ft[0] = model[picked]->vec4ftraslate[0];
		vec4ft[1] = model[picked]->vec4ftraslate[1];
		vec4ft[2] = model[picked]->vec4ftraslate[2];
		ImGui::DragFloat("TRASLATE X", &vec4ft[0], 0.02f);
		ImGui::DragFloat("TRASLATE Y", &vec4ft[1], 0.02f);
		ImGui::DragFloat("TRASLATE Z", &vec4ft[2], 0.02f);
		model[picked]->vec4ftraslate[0] = vec4ft[0];
		model[picked]->vec4ftraslate[1] = vec4ft[1];
		model[picked]->vec4ftraslate[2] = vec4ft[2];
		//ImGui::SliderFloat3("TRASLATE", vec4ft, -2.0f, 2.0f, "ratio = %.01f");
		glm::vec3 auxs(vec4fs[0], vec4fs[1], vec4fs[2]);
		glm::vec3 auxt(vec4ft[0], vec4ft[1], vec4ft[2]);
		modelMatrix = glm::translate(modelMatrix, auxt);
		modelMatrix = glm::scale(modelMatrix, auxs);
		model[picked]->setmodelMatrix(modelMatrix);

		ImGui::Checkbox("Mallado", &model[picked]->mallado);
		ImGui::Checkbox("Puntos", &model[picked]->points);
		ImGui::Checkbox("Relleno", &model[picked]->relleno);
		ImGui::Checkbox("Back Face culling", &model[picked]->back_face_culling);
		ImGui::Checkbox("Ortho", &orthos);
		ImGui::Checkbox("Z-buffer", &model[picked]->zbuffer);
	}

	//if (ImGui::Button("recompile"))
	//{
	//	//delete bwShader;
	//	//std::string vert = Shader::GetSrcFromFile("bw.vert");
	//	//std::string frag = Shader::GetSrcFromFile("bw.frag");


	//	//bwShader = Shader::FromString(vert.c_str(), frag.c_str());
	//	//std::cout << "recompiled" << std::endl;

	//	//std::unique_ptr<RawData> negative{ EDNegativeHA(img->data, img->GetWidth(), img->GetHeight()) };
	//	//texId = GetTexture(negative.get(), img->GetWidth(), img->GetHeight());


	//}

	//if (ImGui::Button("Save Image"))
	//{
	//	ImGui::SameLine();
	//	//ImGui::InputText("imgFile")
	//}
	/*ImGui::FileBrowser fileDialog;
	if (ImGui::Button("Load Image"))
	{
		fileDialog.Open();
	}
		fileDialog.Display();


	if (fileDialog.HasSelected())
	{
		std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
		
		fileDialog.Close();
		fileDialog.ClearSelected();
	}*/


	ImGui::End();

}

void Application::Init() {
	//Shader mainShader(vertexPath, fragmentPath, nullptr);
	bwShader = new Shader(vertexPath, fragmentPath, geometryPath);
	//*bwShader = mainShader;
}

void Application::HelpMarker(const char* desc)
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
