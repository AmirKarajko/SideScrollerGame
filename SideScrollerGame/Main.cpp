#include <irrlicht.h>

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
    IrrlichtDevice* device =
        createDevice(video::EDT_SOFTWARE, dimension2d<u32>(800, 600), 16,
            false, false, false, 0);

    if (!device)
        return 1;

    device->setWindowCaption(L"Side Scroller Game");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        rect<s32>(10, 10, 260, 22), true);

    smgr->addCameraSceneNode(0, vector3df(0, 20, 20), vector3df(0, 5, 0));

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

    while (device->run()) {
        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }

    device->drop();
}