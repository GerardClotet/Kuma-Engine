#include "PanelConfig.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"
#include "ModuleUI.h"
#include "PanelFile.h"


PanelFile::~PanelFile()
{
}

update_status PanelFile::Draw()
{
	if (App->ui->file_load_model)LoadModel();
	if (App->ui->file_save_scene)DisplaySaveScene();
	if (App->ui->file_load_scene)DisplayLoadScene();

	return UPDATE_CONTINUE;
}

void PanelFile::LoadModel()
{
	App->ui->LoadFile("fbx");

}

void PanelFile::DisplaySaveScene()
{
	App->ui->SaveFile("kumaScene");
}

void PanelFile::DisplayLoadScene()
{
	App->ui->LoadFile("kumaScene");
}
