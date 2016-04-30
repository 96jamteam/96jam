#pragma once

#include<SFML/Graphics.hpp>
#include<string>

struct MousePos {
	MousePos(const float& _x, const float& _y) : x(_x), y(_y) { sf::Keyboard::Key k = sf::Keyboard::Key::A; }
	float x, y;
};

struct KeyEvent {
	KeyEvent(const sf::Keyboard::Key& _key, const bool& _pressed) : key(_key), pressed(_pressed) {}
	bool pressed;
	sf::Keyboard::Key key;
};

struct MouseButtonEvent {
	MouseButtonEvent(const sf::Mouse::Button& _button, const bool& _pressed) : button(_button), pressed(_pressed) {}
	bool pressed;
	sf::Mouse::Button button;
};

struct MenuEvent {
	enum Type { hide, show };
	MenuEvent(const std::string& _menu_name, const Type& t) : type(t), menu_name(_menu_name) {}
	Type type;
	std::string menu_name;
};

struct SetMenuScreenEvent {
	SetMenuScreenEvent(const std::string& _menu_name, const std::string& _set_screen) : set_screen(_set_screen), menu_name(_menu_name) {}
	std::string menu_name;
	std::string set_screen;
};

struct BulletTime {
	BulletTime(const float& _factor) : factor(_factor) {}
	float factor;
};

struct SpriteAdded {
	SpriteAdded() {}
};

struct LoadWorld {
	LoadWorld(const std::string& _type, const int& _id) : type(_type), id(_id) {}
	std::string type;
	int id;
};

struct PhysicsUpdate {
	PhysicsUpdate(const float& _timestep) : timestep(_timestep) {}
	float timestep;
};


struct SetListener {
	SetListener(const sf::Vector2f& _pos, const float& _dir) :pos(_pos), dir(_dir) {}
	sf::Vector2f pos;
	float dir;
};

struct PlaySound {
	PlaySound(const std::string& _sound, const sf::Vector2f& _pos = sf::Vector2f()) :pos(_pos), sound(_sound) {}
	sf::Vector2f pos;
	std::string sound;
};

struct AddMusic {
	AddMusic(const std::string& _file, const std::string& _n = "") :file(_file), n(_n) {}
	std::string n, file;
};

struct RegisterMusicCallback {
	RegisterMusicCallback(const std::function<void(const std::string&)>& _callback) :callback(_callback) {}
	std::function<void(const std::string&)> callback;
};

struct StartMusic {
	StartMusic() {}
};

struct StopMusic {
	StopMusic() {}
};

struct Volume {
	Volume(const float& _music, const float& _sound) : music(_music), sound(_sound) {}
	float music,sound;
};

struct SceneUpdate {
	SceneUpdate(){}
};