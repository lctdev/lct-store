#include <spri/spri_figureInstance.h>
#include <spri/spri_assets.h>
#include <spri/spri_data.h>
#include <spri/spri_bindings.h>

#include <grap/grap_resources.h>

#include <imag/imag_data.h>
#include <imag/imag_assets.h>

#include <string.h>

namespace lct
{
namespace spri
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
FigureInstance::FigureInstance()
: m_pFigureAsset(NULL)
, m_pPartArray(NULL)
, m_partCount(0)
, m_pJointArray(NULL)
, m_jointCount(0)
, m_pPatternArray(NULL)
, m_patternCount()
{
}

void FigureInstance::BindSkinAsset(SkinAsset& skinAsset, FigureSkinBinding& figureSkinBinding)
{
	SkinAsset* pSkinAsset = &skinAsset;
	FigureAsset* pFigureAsset = m_pFigureAsset;

	if ((figureSkinBinding.pSkinAsset != pSkinAsset) || (figureSkinBinding.pFigureAsset != pFigureAsset))
	{
		return;
	}

	SkinData* pSkinData = skinAsset.pSkinData;

	for (u32 stripIndex = 0; stripIndex < pSkinData->stripCount; ++stripIndex)
	{
		s32 patternIndex = figureSkinBinding.pPatternIndexArray[stripIndex];
		if (patternIndex >= 0)
		{
			const SkinAsset::Strip& assetStrip = skinAsset.pStripArray[stripIndex];
			const imag::TextureTableAsset* pTextureTableAsset = assetStrip.pTextureTableAsset;
			Pattern& pattern = m_pPatternArray[patternIndex];
			pattern.pTextureTableData = pTextureTableAsset->pTextureTableData;
			pattern.pTextureResourceArray = pTextureTableAsset->pTextureResourceArray;
		}
	}
}

void FigureInstance::ResetProperties()
{
	for (u32 partIndex = 0; partIndex < m_partCount; ++partIndex)
	{
		Part& part = m_pPartArray[partIndex];
		const PartData* pPartData = part.pPartData;

		memcpy(&part.placement, &pPartData->placement, sizeof(part.placement));
		memcpy(part.propertyArray, pPartData->propertyArray, sizeof(part.propertyArray));
	}

	for (u32 jointIndex = 0; jointIndex < m_jointCount; ++jointIndex)
	{
		Joint& joint = m_pJointArray[jointIndex];
		const JointData* pJointData = joint.pJointData;

		memcpy(joint.propertyArray, pJointData->propertyArray, sizeof(joint.propertyArray));
	}

	for (u32 patternIndex = 0; patternIndex < m_patternCount; ++patternIndex)
	{
		Pattern& pattern = m_pPatternArray[patternIndex];
		const PatternData* pPatternData = pattern.pPatternData;

		memcpy(pattern.propertyArray, pPatternData->propertyArray, sizeof(pattern.propertyArray));
	}
}

void FigureInstance::CalculateTextures()
{
	for (u32 patternIndex = 0; patternIndex < m_patternCount; ++patternIndex)
	{
		Pattern& pattern = m_pPatternArray[patternIndex];
		const imag::TextureTableData* pTextureTableData = pattern.pTextureTableData;

		f32 propertyTextureIndex = pattern.propertyArray[PATTERN_PROPERTY_TYPE_TEXTURE_INDEX];
		if (propertyTextureIndex >= 0.0f)
		{
			u32 textureIndex = (u32)propertyTextureIndex;
			if (textureIndex < pTextureTableData->count)
			{
				pattern.pTextureResource = pattern.pTextureResourceArray + textureIndex;
			}
		}
	}
}

void FigureInstance::CalculateTransforms()
{
	for (u32 jointIndex = 0; jointIndex < m_jointCount; ++jointIndex)
	{
		Joint& joint = m_pJointArray[jointIndex];
		const JointData* pJointData = joint.pJointData;

		const f32& scaleX = joint.propertyArray[JOINT_PROPERTY_TYPE_SCALE_X];
		const f32& scaleY = joint.propertyArray[JOINT_PROPERTY_TYPE_SCALE_Y];
		const f32& rotations = joint.propertyArray[JOINT_PROPERTY_TYPE_ROTATE];
		const f32& translateX = joint.propertyArray[JOINT_PROPERTY_TYPE_TRANSLATE_X];
		const f32& translateY = joint.propertyArray[JOINT_PROPERTY_TYPE_TRANSLATE_Y];

		bool scaleChildren = pJointData->flags & (1 << JOINT_FLAG_TYPE_SCALE_CHILDREN);
		if (scaleChildren)
		{
			// children will be transformed by our full transform
			foun::Matrix32ScaleRotateTranslate(joint.childTransform, scaleX, scaleY, rotations, translateX, translateY);

			// same transform can be used used for our local transform
			memcpy(&joint.localTransform, &joint.childTransform, sizeof(joint.localTransform));
		}
		else
		{
			// children will only directly be transformed by rotation and translation
			foun::Matrix32RotateTranslate(joint.childTransform, rotations, translateX, translateY);

			// multiply in scale for our local transform
			memcpy(&joint.localTransform, &joint.childTransform, sizeof(joint.localTransform));
			joint.localTransform.m[0][0] *= scaleX;
			joint.localTransform.m[0][1] *= scaleX;
			joint.localTransform.m[1][0] *= scaleY;
			joint.localTransform.m[1][1] *= scaleY;
		}

		if (pJointData->parentIndex == -1)
		{
			// no parent, our transform is local
			joint.transform = joint.localTransform;
		}
		else
		{
			Joint& parentJoint = m_pJointArray[pJointData->parentIndex];
			const JointData* pParentJointData = parentJoint.pJointData;

			bool parentScaleChildren = pParentJointData->flags & (1 << JOINT_FLAG_TYPE_SCALE_CHILDREN);
			if (!parentScaleChildren)
			{
				const f32& parentScaleX = parentJoint.propertyArray[JOINT_PROPERTY_TYPE_SCALE_X];
				const f32& parentScaleY = parentJoint.propertyArray[JOINT_PROPERTY_TYPE_SCALE_Y];

				// parent is not scaling children, but we still need to mutiply the parent scale into our child translation and local translation
				joint.childTransform.m[2][0] *= parentScaleX;
				joint.childTransform.m[2][1] *= parentScaleY;

				joint.localTransform.m[2][0] *= parentScaleX;
				joint.localTransform.m[2][1] *= parentScaleY;
			}

			// multiply our parent's child transform into our own child transform
			foun::Matrix32 tempTransform;
			foun::Matrix32Multiply(tempTransform, joint.childTransform, parentJoint.childTransform);
			memcpy(&joint.childTransform, &tempTransform, sizeof(joint.childTransform));

			// multiply the parent child transform with our local transform for the final transform
			foun::Matrix32Multiply(joint.transform, joint.localTransform, parentJoint.childTransform);
		}
	}
}

//namespace lct
}
//namespace fram
}
