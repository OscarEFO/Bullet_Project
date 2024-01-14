#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void Teleport(int num);
	void AssistDirection(float hardness);
	void CheckPoints();

public:

	PhysVehicle3D* vehicle;
	PhysVehicle3D* vehicleSensor;

	float turn;
	float acceleration;
	float brake;

	bool respawn;
	int lastCheckpoint;
	Cube cubeSensor;
	PhysBody3D* bodySensor;

	bool drag, slippery;
	bool win;

private:

	btVector3 forwardVector;
	btVector3 perpendicularVector;

	int velocity;
	float assistDirection;
	float calc;
	float floorOffset;
	btVector3 positionCM;

	//Gameplay conditioners

	bool CPactive = false;
	bool falling = false;
	//Apearence

	Color color;

	//fx
	unsigned int brakefx, deadfx, driftfx, enginefx;

	float angle = -0.25;

};