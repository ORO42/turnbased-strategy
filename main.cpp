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

// slot related functions
// Example function that emits a signal with a payload
void exampleFunction(Signal<std::string> &signal)
{
    std::cout << "Inside example function, emitting signal with payload...\n";
    signal.emit("Hello, world!");
}

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
    Unit *selectedUnit = nullptr;
    Unit *hoveredUnit = nullptr;
    Tile *hoveredTile = nullptr;
    Ability *selectedAbility = nullptr;
    std::vector<Unit> allUnits;
    std::vector<Tile> allTiles;
    std::vector<Obstacle> allObstacles;
    std::vector<GridSubdivision> allGridSubdivisions;
    // std::unordered_map<UnitType, float> unitCreationCostPlayerAp = {{UnitType::ATSOLIDER, 10.0f}, {UnitType::ENGINEER, 15.0f}};

    // UI Modes

    // define signals
    Signal<PrepareTrapezoidRotationPayload> prepareTrapezoidRotationSignal;

    // connect signal slots
    prepareTrapezoidRotationSignal.connect([&](const PrepareTrapezoidRotationPayload &payload)
                                           { setVisionTrapezoidRotation(payload); });

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

    // NOTE when a unit is selected, FPS tanks at higher gird size, indicating there is an optimization necessary for grid/tile related functions when the unit is selected. It is the sPrepareMove function
    setupRectangularGrid(192, 192, allTiles, grassTex);
    createGridSubdivisions(allGridSubdivisions, allTiles, 192 * 32, 192 * 32, 8, 8);

    Player player = {10.0f, 0.0f};
    player.ap = 24.0f;
    player.xp = 0.0f;

    Vector2 screenMousePos;
    Vector2 worldMousePos;

    createUnit(UnitType::RIFLEMAN, {320.0f, 320.0f}, player, allUnits, unitTex);
    // createUnit(UnitType::RIFLEMAN, {384.0f, 384.0f}, player, allUnits, unitTex);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        // Update
        screenMousePos = GetMousePosition();
        worldMousePos = GetScreenToWorld2D(screenMousePos, camera);

        sKeyInputMoveCamera(camera);
        sTileHover(allTiles, hoveredTile, worldMousePos);
        sSelectUnit(selectedUnit, selectedAbility, allUnits, worldMousePos);
        sAutoDeselectAbility(selectedUnit, selectedAbility);
        sSelectAbility(selectedUnit, selectedAbility, worldMousePos);
        sUseAbility(selectedUnit, selectedAbility, hoveredTile, player, allUnits, allObstacles, prepareTrapezoidRotationSignal);
        sClearStates(selectedUnit, selectedAbility);
        // sPositionVisionTrapezoids(allUnits);

        sDestroyUnits(allUnits, selectedUnit, selectedAbility);
        sDestroyObstacles(allObstacles);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // sDrawRectangularGrid();
        // sDrawTileIndicators();
        // sDrawDistanceAtMouse();

        BeginMode2D(camera);

        sDrawAllTextures(allUnits, allTiles, camera);
        sDrawSelectedUnitIndicator(selectedUnit);
        sDrawHoveredTileIndicator(hoveredTile, selectedAbility);
        sDrawEffectRadius(selectedUnit, selectedAbility, hoveredTile);
        sDrawRotationChevron(allUnits, camera, chevronTex);
        sDrawVisionTrapezoids(allUnits, camera);

        EndMode2D();
        // elements that follow camera are drawn outside of 2D mode
        sDrawUI(player, selectedUnit, allUnits, selectedAbility, hoveredTile);
        DrawFPS(screenHeight + 20, screenWidth / 2);

        EndDrawing();
        //----------------------------------------------------------------------------------

        currentFrame++;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}