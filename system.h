#ifndef SYSTEM_H
#define SYSTEM_H

#include "eventchannel.h"

class System
{
    protected:
    
    EventChannel mChannel;
    public:
        System(){}
        virtual ~System(){}
        virtual void update() = 0;
};

#endif // SYSTEM_H
