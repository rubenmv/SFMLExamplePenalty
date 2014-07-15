/* 
 * File:   main.cpp
 * Author: Ruben Martinez Vilar
 *
 * Created on 05 February 2013, 17:24
 */
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#define kBallSpeed 250
#define kPlayerSpeed 200

int main() {
	// Dimensiones de la ventana
    const int width = 640;
    const int height = 480;
    // Vectores de direccion de la bola
    sf::Vector2i direction(1, 1);
    // Contadores de vida y puntos de rebote
    int vidas = 3;
    int puntos = 0;
    // 0 = Fin de juego, 1 = Jugando
    int gameState = 1;

    sf::RenderWindow window(sf::VideoMode(width, height), "Practica 0: Pong");
    window.setFramerateLimit(60);

    sf::Font letra;

    // Tipografias
    if(!letra.loadFromFile("assets/emulogic.ttf")) {
        std::cerr << "Error cargando la tipografia emulogic.ttf";
        return EXIT_FAILURE;
    }

    // Imagen de la barra
    sf::Texture texPala;
    if (!texPala.loadFromFile("assets/portero.png")) {
        std::cerr << "Error cargando la imagen portero.png";
        return EXIT_FAILURE;
    }

    // Imagen de la bola
    sf::Texture texBola;
    if (!texBola.loadFromFile("assets/bola.png")) {
        std::cerr << "Error cargando la imagen bola.png";
        return EXIT_FAILURE;
    }

    // Creamos los sprites
    sf::Sprite sprPala(texPala);
    sf::Sprite sprBola(texBola);

	// Guardamos las dimensiones de los objetos pala y bola
	float palaW = sprPala.getGlobalBounds().width;
	float palaH = sprPala.getGlobalBounds().height;
	float bolaW = sprBola.getGlobalBounds().width;
	float bolaH = sprBola.getGlobalBounds().height;
	
	// Ponemos el centroide en el centro de los sprites
    sprPala.setOrigin(palaW / 2, palaH / 2);
    sprBola.setOrigin(bolaW / 2, bolaH / 2);

    // Lo dispongo en la mitad inferior de la pantalla
    sprPala.setPosition(width/2, height - 50);
    sprBola.setPosition(width/2, -50);

    // Texto del titulo del juego
    sf::Text titulo("PONG!", letra, 30);
	titulo.setOrigin(titulo.getGlobalBounds().width / 2, titulo.getGlobalBounds().height / 2);
    titulo.setPosition(width / 2, 40); // Texto centrado en x
    titulo.setColor(sf::Color(50, 50, 50));
    
    // Texto de informacion de vidas
    sf::Text textoVidas("Vidas restantes", letra, 8);
	textoVidas.setOrigin(0, textoVidas.getGlobalBounds().height / 2);
    textoVidas.setPosition(10, 50);
    textoVidas.setColor(sf::Color(40, 40, 40));
    // Texto de contador de vidas (Diferente Text para utilizar otro tamanyo de fuente)
    sf::Text contVidas("3", letra, 20);
	contVidas.setOrigin(0, contVidas.getGlobalBounds().height / 2);
    contVidas.setPosition(textoVidas.getPosition().x, textoVidas.getPosition().y + 20);
    contVidas.setColor(sf::Color(40, 40, 40));
    
    // Texto de informacion de puntos
    sf::Text textoPuntos("Puntos", letra, 8);
	textoPuntos.setOrigin(0, textoPuntos.getGlobalBounds().height / 2);
    textoPuntos.setPosition(width - textoPuntos.getGlobalBounds().width - 50, 50);
    textoPuntos.setColor(sf::Color(40, 40, 40));
    // Texto de contador de puntos (Diferente Text para utilizar otro tamanyo de fuente)
    sf::Text contPuntos("0", letra, 20);
	contPuntos.setOrigin(0, contPuntos.getGlobalBounds().height / 2);
    contPuntos.setPosition(textoPuntos.getPosition().x, textoPuntos.getPosition().y + 20);
    contPuntos.setColor(sf::Color(40, 40, 40));
    
    // Texto de fin de juego
    sf::Text textoFin("FIN DE JUEGO\n\nESPACIO para reiniciar\nESCAPE para salir", letra, 20);
	textoFin.setOrigin(textoFin.getGlobalBounds().width / 2, textoFin.getGlobalBounds().height / 2);
    textoFin.setPosition(width / 2, height / 2); // Texto centrado en x e y
    textoFin.setColor(sf::Color(40, 40, 40));

    // Inicia el reloj
    sf::Clock clock;

    // Bucle del juego
    while (window.isOpen()) {
		
		sf::Time elapsedTime = clock.restart();
        window.clear(sf::Color(165, 217, 163));
        window.draw(titulo);
        window.draw(textoVidas);
        window.draw(textoPuntos);
		window.draw(contVidas);
        window.draw(contPuntos);
        window.draw(sprPala);
        window.draw(sprBola);
        
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
								sprPala.setPosition(width/2, height - 50);
								// Reiniciamos la posicion de la bola
								sprBola.setPosition(width/2, -50);
							}
                        break;
                    }
                // Fin CASE
            }
        }
        
        // JUEGO TERMINADO
        if(gameState == 0) {
            window.draw(textoFin);
        }
        // JUEGO EN MARCHA
        else {
			// Movimiento del jugador. De esta manera se consigue un movimiento suave
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
					(sprPala.getPosition().x - texPala.getSize().x/2) > 0) {
				sprPala.move(-kPlayerSpeed * elapsedTime.asSeconds(), 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
					(sprPala.getPosition().x + texPala.getSize().x/2) < width) {
				sprPala.move(kPlayerSpeed * elapsedTime.asSeconds(), 0);
			}
			
            // Movimiento de la bola
            sprBola.move(kBallSpeed * direction.x * elapsedTime.asSeconds(), kBallSpeed * direction.y * elapsedTime.asSeconds());

            // Cambio de direccion de la bola
            // x
            if(sprBola.getPosition().x - bolaW / 2 <= 0) {
                direction.x = 1;
            }
            else if(sprBola.getPosition().x + bolaW / 2 >= width) {
                direction.x = -1;
            }
            // y
            if(sprBola.getPosition().y - bolaH / 2 <= 0) {
                direction.y = 1;
            }
            // La bola sale por la parte inferior de la pantalla
            else if(sprBola.getPosition().y >= height) {
                // Restamos una vida
                vidas--;
                if(vidas == 0) gameState = 0;
				else puntos = 0;
                // Reiniciamos la posicion de la bola
                sprBola.setPosition(width/2, -50);
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
				
        window.display();
    }
    
    return EXIT_SUCCESS;
}