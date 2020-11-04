#include <Application/GraphApp.h>
#include <Logic/GraphGame.h>

#ifdef _DEBUG
#include <VLD/vld.h>
#endif

int main(int argc, const char** argv)
{
    GraphApp app;

    if (!app.Init())
    {
        return 1;
    }

    app.Run();

    app.Cleanup();

    return 0;
}