#include "Input.hpp"
#include "Core/Application.hpp"
#include "Core/Window.hpp"
#include <GLFW/glfw3.h>

namespace Hollow {

static bool s_CurrentKeys[1024] = {};
static bool s_PreviousKeys[1024] = {};

static bool s_CurrentMouse[8] = {};
static bool s_PreviousMouse[8] = {};

static float s_MouseX = 0.0f;
static float s_MouseY = 0.0f;
static float s_LastMouseX = 0.0f;
static float s_LastMouseY = 0.0f;

static GLFWwindow* GetWindow()
{
    auto& window = Application::Get().GetWindow();
    return (GLFWwindow*)window.GetNativeWindow();
}

void Input::Update()
{
    auto window = GetWindow();

    for (int i = 0; i < 1024; i++) {
        s_PreviousKeys[i] = s_CurrentKeys[i];
        s_CurrentKeys[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }

    for (int i = 0; i < 8; i++) {
        s_PreviousMouse[i] = s_CurrentMouse[i];
        s_CurrentMouse[i] = glfwGetMouseButton(window, i) == GLFW_PRESS;
    }

    s_LastMouseX = s_MouseX;
    s_LastMouseY = s_MouseY;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    s_MouseX = (float)x;
    s_MouseY = (float)y;
}

bool Input::IsKeyPressed(int key)
{
    return s_CurrentKeys[key];
}

bool Input::IsKeyReleased(int key)
{
    return !s_CurrentKeys[key];
}

bool Input::IsKeyDown(int key)
{
    return s_CurrentKeys[key] && !s_PreviousKeys[key];
}

bool Input::IsMousePressed(int button)
{
    return s_CurrentMouse[button];
}

bool Input::IsMouseReleased(int button)
{
    return !s_CurrentMouse[button];
}

bool Input::IsMouseDown(int button)
{
    return s_CurrentMouse[button] && !s_PreviousMouse[button];
}

float Input::GetMouseX()
{
    return s_MouseX;
}

float Input::GetMouseY()
{
    return s_MouseY;
}

float Input::GetMouseDeltaX()
{
    return s_MouseX - s_LastMouseX;
}

float Input::GetMouseDeltaY()
{
    return s_MouseY - s_LastMouseY;
}

}
