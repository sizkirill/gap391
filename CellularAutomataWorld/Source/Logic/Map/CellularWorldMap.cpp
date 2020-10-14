#include "CellularWorldMap.h"
#include <Application/Graphics/Textures/Sprite.h>
#include <Utils/Random.h>
#include <Application/Resources/ResourceCache.h>
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/StringHash.h>
#include <Logic/Map/Generators/LakeGenerator.h>
#include <Logic/Map/Generators/ForestGenerator.h>
#include <Logic/Map/Generators/ForestPropagator.h>
#include <cassert>

CellularWorldMap::CellularWorldMap()
	:m_tileUpdater(*this, m_currentTiles, m_tileBuffer)
{
}

bool CellularWorldMap::Init(std::string_view pathToSettings)
{
	using namespace tinyxml2;

	XMLDocument doc;
	XMLError error = doc.LoadFile(pathToSettings.data());

	if (error != tinyxml2::XML_SUCCESS)
	{
		LOG(Error, "Failed to load map settings: %s -- %s", pathToSettings.data(), XMLDocument::ErrorIDToName(error));
		return false;
	}

	XMLElement* pRoot = doc.RootElement();

	XMLElement* pMapSize = pRoot->FirstChildElement("MapSize");
	if (pMapSize)
	{
		m_mapSize = yang::IVectorFromXML(pMapSize);
	}

	XMLElement* pTileSize = pRoot->FirstChildElement("TileSize");
	if (pTileSize)
	{
		m_tileSize = yang::FVectorFromXML(pTileSize);
	}

	m_frameRate = pRoot->FloatAttribute("frameRate", 0.03f);
	m_timeSinceLastFrame = 0;

	m_currentTiles.resize(m_mapSize.x * m_mapSize.y);
	m_tileBuffer.resize(m_mapSize.x * m_mapSize.y);

	XMLElement* pMapTileset = pRoot->FirstChildElement("Tileset");

	using namespace yang::literals;
	static constexpr uint32_t tileNames[static_cast<size_t>(TileStatus::kMaxTiles)] =
	{
	"grass"_hash32,
	"lake"_hash32,
	"forest"_hash32,
	"village"_hash32
	};

	auto spriteLambda = [](std::string_view spriteName, uint32_t defaultVal)
	{
		uint32_t hashVal = StringHash32(spriteName.data());
		auto ptr = std::find(tileNames, tileNames + static_cast<size_t>(TileStatus::kMaxTiles), hashVal);
		return ptr - tileNames;
	};

	if (pMapTileset)
	{
		const char* pTextureSrc = pMapTileset->Attribute("src");

		if (!pTextureSrc)
		{
			LOG(Error, "Texture haven't been found");
			return false;
		}

		auto pTilesetTexture = yang::ResourceCache::Get()->Load<yang::ITexture>(pTextureSrc);

		for (XMLElement* pSprite = pMapTileset->FirstChildElement("Sprite"); pSprite != nullptr; pSprite = pSprite->NextSiblingElement("Sprite"))
		{
			const char* pSpriteName = pSprite->Attribute("name");
			if (!pSpriteName)
			{
				LOG(Warning, "Sprite doesn't have a name");
				continue;
			}

			m_tileStatusToDrawable[spriteLambda(pSpriteName, "grass"_hash32)] = std::make_shared<yang::Sprite>(pTilesetTexture, yang::IRectFromXML(pSprite->FirstChildElement("SourceRect")), yang::TextureDrawParams{});
		}
	}

	yang::XorshiftRNG rng;
	std::generate(m_tileBuffer.begin(), m_tileBuffer.end(), [rng]() mutable 
		{
			if (rng.FRand<float>() < 0.0005f)
			{
				return TileStatus::kLake;
			}

			return TileStatus::kGrass;
		});

	size_t numThreads = pRoot->UnsignedAttribute("threads", 8);

	m_tileUpdater.Init(numThreads, rng.GetState());
	m_tileUpdater.AddGenerator(std::make_shared<LakeGenerator>(16, 1, 1, 0.3f));
	m_tileUpdater.AddGenerator(std::make_shared<ForestGenerator>(1, 3, 0.001f, [](int count) {return Math::SmootherStep(static_cast<float>(count) / 100.f); }));
	m_tileUpdater.AddGenerator(std::make_shared<ForestPropagator>(14, 
		ScoreFuncWrapper{3, [](int count) {return Math::SmootherStep(static_cast<float>(count) / 100.f); } },
		ScoreFuncWrapper{1, [](int count) {return Math::SmootherStep(static_cast<float>(count) / 5.f); } },
		ScoreFuncWrapper()
		));

	return true;
}

void CellularWorldMap::UpdateFrame(float deltaSeconds)
{
	m_timeSinceLastFrame += deltaSeconds;
	if (m_timeSinceLastFrame > m_frameRate)
	{
		m_tileUpdater.UpdateFrame();
		m_timeSinceLastFrame = 0;
	}
}

void CellularWorldMap::Render(yang::IGraphics* pGraphics)
{
	bool success = true;

	for (int i = 0; i < (int)m_currentTiles.size(); ++i)
	{
		yang::IRect tileTarget;
		auto gridPoint = GetGridPointFromIndex(i);
		tileTarget.x = (m_tileSize.x + 1) * gridPoint.x;
		tileTarget.y = (m_tileSize.y + 1) * gridPoint.y;
		tileTarget.height = m_tileSize.y;
		tileTarget.width = m_tileSize.x;

		success = success && std::visit([pGraphics, tileTarget](auto&& drawable) -> bool
			{
				using Type = std::decay_t<decltype(drawable)>;
				if constexpr (std::is_same_v<Type, std::shared_ptr<yang::Sprite>>)
				{
					return pGraphics->DrawSprite(drawable, tileTarget);
				}
				else if constexpr (std::is_same_v<Type, yang::IColor>)
				{
					return pGraphics->FillRect(tileTarget, drawable);
				}
			}, m_tileStatusToDrawable[static_cast<size_t>(m_currentTiles[i])]);
	}
}

int CellularWorldMap::GetIndexFromGridPoint(int x, int y) const
{
	return y * m_mapSize.x + x;
}

int CellularWorldMap::GetIndexFromGridPoint(yang::IVec2 gridPoint) const
{
	return GetIndexFromGridPoint(gridPoint.x, gridPoint.y);
}

yang::IVec2 CellularWorldMap::GetGridPointFromIndex(int index) const
{
	return { index % m_mapSize.x, index / m_mapSize.x };
}

std::array<int, static_cast<size_t>(TileStatus::kMaxTiles)> CellularWorldMap::GetNeighborCounts(int index) const
{
	return GetNeighborCounts(index, 1);
}

std::array<int, static_cast<size_t>(TileStatus::kMaxTiles)> CellularWorldMap::GetNeighborCounts(int index, int neighborhoodSize) const
{
	std::array<int, static_cast<size_t>(TileStatus::kMaxTiles)> result;
	std::fill(result.begin(), result.end(), 0);

	yang::IVec2 gridPoint = GetGridPointFromIndex(index);

	for (int y = gridPoint.y - neighborhoodSize; y < gridPoint.y + neighborhoodSize + 1; ++y)
	{
		for (int x = gridPoint.x - neighborhoodSize; x < gridPoint.x + neighborhoodSize + 1; ++x)
		{
			if (x == gridPoint.x && y == gridPoint.y)
			{
				continue;
			}

			if (CheckBounds(x, y))
			{
				++result[static_cast<size_t>(m_currentTiles[GetIndexFromGridPoint(x, y)])];
			}
		}
	}
	return result;
}

int CellularWorldMap::GetNeighborTileCount(int index, TileStatus tile, int neighborhoodSize) const
{
	return GetNeighborCounts(index, neighborhoodSize)[static_cast<size_t>(tile)];
}

bool CellularWorldMap::CheckBounds(int x, int y) const
{
	return !(x < 0 || x >= m_mapSize.x || y < 0 || y >= m_mapSize.y);
}

TileStatus CellularWorldMap::GetTile(int index) const
{
	assert(index >= 0 && index < m_mapSize.x * m_mapSize.y);
	return m_currentTiles[index];
}

CellularWorldMap::TileUpdater::TileUpdater(const CellularWorldMap& owner, std::vector<TileStatus>& tiles, std::vector<TileStatus>& buffer)
	:m_numThreads(0)
	,m_owner(owner)
	,m_currentTiles(tiles)
	,m_tileBuffer(buffer)
	,m_workers()
	,m_mutex()
{
}

CellularWorldMap::TileUpdater::~TileUpdater()
{
	m_jobStatus = JobStatus::kFinished;
	m_exitSignal.notify_all();
	for (auto& t : m_workers)
	{
		t.join();
	}
}

bool CellularWorldMap::TileUpdater::Init(size_t numThreads, uint64_t rngSeed)
{
	m_numThreads = numThreads;
	m_rng = yang::XorshiftRNG(rngSeed);
	m_workers.reserve(m_numThreads);

	size_t workAmount = m_currentTiles.size() / m_numThreads;


	for (size_t i = 0; i < m_numThreads; ++i)
	{
		int startIndex = static_cast<int>(i * workAmount);
		int endIndex = (i == m_numThreads - 1 ? (int)m_currentTiles.size() : startIndex + (int)workAmount);
		yang::XorshiftRNG threadRng(m_rng());
		m_workers.emplace_back([this, threadRng] (int startIndex, int endIndex) mutable
			{
				UpdateOneThread(startIndex, endIndex, threadRng);
			}, startIndex, endIndex);
	}

	return true;
}

void CellularWorldMap::TileUpdater::UpdateFrame()
{
	if (m_currentGeneratorIndex >= m_generators.size())
	{
		return;
	}

	auto pGenerator = m_generators[m_currentGeneratorIndex];

	if (pGenerator && pGenerator->Iterations() <= 0)
	{
		std::unique_lock lock(m_mutex);
		++m_currentGeneratorIndex;

		if (m_currentGeneratorIndex >= m_generators.size())
			return;
	}

	if (m_mutex.try_lock())
	{
		std::swap(m_currentTiles, m_tileBuffer);
		m_mutex.unlock();
		m_exitSignal.notify_all();
		m_generators[m_currentGeneratorIndex]->Decrement();
	}
}

void CellularWorldMap::TileUpdater::AddGenerator(std::shared_ptr<Generator> pGen)
{
	std::unique_lock lock(m_mutex);
	m_generators.push_back(pGen);
}

void CellularWorldMap::TileUpdater::UpdateOneThread(int startIndex, int endIndex, yang::XorshiftRNG& rng)
{
	LOG(Info, "Spinning up thread ID: %d", std::this_thread::get_id());

	while (m_jobStatus != JobStatus::kFinished)
	{
		std::shared_lock sharedLock(m_mutex);
		m_exitSignal.wait(sharedLock);

		if (m_jobStatus == JobStatus::kFinished)
		{
			LOG(Info, "Thread ID: %d finished", std::this_thread::get_id());
			return;
		}

		if (m_currentGeneratorIndex >= m_generators.size())
		{
			LOG(Info, "Thread ID: %d. Invalid generator index. Something bad happened", std::this_thread::get_id());
			return;
		}

		for (int i = startIndex; i < endIndex; ++i)
		{
			m_generators[m_currentGeneratorIndex]->Propagate(i, m_currentTiles, m_tileBuffer, m_owner, rng);
		}
		
		sharedLock.unlock();
	}
}