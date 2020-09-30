#include "BspTree.h"
#include <Utils/Random.h>

BspTreeNode::BspTreeNode(BspTreeNode* pParent, yang::IRect areaRect, yang::XorshiftRNG* pRng, yang::IVec2 roomHeightRange, yang::IVec2 roomWidthRange)
    :m_pParent(pParent)
    , m_pLeft(nullptr)
    , m_pRight(nullptr)
    , m_areaRect(areaRect)
    , m_pRng(pRng)
    , m_roomHeightRange(roomHeightRange)
    , m_roomWidthRange(roomWidthRange)
{

}

BspTreeNode::BspTreeNode(yang::IRect areaRect, yang::XorshiftRNG* pRng, yang::IVec2 roomHeightRange, yang::IVec2 roomWidthRange)
    :m_pParent(nullptr)
    , m_pLeft(nullptr)
    , m_pRight(nullptr)
    , m_areaRect(areaRect)
    , m_pRng(pRng)
    , m_roomHeightRange(roomHeightRange)
    , m_roomWidthRange(roomWidthRange)
{

}

BspTreeNode::~BspTreeNode()
{
    delete m_pLeft;
    delete m_pRight;
}

void BspTreeNode::Split()
{
    bool haveChoice = IsSplittableHorizontally() && IsSplittableVertically();
    bool splittingHorizontally = IsSplittableHorizontally();

    if (haveChoice)
        splittingHorizontally = static_cast<bool>(m_pRng->Rand(2));

    if (splittingHorizontally)
    {
        int low = m_areaRect.y + m_roomHeightRange.x;
        int high = m_areaRect.y + m_areaRect.height - m_roomHeightRange.x;

        // ?
        //if (high < low)
        //    return;

        int splitY = m_pRng->Rand(low, high + 1);
        m_pLeft = new BspTreeNode(this, { m_areaRect.x, m_areaRect.y, m_areaRect.width, splitY - m_areaRect.y }, m_pRng, m_roomHeightRange, m_roomWidthRange);
        m_pRight = new BspTreeNode(this, { m_areaRect.x, splitY, m_areaRect.width, m_areaRect.y + m_areaRect.height - splitY }, m_pRng, m_roomHeightRange, m_roomWidthRange);
    }
    else
    {
        int low = m_areaRect.x + m_roomWidthRange.x;
        int high = m_areaRect.x + m_areaRect.width - m_roomWidthRange.x;

        // ?
        //if (high < low)
        //    return;

        int splitX = m_pRng->Rand(low, high + 1);
        m_pLeft = new BspTreeNode(this, { m_areaRect.x, m_areaRect.y, splitX - m_areaRect.x, m_areaRect.height }, m_pRng, m_roomHeightRange, m_roomWidthRange);
        m_pRight = new BspTreeNode(this, { splitX, m_areaRect.y, m_areaRect.x + m_areaRect.width - splitX, m_areaRect.height }, m_pRng, m_roomHeightRange, m_roomWidthRange);
    }
}

bool BspTreeNode::IsSplittableHorizontally() const
{
    return m_areaRect.height > m_roomHeightRange.x * 2;
}

bool BspTreeNode::IsSplittableVertically() const
{
    return m_areaRect.width > m_roomWidthRange.x * 2;
}

bool BspTreeNode::IsSplittable() const
{
    return IsSplittableHorizontally() || IsSplittableVertically();
}

bool BspTreeNode::WasSplitHorizontally() const
{
    if (!m_pLeft || !m_pRight)
        return false;

    return m_pLeft->m_areaRect.x == m_pRight->m_areaRect.x;
}

bool BspTreeNode::WasSplitVertically() const
{
    if (!m_pLeft || !m_pRight)
        return false;

    return m_pLeft->m_areaRect.y == m_pRight->m_areaRect.y;
}

std::pair<yang::IVec2, yang::IVec2> BspTreeNode::GenerateDoorTileIndices() const
{
    yang::IVec2 result;
    yang::IVec2 adjacent;

    if (WasSplitHorizontally())
    {
        result.x = m_pRng->Rand(m_areaRect.x, m_areaRect.x + m_areaRect.width);

        result.y = std::max(m_pLeft->m_areaRect.y, m_pRight->m_areaRect.y);

        adjacent = { result.x, result.y - 1 };
    }
    else
    {
        result.y = m_pRng->Rand(m_areaRect.y, m_areaRect.y + m_areaRect.height);

        result.x = std::max(m_pLeft->m_areaRect.x, m_pRight->m_areaRect.x);

        adjacent = { result.x - 1, result.y };
    }

    return { result, adjacent };
}

std::vector<std::unordered_set<BspTreeNode*>> BspTreeNode::GetNodesByDepth()
{
    std::vector<std::unordered_set<BspTreeNode*>> result;
    result.emplace_back();
    result[0].insert(this);
    GetNodesByDepthHelper(result, 1);
    return result;
}

std::vector<BspTreeNode*> BspTreeNode::Split(BspTreeNode* pRoot, int desiredRoomCount)
{
    std::vector<BspTreeNode*> leafNodes;
    std::vector<BspTreeNode*> roomNodes;
    leafNodes.push_back(pRoot);

    while (!leafNodes.empty())
        //while(roomNodes.size() < desiredRoomCount)
    {
        auto pLeafIter = leafNodes.begin() + pRoot->m_pRng->Rand(leafNodes.size());
        auto pLeaf = *pLeafIter;
        pLeaf->Split();

        std::iter_swap(pLeafIter, leafNodes.end() - 1);
        leafNodes.pop_back();

        if (auto pLeftLeaf = pLeaf->m_pLeft; pLeftLeaf && pLeftLeaf->IsSplittable())
        {
            leafNodes.push_back(pLeftLeaf);
        }
        else if (auto pLeftLeaf = pLeaf->m_pLeft; pLeftLeaf && !pLeftLeaf->IsSplittable())
        {
            roomNodes.push_back(pLeftLeaf);
        }

        if (auto pRightLeaf = pLeaf->m_pRight; pRightLeaf && pRightLeaf->IsSplittable())
        {
            leafNodes.push_back(pRightLeaf);
        }
        else if (auto pRightLeaf = pLeaf->m_pRight; pRightLeaf && !pRightLeaf->IsSplittable())
        {
            roomNodes.push_back(pRightLeaf);
        }
    }

    return roomNodes;
}

BspTreeNode* BspTreeNode::GetSibling() const
{
    if (!m_pParent)
    {
        return nullptr;
    }

    return (m_pParent->m_pLeft == this ? m_pParent->m_pRight : m_pParent->m_pLeft);
}

void BspTreeNode::GetNodesByDepthHelper(std::vector<std::unordered_set<BspTreeNode*>>& out, size_t depth)
{
    if (out.size() < depth + 1)
    {
        out.emplace_back();
    }

    if (m_pLeft)
    {
        out[depth].insert(m_pLeft);
        m_pLeft->GetNodesByDepthHelper(out, depth + 1);
    }

    if (m_pRight)
    {
        out[depth].insert(m_pRight);
        m_pRight->GetNodesByDepthHelper(out, depth + 1);
    }
}
