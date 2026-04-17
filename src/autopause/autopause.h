#pragma once

namespace AutoPause {
    class Handler {
    public:
        virtual void setUpdateCallback(
            void(&callback)(float const &)) = 0;
        virtual void startGame() = 0;
        virtual void show2PGuide() = 0;
    };

    void init(Handler &mHandler, bool const &isClassic2P);
}