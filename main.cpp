#include <cmath>
#include <numeric>
#include <execution>
#include <algorithm>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "include/hsv.hpp"

const uint16_t width = 1280;
const uint16_t height = 720;
const long double aspect = width / (long double)height;

const uint16_t maxIter = 200;
const uint16_t maxPeriod = (int)sqrt(maxIter);

sf::Image img;
sf::Texture text;

uint16_t iterCounts[height][width];
long double exitDists[height][width];

sf::Uint8 testImg[height * width * 4];

// number of iterations per pixel.
uint16_t nipp[maxIter];

uint64_t totalIter = 0;

void mandelbrot(long double xOffset, long double yOffset, long double range) {
	uint16_t columns[height];
	std::iota(columns, columns + height, 0);
	totalIter = 0;
	
	if (range > powl(2, -14)) {
		std::for_each(std::execution::par_unseq, columns, columns + height, [xOffset, yOffset, range](uint16_t py) {
			for (uint16_t px = 0; px < width; px++) {
				float x = 0, y = 0;
				float x2 = 0, y2 = 0;
				float xOld = 0, yOld = 0;
				float x0 = range * aspect * (px / (float)width - 0.5f) + xOffset;
				float y0 = range * (py / (float)height - 0.5f) + yOffset;

				uint16_t i = 0;
				uint16_t period = 0;

				// period 2 bulb
				if (x0 * x0 + x0 + x0 + y0 * y0 <= -0.9375f) i = maxIter;
				while (i < maxIter && x2 + y2 <= 16) {
					y = (x + x) * y + y0;
					x = x2 - y2 + x0;
					x2 = x * x;
					y2 = y * y;

					if (xOld == x && yOld == y) {
						i = maxIter;
						break;
					}

					period++;
					if (period > maxPeriod) {
						period = 0;
						xOld = x;
						yOld = y;
					}

					i++;
				}

				iterCounts[py][px] = i;
				exitDists[py][px] = x2 + y2;
				nipp[i]++;
				totalIter += i;
			}
		});
		
	} else if (range > powl(2, -43)) {
		std::for_each(std::execution::par_unseq, columns, columns + height, [xOffset, yOffset, range](uint16_t py) {
			for (uint16_t px = 0; px < width; px++) {
				double x = 0, y = 0;
				double x2 = 0, y2 = 0;
				double xOld = 0, yOld = 0;
				double x0 = (aspect * range * px / (double)width) + xOffset - (range * aspect / 2);
				double y0 = (range * py / (double)height) + yOffset - (range / 2);

				uint16_t i = 0;
				uint16_t period = 0;

				// period 2 bulb
				if (x0 * x0 + x0 + x0 + y0 * y0 <= -0.9375) i = maxIter;
				while (i < maxIter && x2 + y2 <= 16) {
					y = (x + x) * y + y0;
					x = x2 - y2 + x0;
					x2 = x * x;
					y2 = y * y;

					if (xOld == x && yOld == y) {
						i = maxIter;
						break;
					}

					period++;
					if (period > maxPeriod) {
						period = 0;
						xOld = x;
						yOld = y;
					}

					i++;
				}

				iterCounts[py][px] = i;
				exitDists[py][px] = x2 + y2;
				nipp[i]++;
				totalIter += i;
			}
		});
	} else {
		std::for_each(std::execution::par_unseq, columns, columns + height, [xOffset, yOffset, range](uint16_t py) {
			for (uint16_t px = 0; px < width; px++) {
				long double x = 0, y = 0;
				long double x2 = 0, y2 = 0;
				long double xOld = 0, yOld = 0;
				long double x0 = (aspect * range * px / (long double)width) + xOffset - (range * aspect / 2);
				long double y0 = (range * py / (long double)height) + yOffset - (range / 2);

				uint16_t i = 0;
				uint16_t period = 0;

				// period 2 bulb
				if (x0 * x0 + x0 + x0 + y0 * y0 <= -0.9375l) i = maxIter;
				while (i < maxIter && x2 + y2 <= 16) {
					y = (x + x) * y + y0;
					x = x2 - y2 + x0;
					x2 = x * x;
					y2 = y * y;

					if (xOld == x && yOld == y) {
						i = maxIter;
						break;
					}

					period++;
					if (period > maxPeriod) {
						period = 0;
						xOld = x;
						yOld = y;
					}

					i++;
				}

				iterCounts[py][px] = i;
				exitDists[py][px] = x2 + y2;
				nipp[i]++;
				totalIter += i;
			}
		});
	}

	std::for_each(std::execution::par_unseq, columns, columns + height, [](uint16_t py) {
		for (uint16_t px = 0; px < width; px++) {
			uint16_t iter = iterCounts[py][px];
			if (iter == maxIter) {
				img.setPixel(px, py, sf::Color::Black);
			} else {
				double pixcol = 0;
				for (uint16_t i = 0; i <= iter; i++) {
					pixcol += nipp[i] / (double)totalIter;
				}
				double iterSmooth = (pixcol + iter) + 1 - log(log(abs(exitDists[py][px]))) / log(2);
				img.setPixel(px, py, hsv(3 * iterSmooth, 1, 1));
				// testImg[4 * (py * width + px) + 0] = col.r;
				// testImg[4 * (py * width + px) + 1] = col.g;
				// testImg[4 * (py * width + px) + 2] = col.b;
				// testImg[4 * (py * width + px) + 3] = 255;
			}
		}
	});
	text.update(img);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot");

	img.create(width, height, testImg);
	text.create(width, height);

	long double range = 2;
	long double xOffset = -0.75;
	long double yOffset = 0;
	
	mandelbrot(xOffset, yOffset, range);
	window.draw(sf::Sprite(text));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			// else if (event.type == sf::Event::MouseWheelScrolled) { 
			// 	range *= pow(1.1, event.mouseWheelScroll.delta);
			// 	xOffset += range * aspect * (sf::Mouse::getPosition(window).x / (long double)width - 0.5);
			// 	yOffset += range * (sf::Mouse::getPosition(window).y / (long double)height - 0.5);
			// 	mandelbrot(xOffset, yOffset, range);
			// }
		}

		if (window.hasFocus()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				range /= 2;
				xOffset = range * aspect * (sf::Mouse::getPosition(window).x / (long double)width - 0.5l) + xOffset;
				yOffset = range * (sf::Mouse::getPosition(window).y / (long double)height - 0.5l) + yOffset;
				mandelbrot(xOffset, yOffset, range);
			} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				xOffset -= range * aspect * (sf::Mouse::getPosition(window).x / (long double)width - 0.5);
				yOffset -= range * (sf::Mouse::getPosition(window).y / (long double)height - 0.5);
				range *= 2;
				mandelbrot(xOffset, yOffset, range);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				range = 2;
				xOffset = -0.75;
				yOffset = 0;
				mandelbrot(xOffset, yOffset, range);
			}
			window.draw(sf::Sprite(text));
		}

		window.display();
	}

	return 0;
}