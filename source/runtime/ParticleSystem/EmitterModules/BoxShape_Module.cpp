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

//= INCLUDES ======================
#include "pch.h"
#include "BoxShape_Module.h"
#include "../ParticleData.h"
//=================================

void spartan::BoxShape_Module::OnInitialize(ParticleData* data, uint32_t start_index, uint32_t end_index)
{
    for (uint32_t i = start_index; i < end_index; ++i)
    {
        float rx = math::random(-extents.x, extents.x);
        float ry = math::random(-extents.y, extents.y);
        float rz = math::random(-extents.z, extents.z);
        math::Vector3 local_position = { rx, ry, rz };
        
        math::Vector3 rotated_position = local_rotation * local_position;
        
        data->positions[i] = local_offset + rotated_position;
    }
}
