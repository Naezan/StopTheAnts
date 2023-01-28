#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	void UpdateTimer();
	void RenderTimer(HWND _hwnd, WCHAR(&title)[100]);

	inline static const float GetDeltaTime() { return DeltaTime; }

private:
	inline float get_seconds_per_frame(LARGE_INTEGER _start_counter,
		LARGE_INTEGER _end_counter)
	{

		return ((float)(_end_counter.QuadPart - _start_counter.QuadPart) / (float)frequency.QuadPart);
	}

private:
	static float DeltaTime;

	float target_seconds_per_frame = 1.0f / 120.0f;

	LARGE_INTEGER frequency;

	LARGE_INTEGER start_counter, end_counter, counts, fps, ms;
};
