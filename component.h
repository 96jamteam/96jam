#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

class Component {
public:
	unsigned entityID;
	bool active = true;
	virtual ~Component(){}
	virtual void destroy(){}
};

#endif // COMPONENT_H_INCLUDED
