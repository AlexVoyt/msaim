struct hero_spec;

struct spell
{
    s32 ID;
    std::string Name;

    s32 Distance;
    s32 Radius;
    s32 Damage;
    s32 ApCost;
    s32 Cooldown;
};

struct hero_spec
{
    s32 ID;
    std::string Name;

    s32 MinStartingAP;
    s32 MaxStartingAP;

    s32 MinStartingHP;
    s32 MaxStartingHP;

    s32 MinStartingIni;
    s32 MaxStartingIni;
};

struct available_spell
{
    s32 SpecID;
    s32 SpellID;
};

inline bool operator==(const available_spell& lhs, const available_spell& rhs)
{
    return lhs.SpecID  == rhs.SpecID &&
           lhs.SpellID == rhs.SpellID;
}

static std::vector<spell> Spells;
static std::vector<hero_spec> HeroSpecs;
static std::vector<available_spell> AvailableSpells;

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
        make_table("AvailableSpells",
                    make_column("SpecID", &available_spell::SpecID),
                    make_column("SpellID", &available_spell::SpellID),
                    primary_key(&available_spell::SpecID, &available_spell::SpellID),
                    foreign_key(&available_spell::SpecID)
                        .references(&hero_spec::ID).on_delete.cascade(),
                    foreign_key(&available_spell::SpellID)
                        .references(&spell::ID).on_delete.cascade()));


    Storage.sync_schema(true);

    return Storage;
}

using storage = decltype(CreateDatabase());
std::unique_ptr<storage> Storage;

template<class T>
void LoadItems(std::unique_ptr<storage>& Storage,
               std::vector<T>& Items)
{
    Items = Storage->get_all<T>();
}

template<class T>
void AddItemToDatabase(std::unique_ptr<storage>& Storage,
                       std::vector<T>& Items,
                       T* Item)
{
    T ToAdd = *Item;
    auto InsertedID = Storage->insert(ToAdd);
    ToAdd.ID = InsertedID;
    Items.push_back(ToAdd);
}

template<>
void AddItemToDatabase<available_spell>(std::unique_ptr<storage>& Storage,
                                        std::vector<available_spell>& Items,
                                        available_spell* Item)
{
    Storage->replace(*Item);
    Items.push_back(*Item);
}

template<class T>
void UpdateItemInDatabase(std::unique_ptr<storage>& Storage,
                          T* Item)
{
    Storage->update(*Item);
}

template<class T>
void DeleteItemFromDatabase(std::unique_ptr<storage>& Storage,
                            T* Item)
{
    Storage->remove<T>(Item->ID);
}
