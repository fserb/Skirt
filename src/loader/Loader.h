#ifndef __LOADER_LOADER_H__
#define __LOADER_LOADER_H__

#include "core/skirt.h"

namespace skirt {

void LoadSceneFile(const string& filename);
void LoadSceneString(const string& data);

}  // namespace skirt

#endif  // __LOADER_LOADER_H__
