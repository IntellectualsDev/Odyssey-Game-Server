#ifndef ODYSSEY_GAME_SERVER_GAME_H
#define ODYSSEY_GAME_SERVER_GAME_H

#include "FPSRules.h"
#include "Player.h"
#include <vector>



class Game {
public:
    Game();
    size_t createNewPlayer(Vector3 initPosition, Vector3 initVelocity, Vector3 initHitBox, float dt);
    void updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, vector<BoundingBox> &terrainList,vector<BoundingBox> &topBoxVector,bool sprint,bool crouch);
    void updateEntities();
    void checkEntityCollisions();
    void endGame(); // TODO: later

    // order of game method calls:

    // 1. updatePlayer
    // 2. updateEntities
    // 3. checkEntityCollisions()
    // 4. Optional: display Server GUI



private:
    static constexpr const float wallWidth = 1.0f;
    static constexpr const float wallHeight = 5.0f;
    static constexpr const float wallLength = 32.0f;
    static constexpr const float floorLength = 50.0f;
    static std::vector<FPSClientState> buffer;
    static std::vector<BoundingBox> terrainVector;
    static std::vector<BoundingBox> topBoxVector;

    vector<FPSClientState*> previousStatesOfPlayers;
    vector<FPSClientState*> currentStatesOfPlayers;


};

#endif //ODYSSEY_GAME_SERVER_GAME_H