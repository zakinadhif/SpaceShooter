#include "UserInterface/Starfield.hpp"

#include "Utility/Random.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <cmath>

namespace
{
	sf::Color calculateDimmedColor(sf::Color color, float dimmingFactor)
	{
		assert(dimmingFactor >= 0.f);

		return
		{
			sf::Uint8(std::floor(color.r * (1.f / (dimmingFactor + 1.f)))),
			sf::Uint8(std::floor(color.g * (1.f / (dimmingFactor + 1.f)))),
			sf::Uint8(std::floor(color.b * (1.f / (dimmingFactor + 1.f))))
		};
	}
}

namespace astro
{

Starfield::Starfield()
	: m_baseColor{255, 255, 255}
{

}

void Starfield::setSize(sf::Vector2f size)
{
	m_size = size;
	m_needRegeneration = true;
}

void Starfield::setDirection(Direction direction)
{
	m_direction = direction;
	m_needRegeneration = true;
}

void Starfield::addLayer(float speed, float dimmingFactor)
{
	sf::Vector2f movementVelocity = determineVelocity(speed, m_direction);
	auto [layer, layerVelocity] = generateLayer(m_baseColor, m_size, movementVelocity, dimmingFactor);

	m_layers.push_back(layer);
	m_layerVelocities.push_back(layerVelocity);
}

void Starfield::setLayer(std::size_t index, float speed, float dimmingFactor)
{
	sf::Vector2f movementVelocity = determineVelocity(speed, m_direction);
	auto [layer, layerVelocity] = generateLayer(m_baseColor, m_size, movementVelocity, dimmingFactor);

	m_layers.at(index) = layer;
	m_layerVelocities.at(index) = layerVelocity;
}

void Starfield::removeLayer(std::size_t index)
{
	m_layers.erase(m_layers.begin() + index);
	m_layerVelocities.erase(m_layerVelocities.begin() + index);
}

sf::Vector2f Starfield::determineVelocity(float speed, Direction direction)
{
	// Calculate view movement velocity
	sf::Vector2f velocity;

	switch (direction)
	{
		case Direction::Up:
			velocity = {0.f, speed};
			break;
		case Direction::Down:
			velocity = {0.f, -speed};
			break;
		case Direction::Left:
			velocity = {speed, 0.f};
			break;
		case Direction::Right:
			velocity = {-speed, 0.f};
			break;
	}

	return velocity;
}

float Starfield::determineSpeed(sf::Vector2f velocity)
{
	if (velocity.x == 0)
	{
		return std::abs(velocity.y);
	}

	return std::abs(velocity.x);
}

std::tuple<Starfield::Layer, Starfield::LayerVelocity>
Starfield::generateLayer(sf::Color color, sf::Vector2f size, sf::Vector2f viewVelocity, float dimmingFactor)
{
	// Generate starfield
	const sf::Vector2f starfieldSize     {size.x, size.y};
	const sf::Vector2f starfieldViewSize {size.x, size.y};

	const sf::Color starfieldColor = calculateDimmedColor(color, dimmingFactor);

	sf::VertexArray starfield = generateStars(starfieldColor, starfieldSize);
	sf::View starfieldView({0, 0, starfieldViewSize.x, starfieldViewSize.y});

	return {{starfield, starfieldView}, {viewVelocity}};
}

sf::VertexArray Starfield::generateStars(sf::Color color, sf::Vector2f size)
{
	const std::size_t starCount = 250u;

	sf::VertexArray stars;
	stars.setPrimitiveType(sf::Points);
	stars.resize(starCount);

	for (std::size_t x = 0; x < starCount; ++x)
	{
		sf::Vector2f starPosition {
			zfge::Random::getFloat(0, size.x),
			zfge::Random::getFloat(0, size.y)
		};

		stars[x].color = color;
		stars[x].position = starPosition;
	}

	return stars;
}

void Starfield::regenerateAllLayers()
{
	// Preserve star colors
	std::vector<sf::Color> colors;

	for (const auto& [starfield, _] : m_layers)
	{
		if (starfield.getVertexCount() == 0)
		{
			continue;
		}

		colors.push_back(starfield[0].color);
	}

	// Preserve star velocities
	std::vector<sf::Vector2f> velocities = m_layerVelocities;

	// Rotate all star velocity vectors to specified direction
	for (auto& velocity : velocities)
	{
		float speed = determineSpeed(velocity);
		velocity = determineVelocity(speed, m_direction);
	}

	// Regenerate all layers
	std::size_t layerCount = m_layers.size();

	m_layers.clear();
	m_layerVelocities.clear();

	for (std::size_t x = 0; x < layerCount; ++x)
	{
		auto [layer, layerVelocity] = generateLayer(colors[x], m_size, velocities[x]);
		m_layers.push_back(layer);
		m_layerVelocities.push_back(layerVelocity);
	}
}

sf::View Starfield::calculateComplementaryView(sf::View view, sf::Vector2f velocity)
{
	sf::View complementaryView(view);

	const sf::Vector2f viewCenter = view.getCenter();
	const sf::Vector2f viewSize = view.getSize();

	if (velocity.y < 0.f)
	{
		complementaryView.setCenter(viewCenter.x, viewCenter.y - viewSize.y);
	}
	else if (velocity.y > 1.f)
	{
		complementaryView.setCenter(viewCenter.x, viewCenter.y + viewSize.y);
	}
	else if (velocity.x < 0.f)
	{
		complementaryView.setCenter(viewCenter.x + viewSize.x, viewCenter.y);
	}
	else if (velocity.x > 0.f)
	{
		complementaryView.setCenter(viewCenter.x - viewSize.x, viewCenter.y);
	}

	return complementaryView;
}

bool Starfield::isViewOutsideStarfield(sf::View view, sf::Vector2f size)
{

}

void Starfield::update(float deltaTime)
{
	if (m_needRegeneration)
	{
		regenerateAllLayers();
		m_needRegeneration = false;
	}

	for (std::size_t x = 0; x < m_layers.size(); ++x)
	{
		const auto velocity = m_layerVelocities.at(x);
		auto& [_, view] = m_layers.at(x);

		view.move(velocity.x * deltaTime, velocity.y * deltaTime);
	}
}

void Starfield::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::View oldView = target.getView();

	for (const auto& [starfield, view] : m_layers)
	{
		target.setView(view);
		target.draw(starfield);
	}

	target.setView(oldView);
}

}
