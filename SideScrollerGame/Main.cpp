#include <irrlicht.h>

#include "MyEventReceiver.h"
#include "Player.h"
#include <vector>
#include "Pickup.h"

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
        createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 16,
            false, true, false, &receiver);

    if (!device)
        return 1;

    device->setWindowCaption(L"Side Scroller Game");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        rect<s32>(10, 10, 260, 22), true);

    // Font
    IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();

    // Camera
    ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0, 20, 20), vector3df(0, 0, 0));
    camera->setNearValue(1.0f);
    camera->setFarValue(1000.0f);

    // Player
    Player player = Player(vector3df(0, 10, 0.5f));

    // Player Mesh
    IAnimatedMesh* playerMesh = smgr->getMesh("assets/player/player.md2");
    if (!playerMesh) {
        device->drop();
        return 1;
    }
    IAnimatedMeshSceneNode* playerNode = smgr->addAnimatedMeshSceneNode(playerMesh);
    if (playerNode) {
        playerNode->setMaterialFlag(EMF_LIGHTING, false);
        playerNode->setMaterialTexture(0, driver->getTexture("assets/player/player.bmp"));
        playerNode->setPosition(player.position);
        // Player Shadow
        playerNode->addShadowVolumeSceneNode();
        // Player IDLE Animation
        playerNode->setFrameLoop(0, 0);
    }
    // Player Collision
    ISceneNodeAnimatorCollisionResponse* playerCollision;

    // Light
    ILightSceneNode* light = smgr->addLightSceneNode(0, vector3df(0, 10, 0));

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
    playerCollision = smgr->createCollisionResponseAnimator(world, playerNode, vector3df(0.5f, 2, 0.5f), vector3df(0, -0.1f, 0));
    playerNode->addAnimator(playerCollision);
    playerCollision->drop();

    // Pickup
    std::vector<Pickup> pickup;
    // Add Pickups to the map
    pickup.push_back(Pickup(vector3df(-5, 10, 0.5f)));
    pickup.push_back(Pickup(vector3df(5, 10, 0.5f)));
    pickup.push_back(Pickup(vector3df(-22, 13, 0.5f)));
    pickup.push_back(Pickup(vector3df(-26, 13, 0.5f)));
    pickup.push_back(Pickup(vector3df(-34, 13, 0.5f)));
    pickup.push_back(Pickup(vector3df(-38, 13, 0.5f)));
    pickup.push_back(Pickup(vector3df(-219, 18, 0.5f)));
    pickup.push_back(Pickup(vector3df(-223, 18, 0.5f)));
    pickup.push_back(Pickup(vector3df(-221, 28, 0.5f)));
    pickup.push_back(Pickup(vector3df(-221, 33, 0.5f)));
    pickup.push_back(Pickup(vector3df(-22, 13, 0.5f)));
    pickup.push_back(Pickup(vector3df(-254, -3, 0.5f)));
    pickup.push_back(Pickup(vector3df(-240, 17, 0.5f)));
    pickup.push_back(Pickup(vector3df(-303, 14, 0.5f)));
    pickup.push_back(Pickup(vector3df(-307, 14, 0.5f)));
    pickup.push_back(Pickup(vector3df(-307, 18, 0.5f)));
    pickup.push_back(Pickup(vector3df(-303, 18, 0.5f)));
    pickup.push_back(Pickup(vector3df(-303, 77, 0.5f)));
    pickup.push_back(Pickup(vector3df(-306, 77, 0.5f)));
    pickup.push_back(Pickup(vector3df(-314, 77, 0.5f)));
    pickup.push_back(Pickup(vector3df(-427, 6, 0.5f)));
    pickup.push_back(Pickup(vector3df(-437, 6, 0.5f)));
    // Pickup Mesh
    IMesh* pickupMesh = smgr->getMesh("assets/pickup/pickup.obj");
    if (!pickupMesh) {
        device->drop();
        return 1;
    }
    std::vector<IMeshSceneNode*> pickupNode;
    for (int i = 0; i < pickup.size(); i++) {
        pickupNode.push_back(smgr->addMeshSceneNode(pickupMesh));
    }
    for (int i = 0; i < pickupNode.size(); i++) {
        if (pickupNode[i]) {
            pickupNode[i]->setMaterialFlag(EMF_LIGHTING, false);
            pickupNode[i]->setPosition(pickup[i].position);
        }
    }

    // Time
    u32 then = device->getTimer()->getTime();

    while (device->run()) {
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.0f;
        then = now;

        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        smgr->drawAll();
        guienv->drawAll();

        // Draw Score
        stringw scoreString = L"Score: "; scoreString += player.score;
        font->draw(scoreString, rect<s32>(10, 50, 300, 100), SColor(255, 255, 255, 0));

        driver->endScene();

        // Light follow player
        light->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + 20, playerNode->getPosition().Z));

        if (receiver.isKeyDown(KEY_KEY_D)) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X - (player.speed * frameDeltaTime), playerNode->getPosition().Y, playerNode->getPosition().Z));
            if (player.state != player.WALK) {
                player.state = player.WALK;
                playerNode->setFrameLoop(0, 80);
            }
        }
        if (receiver.isKeyDown(KEY_KEY_A)) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X + (player.speed * frameDeltaTime), playerNode->getPosition().Y, playerNode->getPosition().Z));
            if (player.state != player.WALK) {
                player.state = player.WALK;
                playerNode->setFrameLoop(0, 80);
            }
        }
        if (!receiver.isKeyDown(KEY_KEY_D) && !receiver.isKeyDown(KEY_KEY_A)) {
            if (player.state != player.IDLE) {
                player.state = player.IDLE;
                playerNode->setFrameLoop(0, 0);
            }
        }
        if (receiver.isKeyDown(KEY_SPACE)) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + (player.jumpSpeed * frameDeltaTime), playerNode->getPosition().Z));
        }
        if (playerCollision->collisionOccurred())
            playerCollision->jump(0.1f * frameDeltaTime);

        camera->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + 10, playerNode->getPosition().Z + 10));
        camera->setTarget(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y - 5, playerNode->getPosition().Z - 5));

        // Player & pickups
        for (int i = 0; i < pickupNode.size(); i++) {
            if (pickupNode[i]->isVisible()) {
                if (playerNode->getPosition().X + 1 >= pickupNode[i]->getPosition().X && playerNode->getPosition().X <= pickupNode[i]->getPosition().X + 1
                    && playerNode->getPosition().Y + 1 >= pickupNode[i]->getPosition().Y && playerNode->getPosition().Y <= pickupNode[i]->getPosition().Y + 1
                    && playerNode->getPosition().Z + 1 >= pickupNode[i]->getPosition().Z && playerNode->getPosition().Z <= pickupNode[i]->getPosition().Z + 1) {
                    pickupNode[i]->setVisible(false);
                    player.score += 10;
                }
            }
        }
    }

    device->drop();
}