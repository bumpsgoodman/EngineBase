#include "PlayerPCH.h"
#include "Renderer.h"

Renderer::~Renderer()
{
	Shutdown();
}

Bool Renderer::Initialize(const HWND hWnd)
{
	AssertW(hWnd != nullptr, L"hWnd is nullptr");

	mhWnd = hWnd;

	mDDraw = new DDraw();
	AssertW(mDDraw != nullptr, L"Failed to allocate DDraw object");

	if (!mDDraw->Initialize(hWnd))
	{
		AssertW(false, L"Failed to initialize DDraw object");
		goto FAILED;
	}

	//mPlayerPos.X = (Float)(mDDraw->GetWidth() / 2 - 1);
	//mPlayerPos.Y = (Float)(mDDraw->GetHeight() / 2 - 1);

	mPlayerPos.X = 100.0f;
	mPlayerPos.Y = 100.0f;

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
	Vector2 dir = {};

	if (event::keyboard::IsKeyPressed(VK_LEFT))
	{
		dir.X = -1;
	}

	if (event::keyboard::IsKeyPressed(VK_RIGHT))
	{
		dir.X = 1;
	}

	if (event::keyboard::IsKeyPressed(VK_UP))
	{
		dir.Y = -1;
	}

	if (event::keyboard::IsKeyPressed(VK_DOWN))
	{
		dir.Y = 1;
	}

	mPlayerPos = mPlayerPos + (dir * mPlayerSpeed);
}

void Renderer::draw() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	mDDraw->BeginDraw();
	{
		mDDraw->Clear(Color::ToARGBHex(colors::WHITE));

		drawGrid();
		drawLine();
		drawPlayer();
	}
	mDDraw->EndDraw();

	mDDraw->Blt();
}

void Renderer::drawGrid() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	static const Uint32 WINDOW_WIDTH = mDDraw->GetWidth();
	static const Uint32 WINDOW_HEIGHT = mDDraw->GetHeight();

	Vector2 originPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	const Uint32 ROW_BOUNDARY = 100;
	const Uint32 COL_BOUNDARY = 100;
	const Uint32 LEVEL_ROW_BOUDNARY = ROW_BOUNDARY / 5;
	const Uint32 LEVEL_COL_BOUDNARY = COL_BOUNDARY / 5;

	for (Uint32 i = 0; i < WINDOW_WIDTH / LEVEL_COL_BOUDNARY; ++i)
	{
		Uint32 col = i * LEVEL_COL_BOUDNARY;

		mDDraw->DrawLineBresenham(col - 1, 0, col - 1, WINDOW_HEIGHT - 1, Color::ToARGBHex(colors::LIGHT_GRAY));
	}

	for (Uint32 i = 0; i < WINDOW_HEIGHT / LEVEL_ROW_BOUDNARY; ++i)
	{
		Uint32 row = i * LEVEL_ROW_BOUDNARY;

		mDDraw->DrawLineBresenham(0, row - 1, WINDOW_WIDTH - 1, row - 1, Color::ToARGBHex(colors::LIGHT_GRAY));
	}

	for (Uint32 i = 0; i < WINDOW_WIDTH / COL_BOUNDARY; ++i)
	{
		Uint32 col = i * COL_BOUNDARY;

		mDDraw->DrawLineBresenham(col - 1, 0, col - 1, WINDOW_HEIGHT - 1, Color::ToARGBHex(colors::BLACK));
	}

	for (Uint32 i = 0; i < WINDOW_HEIGHT / ROW_BOUNDARY; ++i)
	{
		Uint32 row = i * ROW_BOUNDARY;

		mDDraw->DrawLineBresenham(0, row - 1, WINDOW_WIDTH - 1, row - 1, Color::ToARGBHex(colors::BLACK));
	}

	mDDraw->DrawLineBresenham(0, (Int32)originPos.Y - 1, WINDOW_WIDTH - 1, (Int32)originPos.Y - 1, Color::ToARGBHex(colors::RED));
	mDDraw->DrawLineBresenham((Int32)originPos.X - 1, 0, (Int32)originPos.X - 1, WINDOW_HEIGHT - 1, Color::ToARGBHex(colors::RED));
}

void Renderer::drawLine() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	static Float lineLength = 100.0f;
	Vector2 startPos = mPlayerPos * lineLength;
	Vector2 endPos = mPlayerPos * -lineLength;

	mDDraw->DrawLineBresenham(startPos.X, startPos.Y, endPos.X, endPos.Y, Color::ToARGBHex(colors::GREEN));
}

void Renderer::drawPlayer() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	mDDraw->DrawCircle((Int32)mPlayerPos.X, (Int32)mPlayerPos.Y, 5, Color::ToARGBHex(colors::BLUE));
}