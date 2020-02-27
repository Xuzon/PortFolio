#pragma once

#include <thread>
#include <atomic>
#include "Queue.h"
#include "CommandBuffer.h"
class RenderThread
{

public:

	void Start();
	void Stop();
	int PendingFrames() { return pendingFrames; }
	void EnqueueAction(const CommandBuffer& command);

private:
	void RenderLoop();
	void DrawBox(int x, int y, int sizeX, int sizeY, int color);

	ConcurrentQueue<CommandBuffer> commandQueue;
	std::thread* thread = nullptr;
	std::atomic_bool quit = false;
	int pendingFrames = 0;
};

