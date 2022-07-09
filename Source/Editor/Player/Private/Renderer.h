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
	void Shutdown();
	Bool Tick();

	void UpdateWindowPos();

	inline Bool IsUpdateFPS() const { return mbUpdateFPS; }
	inline Float GetFPS() const { return mFPS; }

private:
	void update(const Float deltaTime);
	void draw() const;

	Vector2 toScreenPos(const Vector2& pos) const;

	void drawGrid() const;
	void drawLine() const;
	void drawPlayer() const;
	
private:
	static constexpr Uint32 FRAME = 60;
	static constexpr Float TICKS_PER_FRAME = 1000.0f / (Float)FRAME;

	uint32_t mFrameCount = 0;
	Float mFPS = 0.0f;
	Bool mbUpdateFPS = false;

private:
	HWND mhWnd = nullptr;
	DDraw* mDDraw = nullptr;

	Vector2 mPlayerPos = {};
	Float mPlayerSpeed = 100.0f;
};