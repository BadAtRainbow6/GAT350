#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <cmath>

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](auto& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});
	}

	void Monochrome(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](auto& color)
			{
				uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

				color.r = average;
				color.g = average;
				color.b = average;
			});
	}
	void Brightness(std::vector<color_t>& buffer, int16_t brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](auto& color)
			{
				color.r = static_cast<uint8_t>(Clamp(color.r + brightness, 0, 255));
				color.g = static_cast<uint8_t>(Clamp(color.g + brightness, 0, 255));
				color.b = static_cast<uint8_t>(Clamp(color.b + brightness, 0, 255));
			});
	}
	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{
		std::for_each(buffer.begin(), buffer.end(), [ro, go, bo](auto& color)
			{
				color.r = static_cast<uint8_t>(Clamp(color.r + ro, 0, 255));
				color.g = static_cast<uint8_t>(Clamp(color.g + go, 0, 255));
				color.b = static_cast<uint8_t>(Clamp(color.b + bo, 0, 255));
			});
	}

	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{
		std::for_each(buffer.begin(), buffer.end(), [noise](auto& color)
			{
				int offsetR = (rand() % ((noise * 2) + 1)) - noise;
				int offsetG = (rand() % ((noise * 2) + 1)) - noise;
				int offsetB = (rand() % ((noise * 2) + 1)) - noise;

				color.r = static_cast<uint8_t>(Clamp(color.r + offsetR, 0, 255));
				color.g = static_cast<uint8_t>(Clamp(color.g + offsetG, 0, 255));
				color.b = static_cast<uint8_t>(Clamp(color.b + offsetB, 0, 255));
			});
	}

	void Threshold(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](auto& color)
			{
				uint8_t value = static_cast<uint8_t>((color.r + color.g + color.b) / 3);
				color.r = value >= threshold ? 255 : 0;
				color.g = value >= threshold ? 255 : 0;
				color.b = value >= threshold ? 255 : 0;
			});
	}

	void Posterize(std::vector<color_t>& buffer, uint8_t levels)
	{
		if (levels == 0) levels = 1;

		uint8_t level = 255 / levels; // calculate the number of colors per level
		std::for_each(buffer.begin(), buffer.end(), [level, levels](auto& color)
			{
				color.r = static_cast<uint8_t>((color.r / level) * level);
				color.g = static_cast<uint8_t>((color.g / level) * level);
				color.b = static_cast<uint8_t>((color.b / level) * level);
			});
	}

	void Alpha(std::vector<color_t>& buffer, uint8_t alpha)
	{
		std::for_each(buffer.begin(), buffer.end(), [alpha](auto& color)
			{
				color.a = alpha;
			});
	}
	void BoxBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source(buffer);

		int k[3][3] =
		{
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 }
		};
		for(int i = 0; i < buffer.size(); i++)
		{
			// % 5 : 1 2 3 4 5 6 7 8 9 10
			//     : 1 2 3 4 0 1 2 3 4  0
			int x = i % width;
			int y = i / width;

			//Skip if out of range
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++) 
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(r / 9);
			color.g = static_cast<uint8_t>(g / 9);
			color.b = static_cast<uint8_t>(b / 9);
		}

	}

	void GaussianBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source(buffer);

		int k[3][3] =
		{
			{ 1, 2, 1 },
			{ 2, 4, 2 },
			{ 1, 2, 1 }
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			// % 5 : 1 2 3 4 5 6 7 8 9 10
			//     : 1 2 3 4 0 1 2 3 4  0
			int x = i % width;
			int y = i / width;

			//Skip if out of range
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(r / 16);
			color.g = static_cast<uint8_t>(g / 16);
			color.b = static_cast<uint8_t>(b / 16);
		}

	}

	void Sharpen(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source(buffer);

		int k[3][3] =
		{
			{ 0, -1, 0 },
			{ -1, 5, -1 },
			{ 0, -1, 0 }
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			// % 5 : 1 2 3 4 5 6 7 8 9 10
			//     : 1 2 3 4 0 1 2 3 4  0
			int x = i % width;
			int y = i / width;

			//Skip if out of range
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
		}
	}

	void Edge(std::vector<color_t>& buffer, int width, int height, int threshhold)
	{
		std::vector<color_t> source(buffer);

		int hk[3][3] =
		{
			{ 1, 0, -1 },
			{ 2, 0, -2 },
			{ 1, 0, -1 }
		};
		int vk[3][3] =
		{
			{ -1, -2, -1 },
			{ 0, 0, 0 },
			{ 1, 2, 1 }
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			// % 5 : 1 2 3 4 5 6 7 8 9 10
			//     : 1 2 3 4 0 1 2 3 4  0
			int x = i % width;
			int y = i / width;

			//Skip if out of range
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int h = 0;
			int v = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					
					h += pixel.r * hk[iy][ix];
					v += pixel.r * vk[iy][ix];
				}
			}
			int m = static_cast<int>(std::sqrt(h * h + v * v));
			m = (m >= threshhold) ? m : 0;
			uint8_t c = Clamp(m, 0, 255);

			color_t& color = buffer[i];

			color.r = c;
			color.g = c;
			color.b = c;
		}
	}

	void Emboss(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source(buffer);

		int k[3][3] =
		{
			{ -2, -1, 0 },
			{ -1, 1, 1 },
			{ 0, 1, 2 }
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			// % 5 : 1 2 3 4 5 6 7 8 9 10
			//     : 1 2 3 4 0 1 2 3 4  0
			int x = i % width;
			int y = i / width;

			//Skip if out of range
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			// Add a bias to make sure the pixel values stay within 0-255 range
			r = std::min(std::max(r + 128, 0), 255);
			g = std::min(std::max(g + 128, 0), 255);
			b = std::min(std::max(b + 128, 0), 255);

			// Update the current pixel with the new embossed values
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(r);
			color.g = static_cast<uint8_t>(g);
			color.b = static_cast<uint8_t>(b);
		}

	}


}

