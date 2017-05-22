#ifndef _AVALANCHE_POSITION_PROVIDER_
#define _AVALANCHE_POSITION_PROVIDER_

#include <mclib/common/Types.h>
#include <mclib/common/Vector.h>
#include <random>

#undef min
#undef max

namespace avalanche {

class PositionProvider {
public:
    virtual mc::Vector3i NextPosition() = 0;
};

class IncrementalPositionProvider : public PositionProvider {
private:
    mc::Vector3i m_Position;
    mc::Vector3i m_Increment;

public:
    IncrementalPositionProvider(mc::Vector3i initialPos, s32 xIncrement, s32 zIncrement)
        : m_Position(initialPos),
        m_Increment(xIncrement, 0, zIncrement)
    {

    }

    mc::Vector3i NextPosition() override {
        m_Position += m_Increment;
        return m_Position;
    }
};

class RandomPositionProvider : public PositionProvider {
private:
    std::mt19937 m_Engine;

public:
    RandomPositionProvider()
        : m_Engine(std::random_device()())
    {

    }

    mc::Vector3i NextPosition() override {
        std::uniform_int_distribution<s32> dist(
            std::numeric_limits<s32>::min(),
            std::numeric_limits<s32>::max()
        );

        return mc::Vector3i(dist(m_Engine), 64, dist(m_Engine));
    }
};

} // ns avalanche

#endif
