//== File Information =========================================================
//= File: 	screen.hpp
//= Author:	Salepate
//= Version:	0.02
//= Updated:	01/11/11 16:34:27
//=
//== File Description =========================================================
//= Screen Handler (help manipulate screens)
//=
//== License ==================================================================
//= This program is free software: you can redistribute it and/or modify
//= it under the terms of the GNU General Public License as published by
//= the Free Software Foundation, either version 3 of the License, or
//= (at your option) any later version.
//=
//= This program is distributed in the hope that it will be useful,
//= but WITHOUT ANY WARRANTY; without even the implied warranty of
//= MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//= GNU General Public License for more details.
//=
//= You should have received a copy of the GNU General Public License
//= along with this program.  If not, see <http://www.gnu.org/licenses/>.
//=============================================================================

#ifndef __classScreen__
#define __classScreen__

#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

/** \class Screen
 *  \brief abstract class
 */
class Screen {
	public:
		/** different screen status.
		 * - update_alive tells the program to keep working on the current screen
		 * - update_die tells the program to kill the current screen, and pass
		 * onto the next one
		 */
		enum update_states { update_alive = 0, update_die };

		/** different screen layers
		 */
		enum layers {
			layer_default = 0,
			layer_hud,
			layer_count,
			layer_all
		};



		/** Constructor
		 */
		Screen();
		/** Destructor
		 */
		virtual ~Screen();

		/** 	defines child-reserved variables about screen size.
		 *	@param win -> Active window
		 */
		static void SetSize(sf::RenderWindow &win);

		/**	Get a reference to the screen list
		 */
		static std::vector<Screen*>& GetScreenList();


		// subroutine (dont touch!)
		void	__init();
		int	__update(sf::RenderWindow &win);
		void	__display(sf::RenderWindow &win);
		void	__catchEvent(sf::Event &e);

		/**
		 * Catches events
		 * @param e -> container for the triggered event
		 */
		virtual void GetEvent(sf::Event &e);

		/** Triggers a screen effect
		 * List of effects:
		 * - Coming soon
		 * @param layer -> on which layer
		 * @param sfx -> effect to be triggered
		 * @param delay -> when to start (0 = instant)
		 * @param length -> how long
		 */
		void Trigger(layers layer, int sfx, unsigned int delay, unsigned int length);
		/** Key struct
		*/
		class Key {
			public:
			inline Key() { pressed = down = event = 0; }
			int pressed, down, event;
		};

	protected:
		/** self-explicit, everything happens here.
		*/
		virtual int Update() = 0;

		/** allow children to initiate themselves
		  */
		virtual void Init();

		/** attach a drawable object to a layer
		 */
		void Attach( sf::Drawable&, layers = layers(0));

		/** return times.
		 *  @return time in (ms)
		 */
		static unsigned int GetTime();

		/** return screen size.
		 * @return integer vector
		 */
		static const sf::Vector2i& GetSize();

		/* Keyboard Input */
		std::map<sf::Keyboard::Key,Key>myKey;

	private:
		static sf::Clock _clock;
		static sf::Vector2i _size;
		static std::vector<Screen*>_screenList;

		/* <IN-DEV:Salepate */
		struct SFX;

		struct Layer {
			std::vector<sf::Drawable*>entities;
			SFX *sfx;
		};

		struct SFX {
			unsigned int start, end;
			sf::Shader shader;
		};
		
		std::map<layers, Layer*>_layers;
		std::vector<SFX*>_sfx;
		/* </IN-DEV> */

};

#endif
