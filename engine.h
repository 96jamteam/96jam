#ifndef ENGINE_H
#define ENGINE_H

#include<list>
#include"system.h"

class Engine
{
    public:
        Engine(){
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
        void stop(){
            running = false;
        }
        void run(){
            running=true;
            while(running)
            for(auto sys: systems){
                sys->update();
            }
        }
    private:
        bool running;
        std::list<std::shared_ptr<System>>systems;
};

#endif // ENGINE_H
