#pragma once
#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <string>
#include <vector>

#include<irrlicht.h>

#include "MyEventReceiver.h"
#include "Player.h"
#include "Pickup.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class GameContext {
private:
	u32 then = 0;

	IVideoDriver* driver;
	IrrlichtDevice* device;

    IGUIEnvironment* guienv;

    ILightSceneNode* light;

    Player* player;
    IAnimatedMeshSceneNode* playerNode;
    ISceneNodeAnimatorCollisionResponse* playerCollision;

    ITriangleSelector* world;

    int score = 0;

    std::vector<IMeshSceneNode*> pickupNode;

    IGUIFont* font;

    const std::string ASSETS_PATH = "assets/";
    const std::string MAP_PATH = ASSETS_PATH + "map/";
    const std::string PLAYER_PATH = ASSETS_PATH + "player/";
    const std::string PICKUP_PATH = ASSETS_PATH + "pickup/";

    void addCamera() {
        // Camera
        camera = smgr->addCameraSceneNode(0, vector3df(0, 20, 20), vector3df(0, 0, 0));
        camera->setNearValue(1.0f);
        camera->setFarValue(1000.0f);
    }

    void addWorld() {
        // Light
        light = smgr->addLightSceneNode(0, vector3df(0, 20, 0));

        // Map
        std::string mapObjFilePath = MAP_PATH;
        mapObjFilePath += "map.obj";

        IMesh* mapMesh = smgr->getMesh(mapObjFilePath.c_str());
        IMeshSceneNode* mapNode = smgr->addMeshSceneNode(mapMesh);
        if (mapNode) {
            mapNode->setMaterialFlag(EMF_LIGHTING, false);
        }

        // Add Collision
        world = smgr->createOctreeTriangleSelector(mapNode->getMesh(), mapNode);
    }

    void addPlayer() {
        // Player
        player = new Player(vector3df(0, 10, 0.5f));

        // Player Mesh
        std::string playerMd2FilePath = PLAYER_PATH;
        playerMd2FilePath += "player.md2";
        std::string playerTextureFilePath = PLAYER_PATH;
        playerTextureFilePath += "player.bmp";

        IAnimatedMesh* playerMesh = smgr->getMesh(playerMd2FilePath.c_str());
        playerNode = smgr->addAnimatedMeshSceneNode(playerMesh);
        if (playerNode) {
            playerNode->setMaterialFlag(EMF_LIGHTING, false);
            playerNode->setMaterialTexture(0, driver->getTexture(playerTextureFilePath.c_str()));
            playerNode->setPosition(player->position);
            // Player Shadow
            playerNode->addShadowVolumeSceneNode();
            // Player IDLE Animation
            playerNode->setFrameLoop(0, 0);
            playerNode->setAnimationSpeed(300);
        }

        playerCollision = smgr->createCollisionResponseAnimator(world, playerNode, vector3df(0.5f, 2, 0.5f), vector3df(0, -0.1f, 0));
        playerNode->addAnimator(playerCollision);
        playerCollision->drop();
    }

    void addPickups() {
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
        std::string pickupObjFilePath = PICKUP_PATH;
        pickupObjFilePath += "pickup.obj";

        IMesh* pickupMesh = smgr->getMesh(pickupObjFilePath.c_str());

        for (int i = 0; i < pickup.size(); i++) {
            pickupNode.push_back(smgr->addMeshSceneNode(pickupMesh));
        }
        for (int i = 0; i < pickupNode.size(); i++) {
            if (pickupNode[i]) {
                pickupNode[i]->setMaterialFlag(EMF_LIGHTING, false);
                pickupNode[i]->setPosition(pickup[i].position);
            }
        }
    }

public:
	ISceneManager* smgr;

	MyEventReceiver* eventReceiver;

	ICameraSceneNode* camera;

	void loadLevel() {
		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

        // Font
        font = device->getGUIEnvironment()->getBuiltInFont();

        addCamera();
        addWorld();
        addPlayer();
        addPickups();
	}

	void setDevice(IrrlichtDevice* device) {
		this->device = device;
		then = device->getTimer()->getTime();

		eventReceiver = new MyEventReceiver();
		device->setEventReceiver(eventReceiver);
        smgr = device->getSceneManager();

        guienv = device->getGUIEnvironment();

		device->setWindowCaption(L"Side Scroller Game");
	}

	void setDriver(IVideoDriver* driver) {
		this->driver = driver;
	}

	void update() {
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
		then = now;

        updateCamera();
        updatePlayerControls(frameDeltaTime);
        updatePlayer();
	}

    void updateCamera() {
        camera->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + 10, playerNode->getPosition().Z + 10));
        camera->setTarget(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y - 5, playerNode->getPosition().Z - 5));
    }

    void updatePlayer() {
        // Light follows player
        light->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + 20, playerNode->getPosition().Z));

        // Player & pickups
        for (int i = 0; i < pickupNode.size(); i++) {
            if (pickupNode[i]->isVisible()) {
                if (playerNode->getPosition().X + 1 >= pickupNode[i]->getPosition().X && playerNode->getPosition().X <= pickupNode[i]->getPosition().X + 1
                    && playerNode->getPosition().Y + 1 >= pickupNode[i]->getPosition().Y && playerNode->getPosition().Y <= pickupNode[i]->getPosition().Y + 1
                    && playerNode->getPosition().Z + 1 >= pickupNode[i]->getPosition().Z && playerNode->getPosition().Z <= pickupNode[i]->getPosition().Z + 1) {
                    pickupNode[i]->setVisible(false);
                    score += 10;
                }
            }
        }
    }

    void updatePlayerControls(f32 frameDeltaTime) {
        const bool leftKey = eventReceiver->isKeyDown(KEY_KEY_A);
        const bool rightKey = eventReceiver->isKeyDown(KEY_KEY_D);
        const bool spaceKey = eventReceiver->isKeyDown(KEY_SPACE);

        if (!leftKey && rightKey) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X - (player->speed * frameDeltaTime), playerNode->getPosition().Y, playerNode->getPosition().Z));
            if (!spaceKey) {
                if (playerCollision->collisionOccurred())
                    if (player->state != player->WALK) {
                        player->state = player->WALK;
                        playerNode->setFrameLoop(0, 159);
                        playerNode->setAnimationSpeed(300);
                    }
            }
            else {
                if (player->state != player->JUMP) {
                    player->state = player->JUMP;
                    playerNode->setFrameLoop(166, 330);
                    playerNode->setAnimationSpeed(150);
                }
            }
            playerNode->setRotation(vector3df(0, 0, 0));
        }
        else if (leftKey && !rightKey) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X + (player->speed * frameDeltaTime), playerNode->getPosition().Y, playerNode->getPosition().Z));
            if (!spaceKey) {
                if (playerCollision->collisionOccurred())
                    if (player->state != player->WALK) {
                        player->state = player->WALK;
                        playerNode->setFrameLoop(0, 159);
                        playerNode->setAnimationSpeed(300);
                    }
            }
            else {
                if (player->state != player->JUMP) {
                    player->state = player->JUMP;
                    playerNode->setFrameLoop(166, 330);
                    playerNode->setAnimationSpeed(150);
                }
            }
            playerNode->setRotation(vector3df(0, 180, 0));
        }
        else {
            if (!spaceKey) {
                if (playerCollision->collisionOccurred())
                    if (player->state != player->IDLE) {
                        player->state = player->IDLE;
                        playerNode->setFrameLoop(0, 0);
                        playerNode->setAnimationSpeed(300);
                    }
            }
            else {
                if (player->state != player->JUMP) {
                    player->state = player->JUMP;
                    playerNode->setFrameLoop(166, 330);
                    playerNode->setAnimationSpeed(150);
                }
            }
        }

        if (spaceKey) {
            playerNode->setPosition(vector3df(playerNode->getPosition().X, playerNode->getPosition().Y + (player->jumpSpeed * frameDeltaTime), playerNode->getPosition().Z));
        }

        if (eventReceiver->isKeyDown(KEY_KEY_D) || eventReceiver->isKeyDown(KEY_KEY_A)) {
            if (playerCollision->collisionOccurred())
                playerCollision->jump(1.f * frameDeltaTime);
        }

        if (eventReceiver->isKeyDown(KEY_F5)) {
            reset();
        }
    }

    void drawHUD() {
        stringw scoreString = L"Score: "; scoreString += score;
        font->draw(scoreString, rect<s32>(10, 50, 300, 100), SColor(255, 255, 255, 0));
    }

    void reset() {
        playerNode->setPosition(player->position);
        light->setPosition(vector3df(0, 20, 0));
    }
	
};

#endif // !GAMECONTEXT_H
