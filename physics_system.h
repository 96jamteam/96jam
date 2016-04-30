#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>

#include "system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "physics_cmp.h"
#include "transform_cmp.h"

class PhysicsSystem : public System {

private:
    EventChannel channel;
	b2World* world;
	ComponentContainer* cc;
	componentContainer::container<Physics>* physicsComponents;
	componentContainer::container<Transform>* transformComponents;

	float FIXED_TIMESTEP = stuff::normalTimestep;
	float fixedTimestepAccumulator_;
	float fixedTimestepAccumulatorRatio_;
    float factor = 1.f;

	sf::Clock clock;


public:
	PhysicsSystem(b2World* w, ComponentContainer& _cc) : world(w), cc(&_cc) {
        fixedTimestepAccumulator_ = 0;
        channel.add<BulletTime>(*this);

		physicsComponents = cc->getComponentStorage<Physics>();
		if (physicsComponents == nullptr)
			physicsComponents = cc->addComponentStorage<Physics>();

		transformComponents = cc->getComponentStorage<Transform>();
		if (transformComponents == nullptr)
			transformComponents = cc->addComponentStorage<Transform>();
	}

	virtual ~PhysicsSystem(){

	}

    void handle(const BulletTime& btime)
    {
        factor = btime.factor;
        FIXED_TIMESTEP = stuff::normalTimestep*factor;
    }


	virtual void update() {
		float dt = clock.restart().asSeconds()*factor;

		const int MAX_STEPS = 50;

		fixedTimestepAccumulator_ += dt;
		const int nSteps = static_cast<int> ( std::floor(fixedTimestepAccumulator_ / FIXED_TIMESTEP) );

		if (nSteps > 0){
			fixedTimestepAccumulator_ -= nSteps * FIXED_TIMESTEP;
		}

		assert(
			"Accumulator must have a value lesser than the fixed time step" &&
			fixedTimestepAccumulator_ < FIXED_TIMESTEP + FLT_EPSILON
			);
		fixedTimestepAccumulatorRatio_ = fixedTimestepAccumulator_ / FIXED_TIMESTEP;

		const int nStepsClamped = std::min(nSteps, MAX_STEPS);

		for (int i = 0; i < nStepsClamped; ++i) {
            channel.broadcast(PhysicsUpdate(FIXED_TIMESTEP));
			world->Step(FIXED_TIMESTEP, 8, 3);
		}

		world->ClearForces();

		Transform* transform;
		for (int j = 0; j < physicsComponents->size(); j++) {
			if ((*physicsComponents)[j].active) {
				transform = cc->getComponent<Transform>((*physicsComponents)[j].entityID);
				if (transform != nullptr) {
					transform->angle = (*physicsComponents)[j].body->GetAngle();
					transform->x = (*physicsComponents)[j].body->GetPosition().x*stuff::SCALE;
                    transform->y = (*physicsComponents)[j].body->GetPosition().y*stuff::SCALE;
				}
			}
		}

	}
};
