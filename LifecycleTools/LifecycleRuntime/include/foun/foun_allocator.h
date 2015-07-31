#ifndef FOUN_ALLOCATOR_H
#define FOUN_ALLOCATOR_H

#include <foun/foun_primitives.h>

#include <new>

namespace lct
{
namespace foun
{

class Allocator
{
public:
	Allocator();
	virtual ~Allocator();

	virtual void* Alloc(u32 size, u32 align);
	virtual void Free(void* pMemory);

	template <class T>
	T* AllocType()
	{
		u32 size = sizeof(T);
		void* pMemory = Alloc(size, 4);
		if (pMemory != NULL)
		{
			T* pType = new (pMemory) T();
			return pType;
		}
		else
		{
			return NULL;
		}
	}

	template <class T>
	T* AllocTypeArray(u32 count)
	{
		if (count > 0)
		{
			u32 size = sizeof(T) * count;
			void* pMemory = Alloc(size, 4);
			if (pMemory != NULL)
			{
				T* pTypes = reinterpret_cast<T*>(pMemory);
				for (u32 index = 0; index < count; ++index)
				{
					T* pType = pTypes + index;
					pType = new (pType) T();
				}
				return pTypes;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_ALLOCATOR_H
