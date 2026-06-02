#include "application.h"

int main()
{
    Platform platform;
    auto app = std::make_unique<Application>(platform);
    app->run();
    return 0;
}

