#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		Title,
		AppBackground1,
		AppBackground2,
		AppBackground3,
		MenuBackground,
		HomeIcon,
		AVLThumbnail,
		HeapThumbnail,
		LLThumbnail,
		GraphThumbnail,
		Prev,
		Next,
		First,
		Last,
		Pause,
		Play,
		SettingIcon,
		NavigateLeft,
		NavigateRight,
		Minus,
		Plus,
		SelectedRadioButton,
		UnselectedRadioButton,
		VolumeOn,
		VolumeOff,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		UI,
		Node
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, 	Fonts::ID>		FontHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP