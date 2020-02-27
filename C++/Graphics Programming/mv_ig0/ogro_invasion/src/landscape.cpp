#ifdef _WIN32
#include <windows.h>
#endif

#include <cassert>
#include "landscape.h"
#include "terraincollider.h"

using std::string;

Landscape::Landscape(GameWorld* world, const string& heightmap, const string& vertexShader, const string& fragmentShader,
                     const std::string& waterVert, const std::string& waterFrag):
Entity(world),
m_terrain(vertexShader, fragmentShader, waterVert, waterFrag),
m_heightmap(heightmap)
{
    m_collider = std::auto_ptr<Collider>(new TerrainCollider(this, &m_terrain));
}

Landscape::~Landscape()
{

}

bool Landscape::onInitialize()
{
    const string grassTexture = "data/textures/grass.tga";
    const string heightTexture = "data/textures/height.tga";
    const string waterTexture = "data/textures/water.tga";
    bool result = m_terrain.loadHeightmap(m_heightmap, grassTexture, heightTexture, 65, true, waterTexture);
    if (result) {
        m_terrain.normalizeTerrain();
        m_terrain.scaleHeights(4.0f);
    }

    return result;
}

void Landscape::onRender() const
{
    m_terrain.render();
    m_terrain.renderWater();
}

void Landscape::onShutdown()
{

}

void Landscape::onCollision(Entity* entity)
{
    assert(entity != NULL);
    assert(entity->getCollider());

    if (entity->canBeRemoved())
    {
        return;
    }

    /*
        On collision with the terrain the entity is moved upwards (if it's not dead)
    */
    Vector3 entityPos = entity->getPosition();
    float terrainHeight = m_terrain.getHeightAt(entityPos.x, entityPos.z);
    float entityRadius = entity->getCollider()->getRadius();

    entityPos.y = terrainHeight + entityRadius;
    entity->setPosition(entityPos);
}

void Landscape::onPrepare(float dT)
{

}

void Landscape::onPostRender()
{

}
