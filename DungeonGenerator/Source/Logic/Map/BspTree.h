#pragma once
#include <Utils/Vector2.h>
#include <Utils/Rectangle.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace yang
{
    class XorshiftRNG;
}

class BspTreeNode
{
public:
    BspTreeNode* m_pParent;
    BspTreeNode* m_pLeft;
    BspTreeNode* m_pRight;

    yang::IRect m_areaRect;
    yang::XorshiftRNG* m_pRng;

    yang::IVec2 m_roomHeightRange;
    yang::IVec2 m_roomWidthRange;

public:

    BspTreeNode(BspTreeNode* pParent, yang::IRect areaRect, yang::XorshiftRNG* pRng, yang::IVec2 roomHeightRange, yang::IVec2 roomWidthRange);
    BspTreeNode(yang::IRect areaRect, yang::XorshiftRNG* pRng, yang::IVec2 roomHeightRange, yang::IVec2 roomWidthRange);
    ~BspTreeNode();

    void Split();

    bool IsSplittableHorizontally() const;

    bool IsSplittableVertically() const;

    bool IsSplittable() const;

    bool WasSplitHorizontally() const;

    bool WasSplitVertically() const;

    std::pair<yang::IVec2, yang::IVec2> GenerateDoorTileIndices() const;

    template<class F>
    void ForEach(F&& f);

    std::vector<std::unordered_set<BspTreeNode*>> GetNodesByDepth();

    static std::vector<BspTreeNode*> Split(BspTreeNode* pRoot, int desiredRoomCount);

    BspTreeNode* GetSibling() const;

private:
    void GetNodesByDepthHelper(std::vector<std::unordered_set<BspTreeNode*>>& out, size_t depth);
};

template<class F>
inline void BspTreeNode::ForEach(F&& f)
{
    if (m_pLeft)
    {
        f(m_pLeft);
        m_pLeft->ForEach(std::forward<F>(f));
    }
    if (m_pRight)
    {
        f(m_pRight);
        m_pRight->ForEach(std::forward<F>(f));
    }
}