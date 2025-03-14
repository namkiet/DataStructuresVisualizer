
#pragma once
#include <vector>
#include "GUI/Component.hpp"
#include <memory>

namespace GUI
{

class Container : public Component
{
    public:
        typedef std::shared_ptr<Container> Ptr;
            

	public:
							Container();

        void				pack(Component::Ptr component);

        virtual bool		isSelectable() const; // false
        virtual void		handleEvent(const sf::Event& event);
        int                 getSize();
        void                makeEmpty();
        void                ChangeActivateChild(std::size_t index);

    private:
        virtual void		draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool				hasSelection() const;
        void				select(std::size_t index);


        bool                hasMode();


    private:
        std::vector<Component::Ptr>		mChildren;
        int								mSelectedChild;
        int                             mActivateChild;
};

}


