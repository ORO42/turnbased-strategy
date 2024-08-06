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

Position rotatePoint(Position origin, Position point, float angle)
{
    float rad = angle * (M_PI / 180.0f); // Convert to radians
    float s = std::sin(rad);
    float c = std::cos(rad);

    // Translate point back to origin
    point.x -= origin.x;
    point.y -= origin.y;

    // Rotate point
    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    // Translate point back
    point.x = xnew + origin.x;
    point.y = ynew + origin.y;

    return point;
}

void rotateTrapezoid(IsoscelesTrapezoid &trapezoid, float angle)
{
    trapezoid.p1 = rotatePoint(trapezoid.originPos, trapezoid.p1, angle);
    trapezoid.p2 = rotatePoint(trapezoid.originPos, trapezoid.p2, angle);
    trapezoid.p3 = rotatePoint(trapezoid.originPos, trapezoid.p3, angle);
    trapezoid.p4 = rotatePoint(trapezoid.originPos, trapezoid.p4, angle);
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

float angleDifference(float angle1, float angle2)
{
    float diff = std::fmod(angle2 - angle1, 360.0f);
    if (diff < -180.0f)
        diff += 360.0f;
    else if (diff > 180.0f)
        diff -= 360.0f;

    return diff;
}