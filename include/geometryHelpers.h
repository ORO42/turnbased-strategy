#pragma once

#include <iostream>
#include <cmath>
#include "components.h"

float getAngleBetweenPoints(Position p1, Position p2);
Position getRectCenter(Rectangle rect);
void rotateTrapezoid(IsoscelesTrapezoid &trapezoid, float angle);
float angleDifference(float angle1, float angle2);
bool checkTrapRectOverlap(const IsoscelesTrapezoid &trap, const Rectangle &rect);