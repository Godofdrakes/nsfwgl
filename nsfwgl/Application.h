#pragma once
#include "nsfw.h"

namespace nsfw {

    class Application {
    public:
        virtual void onInit() {
#pragma message ( __ERR__ "OVERRIDE: Load everything here!" )
        };

        virtual void onTerm() {
#pragma message ( __ERR__ "OVERRIDE: Free your data!")
        };

        virtual void onPlay() {
#pragma message ( __ERR__ "OVERRIDE: Setup/reset the game state (or do nothing)!")
        };

        virtual void onStep() {
#pragma message ( __ERR__ "OVERRIDE: Render Passes should draw, objects should update!")
        };

        void init( int width = 800, int height = 600, const char* name = "Not Suited For Work" ) {
            Window::instance().init( width, height );
            Assets::instance().init();
            onInit();
        }

        void term() {
            onTerm();
            Assets::instance().term();
            Window::instance().term();
        }

        void play() {
            onPlay();

            do {
                step();
            }
            while ( !Window::instance().getShouldClose() );
        }

        void step() {
            Window::instance().step();
            onStep();
        }
    };
}
