#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "autopause/autopause.h"

using namespace geode::prelude;

class AutoPauseHandler : public AutoPause::Handler {
    void(*updateCallback)(float const &dt);
    PlayLayer* pPlayLayer;
    bool hasStarted = true;
    bool hasFirstPause;
    bool usesOriginal = false;
public:
    void reset(PlayLayer &mPlayLayer) {
        pPlayLayer = &mPlayLayer;
        hasStarted = false;
        hasFirstPause = false;
    }
    void usedEditorLayer() {
        hasStarted = true;
    }
    bool gameHasStarted() {
        return hasStarted;
    }
    bool hadFirstPause() {
        return hasFirstPause;
    }
    bool usesOriginalMethod() {
        return usesOriginal;
    }
    void firstPause() {
        pPlayLayer->pauseGame(false);
        if (pPlayLayer->m_isPaused) {
            hasFirstPause = true;
        }
    }
    void update(float const &dt) {
        updateCallback(dt);
    }

    void setUpdateCallback(void(&callback)(float const &)) override {
        updateCallback = &callback;
    }
    void startGame() override {
        usesOriginal = true;
        pPlayLayer->startGame();
        hasStarted = true;
        usesOriginal = false;
    }
    void show2PGuide() override {
        usesOriginal = true;
        pPlayLayer->showTwoPlayerGuide();
        usesOriginal = false;
    }
} handler;

class $modify(GJBaseGameLayer) {
    virtual void update(float dt) {
        if (handler.gameHasStarted()) {
            GJBaseGameLayer::update(dt);
            return;
        }
        if (GameManager::get()->getEditorLayer()) {
            handler.usedEditorLayer();
            GJBaseGameLayer::update(dt);
            return;
        }
        if (!handler.hadFirstPause()) {
            handler.firstPause();
            return;
        }
        handler.update(dt);
        GJBaseGameLayer::update(dt);
    }
};

class $modify(AutoPausePlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        handler.reset(*this);
        AutoPause::init(handler,
            !level->isPlatformer() && level->m_twoPlayerMode);
        return true;
    }
    void startGame() {
        if (!handler.usesOriginalMethod()) {
            return;
        }
        PlayLayer::startGame();
    }
    void showTwoPlayerGuide() {
        if (!handler.usesOriginalMethod()) {
            return;
        }
        PlayLayer::showTwoPlayerGuide();
    }
};
