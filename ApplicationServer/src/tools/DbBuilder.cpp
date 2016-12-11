#include <algorithm>
#include "DbBuilder.h"
#include "../session/SessionManager.h"

const char *JOB_POSITIONS_FILE = "{\n"
        "    \"job_positions\":[\n"
        "    {\n"
        "        \"name\":\"Developer\",\n"
        "        \"description\":\"Un desarrollador de software común y silvestre.\",\n"
        "        \"category\":\"Programación\"\n"
        "    },\n"
        "    {\n"
        "        \"name\":\"CEO\",\n"
        "        \"description\":\"Ruler of the world, empezando por una empresa.\",\n"
        "        \"category\":\"Management\"\n"
        "    },\n"
        "    {\n"
        "        \"name\":\"Barista\",\n"
        "        \"description\":\"Toco botones artísticamente.\",\n"
        "        \"category\":\"Cafetería\"\n"
        "    },\n"
        "    {\n"
        "        \"name\":\"Ninja\",\n"
        "        \"description\":\"Puede resolver toto tipo de problemas técnicos en una organización\",\n"
        "        \"category\":\"Programación\"\n"
        "    }\n"
        "]\n"
        "}";

const char *SKILLS_FILE = "{\n"
        "    \"skills\":[\n"
        "    {\n"
        "        \"name\":\"Café\",\n"
        "        \"description\":\"Habilidad para apretar el botón de la máquina de café sin le que tiemble el pulso.\",\n"
        "        \"category\":\"Cafetería\"\n"
        "    },\n"
        "    {\n"
        "        \"name\":\"Liderazgo\",\n"
        "        \"description\":\"Superpotencia mundial en hacer que los demás hagan lo que desee.\",\n"
        "        \"category\":\"Management\"\n"
        "    },\n"
        "    {\n"
        "        \"name\":\"PHP\",\n"
        "        \"description\":\"Conocimiento novato del lenguaje PHP.\",\n"
        "        \"category\":\"Programación\"\n"
        "    },\n"
        "    {\n"
        "        \"name\":\"JavaScript avanzado\",\n"
        "        \"description\":\"Conocimiento avanzado de lenguaje JavaScript.\",\n"
        "        \"category\":\"Programación\"\n"
        "    }\n"
        "]\n"
        "}";

DbBuilder::DbBuilder() {
    this->namedb = new string(Config::getInstance()->get(Config::NAME_DB));
    db = DBWrapper::openDb(namedb);
    this->loadAvailableJobPositions();
    this->loadAvailableSkills();
}

DbBuilder::~DbBuilder() {
    DBWrapper::ResetInstance();
    delete namedb;

    long sizePos = this->jobPositions.size();
    for (int i = 0; i != sizePos; i++) {
        this->jobPositions.pop_back();
    }

    long sizeSkills = this->skills.size();
    for (int i = 0; i != sizeSkills; i++) {
        delete this->skills[i];
        this->skills.pop_back();
    }
}

void DbBuilder::loadUsers() {
   // setLastId();
    PersonManager *personManager = new PersonManager(this->db);
    SessionManager* sessionManager = new SessionManager(this->db);
    vector<Person*> fakePeople;
    fakePeople.push_back(this->getFakePerson1());
    fakePeople.push_back(this->getFakePerson2());
    fakePeople.push_back(this->getFakePerson3());
    fakePeople.push_back(this->getFakePerson4());
    fakePeople.push_back(this->getFakePerson5());
    fakePeople.push_back(this->getFakePerson6());
    fakePeople.push_back(this->getFakePerson7());
    fakePeople.push_back(this->getFakePerson8());
    fakePeople.push_back(this->getFakePerson9());
    fakePeople.push_back(this->getFakePerson10());
    fakePeople.push_back(this->getFakePerson11());
    fakePeople.push_back(this->getFakePerson12());
    fakePeople.push_back(this->getFakePerson13());
    fakePeople.push_back(this->getFakePerson14());
    fakePeople.push_back(this->getFakePerson15());
    fakePeople.push_back(this->getFakePerson16());
    for (int i = 1; i <= 16; i++) {
        try {
            Person* person = fakePeople[i - 1];
            personManager->savePerson(person->serializeMe());
            string token = "tokenUser" + to_string(i);
            sessionManager->saveToken(token, person->getEmail());
        } catch (UserAlreadyExistsException &exception) {}
    }
    long size = fakePeople.size();
    for (int i = 0; i != size; i++) {
        delete fakePeople[i];
        fakePeople.pop_back();
    }
    delete personManager;
    delete sessionManager;
}

Person* DbBuilder::getFakePerson1() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("John");
    person->setLastName("Doe");
    person->setEmail("john@doe.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/01/1990");
    person->setCity("Buenos Aires");
    person->setSummary("Hi, this is John Doe");
    person->setLocation(-58.368368, -34.617589);
    person->addWorkHistory(this->getWorkHistory(0, "IBM", "10/2012", "11/2014"));
    person->addWorkHistory(this->getWorkHistory(1, "Amazon", "12/2014", ""));
    person->addSkill(this->getSkill(0));
    person->addSkill(this->getSkill(1));
    return person;
}

Person* DbBuilder::getFakePerson2() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Jane");
    person->setLastName("Doe");
    person->setEmail("jane@doe.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/11/1991");
    person->setCity("Madrid");
    person->setSummary("Hi, this is Jane Doe");
    person->setLocation(-58.368368, -34.617589);
    person->addWorkHistory(this->getWorkHistory(2, "Microsoft", "10/2013", "11/2015"));
    person->addWorkHistory(this->getWorkHistory(3, "E-bay", "12/2015", ""));
    person->addSkill(this->getSkill(2));
    person->addSkill(this->getSkill(3));
    return person;
}

Person* DbBuilder::getFakePerson3() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Carlos");
    person->setLastName("Rodriguez");
    person->setEmail("crodriguez@gmail.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/01/1990");
    person->setCity("CABA");
    person->setSummary("Hola, soy Carlos Rodriguez");
    person->setLocation(-58.368368, -34.617589);
    person->addWorkHistory(this->getWorkHistory(2, "Microsoft", "10/2012", "11/2014"));
    person->addSkill(this->getSkill(2));
    return person;
}

Person* DbBuilder::getFakePerson4() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Marty");
    person->setLastName("McFly");
    person->setEmail("marty@mcfly.com");
    person->setPassword("123abc");
    person->setDateOfBirth("06/09/1968");
    person->setCity("Hill Valley, California");
    person->setSummary("Mi gran habilidad es viajar por el tiempo.");
    person->setLocation(-58.368370, -34.617500);
    person->addWorkHistory(this->getWorkHistory(0, "IBM", "10/2012", "11/2014"));
    person->addWorkHistory(this->getWorkHistory(1, "Amazon", "12/2014", ""));
    person->addSkill(this->getSkill(1));
    person->addSkill(this->getSkill(2));
    return person;
}

Person* DbBuilder::getFakePerson5() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Tony");
    person->setLastName("Stark");
    person->setEmail("tony@stark.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/01/1970");
    person->setCity("Manhattan");
    person->setSummary("Mi hobbie es salvar al mundo.");
    person->setLocation(-58.368368, -34.617589);

    person->addWorkHistory(this->getWorkHistory(2, "Microsoft", "10/2012", "11/2014"));
    person->addSkill(this->getSkill(2));

    return person;
}

Person* DbBuilder::getFakePerson6() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Madonna");
    person->setLastName("Ciccone");
    person->setEmail("madonna@ciccone.com");
    person->setPassword("123abc");
    person->setDateOfBirth("16/08/1958");
    person->setCity("Michigan");
    person->setSummary("soy la reina del pop.");
    person->setLocation(-58.368468, -34.627589);
    person->addWorkHistory(this->getWorkHistory(0, "Sony", "10/1970", ""));
    person->addSkill(this->getSkill(0));
    return person;
}

Person* DbBuilder::getFakePerson7() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Lionel");
    person->setLastName("Messi");
    person->setEmail("lionel@messi.com");
    person->setPassword("123abc");
    person->setDateOfBirth("24/06/1987");
    person->setCity("Barcelona");
    person->setSummary("Me gusta mucho jugar al futbol.");
    person->setLocation(-58.368468, -31.627529);
    person->addWorkHistory(this->getWorkHistory(1, "Barcelona FC", "10/2000", ""));
    person->addSkill(this->getSkill(1));
    person->addSkill(this->getSkill(2));
    return person;
}

Person* DbBuilder::getFakePerson8() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Jack");
    person->setLastName("Sparrow");
    person->setEmail("jack@sparrow.com");
    person->setPassword("123abc");
    person->setDateOfBirth("24/06/1980");
    person->setCity("Caribe");
    person->setSummary("Soy un gran pirata.");
    person->setLocation(-57.368468, -31.627529);
    person->addWorkHistory(this->getWorkHistory(1, "La Perla Negra", "10/2000", ""));
    person->addSkill(this->getSkill(3));
    return person;
}

Person* DbBuilder::getFakePerson9() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Maria");
    person->setLastName("Sanchez");
    person->setEmail("maria@sanchez.com");
    person->setPassword("123abc");
    person->setDateOfBirth("04/06/1975");
    person->setCity("Los Angeles");
    person->setSummary("Hola, soy Maria Sanchez");
    person->setLocation(-57.368468, -21.617529);
    person->addWorkHistory(this->getWorkHistory(0, "Microsoft", "10/2000", "10/2014"));
    person->addWorkHistory(this->getWorkHistory(3, "IBM", "11/2014", ""));
    person->addSkill(this->getSkill(2));
    person->addSkill(this->getSkill(3));
    return person;
}

Person* DbBuilder::getFakePerson10() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Antonella");
    person->setLastName("Roccuzzo");
    person->setEmail("antonella@roccuzzo.com");
    person->setPassword("123abc");
    person->setDateOfBirth("26/02/1988");
    person->setCity("Barcelona");
    person->setSummary("Hola, soy la novia de Messi.");
    person->setLocation(-57.348478, -21.617529);
    person->addWorkHistory(this->getWorkHistory(3, "Barcelona FC", "10/2000", ""));
    person->addSkill(this->getSkill(0));
    return person;
}

Person* DbBuilder::getFakePerson11() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Luke");
    person->setLastName("Skywalker");
    person->setEmail("luke@skywalker.com");
    person->setPassword("123abc");
    person->setDateOfBirth("26/02/1968");
    person->setCity("Tatooine");
    person->setSummary("Soy Luke Skywalker, mejor conocido como el hijo de Darth Vader.");
    person->setLocation(-55.348478, -11.617529);
    person->addWorkHistory(this->getWorkHistory(2, "Granjero", "09/1969", "09/1070"));
    person->addWorkHistory(this->getWorkHistory(1, "La Rebelion", "10/1070", ""));
    person->addSkill(this->getSkill(0));
    person->addSkill(this->getSkill(1));
    return person;
}

Person* DbBuilder::getFakePerson12() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Britney");
    person->setLastName("Spears");
    person->setEmail("britney@spears.com");
    person->setPassword("123abc");
    person->setDateOfBirth("01/12/1981");
    person->setCity("Mississippi");
    person->setSummary("Ups, I did it again.");
    person->setLocation(-55.348478, -11.617529);
    person->addWorkHistory(this->getWorkHistory(2, "Sony", "01/1992", ""));
    person->addSkill(this->getSkill(3));
    return person;
}

Person* DbBuilder::getFakePerson13() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Evan");
    person->setLastName("Baxter");
    person->setEmail("evan@baxter.com");
    person->setPassword("123abc");
    person->setDateOfBirth("22/07/2007");
    person->setCity("Manhattan");
    person->setSummary("Soy un presentador de noticias.");
    person->setLocation(-57.348478, -11.617529);
    person->addWorkHistory(this->getWorkHistory(3, "7 News", "01/1992", ""));
    person->addSkill(this->getSkill(1));
    return person;
}

Person* DbBuilder::getFakePerson14() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Michael");
    person->setLastName("Jordan");
    person->setEmail("michael@jordan.com");
    person->setPassword("123abc");
    person->setDateOfBirth("17/02/1963");
    person->setCity("Brooklyn");
    person->setSummary("Me gusta mucho jugar al basket.");
    person->setLocation(-57.348378, -11.617529);
    person->addWorkHistory(this->getWorkHistory(0, "Starbucks", "01/1992", "01/1994"));
    person->addWorkHistory(this->getWorkHistory(1, "NBA", "02/1994", ""));
    person->addSkill(this->getSkill(1));
    person->addSkill(this->getSkill(3));
    return person;
}

Person* DbBuilder::getFakePerson15() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Harry");
    person->setLastName("Potter");
    person->setEmail("harry@potter.com");
    person->setPassword("123abc");
    person->setDateOfBirth("17/12/1990");
    person->setCity("Hogwarts");
    person->setSummary("Soy un gran mago.");
    person->setLocation(-55.348378, -11.627529);
    person->addWorkHistory(this->getWorkHistory(0, "Starbucks", "01/1992", "01/1994"));
    person->addWorkHistory(this->getWorkHistory(1, "Wendys", "01/1995", "01/1999"));
    person->addWorkHistory(this->getWorkHistory(2, "Hogwarts", "02/2000", ""));
    person->addSkill(this->getSkill(1));
    person->addSkill(this->getSkill(2));
    return person;
}

Person* DbBuilder::getFakePerson16() {
    Person* person = new Person();
    person->setId(0);
    person->setFirstName("Maggie");
    person->setLastName("Sparrow");
    person->setEmail("jacksparrow@idem.com");
    person->setPassword("123abc");
    person->setDateOfBirth("24/06/1992");
    person->setCity("Caribe");
    person->setSummary("Soy un gran pirata.");
    person->setLocation(-57.368468, -31.627529);
    person->addWorkHistory(this->getWorkHistory(1, "La Perla Negra", "10/2000", ""));
    person->addSkill(this->getSkill(3));
    return person;
}

WorkHistory* DbBuilder::getWorkHistory(int jobPositionIndex, string company, string fromDate, string toDate) {
    WorkHistory* workHistory = new WorkHistory();
    workHistory->setCompany(company);
    workHistory->setPositionTitle(this->jobPositions[jobPositionIndex]);
    workHistory->setFromDate(fromDate);
    workHistory->setToDate(toDate);
    return workHistory;
}

Skill* DbBuilder::getSkill(int skillIndex) {
    Skill* skill = new Skill();
    skill->setName(this->skills[skillIndex]->getName());
    skill->setDescription(this->skills[skillIndex]->getDescription());
    skill->setCategory(this->skills[skillIndex]->getCategory());
    return skill;
}

void DbBuilder::loadAvailableJobPositions() {
    string jobPositionsAsString = JOB_POSITIONS_FILE;
    Json::Value jobPositionsAsArray = this->parseFile(jobPositionsAsString)["job_positions"];
    for (int index = 0; index < jobPositionsAsArray.size(); index++) {
        this->jobPositions.push_back(jobPositionsAsArray[index]["name"].asString());
    }
}

void DbBuilder::loadAvailableSkills() {
    string skillsAsString = SKILLS_FILE;
    Json::Value skillsAsArray = this->parseFile(skillsAsString)["skills"];
    for (int index = 0; index < skillsAsArray.size(); index++) {
        Skill* skill = new Skill(skillsAsArray[index]);
        this->skills.push_back(skill);
    }
}

void DbBuilder::setLastId(){
    std::string last_id = std::to_string(3);
    db->putKey("lastID", &last_id);
}

Json::Value DbBuilder::parseFile(string fileContent) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(fileContent, root);
    if (!parsingSuccessful) {
        throw "Invalid Config file.";
    }
    return root;
}
