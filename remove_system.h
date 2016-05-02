#ifndef REMOVE_SYSTEM_H_INCLUDED
#define REMOVE_SYSTEM_H_INCLUDED
#include"system.h"
#include"engine.h"
#include"views.h"
#include"structures_for_broadcast.h"
#include"scene_manager.h"
#include "stuff.h"
#include <vector>
#include <set>

class RemoveSystem : public System{
    ComponentContainer* cc;
    std::set<int>ids;
public:
    RemoveSystem(ComponentContainer* _cc):System(), cc(_cc){
       mChannel.add<DeleteThisShit>(*this);
    }

    void update(){
        bool cyka=0;
        for(int i : ids){
            P("DZIABA");
            cc->deleteComponents(i);
            cyka=1;
        }
        if(cyka)
            mChannel.broadcast(SpriteAdded());
        ids.clear();
    }

    void handle(const DeleteThisShit& e){
        ids.insert(e.entityid);
    }
};



#endif // REMOVE_SYSTEM_H_INCLUDED
