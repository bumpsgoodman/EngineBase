#include "PlayerPCH.h"
#include "Renderer.h"

Renderer::~Renderer()
{
	Shutdown();
}

Bool Renderer::Initialize(const HWND hWnd, const Uint32 width, const Uint32 height)
{
	AssertW(hWnd != nullptr, L"hWnd is nullptr");

	mhWnd = hWnd;
	mWidth = width;
	mHeight = height;

	mDDraw = new DDraw();
	AssertW(mDDraw != nullptr, L"Failed to allocate DDraw object");

	if (!mDDraw->Initialize(hWnd))
	{
		AssertW(false, L"Failed to initialize DDraw object");
		goto FAILED;
	}

	return true;

FAILED:
	Shutdown();
	return false;
}

void Renderer::Shutdown()
{
	SAFE_DELETE(mDDraw);
}

Bool Renderer::Tick()
{
	static Timer tickTimer(TICKS_PER_FRAME);
	static Timer fpsTimer(1000.0f);

	tickTimer.Update();
	fpsTimer.Update();

	if (tickTimer.IsOnTick())
	{
		update();
		draw();
	}

	if (fpsTimer.IsOnTick())
	{
		mFPS = (Float)mFrameCount / 1000.0f;
		mFrameCount = 0;
		mbUpdateFPS = true;
	}
	else
	{
		++mFrameCount;
		mbUpdateFPS = false;
	}

	return true;
}

void Renderer::UpdateWindowPos()
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	mDDraw->UpdateWindowPos();
}

void Renderer::update()
{

}

void Renderer::draw() const
{
	mDDraw->BeginDraw();
	{
		mDDraw->Clear(0x000000);
	}
	mDDraw->EndDraw();

	mDDraw->Blt();
}