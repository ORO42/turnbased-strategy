#include "include/cameraHelpers.h"

void sKeyInputMoveCamera(Camera2D &camera)
{
    if (IsKeyDown(KEY_W))
    {
        camera.target = Vector2Subtract(camera.target, {0, 24});
    }

    if (IsKeyDown(KEY_S))
    {
        camera.target = Vector2Add(camera.target, {0, 24});
    }

    if (IsKeyDown(KEY_A))
    {
        camera.target = Vector2Subtract(camera.target, {24, 0});
    }

    if (IsKeyDown(KEY_D))
    {
        camera.target = Vector2Add(camera.target, {24, 0});
    }

    const float zoomIncrement = 0.2f;
    const float minZoom = 0.1f;

    if (IsKeyPressed(KEY_EQUAL))
    {
        camera.zoom += zoomIncrement;
    }

    if (IsKeyPressed(KEY_MINUS))
    {
        camera.zoom = fmaxf(camera.zoom - zoomIncrement, minZoom);
    }
}