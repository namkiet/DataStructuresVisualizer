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

    ButtonSize = sf::Vector2f(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);
    TextBoxSize = sf::Vector2f(120.f, 40.f);

    mSeperateToolBoxLine[3] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[3][0].position = UI::OPERATIONBOX::Position + sf::Vector2f(ButtonSize.x, 0);
    mSeperateToolBoxLine[3][1].position = UI::CONTROLBOX::Position + sf::Vector2f(ButtonSize.x, 0);


    sf::Sprite HomeSprite;
    HomeSprite.setTexture(textures.get(Textures::HomeIcon));
    BackButtons = std::make_shared<GUI::Button>(mFont, sf::Vector2f(20.f,20.f), "", sf::Vector2f(50.f,50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
    BackButtons->setSprite(HomeSprite);

    mSpeedButton = std::make_shared<GUI::Button>(mFont, sf::Vector2f(SCREEN::Width - 170, SCREEN::Height - 35), "1x", sf::Vector2f(20, 20), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Text);
    mSpeedButton->setToggle(false);
    mSpeedButton->setActivatedColor(UI::BUTTON::FillColor);
    mSpeedButton->setSelectedColor(UI::BUTTON::FillColor);
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
    mStepByStepButton->setToggle(false);
    // mStepByStepButton->setSprite(mUnselectedRadioButtonSprite);
    mStepByStepButton->setNormalColor(UI::BUTTON::FillColor);
    mStepByStepButton->setSelectedColor(UI::BUTTON::SelectedColor);

    mAtOnceButton = std::make_shared<GUI::Button>(mFont, UI::CONTROLBOX::Position + sf::Vector2f(UI::CONTROLBOX::Size.x / 2, 0), "Run at once", sf::Vector2f(UI::CONTROLBOX::Size.x / 2.f, UI::CONTROLBOX::Size.y), GUI::Button::ShapeType::Rectangle, GUI::Button::ContentType::Text);
    mAtOnceButton->setToggle(false);
    // mAtOnceButton->setSprite(mSelectedRadioButtonSprite);
    mAtOnceButton->setNormalColor(UI::BUTTON::ActivatedColor);
    mAtOnceButton->setSelectedColor(UI::BUTTON::SelectedColor);
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

    // Add Insert button
    GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);
    GUI::TextBox::Ptr InputBoxInsert = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");
    InputBoxInsert->setCallback([InsertButton, InputBoxInsert, info]() {
        InsertButton->setSubComponentInfo(InputBoxInsert->getInputNum(),0);
    });
    InsertButton->addSubComponent(InputBoxInsert);
    InsertButton->setFunc([this,InsertButton, avl, info]()
    {
        if (!avl->isRunning())
        {
            int ActionType = InsertButton->getSubComponentInfo().InfoID;
            int num = InsertButton->getSubComponentInfo().num;
            if (ActionType == -1) return;

            if (ActionType == 0){
                if (num < 0 || num > 99)
                    info->setText("!Input number must be from 0 to 99.");
                else if (avl->getSize() >= 50)
                    info->setText("!Maximum size reached.");
                else
                {
                    info->setText("");
                    avl->insert(num);
                }
            }
        }

        InsertButton->resetSubComponentInfo();
    });

    // Add Delete button
    GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
    GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");
    InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete, info]()
    {
        DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
    });
    DeleteButton->addSubComponent(InputBoxDelete);
    DeleteButton->setFunc([this,DeleteButton, avl, info]()
    {
        if (!avl->isRunning())
        {
            int num = DeleteButton->getSubComponentInfo().num;
            int ActionType = DeleteButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
            {
                if (num < 0 || num > 99)
                    info->setText("!Input number must be from 0 to 99.");
                else
                {
                    info->setText("");
                    avl->remove(num);
                }
            }
        }

        DeleteButton->resetSubComponentInfo();
    });

    // Add Search button
    GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");
    InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch, info]() {
        SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
    });
    SearchButton->addSubComponent(InputBoxSearch);
    SearchButton->setFunc([this,SearchButton,avl, info]()
    {
        if (!avl->isRunning())
        {
            int num = SearchButton->getSubComponentInfo().num;
            int ActionType = SearchButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            if (ActionType == 0)
            {
                if (num < 0 || num > 99)
                    info->setText("!Input number must be from 0 to 99.");
                else
                {
                    info->setText("");
                    avl->search(num);
                }
            }
        }
        SearchButton->resetSubComponentInfo();
    });

    // Pack all buttons
    OperationButtonsList->pack(InsertButton);
    OperationButtonsList->pack(DeleteButton);
    OperationButtonsList->pack(SearchButton);
}

void MainUI::initHeapButtons(HeapTree* heap, InfoPanel *info)
{
    // Add Push button
    GUI::ExpandableButton::Ptr PushButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Push",ButtonSize);
    GUI::TextBox::Ptr InputBoxPush = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");
    InputBoxPush->setCallback([PushButton, InputBoxPush]()
    {
        PushButton->setSubComponentInfo(InputBoxPush->getInputNum(),0);
    });
    
    PushButton->addSubComponent(InputBoxPush);
    PushButton->setFunc([this,PushButton, heap, info]()
    {
        if (!heap->isRunning())
        {
            int ActionType = PushButton->getSubComponentInfo().InfoID;
            int num = PushButton->getSubComponentInfo().num;
            if (ActionType == -1) return;

            if (ActionType == 0){
                if (num < 0 || num > 99)
                    info->setText("!Input number must be from 0 to 99.");
                else if (heap->getSize() >= 50)
                    info->setText("!Maximum size reached.");
                else
                {
                    info->setText("");
                    heap->insert(num);
                }
            }
        }
        PushButton->resetSubComponentInfo();
    });

    // Add Pop button
    GUI::ExpandableButton::Ptr PopButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Pop",ButtonSize);
    GUI::Button::Ptr SubmitButton = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), "Extract Top", TextBoxSize);
    SubmitButton->setCallback([this,PopButton, SubmitButton]() { PopButton->setSubComponentInfo(0); });
    SubmitButton->setOutlineColor(sf::Color(255, 255, 255, 50));

    PopButton->addSubComponent(SubmitButton);
    PopButton->setFunc([this,PopButton, heap]()
    {
        if (!heap->isRunning())
        {
            int ActionType = PopButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
            {
                heap->remove(0);
            }
        }

        PopButton->resetSubComponentInfo();
    });

    // Add Search Button
    GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");
    InputBoxSearch->setCallback([this,SearchButton, InputBoxSearch]()
    {
        SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
    });

    SearchButton->addSubComponent(InputBoxSearch);
    SearchButton->setFunc([this,SearchButton, heap, info]()
    {
        if (!heap->isRunning())
        {
            int num = SearchButton->getSubComponentInfo().num;
            int ActionType = SearchButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
            {
                if (num < 0 || num > 99)
                    info->setText("!Input number must be from 0 to 99.");
                else
                {
                    info->setText("");
                    heap->search(num);
                }
            }
        }
        
        SearchButton->resetSubComponentInfo();
    });

    // Pack all buttons
    OperationButtonsList->pack(PushButton);
    OperationButtonsList->pack(PopButton);
    OperationButtonsList->pack(SearchButton);
}

void MainUI::initLinkedListButtons(LinkedList* ll, InfoPanel *info)
{
    GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);
    GUI::TextBox::Ptr InputBoxInsertAtHead = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[1].y + OperationButtonPosition[2].y - TextBoxSize.y) / 2), TextBoxSize, "x = (head)");
    InputBoxInsertAtHead->setCallback([InsertButton, InputBoxInsertAtHead]()
    {
        InsertButton->setSubComponentInfo(InputBoxInsertAtHead->getInputNum(), 0);
    });

    GUI::TextBox::Ptr InputBoxInsertAtLast = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = (tail)");
    InputBoxInsertAtLast->setCallback([InsertButton, InputBoxInsertAtLast]()
    {
        InsertButton->setSubComponentInfo(InputBoxInsertAtLast->getInputNum(), 1);
    });

    GUI::TextBox::Ptr InputBoxInsertAtIndex = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[3].y + OperationButtonPosition[4].y - TextBoxSize.y) / 2), TextBoxSize, "x =, index =", GUI::TextBox::InputType::VectorNum);
    InputBoxInsertAtIndex->setCallback([InsertButton, InputBoxInsertAtIndex]()
    {
        InsertButton->setSubComponentInfo(InputBoxInsertAtIndex->getInputNumList(), 2);
    });

    InsertButton->addSubComponent(InputBoxInsertAtHead);
    InsertButton->addSubComponent(InputBoxInsertAtLast);
    InsertButton->addSubComponent(InputBoxInsertAtIndex);


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
        else if (ActionType == 2) // insert at index
        {
            int v = InsertButton->getSubComponentInfo().VecNum[0];
            int id = InsertButton->getSubComponentInfo().VecNum[1];
            ll->insertAtIndex(v, id);
        }
        InsertButton->resetSubComponentInfo();
    });

    GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
    GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");

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
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "x = ");

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

    OperationButtonsList->pack(InsertButton);
    OperationButtonsList->pack(DeleteButton);
    OperationButtonsList->pack(SearchButton);
}

void MainUI::initGraphButtons(Graph* g, InfoPanel *info)
{
    // Add Create button
    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

    GUI::Button::Ptr RandomButton   = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[1].y + OperationButtonPosition[1].y - TextBoxSize.y) / 2), "Random", TextBoxSize);
    GUI::Button::Ptr LoadButton     = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), "Load", TextBoxSize);
    GUI::Button::Ptr EmptyButton    = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[4].y + OperationButtonPosition[4].y - TextBoxSize.y) / 2), "Empty", TextBoxSize);

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
            int nodeCount = 10;
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
            g->loadTimer = 1.f;
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
                // g->empty();
                std::cerr << "No file selected or an error occurred." << std::endl;
            }
        }
        else if (type == 2)
            g->empty();

        CreateButton->resetSubComponentInfo();
    });

    // Add MST Button
    GUI::ExpandableButton::Ptr MSTButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "MST", ButtonSize);
    GUI::Button::Ptr PrimButton = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), "Prim", TextBoxSize);

    PrimButton->setCallback([this,MSTButton, PrimButton]() { MSTButton->setSubComponentInfo(0); });
    PrimButton->setOutlineColor(sf::Color(255, 255, 255, 50));

    MSTButton->addSubComponent(PrimButton);

    MSTButton->setFunc([MSTButton, g]() 
    {
        if (!g->isRunning())
        {
            int ActionType = MSTButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
            {
                g->Prim();
            }   
        }
        MSTButton->resetSubComponentInfo();
    });

    // Add init button
    sf::Vector2f BoxSize = sf::Vector2f(UI::OPERATIONBOX::Size.x - OperationBox.getSize().x, UI::OPERATIONBOX::Size.y) * 0.8;
    sf::Vector2f BoxPosition = sf::Vector2f((UI::OPERATIONBOX::Size.x + OperationBox.getSize().x - BoxSize.x) / 2.f, UI::OPERATIONBOX::Position.y + (UI::OPERATIONBOX::Size.y - BoxSize.y) / 2.f);

    GUI::ExpandableButton::Ptr InitButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Init", ButtonSize);
    GUI::TextBox::Ptr InputBoxInit = std::make_shared<GUI::TextBox>(mFont, BoxPosition, BoxSize, "Input edge list.\nOne edge each line", GUI::TextBox::InputType::VectorNum);
    InputBoxInit->setPlaceholderPosition(sf::Vector2f(0, 0));

    InputBoxInit->setCallback([this, InitButton, InputBoxInit]()
    {
        InitButton->setSubComponentInfo(InputBoxInit->getInputNumList(),0);
    });
    sf::Vector2f textPos = BoxPosition + sf::Vector2f(5.f, InputBoxInit->mCharSize/2);
    InputBoxInit->mText.setPosition(textPos);
    InitButton->addSubComponent(InputBoxInit);
    InputBoxInit->setMessage("Shift + Enter to start a new line.");
    InputBoxInit->setAllowedEndLine(true);
    InputBoxInit->mText.setLineSpacing(1.f);


    InitButton->setFunc([this,InitButton,g, info]()
    {
        if (!g->isRunning())
        {
        std::vector<int> nums = InitButton->getSubComponentInfo().VecNum;
        int ActionType = InitButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            if (!g->loadFromVector(nums))
                info->setText("!Invalid input.");
        }
        }
        InitButton->resetSubComponentInfo();

    });
    

    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(MSTButton);
    OperationButtonsList->pack(InitButton);
}

void MainUI::createButtonList(World::Mode mode, DS* mDataStructure, InfoPanel* info)
{
    OperationButtonsList->makeEmpty();

    mStepByStepButton->setCallback([=]()
    {
        mDataStructure->isStepByStep = true;
        mDataStructure->stop = true;
        mStepByStepButton->setNormalColor(UI::BUTTON::ActivatedColor);
        mAtOnceButton->setNormalColor(UI::BUTTON::FillColor);
    });

    mAtOnceButton->setCallback([=]()
    {
        mDataStructure->isStepByStep = false;
        mDataStructure->stop = false;
        mStepByStepButton->setNormalColor(UI::BUTTON::FillColor);
        mAtOnceButton->setNormalColor(UI::BUTTON::ActivatedColor);
    });

    if (mode != World::Mode::GraphMode)
    {
        // Add Create button
        GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

        GUI::TextBox::Ptr InputBoxRandom    = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, OperationButtonPosition[1].y - TextBoxSize.y / 2), TextBoxSize, "N = ");
        GUI::TextBox::Ptr InputBoxInit      = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, OperationButtonPosition[2].y - TextBoxSize.y / 2), TextBoxSize, "array = ", GUI::TextBox::InputType::VectorNum);
        GUI::Button::Ptr LoadButton         = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, OperationButtonPosition[3].y - TextBoxSize.y / 2), "Load", TextBoxSize);
        GUI::Button::Ptr EmptyButton        = std::make_shared<GUI::Button>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, OperationButtonPosition[4].y - TextBoxSize.y / 2), "Empty", TextBoxSize);

        InputBoxRandom->setCallback([CreateButton, InputBoxRandom]() { CreateButton->setSubComponentInfo(InputBoxRandom->getInputNum(), 0); });
        InputBoxInit->setCallback([CreateButton, InputBoxInit]() { CreateButton->setSubComponentInfo(InputBoxInit->getInputNumList(), 1); });
        LoadButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(2); });
        EmptyButton->setCallback([CreateButton]() { CreateButton->setSubComponentInfo(3); });

        InputBoxRandom->setMessage("Input the size of the random data.");
        InputBoxInit->setMessage("Input the list for initialization.");
        LoadButton->setOutlineColor(sf::Color(255, 255, 255, 50));
        EmptyButton->setOutlineColor(sf::Color(255, 255, 255, 50));

        CreateButton->addSubComponent(InputBoxRandom);
        CreateButton->addSubComponent(InputBoxInit);
        CreateButton->addSubComponent(LoadButton);
        CreateButton->addSubComponent(EmptyButton);

        CreateButton->setFunc([this, CreateButton,mDataStructure, info]()
        {
            int type = CreateButton->getSubComponentInfo().InfoID;

            if (type == -1) return;
            else if (type == 0)
            {
                int N = CreateButton->getSubComponentInfo().num;
                std::srand(std::time(nullptr)); 
                std::vector<int> randomList(N);
                for (int &num : randomList)
                    num = std::rand() % 100; // Random numbers from 0 to 99

                if (!mDataStructure->loadFromVector(randomList))
                    info->setText("!Maximum size is 50.");
            }
            else if (type == 1)
            {
                std::vector<int> list = CreateButton->getSubComponentInfo().VecNum;
                bool valid = true;
                for (int x: list) if (x < 0 || x > 99) valid = false;
                if (valid)
                {
                    if (!mDataStructure->loadFromVector(list))
                        info->setText("!Maximum size is 50.");
                }
                else
                    info->setText("!All numbers in the list must be from 0 to 99.");
            }
            else if (type == 2)
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
                        bool valid = true;
                            for (int x: numsFromFile) if (x < 0 || x > 99) valid = false;
                            if (valid)
                            {
                                if (!mDataStructure->loadFromVector(numsFromFile))
                                    info->setText("!Maximum size is 50.");
                            }
                            else
                                info->setText("!All numbers in the list must be from 0 to 99.");
                    }
                    fin.close();
                } 
                else {
                    std::cerr << "No file selected or an error occurred." << std::endl;
                }
            }
            else if (type == 3)
                mDataStructure->empty();

            CreateButton->resetSubComponentInfo();
        });

        // add update button
        GUI::ExpandableButton::Ptr UpdateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[4], "Update", ButtonSize);
        GUI::TextBox::Ptr InputBoxUpdate = std::make_shared<GUI::TextBox>(mFont, sf::Vector2f((UI::OPERATIONBOX::Size.x + ButtonSize.x - TextBoxSize.x) / 2.f, (OperationButtonPosition[2].y + OperationButtonPosition[3].y - TextBoxSize.y) / 2), TextBoxSize, "old =, new =", GUI::TextBox::InputType::VectorNum);

        InputBoxUpdate->setCallback([this, UpdateButton, InputBoxUpdate]()
        {
            UpdateButton->setSubComponentInfo(InputBoxUpdate->getInputNumList(),0);
        });

        UpdateButton->addSubComponent(InputBoxUpdate);

        UpdateButton->setFunc([this,UpdateButton,mDataStructure, info]()
        {
            if (!mDataStructure->isRunning())
            {
                std::vector<int> nums = UpdateButton->getSubComponentInfo().VecNum;
                int ActionType = UpdateButton->getSubComponentInfo().InfoID;
                if (ActionType == -1) return;
                if (ActionType == 0)
                {
                    if (nums.size() != 2)
                        info->setText("!Exactly 2 numbers are required.");
                    else if (nums[0] < 0 || nums[0] > 99 || nums[1] < 0 || nums[1] > 99)
                        info->setText("!Invalid input.");
                    else
                    {
                        info->setText("");
                        mDataStructure->updateValue(nums[0], nums[1]);
                    }
                }
            }
            UpdateButton->resetSubComponentInfo();
        });

        OperationButtonsList->pack(CreateButton);
        OperationButtonsList->pack(UpdateButton);
    }

    if (mode == World::Mode::AVLMode)
    {
        auto avl = dynamic_cast<AVLTree*>(mDataStructure);
        initAVLButtons(avl, info);
	}
    else if (mode == World::Mode::LinkedListMode)
    {
        auto ll = static_cast<LinkedList*>(mDataStructure);
        initLinkedListButtons(ll, info);
    }
    else if (mode == World::Mode::HeapMode)
    {
        auto heap = static_cast<HeapTree*>(mDataStructure);
        initHeapButtons(heap, info);
    }
    else if (mode == World::Mode::GraphMode)
    {
        auto g = static_cast<Graph*>(mDataStructure);
        initGraphButtons(g, info);
    }
}

void MainUI::handleEvent(const sf::Event& event)
{
    OperationButtonsList->handleEvent(event);
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