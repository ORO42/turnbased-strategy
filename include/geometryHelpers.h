#pragma once

#include <iostream>
#include <cmath>
#include "components.h"

float getAngleBetweenPoints(Position p1, Position p2);
float getRandomAngleWithinRange(float originalAngle, float range);
Position getRectCenter(Rectangle rect);
Position rotatePoint(Position origin, Position point, float angle);
void rotateTrapezoid(IsoscelesTrapezoid &trapezoid, float angle);
float angleDifference(float angle1, float angle2);
bool checkTrapRectOverlap(const IsoscelesTrapezoid &trap, const Rectangle &rect);
bool lineRectOverlap(Rectangle rect, Line line);
std::vector<Line> getCornerToCornerLines(Rectangle rect1, Rectangle rect2);
PointsRect createRotatedAABBRectangle(const Rectangle &rect, float angle);
bool checkCollisionAABBWithPointsRect(const Rectangle &rect, const PointsRect &rotatedRect);
PointsRect movePointsRect(const PointsRect &rect, const Position &direction);
void drawPointsRect(const PointsRect &rect, Color color);
float distanceFromRectCenterToCorner(float &w, float &h);
Position findLineIntersectEnclosingRect(const Rectangle &enclosingRect, const Position &origin, float angle);
std::vector<Line> createLinesWithinRect(const Rectangle &originRect, const Rectangle &enclosingRect, int degreeStep);