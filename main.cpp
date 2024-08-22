#include <iostream>
#include <unordered_map>
#include "include/raylib.h"
#include "include/rlgl.h"
#include "include/components.h"
#include "include/gridHelpers.h"
#include "include/cameraHelpers.h"
#include "include/sig.h"
#include "include/uiHelpers.h"
#include "include/uuid.h"
#include "include/unitHelpers.h"
#include "include/selectionHelpers.h"
#include "include/abilityHelpers.h"
#include "include/destructionHelpers.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    Camera2D camera = {0};
    camera.target = {screenWidth / 2, screenHeight / 2};
    camera.offset = {screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // maps
    std::unordered_map<UnitType, std::string> unitStrMap;
    unitStrMap[UnitType::NA] = "";
    unitStrMap[UnitType::RIFLEMAN] = "Rifleman";

    std::unordered_map<ObstacleType, std::string> obstacleStrMap;
    obstacleStrMap[ObstacleType::WALL] = "Wall";

    std::unordered_map<TerrainType, std::string> terrainStrMap;
    terrainStrMap[TerrainType::FOREST] = "Forest";
    terrainStrMap[TerrainType::HIGHGROUND] = "High ground";
    terrainStrMap[TerrainType::LOWGROUND] = "Low ground";
    terrainStrMap[TerrainType::MUD] = "Mud";
    terrainStrMap[TerrainType::NA] = "";
    terrainStrMap[TerrainType::NORMAL] = "Normal";

    std::unordered_map<Stance, std::string> stanceStrMap;
    stanceStrMap[Stance::NA] = "";
    stanceStrMap[Stance::PRONE] = "Prone";
    stanceStrMap[Stance::STANDING] = "Standing";

    // my vars
    int currentFrame;
    int currentTurn;
    float deltaTime;
    SharedPointer<Unit> selectedUnit = nullptr;
    SharedPointer<Unit> hoveredUnit = nullptr;
    SharedPointer<Tile> hoveredTile = nullptr;
    SharedPointer<Obstacle> hoveredObstacle = nullptr;
    SharedPointer<Ability> selectedAbility = nullptr;
    VectorSharedPointer<Unit> allUnits;
    VectorSharedPointer<Tile> allTiles;
    VectorSharedPointer<Obstacle> allObstacles;
    VectorSharedPointer<GridSubdivision> allGridSubdivisions;
    VectorSharedPointer<Projectile> allProjectiles;
    // std::unordered_map<UnitType, float> unitCreationCostPlayerAp = {{UnitType::ATSOLIDER, 10.0f}, {UnitType::ENGINEER, 15.0f}};

    // UI Modes

    // define signals
    // Signal<PrepareTrapezoidRotationPayload> prepareTrapezoidRotationSignal;

    // // // connect signal slots
    // prepareTrapezoidRotationSignal.connect([&](const PrepareTrapezoidRotationPayload &payload)
    //                                        { setVisionTrapezoidRotation(payload); });

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetWindowPosition(0, 0);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Load textures, must be done after InitWindow()
    Texture2D crateTex = LoadTexture("assets/crate_png.png");
    Texture2D grassTex = LoadTexture("assets/Grass_seamless.png");
    Texture2D fobTex = LoadTexture("assets/fob.png");
    Texture2D stoneTileTex = LoadTexture("assets/stone_tile.png");
    Texture2D unitTex = LoadTexture("assets/unit.png");
    Texture2D HQTex = LoadTexture("assets/mil_tent.png");
    Texture2D goldmineTex = LoadTexture("assets/goldmine.png");
    Texture2D oildrillTex = LoadTexture("assets/oildrill.png");
    Texture2D proxsensorTex = LoadTexture("assets/proximitysensor.png");
    Texture2D sandbagTex = LoadTexture("assets/sandbag.png");
    Texture2D silvermineTex = LoadTexture("assets/silvermine.png");
    Texture2D trenchTex = LoadTexture("assets/trench.png");
    Texture2D tunnelTex = LoadTexture("assets/tunnel.png");
    Texture2D wallTex = LoadTexture("assets/wall.png");
    Texture2D chevronTex = LoadTexture("assets/chevron.png");
    Texture2D projectileTex = LoadTexture("assets/projectile.png");

    setupRectangularGrid(192, 192, allTiles, grassTex);
    createGridSubdivisions(allGridSubdivisions, allTiles, 192 * 32, 192 * 32, 8, 8);

    SharedPointer<Player> player = std::make_shared<Player>(Player{10.0f, 0.0f});
    player->ap = 24.0f;
    player->xp = 0.0f;
    player->team = Teams::BLUETEAM;

    Vector2 screenMousePos;
    Vector2 worldMousePos;

    createUnit(UnitType::RIFLEMAN, {320.0f, 320.0f}, player, Teams::BLUETEAM, allUnits, unitTex);
    createUnit(UnitType::RIFLEMAN, {320.0f + 32.0f, 320.0f}, player, Teams::BLUETEAM, allUnits, unitTex);
    createUnit(UnitType::RIFLEMAN, {320.0f, 320.0f + 32.0f * 2}, player, Teams::REDTEAM, allUnits, unitTex);
    createObstacle(ObstacleType::WALL, {320.0f, 320.0f + 32.0f}, wallTex, allObstacles);
    createObstacle(ObstacleType::WALL, {320.0f, 320.0f + 32.0f * 2}, wallTex, allObstacles);
    createObstacle(ObstacleType::WALL, {320.0f, 320.0f + 32.0f * 3}, wallTex, allObstacles);
    createObstacle(ObstacleType::WALL, {320.0f, 320.0f + 32.0f * 4}, wallTex, allObstacles);
    createObstacle(ObstacleType::WALL, {320.0f, 320.0f + 32.0f * 5}, wallTex, allObstacles);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        // Update
        screenMousePos = GetMousePosition();
        worldMousePos = GetScreenToWorld2D(screenMousePos, camera);
        deltaTime = GetFrameTime();

        sKeyInputMoveCamera(camera);
        sTileHover(allTiles, hoveredTile, worldMousePos);

        // sUnitHover(allUnits, hoveredUnit, worldMousePos);
        // sObstacleHover(allObstacles, hoveredObstacle, worldMousePos);

        sSelectUnit(selectedUnit, selectedAbility, allUnits, worldMousePos, player);
        sAutoDeselectAbility(selectedUnit, selectedAbility);
        sSelectAbility(selectedUnit, selectedAbility, worldMousePos);
        sUseAbility(selectedUnit, selectedAbility, hoveredTile, hoveredUnit, hoveredObstacle, player, allUnits, allObstacles, worldMousePos, allGridSubdivisions, allProjectiles, projectileTex);
        sClearStates(selectedUnit, selectedAbility);
        sPositionVisionTrapezoids(allUnits);
        sVisibility(allUnits, player);
        sMoveUnits(allUnits, deltaTime);
        // sMoveProjectiles(allProjectiles, allUnits, allObstacles, deltaTime);
        // sProjectileDamage(allProjectiles, allUnits, allObstacles);
        // DEBUGsHoveredTileOverlappingTrap(hoveredTile, allUnits);

        sDestroyUnits(allUnits, selectedUnit, selectedAbility);
        sDestroyObstacles(allObstacles);
        sDestroyProjectiles(allProjectiles);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // sDrawRectangularGrid();
        // sDrawTileIndicators();
        // sDrawDistanceAtMouse();

        BeginMode2D(camera);

        sDrawAllTextures(allUnits, allTiles, allObstacles, allProjectiles, camera, player);
        sDrawSelectedUnitIndicator(selectedUnit);
        sDrawHoveredTileIndicator(hoveredTile, selectedAbility);
        sDrawaccuracyRadius(selectedUnit, selectedAbility, hoveredTile);
        sDrawRotationChevron(allUnits, camera, chevronTex, player);
        sDrawVisionTrapezoids(allUnits, camera, player);
        // DEBUGsDrawAngleToHoveredTile(selectedUnit, hoveredTile);
        sDrawFacingAngleIndicator(allUnits, player);
        sDrawDistanceIndicators(selectedAbility, selectedUnit, hoveredTile, allObstacles, allUnits, worldMousePos);
        sDrawReachRadiusRect(selectedAbility, selectedUnit);
        sDrawHealthValues(allUnits, allObstacles);

        EndMode2D();
        // elements that follow camera are drawn outside of 2D mode
        sDrawUI(player, selectedUnit, allUnits, selectedAbility, hoveredTile);
        DrawRectangle(screenHeight + 20, screenWidth / 2, 64.0, 32.0, WHITE);
        DrawFPS(screenHeight + 20, screenWidth / 2);

        EndDrawing();
        //----------------------------------------------------------------------------------

        currentFrame++;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(crateTex);
    UnloadTexture(grassTex);
    UnloadTexture(fobTex);
    UnloadTexture(stoneTileTex);
    UnloadTexture(unitTex);
    UnloadTexture(HQTex);
    UnloadTexture(goldmineTex);
    UnloadTexture(oildrillTex);
    UnloadTexture(proxsensorTex);
    UnloadTexture(sandbagTex);
    UnloadTexture(silvermineTex);
    UnloadTexture(trenchTex);
    UnloadTexture(tunnelTex);
    UnloadTexture(wallTex);
    UnloadTexture(chevronTex);
    UnloadTexture(projectileTex);
    // TODO once sound is added, unload sound
    // UnloadSound(fxWav); // Unload sound data
    // CloseAudioDevice(); // Close audio device
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}