#include <irrlicht.h>

#include "MyEventReceiver.h"
#include "Player.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main() {
    MyEventReceiver receiver;

    IrrlichtDevice* device =
        createDevice(video::EDT_SOFTWARE, dimension2d<u32>(800, 600), 16,
            false, false, false, &receiver);

    if (!device)
        return 1;

    device->setWindowCaption(L"Side Scroller Game");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        rect<s32>(10, 10, 260, 22), true);

    // Camera
    ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0, 20, 20), vector3df(0, 0, 0));
    camera->setFarValue(1000.0f);
    camera->setNearValue(1.0f);

    // Player
    Player player = Player(vector3df(0, 10, 0));

    // Player Mesh
    IMesh* playerMesh = smgr->getMesh("assets/player/player.obj");
    if (!playerMesh) {
        device->drop();
        return 1;
    }
    IMeshSceneNode* playerNode = smgr->addMeshSceneNode(playerMesh);
    if (playerNode) {
        playerNode->setMaterialFlag(EMF_LIGHTING, false);
        playerNode->setPosition(player.position);
    }
    // Player Collision
    ISceneNodeAnimatorCollisionResponse* playerCollision;

    // Map
    IMesh* mapMesh = smgr->getMesh("assets/map/map.obj");
    if (!mapMesh) {
        device->drop();
        return 1;
    }
    IMeshSceneNode* mapNode = smgr->addMeshSceneNode(mapMesh);
    if (mapNode) {
        mapNode->setMaterialFlag(EMF_LIGHTING, false);
    }

    // Add Collision
    ITriangleSelector* world = smgr->createOctreeTriangleSelector(mapNode->getMesh(), mapNode);
    playerCollision = smgr->createCollisionResponseAnimator(world, playerNode, vector3df(1, 2, 1), vector3df(0, -2.0f, 0));
    playerNode->addAnimator(playerCollision);
    playerCollision->drop();

    // Time
    u32 then = device->getTimer()->getTime();

    while (device->run()) {
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.0f;
        then = now;

        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        smgr->drawAll();
        guienv->drawAll();

        if (receiver.isKeyDown(KEY_KEY_D)) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X - (player.speed * frameDeltaTime), playerNode->getPosition().Y, playerNode->getPosition().Z));
        }
        if (receiver.isKeyDown(KEY_KEY_A)) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X + (player.speed * frameDeltaTime), playerNode->getPosition().Y, playerNode->getPosition().Z));
        }
        if (receiver.isKeyDown(KEY_SPACE)) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + (player.jumpSpeed * frameDeltaTime), playerNode->getPosition().Z));
        }

        camera->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + 20, playerNode->getPosition().Z + 20));
        camera->setTarget(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y - 5, playerNode->getPosition().Z - 5));

        driver->endScene();
    }

    device->drop();
}