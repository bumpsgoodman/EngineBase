#include "PlayerPCH.h"

#include <vector>

#include "Renderer.h"

Renderer::~Renderer()
{
	Release();
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
	Release();
	return false;
}

void Renderer::Release()
{
	SAFE_DELETE(mDDraw);
}

Bool Renderer::Tick()
{
	static Timer gameFrameTimer(TICKS_PER_FRAME);
	
	gameFrameTimer.Update();
	if (gameFrameTimer.IsOnTick())
	{
		Float deltaTime = gameFrameTimer.GetDeltaTime();
		update(deltaTime);
	}

	draw();

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

	Vector2 pos = mPlayerPos + dir * (mPlayerSpeed * deltaTime);
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

void Renderer::draw()
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

	static Timer fpsTimer(1000.0f);
	fpsTimer.Update();

	++mFrameCount;
	if (fpsTimer.IsOnTick())
	{
		mFPS = mFrameCount;
		mFrameCount = 0;
		mbUpdateFPS = true;
	}
	else
	{
		mbUpdateFPS = false;
	}
}

IntVector2 Renderer::toScreenPos(const Vector2& pos) const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	static const Uint32 WINDOW_WIDTH = mDDraw->GetWidth();
	static const Uint32 WINDOW_HEIGHT = mDDraw->GetHeight();

	IntVector2 screenPos(ROUND(WINDOW_WIDTH * 0.5f + pos.X), ROUND(WINDOW_HEIGHT * 0.5f + -pos.Y));
	return screenPos;

	//return IntVector2(ROUND(WINDOW_WIDTH * 0.5f + pos.X), ROUND(WINDOW_HEIGHT * 0.5f + -pos.Y));
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

	static const IntVector2 START_X_AXIS_POS = toScreenPos({ MIN_X, 0.0f });
	static const IntVector2 END_X_AXIS_POS = toScreenPos({ MAX_X, 0.0f });
	static const IntVector2 START_Y_AXIS_POS = toScreenPos({ 0.0f, MIN_Y });
	static const IntVector2 END_Y_AXIS_POS = toScreenPos({ 0.0f, MAX_Y });

	Vector2 verticalLinePos(MIN_X, MIN_Y);
	Vector2 horizontalLinePos(MIN_X, MIN_Y);

	for (; verticalLinePos.X <= MAX_X; verticalLinePos.X += LINE_DISTANCE)
	{
		IntVector2 startScreenPos = toScreenPos(verticalLinePos);
		IntVector2 endScreenPos = toScreenPos({ verticalLinePos.X, MAX_Y });
		mDDraw->DrawLineBresenham(startScreenPos, endScreenPos, Color::ToARGBHex(colors::LIGHT_GRAY));
	}

	for (; horizontalLinePos.Y <= MAX_Y; horizontalLinePos.Y += LINE_DISTANCE)
	{
		IntVector2 startScreenPos = toScreenPos(horizontalLinePos);
		IntVector2 endScreenPos = toScreenPos({ MAX_X, horizontalLinePos.Y });
		mDDraw->DrawLineBresenham(startScreenPos, endScreenPos, Color::ToARGBHex(colors::LIGHT_GRAY));
	}

	mDDraw->DrawLineBresenham(START_X_AXIS_POS, END_X_AXIS_POS, Color::ToARGBHex(colors::RED));
	mDDraw->DrawLineBresenham(START_Y_AXIS_POS, END_Y_AXIS_POS, Color::ToARGBHex(colors::RED));
}

void Renderer::drawLine() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	static Float lineLength = 5000.0f;
	Vector2 startPos = mPlayerPos * lineLength;
	Vector2 endPos = mPlayerPos * -lineLength;

	IntVector2 startScreenPos = toScreenPos(startPos);
	IntVector2 endScreenPos = toScreenPos(endPos);
	if (mDDraw->ClipLineCoham(&startScreenPos, &endScreenPos, { 0, 0 }, { mDDraw->GetWidth(), mDDraw->GetHeight() }))
	{
		mDDraw->DrawLineBresenham(startScreenPos, endScreenPos, Color::ToARGBHex(colors::GREEN));
	}
}

void Renderer::drawPlayer() const
{
	AssertW(mDDraw != nullptr, L"DDraw object is nullptr");

	IntVector2 screenPos = toScreenPos(mPlayerPos);
	//mDDraw->DrawCircle(screenPos, 5, Color::ToARGBHex(colors::BLUE));

	static const Int32 RADIUS = 10;
	static const Uint32 SQUARED_RADIUS = RADIUS * RADIUS;

	static std::vector<IntVector2> posVect;
	if (posVect.empty())
	{
		for (Int32 i = -RADIUS; i <= RADIUS; ++i)
		{
			for (Int32 j = -RADIUS; j <= RADIUS; ++j)
			{
				Uint32 squared = i * i + j * j;
				if (squared <= SQUARED_RADIUS)
				{
					posVect.push_back({ i, j });
				}
			}
		}
	}

	for (const IntVector2& pos : posVect)
	{
		mDDraw->DrawPixel(screenPos + pos, Color::ToARGBHex(colors::BLUE));
	}
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