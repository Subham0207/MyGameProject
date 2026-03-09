#pragma once
#include <GenericFactory.hpp>
#include <Controls/PlayerController.hpp>

class WarriorPlayerController: public Controls::PlayerController
{
    REGISTER_BODY(WarriorPlayerController)

    public:
        WarriorPlayerController();

        void OnStart() override {};
        void OnTick(float deltaTime) override {};
        void OnDestroy() override {};
};