#pragma once

class Data {
public:
    enum class DATATYPE { 
        NOTHING,
        UNEQUIP,
        WEAP,
        SPELL,
        SHOUT,
        ARMOR,
        POTION,
        POTION_AUTO_HIGHEST,
        POTION_AUTO_LOWEST
    };

    DATATYPE type;
    std::string name;

public:
    Data() : type(DATATYPE::NOTHING), name("") {}
    Data(DATATYPE _type, std::string _name) : type(_type), name(_name) {}
    virtual std::string Pack() = 0;
};

class DataWeapon : public Data {
public:
    uint32_t enchNum;
    std::string enchName;
    float tempVal;
    RE::TESForm* form;

public:
    DataWeapon();
    DataWeapon(DATATYPE _type, const std::string& _name, const uint32_t& _enchNum, const std::string& _enchName,
               const float& _tempVal, RE::TESForm* _form)
        : Data(_type, _name), enchNum(_enchNum), enchName(_enchName), tempVal(_tempVal), form(_form) {}
    virtual std::string Pack() override;
    static DataWeapon Unpack(const std::string& _packed);

    void operator=(const DataWeapon& _object) {
        this->type = _object.type;
        this->name = _object.name;
        this->enchNum = _object.enchNum;
        this->enchName = _object.enchName;
        this->tempVal = _object.tempVal;
        this->form = _object.form;
    }

    bool operator==(const DataWeapon& _object) const {
        return this->type == _object.type &&
               this->name == _object.name &&
               this->enchNum == _object.enchNum &&
               this->enchName == _object.enchName &&
               this->tempVal == _object.tempVal &&
               this->form == _object.form;
    }

    bool operator!=(const DataWeapon& _object) const {
        return !(*this == _object);
    }
};

class DataShout : public Data {
public:
    RE::TESForm* form;

public:
    DataShout();
    DataShout(DATATYPE _type, const std::string& _name, RE::TESForm* _form) : Data(_type, _name), form(_form) {}
    virtual std::string Pack() override;
    static DataShout Unpack(const std::string& _packed);

    void operator=(const DataShout& _object) {
        this->type = _object.type;
        this->name = _object.name;
        this->form = _object.form;
    }

    bool operator==(const DataShout& _object) const {
        return this->type == _object.type &&
               this->name == _object.name &&
               this->form == _object.form;
    }

    bool operator!=(const DataShout& _object) const {
        return !(*this == _object);
    }
};

class DataArmor : public Data {
public:
    uint32_t enchNum;
    std::string enchName;
    float tempVal;
    RE::TESForm* form;

public:
    DataArmor();
    DataArmor(DATATYPE _type, const std::string& _name, const uint32_t& _enchNum, const std::string& _enchName,
              const float& _tempVal, RE::TESForm* _form)
        : Data(_type, _name), enchNum(_enchNum), enchName(_enchName), tempVal(_tempVal), form(_form) {}
    virtual std::string Pack() override;
    static DataArmor Unpack(const std::string& _packed);

    void operator=(const DataArmor& _object) {
        this->type = _object.type;
        this->name = _object.name;
        this->enchNum = _object.enchNum;
        this->enchName = _object.enchName;
        this->tempVal = _object.tempVal;
        this->form = _object.form;
    }

    bool operator==(const DataArmor& _object) const {
        return this->type == _object.type &&
               this->name == _object.name &&
               this->enchNum == _object.enchNum &&
               this->enchName == _object.enchName &&
               this->tempVal == _object.tempVal &&
               this->form == _object.form;
    }

    bool operator!=(const DataArmor& _object) const {
        return !(*this == _object);
    }
};

class DataPotion : public Data {
public:
    RE::TESForm* form;

public:
    DataPotion();
    DataPotion(DATATYPE _type, const std::string& _name, RE::TESForm* _form) : Data(_type, _name), form(_form) {}
    virtual std::string Pack() override;
    static DataPotion Unpack(const std::string& _packed);

    void operator=(const DataPotion& _object) {
        this->type = _object.type;
        this->name = _object.name;
        this->form = _object.form;
    }

    bool operator==(const DataPotion& _object) const {
        return this->type == _object.type &&
               this->name == _object.name &&
               this->form == _object.form;
    }

    bool operator!=(const DataPotion& _object) const {
        return !(*this == _object);
    }
};

class DataHandler {
public:
    void Init();
    void InitWeapon();
    void InitShout();
    void InitArmor();
    void InitPotion();
    void Clear();

    std::vector<DataWeapon> weapon;
    std::vector<DataShout> shout;
    std::vector<DataArmor> armor;
    std::vector<DataPotion> potion;
    std::vector<DataPotion> potion_health;
    std::vector<DataPotion> potion_magicka;
    std::vector<DataPotion> potion_stamina;

public:
    static DataHandler* GetSingleton() {
        static DataHandler singleton;
        return std::addressof(singleton);
    }

private:
    DataHandler() {}
    DataHandler(const DataHandler&) = delete;
    DataHandler(DataHandler&&) = delete;

    ~DataHandler() = default;

    DataHandler& operator=(const DataHandler&) = delete;
    DataHandler& operator=(DataHandler&&) = delete;
};

class WidgetIcon {
public:
    bool enable{false};
    std::string type{"_NONE"};
    int offsetX{0};
    int offsetY{0};

public:
    std::string Pack();
    static WidgetIcon Unpack(const std::string& _packed);
};

class WidgetText {
public:
    enum ALIGN_TYPE { LEFT, RIGHT, CENTER };

    bool enable{false};
    ALIGN_TYPE align{ALIGN_TYPE::CENTER};
    int offsetX{0};
    int offsetY{0};

public:
    std::string Pack();
    static WidgetText Unpack(const std::string& _packed);
};

class WidgetID {
public:
    uint32_t icon;
    uint32_t background{0U};
    uint32_t text1{0U};
    uint32_t text2{0U};
};