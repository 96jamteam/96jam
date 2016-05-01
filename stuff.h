#ifndef STUFF_H_INCLUDED
#define STUFF_H_INCLUDED

#include<SFML/Graphics.hpp>
#include <Box2d/Box2d.h>
#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <cstddef>

# define P(x) std::cout<<x<<std::endl ;

# define SNIEGY ;
//https://github.com/SFML/SFML/wiki/Tutorial:-Object-Properties

namespace stuff
{

	static const int MAX_TEXTURES = 100;
	static const int MAX_STYLESHEETS = 10;
	static const int MAX_FONTS = 10;
	static const float SCALE = 30.f;
	static const float PI = 3.141592653589793;

	static const float normalTimestep = 1.f / 60.f;





    static int s2i(const std::string& str)
    {
        return  std::atoi(str.c_str());
    }

	static float random(const float& LO, const float& HI) {
		return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	}

	static float radtodeg(const float& rad)
	{
		return 180.f*rad / 3.141592653589793;
	}

	static float degtorad(const float& deg)
	{
		return 3.141592653589793*deg / 180.f;
	}

	static float distance(const float& x1, const float& y1, const float& x2, const float& y2)
	{
		return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}

	static sf::Vector2f norm_vec(const sf::Vector2f& source)
	{
		float length = sqrt((source.x * source.x) + (source.y * source.y));
		if (length != 0)
			return sf::Vector2f(source.x / length, source.y / length);
		else
			return source;
	}


	class Delegate
	{
		typedef void* InstancePtr;
		typedef void(*InternalFunction)(InstancePtr);
		typedef std::pair<InstancePtr, InternalFunction> Stub;

		// turns a free function into our internal function stub
		template <void(*Function)()>
		static inline void FunctionStub(InstancePtr)
		{
			// we don't need the instance pointer because we're dealing with free functions
			return (Function)();
		}

		// turns a member function into our internal function stub
		template <class C, void (C::*Function)()>
		static inline void ClassMethodStub(InstancePtr instance)
		{
			// cast the instance pointer back into the original class instance
			return (static_cast<C*>(instance)->*Function)();
		}

	public:
		Delegate(void)
			: m_stub(nullptr, nullptr)
		{
		}

		/// Binds a free function
		template <void(*Function)()>
		void Bind(void)
		{
			m_stub.first = nullptr;
			m_stub.second = &FunctionStub<Function>;
		}

		/// Binds a class method
		template <class C, void (C::*Function)()>
		void Bind(C* instance)
		{
			m_stub.first = instance;
			m_stub.second = &ClassMethodStub<C, Function>;
		}

		/// Invokes the delegate
		void Invoke() const
		{
			//assert(m_stub.second != nullptr, "Cannot invoke unbound delegate. Call Bind() first.")();
			return m_stub.second(m_stub.first);
		}

		template <class C, void (C::*Function)()>
		void BindAndInvoke(C* instance)
		{
			m_stub.first = instance;
			m_stub.second = &ClassMethodStub<C, Function>;
			return m_stub.second(m_stub.first);
		}

	private:
		Stub m_stub;
	};


	class FPS
	{
	public:
		/// @brief Constructor with initialization.
		///
		FPS() : mFrame(0), mFps(0) {}

		/// @brief Update the frame count.
		///
		void update()
		{
			if (mClock.getElapsedTime().asSeconds() >= 1.f)
			{
				mFps = mFrame;
				mFrame = 0;
				mClock.restart();
			}

			++mFrame;
		}

		/// @brief Get the current FPS count.
		/// @return FPS count.
		const unsigned int getFPS() const { return mFps; }

	private:
		unsigned int mFrame;
		unsigned int mFps;
		sf::Clock mClock;
	};


	static sf::Color generateRandomColor(const sf::Color& mix)
	{
		int red = rand() % 255;
		int green = rand() % 255;
		int blue = rand() % 255;


		red = (red + mix.r) / 2;
		green = (green + mix.g) / 2;
		blue = (blue + mix.b) / 2;


		sf::Color color = sf::Color(red, green, blue);
		return color;
	}


	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	static std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	static bool chceckVis(const b2Vec2& pos1, const b2Vec2& pos2, b2World* world, bool dynamic, bool threeWay=0)
{
    bool hit(0);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
        {
            if(b->GetType()==b2_dynamicBody && !dynamic)
                break;
            b2RayCastOutput output;
            b2RayCastInput input;
            input.p1=pos1;
            input.p2 =pos2;
            input.maxFraction = 1;

            if ( f->RayCast( &output, input ) )
            {
                hit=true;
            }
            if(threeWay)
            {
                float wide=25.f;
                double ang1=atan2(pos2.y -pos1.y, pos2.x-pos1.x);
                b2Vec2 one,two;
                one=b2Vec2(pos1.x + (wide/SCALE)*cos(ang1-degtorad(90)), pos1.y + (wide/SCALE)*sin(ang1-degtorad(90)));
                two=b2Vec2(pos2.x + (wide/SCALE)*cos(ang1-degtorad(90)), pos2.y + (wide/SCALE)*sin(ang1-degtorad(90)));
                input.p1=one;
                input.p2 =two;
                input.maxFraction = 1;

                if ( f->RayCast( &output, input ) )
                {
                    hit=true;
                }

                one=b2Vec2(pos1.x + (wide/SCALE)*cos(ang1+degtorad(90)), pos1.y + (wide/SCALE)*sin(ang1+degtorad(90)));
                two=b2Vec2(pos2.x + (wide/SCALE)*cos(ang1+degtorad(90)), pos2.y + (wide/SCALE)*sin(ang1+degtorad(90)));
                input.p1=one;
                input.p2 =two;
                input.maxFraction = 1;

                if ( f->RayCast( &output, input ) )
                {
                    hit=true;
                }
            }
        }
    return hit;
}
}





#endif // STUFF_H_INCLUDED
