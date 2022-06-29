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

	Bool Initialize(const HWND hWnd, const Uint32 width, const Uint32 height);
	void Shutdown();
	Bool Tick();

	void UpdateWindowPos();

	inline Bool IsUpdateFPS() const { return mbUpdateFPS; }
	inline Float GetFPS() const { return mFPS; }

private:
	void update();
	void draw() const;
	
private:
	static constexpr Uint32 FRAME = 60;
	static constexpr Float TICKS_PER_FRAME = 1000.0f / (Float)FRAME;

	LARGE_INTEGER mFrequency = {};
	LARGE_INTEGER mPrevCounter = {};
	LARGE_INTEGER mFpsCounter = {};

	uint32_t mFrameCount = 0;
	Float mFPS = 0.0f;
	Bool mbUpdateFPS = false;

private:
	HWND mhWnd = nullptr;
	Uint32 mWidth = 0;
	Uint32 mHeight = 0;

	DDraw* mDDraw = nullptr;
};