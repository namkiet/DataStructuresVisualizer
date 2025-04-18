#include "GUI/MainUI.hpp"
#include "Core/World.hpp"
#include "Core/FileReader.hpp"
#include <memory>
#include <DataStructures/LinkedList.hpp>
#include <DataStructures/Graph.hpp>
#include "GUI/ChildComponent.hpp"
#include <GUI/TextBox.hpp>
#include <Core/Variables.hpp>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include <random>
#include <Core/Utility.hpp>

MainUI::MainUI(TextureHolder& textures, FontHolder& fonts)
{
    BackRequest = false;
    mFont = fonts.get(Fonts::ID::UI);
    OperationButtonsList = std::make_shared<GUI::Container>();

    ToolBox.setSize(UI::TOOLBOX::Size);
    ToolBox.setFillColor(UI::TOOLBOX::FillColor);
    ToolBox.setOutlineThickness(2.f);
    ToolBox.setOutlineColor(UI::TOOLBOX::OutlineColor);
    ToolBox.setPosition(UI::TOOLBOX::Position + sf::Vector2f(2, 2));

    sf::Vector2f ToolboxPosition = ToolBox.getPosition();
    sf::Vector2f ToolBoxSize = ToolBox.getSize();

    mSeperateToolBoxLine.resize(4);
    mSeperateToolBoxLine[0] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[0][0].position = UI::INFOBOX::Position;
    mSeperateToolBoxLine[0][1].position = UI::INFOBOX::Position + sf::Vector2f(UI::INFOBOX::Size.x, 0);
    
    mSeperateToolBoxLine[1] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[1][0].position = UI::OPERATIONBOX::Position;
    mSeperateToolBoxLine[1][1].position = UI::OPERATIONBOX::Position + sf::Vector2f(UI::OPERATIONBOX::Size.x, 0);

    mSeperateToolBoxLine[2] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[2][0].position = UI::CONTROLBOX::Position;
    mSeperateToolBoxLine[2][1].position = UI::CONTROLBOX::Position + sf::Vector2f(UI::CONTROLBOX::Size.x, 0);


    OperationBox.setSize(sf::Vector2f(UI::OPERATIONBOX::Size.x * 0.4, UI::OPERATIONBOX::Size.y));
    OperationBox.setPosition(UI::OPERATIONBOX::Position);
    OperationBox.setFillColor(sf::Color::Transparent);

    OperationButtonPosition.resize(5);
    OperationButtonPosition[0] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.0);
    OperationButtonPosition[1] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.2);
    OperationButtonPosition[2] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.4);
    OperationButtonPosition[3] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.6);
    OperationButtonPosition[4] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.8);

    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);
    mSeperateToolBoxLine[3] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[3][0].position = UI::OPERATIONBOX::Position + sf::Vector2f(ButtonSize.x, 0);
    mSeperateToolBoxLine[3][1].position = UI::CONTROLBOX::Position + sf::Vector2f(ButtonSize.x, 0);


    sf::Sprite HomeSprite;
    HomeSprite.setTexture(textures.get(Textures::HomeIcon));
    BackButtons = std::make_shared<GUI::Button>(mFont, sf::Vector2f(20.f,20.f), "", sf::Vector2f(50.f,50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
    BackButtons->setSprite(HomeSprite);

    mSpeedButton = std::make_shared<GUI::Button>(mFont, sf::Vector2f(20.f, 800.f), "1x", sf::Vector2f(100, 100), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Text);
    mSpeedButton->setToggle(false);
    mSpeedButton->setCallback([=]()
    {
        speedIndex = (speedIndex + 1) % speed.size();
        std::ostringstream oss;
        oss << std::setprecision(2) << speed[speedIndex];
        mSpeedButton->setText(oss.str() + "x");
        ANIMATION::Speed = speed[speedIndex];
    });

    mSelectedRadioButtonSprite.setTexture(textures.get(Textures::SelectedRadioButton));
    mUnselectedRadioButtonSprite.setTexture(textures.get(Textures::UnselectedRadioButton));

    mStepByStepButton = std::make_shared<GUI::Button>(mFont, UI::CONTROLBOX::Position, "Run step by step", sf::Vector2f(UI::CONTROLBOX::Size.x / 2.f, UI::CONTROLBOX::Size.y), GUI::Button::ShapeType::Rectangle, GUI::Button::ContentType::Text);
    // mStepByStepButton->setToggle(false);
    mStepByStepButton->setSprite(mUnselectedRadioButtonSprite);

    mAtOnceButton = std::make_shared<GUI::Button>(mFont, UI::CONTROLBOX::Position + sf::Vector2f(UI::CONTROLBOX::Size.x / 2, 0), "Run at once", sf::Vector2f(UI::CONTROLBOX::Size.x / 2.f, UI::CONTROLBOX::Size.y), GUI::Button::ShapeType::Rectangle, GUI::Button::ContentType::Text);
    // mAtOnceButton->setToggle(false);
    mAtOnceButton->setSprite(mSelectedRadioButtonSprite);
}

void MainUI::updateCurrent(sf::Time dt)
{
    // do nothing
}

void MainUI::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    target.draw(ToolBox, states);
    target.draw(OperationBox, states);
    target.draw(*OperationButtonsList, states);
    target.draw(*BackButtons, states);
    target.draw(*mSpeedButton, states);
    target.draw(*mStepByStepButton, states);
    target.draw(*mAtOnceButton, states);

    
    for(auto &lines: mSeperateToolBoxLine)
        target.draw(lines, states);
}

void MainUI::initAVLButtons(AVLTree* avl, InfoPanel* info)
{
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);

    // Add Create button
    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

    GUI::Button::Ptr RandomButton   = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[0].y + OperationButtonPosition[1].y) / 2), "Random", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr LoadButton     = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, OperationButtonPosition[2].y), "Load", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr EmptyButton    = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[3].y + OperationButtonPosition[4].y) / 2), "Empty", sf::Vector2f(100.f,40.f));

    RandomButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(0); });
    LoadButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(1); });
    EmptyButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(2); });

    CreateButton->addSubComponent(RandomButton);
    CreateButton->addSubComponent(LoadButton);
    CreateButton->addSubComponent(EmptyButton);

    CreateButton->setFunc([CreateButton,avl]()
    {
        int type = CreateButton->getSubComponentInfo().InfoID;

        if (type == -1) return;
        else if (type == 0)
        {
            std::srand(std::time(nullptr)); 
            std::vector<int> randomList(10);
            for (int &num : randomList)
                num = std::rand() % 99 + 1; // Random numbers from 0 to 99
            
            avl->loadFromVector(randomList);
        }
        else if (type == 1)
        {
            std::wstring filename = OpenFileDialog();
            if (!filename.empty()) {
                std::wcout << L"Selected file: " << filename << std::endl;
                std::wifstream fin;
                fin.open(filename.c_str());
                if (!fin.is_open())
                {
                    std::cout << "Can't open file!";
                }
                else
                {
                    std::vector <int> numsFromFile;
                    int temp;
                    while (fin >> temp)
                        numsFromFile.push_back(temp);
                    for (int i = 0; i < numsFromFile.size(); i++)
                        std::cout << numsFromFile[i] << " ";
                    avl->loadFromVector(numsFromFile);
                }
                fin.close();
            } 
            else {
                std::cerr << "No file selected or an error occurred." << std::endl;
            }
        }
        else if (type == 2)
            avl->empty();

        CreateButton->resetSubComponentInfo();
    });

    // Add Insert button
    GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);
    GUI::TextBox::Ptr InputBoxInsert = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "x = ");
    InputBoxInsert->setCallback([InsertButton, InputBoxInsert, info]() {
        InsertButton->setSubComponentInfo(InputBoxInsert->getInputNum(),0);
        info->setText("Type a number from 0 to 99");
    });
    InsertButton->addSubComponent(InputBoxInsert);
    InsertButton->setFunc([this,InsertButton, avl]()
    {
        if (avl->isRunning()) return;
        int ActionType = InsertButton->getSubComponentInfo().InfoID;
        int num = InsertButton->getSubComponentInfo().num;
        if (ActionType == -1) return;

        if (ActionType == 0){
            avl->insert(num);
        }


        InsertButton->resetSubComponentInfo();
    });

    // Add Delete button
    GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
    GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "x = ");
    InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete, info]()
    {
        DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
        info->setText("Type a number from 0 to 99");
    });
    DeleteButton->addSubComponent(InputBoxDelete);
    DeleteButton->setFunc([this,DeleteButton, avl]()
    {
        if (avl->isRunning()) return;
        int num = DeleteButton->getSubComponentInfo().num;
        int ActionType = DeleteButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        else if (ActionType == 0)
            avl->remove(num);

        DeleteButton->resetSubComponentInfo();
    });

    // Add Search button
    GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "x =");
    InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch, info]() {
        SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
        info->setText("Type a number from 0 to 99");
    });
    SearchButton->addSubComponent(InputBoxSearch);
    SearchButton->setFunc([this,SearchButton,avl]()
    {
        if (avl->isRunning()) return;
        int num = SearchButton->getSubComponentInfo().num;
        int ActionType = SearchButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            avl->search(num);
        }
        SearchButton->resetSubComponentInfo();
    });

    // add update button
    GUI::ExpandableButton::Ptr UpdateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[4], "Update", ButtonSize);
    GUI::TextBox::Ptr InputBoxUpdate = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "old =, new =", GUI::TextBox::InputType::VectorNum);
    InputBoxUpdate->setCallback([this, UpdateButton, InputBoxUpdate, info]()
    {
        UpdateButton->setSubComponentInfo(InputBoxUpdate->getInputNumList(),0);
        info->setText("Type the old and new value, seperated by a comma.");
    });

    UpdateButton->addSubComponent(InputBoxUpdate);

    UpdateButton->setFunc([this,UpdateButton,avl]()
    {
        if (avl->isRunning()) return;
        std::vector<int> nums = UpdateButton->getSubComponentInfo().VecNum;
        int ActionType = UpdateButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            if (nums.size() == 2)
            {
                avl->updateValue(nums[0], nums[1]);
                std::cout<<"Here we can update the value "<<nums[0]<<" to "<<nums[1]<<"!"<<std::endl;
            }
        }
        UpdateButton->resetSubComponentInfo();

    });

    // Pack all buttons
    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(InsertButton);
    OperationButtonsList->pack(DeleteButton);
    OperationButtonsList->pack(SearchButton);
    OperationButtonsList->pack(UpdateButton);
}

void MainUI::initHeapButtons(HeapTree* heap)
{
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);

    // Add Create button
    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

    GUI::Button::Ptr RandomButton   = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[0].y + OperationButtonPosition[1].y) / 2), "Random", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr LoadButton     = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, OperationButtonPosition[2].y), "Load", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr EmptyButton    = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[3].y + OperationButtonPosition[4].y) / 2), "Empty", sf::Vector2f(100.f,40.f));

    RandomButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(0); });
    LoadButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(1); });
    EmptyButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(2); });

    CreateButton->addSubComponent(RandomButton);
    CreateButton->addSubComponent(LoadButton);
    CreateButton->addSubComponent(EmptyButton);

    CreateButton->setFunc([CreateButton,heap]()
    {

        int type = CreateButton->getSubComponentInfo().InfoID;

        if (type == -1) return;
        else if (type == 0)
        {
            std::srand(std::time(nullptr)); 
            std::vector<int> randomList(10);
            for (int &num : randomList)
                num = std::rand() % 99 + 1; // Random numbers from 0 to 99
            
            heap->loadFromVector(randomList);
        }
        else if (type == 1)
        {
            std::wstring filename = OpenFileDialog();
            if (!filename.empty()) {
                std::wcout << L"Selected file: " << filename << std::endl;
                std::wifstream fin;
                fin.open(filename.c_str());
                if (!fin.is_open())
                {
                    std::cout << "Can't open file!";
                }
                else
                {
                    std::vector <int> numsFromFile;
                    int temp;
                    while (fin >> temp)
                        numsFromFile.push_back(temp);
                    for (int i = 0; i < numsFromFile.size(); i++)
                        std::cout << numsFromFile[i] << " ";
                    heap->loadFromVector(numsFromFile);
                }
                fin.close();
            } 
            else {
                std::cerr << "No file selected or an error occurred." << std::endl;
            }
        }
        else if (type == 2)
            heap->empty();

        CreateButton->resetSubComponentInfo();
    });

    // Add Push button
    GUI::ExpandableButton::Ptr PushButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Push",ButtonSize);
    GUI::TextBox::Ptr InputBoxPush = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "x = ");
    InputBoxPush->setCallback([PushButton, InputBoxPush]()
    {
        PushButton->setSubComponentInfo(InputBoxPush->getInputNum(),0);
    });
    
    PushButton->addSubComponent(InputBoxPush);
    PushButton->setFunc([this,PushButton, heap]()
    {
        if (heap->isRunning()) return;

        int ActionType = PushButton->getSubComponentInfo().InfoID;
        int num = PushButton->getSubComponentInfo().num;
        if (ActionType == -1) return;

        if (ActionType == 0)
        {
            heap->insert(num);
        }
        PushButton->resetSubComponentInfo();
    });

    // Add Pop button
    GUI::ExpandableButton::Ptr PopButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Pop",ButtonSize);
    GUI::Button::Ptr SubmitButton = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, OperationButtonPosition[2].y), "Extract Top", sf::Vector2f(100.f,40.f));
    SubmitButton->setCallback([this,PopButton, SubmitButton]() { PopButton->setSubComponentInfo(0); });
    PopButton->addSubComponent(SubmitButton);
    PopButton->setFunc([this,PopButton, heap]()
    {
        if (heap->isRunning()) return;
        // int num = PopButton->getSubComponentInfo().num;
        int ActionType = PopButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        else if (ActionType == 0)
        {
            heap->remove(0);
        }
        
        PopButton->resetSubComponentInfo();
    });

    // Add Search Button
    GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));
    InputBoxSearch->setCallback([this,SearchButton, InputBoxSearch]()
    {
        SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
    });

    SearchButton->addSubComponent(InputBoxSearch);
    SearchButton->setFunc([this,SearchButton, heap]()
    {
        if (heap->isRunning()) return;
        int num = SearchButton->getSubComponentInfo().num;
        int ActionType = SearchButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        else if (ActionType == 0)
        {
            heap->search(num);
        }
        
        SearchButton->resetSubComponentInfo();
    });

    // add update button
    GUI::ExpandableButton::Ptr UpdateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[4], "Update", ButtonSize);
    GUI::TextBox::Ptr InputBoxUpdate = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "Update a to b", GUI::TextBox::InputType::VectorNum);
    InputBoxUpdate->setCallback([this, UpdateButton, InputBoxUpdate]()
        {
            UpdateButton->setSubComponentInfo(InputBoxUpdate->getInputNumList(),0);
        });

    UpdateButton->addSubComponent(InputBoxUpdate);

    UpdateButton->setFunc([this,UpdateButton,heap]()
    {
        if (heap->isRunning()) return;
        std::vector<int> nums = UpdateButton->getSubComponentInfo().VecNum;
        int ActionType = UpdateButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            if (nums.size() == 2)
            {
                heap->updateValue(nums[0], nums[1]);
                std::cout<<"Here we can update the value "<<nums[0]<<" to "<<nums[1]<<"!"<<std::endl;
            }
        }
        UpdateButton->resetSubComponentInfo();

    });

    // Pack all buttons
    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(PushButton);
    OperationButtonsList->pack(PopButton);
    OperationButtonsList->pack(SearchButton);
    OperationButtonsList->pack(UpdateButton);
}

void MainUI::initLinkedListButtons(LinkedList* ll)
{
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);

    // Add Create button
    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

    GUI::Button::Ptr RandomButton   = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[0].y + OperationButtonPosition[1].y) / 2), "Random", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr LoadButton     = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, OperationButtonPosition[2].y), "Load", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr EmptyButton    = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[3].y + OperationButtonPosition[4].y) / 2), "Empty", sf::Vector2f(100.f,40.f));

    RandomButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(0); });
    LoadButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(1); });
    EmptyButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(2); });

    CreateButton->addSubComponent(RandomButton);
    CreateButton->addSubComponent(LoadButton);
    CreateButton->addSubComponent(EmptyButton);

    CreateButton->setFunc([CreateButton,ll]()
    {
        int type = CreateButton->getSubComponentInfo().InfoID;

        if (type == -1) return;
        else if (type == 0)
        {
            std::srand(std::time(nullptr)); 
            std::vector<int> randomList(10);
            for (int &num : randomList)
                num = std::rand() % 99 + 1; // Random numbers from 0 to 99
            
            ll->loadFromVector(randomList);
        }
        else if (type == 1)
        {
            std::wstring filename = OpenFileDialog();
            if (!filename.empty()) {
                std::wcout << L"Selected file: " << filename << std::endl;
                std::wifstream fin;
                fin.open(filename.c_str());
                if (!fin.is_open())
                {
                    std::cout << "Can't open file!";
                }
                else
                {
                    std::vector <int> numsFromFile;
                    int temp;
                    while (fin >> temp)
                        numsFromFile.push_back(temp);
                    for (int i = 0; i < numsFromFile.size(); i++)
                        std::cout << numsFromFile[i] << " ";
                    ll->loadFromVector(numsFromFile);
                }
                fin.close();
            } 
            else {
                std::cerr << "No file selected or an error occurred." << std::endl;
            }
        }
        else if (type == 2)
            ll->empty();

        CreateButton->resetSubComponentInfo();
    });

    GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);

    // initialize the textbox
    GUI::TextBox::Ptr InputBoxInsertAtHead = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[1].y) , sf::Vector2f(100.f, 40.f), "Insert at Head");
    // attach it to its parent
    InsertButton->addSubComponent(InputBoxInsertAtHead);
    // set callback for the textbox
    InputBoxInsertAtHead->setCallback([InsertButton, InputBoxInsertAtHead]()
    {
        InsertButton->setSubComponentInfo(InputBoxInsertAtHead->getInputNum(),0);
    });

    GUI::TextBox::Ptr InputBoxInsertAtLast = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[3].y) , sf::Vector2f(100.f, 40.f), "Insert at Tail");

    InsertButton->addSubComponent(InputBoxInsertAtLast);

    InputBoxInsertAtLast->setCallback([InsertButton, InputBoxInsertAtLast]()
    {
        InsertButton->setSubComponentInfo(InputBoxInsertAtLast->getInputNum(),1 );
    });


    InsertButton->setFunc([this,InsertButton, ll]()
    {
        if (ll->isRunning()) return;
        int ActionType = InsertButton->getSubComponentInfo().InfoID;
        int num = InsertButton->getSubComponentInfo().num;
        if (ActionType == -1) return;

        if (ActionType == 0) // InsertAtHead
        {
            ll->InsertAtHead(num);
        }
        else if (ActionType == 1) //insertAtLast
        {
            ll->InsertAtLast(num);
        }
        InsertButton->resetSubComponentInfo();
    });

    GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
    GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

    DeleteButton->addSubComponent(InputBoxDelete);
    InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete]()
    {
        DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
    });
    DeleteButton->setFunc([this,DeleteButton, ll]()
    {
        if (ll->isRunning()) return;
        int num = DeleteButton->getSubComponentInfo().num;
        int ActionType = DeleteButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        else if (ActionType == 0)
        {
            ll->remove(num);
        }
        
        DeleteButton->resetSubComponentInfo();
    });

    GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

    SearchButton->addSubComponent(InputBoxSearch);
    InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch]()
    {
        SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
    });

    SearchButton->setFunc([this,SearchButton,ll]()
    {
        if (ll->isRunning()) return;
        int num = SearchButton->getSubComponentInfo().num;
        int ActionType = SearchButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            ll->search(num);
        }
        SearchButton->resetSubComponentInfo();
    });

    // add update button
    GUI::ExpandableButton::Ptr UpdateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[4], "Update", ButtonSize);
    GUI::TextBox::Ptr InputBoxUpdate = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f), "Update a to b", GUI::TextBox::InputType::VectorNum);
    InputBoxUpdate->setCallback([this, UpdateButton, InputBoxUpdate]()
     {
         UpdateButton->setSubComponentInfo(InputBoxUpdate->getInputNumList(),0);
     });

    UpdateButton->addSubComponent(InputBoxUpdate);

    UpdateButton->setFunc([this,UpdateButton,ll]()
    {
        if (ll->isRunning()) return;
        std::vector<int> nums = UpdateButton->getSubComponentInfo().VecNum;
        int ActionType = UpdateButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            if (nums.size() == 2)
            {
                ll->updateValue(nums[0], nums[1]);
                std::cout<<"Here we can update the value "<<nums[0]<<" to "<<nums[1]<<"!"<<std::endl;
            }
        }
        UpdateButton->resetSubComponentInfo();

    });

    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(InsertButton);
    OperationButtonsList->pack(DeleteButton);
    OperationButtonsList->pack(SearchButton);
    OperationButtonsList->pack(UpdateButton);
}

void MainUI::initGraphButtons(Graph* g)
{
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);

    // Add Create button
    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

    GUI::Button::Ptr RandomButton   = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[0].y + OperationButtonPosition[1].y) / 2), "Random", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr LoadButton     = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, OperationButtonPosition[2].y), "Load", sf::Vector2f(100.f,40.f));
    GUI::Button::Ptr EmptyButton    = std::make_shared<GUI::Button>(mFont, sf::Vector2f(ToolBox.getSize().x * 0.55, (OperationButtonPosition[3].y + OperationButtonPosition[3].y) / 2), "Empty", sf::Vector2f(100.f,40.f));

    RandomButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(0); });
    LoadButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(1); });
    EmptyButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(2); });

    CreateButton->addSubComponent(RandomButton);
    CreateButton->addSubComponent(LoadButton);
    CreateButton->addSubComponent(EmptyButton);

    CreateButton->setFunc([CreateButton,g]()
    {
        int type = CreateButton->getSubComponentInfo().InfoID;

        if (type == -1) return;
        else if (type == 0)
        {
            int nodeCount = 8;
            int edgeCount = 13;
            int minWeight = 1;
            int maxWeight = 10;

            std::vector<int> edgeData;
            std::unordered_set<long long> seen;

            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> nodeDist(0, nodeCount - 1);
            std::uniform_int_distribution<int> weightDist(minWeight, maxWeight);

            auto encode = [](int u, int v) {
                if (u > v) std::swap(u, v);
                return (static_cast<long long>(u) << 32) | v;
            };

            // Đảm bảo mỗi node xuất hiện ít nhất một lần
            for (int u = 0; u < nodeCount; ++u) {
                int v;
                do {
                    v = nodeDist(rng);
                } while (v == u || seen.count(encode(u, v)));

                seen.insert(encode(u, v));
                int w = weightDist(rng);
                edgeData.push_back(u);
                edgeData.push_back(v);
                edgeData.push_back(w);
            }

            // Tiếp tục sinh các cạnh còn lại cho đủ số lượng
            while (edgeData.size() < edgeCount * 3) {
                int u = nodeDist(rng);
                int v = nodeDist(rng);
                if (u == v) continue;

                long long key = encode(u, v);
                if (seen.count(key)) continue;
                seen.insert(key);

                int w = weightDist(rng);
                edgeData.push_back(u);
                edgeData.push_back(v);
                edgeData.push_back(w);
            }

            g->loadFromVector(edgeData);

        }
        else if (type == 1)
        {
            std::wstring filename = OpenFileDialog();
            if (!filename.empty()) {
                std::wcout << L"Selected file: " << filename << std::endl;
                std::wifstream fin;
                fin.open(filename.c_str());
                if (!fin.is_open())
                {
                    std::cout << "Can't open file!";
                }
                else
                {
                    std::vector <int> numsFromFile;
                    int temp;
                    while (fin >> temp)
                        numsFromFile.push_back(temp);
                    for (int i = 0; i < numsFromFile.size(); i++)
                        std::cout << numsFromFile[i] << " ";
                    g->loadFromVector(numsFromFile);
                }
                fin.close();
            } 
            else {
                std::cerr << "No file selected or an error occurred." << std::endl;
            }
        }
        else if (type == 2)
            g->empty();

        CreateButton->resetSubComponentInfo();
    });

    // Add MST Button
    GUI::ExpandableButton::Ptr MSTButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "MST", ButtonSize);
    MSTButton->setCallback([MSTButton]{
        MSTButton->setSubComponentInfo(0);
    });
    MSTButton->setFunc([MSTButton, g]() {

        if (g->isRunning()) return;

            int ActionType = MSTButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
                {
                    g->Prim();
                }
                
            MSTButton->resetSubComponentInfo();
            });

    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(MSTButton);
}

void MainUI::createButtonList(World::Mode mode, DS* mDataStructure, InfoPanel* info)
{
    OperationButtonsList->makeEmpty();

    mStepByStepButton->setCallback([=]()
    {
        mDataStructure->isStepByStep = true;
        mDataStructure->stop = true;
        mStepByStepButton->setSprite(mSelectedRadioButtonSprite);
        mAtOnceButton->setSprite(mUnselectedRadioButtonSprite);
    });

    mAtOnceButton->setCallback([=]()
    {
        mDataStructure->isStepByStep = false;
        mDataStructure->stop = false;
        mStepByStepButton->setSprite(mUnselectedRadioButtonSprite);
        mAtOnceButton->setSprite(mSelectedRadioButtonSprite);
    });

    if (mode == World::Mode::AVLMode)
    {
        auto avl = dynamic_cast<AVLTree*>(mDataStructure);
        initAVLButtons(avl, info);
	}
    else if (mode == World::Mode::LinkedListMode)
    {
        auto ll = static_cast<LinkedList*>(mDataStructure);
        initLinkedListButtons(ll);
    }
    else if (mode == World::Mode::HeapMode)
    {
        auto heap = static_cast<HeapTree*>(mDataStructure);
        initHeapButtons(heap);
    }
    else if (mode == World::Mode::GraphMode)
    {
        auto g = static_cast<Graph*>(mDataStructure);
        initGraphButtons(g);
    }
}

void MainUI::handleEvent(const sf::Event& event)
{
    OperationButtonsList->handleEvent(event);

    // mInstructText.setString("");
    // GUI::Component::Ptr ActivateChild = OperationButtonsList->getActivateChild();
    // if(ActivateChild != NULL)
    // {
    //     auto activateChild = std::dynamic_pointer_cast<GUI::ExpandableButton>(ActivateChild);
    //     if (activateChild) {
    //         for (auto& sub : activateChild->mSubComponents) 
    //         {
    //             if(!sub->isSelected()) continue;
    //             auto textbox = std::dynamic_pointer_cast<GUI::TextBox>(sub);
    //             if(textbox) // it is textbox type 
    //             {
    //                 if(textbox->mInputType == GUI::TextBox::InputType::Number)
    //                 {
    //                     mInstructText.setString(instructNumInput);
    //                 }
    //                 else if(textbox->mInputType == GUI::TextBox::InputType::VectorNum)
    //                 {
    //                     mInstructText.setString(instructVecNumInput);
    //                 }
    //                 mInstructText.setPosition(sf::Vector2f(OperationBox.getSize().x + 350.f, textbox->getGlobalBounds().top + 25.f));
    //                 centerOrigin(mInstructText);
    //             }
    //         }
    //     }
    // }

    BackButtons->handleEvent(event);
    if (BackButtons->isActive())
    {
        ANIMATION::Speed = 1;
        BackRequest = true;
    }

    mSpeedButton->handleEvent(event);
    mStepByStepButton->handleEvent(event);
    mAtOnceButton->handleEvent(event);
}

bool MainUI::getBackRequest()
{
    return BackRequest;
}