#include <Logic/Graph/GraphPrinter.h>
#include <Logic/Graph/Graph.h>
#include <Application/GraphApp.h>
#include <Utils/Random.h>
#include <conio.h>
#include <iostream>

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