#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

using namespace std;
using namespace sf;

const int noOfSnowflakes = 300;
const float radius = 5.F;
const float frame = 1.F / 60;

float getRandom(int num1, int num2) {
	float min = static_cast<float>(num1);
	float max = static_cast<float>(num2);
	static random_device randomDevice;
	static mt19937 generator(randomDevice());
	uniform_real_distribution<float> distribution(min, max);

	return (distribution(generator));
}// Method takes two numbers and gets random int between them

float shake(float time) {
	return time < 0.5 ? 2 * time * time : 2 * time * (2 - time) - 1;
}// Algorithm makes snowflake shake

class SnowFlake
{
public:
	Vector2f position;
	float start;
	float change{ getRandom(1, 10) };
	float duration{ getRandom(1, 5) };
	float time{ 0 };
	CircleShape snowflake{ radius / duration };

	explicit SnowFlake(Vector2f startPosition) : position{ startPosition }
	{
		snowflake.setPosition(position);
		start = startPosition.x;
	}// Constructor

	void fall(Vector2u windowSize) {
		time += frame;
		if (time > duration) {
			start += change;
			change = getRandom(1, 15);
			time = 0.F;
		}
		position.y = snowflake.getPosition().y;
		position.y += 1 / duration;
		if (position.y > windowSize.y)
		{
			position.y = -10.F;
			start = getRandom(0, windowSize.x);
			change = getRandom(1, 10);
			time = 0;
			snowflake.setRadius(radius / duration);
		}
		position.x = moveDown();
		snowflake.setPosition(position);
	}// fall method
	void draw(RenderWindow& window) { window.draw(snowflake); }//Draws snowflake

	float moveDown() {
		return start + change * shake(time / duration);
	}//Moves snowflake down
};//SnowFlake Class

class Pterodactyl {
public:
	Vector2f position;
	float start;
	Texture tex_pterodactyl;
	Sprite spr_pterodactyl;

	explicit Pterodactyl(Vector2f startPosition) : position{ startPosition }
	{
		tex_pterodactyl.loadFromFile("pterodactyl.png");
		spr_pterodactyl.setTexture(tex_pterodactyl);
		spr_pterodactyl.setPosition(position);
		spr_pterodactyl.scale(Vector2f(.4f, .4f));
	}// Constructor

	void flyFromLeft(Vector2u windowSize) {
		position.x = spr_pterodactyl.getPosition().x;
		position.y = spr_pterodactyl.getPosition().y;
		position.x -= 3;
		if (position.x > (windowSize.x / 2)) {
			position.y += 0.4;
			spr_pterodactyl.rotate(.2);
		}
		else { 
			position.y -= 0.8; 	
		}
		spr_pterodactyl.setPosition(position);
	}//fly from left of screen
	void flyFromRight(Vector2u windowSize) {
		spr_pterodactyl.setScale({ -.4,.4 });
		position.x = spr_pterodactyl.getPosition().x;
		position.y = spr_pterodactyl.getPosition().y;
		position.x += 3;
		if (position.x < (windowSize.x / 2)) {
			position.y += 0.4;
			spr_pterodactyl.rotate(.02);
		}
		else {
			position.y -= 0.8;
		}
		spr_pterodactyl.setPosition(position);
	}//Fly from right
};// Pterodactyl Class


int main() {
	RenderWindow window{ VideoMode{600, 800}, "Happy Christmas" };
	window.setFramerateLimit(60);

	// Texture Set-up
	Texture tex_background, tex_snowman, tex_pterodactyl, tex_george;
	tex_background.loadFromFile("background.jpg");
	tex_snowman.loadFromFile("snowman.png");
	tex_pterodactyl.loadFromFile("pterodactyl.png");
	tex_george.loadFromFile("georgeBush.png");

	//Sprite setup
	Sprite spr_background, spr_snowman, spr_pterodactyl, spr_george;
	spr_background.setTexture(tex_background);
	spr_snowman.setTexture(tex_snowman);
	spr_george.setTexture(tex_george);

	spr_snowman.scale(Vector2f(.2f, .2f));
	spr_snowman.setPosition(Vector2f(250.0f, 450.0f));
	spr_george.setPosition(450, 100);
	
	//create two pterodactyl objects
	Pterodactyl newPter1(Vector2f(600.0f, 100.0f));
	Pterodactyl newPter2(Vector2f(-900.0f, 100.0f));
	
	vector<SnowFlake> snowFlakes;

	for (int i = 0; i < noOfSnowflakes; i++) {
		float x = getRandom(0, window.getSize().x);
		float y = getRandom(0, window.getSize().y);
		Vector2f position(x, y);
		snowFlakes.emplace_back(SnowFlake(position));
	}// Adds snowflakes to vector


	int loop = 0;

	while (window.isOpen()) {
		loop++;
		Event newEvent;
		while (window.pollEvent(newEvent)) {
			if (newEvent.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		for (auto&& snow : snowFlakes) {
			snow.fall(window.getSize());
		}// Makes each snowflake fall

		newPter1.flyFromLeft(window.getSize());
		if (loop>250){
			newPter2.flyFromRight(window.getSize());
		}// Creates delay between flys

		if (loop > 1000 && loop < 1200) {
			float georgePosition = spr_george.getPosition().x;
			if (spr_george.getPosition().x>-100)
			{
				georgePosition-=5;
				spr_george.setPosition(georgePosition, 100);
			}
		}// Moves Georgy onto Screen
		if (loop > 1200) {
			float georgePosition = spr_george.getPosition().x;
			if (spr_george.getPosition().x < 700)
			{
				georgePosition += 5;
				spr_george.setPosition(georgePosition, 100);
			}
		}// Moves Georgy off Screen

		window.clear();

		// Drawing sprites to screen
		window.draw(spr_background);
		window.draw(spr_snowman);
		for (auto&& snow : snowFlakes) {
			snow.draw(window);
		}// for each snowball, draw snowball
		
		// Drawing Sprites
		window.draw(newPter1.spr_pterodactyl);
		window.draw(newPter2.spr_pterodactyl);
		window.draw(spr_george);
		
		window.display();
	}//Main loop
	return 0;
}//Main
