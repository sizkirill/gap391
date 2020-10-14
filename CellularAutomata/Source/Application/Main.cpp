#include <Application/CellularAutomata_AppLayer.h>
#include <Logic/CellularAutomata_GameLayer.h>

#ifdef _DEBUG
#include <VLD/vld.h>
#endif

int main(int argc, const char** argv)
{
    CellularAutomata_AppLayer app;

    if (!app.Init())
    {
        return 1;
    }

    app.Run();

    app.Cleanup();

    return 0;
}