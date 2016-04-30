#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "eventchannel.h"

namespace sf { class Sound; class Music; }

class SoundSystem : public System {
private:
    EventChannel mChannel;
public:
    SoundSystem(): System(){
        musicVolume=0;
        soundVolume=0;
        mChannel.add<SetListener>(*this);
        mChannel.add<PlaySound>(*this);
        mChannel.add<AddMusic>(*this);
        mChannel.add<RegisterMusicCallback>(*this);
        mChannel.add<StartMusic>(*this);
        mChannel.add<StopMusic>(*this);
        mChannel.add<Volume>(*this);
	}

	virtual ~SoundSystem() {
		for (auto it = mPlaying.begin(); it != mPlaying.end(); mPlaying.erase(it++))
			delete *it;
		for (auto it = mMusic.begin(); it != mMusic.end(); mMusic.erase(it++))
			delete it->second;

	}


	virtual void update() {
		if (!mCurrentSong.empty() && mMusic[mCurrentSong]->getStatus() == sf::Music::Stopped)
			startMusic();


		mUpdated += clock.restart().asSeconds();

		if (mUpdated > 0.33f)
		{
			mUpdated = 0;

			for (auto it = mPlaying.begin(); it != mPlaying.end();)
			{
				if ((*it)->getStatus() == sf::Sound::Stopped)
				{
					delete *it;
					mPlaying.erase(it++);
				}
				else
					++it;
			}
		}
	}

	void handle(const Volume& hand) {
		musicVolume = hand.music;
		soundVolume = hand.sound;
	}

	void handle(const SetListener& hand) {
		sf::Listener::setPosition(hand.pos.x, hand.pos.y, 0);
		sf::Listener::setDirection(cos(hand.dir), sin(hand.dir), 0);
	}

	void handle(const PlaySound& hand) {
		bool loaded = mBuffers.count(hand.sound) > 0;
		sf::SoundBuffer& buf = mBuffers[hand.sound];
		if (!loaded)
			if (!buf.loadFromFile(hand.sound)){
				mBuffers.erase(hand.sound);
				return;
			}

		sf::Sound* snd = new sf::Sound();
		snd->setBuffer(buf);

		if (hand.pos != sf::Vector2f())
		{
			snd->setRelativeToListener(true);
			snd->setPosition(hand.pos.x, hand.pos.y, 0);
		}

		snd->setVolume(soundVolume);

		snd->play();
	}

	void handle(const AddMusic& hand) {
		std::string name = hand.n;
		if (name.empty())
			name = hand.file;

		mMusic[name] = new sf::Music();
		sf::Music* m = mMusic[name];

		if (!m->openFromFile(hand.file)){
			mMusic.erase(name);
			return;
		}
	}

	void handle(const RegisterMusicCallback& hand) {
		mMusicCallback = hand.callback;
	}

	void handle(const StartMusic& hand) {
		startMusic();
	}

	void handle(const StopMusic& hand) {
		mMusic[mCurrentSong]->stop();
		mCurrentSong = "";
	}

private:
	void startMusic(){
		if (!mCurrentSong.empty() && mMusic[mCurrentSong]->getStatus() != sf::Music::Stopped)
			mMusic[mCurrentSong]->stop();

		auto it = mMusic.begin();

		if (mMusic.size() > 1) {
			do
			{
				int id = rand() % mMusic.size(), i = 0;

				for (; it != mMusic.end() && i < id; ++it, ++i)
					;

				if (it == mMusic.end())
					return;
			} while (it->first == mCurrentSong);
		}

		mCurrentSong = it->first;
		it->second->play();
		it->second->setVolume(musicVolume);

		if (mMusicCallback)
			mMusicCallback(mCurrentSong);
	}

	float mUpdated, musicVolume, soundVolume;
	std::unordered_map<std::string, sf::SoundBuffer> mBuffers;
	std::string mCurrentSong;
	std::unordered_map<std::string, sf::Music*> mMusic;
	std::list<sf::Sound*> mPlaying;
	std::function<void(const std::string&)> mMusicCallback;
	sf::Clock clock;
};
