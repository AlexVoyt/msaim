static hero_spec EditingSpec_ = {0, "New Spec", 1, 1, 1, 1, 1, 1};

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

void RenderTextInputTree(char* Name, std::string* String, ImGuiTreeNodeFlags Flags)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::TreeNodeEx(Name, Flags);
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputText("##Input", String, 0, 0, 0);
    ImGui::NextColumn();
}

void RenderTextInput(char* Name, std::string* String)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(Name);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputText("##Input", String, 0, 0, 0);
    ImGui::NextColumn();
}

void ShowHeroSpec(hero_spec* Spec, int uid)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(uid);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    // ImGui::TableSetColumnIndex(1);
    // ImGui::InputText("", &Spec->Name);
    bool NodeOpen = ImGui::TreeNode((void*)(intptr_t)uid, "%s", Spec->Name.data());
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    // TODO:
    if(ImGui::Button("Delete"))
    {
        HeroSpecs.erase(HeroSpecs.begin() + uid);
    }

    if(NodeOpen)
    {
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

        RenderTextInputTree("Name", &Spec->Name, Flags);
        RenderAttributeSliderTree(0, "MinStartingAP", &Spec->MinStartingAP, 1, Spec->MaxStartingAP, Flags);
        RenderAttributeSliderTree(1, "MaxStartingAP", &Spec->MaxStartingAP, Spec->MinStartingAP, 100, Flags);
        RenderAttributeSliderTree(2, "MinStartingHP", &Spec->MinStartingHP, 1, Spec->MaxStartingHP, Flags);
        RenderAttributeSliderTree(3, "MaxStartingHP", &Spec->MaxStartingHP, Spec->MinStartingHP, 100, Flags);
        RenderAttributeSliderTree(4, "MinStartingIni", &Spec->MinStartingIni, 1, Spec->MaxStartingIni, Flags);
        RenderAttributeSliderTree(5, "MaxStartingIni", &Spec->MaxStartingIni, Spec->MinStartingIni, 100, Flags);

#if 1
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TreeNodeEx("Available Spells", Flags);
        ImGui::TableSetColumnIndex(1);
        u32 SpellsLength = Spells.size();
        for(u32 SpellIndex = 0; SpellIndex < SpellsLength; SpellIndex++)
        {
            bool HasSpell = true;
            spell* Spell = &Spells[SpellIndex];
            if(ImGui::Checkbox(Spell->Name.c_str(), &HasSpell))
            {
            }
        }
        ImGui::NextColumn();
#endif

        ImGui::TreePop();
    }
    ImGui::PopID();
}

#if 0
void RenderHeroSpecEditor()
{
    ImGui::Begin("Hero spec editor");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if(ImGui::BeginTable("Hero editor", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        u32 SpecLength = HeroSpecs.size();
        for(u32 SpecIndex = 0; SpecIndex < SpecLength; SpecIndex++)
        {
            hero_spec* Spec = &HeroSpecs[SpecIndex];
            ShowHeroSpec(Spec, SpecIndex);
            // ImGui::Separator();
        }
        ImGui::EndTable();


        if(ImGui::Button("Add new hero spec"))
        {
            hero_spec Spec = {3, "New Spec", 1, 1, 1, 1, 1, 1};
            HeroSpecs.push_back(Spec);
        }

    }
    ImGui::PopStyleVar();
    ImGui::End();
}
#else

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
                    snprintf(Label, 90, "%s##value", Spec->Name.c_str());
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

        // TODO: test if we have no hero specs
        if(Spec)
        {
            Spec = &HeroSpecs[Selected];

            // TODO: alignment
            ImGui::TableNextColumn();
            {
                ImGui::PushID("Editing");
                RenderTextInput("Name", &Spec->Name);
                RenderAttributeSlider(0, "MinStartingAP", &Spec->MinStartingAP, 1, Spec->MaxStartingAP);
                RenderAttributeSlider(1, "MaxStartingAP", &Spec->MaxStartingAP, Spec->MinStartingAP, 100);
                RenderAttributeSlider(2, "MinStartingHP", &Spec->MinStartingHP, 1, Spec->MaxStartingHP);
                RenderAttributeSlider(3, "MaxStartingHP", &Spec->MaxStartingHP, Spec->MinStartingHP, 100);
                RenderAttributeSlider(4, "MinStartingIni", &Spec->MinStartingIni, 1, Spec->MaxStartingIni);
                RenderAttributeSlider(5, "MaxStartingIni", &Spec->MaxStartingIni, Spec->MinStartingIni, 100);
                if (ImGui::Button("Save")) {}
                ImGui::PopID();
            }

            ImGui::TableNextColumn();
            {
                hero_spec* EditingSpec = &EditingSpec_;

                ImGui::PushID("Creating");
                RenderTextInput("Name", &EditingSpec->Name);
                RenderAttributeSlider(0, "MinStartingAP", &EditingSpec->MinStartingAP, 1, EditingSpec->MaxStartingAP);
                RenderAttributeSlider(1, "MaxStartingAP", &EditingSpec->MaxStartingAP, EditingSpec->MinStartingAP, 100);
                RenderAttributeSlider(2, "MinStartingHP", &EditingSpec->MinStartingHP, 1, EditingSpec->MaxStartingHP);
                RenderAttributeSlider(3, "MaxStartingHP", &EditingSpec->MaxStartingHP, EditingSpec->MinStartingHP, 100);
                RenderAttributeSlider(4, "MinStartingIni", &EditingSpec->MinStartingIni, 1, EditingSpec->MaxStartingIni);
                RenderAttributeSlider(5, "MaxStartingIni", &EditingSpec->MaxStartingIni, EditingSpec->MinStartingIni, 100);
                if (ImGui::Button("Add")) {}
                ImGui::PopID();
            }
        }
        ImGui::EndTable();

    }
    ImGui::End();

}
#endif

void ShowSpellAttr(spell* Spell, int uid)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(uid);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    // ImGui::TableSetColumnIndex(1);
    // ImGui::InputText("", &Spec->Name);
    bool NodeOpen = ImGui::TreeNode((void*)(intptr_t)uid, "%s", Spell->Name.data());

    if(NodeOpen)
    {
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

        // Radius and distance cannot be both 0
        i32 MinRadius   = Spell->Distance == 0 ? 1 : 0;
        i32 MinDistance = Spell->Radius   == 0 ? 1 : 0;

        RenderTextInputTree("Name", &Spell->Name, Flags);
        RenderAttributeSliderTree(0, "Distance", &Spell->Distance, MinDistance, 10, Flags);
        RenderAttributeSliderTree(1, "Radius", &Spell->Radius, MinRadius, 10, Flags);
        RenderAttributeSliderTree(2, "Damage", &Spell->Damage, 1, 50, Flags);
        RenderAttributeSliderTree(3, "ApCost", &Spell->ApCost, 1, 10, Flags);
        RenderAttributeSliderTree(4, "Cooldown", &Spell->Cooldown, 1, 10, Flags);

        ImGui::TreePop();
    }

    ImGui::PopID();

}

void RenderSpellEditor()
{
    ImGui::Begin("Spell editor");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if(ImGui::BeginTable("Spell editor", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        u32 SpellLength = Spells.size();
        for(u32 SpellIndex = 0; SpellIndex < SpellLength; SpellIndex++)
        {
            spell* Spell = &Spells[SpellIndex];
            ShowSpellAttr(Spell, SpellIndex);
            //ImGui::Separator();
        }
        ImGui::EndTable();
        if(ImGui::Button("Add new spell"))
        {
            spell Spell = {69, "New Spell", 1, 1, 1, 1, 1};
            Spells.push_back(Spell);
        }

    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void RenderEditor()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    RenderHeroSpecEditor();
    RenderSpellEditor();
    ImGui::ShowDemoWindow();
}
