#ifndef ODYSSEY_GAME_SERVER_FPS_GAME_H
#define ODYSSEY_GAME_SERVER_FPS_GAME_H

#include "FPSRules.h"
#include "FPS_Player.h"
#include "../../Buffers/LIFOCircularBuffer.h"
#include "../../game_state_generated.h"

#include <vector>
#include <map>




class FPS_Game {
public:
    FPS_Game();
    size_t createNewPlayer(Vector3 initPosition, Vector3 initVelocity, float dt);
    void updatePlayer(size_t playerIndex, bool w, bool a, bool s, bool d,Vector2 mouseDelta,bool shoot,bool space,float dt, bool sprint,bool crouch, float serverTickRate);
    void updateEntities();
    void checkEntityCollisions();
    void calculateDeltas();
    void buildServerFlatBuffer(flatbuffers::FlatBufferBuilder& builder,
                               flatbuffers::Offset<flatbuffers::String> sourcePoint,
                               flatbuffers::Offset<flatbuffers::String> destPoint,
                               bool reliable,
                               PacketType packetType,
                               bool delta);
//    void updatePreviousStates();
    void endGame(); // TODO: later
    unique_ptr<FPSClientState> getPlayerCurrentState(size_t index);
    unique_ptr<FPSClientState> getPlayerPreviousState(size_t index);
    unique_ptr<FPSClientState> getPlayerCumulativeState(size_t index);

    static const float getWallWidth();

    static const float getWallHeight();

    static const float getWallLength();

    static const float getFloorLength();

//    static const vector<FPSClientState> &getBuffer();

    static const vector<BoundingBox> &getTerrainVector();

    static const vector<BoundingBox> &getTopBoxVector();

//    const vector<unique_ptr<FPSClientState>> &getPreviousStatesOfPlayers() const;

//    const vector<unique_ptr<FPSClientState>> &getCurrentStatesOfPlayers() const;

//    const vector<unique_ptr<FPSClientState>> &getCumulativeDeltaStatesPlayers() const;

    static const void printVector3(const Vector3& vec);

    static const void printBoundingBox(const BoundingBox& box);

    static const void printCamera3D(const Camera3D& camera);

    static const void printFPSClientState(FPSClientState& state);

    static const bool cameraIsZero(const Camera3D& camera);

    static const bool vector3IsZero(const Vector3& vector);

    static const bool vector2IsZero(const Vector2& vector);

    static const bool boundingBoxIsZero(const BoundingBox& box);

private:
    static constexpr const float wallWidth = 1.0f;
    static constexpr const float wallHeight = 5.0f;
    static constexpr const float wallLength = 32.0f;
    static constexpr const float floorLength = 50.0f;
    static constexpr const size_t MAX_HISTORY_PACKETS = 50;

    static int playerIDCount;
//    static std::vector<FPSClientState> buffer;
    static std::vector<BoundingBox> terrainVector;
    static std::vector<BoundingBox> topBoxVector;

    map<int, LIFOCircularBuffer<unique_ptr<FPSClientState>>> playerStates;
    map<int, LIFOCircularBuffer<unique_ptr<FPSClientState>>> deltaStates;
//    FPSClientState currentStates;
//    vector<unique_ptr<FPSClientState>> previousStatesOfPlayers;
//    vector<unique_ptr<FPSClientState>> currentStatesOfPlayers;
//    vector<unique_ptr<FPSClientState>> cumulativeDeltaStatesPlayers;
};

#endif //ODYSSEY_GAME_SERVER_FPS_GAME_H