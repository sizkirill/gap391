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
#include <Utils/Random.h>
#include "TileStatus.h"

namespace yang
{
    class Sprite;
    class IGraphics;
}

class Generator;

class CellularWorldMap
{
public:
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
        const CellularWorldMap& m_owner;
        std::vector<TileStatus>& m_currentTiles;
        std::vector<TileStatus>& m_tileBuffer;
        std::vector<std::thread> m_workers;
        std::shared_mutex m_mutex;
        std::condition_variable_any m_exitSignal;
        std::atomic<JobStatus> m_jobStatus;
        yang::XorshiftRNG m_rng;

        std::vector<std::shared_ptr<Generator>> m_generators;
        size_t m_currentGeneratorIndex;
    public:
        TileUpdater(const CellularWorldMap& owner, std::vector<TileStatus>& tiles, std::vector<TileStatus>& buffer);
        ~TileUpdater();
        bool Init(size_t numThreads, uint64_t rngSeed);
        void UpdateFrame();
        void AddGenerator(std::shared_ptr<Generator> pGen);
    private:
        void UpdateOneThread(int startIndex, int endIndex, yang::XorshiftRNG& rng);
    };

    CellularWorldMap();

    bool Init(std::string_view pathToSettings);
    void UpdateFrame(float deltaSeconds);
    void Render(yang::IGraphics* pGraphics);

    int GetIndexFromGridPoint(int x, int y) const;
    int GetIndexFromGridPoint(yang::IVec2 gridPoint) const;
    yang::IVec2 GetGridPointFromIndex(int index) const;
    std::array<int, static_cast<size_t>(TileStatus::kMaxTiles)> GetNeighborCounts(int index) const;
    std::array<int, static_cast<size_t>(TileStatus::kMaxTiles)> GetNeighborCounts(int index, int neighborhoodSize) const;
    int GetNeighborTileCount(int index, TileStatus tile, int neighborhoodSize = 1) const;
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
    std::vector<float> m_moistureMap;
    std::vector<float> m_elevationMap;
    std::array<Drawable, static_cast<size_t>(TileStatus::kMaxTiles)> m_tileStatusToDrawable;
    TileUpdater m_tileUpdater;
};