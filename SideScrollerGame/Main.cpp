#include<irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;

#if defined(_MSC_VER)
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "GameContext.h"

int main() {
    IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<u32>(1024, 768), 16, false, true, false, 0);
    if (!device)
        return 1;

    IVideoDriver* driver = device->getVideoDriver();

    GameContext context;
    context.setDevice(device);
    context.setDriver(driver);
    context.loadLevel();

    while (device->run()) {
        context.update();

        driver->beginScene(true, true, SColor(255, 128, 128, 128));

        context.smgr->drawAll();

        context.drawHUD();

        context.guienv->drawAll();

        driver->endScene();
    }

    device->drop();

    return 0;
}