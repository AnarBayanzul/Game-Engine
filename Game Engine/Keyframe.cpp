#include "Keyframe.h"
#include "iostream"

Keyframe::Keyframe() {
	timeStamp = 0.0;
}

Keyframe::Keyframe(float timeIn, std::vector<int> boneID, std::vector<quat> quatIn) {
	timeStamp = timeIn;
	if (boneID.size() != quatIn.size()) {
		std::cout << "bone ID size does not match quat size\n";
	}
	for (int i = 0; i < boneID.size(); ++i) {
		boneTransform.push_back(
			std::make_pair(
				boneID[i],
				quatIn[i]
			)
		);
	}
}

Keyframe::~Keyframe() {

}