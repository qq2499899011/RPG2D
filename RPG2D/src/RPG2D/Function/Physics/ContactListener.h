#pragma once
#include "box2d/box2d.h"
namespace RPG2D {
	class ContactListener:public b2ContactListener
	{
		virtual void BeginContact(b2Contact* contact)override;
		virtual void EndContact(b2Contact* contact)override;
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)override;
	};
}
