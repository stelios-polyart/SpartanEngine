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

//= INCLUDES ================================
#include "pch.h"
#include "Emitter.h"
#include "EmitterModule.h"
#include "../Rendering/Renderer.h"
//===========================================

namespace spartan
{
    Emitter::Emitter()
    {
        particle_data.Resize(1000);
    }
    Emitter::~Emitter()
    {
        for (EmitterModule* module : initialization_modules)
        {
            delete module; // replace
        }
        initialization_modules.clear();

        for (EmitterModule* module : update_modules)
        {
            delete module; // replace
        }
        update_modules.clear();
    }

    void Emitter::InitializeParticles(uint32_t start_index, uint32_t end_index)
    {
        for (EmitterModule* module : initialization_modules)
        {
            module->OnInitialize(&particle_data, start_index, end_index);
        }
    }

    void Emitter::Update(const double& delta_time)
    {
        const float delta_time_float = static_cast<float>(delta_time);

        for (uint32_t i = 0; i < particle_data.alive_particle_count; )
        {
            particle_data.normalized_lifetimes[i] += delta_time_float / particle_data.lifetimes[i];

            if (particle_data.normalized_lifetimes[i] >= 1.0f)
            {
                if (particle_data.alive_particle_count == 0)
                {
                    continue;
                }
                // Swap with the last alive particle
                const uint32_t last_alive_index       = particle_data.alive_particle_count - 1;
                particle_data.lifetimes[i]            = particle_data.lifetimes[last_alive_index];
                particle_data.normalized_lifetimes[i] = particle_data.normalized_lifetimes[last_alive_index];
                particle_data.positions[i]            = particle_data.positions[last_alive_index];
                particle_data.velocities[i]           = particle_data.velocities[last_alive_index];
                particle_data.colors[i]               = particle_data.colors[last_alive_index];
                particle_data.random_values[i]        = particle_data.random_values[last_alive_index];

                particle_data.normalized_lifetimes[last_alive_index] = 1.0f;

                // Decrease alive count
                --particle_data.alive_particle_count;
            }
            else
            {
                Renderer::DrawSphere(
                    particle_data.positions[i],
                    0.03f,
                    8,
                    particle_data.colors[i]
                );
                ++i;
            }
        }

        const uint32_t last_alive_index = particle_data.GetLastAliveIndex();

        for (EmitterModule* module : update_modules)
        {
            module->OnUpdate(&particle_data, delta_time);
        }

        const uint32_t max_spawn_count =  std::ceil( (float)delta_time * spawn_rate);
        const uint32_t start_index = last_alive_index + 1;
        const uint32_t end_index = std::min(start_index + max_spawn_count, particle_data.max_particle_count - 1);

        particle_data.alive_particle_count = end_index;

        InitializeParticles(start_index, end_index);
    }
}
