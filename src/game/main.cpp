#include "application.h"

int main()
{
    Lumos::Platform platform;
    Application app{platform};
    app.run();
    return 0;
}

