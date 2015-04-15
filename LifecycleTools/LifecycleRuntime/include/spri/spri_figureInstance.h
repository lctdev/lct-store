#ifndef SPRI_FIGURE_INSTANCE_H
#define SPRI_FIGURE_INSTANCE_H

#include <foun/foun_primitives.h>
#include <foun/foun_vector.h>
#include <foun/foun_matrix.h>
#include <foun/foun_color.h>
#include <foun/foun_keys.h>
#include <foun/foun_debug.h>

#include <spri/spri_constants.h>

namespace lct
{
namespace imag
{
struct TextureTableData;
struct TextureResource;
}

namespace spri
{

struct FigureAsset;
struct SkinAsset;
struct PartData;
struct JointData;
struct PatternData;
struct FigureData;

struct FigureSkinBinding;

class FigureInstance
{
public:
	FigureInstance();

	void BindSkinAsset(SkinAsset& skinAsset, FigureSkinBinding& figureSkinBinding);
	void ResetProperties();
	void CalculateTextures();
	void CalculateTransforms();

	u32 GetJointCount() { return m_jointCount; }
	const foun::Matrix32& GetJointTransform(u32 index);

protected:
	FigureAsset* m_pFigureAsset;

	struct Part
	{
		const PartData* pPartData;

		f32 propertyArray[PART_PROPERTY_TYPE_COUNT];

		foun::Vector4 placement;
	};
	Part* m_pPartArray;
	u32 m_partCount;

	struct Joint
	{
		const JointData* pJointData;

		f32 propertyArray[JOINT_PROPERTY_TYPE_COUNT];

		foun::Matrix32 localTransform;
		foun::Matrix32 childTransform;
		foun::Matrix32 transform;
	};
	Joint* m_pJointArray;
	u32 m_jointCount;

	struct Pattern
	{
		const PatternData* pPatternData;
		const imag::TextureTableData* pTextureTableData;

		f32 propertyArray[PATTERN_PROPERTY_TYPE_COUNT];
		imag::TextureResource* pTextureResourceArray;

		imag::TextureResource* pTextureResource;
	};
	Pattern* m_pPatternArray;
	u32 m_patternCount;

	friend class InstanceBuilder;
	friend class AnimationInstance;
	friend class DrawContext;
};

inline const foun::Matrix32& FigureInstance::GetJointTransform(u32 index)
{
	LCT_ASSERT(index < m_jointCount);
	return m_pJointArray[index].transform;
}

//namespace spri
}
//namespace lct
}

#endif//SPRI_FIGURE_INSTANCE_H
