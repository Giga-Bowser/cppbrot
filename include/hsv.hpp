#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

sf::Color hsv(long double hue, long double sat, long double val) {
	hue = fmod(hue, 360);
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = (int)hue / 60;
	long double f = hue / 60 - h;
	long double p = val * (1.f - sat);
	long double q = val * (1.f - sat * f);
	long double t = val * (1.f - sat * (1 - f));

	switch (h) {
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}

sf::Color hsv2(long double hue) {
	hue = fmod(hue, 360);
	while (hue < 0) hue += 360;

	int h = (int)hue / 60;
	long double f = hue / 60 - h;
	long double q = 1 - f;

	switch (h) {
	default:
	case 0:
	case 6: return sf::Color(255, f * 255, 0);
	case 1: return sf::Color(q * 255, 255, 0);
	case 2: return sf::Color(0, 255, f * 255);
	case 3: return sf::Color(0, q * 255, 255);
	case 4: return sf::Color(f * 255, 0, 255);
	case 5: return sf::Color(255, 0, q * 255);
	}
}
