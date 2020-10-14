#include <Application/GameOfLife_AppLayer.h>
#include <Logic/GameOfLife_GameLayer.h>

#ifdef _DEBUG
#include <VLD/vld.h>
#endif

int main(int argc, const char** argv)
{
    GameOfLife_AppLayer app;

    if (!app.Init())
    {
        return 1;
    }

    app.Run();

    app.Cleanup();

    return 0;
}