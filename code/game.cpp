#ifndef GAME_CPP
#define GAME_CPP

// TODO: maybe pass layout by pointer
void RenderHex(ImDrawList* DrawList, layout Layout, hex Hex, u32 Color = 0xFF00FFFF)
{
    std::vector<v2> Corners = PolygonCorners(Layout, Hex);

    u32 Thickness = 4;
    v2 Offset = V2(0, 0);
    for(u32 Index = 0; Index < 6; Index++)
    {
        DrawList->AddLine(Offset + Corners[Index],
                          Offset + Corners[(Index + 1) % 6], Color, Thickness);
    }
}

void RenderHeroes(ImDrawList* DrawList, map* Map, layout Layout)
{
    for(const std::pair<hex, hero*>& Pair : Map->Heroes)
    {
        u32 Size = 15;
        v2 Center = HexToPixel(Layout, Pair.first);
        DrawList->AddCircleFilled(Center, Size, 0xFF00FF00, 30);
    }
}

bool HeroUnderMouse(map* Map, layout Layout, hero* HeroResult)
{
    bool Result = false;
    hex Hex(0, 0, 0);
    bool HexValid = GetHexUnderMouse(Map, Layout, &Hex);
    if(HexValid)
    {
        auto Search = Map->Heroes.find(Hex);
        if(Search != Map->Heroes.end())
        {
            // TODO: FIX this
            HeroResult->SpecID   = Search->second->SpecID;
            HeroResult->Position = Search->second->Position;
            HeroResult->Team     = Search->second->Team;
            Result = true;
        }
    }

    return Result;
}

void RenderDebugOverlay(map* Map, layout Layout)
{
    static bool Enable = true;
    ImGuiIO& IO = ImGui::GetIO();
    ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("Debug overlay", &Enable, WindowFlags))
    {
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", IO.MousePos.x, IO.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
    }

    hex SupposedHex = Round(PixelToHex(Layout, V2(IO.MousePos.x, IO.MousePos.y)));
    ImGui::Text("Supposed hex: (%d, %d, %d)", SupposedHex.Q, SupposedHex.R, SupposedHex.S);
    if(HasHex(Map, SupposedHex))
    {
        ImGui::Text("Hex: (%d, %d, %d)", SupposedHex.Q, SupposedHex.R, SupposedHex.S);
    }
    else
    {
        ImGui::Text("Hex: <invalid>");
    }

    s32 Index = -1;
    hero HoveredHero;
    if(HeroUnderMouse(Map, Layout, &HoveredHero))
    {
        // TODO: find hero_spec with this id
        for(u32 SpecIndex = 0; SpecIndex < HeroSpecs.size(); SpecIndex++)
        {
            if(HeroSpecs[SpecIndex].ID == HoveredHero.SpecID)
            {
                Index = SpecIndex;
                break;
            }
        }

        ImGui::Text("Hero class: %s", HeroSpecs[Index].Name.c_str());
        ImGui::Text("Hero team: %d", HoveredHero.Team);
    }

    ImGui::End();
}

void RenderGame()
{
    ImGui::Begin("Game");
    ImDrawList* DrawList = ImGui::GetWindowDrawList();

    ImVec2 CursorPos = ImGui::GetCursorScreenPos();
    ImVec2 WindowPos = ImGui::GetWindowPos();

    // DrawList->AddNgonFilled(ImVec2(CursorPos.x, CursorPos.y), 100, 0xFF0000FF, 6);
    // DrawList->AddNgonFilled(ImVec2(WindowPos.x, WindowPos.y), 100, 0xFFFF00FF, 6);

    static v2 CameraOffset = V2(550, 300);
    ImGuiIO& io = ImGui::GetIO();
    if(ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0))
    {
        CameraOffset.x += io.MouseDelta.x;
        CameraOffset.y += io.MouseDelta.y;
    }

    hero Hero = {HeroSpecs[0].ID, hex(0, 1, -1), 1};
    map Map = RectangularMap(-5, 5, -2, 2);
    PlaceHero(&Map, &Hero, hex(0, 1, -1));
    layout Layout = layout::Get();
    Layout.Origin.x = WindowPos.x + CameraOffset.x;
    Layout.Origin.y = WindowPos.y + CameraOffset.y;

    for(const hex& Hex : Map.Hexes)
    {
        RenderHex(DrawList, Layout, Hex);
    }

    hex Hovered(0, 0, 0);
    if(GetHexUnderMouse(&Map, Layout, &Hovered))
    {
        RenderHex(DrawList, Layout, Hovered, 0xFF0000FF);
    }

    RenderHeroes(DrawList, &Map, Layout);
    RenderDebugOverlay(&Map, Layout);



    ImGui::End();
}

#endif /* GAME_CPP */
