#pragma once

namespace App
{

    class Project
    {
    public:
        virtual ~Project() = default;
        virtual void onInitialize();
        virtual void onUpdate();
        virtual void onShutdown();
    };

}
