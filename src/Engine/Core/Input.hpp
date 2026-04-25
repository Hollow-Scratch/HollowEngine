#pragma once

namespace Raycaster {

class Input {
public:
    static void Update();

    static bool IsKeyPressed(int key);
    static bool IsKeyReleased(int key);
    static bool IsKeyDown(int key);

    static bool IsMousePressed(int button);
    static bool IsMouseReleased(int button);
    static bool IsMouseDown(int button);

    static float GetMouseX();
    static float GetMouseY();

    static float GetMouseDeltaX();
    static float GetMouseDeltaY();
};

}