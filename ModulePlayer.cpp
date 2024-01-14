#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

#define PI 3.1415

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");


	//start
	brakefx = App->audio->LoadFx("Assets/Audio/Fx/brake.wav");
	deadfx = App->audio->LoadFx("Assets/Audio/Fx/dead.wav");
	driftfx = App->audio->LoadFx("Assets/Audio/Fx/drift.wav");
	enginefx = App->audio->LoadFx("Assets/Audio/Fx/engine.wav");


	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(1.5f, 0.5f,2.5f);
	car.chassis_offset.Set(0, 1, 0);

	car.cabin_size.Set(0.5f, 0.25f, 1);
	car.cabin_offset.Set(0, car.chassis_offset.y + car.chassis_size.y / 2 + car.cabin_size.y / 2, -0.25f);

	car.aleronL_size.Set(0.1f,0.6f,0.2f);
	car.aleronL_offset.Set(0.5f,1.55f,-1.15f);

	car.aleronR_size.Set(0.1f, 0.6f, 0.2f);
	car.aleronR_offset.Set(-0.5f, 1.55f, -1.15f);

	car.aleron_size.Set(1.1f, 0.15f, 0.2f);
	car.aleron_offset.Set(0, 1.55f, -1.15f);

	car.aleron2_size.Set(1.1f, 0.15f, 0.5f);
	car.aleron2_offset.Set(0, 1.8f, -1.3f);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 1;
	float suspensionRestLength = 0.9f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width + 0.7f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width - 0.7f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width + 0.7f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width - 0.7f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	// Sensors
	{
		VehicleInfo sensorInf;

		sensorInf.sensor_size.Set(0.75f, 0.5f, 0.75f);
		sensorInf.sensor_offset.Set(0, 0, 0);
		sensorInf.mass = 0.001f;
		sensorInf.num_wheels = 0;


		vehicleSensor = App->physics->AddVehicle(sensorInf);
		vehicleSensor->body->setGravity({ 0,0,0 });
		vehicleSensor->color.Set(1, 0.5, 0.5);
		vehicleSensor->SetAsSensor(true);
		vehicleSensor->body->setUserPointer(vehicleSensor);
		vehicleSensor->body->setCollisionFlags(vehicleSensor->body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);


		cubeSensor.SetPos(0, 10, 0);
		cubeSensor.size = { 0.25,0.25,0.25 };
		cubeSensor.color = Color(1, 0.5, 0.3);
		bodySensor = App->physics->AddBody(cubeSensor, 0);

		bodySensor->collision_listeners.add(this);
		bodySensor->SetAsSensor(true);
		bodySensor->body->setCollisionFlags(bodySensor->body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);

		bodySensor->SetPos(0, 10, 0);
	}
	vehicle = App->physics->AddVehicle(car);
	vehicle->ctype = ColliderType::PLAYER;
	vec3 cam = App->scene_intro->checkpoints.at(0).data->GetPos();
	float* pos = App->scene_intro->checkpoints.at(0).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);

	lastCheckpoint = 0;
	slippery = false;
	drag = false;
	win = false;
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	positionCM = vehicle->body->getCenterOfMassPosition();
	brake = 2.5f;
	turn = acceleration = brake = 0.0f;
	AssistDirection(90.0f);

	
	vehicle->vehicle->getChassisWorldTransform();

	btQuaternion q = vehicle->vehicle->getChassisWorldTransform().getRotation();

	cubeSensor.SetPos(positionCM.getX(), positionCM.getY() - 0.55, positionCM.getZ());
	vehicle->vehicle->getChassisWorldTransform().getOpenGLMatrix(&cubeSensor.transform);
	btVector3 offset(0, -0.55, 0);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	cubeSensor.transform.M[12] += offset.getX();
	cubeSensor.transform.M[13] += offset.getY();
	cubeSensor.transform.M[14] += offset.getZ();
	float* pos = cubeSensor.transform.M;
	bodySensor->SetTransform(pos);
	vehicleSensor->SetTransform(pos);

	if (vehicle->body->getCenterOfMassPosition().getY() < App->scene_intro->platOffset - 10) respawn = true;
	CheckPoints();

	/*if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		acceleration = -(MAX_ACCELERATION);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		acceleration = MAX_ACCELERATION * 2;
	}*/
	vehicle->SetColor(color);
	color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	forwardVector = vehicle->vehicle->getForwardVector().normalize();

	perpendicularVector = { -forwardVector.getZ(), forwardVector.getY(), forwardVector.getX() };

	if (!drag)
	{
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT &&
			(vehicle->state != State::IN_AIR || vehicle->state == State::TURBO) &&
			App->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
		{
			velocity = MAX_ACCELERATION * 3;

			vehicle->state = TURBO;
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-99,0 });
		}
		else
		{
			velocity = MAX_ACCELERATION;
			if (vehicle->state != State::IN_AIR)vehicle->state = State::IDLE;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		App->audio->PlayFx(enginefx);
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (vehicle->state != State::TURBO && vehicle->state != State::IN_AIR)vehicle->state = State::WALK;
		vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-70,0 });
		if (vehicle->vehicle->getCurrentSpeedKmHour() <= -2.25)
		{
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-50,0 });
			brake = BRAKE_POWER / 1.5f;
			color.Set(1.0f, 0.0f, 0.0f, 1.0f);
			acceleration = MAX_ACCELERATION;
		}
		else
		{
			acceleration = velocity;
			vehicle->body->applyTorque(perpendicularVector * -40);
		}

	}


	if (!slippery)
	{
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			App->audio->PlayFx(brakefx);
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (vehicle->state != State::TURBO && vehicle->state != State::IN_AIR)vehicle->state = State::WALK;
			if (vehicle->vehicle->getCurrentSpeedKmHour() > +2.25)
			{
				brake = BRAKE_POWER / 1.5f;
				color.Set(1.0f, 0.0f, 0.0f, 1.0f);
				vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-50,0 });

			}
			else
			{
				acceleration = velocity * -1;
				vehicle->body->applyTorque(perpendicularVector * 80);
			}
		}
	}
	if (slippery)
	{
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			App->audio->PlayFx(driftfx);
		}
	}
	

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += (TURN_DEGREES)-assistDirection;
		brake = 15;

		if (vehicle->state == State::IN_AIR)
		{
			vehicle->body->applyTorque(forwardVector * -45);
		}
		else
		{
			vehicle->body->applyTorque(forwardVector * -200);
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= (TURN_DEGREES)-assistDirection;
		brake = 15;

		if (vehicle->state == State::IN_AIR)
		{
			vehicle->body->applyTorque(forwardVector * 45);
		}
		else
		{
			vehicle->body->applyTorque(forwardVector * 200);
		}
	}
	if (drag)
	{
		acceleration = -MAX_ACCELERATION;
		if (acceleration < 0) acceleration = 0;
	}
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char* title = "FuimRace";
	if (lastCheckpoint == 4)
	{
		win = true;
	}

	App->window->SetTitle(title);
	if (win)
	{
		sprintf(title, "YOU WIN!!¡!!1¡!!11¡!1!", 1);
		App->window->SetTitle(title);
	}
	/*char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());*/

	return UPDATE_CONTINUE;
}

void ModulePlayer::Teleport(int num)
{
	respawn = false;
	vehicle->body->setLinearVelocity({ 0,0,0 });
	vehicle->body->setAngularVelocity({ 0,0,0 });
	vec3 cam = App->scene_intro->checkpoints.at(num).data->GetPos();
	float* pos = App->scene_intro->checkpoints.at(num).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);
}

void ModulePlayer::AssistDirection(float hardness)
{
	float turnDegrees = (TURN_DEGREES / DEGTORAD);
	calc = (vehicle->GetKmh() / 16) * (hardness / 100.0f);
	if (calc <= turnDegrees - 5)
		assistDirection = calc * DEGTORAD;
	else assistDirection = (turnDegrees - 5) * DEGTORAD;

}

void ModulePlayer::CheckPoints()
{
	if (respawn) Teleport(lastCheckpoint);
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) Teleport(0);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) Teleport(1);
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) Teleport(2);
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) Teleport(3);
}



