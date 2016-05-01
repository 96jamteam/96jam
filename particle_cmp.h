//
//  particle_cmp.h
//  maybe
//
//  Created by Janek on 01/05/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef particle_cmp_h
#define particle_cmp_h

class Particle : public Component{
public:
    sf::Vector3f velocity;
    float lifespan;
    sf::Color color;
};

#endif /* particle_cmp_h */
