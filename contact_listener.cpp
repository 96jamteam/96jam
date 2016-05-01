//
//  contact_listener.cpp
//  heist
//
//  Created by Janek on 06/04/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#include "contact_listener.h"
#include <iostream>
#include "contact_name_cmp.h"
#include <cstdint>

ContactListener::ContactListener(ComponentContainer* _cc) : cc(_cc){}

void ContactListener::BeginContact(b2Contact* contact)
{
    
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Body* bodyA = fixtureA->GetBody();
    intptr_t entityIDa = (intptr_t)bodyA->GetUserData();
    
    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Body* bodyB = fixtureB->GetBody();
    intptr_t entityIDb = (intptr_t)bodyB->GetUserData();
    
}


void ContactListener::EndContact(b2Contact* contact)
{
    std::cout << "ended" << std::endl;
}