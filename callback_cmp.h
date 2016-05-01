#ifndef CALLBACK_CMP_H_INCLUDED
#define CALLBACK_CMP_H_INCLUDED

#include <functional>
#include <map>
#include <string>

#include"component.h"

class CallbackCmp : public Component{
    std::map<std::string,std::function>callbacks;
};

#endif // CALLBACK_CMP_H_INCLUDED
