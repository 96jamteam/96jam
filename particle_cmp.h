//
//  particle_cmp.h
//  maybe
//
//  Created by Janek on 01/05/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef particle_cmp_h
#define particle_cmp_h

class ParticleEmitter : public Component{
public:
    sf::Vector3f velocity;
    float timetoemit;
    float emitspan;

    int formula;
};

class Particle : public Component{
public:
    float lifespan;
};

#endif /* particle_cmp_h */
