#include <foun/foun_randomGenerator.h>

#include <foun/foun_math.h>

/*
* Internal Constants
*/
static const u32 RANDOM_MULT = 8253729;
static const u32 RANDOM_ADD = 2396403;
static const u32 RANDOM_MAX = 32767;

/*
* Internal Helper
*/
inline void RandomizeSeed(u32& seed)
{
	seed = (RANDOM_MULT * seed + RANDOM_ADD);
}

namespace lct
{
namespace foun
{

/*
* Public Instance
*/
RandomGenerator::RandomGenerator()
: m_seed(0)
{
}

void RandomGenerator::SetSeed(u32 seed)
{
	m_seed = seed;
}

f32 RandomGenerator::GenerateFloat()
{
	RandomizeSeed(m_seed);
	return (f32)(m_seed % RANDOM_MAX) / (f32)RANDOM_MAX;
}

f32 RandomGenerator::GenerateFloat(f32 min, f32 max)
{
	RandomizeSeed(m_seed);
	f32 difference = max - min;
	return min + ((f32)(m_seed % RANDOM_MAX) / (f32)RANDOM_MAX * difference);
}

//namespace lct
}
//namespace foun
}
