#include "Animation.h"

Animation::Animation(int keyIn, int boneIn, const std::vector<Keyframe>& data) {
	keyCount = keyIn;
	boneCount = boneIn;
	animData = data;
}

Animation::~Animation() {

}