#ifndef CALLBACK_CMP_H_INCLUDED
#define CALLBACK_CMP_H_INCLUDED

#include <functional>
#include <map>
#include <string>

#include"component.h"

class CallbackCmp : public Component{
public:
    std::map<std::string,std::function<void()>>callbacks;
};

#endif // CALLBACK_CMP_H_INCLUDED
