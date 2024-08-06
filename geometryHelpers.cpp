#include "include/geometryHelpers.h"

float getAngleBetweenPoints(Position p1, Position p2)
{
    float deltaY = p2.y - p1.y;
    float deltaX = p2.x - p1.x;
    float angleInRadians = atan2(deltaY, deltaX);
    float angleInDegrees = angleInRadians * (180.0f / M_PI); // Convert radians to degrees
    return angleInDegrees;
}

Position getRectCenter(Rectangle rect)
{
    Position center;
    center.x = rect.x + rect.width / 2.0f;
    center.y = rect.y + rect.height / 2.0f;
    return center;
}

// Function to rotate a point around a center by a certain angle
Position rotatePoint(const Position &point, const Position &center, float angleDegrees)
{
    // Convert angle from degrees to radians
    float angleRadians = angleDegrees * (M_PI / 180.0);

    // Calculate the difference between the point and the center
    float dx = point.x - center.x;
    float dy = point.y - center.y;

    // Calculate the new coordinates after rotation
    float xNew = center.x + dx * cos(angleRadians) - dy * sin(angleRadians);
    float yNew = center.y + dx * sin(angleRadians) + dy * cos(angleRadians);

    return {xNew, yNew};
}

// Function to rotate the trapezoid
void rotateTrapezoid(IsoscelesTrapezoid &trapezoid, float angleDegrees)
{
    // Rotate each vertex around the origin position
    trapezoid.p1 = rotatePoint(trapezoid.p1, trapezoid.originPos, angleDegrees);
    trapezoid.p2 = rotatePoint(trapezoid.p2, trapezoid.originPos, angleDegrees);
    trapezoid.p3 = rotatePoint(trapezoid.p3, trapezoid.originPos, angleDegrees);
    trapezoid.p4 = rotatePoint(trapezoid.p4, trapezoid.originPos, angleDegrees);
}

// Function to reposition the trapezoid
void repositionTrapezoid(IsoscelesTrapezoid &trapezoid, const Position &newOriginPos)
{
    // Calculate the difference between the old and new origin positions
    float dx = newOriginPos.x - trapezoid.originPos.x;
    float dy = newOriginPos.y - trapezoid.originPos.y;

    // Update the origin position
    trapezoid.originPos = newOriginPos;

    // Apply the difference to all the points of the trapezoid
    trapezoid.p1.x += dx;
    trapezoid.p1.y += dy;
    trapezoid.p2.x += dx;
    trapezoid.p2.y += dy;
    trapezoid.p3.x += dx;
    trapezoid.p3.y += dy;
    trapezoid.p4.x += dx;
    trapezoid.p4.y += dy;
}

Vector2 getNormal(Vector2 v)
{
    return (Vector2){-v.y, v.x};
}

bool isPointBetweenLongSides(Vector2 point, const IsoscelesTrapezoid &trapezoid)
{
    Vector2 A = {trapezoid.p1.x, trapezoid.p1.y};
    Vector2 B = {trapezoid.p2.x, trapezoid.p2.y};
    Vector2 C = {trapezoid.p3.x, trapezoid.p3.y};
    Vector2 D = {trapezoid.p4.x, trapezoid.p4.y};

    Vector2 AB = {B.x - A.x, B.y - A.y};
    Vector2 CD = {D.x - C.x, D.y - C.y};

    Vector2 normalAB = getNormal(AB);
    Vector2 normalCD = getNormal(CD);

    Vector2 AP = {point.x - A.x, point.y - A.y};
    Vector2 CP = {point.x - C.x, point.y - C.y};

    float dotAB = normalAB.x * AP.x + normalAB.y * AP.y;
    float dotCD = normalCD.x * CP.x + normalCD.y * CP.y;

    return (dotAB > 0 && dotCD < 0) || (dotAB < 0 && dotCD > 0);
}