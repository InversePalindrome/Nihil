/*
Copyright (c) 2017 InversePalindrome
Nihil - PauseState.cpp
InversePalindrome.com
*/


#include "PauseState.hpp"
#include "GameState.hpp"
#include "StateMachine.hpp"
#include "SpriteParser.hpp"
#include "TextStyleParser.hpp"


PauseState::PauseState(StateMachine& stateMachine, StateData& stateData) :
    State(stateMachine, stateData),
    resumeButton(sfg::Button::Create("     Resume      ")),
    shopButton(sfg::Button::Create("        Shop        ")),
    achievementsButton(sfg::Button::Create("Achievements")),
    settingsButton(sfg::Button::Create("    Settings     ")),
    quitButton(sfg::Button::Create("        Quit         ")),
    isBackgroundVisible(true)
{
    Parsers::parseStyle(stateData.resourceManager, "StateTitle.txt", title);
    title.setString("Paused");
    title.setPosition(752.f, 280.f);

    Parsers::parseSprite(stateData.resourceManager, "PauseBackground.txt", background);
    background.setPosition(660.f, 428.f);

    Parsers::parseSprite(stateData.resourceManager, "StateTitleBar.txt", titleBar);
    titleBar.setPosition(617.f, 230.f);

    resumeButton->SetPosition({ 740.f, 535.f });
    resumeButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]()
        {
            resumeGame();
        });

    shopButton->SetPosition({ 740.f, 685.f });
    shopButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this, &stateMachine]()
        {
            isBackgroundVisible = false;
            stateMachine.pushState(StateID::Shop);
        });

    achievementsButton->SetPosition({ 740.f, 835.f });
    achievementsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this, &stateMachine]()
        {
            isBackgroundVisible = false;
            stateMachine.pushState(StateID::Achievements);
        });

    settingsButton->SetPosition({ 740.f, 985.f });
    settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this, &stateMachine]()
        {
            isBackgroundVisible = false;
            stateMachine.pushState(StateID::Settings);
        });

    quitButton->SetPosition({ 740.f, 1135.f });
    quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this]()
        {
            transitionToMenu();
        });

    stateData.guiManager.addWidget(resumeButton);
    stateData.guiManager.addWidget(shopButton);
    stateData.guiManager.addWidget(achievementsButton);
    stateData.guiManager.addWidget(settingsButton);
    stateData.guiManager.addWidget(quitButton);
}

void PauseState::handleEvent(const sf::Event& event)
{
    if (this->stateData.inputHandler.isActive(Action::Escape))
    {
        this->resumeGame();
    }
}

void PauseState::update(float deltaTime)
{
}

void PauseState::draw()
{
    if (this->isBackgroundVisible)
    {
        this->stateData.window.draw(this->background);
        this->stateData.window.draw(this->titleBar);
        this->stateData.window.draw(this->title);
    }
}

bool PauseState::isTransparent() const
{
    return true;
}

void PauseState::showWidgets(bool showStatus)
{
    this->resumeButton->Show(showStatus);
    this->settingsButton->Show(showStatus);
    this->shopButton->Show(showStatus);
    this->achievementsButton->Show(showStatus);
    this->quitButton->Show(showStatus);

    this->isBackgroundVisible = showStatus;
}

void PauseState::resumeGame()
{
    auto game = this->stateMachine[stateMachine.size() - 2].get();

    game->showWidgets(true);

    this->stateMachine.popState();
}

void PauseState::transitionToMenu()
{
    this->stateData.inputHandler.clearCallbacks();

    this->stateMachine.clearStates();
    this->stateMachine.pushState(StateID::Menu);
}