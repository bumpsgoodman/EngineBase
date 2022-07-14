#pragma once

class Renderer final
{
public:
	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = default;
	Renderer& operator=(Renderer&&) = default;
	~Renderer();

	Bool Initialize(const HWND hWnd);
	void Release();
	Bool Tick();

	void UpdateWindowPos();

	inline Bool IsUpdateFPS() const { return mbUpdateFPS; }
	inline Uint32 GetFPS() const { return mFPS; }

private:
	void update(const Float deltaTime);
	void draw();

	IntVector2 toScreenPos(const Vector2& pos) const;

	void drawGrid() const;
	void drawLine() const;
	void drawPlayer() const;
	void drawPlayerPos() const;
	
private:
	// FPS
	static constexpr Uint32 FRAME = 144;
	static constexpr Float TICKS_PER_FRAME = 1000.0f / (Float)FRAME;

	Uint32 mFrameCount = 0;
	Uint32 mFPS = 0;
	Bool mbUpdateFPS = false;

	// DDraw
	HWND mhWnd = nullptr;
	DDraw* mDDraw = nullptr;

	// User defines
	Vector2 mPlayerPos = {};
	Float mPlayerSpeed = 100.0f;
};