#include "GUI/MainUI.hpp"
#include "Core/World.hpp"
#include <memory>
#include <DataStructures/LinkedList.hpp>
#include <DataStructures/Graph.hpp>
#include "GUI/ChildComponent.hpp"
#include <GUI/TextBox.hpp>
#include <Core/Variables.hpp>
#include <cstdlib>
#include <ctime>

MainUI::MainUI(TextureHolder& textures, FontHolder& fonts)
{
    BackRequest = false;
    mFont = fonts.get(Fonts::ID::Main);
    OperationButtonsList = std::make_shared<GUI::Container>();
    // Background.setSize(window.getView().getSize());
    // Background.setFillColor(sf::Color(37, 50, 56));
    // Background.setPosition(0.f,0.f);

    ToolBox.setSize(UI::TOOLBOX::Size);
    ToolBox.setFillColor(UI::TOOLBOX::Color);
    ToolBox.setPosition(UI::TOOLBOX::Position);

    sf::Vector2f ToolboxPosition = ToolBox.getPosition();
    sf::Vector2f ToolBoxSize = ToolBox.getSize();

    mSeperateToolBoxLine.resize(3);
    mSeperateToolBoxLine[0] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[0][0].position = sf::Vector2f(ToolboxPosition.x, ToolboxPosition.y + 0.4 * ToolBoxSize.y);
    mSeperateToolBoxLine[0][1].position = sf::Vector2f(ToolBoxSize.x + ToolboxPosition.x , ToolboxPosition.y + 0.4 * ToolBoxSize.y);
    
    mSeperateToolBoxLine[1] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[1][0].position = sf::Vector2f(ToolboxPosition.x, ToolboxPosition.y + 0.5 * ToolBoxSize.y);
    mSeperateToolBoxLine[1][1].position = sf::Vector2f(ToolBoxSize.x + ToolboxPosition.x , ToolboxPosition.y + 0.5 * ToolBoxSize.y);

    mSeperateToolBoxLine[2] = sf::VertexArray(sf::Lines, 2);
    mSeperateToolBoxLine[2][0].position = sf::Vector2f(ToolboxPosition.x, ToolboxPosition.y + 0.85 * ToolBoxSize.y);
    mSeperateToolBoxLine[2][1].position = sf::Vector2f(ToolBoxSize.x + ToolboxPosition.x , ToolboxPosition.y + 0.85 * ToolBoxSize.y);

    OperationBox.setSize(sf::Vector2f(ToolBoxSize.x * 0.4, 0.35 * ToolBoxSize.y));
    OperationBox.setFillColor(UI::OperationBoxColor);
    OperationBox.setPosition(mSeperateToolBoxLine[1][0].position);

    OperationButtonPosition.resize(5);
    OperationButtonPosition[0] = sf::Vector2f(OperationBox.getPosition());
    OperationButtonPosition[1] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.2);
    OperationButtonPosition[2] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.4);
    OperationButtonPosition[3] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.6);
    OperationButtonPosition[4] = sf::Vector2f(OperationBox.getPosition().x, OperationBox.getPosition().y + OperationBox.getSize().y * 0.8);

    sf::Sprite HomeSprite;
    HomeSprite.setTexture(textures.get(Textures::HomeIcon));
    BackButtons = std::make_shared<GUI::Button>(mFont, sf::Vector2f(20.f,20.f), "", sf::Vector2f(50.f,50.f), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
    BackButtons->setSprite(HomeSprite);
}

void MainUI::updateCurrent(sf::Time dt)
{
}

void MainUI::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    // target.draw(Background);
    target.draw(ToolBox, states);
    target.draw(OperationBox, states);
    target.draw(*OperationButtonsList, states);
    target.draw(*BackButtons, states);
    
    for(auto &lines: mSeperateToolBoxLine)
        target.draw(lines, states);
}

void MainUI::initAVLButtons(AVLTree* avl)
{
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);
    // Add Create button
    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);
    GUI::Button::Ptr RandomButton = std::make_shared<GUI::Button>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y), "Random", sf::Vector2f(100.f,40.f));
    RandomButton->setCallback([CreateButton]() {
        CreateButton->setSubComponentInfo(0);
    });
    CreateButton->addSubComponent(RandomButton);
    CreateButton->setFunc([CreateButton,avl]()
    {
        if (CreateButton->getSubComponentInfo().InfoID == -1) 
            return;

        if (CreateButton->getSubComponentInfo().InfoID == 0) // RANDOM
        {
            std::srand(std::time(nullptr)); 
            std::vector<int> randomList(10);
            for (int &num : randomList)
                num = std::rand() % 100; // Random numbers from 0 to 99
            
            avl->loadFromVector(randomList);
        }   

        CreateButton->resetSubComponentInfo();
    });

    // Add Insert button
    GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);
    GUI::TextBox::Ptr InputBoxInsert = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));
    InputBoxInsert->setCallback([InsertButton, InputBoxInsert]() {
        InsertButton->setSubComponentInfo(InputBoxInsert->getInputNum(),0);
    });
    InsertButton->addSubComponent(InputBoxInsert);
    InsertButton->setFunc([this,InsertButton, avl]()
    {
        int ActionType = InsertButton->getSubComponentInfo().InfoID;
        int num = InsertButton->getSubComponentInfo().num;
        if (ActionType == -1) return;

        if (ActionType == 0)
            avl->insert(num);

        InsertButton->resetSubComponentInfo();
    });

    // Add Delete button
    GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
    GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));
    InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete](){
        DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
    });
    DeleteButton->addSubComponent(InputBoxDelete);
    DeleteButton->setFunc([this,DeleteButton, avl](){
        int num = DeleteButton->getSubComponentInfo().num;
        int ActionType = DeleteButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        else if (ActionType == 0)
            avl->remove(num);

        DeleteButton->resetSubComponentInfo();
    });

    // Add Search button
    GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
    GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));
    InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch]() {
        SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
    });
    SearchButton->addSubComponent(InputBoxSearch);
    SearchButton->setFunc([this,SearchButton,avl]()
    {
        int num = SearchButton->getSubComponentInfo().num;
        int ActionType = SearchButton->getSubComponentInfo().InfoID;
        if (ActionType == -1) return;
        if (ActionType == 0)
        {
            avl->search(num);
        }
        SearchButton->resetSubComponentInfo();
    });

    

    // Pack all buttons
    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(InsertButton);
    OperationButtonsList->pack(DeleteButton);
    OperationButtonsList->pack(SearchButton);
}

void MainUI::createButtonList(World::Mode mode, DS* mDataStructure)
{
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);
    OperationButtonsList->makeEmpty();

    if (mode == World::Mode::AVLMode)
    {
        auto avl = dynamic_cast<AVLTree*>(mDataStructure);
        initAVLButtons(avl);
	}
    else if (mode == World::Mode::LinkedListMode)
    {
        auto mLinkedList = static_cast<LinkedList*>(mDataStructure);

        GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

        GUI::Button::Ptr RandomButton = std::make_shared<GUI::Button>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y), "Random", sf::Vector2f(100.f,40.f));
        CreateButton->addSubComponent(RandomButton);
        RandomButton->setCallback([CreateButton]()
        {
            CreateButton->setSubComponentInfo(0);
        });

        CreateButton->setFunc([CreateButton,mLinkedList]()
        {
            if (CreateButton->getSubComponentInfo().InfoID == -1) return;

            if (CreateButton->getSubComponentInfo().InfoID == 0) // RANDOM
            {
                std::srand(std::time(nullptr)); 
                std::vector<int> randomList(10);
                for (int &num : randomList)
                    num = std::rand() % 100; // Random numbers from 0 to 99
                
                mLinkedList->loadFromVector(randomList);
            }   
        });

        GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);

        // initialize the textbox
        GUI::TextBox::Ptr InputBoxInsertAtHead = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[1].y) , sf::Vector2f(100.f, 40.f), 24.f, "Insert at Head");
        // attach it to its parent
        InsertButton->addSubComponent(InputBoxInsertAtHead);
        // set callback for the textbox
        InputBoxInsertAtHead->setCallback([InsertButton, InputBoxInsertAtHead]()
        {
            InsertButton->setSubComponentInfo(InputBoxInsertAtHead->getInputNum(),0);
        });

        GUI::TextBox::Ptr InputBoxInsertAtLast = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[3].y) , sf::Vector2f(100.f, 40.f), 24.f, "Insert at Tail");

        InsertButton->addSubComponent(InputBoxInsertAtLast);

        InputBoxInsertAtLast->setCallback([InsertButton, InputBoxInsertAtLast]()
        {
            InsertButton->setSubComponentInfo(InputBoxInsertAtLast->getInputNum(),1 );
        });


        InsertButton->setFunc([this,InsertButton, mLinkedList]()
        {
            int ActionType = InsertButton->getSubComponentInfo().InfoID;
            int num = InsertButton->getSubComponentInfo().num;
            if (ActionType == -1) return;

            if (ActionType == 0) // InsertAtHead
            {
                mLinkedList->InsertAtHead(num);
            }
            else if (ActionType == 1) //insertAtLast
            {
                mLinkedList->InsertAtLast(num);
            }
            InsertButton->resetSubComponentInfo();
        });

        GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
        GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

        DeleteButton->addSubComponent(InputBoxDelete);
        InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete]()
        {
            DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
        });
        DeleteButton->setFunc([this,DeleteButton, mLinkedList]()
        {
            int num = DeleteButton->getSubComponentInfo().num;
            int ActionType = DeleteButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
            {
                mLinkedList->remove(num);
            }
            
            DeleteButton->resetSubComponentInfo();
        });

        GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
        GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

        SearchButton->addSubComponent(InputBoxSearch);
        InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch]()
        {
            SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
        });

        SearchButton->setFunc([this,SearchButton,mLinkedList]()
        {
            int num = SearchButton->getSubComponentInfo().num;
            int ActionType = SearchButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            if (ActionType == 0)
            {
                mLinkedList->search(num);
            }
            SearchButton->resetSubComponentInfo();
        });

        OperationButtonsList->pack(CreateButton);
        OperationButtonsList->pack(InsertButton);
        OperationButtonsList->pack(DeleteButton);
        OperationButtonsList->pack(SearchButton);
    }

    else if (mode == World::Mode::HeapMode)
    {
        auto mHeap = static_cast<HeapTree*>(mDataStructure);

        // Add Create button
        GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);
        {    
            GUI::Button::Ptr RandomButton = std::make_shared<GUI::Button>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y), "Random", sf::Vector2f(100.f,40.f));
            RandomButton->setCallback([CreateButton]()
            {
                CreateButton->setSubComponentInfo(0);
            });
            
            CreateButton->addSubComponent(RandomButton);
            CreateButton->setFunc([CreateButton,mHeap]()
            {
                if (CreateButton->getSubComponentInfo().InfoID == -1) 
                    return;
    
                if (CreateButton->getSubComponentInfo().InfoID == 0) // RANDOM
                {
                    std::srand(std::time(nullptr)); 
                    std::vector<int> randomList(10);
                    for (int &num : randomList)
                        num = std::rand() % 100; // Random numbers from 0 to 99
                    
                    mHeap->loadFromVector(randomList);
                }   
    
                CreateButton->resetSubComponentInfo();
            });
        }

        // Add Insert button
        GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);

        {        
            GUI::TextBox::Ptr InputBoxInsert = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));
            InputBoxInsert->setCallback([InsertButton, InputBoxInsert]()
            {
                InsertButton->setSubComponentInfo(InputBoxInsert->getInputNum(),0);
            });
            
            InsertButton->addSubComponent(InputBoxInsert);
            InsertButton->setFunc([this,InsertButton, mHeap]()
            {
                int ActionType = InsertButton->getSubComponentInfo().InfoID;
                int num = InsertButton->getSubComponentInfo().num;
                if (ActionType == -1) return;

                if (ActionType == 0) // InsertAtHead
                {
                    mHeap->insert(num);
                }
                InsertButton->resetSubComponentInfo();
            });
        }

        // Add Delete button
        GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
            
        {
            GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(ToolBox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));
            InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete]()
            {
                DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
            });

            DeleteButton->addSubComponent(InputBoxDelete);
            DeleteButton->setFunc([this,DeleteButton, mHeap]()
            {
                int num = DeleteButton->getSubComponentInfo().num;
                int ActionType = DeleteButton->getSubComponentInfo().InfoID;
                if (ActionType == -1) return;
                else if (ActionType == 0)
                {
                    mHeap->remove(num);
                }
                
                DeleteButton->resetSubComponentInfo();
            });
        }


        // Pack all buttons
        OperationButtonsList->pack(CreateButton);
        OperationButtonsList->pack(InsertButton);
        OperationButtonsList->pack(DeleteButton);
    }

    else if (mode == World::Mode::GraphMode)
{
    auto mGraph = static_cast<Graph*>(mDataStructure);

    GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

    GUI::Button::Ptr RandomButton = std::make_shared<GUI::Button>(
        mFont,
        sf::Vector2f(ToolBox.getSize().x * 0.55, OperationButtonPosition[2].y),
        "Random",
        sf::Vector2f(100.f, 40.f)
    );

    RandomButton->setCallback([CreateButton]() {
        CreateButton->setSubComponentInfo(0);
    });

    CreateButton->addSubComponent(RandomButton);

    CreateButton->setFunc([CreateButton, mGraph]() {
        if (CreateButton->getSubComponentInfo().InfoID == -1) return;

        if (CreateButton->getSubComponentInfo().InfoID == 0) // RANDOM
        {
        }

        CreateButton->resetSubComponentInfo();
    });

    GUI::ExpandableButton::Ptr MSTButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "MST", ButtonSize);
    MSTButton->setCallback([MSTButton]{
        MSTButton->setSubComponentInfo(0);
    });
    MSTButton->setFunc([MSTButton, mGraph]() {
            int ActionType = MSTButton->getSubComponentInfo().InfoID;
            if (ActionType == -1) return;
            else if (ActionType == 0)
                {
                    mGraph->Prim();
                }
                
            MSTButton->resetSubComponentInfo();
            });

    OperationButtonsList->pack(CreateButton);
    OperationButtonsList->pack(MSTButton);
}
}

void MainUI::handleEvent(const sf::Event& event)
{
    OperationButtonsList->handleEvent(event);
    BackButtons->handleEvent(event);
    if (BackButtons->isActive())
    {
        BackRequest = true;
    }
}

bool MainUI::getBackRequest()
{
    return BackRequest;
}