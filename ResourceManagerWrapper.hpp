#ifndef RESOURCEMANAGERWRAPPER_HPP_INCLUDED
#define RESOURCEMANAGERWRAPPER_HPP_INCLUDED

#include"ResourceManager.hpp"
#include"stuff.h"

template <class T, int x>
class Manager : public ResourceManager<T, std::string, x>
{
public:

    typedef Resource<T, std::string, x> res;
    void addTranslation(std::string name,std::string path)
    {
        translation[name]=path;
    }
    
    

protected:
    virtual bool Load(T &resource, const std::string &name) const
    {
        return resource.loadFromFile( translation.at(name) );
    }
private:
    std::map<std::string,std::string> translation;
};

#endif // RESOURCEMANAGERWRAPPER_HPP_INCLUDED
