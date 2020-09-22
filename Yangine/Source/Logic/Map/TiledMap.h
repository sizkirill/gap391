#pragma once
/** \file TiledMap.h */
/** Tiled Map description */
#include <vector>
#include <string>
#include <memory>
#include <Utils/Vector2.h>
#include <unordered_map>
#include <variant>
#include <optional>

namespace tinyxml2
{
    class XMLElement;
}

//! \namespace yang Contains all Yangine code
namespace yang
{
    class ITexture;
    class IGraphics;
/** \class TiledMap */
/** Class that stores Tiled Map data */
class TiledMap
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	TiledMap() = default;

	/** Default Destructor */
	virtual ~TiledMap()= default;

    /// Load map from the XML file
    /// \param filepath - path to the XML file that contains map description
    /// \return true if successfully loaded
    virtual bool LoadMap(const char* filepath);

    /// Render the map
    /// \param pGraphics - graphics system to use
    /// \return true if successfully rendered
    bool Render(IGraphics* pGraphics);

    /// Get point in the world from tile index
    /// \param index - tile index to get world point from
    /// \return point in the world space
    FVec2 GetWorldPointFromTileIndex(size_t index) const;

    /// Get tile index from point in the world
    /// \param position - position in the world coordinates
    /// \return index of the tile
    size_t GetTileIndexFromWorldPoint(FVec2 position) const;

    /// Get coordinates in map space from tile index
    /// \param index - tile index
    /// \return tile coordinates in map
    Vector2<size_t> GetCoordsFromIndex(size_t index) const;

    /// Get tile index from map coordinates
    /// \param coords - coordinates in map space
    /// \return tile index
    size_t GetIndexFromCoords(Vector2<size_t> coords) const;

    /// Get tile ID from index in the map
    /// \param layerIndex - index of the layer where tile lives
    /// \param index - tile index
    /// \return tile ID
    size_t GetTileIdFromIndex(size_t layerIndex, size_t tileIndex) const;

    /// Sets the tile ID for a tile at specified index in the specified layer
    /// \param layerIndex - index of the layer where tiles lives
    /// \param tileIndex - index of the tile in that layer
    /// \param tileId - tile ID to set
    void SetTileIdAtIndex(size_t layerIndex, size_t tileIndex, size_t tileId);

    template <typename PropertyType>
    std::optional<PropertyType> GetTileProperty(size_t tileIndex, const char* propertyName) const;

    /// \class TilesetData
    /// Contains information about the tileset used in the map
    class TilesetData
    {
    public:
        /// Constructor. Also initializes data from an xml element
        /// \param firstGid - first global tile ID of a tileset
        /// \param source - path to the tileset data file
        /// \param pData - XML element to initialize the tileset from
        TilesetData(int firstGid, std::string&& source, tinyxml2::XMLElement* pData);

        /// alias for the map of tile properties
        using TileProperties = std::unordered_map<uint32_t, std::variant<bool, int, float, std::string>>;

        /// Gets the specified property if it exists
        /// \tparam PropertyType - property type to try to get. Can be one of those: bool, int, float, std::string
        /// \param tileId - ID of the tile to get property for
        /// \param propertyName - name of the property
        /// \return std::optional with a property value if it exists or empty optional if there is no such property, or property contains other type.
        template <typename PropertyType>
        std::optional<PropertyType> GetProperty(int tileId, const char* propertyName) const;

        /// Getter for m_firstGid
        /// \return firstGid of this tileset
        int GetFirstGid() const { return m_firstGid; }

        /// Renders the tile
        /// \param pGraphics - renderer to use
        /// \param tileIndex - index of the tile to render
        /// \param dstPos - destination on the screen where to render
        /// \return true if rendered successfully
        bool RenderTile(IGraphics* pGraphics, size_t tileIndex, IVec2 dstPos) const;

        /// Getter for the texture of this tileset
        /// \return pointer to a texture
        ITexture* GetTilesetTexture() const;

        int GetTileCount() const { return m_tileCount; }
    private:
        int m_firstGid;         ///< The first global tile ID of this tileset (this global ID maps to the first tile in this tileset).
        std::string m_source;   ///< Path to the image containing this tileset

        int m_tileWidth;        ///< Width of the single tile
        int m_tileHeight;       ///< Height of the single tile
        int m_tileCount;        ///< Total number of tiles in the tileset
        int m_columns;          ///< Width of the tileset image in tiles (number of tile columns)

        int m_imageWidth;       ///< Width of the tileset image in pixels
        int m_imageHeight;      ///< Height of the tileset image in pixels

        std::shared_ptr<ITexture> m_pTilesetTexture;    ///< Texture that contains this tileset

        std::vector<TileProperties> m_tileProperties;   ///< Properties of tiles in this tileset

        /// Internal function to add properties to the tiles. Used in parsing xml
        /// \tparam PropertyType - type of the property to add. Can be one of these: bool, int, float, const char*
        /// \param name - name of the property
        /// \param value - value of the property
        template <typename PropertyType>
        void AddProperty(const char* name, PropertyType value);

        /// Reads tileset data from an xml element
        /// \param pData - XML element to read from
        void ReadTilesetData(tinyxml2::XMLElement* pData);
    };

    /// \struct TileData
    /// Contains data about a single tile
    struct TileData
    {
        int m_id;                           ///< Tile ID
    };

    /// \struct LayerData
    /// Contains data about a single layer
    struct LayerData
    {
        int m_id;               ///< Layer ID
        int m_width;            ///< Layer width in tiles
        int m_height;           ///< Layer height in tiles
        float m_opacity;        ///< Layer opacity
        uint32_t m_hashName;    ///< Hash value of the layer's name
        std::vector<TileData> m_tileData;   ///< Vector containing this layer's tiles data. \see yang::TiledMap::TileData
        bool m_isVisible;       ///< Is the layer visible?
    };

    /// \enum MapType
    /// Type of the map
	enum class MapType
	{
		kIsometric,             ///< Isometric Map
		kOrthogonal,            ///< Orthogonal Map
		kMaxTypes               ///< Maximum count of supported map types
	};

    /// \struct MapData
    /// Contains data describing the map
    struct MapData
    {
        int m_mapWidth;         ///< Map width in tiles
        int m_mapHeight;        ///< Map height in tiles
        int m_tileWidth;        ///< Width of a single tile in the map
        int m_tileHeight;       ///< Height of a single tile in the map

		MapType m_mapType;      ///< Map type \see yang::TiledMap::MapType
        std::vector<LayerData> m_layerData; ///< Vector that contains data for each layer in the map. \see yang::TiledMap::LayerData
    };

protected:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    MapData m_mapData;                      ///< Map data \see yang::TiledMap::MapData

    std::vector<TilesetData> m_tilesetData; ///< Tileset data for the map \see yang::TiledMap::TilesetData

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    size_t GetLayerIndex(const char* pName);

};
template<typename PropertyType>
inline std::optional<PropertyType> TiledMap::GetTileProperty(size_t tileIndex, const char* propertyName) const
{
    size_t tileId = m_mapData.m_layerData[0].m_tileData[tileIndex].m_id;

    auto tilesetIt = std::upper_bound(m_tilesetData.begin(), m_tilesetData.end(), tileId, [](int firstgid, const TilesetData& tilesetData)
        {
            return firstgid < tilesetData.GetFirstGid();
        });

    --tilesetIt;

    return tilesetIt->GetProperty<PropertyType>(tileId - tilesetIt->GetFirstGid(), propertyName);
}
}