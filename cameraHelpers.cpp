#include "include/cameraHelpers.h"

void sKeyInputMoveCamera(Camera2D &camera)
{
    if (IsKeyDown(KEY_W))
    {
        camera.target = Vector2Subtract(camera.target, {0, 8});
    }

    if (IsKeyDown(KEY_S))
    {
        camera.target = Vector2Add(camera.target, {0, 8});
    }

    if (IsKeyDown(KEY_A))
    {
        camera.target = Vector2Subtract(camera.target, {8, 0});
    }

    if (IsKeyDown(KEY_D))
    {
        camera.target = Vector2Add(camera.target, {8, 0});
    }

    if (IsKeyPressed(KEY_EQUAL))
    {
        camera.zoom += 0.2;
    }

    if (IsKeyPressed(KEY_MINUS))
    {
        camera.zoom -= 0.2;
    }
}