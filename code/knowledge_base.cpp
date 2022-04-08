struct hero_spec;

struct spell
{
    i32 ID;
    std::string Name;

    i32 Distance;
    i32 Radius;
    i32 Damage;
    i32 ApCost;
    i32 Cooldown;
};

struct hero_spec
{
    i32 ID;
    std::string Name;

    i32 MinStartingAP;
    i32 MaxStartingAP;

    i32 MinStartingHP;
    i32 MaxStartingHP;

    i32 MinStartingIni;
    i32 MaxStartingIni;
};

struct hero_spec_spell
{
    i32 SpecID;
    i32 SpellID;
};

static std::vector<spell> Spells;
static std::vector<hero_spec> HeroSpecs;

inline auto CreateDatabase()
{
    using namespace sqlite_orm;
    auto Storage = make_storage("db.sqlite",
        make_table("HeroSpecs",
                    make_column("ID", &hero_spec::ID, autoincrement(), primary_key()),
                    make_column("Name", &hero_spec::Name),
                    make_column("MinStartingAP", &hero_spec::MinStartingAP),
                    make_column("MaxStartingAP", &hero_spec::MaxStartingAP),
                    make_column("MinStartingHP", &hero_spec::MinStartingHP),
                    make_column("MaxStartingHP", &hero_spec::MaxStartingHP),
                    make_column("MinStartingIni", &hero_spec::MinStartingIni),
                    make_column("MaxStartingIni", &hero_spec::MaxStartingIni)),
        make_table("Spells",
                    make_column("ID", &spell::ID, autoincrement(), primary_key()),
                    make_column("Name", &spell::Name),
                    make_column("Distance", &spell::Distance),
                    make_column("Radius", &spell::Radius),
                    make_column("Damage", &spell::Damage),
                    make_column("ApCost", &spell::ApCost),
                    make_column("Cooldown", &spell::Cooldown)),
        make_table("HeroSpecs_Spells",
                    make_column("SpecID", &hero_spec_spell::SpecID),
                    make_column("SpellID", &hero_spec_spell::SpellID),
                    primary_key(&hero_spec_spell::SpecID, &hero_spec_spell::SpellID),
                    foreign_key(&hero_spec_spell::SpecID)
                        .references(&hero_spec::ID).on_delete.cascade(),
                    foreign_key(&hero_spec_spell::SpellID)
                        .references(&spell::ID).on_delete.cascade()));


    Storage.sync_schema(true);

    return Storage;
}

using storage = decltype(CreateDatabase());
void LoadHeroSpecs(std::unique_ptr<storage>& Storage)
{
    HeroSpecs = Storage->get_all<hero_spec>();
}

void AddHeroSpecToDatabase()
{
}
