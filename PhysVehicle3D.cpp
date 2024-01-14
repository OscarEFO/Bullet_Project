#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;
		wheel.color = Black;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color = Color(0.30196, 0.85882, 1.00000);

	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	btVector3 offset5(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset5 = offset5.rotate(q.getAxis(), q.getAngle());

	cabin.transform.M[12] += offset5.getX();
	cabin.transform.M[13] += offset5.getY();
	cabin.transform.M[14] += offset5.getZ();
	cabin.color = Color(1.00000, 0.10196, 0.10196);

	Cube aleronL(info.aleronL_size.x, info.aleronL_size.y, info.aleronL_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aleronL.transform);
	btVector3 offset2(info.aleronL_offset.x, info.aleronL_offset.y, info.aleronL_offset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	aleronL.transform.M[12] += offset2.getX();
	aleronL.transform.M[13] += offset2.getY();
	aleronL.transform.M[14] += offset2.getZ();
	aleronL.color = Color(1.00000, 0.30196, 0.58039);

	Cube aleronR(info.aleronR_size.x, info.aleronR_size.y, info.aleronR_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aleronR.transform);
	btVector3 offset3(info.aleronR_offset.x, info.aleronR_offset.y, info.aleronR_offset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());

	aleronR.transform.M[12] += offset3.getX();
	aleronR.transform.M[13] += offset3.getY();
	aleronR.transform.M[14] += offset3.getZ();
	aleronR.color = Color(1.00000, 0.30196, 0.58039);

	Cube aleron(info.aleron_size.x, info.aleron_size.y, info.aleron_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aleron.transform);
	btVector3 offset4(info.aleron_offset.x, info.aleron_offset.y, info.aleron_offset.z);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	aleron.transform.M[12] += offset4.getX();
	aleron.transform.M[13] += offset4.getY();
	aleron.transform.M[14] += offset4.getZ();
	aleron.color = Color(1.00000, 0.30196, 0.58039);

	Cube aleron2(info.aleron2_size.x, info.aleron2_size.y, info.aleron2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aleron2.transform);
	btVector3 offset6(info.aleron2_offset.x, info.aleron2_offset.y, info.aleron2_offset.z);
	offset6 = offset6.rotate(q.getAxis(), q.getAngle());

	aleron2.transform.M[12] += offset6.getX();
	aleron2.transform.M[13] += offset6.getY();
	aleron2.transform.M[14] += offset6.getZ();
	aleron2.color = Color(1.00000, 0.30196, 0.58039);

	chassis.Render();
	cabin.Render();
	aleronL.Render();
	aleronR.Render();
	aleron.Render();
	aleron2.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}