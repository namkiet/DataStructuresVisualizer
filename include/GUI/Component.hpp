#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include<functional>
namespace GUI
{

class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
    public:
        typedef std::shared_ptr<Component> Ptr;

        typedef std::function<void()>		Callback;

	public:
							Component();
        virtual				~Component();

        virtual bool		isSelectable() const = 0;
		bool				isSelected() const;
        virtual void	    select();
        virtual void		deselect();

        virtual bool		isActive() const;
        virtual void		activate();
        virtual void		deactivate();

        virtual void		handleEvent(const sf::Event& event) = 0;
        void                setCallback(Callback func);

    public:
        Callback            mCallback;
    private:
        bool				mIsSelected;
        bool				mIsActive;
};

}

