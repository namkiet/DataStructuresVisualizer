#include <GUI/ChildComponent.hpp>

namespace GUI
{
    ChildComponent::ChildComponent(){}
    void ChildComponent::setInfoID(int id){
    mInfoID = id;
    }

    void ChildComponent::setButtonParent(std::shared_ptr<ExpandableButton> Button){
        ButtonParent = Button;
    }
    void ChildComponent::setChild(GUI::Component::Ptr component){
        Child = component;
    }
}

/*
GUI::ChildComponent randonButton;
randomButotn.setButtonParent(InsertButton);
randomButton.setChild(Textbox);
randomButton->Child->setCallBack([this,ButtonParent](){
    ButtonParent->setSubComponentInfo(mInfo);
})

call back của button là truyền vào 1 biến true cho
*/