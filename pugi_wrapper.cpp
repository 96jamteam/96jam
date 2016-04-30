#include "pugi_wrapper.h"

void XML::load(const std::string& s)
{
    pugi::xml_parse_result result = this->doc.load_file(s.c_str());
    
	node = doc;
}

pugi::xml_node XML::get_internal(const std::string& s, std::string* attr)
{
	std::vector<std::string> vec = stuff::split(s, '.');

	pugi::xml_node current_node = node;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec.at(i).find(":") == std::string::npos) // nie ma atrybutu
		{
			current_node = current_node.child(vec.at(i).c_str()); // c_str = "c++ to gówno";
		}
		else // jest
		{
			std::vector<std::string> vec2 = stuff::split(vec.at(i), ':');
            if(vec2.at(0) != ""){
                *attr = vec2.at(1);
                current_node = current_node.child(vec2.at(0).c_str());
            }else{
            
                *attr = vec2.at(1);
            }
		}
	}

	return current_node;
}
