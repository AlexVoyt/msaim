static void ShowPlaceholderObject(const char* prefix, int uid, hero_spec* Spec)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(uid);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    // ImGui::TableSetColumnIndex(1);
    // ImGui::InputText("", &Spec->ClassName);
    bool node_open = ImGui::TreeNode(prefix);

    if (node_open)
    {
        ImGui::PushID(0);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
        ImGui::TreeNodeEx("MinStartingAP", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##value", &Spec->MinStartingAP, 1, Spec->MaxStartingAP);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("MaxStartingAP", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##value", &Spec->MaxStartingAP, Spec->MinStartingAP, 100);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID(2);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("MinStartingHP", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##value", &Spec->MinStartingHP, 1, Spec->MaxStartingHP);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID(3);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("MaxStartingHP", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##value", &Spec->MaxStartingHP, Spec->MinStartingHP, 100);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID(4);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("MinStartingIni", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##value", &Spec->MinStartingIni, 1, Spec->MaxStartingIni);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID(5);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("MaxStartingIni", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##value", &Spec->MaxStartingIni, Spec->MinStartingIni, 100);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void RenderHeroSpecEditor()
{
    static std::vector<hero_spec> HeroSpecs = {{"Warrior", 3, 5, 15, 48, 3, 8}};

    ImGui::Begin("Hero spec editor");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        u32 SpecLength = HeroSpecs.size();
        for(u32 SpecIndex = 0; SpecIndex < SpecLength; SpecIndex++)
        {
            hero_spec* Spec = &HeroSpecs[SpecIndex];
            ShowPlaceholderObject(Spec->ClassName.c_str(), SpecIndex, Spec);
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

void RenderEditor()
{
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    RenderHeroSpecEditor();
    ImGui::ShowDemoWindow();
}
