static hero_spec EditingSpec_ = {0, "New Spec", 1, 1, 1, 1, 1, 1};
static spell EditingSpell_ = {0, "New Spell", 1, 1, 1, 1, 1};
static bool True = true;
static bool False = false;

void RenderAttributeSliderTree(int ID, char* Name, i32* Value, i32 MinValue, i32 MaxValue, ImGuiTreeNodeFlags Flags)
{
    ImGui::PushID(ID);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::TreeNodeEx(Name, Flags);
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SliderInt("##value", Value, MinValue, MaxValue);
    ImGui::NextColumn();
    ImGui::PopID();
}

void RenderAttributeSlider(int ID, char* Name, i32* Value, i32 MinValue, i32 MaxValue)
{
    ImGui::PushID(ID);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(Name);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SliderInt("##value", Value, MinValue, MaxValue);
    ImGui::NextColumn();
    ImGui::PopID();
}

void RenderAttributeRangeDrag(int ID, char* Name, i32* Min, i32* Max, u32 Step, i32 MinValue, i32 MaxValue, char* MinFormat, char* MaxFormat)
{
    ImGui::PushID(ID);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(Name);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::DragIntRange2("##value", Min, Max, Step, MinValue, MaxValue, MinFormat, MaxFormat);
    ImGui::PopID();
}

bool IsLetter(ImWchar c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

static int AsciiLetters(ImGuiInputTextCallbackData* Data)
{
    if(IsLetter(Data->EventChar))
    {
        return 0;
    }
    return 1;
}

void RenderTextInput(char* Name, std::string* String)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(Name);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputText("##Input", String, ImGuiInputTextFlags_CallbackCharFilter, AsciiLetters);
    ImGui::NextColumn();
}

void RenderHeroSpecEditor()
{
    ImGui::Begin("Hero spec editor");
    if(ImGui::BeginTable("Hero editor", 3, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        ImGui::TableNextColumn();
        hero_spec* Spec = 0;
        static int Selected = 0;
        {
            ImGui::BeginChild("Spec names", ImVec2(0, 0), false);
            u32 SpecLength = HeroSpecs.size();
            for(u32 SpecIndex = 0; SpecIndex < SpecLength; SpecIndex++)
            {
                // TODO: generating id feels hacky
                Spec = &HeroSpecs[SpecIndex];
                char Label[128];
                if(Spec->Name.size())
                {
                    snprintf(Label, 90, "%s##%d", Spec->Name.c_str(), SpecIndex);
                }
                else
                {
                    snprintf(Label, 90, "<Enter name>##%d", SpecIndex);
                }

                if(ImGui::Selectable(Label, Selected == SpecIndex))
                    Selected = SpecIndex;
            }
            ImGui::EndChild();
        }

        ImGui::TableNextColumn();
        if(Spec)
        {
            Spec = &HeroSpecs[Selected];
            {
                ImGui::PushID("Editing");
                RenderTextInput("Name", &Spec->Name);
                RenderAttributeRangeDrag(0, "Starting AP", &Spec->MinStartingAP, &Spec->MaxStartingAP, 1, 1, 100, "Min: %d AP", "Max: %d AP");
                RenderAttributeRangeDrag(1, "Starting HP", &Spec->MinStartingHP, &Spec->MaxStartingHP, 1, 1, 100, "Min: %d HP", "Max: %d HP");
                RenderAttributeRangeDrag(2, "Starting Ini", &Spec->MinStartingIni, &Spec->MaxStartingIni, 1, 1, 100, "Min: %d Ini", "Max: %d Ini");

                ImGui::BeginChild("Available spells", ImVec2(0, ImGui::GetContentRegionAvail().y * 0.9), false);
                for(u32 SpellIndex = 0; SpellIndex < Spells.size(); SpellIndex++)
                {
                    available_spell AvailableSpell = {};
                    AvailableSpell.SpecID = Spec->ID;
                    AvailableSpell.SpellID = Spells[SpellIndex].ID;

                    auto FindResult = std::find(std::begin(AvailableSpells),
                                                std::end(AvailableSpells),
                                                AvailableSpell);
                    bool* IsAvailable;
                    (FindResult == AvailableSpells.end()) ? IsAvailable = &False : IsAvailable = &True;

                    if(ImGui::Checkbox(Spells[SpellIndex].Name.c_str(), IsAvailable))
                    {
                        // Return our static variable state to where it belongs
                        *IsAvailable = !(*IsAvailable);

                        if(FindResult == AvailableSpells.end())
                        {
                            AddItemToDatabase<available_spell>(Storage, AvailableSpells,
                                                               &AvailableSpell);
                        }
                        else
                        {
                            Storage->remove<available_spell>(AvailableSpell.SpecID,
                                                             AvailableSpell.SpellID);
                            AvailableSpells.erase(FindResult);
                        }
                    }
                }
                ImGui::EndChild();

                u32 DisableSave = Spec->Name.size() == 0 ? 1 : 0;
                if(DisableSave)
                    ImGui::BeginDisabled();

                if(ImGui::Button("Save"))
                {
                    UpdateItemInDatabase(Storage, Spec);
                }

                if(DisableSave && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("You must enter name before saving hero specification");

                if(DisableSave)
                    ImGui::EndDisabled();

                ImGui::SameLine();
                if(ImGui::Button("Delete"))
                {
                    DeleteItemFromDatabase(Storage, Spec);
                    HeroSpecs.erase(HeroSpecs.begin() + Selected);
                    Selected = 0;
                }

                ImGui::PopID();
            }
        }

        ImGui::TableNextColumn();
        {
            hero_spec* EditingSpec = &EditingSpec_;

            ImGui::PushID("Creating");
            RenderTextInput("Name", &EditingSpec->Name);
            RenderAttributeRangeDrag(0, "Starting AP", &EditingSpec->MinStartingAP, &EditingSpec->MaxStartingAP, 1, 1, 100, "Min: %d AP", "Max: %d AP");
            RenderAttributeRangeDrag(1, "Starting HP", &EditingSpec->MinStartingHP, &EditingSpec->MaxStartingHP, 1, 1, 100, "Min: %d HP", "Max: %d HP");
            RenderAttributeRangeDrag(2, "Starting Ini", &EditingSpec->MinStartingIni, &EditingSpec->MaxStartingIni, 1, 1, 100, "Min: %d Ini", "Max: %d Ini");

            u32 DisableAdd = EditingSpec->Name.size() == 0 ? 1 : 0;
            if(DisableAdd)
                ImGui::BeginDisabled();

            if(ImGui::Button("Add"))
            {
                AddItemToDatabase<hero_spec>(Storage, HeroSpecs, EditingSpec);
            }

            if(DisableAdd && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("You must enter name before adding hero specification");

            if(DisableAdd)
                ImGui::EndDisabled();

            ImGui::PopID();
        }
        ImGui::EndTable();

    }
    ImGui::End();

}

void RenderSpellEditor()
{
    ImGui::Begin("Spell editor");
    if(ImGui::BeginTable("Spell editor", 3, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        ImGui::TableNextColumn();
        spell* Spell = 0;
        static int Selected = 0;
        {
            ImGui::BeginChild("Spell names", ImVec2(0, 0), false);
            u32 SpellLength = Spells.size();
            for(u32 SpellIndex = 0; SpellIndex < SpellLength; SpellIndex++)
            {
                // TODO: generating id feels hacky
                Spell = &Spells[SpellIndex];
                char Label[128];
                if(Spell->Name.size())
                {
                    snprintf(Label, 90, "%s##%d", Spell->Name.c_str(), SpellIndex);
                }
                else
                {
                    snprintf(Label, 90, "<Enter name>##%d", SpellIndex);
                }

                if(ImGui::Selectable(Label, Selected == SpellIndex))
                    Selected = SpellIndex;
            }
            ImGui::EndChild();
        }

        ImGui::TableNextColumn();
        if(Spell)
        {
            Spell = &Spells[Selected];
            {
                // Radius and distance cannot be both 0
                i32 MinRadius   = Spell->Distance == 0 ? 1 : 0;
                i32 MinDistance = Spell->Radius   == 0 ? 1 : 0;

                ImGui::PushID("Editing");
                RenderTextInput("Name", &Spell->Name);
                RenderAttributeSlider(0, "Distance", &Spell->Distance, MinDistance, 10);
                RenderAttributeSlider(1, "Radius", &Spell->Radius, MinRadius, 10);
                RenderAttributeSlider(2, "Damage", &Spell->Damage, 1, 50);
                RenderAttributeSlider(3, "ApCost", &Spell->ApCost, 1, 10);
                RenderAttributeSlider(4, "Cooldown", &Spell->Cooldown, 1, 10);

                u32 DisableSave = Spell->Name.size() == 0 ? 1 : 0;
                if(DisableSave)
                    ImGui::BeginDisabled();

                if(ImGui::Button("Save"))
                {
                    UpdateItemInDatabase(Storage, Spell);
                }

                if(DisableSave && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("You must enter name before saving hero specification");

                if(DisableSave)
                    ImGui::EndDisabled();

                ImGui::SameLine();
                if(ImGui::Button("Delete"))
                {
                    DeleteItemFromDatabase(Storage, Spell);
                    Spells.erase(Spells.begin() + Selected);
                    Selected = 0;
                }

                ImGui::PopID();
            }
        }

        ImGui::TableNextColumn();
        {
            spell* EditingSpell = &EditingSpell_;

            // Radius and distance cannot be both 0
            i32 MinRadius   = EditingSpell->Distance == 0 ? 1 : 0;
            i32 MinDistance = EditingSpell->Radius   == 0 ? 1 : 0;

            ImGui::PushID("Creating");
            RenderTextInput("Name", &EditingSpell->Name);
            RenderAttributeSlider(0, "Distance", &EditingSpell->Distance, MinDistance, 10);
            RenderAttributeSlider(1, "Radius", &EditingSpell->Radius, MinRadius, 10);
            RenderAttributeSlider(2, "Damage", &EditingSpell->Damage, 1, 50);
            RenderAttributeSlider(3, "ApCost", &EditingSpell->ApCost, 1, 10);
            RenderAttributeSlider(4, "Cooldown", &EditingSpell->Cooldown, 1, 10);

            u32 DisableAdd = EditingSpell->Name.size() == 0 ? 1 : 0;
            if(DisableAdd)
                ImGui::BeginDisabled();

            if(ImGui::Button("Add"))
            {
                AddItemToDatabase<spell>(Storage, Spells, EditingSpell);
            }

            if(DisableAdd && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("You must enter name before adding hero specification");

            if(DisableAdd)
                ImGui::EndDisabled();

            ImGui::PopID();
        }
        ImGui::EndTable();

    }
    ImGui::End();

}

void RenderEditor()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    RenderHeroSpecEditor();
    RenderSpellEditor();
    ImGui::ShowDemoWindow();
}
