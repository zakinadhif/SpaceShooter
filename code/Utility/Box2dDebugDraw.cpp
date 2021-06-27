#include "Utility/Box2dDebugDraw.hpp"
#include "Utility/ColorConverter.hpp"
#include "Utility/VectorConverter.hpp"
#include "World/UnitScaling.hpp"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

namespace astro
{

Box2dDebugDraw::Box2dDebugDraw(sf::RenderTarget& window)
	: m_window(window)
{
}

void Box2dDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color)
{
	sf::ConvexShape polygon(vertexCount);

	for (int32 i = 0; i < vertexCount; ++i)
	{
		sf::Vector2f vertex = toPixels(vertices[i]);
		polygon.setPoint(i, vertex);
	}

	polygon.setOutlineThickness(-0.05f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(b2ColorToSfColor(color));

	m_window.draw(polygon);
}

void Box2dDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	sf::ConvexShape polygon(vertexCount);

	for (int32 i = 0; i < vertexCount; ++i)
	{
		sf::Vector2f vertex = toPixels(vertices[i]);
		polygon.setPoint(i, vertex);
	}

	b2Color fillColor = color;
	fillColor.a = 0.5;

	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(b2ColorToSfColor(fillColor));
	polygon.setOutlineColor(b2ColorToSfColor(color));

	m_window.draw(polygon);
}

void Box2dDebugDraw::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
{
	sf::CircleShape circle(radius * PPM);
	circle.setOrigin(radius, radius);
	circle.setPosition(center.x, center.y);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(b2ColorToSfColor(color));

	m_window.draw(circle);
}

void Box2dDebugDraw::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
{
	b2Color fillColor = color;
	fillColor.a = 0.5f;

	sf::CircleShape circle(radius * PPM);
	circle.setOrigin(radius, radius);
	circle.setPosition(center.x, center.y);
	circle.setFillColor(b2ColorToSfColor(fillColor));
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(b2ColorToSfColor(color));

	m_window.draw(circle);
}

void Box2dDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
	std::array<sf::Vertex, 2> line =
	{
		{
			{{toPixels(p1)}, b2ColorToSfColor(color)},
			{{toPixels(p2)}, b2ColorToSfColor(color)}
		}
	};

	m_window.draw(line.data(), 2, sf::Lines);
}

void Box2dDebugDraw::DrawTransform(const b2Transform &xf)
{
	float lineLength = 0.4f;

	b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
	
	std::array<sf::Vertex, 2> redLine =
	{
		{
			{toPixels(xf.p), sf::Color::Red},
			{toPixels(xAxis), sf::Color::Red}
		}
	};

	b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();

	std::array<sf::Vertex, 2> greenLine =
	{
		{
			{toPixels(xf.p), sf::Color::Green},
			{toPixels(yAxis), sf::Color::Green}
		}
	};

	m_window.draw(redLine.data(), 2, sf::Lines);
	m_window.draw(greenLine.data(), 2, sf::Lines);
}

void Box2dDebugDraw::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
	sf::Vertex point(toPixels(p), b2ColorToSfColor(color));

	m_window.draw(&point, 1, sf::Points);
}

} // namespace astro
