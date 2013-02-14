#include "FXSkewSurfaceBuilder.h"

using namespace utilities;
typedef AbstractBuilder super;

void FXSkewSurfaceBuilder::init(Configuration* cfg){
	super::init(cfg);
}

FXSkewSurface* FXSkewSurfaceBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
}