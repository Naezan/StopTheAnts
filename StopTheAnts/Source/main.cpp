#include "pch.h"
#include "Application.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            auto app = Application::GetInstance();

            if (SUCCEEDED(app.lock()->Initialize(hInstance)))
            {
                app.lock()->RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return WM_QUIT;
}
