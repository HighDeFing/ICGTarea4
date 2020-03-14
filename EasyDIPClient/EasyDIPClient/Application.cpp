#include "Application.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <WinBase.h>
#include <Commdlg.h>
#include <locale>
#include <codecvt>

using std::string;
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

std::string openfilenametexture()
{
	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"All Files\0*.png;*.jpg";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";
	std::string fileNameStr;

	if (GetOpenFileName(&ofn)) {
		std::wstring aux(fileName);
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
Shader *lampShader;

const char* vertexPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader.vert";
const char* fragmentPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader.frag";
const char* geometryPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader.geom";

const char* vertex_LightPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader_light.vert";
const char* fragment_LightPath = "./../EasyDIPAPI/EasyDIPAPI/shaders/shader_light.frag";

quat qRot = quat(1.f, 0.f, 0.f, 0.f);
mat4 modelMatrix;
mat4 light_modelMatrix = glm::mat4(1.0f);
mat4 viewMatrix;

//light1 bool
bool only_one_light1 =  false;

static float light_col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
static float col1[4] = { 0.2f,0.3f,0.3f,1.0f};
static float col4[4] = { 0.0f,0.0f,0.0f,0.0f };
static float col3[4] = { 1.0f,1.0f,1.0f,1.0f };

static float vec4fs[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
static float vec4ft[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

static float light_vec4fs[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
static float light_vec4ft[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

bool Iwant_torotate = 0;
bool orthos;
glm::mat4 proj;
glm::mat4 orthogonal;
static int picked = -1;
static int light_picked = -1;

std::vector <Mesh *> model;
Light *lights;

//CAMARA
glm::mat4 view;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//MOUSE
bool firstMouse = true;
float yaw1 = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch1 = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

bool zbuffer = false;
bool move;

//INTENSIDAD DE LUZ
static float intensity_ambiental = 0.8f;
static float intensity_specular = 0.8f;

//TEXTURE MODE
int texture_mode = 0;

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

		//Demo
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// Get mouse position
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ImGui();
		processInput();
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		if (move) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		// Rendering
		ImGui::Render();
		Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	if (move) {
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw1 += xoffset;
		pitch1 += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch1 > 89.0f)
			pitch1 = 89.0f;
		if (pitch1 < -89.0f)
			pitch1 = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw1)) * cos(glm::radians(pitch1));
		front.y = sin(glm::radians(pitch1));
		front.z = sin(glm::radians(yaw1)) * cos(glm::radians(pitch1));
		cameraFront = glm::normalize(front);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (move) {
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}
}

void Application::processInput()
{

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		firstMouse = true;
		move = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		move = false;


}

void Application::Render()
{
	if (orthos)
	{
		//Orthogonal
		proj = glm::mat4(1.0f);
		proj = glm::ortho(-(float)windowWidth / 800.0f, (float)windowWidth / 800.0f, -(float)windowHeight / 800.0f, (float)windowHeight / 800.0f, NCP, 1000.0f);
	}
	else
	{
		//Perspective
		proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(fov), (float)windowWidth / (float)windowHeight, NCP, 1000.0f);
	}
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	if (model.size() > 0) {
		for (int i = 0; i < model.size(); i++)
		{
			if (bwShader) {
				bwShader->use();
				bwShader->setVec3("cameraPos", cameraPos);
				//glActiveTexture(0);
				//glBindTexture(GL_TEXTURE_2D, texId);
				//bwShader->setInt("tex", 0);
				//bwShader->setFloat("test", test);
				model[i]->setmodelMatrix();
				model[i]->BindTexture();
				model[i]->Bind();
				model[i]->setView(view);
				model[i]->setproj(proj);
				model[i]->Draw();
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
	if (only_one_light1) {
		if (lampShader) {
			bwShader->use();
			lights->Connect_shader();
			lampShader->use();
			lights->Bind();
			lights->setView(view);
			lights->setproj(proj);
			lights->Draw();
		}
	}
	
	//Quad *quad = Quad::Instance();
	//if (bwShader) {
	//	bwShader->use();
	//	//glBindTexture(GL_TEXTURE_2D, texId);
	//	//wShader->setInt("tex", 0);
	//	//bwShader->setFloat("test", test);
	//	//quad->Bind();
	//	quad->BindTexture();
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
	if (ImGui::Button("Load Model"))
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

	ImGui::PushItemWidth(100);
	if (ImGui::DragFloat("Near Clipping Plane", &NCP, 0.01f));
	ImGui::PopItemWidth();

	if (ImGui::Checkbox("Moverse con mouse", &move))
	{
		firstMouse = true;
	}
	ImGui::SameLine(); HelpMarker("Press Esc to leave camara mode and M to enter use WASD to move\n");
	if (ImGui::CollapsingHeader("Opciones de textura:"))
	{
		if (!model.empty() && picked < model.size() && picked >= 0)
			if (ImGui::Button("Load texture"))
			{
				std::string b = openfilenametexture();
				const char* c = b.c_str();
				model[picked]->loadCreateTexture(c);
				model[picked]->only_color = false;
				model[picked]->only_texture = true;
			}
		if (!model.empty() && picked < model.size() && picked >= 0)
		{
			ImGui::Checkbox("Habilitar Textura", &model[picked]->texture_drawing);
			if (model[picked]->texture_drawing) 
			{

				ImGui::Text("Variaciones de Textura:");
				texture_mode = model[picked]->texture_mode;
				ImGui::Combo("", &texture_mode, "Textura con color\0Solo color\0Solo textura\0\0");
				if (texture_mode == 0)
				{
					model[picked]->texture_with_color = true;
					model[picked]->only_color = false;
					model[picked]->only_texture = false;
				}
				if (texture_mode == 1)
				{
					model[picked]->only_color = true;
					model[picked]->texture_with_color = false;
					model[picked]->only_texture = false;
				}
				if (texture_mode == 2)
				{
					model[picked]->only_texture = true;
					model[picked]->texture_with_color = false;
					model[picked]->only_color = false;
				}
				model[picked]->texture_mode = texture_mode;
			}
			/*ImGui::Checkbox("Textura con color", &model[picked]->texture_with_color);
			if (model[picked]->texture_with_color)
			{
				model[picked]->only_color = false;
				model[picked]->only_texture = false;
			}
			ImGui::Checkbox("Solo color", &model[picked]->only_color);
			if (model[picked]->only_color)
			{
				model[picked]->texture_with_color = false;
				model[picked]->only_texture = false;
			}
			ImGui::Checkbox("Solo textura", &model[picked]->only_texture);
			if (model[picked]->only_texture)
			{
				model[picked]->texture_with_color = false;
				model[picked]->only_color = false;
			}*/
		}
	}
	//LUZ
	if (ImGui::CollapsingHeader("Light 1"))
	{
		if (!only_one_light1) {
			if (ImGui::Button("Create Light"))
			{
					Light* light = new Light();
					light->setupMesh();
					lights = light;
					only_one_light1 = true;

			}
		}
		if (only_one_light1)
		{
			light_col[0] = lights->colorrelleno[0];
			light_col[1] = lights->colorrelleno[1];
			light_col[2] = lights->colorrelleno[2];
			light_col[3] = lights->colorrelleno[3];
			ImGui::Text("Color de la luz");
			ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");
			ImGui::ColorEdit4("Color Light", light_col);
			lights->colorrelleno = glm::vec4(light_col[0], light_col[1], light_col[2], light_col[3]);

			ImGui::Separator();

			//Ambient or diffuse
			ImGui::Checkbox("Ambiental", &lights->bambient); ImGui::SameLine();
			ImGui::Checkbox("Difusa", &lights->bdiffuse); ImGui::SameLine(); 
			ImGui::Checkbox("Especular", &lights->bspecular);

			ImGui::Text("Intensidad Ambiental:");
			intensity_ambiental = lights->intensity_ambiental;
			ImGui::SliderFloat("Drag Intensidad Ambiental", &intensity_ambiental, 0.0f, 1.0f, "ratio = %.05f");
			lights->intensity_ambiental = intensity_ambiental;
			intensity_specular = lights->intensity_specular;
			ImGui::Text("Intensidad Especular:");
			ImGui::SliderFloat("Drag Intensidad Especular", &intensity_specular, 0.0f, 1.0f, "ratio = %.05f");
			lights->intensity_specular = intensity_specular;

			ImGui::Separator();

			//Traslate, scale
			ImGui::Text("X, Y, Z");
			light_vec4fs[0] = lights->vec4fscale[0];
			light_vec4fs[1] = lights->vec4fscale[1];
			light_vec4fs[2] = lights->vec4fscale[2];
			ImGui::DragFloat("SCALE X-Light", &light_vec4fs[0], 0.01f);
			ImGui::DragFloat("SCALE Y-Light", &light_vec4fs[1], 0.01f);
			ImGui::DragFloat("SCALE Z-Light", &light_vec4fs[2], 0.01f);
			lights->vec4fscale[0] = light_vec4fs[0];
			lights->vec4fscale[1] = light_vec4fs[1];
			lights->vec4fscale[2] = light_vec4fs[2];
			if (ImGui::DragFloat("SCALE ALL-Light", &light_vec4fs[0], 0.01f, 0.0f))
			{
				light_vec4fs[1] = light_vec4fs[0];
				light_vec4fs[2] = light_vec4fs[0];
			}
			lights->vec4fscale[0] = light_vec4fs[0];
			lights->vec4fscale[1] = light_vec4fs[1];
			lights->vec4fscale[2] = light_vec4fs[2];

			//ImGui::SliderFloat3("SCALE", vec4fs, 0.0f, 3.0f);
			ImGui::Text("X, Y, Z");
			light_vec4ft[0] = lights->vec4ftraslate[0];
			light_vec4ft[1] = lights->vec4ftraslate[1];
			light_vec4ft[2] = lights->vec4ftraslate[2];
			ImGui::DragFloat("TRASLATE X-Light", &light_vec4ft[0], 0.02f);
			ImGui::DragFloat("TRASLATE Y-Light", &light_vec4ft[1], 0.02f);
			ImGui::DragFloat("TRASLATE Z-Light", &light_vec4ft[2], 0.02f);
			lights->vec4ftraslate[0] = light_vec4ft[0];
			lights->vec4ftraslate[1] = light_vec4ft[1];
			lights->vec4ftraslate[2] = light_vec4ft[2];
			//ImGui::SliderFloat3("TRASLATE", vec4ft, -2.0f, 2.0f, "ratio = %.01f");
			glm::vec3 auxs_light(light_vec4fs[0], light_vec4fs[1], light_vec4fs[2]);
			glm::vec3 auxt_light(light_vec4ft[0], light_vec4ft[1], light_vec4ft[2]);
			light_modelMatrix = glm::mat4(1.0f);
			light_modelMatrix = glm::translate(light_modelMatrix, auxt_light);
			light_modelMatrix = glm::scale(light_modelMatrix, auxs_light);
			lights->setmodelMatrix(light_modelMatrix);
		}
	}

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

	if (ImGui::CollapsingHeader("Objeto"))
	{
		if (!model.empty() && picked < model.size() && picked >= 0)
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

			ImGui::Separator();
			//rotate
			//modelMatrix = model[picked]->modelMatrix;
			ImGui::Text("Red is X, Green is Y, Blue is Z");
			ImGui::Checkbox("Rotate", &Iwant_torotate);
			if (Iwant_torotate)
			{
				qRot = model[picked]->Qrotacion;
				ImGui::gizmo3D("##gizmo1", qRot /*, size,  mode */);
			}
			model[picked]->Qrotacion = qRot;
			//Traslate, scale
			ImGui::Text("X, Y, Z");
			vec4fs[0] = model[picked]->vec4fscale.x;
			vec4fs[1] = model[picked]->vec4fscale.y;
			vec4fs[2] = model[picked]->vec4fscale.z;
			ImGui::DragFloat("SCALE X", &vec4fs[0], 0.01f);
			ImGui::DragFloat("SCALE Y", &vec4fs[1], 0.01f);
			ImGui::DragFloat("SCALE Z", &vec4fs[2], 0.01f);
			model[picked]->vec4fscale.x = vec4fs[0];
			model[picked]->vec4fscale.y = vec4fs[1];
			model[picked]->vec4fscale.z = vec4fs[2];
			if (ImGui::DragFloat("SCALE ALL", &vec4fs[0], 0.01f, 0.0f))
			{
				vec4fs[1] = vec4fs[0];
				vec4fs[2] = vec4fs[0];
			}
			model[picked]->vec4fscale.x = vec4fs[0];
			model[picked]->vec4fscale.y = vec4fs[1];
			model[picked]->vec4fscale.z = vec4fs[2];

			//ImGui::SliderFloat3("SCALE", vec4fs, 0.0f, 3.0f);
			ImGui::Text("X, Y, Z");
			vec4ft[0] = model[picked]->vec4ftraslate.x;
			vec4ft[1] = model[picked]->vec4ftraslate.y;
			vec4ft[2] = model[picked]->vec4ftraslate.z;
			ImGui::DragFloat("TRASLATE X", &vec4ft[0], 0.02f);
			ImGui::DragFloat("TRASLATE Y", &vec4ft[1], 0.02f);
			ImGui::DragFloat("TRASLATE Z", &vec4ft[2], 0.02f);
			model[picked]->vec4ftraslate.x = vec4ft[0];
			model[picked]->vec4ftraslate.y = vec4ft[1];
			model[picked]->vec4ftraslate.z = vec4ft[2];
			//ImGui::SliderFloat3("TRASLATE", vec4ft, -2.0f, 2.0f, "ratio = %.01f");
			/*glm::vec3 auxs(vec4fs[0], vec4fs[1], vec4fs[2]);
			glm::vec3 auxt(vec4ft[0], vec4ft[1], vec4ft[2]);
			modelMatrix = mat4_cast(model[picked]->Qrotacion);
			modelMatrix = glm::translate(modelMatrix, auxt);
			modelMatrix = glm::scale(modelMatrix, auxs);
			model[picked]->setmodelMatrix(modelMatrix);*/

			ImGui::Separator();

			ImGui::Checkbox("Mallado", &model[picked]->mallado);
			ImGui::Checkbox("Puntos", &model[picked]->points);
			ImGui::Checkbox("Relleno", &model[picked]->relleno);
			ImGui::Checkbox("Back Face culling", &model[picked]->back_face_culling);
			if (ImGui::Checkbox("Ortho", &orthos))
			{
				NCP = 0.0f;
			}
			else
			{
				NCP = 0.01f;
			}
			ImGui::Checkbox("Z-buffer", &zbuffer);
			if (zbuffer) 
			{
				glEnable(GL_DEPTH_TEST);
				//glDepthMask(GL_FALSE);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			} else
				{
				glDisable(GL_DEPTH_TEST);
				}

		}
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
	bwShader = new Shader(vertexPath, fragmentPath, NULL);
	lampShader = new Shader(vertex_LightPath, fragment_LightPath, NULL);
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
