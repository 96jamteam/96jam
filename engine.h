#ifndef ENGINE_H
#define ENGINE_H

#include<list>
#include"system.h"
#include"eventchannel.h"

class Engine
{
    public:
        struct StopEvent {};
        Engine(){
            channel.add<StopEvent>(*this);
            running=0;
        }
        virtual ~Engine(){
        }
        void add(const std::shared_ptr<System>& sys){
            systems.push_back(sys);
        }
        bool isRunning(){
            return running;
        }

        void run(){
            running=true;
            while(running)
            for(auto sys: systems){
                sys->update();
            }
        }

        void handle(const StopEvent&){
            stop();
        }

    private:
        void stop(){
            running=false;
        }
        bool running;
        std::list<std::shared_ptr<System>>systems;
        EventChannel channel;
};

#endif // ENGINE_H
