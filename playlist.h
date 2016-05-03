#ifndef PLAYLIST_H_INCLUDED
#define PLAYLIST_H_INCLUDED

#include<list>
#include<string>

#include"structures_for_broadcast.h"
#include"eventchannel.h"

class Playlist{
public:
    std::list<std::string>tracks;

    void set(){
        chan.broadcast(ClearMusic());
        for(auto& name : tracks){
            chan.broadcast(AddMusic(name));
        }
    }
private:
    EventChannel chan;
};

#endif // PLAYLIST_H_INCLUDED
