#include <Application/PerlinNoiseWorld_AppLayer.h>
#include <Logic/PerlinNoiseWorld_GameLayer.h>

#ifdef _DEBUG
#include <VLD/vld.h>
#endif

int main(int argc, const char** argv)
{
    PerlinNoiseWorld_AppLayer app;

    if (!app.Init())
    {
        return 1;
    }

    app.Run();

    app.Cleanup();

    return 0;
}