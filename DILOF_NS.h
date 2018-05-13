#pragma once

#include "DILOF.h"

class DILOF_NS : public DILOF {
public:
	DILOF_NS(LOFConfiguration& conf);
	void insertion(Instance* instance);
};

DILOF_NS::DILOF_NS(LOFConfiguration& conf): DILOF(conf) { }

void DILOF_NS::insertion(Instance* instance) {
	IncrementalLOF::insertion(instance, false);

	if (dataset.getNumInstances() == W) {
		summarize();
	}
}