#include "include/geometryHelpers.h"

// get dot product of two vectors
float dotProduct(const Position &v1, const Position &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

// project a point onto an axis
float projectPointOnAxis(const Position &point, const Position &axis)
{
    return dotProduct(point, axis);
}

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

// Helper function to check if two lines intersect
bool doLinesIntersect(Line &line1, Line &line2)
{
    auto orientation = [](float x1, float y1, float x2, float y2, float x3, float y3)
    {
        float val = (y2 - y1) * (x3 - x2) - (x2 - x1) * (y3 - y2);
        if (val == 0)
            return 0;             // collinear
        return (val > 0) ? 1 : 2; // clock or counterclockwise
    };

    auto onSegment = [](float x1, float y1, float x2, float y2, float x3, float y3)
    {
        if (x2 <= std::max(x1, x3) && x2 >= std::min(x1, x3) && y2 <= std::max(y1, y3) && y2 >= std::min(y1, y3))
            return true;
        return false;
    };

    int o1 = orientation(line1.startX, line1.startY, line1.endX, line1.endY, line2.startX, line2.startY);
    int o2 = orientation(line1.startX, line1.startY, line1.endX, line1.endY, line2.endX, line2.endY);
    int o3 = orientation(line2.startX, line2.startY, line2.endX, line2.endY, line1.startX, line1.startY);
    int o4 = orientation(line2.startX, line2.startY, line2.endX, line2.endY, line1.endX, line1.endY);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(line1.startX, line1.startY, line2.startX, line2.startY, line1.endX, line1.endY))
        return true;
    if (o2 == 0 && onSegment(line1.startX, line1.startY, line2.endX, line2.endY, line1.endX, line1.endY))
        return true;
    if (o3 == 0 && onSegment(line2.startX, line2.startY, line1.startX, line1.startY, line2.endX, line2.endY))
        return true;
    if (o4 == 0 && onSegment(line2.startX, line2.startY, line1.endX, line1.endY, line2.endX, line2.endY))
        return true;

    return false;
}

bool lineRectOverlap(Rectangle rect, Line line)
{
    Line rectSeg1 = {rect.x, rect.y, rect.x + rect.width, rect.y};
    Line rectSeg2 = {rect.x + rect.width, rect.y, rect.x + rect.width, rect.y + rect.height};
    Line rectSeg3 = {rect.x + rect.width, rect.y + rect.height, rect.x, rect.y + rect.height};
    Line rectSeg4 = {rect.x, rect.y + rect.height, rect.x, rect.y};

    return doLinesIntersect(line, rectSeg1) || doLinesIntersect(line, rectSeg2) || doLinesIntersect(line, rectSeg3) || doLinesIntersect(line, rectSeg4);
}

std::vector<Line> getCornerToCornerLines(Rectangle rect1, Rectangle rect2)
{
    std::vector<Line> lines;

    // Define the corners of the first rectangle
    std::vector<std::pair<float, float>> cornersRect1 = {
        {rect1.x, rect1.y},                             // Top-left
        {rect1.x + rect1.width, rect1.y},               // Top-right
        {rect1.x, rect1.y + rect1.height},              // Bottom-left
        {rect1.x + rect1.width, rect1.y + rect1.height} // Bottom-right
    };

    // Define the corners of the second rectangle
    std::vector<std::pair<float, float>> cornersRect2 = {
        {rect2.x, rect2.y},                             // Top-left
        {rect2.x + rect2.width, rect2.y},               // Top-right
        {rect2.x, rect2.y + rect2.height},              // Bottom-left
        {rect2.x + rect2.width, rect2.y + rect2.height} // Bottom-right
    };

    // Create lines between every corner of rect1 and every corner of rect2
    for (const auto &corner1 : cornersRect1)
    {
        for (const auto &corner2 : cornersRect2)
        {
            lines.push_back(Line{
                corner1.first, corner1.second, // Start point (corner of rect1)
                corner2.first, corner2.second  // End point (corner of rect2)
            });
        }
    }

    return lines;
}

PointsRect createRotatedAABBRectangle(const Rectangle &rect, float angle)
{
    Position origin = {rect.x + rect.width / 2, rect.y + rect.height / 2};

    Position topLeft = {rect.x, rect.y};
    Position topRight = {rect.x + rect.width, rect.y};
    Position bottomLeft = {rect.x, rect.y + rect.height};
    Position bottomRight = {rect.x + rect.width, rect.y + rect.height};

    PointsRect rotatedRect;
    rotatedRect.tl = rotatePoint(origin, topLeft, angle);
    rotatedRect.tr = rotatePoint(origin, topRight, angle);
    rotatedRect.br = rotatePoint(origin, bottomRight, angle);
    rotatedRect.bl = rotatePoint(origin, bottomLeft, angle);

    return rotatedRect;
}

// Function to check for collision between an axis-aligned rectangle and a rotated rectangle
bool checkCollisionAABBWithPointsRect(const Rectangle &rect, const PointsRect &rotatedRect)
{
    // Axes to test (normalized)
    Position axes[2] = {
        {rotatedRect.tr.x - rotatedRect.tl.x, rotatedRect.tr.y - rotatedRect.tl.y}, // Edge from p1 to p2
        {rotatedRect.bl.x - rotatedRect.tl.x, rotatedRect.bl.y - rotatedRect.tl.y}  // Edge from p1 to p4
    };

    // Normalize the axes
    for (int i = 0; i < 2; i++)
    {
        float length = std::sqrt(axes[i].x * axes[i].x + axes[i].y * axes[i].y);
        axes[i].x /= length;
        axes[i].y /= length;
    }

    // Test both axes
    for (int i = 0; i < 2; i++)
    {
        Position axis = axes[i];

        // Project the rectangle points onto the axis
        float rectMin = std::min({projectPointOnAxis({rect.x, rect.y}, axis),
                                  projectPointOnAxis({rect.x + rect.width, rect.y}, axis),
                                  projectPointOnAxis({rect.x, rect.y + rect.height}, axis),
                                  projectPointOnAxis({rect.x + rect.width, rect.y + rect.height}, axis)});
        float rectMax = std::max({projectPointOnAxis({rect.x, rect.y}, axis),
                                  projectPointOnAxis({rect.x + rect.width, rect.y}, axis),
                                  projectPointOnAxis({rect.x, rect.y + rect.height}, axis),
                                  projectPointOnAxis({rect.x + rect.width, rect.y + rect.height}, axis)});

        // Project the rotated rectangle points onto the axis
        float rotatedMin = std::min({projectPointOnAxis(rotatedRect.tl, axis),
                                     projectPointOnAxis(rotatedRect.tr, axis),
                                     projectPointOnAxis(rotatedRect.br, axis),
                                     projectPointOnAxis(rotatedRect.bl, axis)});
        float rotatedMax = std::max({projectPointOnAxis(rotatedRect.tl, axis),
                                     projectPointOnAxis(rotatedRect.tr, axis),
                                     projectPointOnAxis(rotatedRect.br, axis),
                                     projectPointOnAxis(rotatedRect.bl, axis)});

        // Check for overlap on this axis
        if (!projectionOverlap(rectMin, rectMax, rotatedMin, rotatedMax))
        {
            // No overlap found, so the rectangles do not collide
            return false;
        }
    }

    // If we find no separating axis, then the rectangles must be colliding
    return true;
}

PointsRect movePointsRect(const PointsRect &rect, const Position &direction)
{
    PointsRect movedRect;

    movedRect.tl.x = rect.tl.x + direction.x;
    movedRect.tl.y = rect.tl.y + direction.y;

    movedRect.tr.x = rect.tr.x + direction.x;
    movedRect.tr.y = rect.tr.y + direction.y;

    movedRect.br.x = rect.br.x + direction.x;
    movedRect.br.y = rect.br.y + direction.y;

    movedRect.bl.x = rect.bl.x + direction.x;
    movedRect.bl.y = rect.bl.y + direction.y;

    return movedRect;
}

void drawPointsRect(const PointsRect &rect, Color color)
{
    // Draw lines between each consecutive corner
    DrawLine(rect.tl.x, rect.tl.y, rect.tr.x, rect.tr.y, color); // Top edge
    DrawLine(rect.tr.x, rect.tr.y, rect.br.x, rect.br.y, color); // Right edge
    DrawLine(rect.br.x, rect.br.y, rect.bl.x, rect.bl.y, color); // Bottom edge
    DrawLine(rect.bl.x, rect.bl.y, rect.tl.x, rect.tl.y, color); // Left edge
}

float distanceFromRectCenterToCorner(float &w, float &h)
{
    float halfWidth = w / 2.0;
    float halfHeight = h / 2.0;

    // pythagorean theorem
    return std::sqrt(halfWidth * halfWidth + halfHeight * halfHeight);
}

// std::vector<Line> createRaysInRectRadius(Position &originPoint, int &degreeStep, int &numberOfTiles, float &tileSize)
// {
// }

// Function to find the intersection of a line with the enclosing rectangle
Position findLineIntersectEnclosingRect(const Rectangle &enclosingRect, const Position &origin, float angle)
{
    // Convert the angle to radians
    float radian = angle * (M_PI / 180.0);

    // Calculate the direction vector
    float dx = std::cos(radian);
    float dy = std::sin(radian);

    // Calculate the t values for intersection with each side of the enclosing rectangle
    float t1 = (enclosingRect.x - origin.x) / dx;
    float t2 = (enclosingRect.x + enclosingRect.width - origin.x) / dx;
    float t3 = (enclosingRect.y - origin.y) / dy;
    float t4 = (enclosingRect.y + enclosingRect.height - origin.y) / dy;

    // We need to find the smallest positive t, which gives us the first intersection point
    float tMin = std::min({t1, t2, t3, t4}, [](float a, float b)
                          { return (a > 0 && b > 0) ? a < b : a > 0; });

    // Calculate the intersection point
    Position intersection;
    intersection.x = origin.x + tMin * dx;
    intersection.y = origin.y + tMin * dy;

    return intersection;
}

// Function to create lines from a rectangle within an enclosing rectangle, separated by a degree step
std::vector<Line> createLinesWithinRect(const Rectangle &originRect, const Rectangle &enclosingRect, int degreeStep)
{
    std::vector<Line> lines;

    // Calculate the center of the originRect
    Position originCenter;
    originCenter.x = originRect.x + originRect.width / 2.0;
    originCenter.y = originRect.y + originRect.height / 2.0;

    // Iterate through the circle in steps of degreeStep
    for (int angle = 0; angle < 360; angle += degreeStep)
    {
        // Find the intersection of the line with the enclosing rectangle
        Position endPoint = findLineIntersectEnclosingRect(enclosingRect, originCenter, angle);

        // Create the line from originCenter to endPoint
        Line line = {originCenter.x, originCenter.y, endPoint.x, endPoint.y};
        lines.push_back(line);
    }

    return lines;
}