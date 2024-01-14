#pragma once
#include "Module.h"
#include "PhysBody3D.h"
#include "Globals.h"
#include "Primitive.h"

struct Cube;

class ModulePlayground : public Module
{
public:
	ModulePlayground(Application* app, bool start_enabled = true);
	~ModulePlayground();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	void CreateScenarioCube(vec3 size, vec3 pos, float angle = 0.0f, ColliderType ctype = ColliderType::UNKNOWN, Color color = Color(1, 1, 1, 1),  bool sensor = false);

	void CreatePlayground();

	p2List<Cube*> scenaryCubes;
	p2List<Cylinder> scenaryCylinders;
	p2List<Sphere> scenarySpheres;

	p2List<PhysBody3D*> pbCube;

	
	
};
