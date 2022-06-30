#pragma once

class EntityManager final
{
public:
	EntityManager() = default;
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager(EntityManager&&) = default;
	EntityManager& operator=(EntityManager&&) = default;
	~EntityManager();

	Entity CreateEntity();

private:
	Uint32 mId = 0;
};