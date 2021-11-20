#pragma once

#include <cstdint>
namespace astro
{

struct GameStateComponent
{
	float score = 0.f;
	int health = 3.f;

	int screenWidth = 0.f;
	int screenHeight = 0.f;

	uint32_t selectedEntity = 0;
};

/*
 * How do you achieve reflectiveness?
 *
 * In runtime, the software needs to be able to query a struct/class's
 * structure so that it can do any operations with it appropriately.
 * Such can be achieved by making a type:variable_address table.
 *
 * - Score
 *   - Type: Float
 *   - Getter (address, offset, getter function): &GameStateComponent::MemberVariable(of instance)
 *   - Setter (address, offset, setter function): %GameStateComponent::MemberVariable(of instance)
 */

}
