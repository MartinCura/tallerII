#include <algorithm>
#include "DbBuilder.h"
#include "../session/SessionManager.h"


DbBuilder::DbBuilder() {
    this->namedb = new string(Config::getInstance()->get(Config::NAME_DB));
    db = DBWrapper::openDb(namedb);
}

DbBuilder::~DbBuilder() {
    DBWrapper::ResetInstance();
    delete namedb;
}

void DbBuilder::setLastId(){
    std::string last_id = std::to_string(3);
    db->putKey("lastID", &last_id);
}

void DbBuilder::loadUsers() {
    setLastId();
    PersonManager *personManager = new PersonManager(this->db);
    SessionManager* sessionManager = new SessionManager(this->db);
    RecommendationsManager* recommendationsManager = new RecommendationsManager(this->db);
    try {
        Person* person1 = this->getFakePerson1();
        personManager->savePerson(person1->serializeMe(), (long) 1);
        sessionManager->saveToken("tokenUser1", person1->getEmail());
        delete person1;
    } catch (UserAlreadyExistsException &exception) {}
    try {
        Person* person2 = this->getFakePerson2();
        personManager->savePerson(person2->serializeMe(), (long) 2);
        sessionManager->saveToken("tokenUser2", person2->getEmail());
        delete person2;
    } catch (UserAlreadyExistsException &exception) {}
    try {
        Person* person3 = this->getFakePerson3();
        personManager->savePerson(person3->serializeMe(), (long) 3);
        sessionManager->saveToken("tokenUser3", person3->getEmail());
        delete person3;
    } catch (UserAlreadyExistsException &exception) {}


    vector<long> users_id;
    vector<Skill*> skills_disponibles = getSkillsDisponibles();
    vector<WorkHistory*> trabajos_disponibles = getTrabajosDisponibles();
    for (int i = 0; i < 100; i++) {
        Person* user = new Person();
        user->setId(0);
        user->setCity("Ciudad" + std::to_string(rand() % 5));
        user->setDateOfBirth(std::to_string(rand() % 30 + 1) + "/" + std::to_string(rand() % 12 + 1) + "/1993");
        user->setEmail("usuariofalso" + std::to_string(i+1) + "@gmail.com");
        user->setLastName("ApellidoFalso" + std::to_string(i + 1));
        user->setFirstName("NombreFalso" + std::to_string(rand() % 100 + 1));
        user->setSummary("DescripcionFalsa" + std::to_string(i + 1));
        float x = -180.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(180.0 + 180.0)));
        float y =  -180.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(180.0 + 180.0)));
        user->setLocation(x, y);
        user->setPassword("usuarioFalso" + std::to_string(i + 1));
        int has_n_skills = rand() % 3;
        int has_n_jobs = rand() % 3;
        for (int j = 0; j < has_n_skills; j++) {
            user->addSkill(skills_disponibles[rand() % skills_disponibles.size()]);
        }

        for (int h = 0; h < has_n_jobs; h++) {
            user->addWorkHistory(trabajos_disponibles[rand() % trabajos_disponibles.size()]);
        }

        try {
            long user_id = personManager->savePerson(user->serializeMe());
            users_id.push_back(user_id);
            delete(user);

        }catch(UserAlreadyExistsException& exception1) {}

    }

    //Simulación de recomendación entre usuarios.
    if (users_id.size() != 0) {
        for (int k = 0; k < 200; k++) {
            long from_id = users_id[rand() % users_id.size()];
            long to_id = users_id[rand() % users_id.size()];
            while (to_id == from_id) {
                to_id = users_id[rand() % users_id.size()];
            }
            recommendationsManager->addRecommendation(from_id, to_id);
        }
    }

    delete personManager;
    delete recommendationsManager;
    delete sessionManager;
    std::vector<Skill*>::iterator it1 = skills_disponibles.begin();
    while (it1 != skills_disponibles.end()) {
        delete (*it1);
        it1 ++;
    }

    std::vector<WorkHistory*>::iterator it2 = trabajos_disponibles.begin();
    while (it2 != trabajos_disponibles.end()) {
        delete (*it2);
        it2 ++;
    }
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

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("IBM");
    workHistory1->setPositionTitle("JavaScript Developer");
    workHistory1->setFromDate("10/2012");
    workHistory1->setToDate("11/2014");
    person->addWorkHistory(workHistory1);

    WorkHistory* workHistory2 = new WorkHistory();
    workHistory2->setCompany("Amazon");
    workHistory2->setPositionTitle("Project Manager");
    workHistory2->setFromDate("12/2014");
    workHistory2->setToDate("");
    person->addWorkHistory(workHistory2);

    Skill* skill1 = new Skill();
    skill1->setName("JavaScript");
    skill1->setDescription("JavaScript programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("PM");
    skill2->setDescription("Project Management");
    skill2->setCategory("management");
    person->addSkill(skill2);

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

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("Microsoft");
    workHistory1->setPositionTitle("QA");
    workHistory1->setFromDate("10/2013");
    workHistory1->setToDate("11/2015");
    person->addWorkHistory(workHistory1);

    WorkHistory* workHistory2 = new WorkHistory();
    workHistory2->setCompany("E-bay");
    workHistory2->setPositionTitle("PHP Developer");
    workHistory2->setFromDate("12/2015");
    workHistory2->setToDate("");
    person->addWorkHistory(workHistory2);

    Skill* skill1 = new Skill();
    skill1->setName("Php");
    skill1->setDescription("PHP programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("QA");
    skill2->setDescription("Software Quality Assurance");
    skill2->setCategory("software");
    person->addSkill(skill2);

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

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("QA");
    workHistory1->setPositionTitle("Microsoft");
    workHistory1->setFromDate("10/2012");
    workHistory1->setToDate("11/2014");
    person->addWorkHistory(workHistory1);

    Skill* skill1 = new Skill();
    skill1->setName("JavaScript");
    skill1->setDescription("JavaScript programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    return person;
}

void DbBuilder::saveToken(string token, string user_mail) {
    std::string token_string, token2_string;
    time_t creation_time;
    Json::Value jtoken, jtoken2;
    Json::FastWriter fastWriter;
    char buff[20];

    creation_time = time(NULL);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&creation_time));
    std::string now  = std::string(buff);

    jtoken["user_token"] = token;
    jtoken["last_used"] = now;

    token_string = fastWriter.write(jtoken);

    jtoken2["user_mail"] = user_mail;
    jtoken2["last_used"] = now;

    token2_string = fastWriter.write(jtoken2);

    this->db->putKey("user:token_" + user_mail, &token_string);
    this->db->putKey("user:token_" + token, &token2_string);
}

vector<Skill *> DbBuilder::getSkillsDisponibles() {
    vector<Skill*> skills_disponibles;
    for (int i = 0; i < 50; i++) {
        Skill* skill = new Skill();
        skill->setName("skill" + std::to_string(i + 1));
        skills_disponibles.push_back(skill);
    }

    return skills_disponibles;
}

vector<WorkHistory *> DbBuilder::getTrabajosDisponibles() {
    vector<WorkHistory*> trabajos_disponibles;
    for (int i = 0; i < 10; i++) {
        WorkHistory* workHistory = new WorkHistory();
        workHistory->setPositionTitle("jobPosition" + std::to_string(i + 1));
        trabajos_disponibles.push_back(workHistory);
    }
    return trabajos_disponibles;
}
