#include "application.h"

int main()
{
    Lumos::Platform platform;
    // auto app = std::make_unique<Application>(platform);
    // app->run();
    Application app{platform};
    app.run();
    return 0;
}

