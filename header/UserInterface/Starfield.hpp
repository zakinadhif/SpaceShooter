#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>

namespace astro
{

class Starfield : public sf::Drawable
{
public:
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};

public:
	Starfield();

	void setSize(sf::Vector2f size);
	void setDirection(Direction direction);

	void addLayer(float speed, float dimmingFactor);
	void setLayer(std::size_t index, float speed, float dimmingFactor);
	void removeLayer(std::size_t index);

	void update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	typedef std::tuple<sf::VertexArray, sf::View> Layer;
	typedef sf::Vector2f LayerVelocity;

	std::vector<Layer> m_layers;
	std::vector<LayerVelocity> m_layerVelocities;

	sf::Color m_baseColor {sf::Color::White};
	sf::Vector2f m_size   {800, 800};
	Direction m_direction {Direction::Down};

	bool m_needRegeneration = true;

	std::tuple<Layer, LayerVelocity> generateLayer(
		sf::Color color,
		sf::Vector2f size,
		sf::Vector2f viewVelocity,
		float dimmingFactor = 0.f
	);
	void regenerateAllLayers();

	sf::Vector2f determineVelocity(float speed, Direction direction) const;
	float determineSpeed(sf::Vector2f velocity) const;

	sf::VertexArray generateStars(sf::Color color, sf::Vector2f size) const;

	sf::View calculateComplementaryView(sf::View view, sf::Vector2f velocity) const;
	bool isViewOutsideStarfield(sf::View view, sf::Vector2f size) const;

	sf::Vector2f calculateViewStartPosition(sf::Vector2f velocity, sf::Vector2f size) const;
};

}
