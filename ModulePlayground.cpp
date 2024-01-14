#include "Globals.h"
#include "Application.h"
#include "ModulePlayground.h"
#include "Primitive.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"

ModulePlayground::ModulePlayground(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayground::~ModulePlayground()
{}

// Load assets
bool ModulePlayground::Start()
{
	CreatePlayground();
	bool ret = true;
	return ret;
}

// Load assets
bool ModulePlayground::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModulePlayground::Update(float dt)
{
	p2List_item<Cube*>* c = scenaryCubes.getFirst();

	while (c != NULL) {
		c->data->Render();
		c = c->next;
	}

	
	return UPDATE_CONTINUE;
}

void ModulePlayground::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModulePlayground::CreatePlayground() {

	// Colors
	Color Red(1.0, 0.0, 0.0);
	Color Green(0.0, 1.0, 0.0);
	Color Blue(0.0, 0.0, 1.0);
	Color Water = Blue;
	Color Dirt(0.4, 0.2, 0.0);

	//big lap
	CreateScenarioCube(vec3(20, 0.5f, 240), vec3(0, App->scene_intro->platOffset, 0), 0,ColliderType::ICE, Color(0.30196, 0.72157, 1.00000, 0));
	//CreateScenarioCube(vec3(20, 0.5f, 240), vec3(0, App->scene_intro->platOffset + 0.01f, 0), 0, ColliderType::ICE, Color(0, 0, 255, 0), true);

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(20.5f, App->scene_intro->platOffset,134),45);

	CreateScenarioCube(vec3(20, 0.5f, 240), vec3(190, App->scene_intro->platOffset, 0), 0, ColliderType::CHECKPOINT);

	CreateScenarioCube(vec3(20, 0.5f, 220), vec3(45, App->scene_intro->platOffset, 155),90, ColliderType::CHECKPOINT);

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(169.5f, App->scene_intro->platOffset, 134), -45);

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(20.5f, App->scene_intro->platOffset, -134), -45);

	CreateScenarioCube(vec3(20, 0.5f, 220), vec3(45, App->scene_intro->platOffset, -155), 90, ColliderType::CHECKPOINT);

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(169.5f, App->scene_intro->platOffset, -134), 45, ColliderType::CHECKPOINT);

	//small lap
	CreateScenarioCube(vec3(20, 0.5f, 240), vec3(-100, App->scene_intro->platOffset, 0), 0, ColliderType::GRASS, Color(0.52157, 0.87843, 0.52157));

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(-79.5f, App->scene_intro->platOffset, 134), 45);

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(-79.5f, App->scene_intro->platOffset, -134), -45);

	CreateScenarioCube(vec3(23, 0.5f, 63.6f), vec3(20.5f, App->scene_intro->platOffset, 134), 45);





}

void ModulePlayground::CreateScenarioCube(vec3 size, vec3 pos, float angle, ColliderType ctype, Color color,  bool sensor) {

	Cube* cube = new Cube(size.x, size.y, size.z);

	cube->SetPos(pos.x, pos.y, pos.z);
	cube->SetRotation(angle, vec3(0, 1, 0));

	PhysBody3D* PhysBody = App->physics->AddBody(*cube, 0);
	pbCube.add(PhysBody);
	PhysBody->SetPos(pos.x, pos.y, pos.z);
	PhysBody->ctype = ctype;	
	if (sensor) {
		PhysBody->SetAsSensor(true);
	}
	PhysBody->SetTransform(cube->transform.M);

	cube->color.Set(color.r, color.g, color.b, color.a);

	scenaryCubes.add(cube);

	if (ctype == ColliderType::CHECKPOINT) {
		App->scene_intro->checkPlat.add(PhysBody);
	}

	if (ctype == ColliderType::ICE)
	{
		pbCube.getLast()->data->body->setFriction(-1.0f);
		App->scene_intro->checkIce.add(PhysBody);
	}

	if (ctype == ColliderType::GRASS)
	{
		pbCube.getLast()->data->body->setFriction(5.0f);
		App->scene_intro->checkGrass.add(PhysBody);
	}
	if (ctype == ColliderType::UNKNOWN)
	{
		App->scene_intro->checkPlatf.add(PhysBody);
	}

}
