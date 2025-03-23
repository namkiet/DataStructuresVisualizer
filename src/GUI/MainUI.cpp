#include "GUI/MainUI.hpp"
#include "Core/World.hpp"
#include <memory>

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
        if(mode == World::Mode::AVL){
            GUI::ExpandableButton::Ptr CreateButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[0], "Create", ButtonSize);


            GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[1], "Insert",ButtonSize);
            // GUI::DeliverTextBox::Ptr InputBoxDelete = std::make_shared<GUI::DeliverTextBox>(mFont, inputBoxDeletePos, sf::Vector2f(100.f, 40.f));
            // InputBoxDelete->setButtonParent(DeleteButton);
            // DeleteButton->setCallback([this,DeleteButton](){
            // 	if(DeleteButton->getSubComponentInfo().num != -1){
            // 		this->mDataStructure->remove(DeleteButton->getSubComponentInfo().num);
            // 		DeleteButton->setSubComponentInfo(-1);
            // 		std::cout<<"Delete num"<<std::endl;
            // 	}
            // 	else if(DeleteButton->getSubComponentInfo().VecNum.size() != 0){
            // 		for(auto& element: DeleteButton->getSubComponentInfo().VecNum){
            // 			this->mDataStructure->remove(element);
            // 			std::cout<<"Delete list of num"<<std::endl;
            // 		}
            // 	}
            // });
            sf::Vector2f insertButtonSize(toolbox.getSize().x * 0.3,ButtonSize.y);
            sf::Vector2f inputBoxInsertPos(toolbox.getSize().x * 0.55,  OperationButtonPosition[2].y);
            GUI::DeliverTextBox::Ptr InputBoxInsert = std::make_shared<GUI::DeliverTextBox>(mFont,inputBoxInsertPos,insertButtonSize);
            InputBoxInsert->setButtonParent(InsertButton);
            InsertButton->setCallback([this,InsertButton,mDataStructure](){
            	if(InsertButton->getSubComponentInfo().num != -1) {
            		mDataStructure->insert(InsertButton->getSubComponentInfo().num);
            		InsertButton->setSubComponentInfo(-1);
        
            		std::cout<<"Insert ok"<<std::endl;

            	}

            	else if(InsertButton->getSubComponentInfo().VecNum.size() != 0){
            		for(auto& element: InsertButton->getSubComponentInfo().VecNum){
            			mDataStructure->insert(element);
            		}
            	}
            });

            GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[2], "Delete",ButtonSize);
            sf::Vector2f deleteButtonSize = insertButtonSize;
            sf::Vector2f inputBoxDeletePos = inputBoxInsertPos;
            GUI::DeliverTextBox::Ptr InputBoxDelete = std::make_shared<GUI::DeliverTextBox>(mFont,inputBoxDeletePos,deleteButtonSize);
            InputBoxDelete->setButtonParent(DeleteButton);
            DeleteButton->setCallback([this,DeleteButton,mDataStructure](){
            	if(DeleteButton->getSubComponentInfo().num != -1){
            		mDataStructure->remove(DeleteButton->getSubComponentInfo().num);
            		DeleteButton->setSubComponentInfo(-1);
            		std::cout<<"Delete num"<<std::endl;
            	}
            	else if(DeleteButton->getSubComponentInfo().VecNum.size() != 0){
            		for(auto& element: DeleteButton->getSubComponentInfo().VecNum){
            			mDataStructure->remove(element);
            			std::cout<<"Delete list of num"<<std::endl;
            		}
            	}
            });

            GUI::ExpandableButton::Ptr SearchButton = std::make_shared<GUI::ExpandableButton>(mFont, OperationButtonPosition[3], "Search",ButtonSize);
            // the same
            GUI::DeliverTextBox::Ptr InputBoxSearch = std::make_shared<GUI::DeliverTextBox>(mFont, inputBoxDeletePos, sf::Vector2f(100.f, 40.f));
            InputBoxSearch->setButtonParent(SearchButton);
            SearchButton->setCallback([this,SearchButton, mDataStructure](){
            	if(SearchButton->getSubComponentInfo().num != -1){
            		mDataStructure->search(SearchButton->getSubComponentInfo().num);
            		SearchButton->setSubComponentInfo(-1);
            		std::cout<<"Search num"<<std::endl;
            	}
            	else if(SearchButton->getSubComponentInfo().VecNum.size() != 0){
            		for(auto& element: SearchButton->getSubComponentInfo().VecNum){
            			mDataStructure->remove(element);
            			std::cout<<"Delete list of num"<<std::endl;
            		}
            	}
            });

            // Add search Button
            InsertButton->addSubComponent(InputBoxInsert);
            // DeleteButton->addSubComponent(InputBoxDelete);
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