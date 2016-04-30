#pragma once

#include <sstream>
#include <memory>

#include "pugixml.hpp"
#include "stuff.h"

class XML{
    
    pugi::xml_node node;
    pugi::xml_document doc;

    pugi::xml_node get_internal(const std::string& s, std::string* attr);
public:
    template <class T>
    T get(const std::string& s) {
        std::string attr_name(""); //to trochê messy ale... c++ to gówno i da siê zwróciæ tylko jedn¹ rzecz na raz XD // ciiiichhooo, jest ok
        pugi::xml_node current_node = this->get_internal(s, &attr_name);
        std::string value;
        
        if (attr_name == "")
            value = current_node.child_value();
        else
            value = current_node.attribute(attr_name.c_str()).as_string();
        std::istringstream ss(value);
        T x;
        ss >> x;
        return x;
    }
    
    std::string getName(){return node.name();}
    
    std::vector<std::shared_ptr<XML>> iter(const std::string& s) {
        
        std::vector<std::shared_ptr<XML>> nodes;
        if (s != "") {
            for (pugi::xml_node n : node.child(s.c_str()).children()) {
                std::shared_ptr<XML> tmpnode = std::make_shared<XML>();
                tmpnode->setNode(n);
                nodes.push_back(tmpnode);
            }
        }else {
            for (pugi::xml_node n : node.children()) {
                std::shared_ptr<XML> tmpnode = std::make_shared<XML>();
                tmpnode->setNode(n);
                nodes.push_back(tmpnode);
            }
        }
        return nodes;
    }
    
    void load(const std::string&);
    void setNode(pugi::xml_node& n){
        node = n;
    }
};