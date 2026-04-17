#include "autopause.h"

namespace AutoPause {
    Handler* pHandler;
    char state;
    float timer;

    void update(float const &dt) {
        timer -= dt;
        if (timer > 0.0f) {
            return;
        }
        if (state == '\x00') {
            pHandler->show2PGuide();
            state = '\x01';
            timer = 1.5f;
            return;
        }
        if (state == '\x01') {
            pHandler->startGame();
        }
    }

    void init(Handler &mHandler, bool const &isClassic2P) {
        (pHandler = &mHandler)->setUpdateCallback(update);
        if (isClassic2P) {
            state = '\x00';
            timer = 0.5f;
            return;
        }
        state = '\x01';
        timer = 1.0f;
    }
}
