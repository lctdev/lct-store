#include <soun/soun_clipInstance.h>
#include <soun/soun_assets.h>
#include <soun/soun_data.h>

#include <audi/audi_resources.h>

#include <string.h>

namespace lct
{
namespace soun
{

/*
* Internal Constants
*/

/*
* Public Instance
*/
ClipInstance::ClipInstance()
: m_pClipAsset(NULL)
, m_propertyArray()
{
}

void ClipInstance::BindClipAsset(const ClipAsset* pClipAsset)
{
	m_pClipAsset = pClipAsset;
}

void ClipInstance::ResetProperties()
{ 
	const ClipData* pClipData = m_pClipAsset->pClipData;

	memcpy(m_propertyArray, pClipData->propertyArray, sizeof(m_propertyArray));
}

//namespace soun
}
//namespace lct
}
