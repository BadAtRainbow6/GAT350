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

    void Brightness(std::vector<color_t>& buffer, int brightness)
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

    void Noise(std::vector<color_t>& buffer, int noise)
    {
        int offset = (rand() % ((noise * 2) + 1)) - noise;

        std::for_each(buffer.begin(), buffer.end(), [offset](auto& color)
            {
                color.r = static_cast<uint8_t>(Clamp(color.r + offset, 0, 255));
                color.g = static_cast<uint8_t>(Clamp(color.g + offset, 0, 255));
                color.b = static_cast<uint8_t>(Clamp(color.b + offset, 0, 255));
            });
    }

    void Threshold(std::vector<color_t>& buffer, int threshold)
    {
        std::for_each(buffer.begin(), buffer.end(), [threshold](auto& color)
            {
                uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

                if (average >= threshold)
                {
                    color.r = 255;
                    color.g = 255;
                    color.b = 255;
                }
                else 
                {
                    color.r = 0;
                    color.g = 0;
                    color.b = 0;
                }
            });
    }

    void BoxBlur(std::vector<color_t>& buffer, int w, int h)
    {
        std::vector<color_t> source(buffer);

        int k[3][3]
        {
            { 1, 1, 1 },
            { 1, 1, 1 },
            { 1, 1, 1 }
        };

        for (int i = 0; i < buffer.size(); i++)
        {
            int x = i % w;
            int y = i / w;

            // skip if out of range
            if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;
            
            int r = 0;
            int g = 0;
            int b = 0;

            for (int iy = 0; iy < 3; iy++)
            {
                for (int ix = 0; ix < 3; ix++)
                {
                    color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
                    int weight = k[iy][ix];

                    r += pixel.r * weight;
                    g += pixel.g * weight;
                    b += pixel.b * weight;
                }
            }

            color_t& color = buffer[i];
            color.r = static_cast<uint8_t>(r / 9);
            color.g = static_cast<uint8_t>(r / 9);
            color.b = static_cast<uint8_t>(r / 9);
        }
    }

    void Edge(std::vector<color_t>& buffer, int w, int h, int threshold)
    {
        std::vector<color_t> source(buffer);

        int hk[3][3]
        {
            { 1, 0, -1 },
            { 2, 0, -2 },
            { 1, 0, -1 }
        };

        int vk[3][3]
        {
            { -1, -2, -1 },
            { 0, 0, 0 },
            { 1, 2, 1 }
        };

        for (int i = 0; i < buffer.size(); i++)
        {
            int x = i % w;
            int y = i / w;

            // skip if out of range
            if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

            int h = 0;
            int v = 0;

            for (int iy = 0; iy < 3; iy++)
            {
                for (int ix = 0; ix < 3; ix++)
                {
                    color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
                    h += pixel.r * hk[iy][ix];
                    v += pixel.r * vk[iy][ix];
                }
            }

            int m = std::sqrt(h * h + v * v);

            m = (m >= threshold) ? m : 0;

            uint8_t c = std::clamp(m, 0, 255);

            color_t& color = buffer[i];
            color.r = c;
            color.g = c;
            color.b = c;
        }
    }
}