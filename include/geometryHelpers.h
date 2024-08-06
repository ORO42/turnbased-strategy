#pragma once

#include <iostream>
#include <cmath>
#include "components.h"

float getAngleBetweenPoints(Position p1, Position p2);
Position getRectCenter(Rectangle rect);
Position rotatePoint(Position origin, Position point, float angleRadians);
Position rotatePoint(Position origin, Position point, float angle);
void rotateTrapezoid(IsoscelesTrapezoid &trapezoid, float angle);
float angleDifference(float angle1, float angle2);