#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "Bullet/include/LinearMath/btVector3.h"
#include "glmath.h"
class btRigidBody;
class Module;

enum class ColliderType
{
	PLAYER,
	PLATFORM,
	DEATH,
	WIN,
	CHECKPOINT,
	ICE,
	GRASS,
	UNKNOWN
};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetPos(const btVector3& pos);
	void SetPos(vec3& pos);
	void SetAsSensor(bool sensor);
	bool GetIsSensor() { return sensor; }

	void isSensor();
private:
	bool sensor;

public:
	btRigidBody* body = nullptr;
	p2List<Module*> collision_listeners;
	ColliderType ctype;
};

#endif // __PhysBody3D_H__