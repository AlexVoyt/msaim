struct spell
{
    std::string Name;

    u32 Distance;
    u32 Radius;
    u32 Damage;
    u32 ApCost;
    u32 Cooldown;
};

struct hero_spec
{
    std::string ClassName;

    int MinStartingAP;
    int MaxStartingAP;

    int MinStartingHP;
    int MaxStartingHP;

    int MinStartingIni;
    int MaxStartingIni;

    std::vector<spell*> Spells;
};
