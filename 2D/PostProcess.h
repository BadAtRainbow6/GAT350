#pragma once
#include "Framebuffer.h"

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer);

	void Monochrome(std::vector<color_t>& buffer);

	void Brightness(std::vector<color_t>& buffer, int brightness);

	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo);

	void Noise(std::vector<color_t>& buffer, int noise);

	void Threshold(std::vector<color_t>& buffer, int threshold);

	void Posterize(std::vector<color_t>& buffer, int levels);



	void BoxBlur(std::vector<color_t>& buffer, int w, int h);
	void GaussianBlur(std::vector<color_t>& buffer, int w, int h);
	void Sharpen(std::vector<color_t>& buffer, int w, int h);
	void Edge(std::vector<color_t>& buffer, int w, int h, int threshold);
	void Emboss(std::vector<color_t>& buffer, int w, int h);
}
