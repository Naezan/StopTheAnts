#include "pch.h"
#include "Application.h"

// 전역 선언
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer; // 전역 선언
ID3D11InputLayout* pLayout; // 입력 레이아웃에 대한 포인터 
ID3D11VertexShader* pVS; // 버텍스 쉐이더 
ID3D11PixelShader* pPS; // 픽셀 쉐이더에 대한 포인터 
ID3D11Buffer* pVBuffer; // 정점 버퍼에 대한 포인터 

// 단일 정점을 정의하는 구조체
struct VERTEX { FLOAT X, Y, Z; D3DCOLORVALUE color; };

// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void RenderFrame(void); // 단일 프레임 렌더링
void CleanD3D(void);         // closes Direct3D and releases memory
void InitGraphics(void); // 렌더링할 모양을 만듭니다. 
void InitPipeline(void); // 셰이더를 로드하고 준비합니다.

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
            Application app;

            if (SUCCEEDED(app.Initialize(hInstance)))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return WM_QUIT;
}

void InitD3D(HWND hWnd)
{
    // Direct3D 초기화 
    // 
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH; // 백 버퍼 너비 설정 
    scd.BufferDesc.Height = SCREEN_HEIGHT; // 백 버퍼 높이 설정
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 전체 화면 전환 허용

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);

    // 렌더링 대상 설정
    // 
    // 백 버퍼의 주소 가져오기 
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // pBackBuffer 주소를 사용하여 렌더링 대상 생성 
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // 렌더링 대상을 백 버퍼로 설정 
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);

    // 뷰포트 설정 
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);

    InitGraphics();
    InitPipeline();
}

// 이것은 단일 프레임을 렌더링하는 데 사용되는 함수입니다. 
void RenderFrame(void)
{
    // 딥 블루로 백 버퍼를 지웁니다. 
    float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    devcon->ClearRenderTargetView(backbuffer, color);

    // 여기서 백 버퍼에서 3D 렌더링을 수행합니다. 
    // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    devcon->Draw(3, 0);

    // 백 버퍼와 프런트 버퍼를 전환합니다. 즉 현재 백버퍼를 프론트버퍼로 바꿔서 실제로 보이게 해준다!
    swapchain->Present(0, 0);
}

void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL); // 창 모드로 전환

    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();

    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}

void InitGraphics()
{
    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] =
    {
        {0.0f, 0.5f, 0.0f, D3DCOLORVALUE{1.0f, 0.0f, 0.0f, 1.0f}},
        {0.45f, -0.5, 0.0f, D3DCOLORVALUE{0.0f, 1.0f, 0.0f, 1.0f}},
        {-0.45f, -0.5f, 0.0f, D3DCOLORVALUE{0.0f, 0.0f, 1.0f, 1.0f}}
    };


    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}

void InitPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* VS = nullptr, * PS = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT VShr = D3DCompileFromFile(L"Resource/Shader/shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &errorBlob);

    if (FAILED(VShr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        if (VS)
            VS->Release();
    }

    errorBlob = nullptr;
    HRESULT PShr = D3DCompileFromFile(L"Resource/Shader/shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &errorBlob);

    if (FAILED(PShr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        if (PS)
            PS->Release();
    }

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}
