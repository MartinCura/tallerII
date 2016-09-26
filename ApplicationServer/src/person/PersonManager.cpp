#include "PersonManager.h"
#include "../DB/ApplicationSDB.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include "../Exceptions/UserNotFoundException.h"

PersonManager::PersonManager() {

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::DB::Open(options, "/tmp/appDB", &db);
    uniqueId = 0;
}

PersonManager::~PersonManager() {
    delete db;
    leveldb::DestroyDB("/tmp/appDB", leveldb::Options());

}

Person* PersonManager::getPersonById(int id) {
    //FIXME: sacar una vez que este la base de datos
    switch (id) {
        case 1:
            return this->getFakePerson1();
        case 2:
            return this->getFakePerson2();
    }
    throw UserNotFoundException(id);
}

//FIXME: sacar una vez que este la base de datos
Person* PersonManager::getFakePerson1() {
    Person* person = new Person();
    person->setId(1);
    person->setFirstName("John");
    person->setLastName("Doe");
    person->setEmail("John@doe.com");
    person->setDateOfBirth("01/01/1990");
    person->setCity("Buenos Aires");
    person->setSummary("Hi, this is John Doe");

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
    skill1->setDescription("JavaScrip programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("PM");
    skill2->setDescription("Project Management");
    skill2->setCategory("management");
    person->addSkill(skill2);

    return person;
}

//FIXME: sacar una vez que este la base de datos
Person* PersonManager::getFakePerson2() {
    Person* person = new Person();
    person->setId(2);
    person->setFirstName("Jane");
    person->setLastName("Doe");
    person->setEmail("Jane@doe.com");
    person->setDateOfBirth("01/11/1991");
    person->setCity("Madrid");
    person->setSummary("Hi, this is Jane Doe");

    WorkHistory* workHistory1 = new WorkHistory();
    workHistory1->setCompany("Microsoft");
    workHistory1->setPositionTitle("QA");
    workHistory1->setFromDate("10/2013");
    workHistory1->setToDate("11/2015");
    person->addWorkHistory(workHistory1);

    WorkHistory* workHistory2 = new WorkHistory();
    workHistory2->setCompany("E-bay");
    workHistory2->setPositionTitle("Php Developer");
    workHistory2->setFromDate("12/2015");
    workHistory2->setToDate("");
    person->addWorkHistory(workHistory2);

    Skill* skill1 = new Skill();
    skill1->setName("Php");
    skill1->setDescription("Php programming language");
    skill1->setCategory("software");
    person->addSkill(skill1);

    Skill* skill2 = new Skill();
    skill2->setName("QA");
    skill2->setDescription("Software Quality Assurance");
    skill2->setCategory("software");
    person->addSkill(skill2);

    return person;
}

void PersonManager::savePerson(Json::Value person_json) {
    //Se supone que el usuario aun no fue creado entonces debería tener id = 0
    //El mail es único.

    std::string user_mailid = "user_" + person_json["email"].asString();

    if (person_json["id"] == 0) {

        std::string result;
        leveldb::Status s = db->Get(leveldb::ReadOptions(), user_mailid, &result);
        if (!s.IsNotFound()) {
            //Ya existe un usuario con dicho mail
            throw  UserAlreadyExistsException();
        } else {
            std::string user_id = "user_" + std::to_string(this->uniqueId);
            Json::FastWriter fastWriter;
            std::string output = fastWriter.write(person_json);
            db->Put(leveldb::WriteOptions(), user_mailid, output);
            db->Put(leveldb::WriteOptions(), user_id, user_mailid );
        }
    } else {
        //The person already exists in the system and it wants to refresh his information
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(person_json);
        db->Put(leveldb::WriteOptions(), user_mailid, output);

    }
}

Person* PersonManager::getPersonById(long id) {

    std::string user_id = "user_" + std::to_string(id);
    std::string user_mailId, user;

    leveldb::Status user_mail_status = db->Get(leveldb::ReadOptions(), user_id, &user_mailId);

    if (!user_mail_status.IsNotFound()) {
        //Se encontro el usuario
        leveldb::Status user_status = db->Get(leveldb::ReadOptions(), user_mailId, &user);
        if (user_status.IsNotFound()) {
            //No debiera suceder
            throw std::exception();
        }
        //TODO: Chequear si funciona
        return new Person(user);

    } else {
        //No se encontro el usuario
        throw UserNotFoundException(id);
    }

}

