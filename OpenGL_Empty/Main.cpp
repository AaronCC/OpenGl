#include "Engine.h"
int main()
{
	Engine glEngine;


	if (!glEngine.init())
		return -1;

	if (!glEngine.bufferModel())
		return -1;

	if (!glEngine.useShaders())
		return -1;

	if (!glEngine.gameLoop())
		return -1;

	return 0;
}
