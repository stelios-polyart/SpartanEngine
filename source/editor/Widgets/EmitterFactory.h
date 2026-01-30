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
#include "pch.h"
#include "../runtime/ParticleSystem/EmitterModule.h"
#include "../runtime/ParticleSystem/Gravity_Module.h"
#include "../runtime/ParticleSystem/EmitterModules/BoxShape_Module.h"
#include "../runtime/ParticleSystem/EmitterModules/AddVelocity_Module.h"
#include "../runtime/ParticleSystem/EmitterModules/ApplyVelocity_Module.h"
#include "../runtime/ParticleSystem/EmitterModules/Randomize_Module.h"
#include "../runtime/ParticleSystem/EmitterModules/SetLifetime_Module.h"
#include "../runtime/ParticleSystem/EmitterModules/SetColor_Module.h"
#include "../ImGui/ImGui_Extension.h"
//============================================

namespace spartan
{
    class EmitterFactory {
    public:

        static std::vector<std::string> GetAvailableInitializationModules() {
            return { "Set Lifetime", "Box Shape", "Randomize", "Add Velocity", "Set Color" };
        }

        static std::vector<std::string> GetAvailableUpdateModules() {
            return { "Add Velocity", "Gravity", "Apply Velocity", "Randomize", "Set Color" };
        }

        static std::string TypeToString(EmitterModuleType type)
        {
            switch (type)
            {
            case EmitterModuleType::Gravity:     return "Gravity";
            case EmitterModuleType::BoxShape:    return "Box Shape";
            case EmitterModuleType::AddVelocity: return "Add Velocity";
            case EmitterModuleType::ApplyVelocity: return "Apply Velocity";
            case EmitterModuleType::SetLifetime: return "Set Lifetime";
            case EmitterModuleType::Randomize:   return "Randomize";
            case EmitterModuleType::SetColor:   return "Set Color";
            default:
                assert(false && "TypeToString: Unknown EmitterModuleType");
                return {};
            }
        }

        static EmitterModuleType StringToType(const std::string& name)
        {
            if (name == "Gravity")     return EmitterModuleType::Gravity;
            if (name == "Box Shape")   return EmitterModuleType::BoxShape;
            if (name == "Add Velocity") return EmitterModuleType::AddVelocity;
            if (name == "Apply Velocity") return EmitterModuleType::ApplyVelocity;
            if (name == "Set Lifetime") return EmitterModuleType::SetLifetime;
            if (name == "Randomize")   return EmitterModuleType::Randomize;
            if (name == "Set Color")   return EmitterModuleType::SetColor;

            assert(false && "StringToType: Unknown component name");
            return EmitterModuleType::Max;
        }

        static ButtonColorPicker& GetColorPicker(const char* label, const void* key)
        {
            static std::unordered_map<const void*, ButtonColorPicker> pickers;

            auto it = pickers.find(key);
            if (it == pickers.end())
            {
                std::string title =
                    "Particle System Color Picker " + std::string(label) + "##" +
                    std::to_string(reinterpret_cast<uintptr_t>(key));

                it = pickers.emplace(key, ButtonColorPicker(title)).first;
            }

            return it->second;
        }

        static void DrawValue(const char* label, float* value) {
            ImGui::DragFloat(label, value, 0.1f);
        }

        static void DrawValue(const char* label, math::Vector3* value) {
            float arr[3] = { value->x, value->y, value->z };
            if (ImGui::DragFloat3(label, arr, 0.1f)) {
                value->x = arr[0];
                value->y = arr[1];
                value->z = arr[2];
            }
        }

        static void DrawValue(const char* label, spartan::Color* value)
        {
            ImGui::PushID(label);

            ButtonColorPicker& picker = GetColorPicker(label, value);

            picker.SetColor(*value);
            picker.Update();
            *value = picker.GetColor();

            ImGui::PopID();
        }

        static void DrawProperty(const char* name, ParticleFloat& property)
        {
            ImGui::PushID(name);

            float dropdown_width = 120.0f;
            float dropdown_pos_x = ImGui::GetContentRegionAvail().x - dropdown_width;

            // Line 1: Label and Type Selector
            ImGui::Text(name);
            ImGui::SameLine(dropdown_pos_x);
            ImGui::SetNextItemWidth(dropdown_width);

            int type = static_cast<int>(property.type);
            if (ImGui::Combo("##type", &type, "Constant\0Range\0Curve\0"))
            {
                property.type = static_cast<ps_property_type>(type);
            }

            // Line 2: Values aligned under the dropdown
            ImGui::SetCursorPosX(dropdown_pos_x);
            ImGui::BeginGroup();
            {
                if (property.type == ps_property_type::CONSTANT)
                {
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("##value", &property.const_value);
                }
                else if (property.type == ps_property_type::RANGE)
                {

                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Min", &property.range_value.first);

                    ImGui::SetCursorPosX(dropdown_pos_x);
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Max", &property.range_value.second);
                }
            }
            ImGui::EndGroup();

            ImGui::Spacing();
            ImGui::PopID();
        }

        static void DrawProperty(const char* name, ParticleVector3& property)
        {
            ImGui::PushID(name);

            float dropdown_width = 120.0f;
            float dropdown_pos_x = ImGui::GetContentRegionAvail().x - dropdown_width;

            ImGui::Text(name);
            ImGui::SameLine(dropdown_pos_x);
            ImGui::SetNextItemWidth(dropdown_width);

            int type = static_cast<int>(property.type);
            if (ImGui::Combo("##type", &type, "Constant\0Range\0Curve\0"))
            {
                property.type = static_cast<ps_property_type>(type);
            }

            ImGui::SetCursorPosX(dropdown_pos_x);
            ImGui::BeginGroup();
            {
                if (property.type == ps_property_type::CONSTANT)
                {
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Const", &property.const_value);
                }
                else if (property.type == ps_property_type::RANGE)
                {
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Min", &property.range_value.first);

                    ImGui::SetCursorPosX(dropdown_pos_x);
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Max", &property.range_value.second);
                }
            }
            ImGui::EndGroup();

            ImGui::Spacing();
            ImGui::PopID();
        }

        static void DrawProperty(const char* name, ParticleColor& property)
        {
            ImGui::PushID(name);

            float dropdown_width = 120.0f;
            float dropdown_pos_x = ImGui::GetContentRegionAvail().x - dropdown_width;

            ImGui::Text(name);
            ImGui::SameLine(dropdown_pos_x);
            ImGui::SetNextItemWidth(dropdown_width);

            int type = static_cast<int>(property.type);
            if (ImGui::Combo("##type", &type, "Constant\0Range\0Curve\0"))
            {
                property.type = static_cast<ps_property_type>(type);
            }

            ImGui::SetCursorPosX(dropdown_pos_x);
            ImGui::BeginGroup();
            {
                if (property.type == ps_property_type::CONSTANT)
                {
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("##const_val", &property.const_value);
                }
                else if (property.type == ps_property_type::RANGE)
                {
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Min", &property.range_value.first);

                    ImGui::SetCursorPosX(dropdown_pos_x);
                    ImGui::SetNextItemWidth(dropdown_width);
                    DrawValue("Max", &property.range_value.second);
                }
            }
            ImGui::EndGroup();

            ImGui::Spacing();
            ImGui::PopID();
        }

        static void DrawGUI(EmitterModule* module)
        {
            if (!module) return;

            static ButtonColorPicker picker_const("Color Picker - Constant");
            static ButtonColorPicker picker_min("Color Picker - Min");
            static ButtonColorPicker picker_max("Color Picker - Max");

            switch (module->GetType())
            {
            case EmitterModuleType::Gravity:
            {
                Gravity_Module* gravity_module = static_cast<Gravity_Module*>(module);

                DrawProperty("Gravity Vector", gravity_module->gravity);
            }
            break;
            case EmitterModuleType::AddVelocity:
            {
                AddVelocity_Module* add_velocity_module = static_cast<AddVelocity_Module*>(module);

                DrawProperty("Velocity Vector", add_velocity_module->velocity);
            }
            break;
            case EmitterModuleType::SetLifetime:
            {
                SetLifetime_Module* set_lifetime_module = static_cast<SetLifetime_Module*>(module);
                DrawProperty("Lifetime (seconds)", set_lifetime_module->lifetime);
            }
            break;
            case EmitterModuleType::ApplyVelocity:
            {
                ApplyVelocity_Module* apply_velocity_module = static_cast<ApplyVelocity_Module*>(module);
                DrawProperty("Damping", apply_velocity_module->damping);
            }
            break;
            case EmitterModuleType::SetColor:
            {
                SetColor_Module* set_color_module = static_cast<SetColor_Module*>(module);
                DrawProperty("Particle Color", set_color_module->color);
            }
            break;
            default:
                break;
            }
        }
    };
}
