#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	Cube* cube;
	platOffset = 100;
	
	App->audio->PlayMusic("Assets/Audio/TheGrefg_Victory.wav");

	//CHECKPOINTS
	{
		// Checkpoint Start 
		cube = new Cube(1, 1, 1);
		cube->SetPos(190, 5 + platOffset, 0);
		cube->SetRotation(0, { 0,1,0 });
		checkpoints.add(cube);

		// Checkpoint Start 
		cube = new Cube(1, 1, 1);
		cube->SetPos(40, 5 + platOffset, 150);
		cube->SetRotation(0, { 0,1,0 });
		checkpoints.add(cube);

		// Checkpoint Start 
		cube = new Cube(1, 1, 1);
		cube->SetPos(40, 5 + platOffset, -150);
		cube->SetRotation(0, { 0,1,0 });
		checkpoints.add(cube);

		// Checkpoint Start 
		cube = new Cube(1, 1, 1);
		cube->SetPos(170, 5 + platOffset, -140);
		cube->SetRotation(0, { 0,1,0 });
		checkpoints.add(cube);
	}
	/*ground = new Plane(0, 1, 0, 0);
	ground->axis = false;*/
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)App->SetDebug();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	switch (body2->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision CHECKPOINT");
		for (size_t i = 0; i < checkPlat.count(); i++)
		{
			if (body1 == checkPlat.at(i).data || body2 == checkPlat.at(i).data)
				App->player->lastCheckpoint = i;
		}
		break;
	case ColliderType::ICE:
		LOG("Collision ICE");
		if (body1->ctype == ColliderType::PLAYER)
		{
			App->player->slippery = true;
		}
		break;
	case ColliderType::GRASS:
		LOG("Collision GRASS");
		if (body1->ctype == ColliderType::PLAYER)
		{
			App->player->drag = true;
		}
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (body1->ctype == ColliderType::PLAYER)
		{
			App->player->slippery = false;
			App->player->drag = false;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		if (body2->ctype == ColliderType::PLAYER)
		{
			App->player->slippery = false;
			App->player->drag = false;
		}
		break;
	}



	switch (body1->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision CHECKPOINT");
		for (size_t i = 0; i < checkPlat.count(); i++)
		{
			if (body1 == checkPlat.at(i).data || body2 == checkPlat.at(i).data)
				App->player->lastCheckpoint = i;
		}
		break;
	case ColliderType::ICE:
		LOG("Collision ICE");
		if (body2->ctype == ColliderType::PLAYER)
		{
			App->player->slippery = true;
		}
		break;
	case ColliderType::GRASS:
		LOG("Collision GRASS");
		if (body2->ctype == ColliderType::PLAYER)
		{
			App->player->drag = true;
		}
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (body2->ctype == ColliderType::PLAYER)
		{
			App->player->slippery = false;
			App->player->drag = false;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		if (body2->ctype == ColliderType::PLAYER)
		{
			App->player->slippery = false;
			App->player->drag = false;
		}
		break;
	}
}

