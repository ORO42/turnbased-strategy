#pragma once

#include <iostream>
#include <cmath>
#include "components.h"

float getAngleBetweenPoints(Position p1, Position p2);
Position getRectCenter(Rectangle rect);
Position rotatePoint(const Position &point, const Position &center, float angleDegrees);
void rotateTrapezoid(IsoscelesTrapezoid &trapezoid, float angleDegrees);
void repositionTrapezoid(IsoscelesTrapezoid &trapezoid, const Position &newOriginPos);
Vector2 getNormal(Vector2 v);
bool isPointBetweenLongSides(Vector2 point, const IsoscelesTrapezoid &trapezoid);