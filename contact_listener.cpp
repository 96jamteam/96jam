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
#include "callback_cmp.h"
#include "physics_cmp.h"
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

    if (cc->getComponent<Physics>(entityIDa)->active && cc->getComponent<Physics>(entityIDb)->active){
        ContactName* contactNameA = cc->getComponent<ContactName>(entityIDa);
        if (contactNameA != nullptr) {
            callback(entityIDb,contactNameA->name+"_begin");
        }
        else{
            callback(entityIDb,"_begin");
        }

        ContactName* contactNameB = cc->getComponent<ContactName>(entityIDb);
        if (contactNameB != nullptr) {
            callback(entityIDa,contactNameB->name+"_begin");
        }
        else{
            callback(entityIDa,"_begin");
        }
    }

}

void ContactListener::callback(int id, std::string s){
    CallbackCmp* callbackCmp = cc->getComponent<CallbackCmp>(id);
    if (callbackCmp != nullptr) {
        if(callbackCmp->callbacks.find(s)!=callbackCmp->callbacks.end())
            callbackCmp->callbacks[s]();
    }
}

void ContactListener::EndContact(b2Contact* contact)
{/*
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Body* bodyA = fixtureA->GetBody();
    intptr_t entityIDa = (intptr_t)bodyA->GetUserData();

    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Body* bodyB = fixtureB->GetBody();
    intptr_t entityIDb = (intptr_t)bodyB->GetUserData();

    if (cc->getComponent<Physics>(entityIDa)->active && cc->getComponent<Physics>(entityIDb)->active){
        ContactName* contactNameA = cc->getComponent<ContactName>(entityIDa);
        if (contactNameA != nullptr) {
            callback(entityIDb,contactNameA->name+"_end");
        }
        else{
            callback(entityIDb,"_end");
        }

        ContactName* contactNameB = cc->getComponent<ContactName>(entityIDb);
        if (contactNameB != nullptr) {
            callback(entityIDa,contactNameB->name+"_end");
        }
        else{
            callback(entityIDa,"_end");
        }
    }*/
}
