// TronProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <queue>

class Apple {
private:
	sf::Vector2<int> location;

	sf::String file;
	sf::Image image;
	sf::Texture texture;
public:
	sf::Sprite sprite;
	Apple(sf::String F, int W, int H);
	void spawn(int W, int H);
	sf::Vector2<int> getLocation();
};

class Snake {
private:
	bool life;
	int dir = 0;
	int len = 1, maxlen;
	int head = 0, tail = 0;
	int w, h;
	int growingTime = 2000;
	int lastMoveTime = 0;
	float speed;
	float acceleration = 0.2;
	sf::Vector2<int>* location;
	std::queue<int> stomach;

	sf::String file;
	sf::Image image;
	sf::Texture texture;
public:
	sf::Sprite sprite;
	Snake(sf::String F, int W, int H);
	Snake(sf::String F, int W, int H, int X, int Y, int Dir, int Speed);
	bool move(int currentTime);
	void eat(Apple &apple, int eatingTime);
	void grow();
	bool update(int currentTime);
	int getLength();
	void increaseSpeed();
	sf::Vector2<int> getBodyPartLocation(int num);
	bool isAlive();
	void setDir(int D);
	int getDir();
	void setLife(bool Life);
	bool biteSnake(Snake &snake);
};

sf::Vector2<int> getMovement(int dir);
int getRandomNumber(int min, int max);

int main() {
	int w = 1920/6, h = 1080/6;
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(6 * w, 6 * h), "Snake", sf::Style::Fullscreen);

	//Field
	int changeColorTime = 800;
	int lastChangeTime;
	int backCol = 0;
	sf::Image field_image;
	field_image.loadFromFile("images/black.png");
	sf::Texture field_texture;
	field_texture.loadFromImage(field_image);
	sf::Sprite s_field;
	s_field.setTexture(field_texture);

	//Snake
	Snake worm("2_ramazan6.png", w, h, 0.20*w, 0, 2, 40);
	//Worm
	Snake snake("ramazan6.png", w, h, 0.8*w, h-1, 3, 40);

	/*
	//Apple
	Apple apple = Apple("apple.png", w, h);
	bool hasCrossed = apple.getLocation() == snake.getBodyPartLocation(0);
	while (hasCrossed) {
		apple = Apple("apple.png", w, h);
		hasCrossed = apple.getLocation() == snake.getBodyPartLocation(0);
	}
	*/

	//Settings
	sf::Clock clock;
	sf::Clock gameTimeClock;
	int gameTimeMillisec = 0;
	int score = snake.getLength() - 1;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (snake.isAlive()&&worm.isAlive()) {
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && ((snake.getLength() == 1) || (snake.getBodyPartLocation(0).y != snake.getBodyPartLocation(1).y))) {
					snake.setDir(0);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && ((snake.getLength() == 1) || (snake.getBodyPartLocation(0).y != snake.getBodyPartLocation(1).y))) {
					snake.setDir(1);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && ((snake.getLength() == 1) || (snake.getBodyPartLocation(0).x != snake.getBodyPartLocation(1).x))) {
					snake.setDir(2);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && ((snake.getLength() == 1) || (snake.getBodyPartLocation(0).x != snake.getBodyPartLocation(1).x))) {
					snake.setDir(3);
				}
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && ((worm.getLength() == 1) || (worm.getBodyPartLocation(0).y != worm.getBodyPartLocation(1).y))) {
					worm.setDir(0);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && ((worm.getLength() == 1) || (worm.getBodyPartLocation(0).y != worm.getBodyPartLocation(1).y))) {
					worm.setDir(1);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && ((worm.getLength() == 1) || (worm.getBodyPartLocation(0).x != worm.getBodyPartLocation(1).x))) {
					worm.setDir(2);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && ((worm.getLength() == 1) || (worm.getBodyPartLocation(0).x != worm.getBodyPartLocation(1).x))) {
					worm.setDir(3);
				}
			}
		}
		//Updating snake
		if (snake.isAlive()&&worm.isAlive()) {
			gameTimeMillisec = gameTimeClock.getElapsedTime().asMilliseconds();
			snake.setLife(snake.update(gameTimeMillisec)&&worm.biteSnake(snake));
			worm.setLife(worm.update(gameTimeMillisec)&&snake.biteSnake(worm));
			gameTimeMillisec = gameTimeClock.getElapsedTime().asMilliseconds();
			//snake.eat(apple, gameTimeMillisec);
			score = snake.getLength() - 1;
		}

		//Drawing
		window.clear();
		if (snake.isAlive()&&worm.isAlive()) {
			//Drawing map
			/*
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					s_field.setPosition(j * 32, i * 32);
					//Drawing cell
					window.draw(s_field);
				}
			}
			*/
			
			//window.draw(apple.sprite);

			for (int i = 0; i < snake.getLength(); i++) {
				snake.sprite.setPosition(snake.getBodyPartLocation(i).x * 6, snake.getBodyPartLocation(i).y * 6);
				window.draw(snake.sprite);
			}
			for (int i = 0; i < worm.getLength(); i++) {
				worm.sprite.setPosition(worm.getBodyPartLocation(i).x * 6, worm.getBodyPartLocation(i).y * 6);
				window.draw(worm.sprite);
			}
			lastChangeTime = gameTimeMillisec;
		}
		else {
			if (lastChangeTime + changeColorTime < gameTimeClock.getElapsedTime().asMilliseconds()) {
				backCol = (backCol + 1) % 3;
				lastChangeTime = gameTimeClock.getElapsedTime().asMilliseconds();
				if (worm.isAlive()) {
					field_image.loadFromFile("images/2_ramazan6.png");
				}
				else {
					field_image.loadFromFile("images/ramazan6.png");
				}
				field_texture.loadFromImage(field_image);
				s_field.setTexture(field_texture);
			}
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					s_field.setPosition(j * 6, i * 6);
					//Drawing cell
					window.draw(s_field);
				}
			}
		}
		window.display();
		//std::cout << gameTimeMillisec / 1000.;
	}
	return 0;
}

Snake::Snake(sf::String F, int W, int H) {
	life = true;
	w = W, h = H;
	maxlen = w * h;
	location = new sf::Vector2<int>[maxlen];
	int xHead, yHead;
	xHead = getRandomNumber(0, W - 1);
	yHead = getRandomNumber(0, H - 1);
	location[head] = sf::Vector2<int>(xHead, yHead);
	file = F;
	image.loadFromFile("images/" + file);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 6, 6));
}
Snake::Snake(sf::String F, int W, int H, int X, int Y, int Dir, int Speed) {
	life = true;
	speed = Speed;
	w = W, h = H;
	dir = Dir;
	maxlen = w * h;
	location = new sf::Vector2<int>[maxlen];
	location[head] = sf::Vector2<int>(X, Y);
	file = F;
	image.loadFromFile("images/" + file);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 6, 6));
}
bool Snake::move(int currentTime) {
	location[(head + 1 + maxlen) % maxlen] = location[head] + getMovement(dir);
	head = (head + 1) % maxlen;
	location[head].x = (location[head].x + w) % w;
	location[head].y = (location[head].y + h) % h;
	tail = (tail + 1) % maxlen;
	lastMoveTime = currentTime;
	bool bite = false;
	for (int i = 3; i < len; i++) {
		bite = bite || (getBodyPartLocation(0) == getBodyPartLocation(i));
	}
	std::cout << "moving\n";
	//Tron addition
	grow();
	//
	return !bite;
}
void Snake::eat(Apple &apple, int eatingTime) {
	if (apple.getLocation() == getBodyPartLocation(0)) {
		stomach.push(eatingTime);
		increaseSpeed();
		std::cout << "EATING!\n";
		bool hasCrossed = true;
		while (hasCrossed) {
			std::cout << "respawn\n";
			apple = Apple("apple.png", w, h);
			hasCrossed = false;
			for (int i = 0; i < getLength(); i++) {
				hasCrossed = hasCrossed || (apple.getLocation() == getBodyPartLocation(i));
			}
		}
	}
	return;
}
void Snake::grow() {
	tail = (tail + maxlen - 1) % maxlen;
	len++;
	std::cout << "growing\n";
	return;
}
bool Snake::update(int currentTime) {
	if (!stomach.empty()) {
		if (stomach.front() + growingTime >= currentTime) {
			grow();
			std::cout << !stomach.empty();
			stomach.pop();
		}
	}
	if ((currentTime - lastMoveTime) >= 1000 / speed) {
		return move(currentTime);
	}
	else return true;
}
int Snake::getLength() {
	return len;
}
sf::Vector2<int> Snake::getBodyPartLocation(int num) {
	if ((0 < num) && (num < len)) {
		return location[(head - num + maxlen) % maxlen];
	}
	else {
		return location[head];
	}
}
void Snake::increaseSpeed() {
	speed += acceleration;
	std::cout << "acceleration";
	return;
}
bool Snake::isAlive() {
	return life;
}
void Snake::setDir(int D) {
	dir = D;
}
int Snake::getDir() {
	return dir;
}
void Snake::setLife(bool Life) {
	life = Life;
	return;
}
bool Snake::biteSnake(Snake &snake) {
	bool bite = false;
	for (int i = 0; i < snake.getLength(); i++) {
		bite = bite || (snake.getBodyPartLocation(i) == getBodyPartLocation(0));
	}
	return !bite;
}

Apple::Apple(sf::String F, int W, int H) {
	spawn(W, H);
	file = F;
	image.loadFromFile("images/" + file);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}
void Apple::spawn(int W, int H) {
	location = sf::Vector2<int>(getRandomNumber(0, W - 1), getRandomNumber(0, H - 1));
	sprite.setPosition(32 * location.x, 32 * location.y);
	return;
}
sf::Vector2<int> Apple::getLocation() {
	return location;
}

int getRandomNumber(int min, int max) {
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

sf::Vector2<int> getMovement(int dir) {
	switch (dir) {
		case (0): return sf::Vector2<int>(1, 0); break;
		case (1): return sf::Vector2<int>(-1, 0); break;
		case (2): return sf::Vector2<int>(0, 1); break;
		default: return sf::Vector2<int>(0, -1); break;
	}
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
