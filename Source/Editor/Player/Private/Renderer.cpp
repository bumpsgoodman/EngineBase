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
		Float deltaTime = tickTimer.GetDeltaTime();
		update(deltaTime);
	}

	draw();

	++mFrameCount;
	if (fpsTimer.IsOnTick())
	{
		mFPS = (Float)mFrameCount;
		mFrameCount = 0;
		mbUpdateFPS = true;
	}
	else
	{
		mbUpdateFPS = false;
	}

	return true;
}

void Renderer::UpdateWindowPos()
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	mDDraw->UpdateWindowPos();
}

void Renderer::update(const Float deltaTime)
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
		dir.Y = 1;
	}

	if (event::keyboard::IsKeyPressed(VK_DOWN))
	{
		dir.Y = -1;
	}

	Vector2 pos = mPlayerPos + (dir * (mPlayerSpeed * deltaTime));
	static const Uint32 WINDOW_WIDTH = mDDraw->GetWidth();
	static const Uint32 WINDOW_HEIGHT = mDDraw->GetHeight();

	static const Float MIN_X = -(Float)WINDOW_WIDTH * 0.5f;
	static const Float MIN_Y = -(Float)WINDOW_HEIGHT * 0.5f;
	static const Float MAX_X = (Float)WINDOW_WIDTH * 0.5f;
	static const Float MAX_Y = (Float)WINDOW_HEIGHT * 0.5f;

	if (pos.X >= MIN_X && pos.X <= MAX_X
		&& pos.Y >= MIN_Y && pos.Y <= MAX_Y)
	{
		mPlayerPos = pos;
	}
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
		drawPlayerPos();
	}
	mDDraw->EndDraw();

	mDDraw->OnDraw();
}

Vector2 Renderer::toScreenPos(const Vector2& pos) const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	const Uint32 WINDOW_WIDTH = mDDraw->GetWidth();
	const Uint32 WINDOW_HEIGHT = mDDraw->GetHeight();

	Vector2 screenPos(ROUND(WINDOW_WIDTH * 0.5f + pos.X), ROUND(WINDOW_HEIGHT * 0.5f + -pos.Y));
	return screenPos;
}

void Renderer::drawGrid() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	static const Uint32 WINDOW_WIDTH = mDDraw->GetWidth();
	static const Uint32 WINDOW_HEIGHT = mDDraw->GetHeight();

	static const Float LINE_DISTANCE = 20;

	static const Float MIN_X = -(Float)WINDOW_WIDTH * 0.5f;
	static const Float MIN_Y = -(Float)WINDOW_HEIGHT * 0.5f;
	static const Float MAX_X = (Float)WINDOW_WIDTH * 0.5f;
	static const Float MAX_Y = (Float)WINDOW_HEIGHT * 0.5f;

	static const Vector2 START_X_AXIS_POS = toScreenPos({ MIN_X, 0.0f });
	static const Vector2 END_X_AXIS_POS = toScreenPos({ MAX_X, 0.0f });
	static const Vector2 START_Y_AXIS_POS = toScreenPos({ 0.0f, MIN_Y });
	static const Vector2 END_Y_AXIS_POS = toScreenPos({ 0.0f, MAX_Y });

	Vector2 verticalLinePos(MIN_X, MIN_Y);
	Vector2 horizontalLinePos(MIN_X, MIN_Y);

	for (; verticalLinePos.X <= MAX_X; verticalLinePos.X += LINE_DISTANCE)
	{
		Vector2 startScreenPos = toScreenPos(verticalLinePos);
		Vector2 endScreenPos = toScreenPos({ verticalLinePos.X, MAX_Y });
		mDDraw->DrawLineBresenham(startScreenPos.X, startScreenPos.Y, endScreenPos.X, endScreenPos.Y, Color::ToARGBHex(colors::LIGHT_GRAY));
	}

	for (; horizontalLinePos.Y <= MAX_Y; horizontalLinePos.Y += LINE_DISTANCE)
	{
		Vector2 startScreenPos = toScreenPos(horizontalLinePos);
		Vector2 endScreenPos = toScreenPos({ MAX_X, horizontalLinePos.Y });
		mDDraw->DrawLineBresenham(startScreenPos.X, startScreenPos.Y, endScreenPos.X, endScreenPos.Y, Color::ToARGBHex(colors::LIGHT_GRAY));
	}

	mDDraw->DrawLineBresenham(START_X_AXIS_POS.X, START_X_AXIS_POS.Y, END_X_AXIS_POS.X, END_X_AXIS_POS.Y, Color::ToARGBHex(colors::RED));
	mDDraw->DrawLineBresenham(START_Y_AXIS_POS.X, START_Y_AXIS_POS.Y, END_Y_AXIS_POS.X, END_Y_AXIS_POS.Y, Color::ToARGBHex(colors::RED));
}

void Renderer::drawLine() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	static Float lineLength = 10000.0f;
	Vector2 startPos = mPlayerPos * lineLength;
	Vector2 endPos = mPlayerPos * -lineLength;

	Vector2 startScreenPos = toScreenPos(startPos);
	Vector2 endScreenPos = toScreenPos(endPos);
	if (mDDraw->ClipLineCoham(&startScreenPos, &endScreenPos, { 0, 0 }, { mDDraw->GetWidth(), mDDraw->GetHeight() }))
	{
		mDDraw->DrawLineBresenham((Int32)startScreenPos.X, (Int32)startScreenPos.Y, (Int32)endScreenPos.X, (Int32)endScreenPos.Y, Color::ToARGBHex(colors::GREEN));
	}
}

void Renderer::drawPlayer() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	Vector2 screenPos = toScreenPos(mPlayerPos);
	mDDraw->DrawCircle((Int32)screenPos.X, (Int32)screenPos.Y, 5, Color::ToARGBHex(colors::BLUE));
}

void Renderer::drawPlayerPos() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	HDC hdc;
	mDDraw->BeginGDI(&hdc);
	{
		WChar playerPosStr[128];
		swprintf(playerPosStr, L"(%.2f, %.2f)", mPlayerPos.X, mPlayerPos.Y);
		mDDraw->PrintText(hdc, playerPosStr, 0, 0, (Uint32)wcslen(playerPosStr), Color::ToARGBHex(colors::RED));
	}
	mDDraw->EndGDI(hdc);
}