#include <Application/DungeonGenerator_AppLayer.h>
#include <Logic/DungeonGenerator_GameLayer.h>

#ifdef _DEBUG
#include <VLD/vld.h>
#endif

int main(int argc, const char** argv)
{
    DungeonGenerator_AppLayer app;

    if (!app.Init())
    {
        return 1;
    }

    app.Run();

    app.Cleanup();

    return 0;
}