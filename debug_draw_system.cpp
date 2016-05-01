#include"debug_draw_system.h"

void DebugDrawSystem::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	sf::Vector2f center;
	for (int i = 0; i < vertexCount; i++)
	{
		//polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
		sf::Vector2f transformedVec = DebugDrawSystem::B2VecToSFVec(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
	}																								   // they still show up though.. but less frequently
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(DebugDrawSystem::GLColorToSFML(color));

	target->draw(polygon);
}
void DebugDrawSystem::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		//polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
		sf::Vector2f transformedVec = DebugDrawSystem::B2VecToSFVec(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
	}																								   // they still show up though.. but less frequently
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(DebugDrawSystem::GLColorToSFML(color, 60));
	polygon.setOutlineColor(DebugDrawSystem::GLColorToSFML(color));

	target->draw(polygon);
}
void DebugDrawSystem::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	sf::CircleShape circle(radius * SCALE);
	circle.setOrigin(radius * SCALE, radius * SCALE);
	circle.setPosition(DebugDrawSystem::B2VecToSFVec(center));
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(DebugDrawSystem::GLColorToSFML(color));

	target->draw(circle);
}
void DebugDrawSystem::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	sf::CircleShape circle(radius * SCALE);
	circle.setOrigin(radius * SCALE, radius * SCALE);
	circle.setPosition(DebugDrawSystem::B2VecToSFVec(center));
	circle.setFillColor(DebugDrawSystem::GLColorToSFML(color, 60));
	circle.setOutlineThickness(1.f);
	circle.setOutlineColor(DebugDrawSystem::GLColorToSFML(color));

	b2Vec2 endPoint = center + radius * axis;
	sf::Vertex line[2] =
	{
		sf::Vertex(DebugDrawSystem::B2VecToSFVec(center), DebugDrawSystem::GLColorToSFML(color)),
		sf::Vertex(DebugDrawSystem::B2VecToSFVec(endPoint), DebugDrawSystem::GLColorToSFML(color)),
	};

	target->draw(circle);
	target->draw(line, 2, sf::Lines);
}
void DebugDrawSystem::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	sf::Vertex line[] =
	{
		sf::Vertex(DebugDrawSystem::B2VecToSFVec(p1), DebugDrawSystem::GLColorToSFML(color)),
		sf::Vertex(DebugDrawSystem::B2VecToSFVec(p2), DebugDrawSystem::GLColorToSFML(color))
	};

	target->draw(line, 2, sf::Lines);
}
#ifdef __APPLE__
void DebugDrawSystem::DrawTransform(const b2Transform& xf)
{
	const float32 k_axisScale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, red);

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, green);
}
#else
void DebugDrawSystem::DrawTransform(const b2Transform& xf)
{
	/*const float32 k_axisScale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, red);

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, green);*/
}
#endif
