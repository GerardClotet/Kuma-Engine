#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Component_Camera.h"
#include "Component_Transform.h"
#include "SDL\include\SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew-2.1.0/lib/glew32.lib")
ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//Create contexts
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else App->saveLog("OpenGL context created");
	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing glew library! %s\n", SDL_GetError());
		ret = false;
	}
	else App->saveLog("Initialized glew lib");
	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		////Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->saveLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}



		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();



		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

	
		actual_camera = App->camera->camera_fake;
	}

	// Projection matrix for
	OnResize(App->window->GetScreenWidth(), App->window->GetScreenHeight());


	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//App->saveLog("Vendor: %s", glGetString(GL_VENDOR));
	//App->saveLog("Renderer: %s", glGetString(GL_RENDERER));
	//App->saveLog("OpenGL version supported %s", glGetString(GL_VERSION));
	//App->saveLog("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


	LOG("Rendererd3D init");
	App->saveLog("Renderer3D init");
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(actual_camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(actual_camera->frustum.pos.x, actual_camera->frustum.pos.y, actual_camera->frustum.pos.z);


	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(actual_camera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawQuadtreeCube(const float3 * corners, Color color)
{
	glDisable(GL_CULL_FACE);
	glColor3f(color.r, color.g, color.b);
	glLineWidth(2.0f);

	glBegin(GL_QUADS);

	//Draw the 6 faces of the cube

	glVertex3fv((const GLfloat*)&corners[1]);
	glVertex3fv((const GLfloat*)&corners[5]);
	glVertex3fv((const GLfloat*)&corners[7]);
	glVertex3fv((const GLfloat*)&corners[3]);

	glVertex3fv((const GLfloat*)&corners[4]);
	glVertex3fv((const GLfloat*)&corners[0]);
	glVertex3fv((const GLfloat*)&corners[2]);
	glVertex3fv((const GLfloat*)&corners[6]);

	glVertex3fv((const GLfloat*)&corners[5]);
	glVertex3fv((const GLfloat*)&corners[4]);
	glVertex3fv((const GLfloat*)&corners[6]);
	glVertex3fv((const GLfloat*)&corners[7]);

	glVertex3fv((const GLfloat*)&corners[0]);
	glVertex3fv((const GLfloat*)&corners[1]);
	glVertex3fv((const GLfloat*)&corners[3]);
	glVertex3fv((const GLfloat*)&corners[2]);

	glVertex3fv((const GLfloat*)&corners[3]);
	glVertex3fv((const GLfloat*)&corners[7]);
	glVertex3fv((const GLfloat*)&corners[6]);
	glVertex3fv((const GLfloat*)&corners[2]);

	glVertex3fv((const GLfloat*)&corners[0]);
	glVertex3fv((const GLfloat*)&corners[4]);
	glVertex3fv((const GLfloat*)&corners[5]);
	glVertex3fv((const GLfloat*)&corners[1]);

	glEnd();
	glEnable(GL_CULL_FACE);

}
