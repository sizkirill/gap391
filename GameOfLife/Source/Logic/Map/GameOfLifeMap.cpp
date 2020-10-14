#include "GameOfLifeMap.h"
#include <Application/Graphics/Textures/Sprite.h>
#include <Utils/Random.h>
#include <Application/Resources/ResourceCache.h>
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/StringHash.h>
#include <cassert>

GameOfLifeMap::GameOfLifeMap()
	:m_tileUpdater(*this, m_currentTiles, m_tileBuffer)
{
}

bool GameOfLifeMap::Init(std::string_view pathToSettings)
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
	"dead"_hash32,
	"alive"_hash32
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

			m_tileStatusToDrawable[spriteLambda(pSpriteName, "dead"_hash32)] = std::make_shared<yang::Sprite>(pTilesetTexture, yang::IRectFromXML(pSprite->FirstChildElement("SourceRect")), yang::TextureDrawParams{});
		}
	}

	yang::XorshiftRNG rng;
	std::generate(m_tileBuffer.begin(), m_tileBuffer.end(), [rng]() mutable {return static_cast<TileStatus>(rng.Rand(static_cast<size_t>(TileStatus::kMaxTiles))); });

	size_t numThreads = pRoot->UnsignedAttribute("threads", 8);
	m_tileUpdater.Init(numThreads);

	return true;
}

void GameOfLifeMap::UpdateFrame(float deltaSeconds)
{
	m_timeSinceLastFrame += deltaSeconds;
	if (m_timeSinceLastFrame > m_frameRate)
	{
		m_tileUpdater.UpdateFrame();
		m_timeSinceLastFrame = 0;
	}
}

void GameOfLifeMap::Render(yang::IGraphics* pGraphics)
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

int GameOfLifeMap::GetIndexFromGridPoint(int x, int y) const
{
	return y * m_mapSize.x + x;
}

int GameOfLifeMap::GetIndexFromGridPoint(yang::IVec2 gridPoint) const
{
	return GetIndexFromGridPoint(gridPoint.x, gridPoint.y);
}

yang::IVec2 GameOfLifeMap::GetGridPointFromIndex(int index) const
{
	return { index % m_mapSize.x, index / m_mapSize.x };
}

int GameOfLifeMap::LiveNeighbors(int index) const
{
	yang::IVec2 gridPoint = GetGridPointFromIndex(index);
	int liveNeighbors = 0;
	for (int y = gridPoint.y - 1; y < gridPoint.y + 2; ++y)
	{
		for (int x = gridPoint.x - 1; x < gridPoint.x + 2; ++x)
		{
			if (x == gridPoint.x && y == gridPoint.y)
			{
				continue;
			}

			if (CheckBounds(x, y) && m_currentTiles[GetIndexFromGridPoint(x, y)] == TileStatus::kAlive)
			{
				++liveNeighbors;
			}
		}
	}
	return liveNeighbors;
}

bool GameOfLifeMap::CheckBounds(int x, int y) const
{
	return !(x < 0 || x >= m_mapSize.x || y < 0 || y >= m_mapSize.y);
}

GameOfLifeMap::TileStatus GameOfLifeMap::GetTile(int index) const
{
	assert(index >= 0 && index < m_mapSize.x * m_mapSize.y);
	return m_currentTiles[index];
}

GameOfLifeMap::TileUpdater::TileUpdater(const GameOfLifeMap& owner, std::vector<GameOfLifeMap::TileStatus>& tiles, std::vector<GameOfLifeMap::TileStatus>& buffer)
	:m_numThreads(0)
	,m_owner(owner)
	,m_currentTiles(tiles)
	,m_tileBuffer(buffer)
	,m_workers()
	,m_mutex()
{
}

GameOfLifeMap::TileUpdater::~TileUpdater()
{
	m_jobStatus = JobStatus::kFinished;
	m_exitSignal.notify_all();
	for (auto& t : m_workers)
	{
		t.join();
	}
}

bool GameOfLifeMap::TileUpdater::Init(size_t numThreads)
{
	m_numThreads = numThreads;
	m_workers.reserve(m_numThreads);

	size_t workAmount = m_currentTiles.size() / m_numThreads;


	for (size_t i = 0; i < m_numThreads; ++i)
	{
		int startIndex = static_cast<int>(i * workAmount);
		int endIndex = (i == m_numThreads - 1 ? (int)m_currentTiles.size() : startIndex + (int)workAmount);
		m_workers.emplace_back([this](int startIndex, int endIndex)
			{
				UpdateOneThread(startIndex, endIndex);
			}, startIndex, endIndex);
	}

	return true;
}

void GameOfLifeMap::TileUpdater::UpdateFrame()
{
	if (m_mutex.try_lock())
	{
		std::swap(m_currentTiles, m_tileBuffer);
		m_mutex.unlock();
		m_exitSignal.notify_all();
	}
}

void GameOfLifeMap::TileUpdater::UpdateOneThread(int startIndex, int endIndex)
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

		for (int i = startIndex; i < endIndex; ++i)
		{
			int liveNeighbors = m_owner.LiveNeighbors(i);
			if (m_owner.GetTile(i) == TileStatus::kAlive && (liveNeighbors == 2 || liveNeighbors == 3))
			{
				m_tileBuffer[i] = TileStatus::kAlive;
			}
			else if (m_owner.GetTile(i) == TileStatus::kDead && liveNeighbors == 3)
			{
				m_tileBuffer[i] = TileStatus::kAlive;
			}
			else
			{
				m_tileBuffer[i] = TileStatus::kDead;
			}
		}
		
		sharedLock.unlock();
	}
}
