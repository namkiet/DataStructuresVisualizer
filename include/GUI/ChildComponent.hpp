#pragma once
#include <memory>
#include<GUI/Component.hpp>

namespace GUI
{
    class ExpandableButton;
    class ChildComponent
    {
        public:
        typedef std::shared_ptr<ChildComponent> Ptr;
        private:
        std::shared_ptr<ExpandableButton> ButtonParent;
        int                               mInfoID;
        
        public:
        std::shared_ptr<Component>        Child;
        public:
        ChildComponent();
        void                                        setChild(GUI::Component::Ptr component);
        void                                        setInfoID(int id);
        void                                        setButtonParent(std::shared_ptr<ExpandableButton> Button);
    };
    
}