#ifndef FONT_ARRAYS_H
#define FONT_ARRAYS_H

#include <font/font_constants.h>
#include <font/font_data.h>

namespace lct
{
	namespace grap
	{
		struct AttributeData;
		struct UniformData;
	}

	namespace font
	{

		const u32 ATTRIBUTE_COUNT = 3;
		extern grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT];

		extern grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT];

		//namespace font
	}
	//namespace lct
}

#endif//FONT_ARRAYS_H
