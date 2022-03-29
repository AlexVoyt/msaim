void RenderAttributeSlider(int Id, char* Name, i32* Value, i32 MinValue, i32 MaxValue, ImGuiTreeNodeFlags Flags)
{
    ImGui::PushID(Id);
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

void ShowHeroSpec(hero_spec* Spec, int uid)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(uid);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    // ImGui::TableSetColumnIndex(1);
    // ImGui::InputText("", &Spec->ClassName);
    bool node_open = ImGui::TreeNode(Spec->ClassName.c_str());

    if(node_open)
    {
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

        RenderAttributeSlider(0, "MinStartingAP", &Spec->MinStartingAP, 1, Spec->MaxStartingAP, Flags);
        RenderAttributeSlider(1, "MaxStartingAP", &Spec->MaxStartingAP, Spec->MinStartingAP, 100, Flags);
        RenderAttributeSlider(2, "MinStartingHP", &Spec->MinStartingHP, 1, Spec->MaxStartingHP, Flags);
        RenderAttributeSlider(3, "MaxStartingHP", &Spec->MaxStartingHP, Spec->MinStartingHP, 100, Flags);
        RenderAttributeSlider(4, "MinStartingIni", &Spec->MinStartingIni, 1, Spec->MaxStartingIni, Flags);
        RenderAttributeSlider(5, "MaxStartingIni", &Spec->MaxStartingIni, Spec->MinStartingIni, 100, Flags);

#if 0
        u32 SpellsLength = Spells.size();
        for(u32 SpellIndex = 0; SpellIndex < SpellsLength; SpellIndex++)
        {
        }
#endif

        ImGui::TreePop();
    }
    ImGui::PopID();
}

static std::vector<spell> Spells = {{"Self-immolation", 0, 2, 15, 3, 5},
                                    {"Fireball", 3, 1, 8, 2, 1}};

static std::vector<hero_spec> HeroSpecs = {{"Warrior", 3, 5, 15, 48, 3, 8}};
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
            //ImGui::Separator();
        }
        ImGui::EndTable();
        if(ImGui::Button("Add new hero spec"))
        {
            hero_spec Spec = {"New Spec", 1, 1, 1, 1, 1, 1};
            HeroSpecs.push_back(Spec);
        }

    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void ShowSpellAttr(spell* Spell, int uid)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(uid);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    // ImGui::TableSetColumnIndex(1);
    // ImGui::InputText("", &Spec->ClassName);
    bool NodeOpen = ImGui::TreeNode(Spell->Name.c_str());

    if(NodeOpen)
    {
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

        // Radius and distance cannot be both 0
        i32 MinRadius   = Spell->Distance == 0 ? 1 : 0;
        i32 MinDistance = Spell->Radius   == 0 ? 1 : 0;

        RenderAttributeSlider(0, "Distance", &Spell->Distance, MinDistance, 10, Flags);
        RenderAttributeSlider(1, "Radius", &Spell->Radius, MinRadius, 10, Flags);
        RenderAttributeSlider(2, "Damage", &Spell->Damage, 1, 50, Flags);
        RenderAttributeSlider(3, "ApCost", &Spell->ApCost, 1, 10, Flags);
        RenderAttributeSlider(4, "Cooldown", &Spell->Cooldown, 1, 10, Flags);

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
            spell Spell = {"New Spell", 1, 1, 1, 1, 1};
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
