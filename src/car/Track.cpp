#include "Track.hpp"
// Rad to Deg -> * 57.2957795f

Track::Track(b2World* world) {
	this->world = world;
	
	float width = 50.0f;
	float height = 200.0f;
	float length = sqrt(((width / 2) * (width / 2)) + ((height / 2) * (height / 2)));
	float heightLengthAngle = acos((height / 2) / length);
	
	// TODO: Possibly read track composition from a file?
	std::vector<std::tuple<float, b2Vec2>> track = {
		std::make_tuple(0.f, b2Vec2(50, 0)), std::make_tuple(0.f, b2Vec2(100, 0)),
		std::make_tuple(0.f, b2Vec2(150, 0)), std::make_tuple(0.f, b2Vec2(200, 0)),
		std::make_tuple(0.f, b2Vec2(250, 0)), std::make_tuple(0.f, b2Vec2(300, 0)),
		std::make_tuple(0.f, b2Vec2(350, 0)), std::make_tuple(0.f, b2Vec2(400, 0)),
		std::make_tuple(0.f, b2Vec2(450, 0)), std::make_tuple(0.f, b2Vec2(500, 0)),
		std::make_tuple(22.5f, b2Vec2(525, -25)), std::make_tuple(45.0f, b2Vec2(550, -50))
	};
	
	sf::Vector2f lastBottomRight;
	
	std::vector<sf::VertexArray> sectors;
	for (auto & element : track) {
		float angle = std::get<0>(element);
		b2Vec2 midPoint = std::get<1>(element);
		newSector(width, height, angle, midPoint);
		
		float bottomLeftYOffset = cos(angle * DEGTORAD - heightLengthAngle) * length;
		float bottomLeftXOffset = sin(angle * DEGTORAD - heightLengthAngle) * length;
		float topRightYOffset = -bottomLeftYOffset;
		float topRightXOffset = -bottomLeftXOffset;
		float topLeftYOffset = sin((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
		float topLeftXOffset = cos((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
		float bottomRightYOffset = -topLeftYOffset;
		float bottomRightXOffset = -topLeftXOffset;
		
		sf::VertexArray sector(sf::LinesStrip, 5);
		sector[0].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
		sector[1].position = sf::Vector2f(midPoint.x + topRightXOffset, midPoint.y + topRightYOffset);
		sector[2].position = sf::Vector2f(midPoint.x + bottomRightXOffset, midPoint.y + bottomRightYOffset);
		sector[3].position = sf::Vector2f(midPoint.x + bottomLeftXOffset, midPoint.y + bottomLeftYOffset);
		sector[4].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
		lastBottomRight = sector[2].position;
		sector[0].color = sf::Color::Blue;
		sector[1].color = sf::Color::Blue;
		sector[2].color = sf::Color::Blue;
		sector[3].color = sf::Color::Blue;
		sector[4].color = sf::Color::Blue;
		
		sectors.push_back(sector);
	}
	
	GUI(sectors); // Return?
}

Track::~Track() {

}

void Track::GUI(std::vector<sf::VertexArray> sectors) {
	
	// Experiment area
	
	sf::Texture sector;
	unsigned int x = 50;
	unsigned int y = 200;
	sector.create(x, y);
	
	sf::Uint8* pixels = new sf::Uint8[x * y * 4];
	int j = 0;
	for (unsigned i = 0; i < x * y * 4; i += 4) {
		if (j < 255) {
			j++;
		} else {
			j = 0;
		}
    	pixels[i] = 0;
    	pixels[i + 1] = 0;
    	pixels[i + 2] = j;
    	pixels[i + 3] = 255;
	}
	sector.update(pixels);
	
	sf::Sprite sprite;
	sprite.setTexture(sector);
	
	// Actual code
	
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "qlearning2");
	window.setVerticalSyncEnabled(true);

	sf::RectangleShape car(sf::Vector2f(40, 30));
	car.setOrigin(20, 15);
	car.setFillColor(sf::Color(255, 55, 55));

	sf::View camera;
	camera.setSize(sf::Vector2f(1000, 1000));

	while (window.isOpen()) {
		sf::Event event;
		//std::cout << "Drawing a frame" << std::endl;
		// TEMPOARY BUBBLEGUM CODE, ERASE LATER *******



		// BUBBLEGUM CODE ENDS ************************
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::Resized) {
				// Update the camera to the new window size.
				camera.setSize(sf::Vector2f(event.size.width, event.size.height));
			}
		}

		window.clear(sf::Color::Black);
		car.move(1, 0);
		camera.setCenter(car.getPosition());
		window.setView(camera);
		for (auto x : sectors) {
			window.draw(x);
		}
		window.draw(car);
		window.draw(sprite);
		window.display();
		
	}
}

// This method creates a track part that the physics engine can utilize. The method takes the width, height, angle and middle point of the track part as parameters.
void Track::newSector(float width, float height, float angle, b2Vec2 middlePoint) {
	b2BodyDef bd;
	bd.position.Set(middlePoint.x, middlePoint.y);
	
	b2Body* trackPart = world->CreateBody(&bd);

	b2EdgeShape shape;
	b2FixtureDef checkpoints;
	checkpoints.shape = &shape;
	checkpoints.isSensor = true;


	// Left vertical
	shape.Set(b2Vec2(-width, -height), b2Vec2(-width, height));
	trackPart->CreateFixture(&checkpoints);

	// Right vertical
	shape.Set(b2Vec2(width, -height), b2Vec2(width, height));
	trackPart->CreateFixture(&checkpoints);

	b2FixtureDef walls;
	walls.shape = &shape;

	walls.isSensor = false;

	// Top horizontal
	shape.Set(b2Vec2(-width, height), b2Vec2(width, height));
	trackPart->CreateFixture(&walls);

	// Bottom horizontal
	shape.Set(b2Vec2(-width, -height), b2Vec2(width, -height));

	trackPart->CreateFixture(&walls);
	trackPart->SetTransform(middlePoint, angle * DEGTORAD);
	circuit.push_back(trackPart);
}
