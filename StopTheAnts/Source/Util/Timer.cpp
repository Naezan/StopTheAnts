#include "../pch.h"
#include "Timer.h"

float Timer::DeltaTime = 0.f;

Timer::Timer()
{
    QueryPerformanceCounter(&start_counter);

    QueryPerformanceFrequency(&frequency);
}

Timer::~Timer()
{
}

void Timer::UpdateTimer()
{
    QueryPerformanceCounter(&end_counter);

    float seconds_per_frame = get_seconds_per_frame(start_counter,
        end_counter);

    if (seconds_per_frame < target_seconds_per_frame)
    {
        DWORD sleep_ms;

        sleep_ms = (DWORD)(1000 * (target_seconds_per_frame - seconds_per_frame));

        Sleep(sleep_ms);

        while (seconds_per_frame < target_seconds_per_frame)
        {
            QueryPerformanceCounter(&end_counter);

            seconds_per_frame = get_seconds_per_frame(start_counter,
                end_counter);
        }
    }

    QueryPerformanceCounter(&end_counter);

    seconds_per_frame = get_seconds_per_frame(start_counter,
        end_counter);

    start_counter = end_counter;

    DeltaTime = seconds_per_frame;
}

void Timer::RenderTimer(HWND _hwnd, WCHAR(&title)[100])
{
    std::wstring fpstitle = std::wstring(title) + L" [" + std::to_wstring(1 / DeltaTime) + L"ms]";
    SetWindowText(_hwnd, fpstitle.c_str());
}
