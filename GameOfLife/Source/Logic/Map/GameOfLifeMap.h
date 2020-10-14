#pragma once
#include <vector>
#include <array>
#include <variant>
#include <thread>
#include <atomic>
#include <string_view>
#include <memory>
#include <shared_mutex>
#include <Utils/Vector2.h>
#include <Utils/Color.h>

namespace yang
{
    class Sprite;
    class IGraphics;
}

class GameOfLifeMap
{
public:
    enum class TileStatus
    {
        kDead,
        kAlive,
        kMaxTiles
    };

    class TileUpdater
    {
    private:
        enum class JobStatus
        {
            kWorking,
            kFinished,
            kMaxStatus
        };

        size_t m_numThreads = 0;
        const GameOfLifeMap& m_owner;
        std::vector<GameOfLifeMap::TileStatus>& m_currentTiles;
        std::vector<GameOfLifeMap::TileStatus>& m_tileBuffer;
        std::vector<std::thread> m_workers;
        std::shared_mutex m_mutex;
        std::condition_variable_any m_exitSignal;
        std::atomic<JobStatus> m_jobStatus;
    public:
        TileUpdater(const GameOfLifeMap& owner, std::vector<GameOfLifeMap::TileStatus>& tiles, std::vector<GameOfLifeMap::TileStatus>& buffer);
        ~TileUpdater();
        bool Init(size_t numThreads);
        void UpdateFrame();
        void UpdateOneThread(int startIndex, int endIndex);
    };

    GameOfLifeMap();

    bool Init(std::string_view pathToSettings);
    void UpdateFrame(float deltaSeconds);
    void Render(yang::IGraphics* pGraphics);

    int GetIndexFromGridPoint(int x, int y) const;
    int GetIndexFromGridPoint(yang::IVec2 gridPoint) const;
    yang::IVec2 GetGridPointFromIndex(int index) const;
    int LiveNeighbors(int index) const;
    bool CheckBounds(int x, int y) const;
    TileStatus GetTile(int index) const;
private:
    using Drawable = std::variant<yang::IColor, std::shared_ptr<yang::Sprite>>;

    float m_frameRate;
    float m_timeSinceLastFrame;
    yang::IVec2 m_mapSize;
    yang::IVec2 m_tileSize;
    std::vector<TileStatus> m_currentTiles;
    std::vector<TileStatus> m_tileBuffer;
    std::array<Drawable, static_cast<size_t>(TileStatus::kMaxTiles)> m_tileStatusToDrawable;
    TileUpdater m_tileUpdater;
};