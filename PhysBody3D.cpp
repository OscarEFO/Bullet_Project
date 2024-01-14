#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body), ctype(ColliderType::UNKNOWN)
{
	ctype = ColliderType::UNKNOWN;
}


// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL)
	{
		if (matrix != NULL)
		{
			btTransform t;
			t.setFromOpenGLMatrix(matrix);
			body->setWorldTransform(t);
		}
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

void PhysBody3D::SetPos(const btVector3& pos)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(pos);
	body->setWorldTransform(t);
}

void PhysBody3D::SetPos(vec3& pos)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(pos.x, pos.y, pos.z));
	body->setWorldTransform(t);
}

void PhysBody3D::isSensor()
{
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void PhysBody3D::SetAsSensor(bool value)
{
	if (this->sensor != value)
	{
		this->sensor = value;
		if (value == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}
