//
//  contact_listener.hpp
//  heist
//
//  Created by Janek on 06/04/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef contact_listener_hpp
#define contact_listener_hpp

#include <stdio.h>
#include <Box2d/Box2d.h>
#include"component_container.h"
#include "weapon_cmp.h"
#include "stuff.h"

class ContactListener : public b2ContactListener
{
public:
    ContactListener(ComponentContainer*);
private:
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void callback(int id, std::string s);

    ComponentContainer* cc;

};
#endif /* contact_listener_hpp */
