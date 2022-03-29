struct spell
{
    std::string Name;

    i32 Distance;
    i32 Radius;
    i32 Damage;
    i32 ApCost;
    i32 Cooldown;
};

struct hero_spec
{
    std::string ClassName;

    i32 MinStartingAP;
    i32 MaxStartingAP;

    i32 MinStartingHP;
    i32 MaxStartingHP;

    i32 MinStartingIni;
    i32 MaxStartingIni;

    std::vector<spell*> Spells;
};
