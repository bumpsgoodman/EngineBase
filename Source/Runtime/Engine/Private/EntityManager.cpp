#include "EnginePCH.h"

EntityManager::~EntityManager()
{

}

Entity EntityManager::CreateEntity()
{
	return { mId++ };
}