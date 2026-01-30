/*
Copyright(c) 2015-2025 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

//= INCLUDES =================================
#include "../Rendering/Color.h"
//============================================

namespace spartan
{  
    enum ps_property_type
    {
        CONSTANT,
        RANGE,
        CURVE
    };

    struct ParticleFloat
    {
        ps_property_type type = ps_property_type::CONSTANT;

        float const_value = 0.0f;
        std::pair<float, float> range_value;

        float GetConstValue()
        {
            return const_value;
        }

        float GetRangedValue(float random_value)
        {
            return std::lerp(range_value.first, range_value.second, random_value);
        }
    };

    struct ParticleVector3
    {
        ps_property_type type = ps_property_type::CONSTANT;

        math::Vector3 const_value{ 0.0f, 0.0f, 0.0f };
        std::pair<math::Vector3, math::Vector3> range_value;

        math::Vector3 GetConstValue()
        {
            return const_value;
        }

        math::Vector3 GetRangedValue(float random_value)
        {
            return math::Vector3::Lerp(range_value.first, range_value.second, random_value);
        }
    };

    struct ParticleColor
    {
        ps_property_type type = ps_property_type::CONSTANT;
        Color const_value{ 1.0f, 1.0f, 1.0f, 1.0f };
        std::pair<Color, Color> range_value;
        Color GetConstValue()
        {
            return const_value;
        }
        Color GetRangedValue(float random_value)
        {
            return Color::Lerp(range_value.first, range_value.second, random_value);
        }
    };

}
