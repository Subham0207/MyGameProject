#include <WarriorStatemachine.hpp>

REGISTER_TYPE(WarriorStatemachine, "WarriorStatemachine", StateMachineFactory)

WarriorStatemachine::WarriorStatemachine(): Controls::StateMachine("characterMovements")
{
    grounded = false;
    dodgeStart = false;
}

void WarriorStatemachine::onStart()
{
    //state machine 
    auto locomotionState = std::make_shared<Controls::State>("Locomotion");
    auto jumpState = std::make_shared<Controls::State>("Jump");
    auto dodgeRollState = std::make_shared<Controls::State>("DodgeRoll");

    locomotionState->toStateWhenCondition.push_back(
        Controls::ToStateWhenCondition(
        jumpState,
        [&]() -> bool {return !grounded;})
    );
    
    jumpState->toStateWhenCondition.push_back(
        Controls::ToStateWhenCondition(
        locomotionState,
        [&]() -> bool {return grounded;})
    );

    locomotionState->toStateWhenCondition.push_back(
        Controls::ToStateWhenCondition(
        dodgeRollState,
        [&]() -> bool {return dodgeStart;})
    );

    dodgeRollState->toStateWhenCondition.push_back(
        Controls::ToStateWhenCondition(
        locomotionState,
        [&]() -> bool {return !dodgeStart;})
    );

    this->setActiveState(locomotionState);

    locomotionBlendspace = new BlendSpace2D("character_blendspace");
    locomotionState->assignBlendspace(locomotionBlendspace);

    std::string walkLeft = "7e21d294-3ab3-4539-984d-ab3c78427e1b";
    std::string idle = "fb3ddbdc-7e68-4cc1-8ae0-d461eda1552a";
    std::string walkRight = "87630ef7-2a72-4b03-99e0-3e194fa8a56f";
    std::string walkBackwards = "4865f2b1-36a6-4d9c-8c84-115ac2adf3fb";
    std::string walkFront = "15943860-1128-4f18-9481-64d26d64d062";
    std::string running = "e94b6c8f-c8f8-4302-a57e-6eb5d292c7fd";
    std::string jump = "9b6c09af-14bc-47b9-a787-3a6e340ae3c8";
    std::string dodgeRoll = "5e16080b-832d-408c-b792-e156604a19b5";

    locomotionState->blendspace->AddBlendPoint(glm::vec2(0.0f, 0.0f), idle);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(-1.0f, 0.0f), walkLeft);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(1.0f, 0.0f), walkRight);


    locomotionState->blendspace->AddBlendPoint(glm::vec2(0.0f, 1.0f), walkFront);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(-1.0f, 1.0f), walkBackwards);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(1.0f, 1.0f), walkRight);

    locomotionState->blendspace->AddBlendPoint(glm::vec2(0.0f, 2.0f), walkRight);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(-1.0f, 2.0f), running);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(1.0f, 2.0f), walkRight);

    locomotionState->blendspace->AddBlendPoint(glm::vec2(-1.0f, -1.0f), walkBackwards);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(0.0f, -1.0f), walkBackwards);
    locomotionState->blendspace->AddBlendPoint(glm::vec2(1.0f, -1.0f), walkBackwards);

    jumpState->assignAnimation(jump, false, []{});
    dodgeRollState->assignAnimation(dodgeRoll, true, [&]{
        dodgeStart = false;
    });
}

void WarriorStatemachine::onTick()
{
}

void WarriorStatemachine::onDestroy()
{
}
