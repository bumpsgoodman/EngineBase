#pragma once

class ComponentManager final
{
public:
	ComponentManager() = default;
	ComponentManager(const ComponentManager&) = delete;
	ComponentManager& operator=(const ComponentManager&) = delete;
	ComponentManager(ComponentManager&&) = default;
	ComponentManager& operator=(ComponentManager&&) = default;

private:

};