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
#include "Component.h"
#include <vector>
//============================================

namespace spartan
{
    class Emitter;

    //enum particle_modifier_t : uint8_t
    //{
    //    PARTICLE_MODIFIER_velocity = 0b00000001,
    //    PARTICLE_MODIFIER_color    = 0b00000010,
    //    PARTICLE_MODIFIER_size     = 0b00000100,
    //    PARTICLE_MODIFIER_newType  = 0b00001000,
    //};

    //struct particle_modifier_data_t
    //{
    //    float*  value;
    //    uint8_t count;
    //};

    //struct particle_data_t
    //{
    //    particle_modifier_data_t* modifiers;
    //    particle_modifier_t*      modifier_types;

    //    float*         lifetimes;
    //    math::Vector3* sizes;
    //    math::Vector3* positions;
    //    math::Vector3* velocities;
    //    Color*         colors;

    //    uint32_t particle_count;
    //};

    //void translate_editor_settings()
    //{
    //    particle_data_t pd = { 0 };

    //    // 2 modules - Velocity, Size

    //    for (uint32_t i = 0; i < emitter_indices[0]; i++)
    //    {
    //        pd.modifier_types[i] = (particle_modifier_t)(PARTICLE_MODIFIER_velocity | PARTICLE_MODIFIER_color);
    //        pd.modifiers[i]      = { ptr_to_floats, 7 };

    //        // pd.lifetimes[i] = ;
    //        // ...
    //    }
    //}

    //void particle_system_tick()
    //{
    //    for (uint32_t i = 0; i < pd.particle_count; i++)
    //    {
    //        if (pd.modifier_types[i] & PARTICLE_MODIFIER_velocity)
    //        {
    //            float* vel = pd.modifiers[i].value

    //            pd.velocities[i] = // vel ...
    //        }
    //    }
    //}

    class ParticleSystem : public Component
    {
    public:
        std::vector<Emitter*> emitters;

    public:
        ParticleSystem(Entity* entity);
        ~ParticleSystem();

        // icomponent
        //void Save(pugi::xml_node& node) override;
        //void Load(pugi::xml_node& node) override;
        void Tick() override;
    };
}
