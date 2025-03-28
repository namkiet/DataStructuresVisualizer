#include "GUI/MainUI.hpp"
#include "Core/World.hpp"
#include <memory>
#include <DataStructures/LinkedList.hpp>
#include "GUI/ChildComponent.hpp"
#include <GUI/TextBox.hpp>
GUI:: MainUI::MainUI(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts)
    {
        BackRequest = false;
        mFont = fonts.get(Fonts::ID::Main);
        OperationButtonsList = std::make_shared<GUI::Container>();
        Background.setSize(window.getView().getSize());
        Background.setFillColor(sf::Color(37, 50, 56));
        Background.setPosition(0.f,0.f);

        toolbox.setSize(sf::Vector2f(300.f,540.f));
        toolbox.setFillColor(sf::Color(31, 42, 47));
        toolbox.setPosition(0.f,(Background.getSize().y - toolbox.getSize().y)/2);

        sf::Vector2f ToolboxPosition = toolbox.getPosition();
        sf::Vector2f ToolBoxSize = toolbox.getSize();


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
        OperationBox.setFillColor(sf::Color(56, 71, 79));
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
void GUI::MainUI::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    
    target.draw(Background);
    target.draw(toolbox);
    for(auto& lines: mSeperateToolBoxLine){
        target.draw(lines);
    }
    target.draw(OperationBox);
    target.draw(*OperationButtonsList);
    target.draw(*BackButtons);
}

void GUI::MainUI::CreateButtonList(World::Mode mode, DS* mDataStructure){
    sf::Vector2f ButtonSize(OperationBox.getSize().x, OperationBox.getSize().y * 0.2);
        OperationButtonsList->makeEmpty();
        if(mode == World::Mode::AVLMode){
            GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

            GUI::Button::Ptr RandomButton = std::make_shared<GUI::Button>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y), "Random", sf::Vector2f(100.f,40.f));
            CreateButton->addSubComponent(RandomButton);
            RandomButton->setCallback([CreateButton](){
                CreateButton->setSubComponentInfo(0);
            });

            CreateButton->setFunc([CreateButton,mDataStructure](){
                if(CreateButton->getSubComponentInfo().InfoID == -1) return;

                if(CreateButton->getSubComponentInfo().InfoID == 0){
                    auto mAVL = dynamic_cast<AVLTree*>(mDataStructure);
                    mAVL->insert(9);
                    mAVL->insert(7);
                    mAVL->insert(2006);
                    std::cout<<"Insert 9 7 2006"<<std::endl;
                }

                CreateButton->resetSubComponentInfo();
            });

            
            GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);

            GUI::TextBox::Ptr InputBoxInsert = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

            InsertButton->addSubComponent(InputBoxInsert);

            InputBoxInsert->setCallback([InsertButton, InputBoxInsert](){
                InsertButton->setSubComponentInfo(InputBoxInsert->getInputNum(),0);
            });

            InsertButton->setFunc([this,InsertButton, mDataStructure](){
                int ActionType = InsertButton->getSubComponentInfo().InfoID;
                int num = InsertButton->getSubComponentInfo().num;
                if(ActionType == -1) return;

                if(ActionType == 0) // InsertAtHead
                {
                    auto mAVL = dynamic_cast<AVLTree*>(mDataStructure);
                    mAVL->insert(num);
                }
                InsertButton->resetSubComponentInfo();
            });


            GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
            GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

            DeleteButton->addSubComponent(InputBoxDelete);
            InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete](){
                DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
            });
            DeleteButton->setFunc([this,DeleteButton, mDataStructure](){
                auto mAVL = dynamic_cast<AVLTree*>(mDataStructure);
                int num = DeleteButton->getSubComponentInfo().num;
                int ActionType = DeleteButton->getSubComponentInfo().InfoID;
                if(ActionType == -1) return;
                else if(ActionType == 0){
                    mAVL->remove(num);
                }
                
                DeleteButton->resetSubComponentInfo();
            });


            GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
            GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

            SearchButton->addSubComponent(InputBoxSearch);
            InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch](){
                SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
            });

            SearchButton->setFunc([this,SearchButton,mDataStructure](){
                auto mAVL = dynamic_cast<AVLTree*>(mDataStructure);
                int num = SearchButton->getSubComponentInfo().num;
                int ActionType = SearchButton->getSubComponentInfo().InfoID;
                if(ActionType == -1) return;
                if(ActionType == 0){
                    mAVL->search(num);
                }
                SearchButton->resetSubComponentInfo();
            });

            OperationButtonsList->pack(CreateButton);
            OperationButtonsList->pack(InsertButton);
            OperationButtonsList->pack(DeleteButton);
            OperationButtonsList->pack(SearchButton);

	}
        else if(mode == World::Mode::LinkedListMode){
            mDataStructure = dynamic_cast<LinkedList*>(mDataStructure);

            GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);

            GUI::Button::Ptr RandomButton = std::make_shared<GUI::Button>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y), "Random", sf::Vector2f(100.f,40.f));
            CreateButton->addSubComponent(RandomButton);
            RandomButton->setCallback([CreateButton](){
                CreateButton->setSubComponentInfo(0);
            });

            CreateButton->setFunc([CreateButton,mDataStructure](){
                if(CreateButton->getSubComponentInfo().InfoID == -1) return;

                if(CreateButton->getSubComponentInfo().InfoID == 0){
                    auto mlinkedlist = dynamic_cast<LinkedList*>(mDataStructure);
                    mlinkedlist->InsertAtHead(9);
                    mlinkedlist->InsertAtHead(7);
                    mlinkedlist->InsertAtLast(2006);
                    CreateButton->resetSubComponentInfo();
                    std::cout<<"Insert 9 7 2006"<<std::endl;
                }
            });




            GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);

            // initialize the textbox
            GUI::TextBox::Ptr InputBoxInsertAtHead = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[1].y) , sf::Vector2f(100.f, 40.f));
            // attach it to its parent
            InsertButton->addSubComponent(InputBoxInsertAtHead);
            // set callback for the textbox
            InputBoxInsertAtHead->setCallback([InsertButton, InputBoxInsertAtHead](){
                InsertButton->setSubComponentInfo(InputBoxInsertAtHead->getInputNum(),0);
            });

            GUI::TextBox::Ptr InputBoxInsertAtLast = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[3].y) , sf::Vector2f(100.f, 40.f));

            InsertButton->addSubComponent(InputBoxInsertAtLast);

            InputBoxInsertAtLast->setCallback([InsertButton, InputBoxInsertAtLast](){
                InsertButton->setSubComponentInfo(InputBoxInsertAtLast->getInputNum(),1 );
            });


            InsertButton->setFunc([this,InsertButton, mDataStructure](){
                int ActionType = InsertButton->getSubComponentInfo().InfoID;
                int num = InsertButton->getSubComponentInfo().num;
                if(ActionType == -1) return;

                if(ActionType == 0) // InsertAtHead
                {
                    auto mlinkedlist = dynamic_cast<LinkedList*>(mDataStructure);
                    mlinkedlist->InsertAtHead(num);
                }
                else if(ActionType == 1) //insertAtLast
                {
                    auto mlinkedlist = dynamic_cast<LinkedList*>(mDataStructure);
                    mlinkedlist->InsertAtLast(num);
                }
                InsertButton->resetSubComponentInfo();


            });

            GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
            GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

            DeleteButton->addSubComponent(InputBoxDelete);
            InputBoxDelete->setCallback([this,DeleteButton, InputBoxDelete](){
                DeleteButton->setSubComponentInfo(InputBoxDelete->getInputNum(),0);
            });
            DeleteButton->setFunc([this,DeleteButton, mDataStructure](){
                auto mlinkedlist = dynamic_cast<LinkedList*>(mDataStructure);
                int num = DeleteButton->getSubComponentInfo().num;
                int ActionType = DeleteButton->getSubComponentInfo().InfoID;
                if(ActionType == -1) return;
                else if(ActionType == 0){
                    mlinkedlist->remove(num);
                }
                
                DeleteButton->resetSubComponentInfo();
            });

            GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
            GUI::TextBox::Ptr InputBoxSearch = std::make_shared<GUI::TextBox>(mFont,sf::Vector2f(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y) , sf::Vector2f(100.f, 40.f));

            SearchButton->addSubComponent(InputBoxSearch);
            InputBoxSearch->setCallback([this, SearchButton, InputBoxSearch](){
                SearchButton->setSubComponentInfo(InputBoxSearch->getInputNum(),0);
            });

            SearchButton->setFunc([this,SearchButton,mDataStructure](){
                auto mlinkedlist = dynamic_cast<LinkedList*>(mDataStructure);
                int num = SearchButton->getSubComponentInfo().num;
                int ActionType = SearchButton->getSubComponentInfo().InfoID;
                if(ActionType == -1) return;
                if(ActionType == 0){
                    mlinkedlist->search(num);
                }
                SearchButton->resetSubComponentInfo();
            });

            OperationButtonsList->pack(CreateButton);
            OperationButtonsList->pack(InsertButton);
            OperationButtonsList->pack(DeleteButton);
            OperationButtonsList->pack(SearchButton);
        }
}

void GUI::MainUI::handleEvent(const sf::Event& event){
    OperationButtonsList->handleEvent(event);
    BackButtons->handleEvent(event);
    if(BackButtons->isActive()){
        BackRequest = true;
    }
}

bool GUI::MainUI::getBackRequest(){
    return BackRequest;
}