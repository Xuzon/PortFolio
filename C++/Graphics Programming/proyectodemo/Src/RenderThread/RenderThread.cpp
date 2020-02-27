#include "RenderThread.h"
#include "../Renderer/Renderer.h"

void RenderThread::Start()
{
	if (thread == nullptr)
	{
		thread = new std::thread(&RenderThread::RenderLoop, this);
	}
}

void RenderThread::Stop()
{
	if (thread != nullptr)
	{
		quit = true;
		EnqueueAction(CommandBuffer{ END_FRAME ,0,0,0});
		thread->join();
		delete(thread);
		thread = nullptr;
	}
}

void RenderThread::EnqueueAction(const CommandBuffer& command)
{
	if (command.type == END_FRAME)
	{
		pendingFrames++;
	}
	commandQueue.push(command);
}

#include <iostream>
void RenderThread::RenderLoop()
{
	while (!quit || pendingFrames > 0)
	{
		CommandBuffer command = commandQueue.pop();
		switch (command.type)
		{
		case START_FRAME:
			Renderer::Clear(0x00000000);
			break;
		case END_FRAME:
			Renderer::Present();
			pendingFrames--;
			break;
		case DRAW_BOX:
			DrawBox(command.x, command.y, command.sizeX, command.sizeY, command.color);
			break;
		case DRAW_BOXES:
			for (auto point : command.boxes)
			{
				DrawBox(point.x, point.y, command.sizeX, command.sizeY, command.color);
			}
			break;
		default:
			Renderer::PutPixel(command.x, command.y, command.color);
			break;
		}
	}
}

void RenderThread::DrawBox(int startX, int startY, int sizeX, int sizeY, int color)
{
	for (int y = startY; y < startY + sizeY; ++y)
	{
		for (int x = startX; x < startX + sizeX; ++x)
		{
			Renderer::PutPixel(x, y, color);
		}
	}
}