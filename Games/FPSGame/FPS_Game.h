#ifndef ODYSSEY_GAME_SERVER_FPS_GAME_H
#define ODYSSEY_GAME_SERVER_FPS_GAME_H

#include "FPSRules.h"
#include "FPS_Player.h"
#include <vector>



class FPS_Game {
public:
    FPS_Game();
    size_t createNewPlayer(Vector3 initPosition, Vector3 initVelocity, float dt);
    void updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, bool sprint,bool crouch);
    void updateEntities();
    void checkEntityCollisions();
    void calculateDeltas();
    void updatePreviousStates();
    void endGame(); // TODO: later
    FPSClientState* getPlayerCurrentState(size_t index);
    FPSClientState* getPlayerPreviousState(size_t index);
    FPSClientState* getPlayerCumulativeState(size_t index);

    static const float getWallWidth();

    static const float getWallHeight();

    static const float getWallLength();

    static const float getFloorLength();

    static const vector<FPSClientState> &getBuffer();

    static const vector<BoundingBox> &getTerrainVector();

    static const vector<BoundingBox> &getTopBoxVector();

    const vector<FPSClientState *> &getPreviousStatesOfPlayers() const;

    const vector<FPSClientState *> &getCurrentStatesOfPlayers() const;

    const vector<FPSClientState *> &getCumulativeDeltaStatesPlayers() const;

private:
    static constexpr const float wallWidth = 1.0f;
    static constexpr const float wallHeight = 5.0f;
    static constexpr const float wallLength = 32.0f;
    static constexpr const float floorLength = 50.0f;
//    static std::vector<FPSClientState> buffer;
    static std::vector<BoundingBox> terrainVector;
    static std::vector<BoundingBox> topBoxVector;

    vector<FPSClientState*> previousStatesOfPlayers;
    vector<FPSClientState*> currentStatesOfPlayers;
    vector<FPSClientState*> cumulativeDeltaStatesPlayers;

};

#endif //ODYSSEY_GAME_SERVER_FPS_GAME_H