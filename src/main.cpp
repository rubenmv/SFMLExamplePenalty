/* 
 * File:   main.cpp
 * Author: Ruben Martinez Vilar
 *
 * Created on 05 February 2013, 17:24
 */
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define kBallSpeed 0.35
#define kPlayerSpeed 0.3
//Window dimensions
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int FPS_LIMIT = 60;
const int LIFE = 3;

//Some globals
sf::RenderWindow window;
sf::Font letra;

sf::Texture texPala;
sf::Sprite sprPala;
sf::Texture texBola;
sf::Sprite sprBola;

sf::Text titulo;
sf::Text contVidas;
sf::Text textoPuntos;
sf::Text contPuntos;
sf::Text textoFin;

float palaW;
float palaH;
float bolaW;
float bolaH;


//Creates and initializes the SFML window and other stuff
bool initWindow() {
	int windowStyle = sf::Style::Titlebar;
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Example: Penalty", windowStyle);
    window.setFramerateLimit(FPS_LIMIT);
	window.setVerticalSyncEnabled(true);
    return true;
}

//Loads fonts, sprites, etc.
bool loadResources() {
	// Tipografias
    if(!letra.loadFromFile("assets/emulogic.ttf")) {
        std::cerr << "Error loading emulogic.ttf" << std::endl;
        return false;
    }
	
	//TEXTURES FOR SPRITES
    if (!texPala.loadFromFile("assets/portero.png")) {
        std::cerr << "Error loading portero.png" << std::endl;
        return false;
    }
    if (!texBola.loadFromFile("assets/bola.png")) {
        std::cerr << "Error loading bola.png" << std::endl;
        return false;
    }

    //SPRITES
	sprPala.setTexture(texPala);
    sprBola.setTexture(texBola);

	// Guardamos las dimensiones de los objetos pala y bola
	palaW = sprPala.getGlobalBounds().width;
	palaH = sprPala.getGlobalBounds().height;
	bolaW = sprBola.getGlobalBounds().width;
	bolaH = sprBola.getGlobalBounds().height;
	
	// Ponemos el centroide en el centro de los sprites
    sprPala.setOrigin(palaW / 2, palaH / 2);
    sprBola.setOrigin(bolaW / 2, bolaH / 2);

    // Lo dispongo en la mitad inferior de la pantalla
    sprPala.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 50);
    sprBola.setPosition(WINDOW_WIDTH/2, -50);
	
	//TEXT
	sf::Color defaultColor = sf::Color(40, 40, 40);
	// Texto del titulo del juego
    titulo.setFont(letra);
	titulo.setCharacterSize(30);
	titulo.setString("PENALTY!");
	titulo.setOrigin(titulo.getGlobalBounds().width / 2, titulo.getGlobalBounds().height / 2);
    titulo.setPosition(WINDOW_WIDTH / 2, 40); // Texto centrado en x
    titulo.setColor(sf::Color(50, 50, 50));
    // Texto de contador de vidas (Diferente Text para utilizar otro tamanyo de fuente)
	contVidas.setFont(letra);
	contVidas.setCharacterSize(20);
	contVidas.setString("3");
	contVidas.setOrigin(0, contVidas.getGlobalBounds().height / 2);
    contVidas.setPosition(20, 20);
    contVidas.setColor(defaultColor);
    // Texto de informacion de puntos
	textoPuntos.setFont(letra);
	textoPuntos.setCharacterSize(8);
	textoPuntos.setString("Points");
	textoPuntos.setOrigin(0, textoPuntos.getGlobalBounds().height / 2);
    textoPuntos.setPosition(WINDOW_WIDTH - textoPuntos.getGlobalBounds().width - 50, 50);
    textoPuntos.setColor(defaultColor);
    // Texto de contador de puntos (Diferente Text para utilizar otro tamanyo de fuente)
	contPuntos.setFont(letra);
	contPuntos.setCharacterSize(20);
	contPuntos.setString("0");
	contPuntos.setOrigin(0, contPuntos.getGlobalBounds().height / 2);
    contPuntos.setPosition(textoPuntos.getPosition().x, textoPuntos.getPosition().y + 20);
    contPuntos.setColor(defaultColor);
    // Texto de fin de juego
	textoFin.setFont(letra);
	textoFin.setCharacterSize(20);
	textoFin.setString("GAME OVER\n\nR to restart\nESC to exit");
	textoFin.setOrigin(textoFin.getGlobalBounds().width / 2, textoFin.getGlobalBounds().height / 2);
    textoFin.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // Texto centrado en x e y
    textoFin.setColor(defaultColor);
	
    return true;
}

int main() {
    
    // Vectores de direccion de la bola
    sf::Vector2i direction(1, 1);
    // Contadores de vida y puntos de rebote
    int vidas = 3;
    int puntos = 0;
    // 0 = Fin de juego, 1 = Jugando
    int gameState = 1;
	
	if(!initWindow()) {
		std::cerr << "initWindow(): there was a problem creating the window" << std::endl;
	}

    loadResources();
	

    // Inicia el reloj
    sf::Clock clock;
    sf::Int32 deltaTime = 0.f;

    // Bucle del juego
    while (window.isOpen()) {
        deltaTime = clock.restart().asMilliseconds();
        
        sf::Event event;
        // Bucle de obtención de eventos de ventana
        while (window.pollEvent(event)) {
            switch(event.type) {
                // Si se recibe el evento de cerrar la ventana la cierro
                case sf::Event::Closed:
                    window.close();
                break;
                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                        // Tecla ESC para salir  
                        case sf::Keyboard::Escape:
                            window.close();
                        break;
						
                        // Tecla ESPACIO para reiniciar (solo en fin de juego)
                        case sf::Keyboard::Space:
							if(gameState == 0) {
								vidas = 3;
								puntos = 0;
								gameState = 1;
								// Reiniciamos la posicion de la pala
								sprPala.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - 50);
								// Reiniciamos la posicion de la bola
								sprBola.setPosition(WINDOW_WIDTH/2, -50);
							}
                        break;
                    }
                // Fin CASE
            }
        }
        // JUEGO EN MARCHA
        if (gameState != 0) {
			// Movimiento del jugador. De esta manera se consigue un movimiento suave
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
					(sprPala.getPosition().x - texPala.getSize().x/2) > 0) {
				sprPala.move(-kPlayerSpeed * deltaTime, 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
					(sprPala.getPosition().x + texPala.getSize().x/2) < WINDOW_WIDTH) {
				sprPala.move(kPlayerSpeed * deltaTime, 0);
			}
			
            // Movimiento de la bola
            sprBola.move(kBallSpeed * direction.x * deltaTime, kBallSpeed * direction.y * deltaTime);

            // Cambio de direccion de la bola
            // x
            if(sprBola.getPosition().x - bolaW / 2 <= 0) {
                direction.x = 1;
            }
            else if(sprBola.getPosition().x + bolaW / 2 >= WINDOW_WIDTH) {
                direction.x = -1;
            }
            // y
            if(sprBola.getPosition().y - bolaH / 2 <= 0) {
                direction.y = 1;
            }
            // La bola sale por la parte inferior de la pantalla
            else if(sprBola.getPosition().y >= WINDOW_HEIGHT) {
                // Restamos una vida
                vidas--;
                if(vidas == 0) gameState = 0;
				else puntos = 0;
                // Reiniciamos la posicion de la bola
                sprBola.setPosition(WINDOW_WIDTH/2, -50);
            }
            // La bola llega al centro de la pala menos 5 pixeles
            else if	((sprBola.getPosition().y + bolaH / 2) >= (sprPala.getPosition().y - 5) &&
					(sprBola.getPosition().y + bolaH / 2) <= sprPala.getPosition().y + 10) {
                // Si esta tocando la pala en x
                if	((sprBola.getPosition().x >= sprPala.getPosition().x-texPala.getSize().x/2) &&
					(sprBola.getPosition().x <= sprPala.getPosition().x+texPala.getSize().x/2)) {
                    direction.y = -1;
                    puntos++;
                }
            }
        }
		
		// Actualizamos los contadores
        std::stringstream cadena;

		cadena << puntos;
		contPuntos.setString(cadena.str());
		cadena.str(std::string());// Limpia la cadena
		
		cadena << vidas;
		contVidas.setString(cadena.str());
		cadena.str(std::string());// Limpia la cadena
		
		window.clear(sf::Color(165, 217, 163));
        window.draw(titulo);
        window.draw(textoPuntos);
		window.draw(contVidas);
        window.draw(contPuntos);
        window.draw(sprPala);
        window.draw(sprBola);
		
		// JUEGO TERMINADO
        if(gameState == 0) {
            window.draw(textoFin);
        }
				
        window.display();
    }
    
    return EXIT_SUCCESS;
}