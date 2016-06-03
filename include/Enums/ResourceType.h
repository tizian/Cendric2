#pragma once

enum class ResourceType {
	Unique, // an unique resource will be released as soon as its owner game object is deleted (destructor) 
	Level, // a level resource will live as long as the current level exists
	Map, // a map resource will live as long as the current map exists
	Global // a global resource will only be released when the game ends
};