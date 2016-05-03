#ifndef PARTICLE_CREATOR_H_INCLUDED
#define PARTICLE_CREATOR_H_INCLUDED

#include <map>
#include <functional>

class ParticleCreator {
public:
	static int createParticle(const int& formula) {
	    if(callbacks.find(formula)!=callbacks.end())
            return callbacks[formula]();
        else
            return -1;
	}

	static void addFormula(const int& formula, const std::function<int()>& call ) {
        callbacks[formula]=call;
	}

private:
    static std::map<int,std::function<int()>>callbacks;
};

#endif // PARTICLE_CREATOR_H_INCLUDED
