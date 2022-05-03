namespace std
{
    template <>
    struct hash<::hex>
    {
        size_t operator()(const ::hex& Hex) const
        {
            hash<int> IntHash;
            size_t Q = IntHash(Hex.Q);
            size_t R = IntHash(Hex.R);
            return Q ^ (R + 0x9e3779b9 + (Q << 6) + (Q >> 2));
        }
    };
}

struct hero;
struct map
{
    std::unordered_set<hex> Hexes;
    std::unordered_map<hex, hero*> Heroes;
};

map RectangularMap(s32 Left, s32 Right, s32 Top, s32 Bottom)
{
    map Result;

    for (int Q = Left; Q <= Right; Q++)
    {
        int QOffset = floor(Q/2.0);
        for (int R = Top - QOffset; R <= Bottom - QOffset; R++)
        {
            Result.Hexes.insert(hex(Q, R, -Q-R));
        }
    }

    return Result;
}

bool HasHex(map* Map, hex Hex)
{
    bool Result = Map->Hexes.find(Hex) != Map->Hexes.end();

    return Result;
}

bool GetHexUnderMouse(map* Map, layout Layout, hex* HexResult)
{
    bool Result = false;
    ImGuiIO& IO = ImGui::GetIO();
    hex SupposedHex = Round(PixelToHex(Layout, V2(IO.MousePos.x, IO.MousePos.y)));
    if(HasHex(Map, SupposedHex))
    {
        *HexResult = SupposedHex;
        Result = true;
    }

    return Result;
}

bool HexEmpty(map* Map, hex Hex)
{
    return Map->Heroes.find(Hex) == Map->Heroes.end();
}

void PlaceHero(map* Map, hero* Hero, hex Hex)
{
    assert(HexEmpty(Map, Hex));
    Map->Heroes.insert(std::pair(Hex, Hero));
    Hero->Position = Hex;
}

