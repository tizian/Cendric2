#include "Screens/MapScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

const std::string vertexShader = \
"void main()" \
"{" \
"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;" \
"    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;" \
"    gl_FrontColor = gl_Color;" \
"}";

const std::string lightFragmentShader = \
"uniform sampler2D texture;" \
"uniform float ambientLevel;" \
"" \
"void main()" \
"{" \
"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
"    float lightLevel = max(0.0, ambientLevel - pixel.r);" \
"    gl_FragColor = vec4(0.0, 0.0, 0.0, lightLevel);" \
"}";

const std::string foregroundFragmentShader = \
"uniform sampler2D texture;" \
"uniform float ambientLevel;" \
"" \
"void main()" \
"{" \
"    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
"    if (pixel.a > 0.0)" \
"        gl_FragColor = vec4(0.0, 0.0, 0.0, ambientLevel);" \
"    else" \
"        gl_FragColor = pixel;" \
"}";

MapScreen::MapScreen(const std::string& mapID, CharacterCore* core) : GameScreen(core)
{
	m_mapID = mapID;

	m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_lightLayerShader.setParameter("texture", sf::Shader::CurrentTexture);
	m_lightLayerShader.loadFromMemory(vertexShader, lightFragmentShader);

	m_foregroundLayerShader.setParameter("texture", sf::Shader::CurrentTexture);
	m_foregroundLayerShader.loadFromMemory(vertexShader, foregroundFragmentShader);
}

Screen* MapScreen::update(const sf::Time& frameTime)
{
	// handle case where a dialogue is open
	if (m_dialogueWindow != nullptr)
	{
		if (!m_dialogueWindow->updateDialogue())
		{
			delete m_dialogueWindow;
			m_dialogueWindow = nullptr;
		}
		updateProgressLog(frameTime);
		updateObjects(GameObjectType::_Light, frameTime);
		return this;
	}
	else
	{
		GameScreen::update(frameTime);
		if (g_inputController->isKeyJustPressed(Key::Escape))
		{
			// store pos & go back to menu screen
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			return new MenuScreen(m_characterCore);
		}
		if (g_inputController->isKeyJustPressed(Key::Quickload))
		{
			// store pos & go back to menu screen
			CharacterCore* newCharacterCore = new CharacterCore();
			if (!newCharacterCore->quickload())
			{
				// no quicksave exists
				setTooltipText(g_textProvider->getText("NoQuicksaveExists"), sf::Color::Red, true);
				delete newCharacterCore;
			}
			else
			{
				delete m_characterCore;
				m_characterCore = newCharacterCore;
				return new LoadingScreen(m_characterCore);
			}
		}
		if (g_inputController->isKeyJustPressed(Key::Quicksave))
		{
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			m_characterCore->quicksave();
			setTooltipText(g_textProvider->getText("GameSaved"), sf::Color::Green, true);
		}
		MapExitBean* bean = m_currentMap.checkLevelEntry((*m_mainChar->getBoundingBox()));
		if (bean == nullptr || m_isOnLevelEntry)
		{
			m_isOnLevelEntry = (bean != nullptr);
			updateObjects(GameObjectType::_MainCharacter, frameTime);
			updateObjects(GameObjectType::_NPC, frameTime);
			updateObjects(GameObjectType::_Light, frameTime);
			updateTooltipText(frameTime);
			deleteDisposedObjects();
			return this;
		}
		else
		{
			m_characterCore->setMap(m_mainChar->getPosition(), m_currentMap.getID());
			m_characterCore->setLevel(bean->levelSpawnPoint, bean->levelID);
			delete bean;
			return new LoadingScreen(getCharacterCore());
		}
	}
}

void MapScreen::loadForRenderTexture()
{
	m_currentMap.loadForRenderTexture(this);
}

void MapScreen::load()
{
	if (!(m_currentMap.load(m_mapID)))
	{
		string errormsg = m_mapID + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		return;
	}
	m_mainChar = new MapMainCharacter(&m_currentMap);
	m_mainChar->setCharacterCore(getCharacterCore());
	addObject(m_mainChar);
	m_currentMap.loadAfterMainChar(this);

	m_interface = new MapInterface(this);
}

void MapScreen::execOnEnter(const Screen *previousScreen)
{
	// nop
}

void MapScreen::execOnExit(const Screen *nextScreen)
{
	m_currentMap.dispose();
}

void MapScreen::setDialogue(const NPCBean& bean)
{
	if (m_dialogueWindow != nullptr)
	{
		delete m_dialogueWindow;
	}

	m_dialogueWindow = new DialogueWindow();
	m_dialogueWindow->load(bean, this);
}

void MapScreen::render(sf::RenderTarget &renderTarget)
{
	// Render map background etc. to window							(Normal map background rendered)
	m_currentMap.drawBackground(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_NPC, renderTarget);
	sf::View adjustedView = renderTarget.getView();

	// Render ambient light level + light sprites to extra buffer	(Buffer contains light levels as grayscale colors)
	m_renderTexture.clear();
	m_renderTexture.setView(adjustedView);
	renderObjects(GameObjectType::_Light, m_renderTexture);
	m_renderTexture.display();

	// Render extra buffer with light level shader to window		(Dimming level + lights added as transparent layer on top of map)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_lightLayerShader.setParameter("ambientLevel", m_currentMap.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_lightLayerShader);

	// Clear extra buffer
	m_renderTexture.clear(sf::Color(0, 0, 0, 0));

	// Render foreground layer to extra buffer
	m_currentMap.drawForeground(m_renderTexture, sf::RenderStates::Default, m_mainChar->getCenter());
	m_renderTexture.display();

	// Render buffer to window										(Normal foreground rendered on top)
	m_sprite.setTexture(m_renderTexture.getTexture());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite);

	//// Render extra buffer with foreground shader to window		(Ambient light level added on top of foreground)
	m_sprite.setTexture(m_renderTexture.getTexture());
	m_foregroundLayerShader.setParameter("ambientLevel", m_currentMap.getDimming());
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_sprite, &m_foregroundLayerShader);

	// Render overlays on top of level; no light levels here		(GUI stuff on top of everything)
	renderTarget.setView(adjustedView);
	renderObjectsAfterForeground(GameObjectType::_MainCharacter, renderTarget);
	renderObjectsAfterForeground(GameObjectType::_NPC, renderTarget);
	renderTooltipText(renderTarget);
	GameScreen::render(renderTarget); // this will set the view to the default view!

	if (m_dialogueWindow != nullptr)
	{
		m_dialogueWindow->render(renderTarget);
	}
	renderTarget.setView(adjustedView);
}