
#pragma once

#include <cstdio>
#include <cmath>
#include <map>
#include <cmath>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <iostream>


#include <list>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <map>
using namespace std;


// Graphic Engine SFML
#include <SFML/Graphics.hpp>
using namespace sf;

// own classes

#include "TileMap.h"
#include "Level.h"
#include "LevelReader.h"
#include "InputController.h"

#include "Screen.h"
#include "Screens/SplashScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/GameScreen.h"
#include "Animation.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "Object.h"
#include "ScreenManager.h"
#include "GameObject.h"
#include "Game.h"
#include "MainCharacter.h"

// extern objects
extern ResourceManager* g_resourceManager;
extern InputController* g_inputController;


