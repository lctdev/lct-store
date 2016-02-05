#ifndef FOUN_RANDOM_GENERATOR_H
#define FOUN_RANDOM_GENERATOR_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

class RandomGenerator
{
public:
	RandomGenerator();

	void SetSeed(u32 seed);

	f32 GenerateFloat();
	f32 GenerateFloat(f32 min, f32 max);

private:
	u32 m_seed;
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_RANDOM_GENERATOR_H
