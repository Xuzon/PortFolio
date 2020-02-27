#ifdef _WIN32
#include <windows.h>
#endif

#include <ctime>
#include <stdexcept>
#include <cassert>
#include <algorithm>

#include "gameworld.h"
#include "ogro.h"
#include "player.h"
#include "collider.h"
#include "landscape.h"
#include "glslshader.h"
#include "camera.h"
#include "keyboardinterface.h"
#include "mouseinterface.h"
#include "rocket.h"
#include "explosion.h"
#include "tree.h"
#include "frustum.h"

using std::list;
using std::string;
const std::string TERRAIN_HEIGHTMAP = "data/island.raw";

GameWorld::GameWorld(KeyboardInterface* keyboardInterface, MouseInterface* mouseInterface):
m_entities(list<Entity*>()),
m_colliders(list<Collider*>()),
m_player(NULL),
m_landscape(NULL),
m_gameCamera(NULL),
m_keyboard(keyboardInterface),
m_mouse(mouseInterface),
m_lastSpawn(0),
m_currentTime(0),
m_relX(0),
m_relY(0)
{
    m_gameCamera = std::auto_ptr<Camera>(new Camera());
    m_frustum = std::auto_ptr<Frustum>(new Frustum());
}

GameWorld::~GameWorld()
{

    //Free any allocated memory
    for (list<Entity*>::iterator entity = m_entities.begin();
         entity != m_entities.end(); ++entity)
    {
        delete (*entity);
        (*entity) = NULL;
    }

    //Clear the entities and colliders
    m_entities.clear();
    m_colliders.clear();
}

/**
    Spawns a new game entity. This is not the best approach
    as you have to edit this code for each new entity type, but
    I'm going for simplicity rather than extensibility.

    One possible more extensible approach is to
    make this a template function like so:

    template <typename T>
    Entity* spawnEntity() {
        Entity* newEntity = new T();
        if (newEntity->getCollider())
            registerCollider(newEntity->getCollider());
        }
        registerEntity(newEntity);

        return newEntity;
    }

    You could then do:

    Entity* ogro = spawnEntity<Ogro>();
    Entity* player = spawnEntity<Player>();
*/
Entity* GameWorld::spawnEntity(EntityType entityType)
{
    Entity* newEntity = NULL;
    bool initialize = true;
    switch(entityType)
    {
        case OGRO:
            //In the case of the ogro, we can reuse old dead ones
            newEntity = findDeadEnemy();
            if (newEntity)
            {
                (dynamic_cast<Enemy*>(newEntity))->bringToLife();
                initialize = false;
            }
            else
            {
                newEntity = new Ogro(this);
            }

            m_lastSpawn = m_currentTime;
        break;
        case PLAYER:
            if (m_player)
            {
                throw std::invalid_argument("Attempted to spawn a second player");
            }
            newEntity = new Player(this);
            m_player = dynamic_cast<Player*>(newEntity);
        break;
        case LANDSCAPE:
        {
            if (m_landscape)
            {
                throw std::invalid_argument("Attempted to spawn a second landscape");
            }
            //If GLSL 1.30 is supported use that shader, otherwise use the glsl1.20 shader
            string vertexShader = (GLSLProgram::glsl130Supported()) ? "data/shaders/glsl1.30/terrain.vert" : "data/shaders/glsl1.20/terrain.vert";
            string fragShader = (GLSLProgram::glsl130Supported()) ? "data/shaders/glsl1.30/terrain.frag" : "data/shaders/glsl1.20/terrain.frag";

            string waterVert = (GLSLProgram::glsl130Supported()) ? "data/shaders/glsl1.30/water.vert" : "data/shaders/glsl1.20/water.vert";
            string waterFrag = (GLSLProgram::glsl130Supported()) ? "data/shaders/glsl1.30/water.frag" : "data/shaders/glsl1.20/water.frag";
            newEntity = new Landscape(this, TERRAIN_HEIGHTMAP, vertexShader, fragShader, waterVert, waterFrag);
            m_landscape = dynamic_cast<Landscape*>(newEntity);
        }
        break;
        case ROCKET:
            newEntity = new Rocket(this);
        break;
        case EXPLOSION:
            newEntity = new Explosion(this, 250);
        break;
        case TREE:
            newEntity = new Tree(this);
        break;
        default:
            throw std::invalid_argument("Attempted to spawn an invalid entity");
    }

    if (initialize && !newEntity->initialize())
    {
        delete newEntity;
        throw std::runtime_error("Could not initialize one of the entities");
    }

    //If this entity has a collider (i.e. responds to physics) then
    //register it as such
    if (newEntity->getCollider())
    {
        registerCollider(newEntity->getCollider());
    }

    registerEntity(newEntity);

    return newEntity;
}

bool GameWorld::initialize()
{
    srand((unsigned int)time(0));

    spawnEntity(LANDSCAPE); //Spawn the landscape

    //Spawn a load of monsters
    for (unsigned int i = 0; i < MAX_ENEMY_COUNT; ++i)
    {
        Entity* newEntity = spawnEntity(OGRO);
        newEntity->setPosition(getRandomPosition());
    }

    for (int i = 0; i < TREE_COUNT; ++i)
    {
        Entity* newEntity = spawnEntity(TREE);

        Vector3 pos(0.0f, -1.0f, 0.0f);
        while (pos.y < 1.1f) {
            pos = getRandomPosition();
        }

        newEntity->setPosition(pos);
    }

    //Spawn the player and center them
    spawnEntity(PLAYER);
    getPlayer()->setPosition(Vector3(10.0f, 0.0f, 0.0f));
    m_gameCamera->attachTo(getPlayer()); //Attach the camera to the player

    m_remainingTime = 60.0f * 5; //5 minutes

    return true;
}

void GameWorld::update(float dT)
{
    m_currentTime += dT; //Update the time since we started
    m_remainingTime -= dT;

    for (EntityIterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
    {
        (*entity)->prepare(dT);
    }

    //Perform all the collisions
    Collider::updateColliders(m_colliders);
    clearDeadEntities(); //Remove any entities that were killed as a result of a collision

    //Spawn an entity every 10 seconds if we have room
    if (getOgroCount() < MAX_ENEMY_COUNT && (m_currentTime - m_lastSpawn) > 10.0f)
    {
        spawnEntity(OGRO)->setPosition(getRandomPosition());
    }

    /*
    This next section of code allows for really smooth mouse movement no matter
    what the frame rate (well... within reason). Basically the last 10 positions
    are stored in a list. Each frame a new relative position is added and the oldest is removed.
    These positions are then added together using a weighting value. The most recent position
    has the most weight, whereas the oldest as the lowest weighting. This value is then averaged
    to produce the new relative mouse position. The difference this makes to the experience is unbelievable.
    */

    static std::list<std::pair<float, float> > mousePositionHistory;

    int x, y;
    m_mouse->getMousePos(x, y);
    m_mouse->showCursor(false);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    //Add the current mouse position - starting position
    mousePositionHistory.push_front(std::make_pair((float)x - (viewport[2] / 2), (float)y - (viewport[3] / 2)));
    if (mousePositionHistory.size() > 10)
    {
        //Make sure only the last 10 positions are stored
        mousePositionHistory.pop_back();
    }

    m_relX = 0.0f;
    m_relY = 0.0f;
    float weight = 1.0f;

    //Calculate a weighted average
    for (std::list<std::pair<float, float> >::iterator i = mousePositionHistory.begin();
        i != mousePositionHistory.end(); ++i)
    {
        m_relX += (*i).first * weight;
        m_relY += (*i).second * weight;
        weight *= 0.5f;
    }

    m_relX /= 10.0f;
    m_relY /= 10.0f;
    //m_relX = x - (viewport[2] / 2);
    //m_relY = y - (viewport[3] / 2);

    //Put the mouse in the middle of the screen
    m_mouse->setMousePos(viewport[2] / 2, viewport[3] / 2);

}

void GameWorld::render() const
{
    m_gameCamera->apply();
    m_frustum->updateFrustum();

    for (ConstEntityIterator entity = m_entities.begin(); entity != m_entities.end(); ++entity)
    {
        Vector3 pos = (*entity)->getPosition();
        if ((*entity)->getType() == LANDSCAPE || (*entity)->getCollider() == NULL)
        {
            (*entity)->render();
            (*entity)->postRender();
        }
        else if (m_frustum->sphereInFrustum(pos.x, pos.y, pos.z, (*entity)->getCollider()->getRadius()))
        {
            (*entity)->render();
            (*entity)->postRender();
        }
    }
}

Vector3 GameWorld::getRandomPosition() const
{
    float minX = getLandscape()->getTerrain()->getMinX();
    float mapWidth = getLandscape()->getTerrain()->getMaxX() - minX;

    float randX = minX + ((rand() / ((float)RAND_MAX + 1)) * mapWidth);
    float randZ = minX + ((rand() / ((float)RAND_MAX + 1)) * mapWidth);
    float y = getLandscape()->getTerrain()->getHeightAt(randX, randZ);

    return Vector3(randX, y, randZ);
}

void GameWorld::clearDeadEntities()
{
    for (EntityIterator entity = m_entities.begin(); entity != m_entities.end();)
    {
        if (!(*entity)->canBeRemoved())
        {
            ++entity;
            continue;
        }

        unregisterCollider((*entity)->getCollider());
        delete (*entity);
        entity = m_entities.erase(entity);
    }
}

void GameWorld::unregisterCollider(const Collider* collider)
{
    if (collider == NULL) {
        return;
    }

    for(ColliderIterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
    {
        if (collider == (*it)) {
            m_colliders.erase(it);
            break;
        }
    }
}

void GameWorld::registerCollider(Collider* collider)
{
    if (std::find(m_colliders.begin(), m_colliders.end(), collider) != m_colliders.end())
    {
        return;
    }

    m_colliders.push_back(collider);
}

void GameWorld::registerEntity(Entity* entity)
{
    if (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end())
    {
        return;
    }
    m_entities.push_back(entity);
}
