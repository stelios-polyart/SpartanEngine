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


///////////////////////////////////////////
// 1. Add profiling
// 2. Visual feedback <-- Rendering
// 3. Setup -> Update();
//     3a. Threading? (thread pool)
//     3b. SIMD?
//     3c. SoA
//     3d. 

namespace spartan
{
    struct ParticleData
    {
        uint32_t max_particle_count = 0;
        uint32_t alive_particle_count = 0;

        float* lifetimes = nullptr;
        float* normalized_lifetimes = nullptr;
        math::Vector3* positions = nullptr;
        math::Vector3* velocities = nullptr;
        Color* colors = nullptr;
        float* random_values = nullptr;

    private:
        void* backing_buffer = nullptr;

    public:
        ParticleData() = default;

        explicit ParticleData(uint32_t initial_max_particle_count)
        {
            Resize(initial_max_particle_count);
        }

        ~ParticleData()
        {
            std::free(backing_buffer);
        }

        void Resize(uint32_t new_max_particle_count)
        {
            if (new_max_particle_count == max_particle_count)
                return;

            size_t total_size =
                sizeof(float) * new_max_particle_count +
                sizeof(float) * new_max_particle_count +
                sizeof(math::Vector3) * new_max_particle_count +
                sizeof(math::Vector3) * new_max_particle_count +
                sizeof(Color) * new_max_particle_count +
                sizeof(float) * new_max_particle_count;

            char* new_buffer = static_cast<char*>(std::malloc(total_size));
            char* ptr = new_buffer;

            float* new_lifetimes = reinterpret_cast<float*>(ptr);
            ptr += sizeof(float) * new_max_particle_count;

            float* new_normalized_lifetimes = reinterpret_cast<float*>(ptr);
            ptr += sizeof(float) * new_max_particle_count;

            math::Vector3* new_positions = reinterpret_cast<math::Vector3*>(ptr);
            ptr += sizeof(math::Vector3) * new_max_particle_count;

            math::Vector3* new_velocities = reinterpret_cast<math::Vector3*>(ptr);
            ptr += sizeof(math::Vector3) * new_max_particle_count;

            Color* new_colors = reinterpret_cast<Color*>(ptr);
            ptr += sizeof(Color) * new_max_particle_count;

            float* new_random_values = reinterpret_cast<float*>(ptr);

            if (backing_buffer)
            {
                uint32_t copy_count = std::min(max_particle_count, new_max_particle_count);

                std::memcpy(new_lifetimes, lifetimes, sizeof(float) * copy_count);
                std::memcpy(new_normalized_lifetimes, normalized_lifetimes, sizeof(float) * copy_count);
                std::memcpy(new_positions, positions, sizeof(math::Vector3) * copy_count);
                std::memcpy(new_velocities, velocities, sizeof(math::Vector3) * copy_count);
                std::memcpy(new_colors, colors, sizeof(math::Vector4) * copy_count);
                std::memcpy(new_random_values, random_values, sizeof(float) * copy_count);

                std::free(backing_buffer);
            }

            backing_buffer = new_buffer;
            max_particle_count = new_max_particle_count;

            lifetimes = new_lifetimes;
            normalized_lifetimes = new_normalized_lifetimes;
            positions = new_positions;
            velocities = new_velocities;
            colors = new_colors;
            random_values = new_random_values;

            alive_particle_count = std::min(alive_particle_count, max_particle_count);
        }

        int32_t GetLastAliveIndex() const
        {
            return alive_particle_count - 1;
        }
    };
}
