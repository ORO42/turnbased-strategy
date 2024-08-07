#include "include/uiHelpers.h"

bool isRectangleInViewport(const Rectangle &rect, const Camera2D &camera)
{
    Rectangle viewport = {camera.target.x - (GetScreenWidth() / 2) / camera.zoom,
                          camera.target.y - (GetScreenHeight() / 2) / camera.zoom,
                          GetScreenWidth() / camera.zoom,
                          GetScreenHeight() / camera.zoom};

    // Check if the rectangle is completely outside the viewport
    if (rect.x + rect.width < viewport.x || rect.x > viewport.x + viewport.width ||
        rect.y + rect.height < viewport.y || rect.y > viewport.y + viewport.height)
    {
        return false;
    }

    return true;
}

void sDrawUI(Player &player, Unit *&selectedUnit, std::vector<Unit> &allUnits, Ability *&selectedAbility, Tile *&hoveredTile)
{
    // draw player stats
    DrawText(TextFormat("AP: %.2f", player.ap), 10, 10, 16, WHITE);
    DrawText(TextFormat("XP: %.2f", player.xp), 10, 30, 16, WHITE);

    // draw unit stats
    if (selectedUnit)
    {
        float unitMaxAp;
        float unitAp;
        float unitXp;
        float unitHealth;
        int unitTurnsAliveCt;

        unitMaxAp = selectedUnit->maxAp;
        unitAp = selectedUnit->ap;
        unitXp = selectedUnit->xp;
        unitHealth = selectedUnit->health;
        unitTurnsAliveCt = selectedUnit->turnsAliveCt;

        DrawText(TextFormat("Unit AP: %.2f/%.2f", unitAp, unitMaxAp), 100, 10, 16, WHITE);
        DrawText(TextFormat("Unit XP: %.2f", unitXp), 100, 30, 16, WHITE);
        DrawText(TextFormat("Unit Health: %.2f", unitHealth), 100, 50, 16, WHITE);
        DrawText(TextFormat("Unit Lifespan: %d", unitTurnsAliveCt), 100, 70, 16, WHITE);

        std::string prefix1 = "shft +";
        std::string prefix2 = "ctrl +";
        const char *prefix1cstr = prefix1.c_str();
        float prefix1W = MeasureText(prefix1cstr, 16);
        const char *prefix2cstr = prefix2.c_str();
        float prefix2W = MeasureText(prefix2cstr, 16);

        int abilityIdx = 0;
        for (auto &ability : selectedUnit->abilities)
        {
            auto color = WHITE;
            if (selectedAbility && selectedAbility->uuid == ability.uuid)
            {
                color = RED;
            }
            if (ability.unitApCost > selectedUnit->ap)
            {
                color = LIGHTGRAY;
            }
            if (ability.useQty && ability.qty == 0)
            {
                color = LIGHTGRAY;
            }
            const char *cstr = ability.displayTitle.c_str();
            if (abilityIdx < 10)
            {
                DrawText(TextFormat("%d", abilityIdx + 1), 10, 100 + (abilityIdx * 20), 16, color);
                DrawText(cstr, 20, 100 + (abilityIdx * 20), 16, color);
            }
            else if ((abilityIdx >= 10 && abilityIdx < 20))
            {
                DrawText(prefix1cstr, 10, 100, 16, WHITE);
                DrawText(TextFormat("%d", abilityIdx + 1), prefix1W + 10, 100 + (abilityIdx * 20), 16, color);
                DrawText(cstr, 20, 100 + (abilityIdx * 20), 16, color);
            }
            else
            {
                DrawText(prefix2cstr, 10, 100, 16, WHITE);
                DrawText(TextFormat("%d", abilityIdx + 1), prefix2W + 10, 100 + (abilityIdx * 20), 16, color);
                DrawText(cstr, 20, 100 + (abilityIdx * 20), 16, color);
            }
            abilityIdx++;
        }
    }
}

void sDrawSelectedUnitIndicator(Unit *&selectedUnit)
{
    if (selectedUnit)
    {
        DrawRectangleLinesEx({selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)}, 0.5, GREEN);
    }
}

void sDrawHoveredTileIndicator(Tile *&hoveredTile, Ability *&selectedAbility)
{
    if (!selectedAbility)
    {
        DrawRectangleLinesEx({hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}, 0.5, YELLOW);
    }
}

void sDrawEffectRadius(Unit *&selectedUnit, Ability *&selectedAbility, Tile *&hoveredTile)
{
    if (selectedAbility)
    {
        Rectangle effectRadiusRect = createRectAroundRect((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)}, selectedAbility->effectRadius);
        DrawRectangleLinesEx({effectRadiusRect.x, effectRadiusRect.y, effectRadiusRect.width, effectRadiusRect.height}, 0.5, BLUE);
    }
}

void sDrawAllTextures(std::vector<Unit> &allUnits, std::vector<Tile> &allTiles, Camera2D &camera, Player &player)
{
    for (auto &tile : allTiles)
    {
        if (isRectangleInViewport({tile.pos.x, tile.pos.y, 32.0f, 32.0f}, camera))
        {
            DrawTexture(tile.tex, tile.pos.x, tile.pos.y, WHITE);
            // DrawText(TextFormat("%.3f", tile.perlinNoise), tile.pos.x, tile.pos.y, 6, WHITE);
        }
    }

    for (auto &unit : allUnits)
    {
        if (shouldRenderUnitDueToVisibility(unit, player))
        {
            if (isRectangleInViewport({unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}, camera))
            {
                DrawTexture(unit.tex, unit.pos.x, unit.pos.y, WHITE);
            }
        }
    }
}

void sDrawRotationChevron(std::vector<Unit> &allUnits, Camera2D &camera, Texture2D &chevronTex, Player &player)
{
    for (auto &unit : allUnits)
    {
        if (isRectangleInViewport({unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}, camera))
        {
            if (shouldRenderUnitDueToVisibility(unit, player))
            {
                DrawTextureEx(chevronTex, {unit.pos.x, unit.pos.y}, 90.0f + 45.0f, 1.0f, WHITE);
            }
        }
    }
}

void sDrawVisionTrapezoids(std::vector<Unit> &allUnits, Camera2D &camera, Player &player)
{
    for (auto &unit : allUnits)
    {
        if (isRectangleInViewport({unit.pos.x, unit.pos.y, static_cast<float>(unit.tex.width), static_cast<float>(unit.tex.height)}, camera))
        {
            if (shouldRenderUnitDueToVisibility(unit, player))
            {
                Position originPos = unit.visionTrapezoid.originPos;
                Position p1 = unit.visionTrapezoid.p1;
                Position p2 = unit.visionTrapezoid.p2;
                Position p3 = unit.visionTrapezoid.p3;
                Position p4 = unit.visionTrapezoid.p4;
                // DrawText(TextFormat("%s", "oP"), originPos.x, originPos.y, 16, RED);
                DrawLineV({p1.x, p1.y}, {p2.x, p2.y}, ORANGE);
                // DrawText(TextFormat("%s", "p1"), p1.x, p1.y, 16, RED);
                DrawLineV({p2.x, p2.y}, {p3.x, p3.y}, ORANGE);
                // DrawText(TextFormat("%s", "p2"), p2.x, p2.y, 16, RED);
                DrawLineV({p3.x, p3.y}, {p4.x, p4.y}, ORANGE);
                // DrawText(TextFormat("%s", "p3"), p3.x, p3.y, 16, RED);
                DrawLineV({p4.x, p4.y}, {p1.x, p1.y}, ORANGE);
                // DrawText(TextFormat("%s", "p4"), p4.x, p4.y, 16, RED);
            }
        }
    }
}

void DEBUGsDrawAngleToHoveredTile(Unit *&selectedUnit, Tile *&hoveredTile)
{
    if (selectedUnit && hoveredTile)
    {
        Position selectedUnitCenter = getRectCenter((Rectangle){selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)});
        Position hoveredTileCenter = getRectCenter((Rectangle){hoveredTile->pos.x, hoveredTile->pos.y, static_cast<float>(hoveredTile->tex.width), static_cast<float>(hoveredTile->tex.height)});
        float angleToRotationTarget = getAngleBetweenPoints(selectedUnitCenter, hoveredTileCenter);
        DrawLine(selectedUnitCenter.x, selectedUnitCenter.y, hoveredTileCenter.x, hoveredTileCenter.y, WHITE);
        DrawText(TextFormat("%0.2f", angleToRotationTarget), hoveredTileCenter.x, hoveredTileCenter.y, 16, WHITE);
    }
}

void DEBUGsDrawFacingAngleIndicator(Unit *&selectedUnit)
{
    if (selectedUnit)
    {
        Position selectedUnitCenter = getRectCenter({selectedUnit->pos.x, selectedUnit->pos.y, static_cast<float>(selectedUnit->tex.width), static_cast<float>(selectedUnit->tex.height)});
        float selectedUnitFacingAngle = selectedUnit->facingAngle;
        float lineLength = 64.0f;

        // Convert angle to radians
        float angleInRadians = selectedUnitFacingAngle * (M_PI / 180.0f);

        // Calculate end point
        float endX = selectedUnitCenter.x + lineLength * std::cos(angleInRadians);
        float endY = selectedUnitCenter.y + lineLength * std::sin(angleInRadians);

        // Draw the line
        DrawLine(selectedUnitCenter.x, selectedUnitCenter.y, endX, endY, WHITE);
    }
}