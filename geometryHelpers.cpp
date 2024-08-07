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

// Utility function to find the minimum and maximum projections on an axis
void axisProjections(const Position &p1, const Position &p2, const Position &axis, float &minProj, float &maxProj)
{
    float proj1 = p1.x * axis.x + p1.y * axis.y;
    float proj2 = p2.x * axis.x + p2.y * axis.y;
    minProj = std::min(proj1, proj2);
    maxProj = std::max(proj1, proj2);
}

// Function to project the rectangle vertices on an axis
void projectRectangle(const Rectangle &rect, const Position &axis, float &minProj, float &maxProj)
{
    Position vertices[4] = {
        {rect.x, rect.y},
        {rect.x + rect.width, rect.y},
        {rect.x, rect.y + rect.height},
        {rect.x + rect.width, rect.y + rect.height}};

    axisProjections(vertices[0], vertices[1], axis, minProj, maxProj);
    for (int i = 1; i < 4; ++i)
    {
        float minProjTemp, maxProjTemp;
        axisProjections(vertices[i], vertices[(i + 1) % 4], axis, minProjTemp, maxProjTemp);
        minProj = std::min(minProj, minProjTemp);
        maxProj = std::max(maxProj, maxProjTemp);
    }
}

// Function to project the trapezoid vertices on an axis
void projectTrapezoid(const IsoscelesTrapezoid &trap, const Position &axis, float &minProj, float &maxProj)
{
    Position vertices[4] = {trap.p1, trap.p2, trap.p3, trap.p4};

    axisProjections(vertices[0], vertices[1], axis, minProj, maxProj);
    for (int i = 1; i < 4; ++i)
    {
        float minProjTemp, maxProjTemp;
        axisProjections(vertices[i], vertices[(i + 1) % 4], axis, minProjTemp, maxProjTemp);
        minProj = std::min(minProj, minProjTemp);
        maxProj = std::max(maxProj, maxProjTemp);
    }
}

// Function to check if two projections overlap
bool projectionOverlap(float minA, float maxA, float minB, float maxB)
{
    return !(maxA < minB || maxB < minA);
}

// Function to check if trapezoid and rectangle overlap
bool checkTrapRectOverlap(const IsoscelesTrapezoid &trap, const Rectangle &rect)
{
    // Get the edges of the trapezoid
    Position edgesTrapezoid[4] = {trap.p1, trap.p2, trap.p3, trap.p4};
    Position edgesRectangle[4] = {
        {rect.x, rect.y},
        {rect.x + rect.width, rect.y},
        {rect.x, rect.y + rect.height},
        {rect.x + rect.width, rect.y + rect.height}};

    // Check all edges of the trapezoid
    for (int i = 0; i < 4; ++i)
    {
        Position edge1 = {edgesTrapezoid[(i + 1) % 4].x - edgesTrapezoid[i].x, edgesTrapezoid[(i + 1) % 4].y - edgesTrapezoid[i].y};
        Position axis = {-edge1.y, edge1.x};

        float minProjTrap, maxProjTrap;
        float minProjRect, maxProjRect;

        projectTrapezoid(trap, axis, minProjTrap, maxProjTrap);
        projectRectangle(rect, axis, minProjRect, maxProjRect);

        if (!projectionOverlap(minProjTrap, maxProjTrap, minProjRect, maxProjRect))
        {
            return false; // No overlap on this axis
        }
    }

    // Check all edges of the rectangle
    for (int i = 0; i < 4; ++i)
    {
        Position edge1 = {edgesRectangle[(i + 1) % 4].x - edgesRectangle[i].x, edgesRectangle[(i + 1) % 4].y - edgesRectangle[i].y};
        Position axis = {-edge1.y, edge1.x};

        float minProjTrap, maxProjTrap;
        float minProjRect, maxProjRect;

        projectTrapezoid(trap, axis, minProjTrap, maxProjTrap);
        projectRectangle(rect, axis, minProjRect, maxProjRect);

        if (!projectionOverlap(minProjTrap, maxProjTrap, minProjRect, maxProjRect))
        {
            return false; // No overlap on this axis
        }
    }

    return true; // Overlap on all axes
}