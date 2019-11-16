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
	if (App->ui->file_window)DisplayFiles();
	if (App->ui->file_save_window)DisplaySaveFiles();

	return UPDATE_CONTINUE;
}

void PanelFile::DisplayFiles()
{
	App->ui->LoadFile("fbx");

}

void PanelFile::DisplaySaveFiles()
{
	App->ui->SaveFile("fbx");
}
