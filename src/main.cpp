//
// Created by cpasjuste on 02/10/18.
//
#include "main.h"
#include "io.h"
#include "filer.h"
#include "menu_main.h"
#include "menu_video.h"
#include "scrapper.h"

#ifdef __SWITCH__
static AppletHookCookie applet_hook_cookie;
static void on_applet_hook(AppletHookType hook, void *arg) {
    Main *main = (Main *) arg;
    switch (hook) {
        case AppletHookType_OnExitRequest:
            main->quit();
            break;
        case AppletHookType_OnFocusState:
            if (appletGetFocusState() == AppletFocusState_InFocus) {
                if (main->getPlayer()->getMpv()->isPaused()) {
                    main->getPlayer()->resume();
                }
            } else {
                if (!main->getPlayer()->getMpv()->isPaused()) {
                    main->getPlayer()->pause();
                }
            }
            break;
        default:
            break;
    }
}
#elif __PS4__
#include <orbis/Sysmodule.h>
#include <orbis/libkernel.h>
extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);
#endif

#ifdef __PS4__
static void do_jailbreak(void)
{
    printf("[pplay] =============================================\n");
    printf("[pplay] Trying to load jb.prx (Itemzflow method)...\n");
    printf("[pplay] =============================================\n");

    // Ładujemy PRX z /data/
    int module_id = sceKernelLoadStartModule("/data/jb.prx", 0, NULL, 0, NULL, NULL);

    if (module_id > 0) {
        printf("[pplay] ✅ SUCCESS! jb.prx loaded (ID: %d)\n", module_id);
        printf("[pplay] Full root access should now be active.\n");
    } else {
        printf("[pplay] ❌ Failed to load jb.prx (error: 0x%X)\n", module_id);
        printf("[pplay] Make sure jb.prx is in /data/ on PS4\n");
    }
}
#endif

using namespace c2d;
using namespace c2d::config;
using namespace pplay;

Main::Main(const c2d::Vector2f &size) : C2DRenderer(size) {
#ifndef NDEBUG
    Renderer::setPrintStats(true);
#endif
    pplayIo = new pplay::Io();
    Main::setIo(pplayIo);
    pplayIo->create(pplayIo->getDataPath() + "mpv");

    Main::getInput()->setRepeatDelay(INPUT_DELAY);
    timer = new C2DClock();
    config = new PPLAYConfig(this);
    scaling = {size.x / 1280.0f, size.y / 720.0f};

    font = new Font();
    font->loadFromFile(Main::getIo()->getRomFsPath() + "skin/font.ttf");
    font->setFilter(Texture::Filter::Point);
    font->setOffset({0, -4.0f});

    statusBox = new StatusBox(this, {0, Main::getSize().y - 16});
    statusBox->setOrigin(Origin::BottomLeft);
    statusBox->setLayer(10);
    Main::add(statusBox);

    Main::getIo()->create(Main::getIo()->getDataPath() + "cache");

    FloatRect filerRect = {0, 0, Main::getSize().x, Main::getSize().y};
    filer = new Filer(this, "/", filerRect);
    filer->setLayer(1);
    Main::add(filer);
    filer->getDir("/");

    statusBar = new StatusBar(this);
    statusBar->setLayer(10);
    Main::add(statusBar);

    player = new Player(this);
    player->setLayer(2);
    Main::add(player);

    // ... reszta menu bez zmian (skróciłem dla czytelności)
    std::vector<MenuItem> items;
    items.emplace_back("Home", "home.png", MenuItem::Position::Top);
#ifdef __SWITCH__
    items.emplace_back("Usb", "usb.png", MenuItem::Position::Top);
#endif
    items.emplace_back("Network", "network.png", MenuItem::Position::Top);
    items.emplace_back("Options", "options.png", MenuItem::Position::Top);
    items.emplace_back("Exit", "exit.png", MenuItem::Position::Bottom);

    menu_main = new MenuMain(this, {-250 * scaling.x, 0, 250 * scaling.x, Main::getSize().y}, items);
    menu_main->setVisibility(Visibility::Hidden, false);
    menu_main->setLayer(3);
    Main::add(menu_main);

    items.clear();
    items.emplace_back("Video", "video.png", MenuItem::Position::Top);
    items.emplace_back("Audio", "audio.png", MenuItem::Position::Top);
    items.emplace_back("Subtitles", "subtitles.png", MenuItem::Position::Top);
    items.emplace_back("Stop", "exit.png", MenuItem::Position::Bottom);
    menu_video = new MenuVideo(this, {Main::getSize().x, 0, 250 * scaling.x, Main::getSize().y}, items);
    menu_video->setVisibility(Visibility::Hidden, false);
    menu_video->setLayer(3);
    Main::add(menu_video);

    float w = Main::getSize().x / 3;
    float h = Main::getSize().y / 3;
    messageBox = new MessageBox({Main::getSize().x / 2, Main::getSize().y / 2, w, h},
                                Main::getInput(), Main::getFont(), (int) getFontSize(Main::FontSize::Medium));
    messageBox->setOrigin(Origin::Center);
    messageBox->setFillColor(COLOR_BG);
    messageBox->setAlpha(240);
    messageBox->setOutlineColor(COLOR_RED);
    messageBox->setOutlineThickness(2);
    Main::add(messageBox);

    scrapper = new Scrapper(this);
}

// Reszta funkcji bez zmian (getPlayer, getFiler itd.)
Main::~Main() { delete (scrapper); delete (config); delete (timer); delete (font); }

bool Main::onInput(...) { /* bez zmian */ }
void Main::onUpdate() { C2DRenderer::onUpdate(); }
void Main::show(MenuType type) { /* bez zmian */ }
bool Main::isExiting() { return exit; }
bool Main::isRunning() { return running; }
void Main::setRunningStop() { running = false; }
void Main::quit() { /* bez zmian */ }

Player *Main::getPlayer() { return player; }
Filer *Main::getFiler() { return filer; }
MenuMain *Main::getMenuMain() { return menu_main; }
MenuVideo *Main::getMenuVideo() { return menu_video; }
PPLAYConfig *Main::getConfig() { return config; }
c2d::Font *Main::getFont() { return font; }
c2d::MessageBox *Main::getMessageBox() { return messageBox; }
StatusBox *Main::getStatus() { return statusBox; }
Vector2f Main::getScaling() { return scaling; }
unsigned int Main::getFontSize(FontSize fontSize) {
    return (unsigned int)((float)fontSize * scaling.y);
}
StatusBar *Main::getStatusBar() { return statusBar; }
pplay::Scrapper *Main::getScrapper() { return scrapper; }

int main() {
#ifdef __PS4__
    do_jailbreak();
#endif

    Vector2f size = {C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT};
    // ... reszta main() bez zmian
#ifdef __SWITCH__
#ifdef NDEBUG
    socketInitializeDefault();
#endif
    appletMainLoop();
    if (appletGetOperationMode() == AppletOperationMode_Console) {
        size = {1920, 1080};
    }
#elif __PS4__
    sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_NET);
#endif

    Main *main = new Main(size);

    // ... reszta pętli bez zmian
    while (main->isRunning()) {
        main->flip();
    }

    delete (main);

#ifdef __SWITCH__
    // ... 
#elif __PS4__
    sceSystemServiceLoadExec((char *) "exit", nullptr);
    while (true) {}
#endif
    return 0;
}
