#include "PersonManager.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include "../Exceptions/BadImplementationException.h"
#include "../Exceptions/KeyNotFound.h"

#include <chrono>
#include <memory>
#include <algorithm>
#include <regex>
#include <set>



PersonManager::PersonManager(DBWrapper *db) {
    this->db = db;
}

PersonManager::~PersonManager() {}

void PersonManager::updateName(Person* new_user, Person* old_user) {
    string user_oldName = old_user->getFullName();
    string user_newName = new_user->getFullName();
    if (user_oldName.compare(user_newName) != 0) {
        deleteUserNameKey(old_user);
        saveUserNameKey(new_user);
    }
}

void PersonManager::updateWorkHistory(vector<WorkHistory*> new_positions, vector<WorkHistory *> old_positions, string user_mail) {
    for (int i = 0; i < new_positions.size(); i++) {
        bool position_exists = false;
        std::string user_newPosition = new_positions[i]->getPositionTitle();
        std::transform(user_newPosition.begin(), user_newPosition.end(), user_newPosition.begin(), ::tolower);
        for (int i2 = 0; i2 < old_positions.size(); i2++) {
            if (old_positions[i2]->getPositionTitle().compare(user_newPosition) == 0) {
                position_exists = true;
                break;
            }
        }
        if (!position_exists) savePosition(user_newPosition, user_mail);
    }
    for (int j = 0; j < old_positions.size(); j++) {

        bool position_exists = false;
        for (int j2 = 0; j2 < new_positions.size(); j2 ++) {
            std::string positionTitle = new_positions[j]->getPositionTitle();
            if (old_positions[j]->getPositionTitle().compare(positionTitle) == 0) {
                position_exists = true;
                break;
            }
        }
        if (!position_exists) deleteUserFromJobPosition(old_positions[j]->getPositionTitle(), user_mail);

    }

}
void PersonManager::updateSkills(vector<Skill *> new_skills, vector<Skill *> old_skills, string user_mail) {

    for (int i = 0; i < new_skills.size(); i++) {
        bool skill_exists = false;
        std::string user_newSkill = new_skills[i]->getName();
        std::transform(user_newSkill.begin(), user_newSkill.end(), user_newSkill.begin(), ::tolower);
        for (int i2 = 0; i2 < old_skills.size(); i2++) {
            if (old_skills[i2]->getName().compare(user_newSkill) == 0) {
                skill_exists = true;
                break;
            }
        }
        if (!skill_exists) saveSkill(new_skills[i], user_mail);
    }
    for (int j = 0; j < old_skills.size(); j++) {

        bool skill_exists = false;
        for (int j2 = 0; j2 < new_skills.size(); j2 ++) {
            std::string skill_name = new_skills[j]->getName();
            if (old_skills[j]->getName().compare(skill_name) == 0) {
                skill_exists = true;
                break;
            }
        }
        if (!skill_exists) deleteUserFromSkill(old_skills[j]->getName(), user_mail);

    }

}

long PersonManager::updateUser(Json::Value juser_new_information) {
    //The person already exists in the system and it wants to refresh his information
    Person* new_user = new Person(juser_new_information);
    std::string user_mail = new_user->getEmail();

    Person* old_user = getUserByMail(user_mail);

    //Actualizar nombre completo de usuario;
    updateName(new_user, old_user);
    updateSkills(new_user->getSkills(), old_user->getSkills(), user_mail);

    //Actualizar skill
    updateSkills(new_user->getSkills(), old_user->getSkills(), user_mail);

    //Actualizar WorkHistory
    vector<WorkHistory*> new_WorkHistory = new_user->getWorkHistory();
    if (new_user->getCurrentJob() != nullptr) new_WorkHistory.push_back(new_user->getCurrentJob());
    vector<WorkHistory*> old_WorkHistory = old_user->getWorkHistory();
    if (old_user->getCurrentJob() != nullptr) old_WorkHistory.push_back(old_user->getCurrentJob());
    updateWorkHistory(new_WorkHistory, old_WorkHistory, user_mail);

    //Actualizar Posicion Actual

    //No olvidar el numero de recomendaciones
    new_user->setTotalRecommendations(old_user->getTotalOfRecommendations());

    //No olvidear la ubicación
    double latitude = (new_user->getLocation()->isEmpty()) ? old_user->getLocation()->getLatitude() : new_user->getLocation()->getLatitude();
    double longitude = (new_user->getLocation()->isEmpty()) ? old_user->getLocation()->getLongitude() : new_user->getLocation()->getLongitude();
    new_user->setLocation(latitude, longitude);

    Json::FastWriter fastWriter;
    std::string person_string = fastWriter.write(new_user->serializeMe());
    db->putKey(USER_MAIL_ID + user_mail, &person_string);
    delete new_user;
    return new_user->getId();
}
long PersonManager::savePerson(Json::Value juser_new_information, long forceID) {
    std::string user_mail, user_password, user_name;
    Person* user = new Person(juser_new_information);
    long uniqueId;
    std::cout << juser_new_information.toStyledString()<<endl;
    user_mail= user->getEmail();
    user_name = user->getFullName();
    user_password = user->getPassword();


    if (userExistsMail(user_mail)) {
        delete user;
        throw  UserAlreadyExistsException();
    }

    uniqueId = (forceID != -1)? forceID:generateID();
    user->setId(uniqueId);
    user->setTotalRecommendations(0); //setting recommendation in 0
    user->setPassword(""); //delete password before saving.

    saveUser(user);
    saveUserNameKey(user);
    saveUserIdKey(user);
    saveUserPasswordKey(user, user_password);

    saveSkills(user->getSkills(), user_mail);
    savePositions(user->getWorkHistory(), user_mail);

    if (user->getCurrentJob() != nullptr) {
        savePosition(user->getCurrentJob()->getPositionTitle(), user_mail);
    }

    delete user;
    return uniqueId;

}
vector<Person*> * PersonManager::getAllUsers() {
    // Get a database iterator
    std::vector<Person*>* all_users = new vector<Person*>();

    leveldb::Slice startSlice = USER_MAIL_ID;
    leveldb::Slice endSlice = USER_NAME_ID;

    shared_ptr<leveldb::Iterator> iterator(db->newIterator());
    for(iterator->Seek(startSlice); iterator->Valid() && ((iterator->key()).ToString().compare(endSlice.ToString()) < 0) ; iterator->Next())
    {   // Read the record
        if( !iterator->value().empty()) {
            std::cout << iterator->key().ToString()<<endl;
            std::cout << endSlice.ToString() << endl;
            cout << (iterator->key()).ToString().compare(endSlice.ToString()) << endl;
            std::cout << iterator->value().ToString()<<endl;

            Json::Value juser = getJsonFromString(iterator->value().ToString());
            Person* user = new Person(juser);
            all_users->push_back(user);
        }
    }
    return all_users;
}
vector<Person *> * PersonManager::searchByName(vector<string> *user_searchName) {
    std::vector<Person*>* users_result = new vector<Person*>();
    leveldb::Slice startSlice = USER_NAME_ID;
    leveldb::Slice endSlice = USER_UUID_ID;
    std::string user_search = (*user_searchName)[0];
    std::transform(user_search.begin(), user_search.end(), user_search.begin(), ::tolower);
    std::regex e ("(.*)("+user_search+")(.*)");
    shared_ptr<leveldb::Iterator> iterator(db->newIterator());
    for(iterator->Seek(startSlice); iterator->Valid() && ((iterator->key()).ToString().compare(endSlice.ToString()) < 0) ; iterator->Next())
    {
        // Read the record
        if( !iterator->value().empty() )
        {
            cout << iterator->key().ToString() << endl;
            if(regex_match(iterator->key().ToString(), e)) {
                users_result->push_back(getUserByMail(iterator->value().ToString()));
            }
        }
    }
    return users_result;
}


void PersonManager::saveSkills(std::vector<Skill *> user_newSkills, string user_mail) {

    for (int index2 = 0; index2 < user_newSkills.size(); index2++) {
        saveSkill(user_newSkills[index2], user_mail);
    }
}


/// Busca por nombre completo del skill que entra por parámetro.
/// \param skills_search
/// \return lista de usuarios que tienen el skill.
vector<Person *> * PersonManager::searchBySkill(vector<string> *skills_search) {
    //fixme
    string users_withSkill;
    std::vector<Person*>* users_result = new vector<Person*>();

    //Se filtra por uno solo de los skills primero
    try {
        //Todos los usuarios con el skill en forma persona1, persona2, persona3
        users_withSkill = getUserSkillKey((*skills_search)[0]);
    } catch (KeyNotFound& exception1) {
        return users_result;
    }

    //Parseo
    size_t last = 1; size_t next = 0;
    std::string delimiter = ",";
    while ((next = users_withSkill.find(delimiter, last)) != string::npos) {
        std::string user_mail = users_withSkill.substr(last, next-last);
        Person* user;
        try {
            user = getUserByMail(user_mail);
            users_result->push_back(user);
        } catch (UserNotFoundException& exception) {
            deleteUserFromSkill((*skills_search)[0], user_mail);
        }
        last = next + 1;
    }

    std::vector<Person*>* result = new std::vector<Person*>();

    for (int i = 0; i < users_result->size(); i++) {
        if ((*users_result)[i]->has_every_skill(skills_search)) result->push_back((*users_result)[i]);
        else {delete (*users_result)[i];};
    }

    delete (users_result);
    return result;

}



void PersonManager::deletePerson(long id) {

    Person *user = getUserById(id);
    try {
        deleteUserIdKey(user);
        deleteUserNameKey(user);
        deleteUserPasswordKey(user);
        deleteUser(user);
    } catch (exception &exception1) {
        throw UserNotFoundException(id);
    }
}



vector<Contact*> PersonManager::getContactsByUserId(long id) {
    if (!this->userExists(id)) {
        throw UserNotFoundException(id);
    }
    RelationsManager *relationsManager = new RelationsManager(this->db);
    vector<Contact*> contacts = relationsManager->getContactsByUserId(id);
    delete relationsManager;
    return contacts;
}

void PersonManager::saveOrUpdateRelation(Json::Value relation) {
    long authorId = relation["author_id"].asLargestInt();
    long contactId = relation["contact_id"].asLargestInt();
    if (!this->userExists(authorId)) throw UserNotFoundException(authorId);
    if (!this->userExists(contactId)) throw UserNotFoundException(contactId);
    if (authorId == contactId) throw InvalidRequestException("Author user and contact user cannot be equals");

    string action = relation["action"].asString();

    RelationsManager* relationsManager = new RelationsManager(this->db);
    relationsManager->saveRelation(authorId, contactId, action);
    delete relationsManager;
}

void PersonManager::saveRecommendation(Json::Value recommendation) {
    long fromUserId = recommendation["from"].asLargestInt();
    long toUserId = recommendation["to"].asLargestInt();
    this->validateUsersOfRequest(fromUserId, toUserId);
    RecommendationsManager* recommendationsManager = new RecommendationsManager(this->db);
    recommendationsManager->addRecommendation(fromUserId, toUserId);
    delete recommendationsManager;
}

Json::Value PersonManager::getRecommendationsByUserId(long userId) {
    RecommendationsManager* recommendationsManager = new RecommendationsManager(this->db);
    Json::Value recommendations = recommendationsManager->getRecommendationsAsJson(userId);
    delete recommendationsManager;
    return recommendations;
}

void PersonManager::removeRecommendation(long fromUserId, long toUserId) {
    this->validateUsersOfRequest(fromUserId, toUserId);
    RecommendationsManager* recommendationsManager = new RecommendationsManager(this->db);
    recommendationsManager->removeRecommendation(fromUserId, toUserId);
    delete recommendationsManager;
}

string PersonManager::saveMessage(Json::Value request) {
    long fromUserId = request["from"].asLargestInt();
    long toUserId = request["to"].asLargestInt();
    string messageToSave = request["message"].asString();
    this->validateUsersOfRequest(fromUserId, toUserId);
    MessagesManager* messagesManager = new MessagesManager(this->db);
    string savedMessage = messagesManager->saveMessage(fromUserId, toUserId, messageToSave);
    delete messagesManager;
    return savedMessage;
}

vector<Message*> PersonManager::getMessages(long fromUserId, long toUserId) {
    this->validateUsersOfRequest(fromUserId, toUserId);
    MessagesManager* messagesManager = new MessagesManager(this->db);
    vector<Message*> messages = messagesManager->getMessages(fromUserId, toUserId);
    delete messagesManager;
    return messages;
}

void PersonManager::validateUsersOfRequest(long fromUserId, long toUserId) {
    if (!this->userExists(fromUserId)) throw UserNotFoundException(fromUserId);
    if (!this->userExists(toUserId)) throw UserNotFoundException(toUserId);
    if (fromUserId == toUserId) throw InvalidRequestException("From user and to user cannot be equals");
}

long PersonManager::generateID() {
    std::string last_id;
    db->getKey("lastID", &last_id);
    std::string new_last_id = std::to_string(std::stol(last_id) + 1);
    db->putKey("lastID", &new_last_id);
    return std::stol(last_id) + 1;
    /*std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() );
    int rand = std::rand();
    return  labs(ms.count() << 2 + rand % 100);*/
}

string PersonManager::getNotificationTokenByUserId(long userId) {
    if (!this->userExists(userId)) throw UserNotFoundException(userId);
    NotificationTokenManager* notificationTokenManager = new NotificationTokenManager(this->db);
    string token = notificationTokenManager->getTokenByUserId(userId);
    delete notificationTokenManager;
    return token;
}

void PersonManager::setOrUpdateNotificationToken(Json::Value request, long userId) {
    if (!request.isMember(NotificationTokenManager::TOKEN_KEY)) throw InvalidRequestException("Missing token");
    if (!this->userExists(userId)) throw UserNotFoundException(userId);
    string token = request[NotificationTokenManager::TOKEN_KEY].asString();
    NotificationTokenManager* notificationTokenManager = new NotificationTokenManager(this->db);
    notificationTokenManager->setOrUpdateToken(userId, token);
    delete notificationTokenManager;
}



void PersonManager::saveSkill(Skill *skill, string user_mail) {
    string users_mail;
    std::string skill_name = skill->getName();
    std::transform(skill_name.begin(), skill_name.end(), skill_name.begin(), ::tolower);
    try {
        users_mail = getUserSkillKey(skill_name);
        users_mail += user_mail;
        users_mail += ",";
    } catch (KeyNotFound& exception1){
        users_mail = ","+user_mail +",";
    }
    saveUserSkillKey(skill_name, users_mail);

}

void PersonManager::deleteUserFromSkill(string skill_name, string user_mail) {
    std::string output;
    try {
        output = getUserSkillKey(skill_name);
        regex reg("(.*)," + user_mail +"(.*)");
        output = regex_replace(output, reg, "$1$2");
        saveUserSkillKey(skill_name, output);
    } catch (KeyNotFound& exception1) {
        throw BadImplementationException();
    }
}

vector<Person *> *PersonManager::searchByJobPosition(vector<string> *job_positions) {
    std::string users_withJobPosition;
    std::vector<Person*>* users_result = new vector<Person*>();
    for(int i = 0; i < job_positions->size(); i++) {
        std::transform((*job_positions)[i].begin(), (*job_positions)[i].end(), (*job_positions)[i].begin(), ::tolower);
    }
    try {
        users_withJobPosition = getUserJobPositionKey((*job_positions)[0]);
    } catch (KeyNotFound& keyNotFound) {
        return users_result;
    }
    size_t last = 1; size_t next = 0;
    std::string delimiter = ",";
    while ((next = users_withJobPosition.find(delimiter, last)) != string::npos) {
        std::string user_mail = users_withJobPosition.substr(last, next-last);
        Person* user;
        try {
            user = getUserByMail(user_mail);
            users_result->push_back(user);
        } catch (UserNotFoundException& userNotFoundException) {
            deleteUserFromJobPosition((*job_positions)[0], user_mail);
        }
        last = next + 1;
    }

    std::vector<Person*>* result = new std::vector<Person*>();

    for (int i = 0; i < users_result->size(); i++) {
        if ((*users_result)[i]->has_every_position(job_positions)) result->push_back((*users_result)[i]);
        else {delete (*users_result)[i];};
    }

    delete (users_result);
    return result;

}

vector<Person *> *PersonManager::searchByMail(vector<string> *user_mail) {
    std::vector<Person*>* users_result = new vector<Person*>();
    leveldb::Slice startSlice = USER_MAIL_ID;
    leveldb::Slice endSlice = USER_NAME_ID;
    std::string user_search = (*user_mail)[0];
    std::transform(user_search.begin(), user_search.end(), user_search.begin(), ::tolower);
    std::regex e ("(.*)("+user_search+")(.*)");
    Json::Value juser;
    shared_ptr<leveldb::Iterator> iterator(db->newIterator());
    for(iterator->Seek(startSlice); iterator->Valid() && ((iterator->key()).ToString().compare(endSlice.ToString())) < 0 ; iterator->Next())
    {
        // Read the record
        if( !iterator->value().empty() )
        {
            std::cout<<iterator->key().ToString()<<endl;
            if(regex_match(iterator->key().ToString(), e)) {
                //busqueda del usuario
                std::string user_information = iterator->value().ToString();
                juser = this->getJsonFromString(user_information);
                Person* user = new Person(juser);
                users_result->push_back(user);
            }

        }
    }

    return users_result;
}

void PersonManager::savePositions(vector<WorkHistory*> positions, string user_mail) {
    for (int index2 = 0; index2 < positions.size(); index2++) {
        std::string jobPosition_title = positions[index2]->getPositionTitle();
        std::transform(jobPosition_title.begin(), jobPosition_title.end(), jobPosition_title.begin(), ::tolower);
        savePosition(jobPosition_title, user_mail);
    }
}

void PersonManager::savePosition(string position_title, string user_mail) {
    string users_mail;
    try {
        users_mail = getUserJobPositionKey(position_title);
        users_mail += user_mail;
        users_mail += ",";
    } catch (KeyNotFound& keyNotFound) {
        users_mail = ","+user_mail +",";
    }
    saveUserJobPositionKey(position_title, users_mail);
}

void PersonManager::deleteUserFromJobPosition(string job_position, string user_mail) {
    std::string output;
    try {
        output = getUserJobPositionKey(job_position);
        regex reg("(.*)," + user_mail +"(.*)");
        output = regex_replace(output, reg, "$1$2");
        saveUserJobPositionKey(job_position, output);
    } catch (KeyNotFound& keyNotFound) {
        throw BadImplementationException();
    }
}

vector<Person *> *PersonManager::search_users_by(map<string, vector<string>*> *search_values) {
    std::vector<Person*>* partial_result = nullptr;
    std::vector<Person*>* result = nullptr;

    if((*search_values)["skill"] != nullptr) partial_result = this->searchBySkill((*search_values)["skill"]);
    if(partial_result == nullptr) {
        //La busqueda se realizó por skills y no se hallaron resultados
        //o la busqueda no se realizó por skills
        if((*search_values)["position"] != nullptr) partial_result = this->searchByJobPosition((*search_values)["position"]);
    }
    else{
        if((*search_values)["position"] != nullptr) {
            for(int i = 0; i < partial_result->size(); i++) {
                if (!(*partial_result)[i]->has_every_position((*search_values)["position"])) delete (*partial_result)[i];
            }
        }
    }if(partial_result == nullptr) {
        if((*search_values)["mail"] != nullptr) partial_result = this->searchByMail((*search_values)["mail"]);
    }
    else{
        if((*search_values)["mail"] != nullptr) {
            std::string search_mail = (*((*search_values)["mail"]))[0];
            std::regex e ("(.*)("+search_mail+")(.*)");
            for(int i = 0; i < partial_result->size(); i++) {
                if ((*partial_result)[i] != nullptr) {
                    if(!regex_match((*partial_result)[i]->getEmail(), e)) delete (*partial_result)[i];
                }
            }
        }
    }if(partial_result == nullptr) {
        if((*search_values)["name"] != nullptr) partial_result = this->searchByName((*search_values)["name"]);
    }
    else{
        if ((*search_values)["name"] != nullptr) {
            std::string search_name = (*((*search_values)["name"]))[0];
            std::transform(search_name.begin(), search_name.end(), search_name.begin(), ::tolower);
            std::regex e ("(.*)("+search_name+")(.*)");
            for(int i = 0; i < partial_result->size(); i++) {
                if ((*partial_result)[i] != nullptr) {
                    if (!regex_match((*partial_result)[i]->getFirstName(), e) and !regex_match((*partial_result)[i]->getLastName(), e)) {
                        delete (*partial_result)[i];
                    }
                }
            }
        }
    }if(partial_result == nullptr) {
        if((*search_values)["distance"] != nullptr) partial_result = this->searchByDistance((*search_values)["distance"]);
    }
    else{
        if ((*search_values)["distance"] != nullptr) {
            vector<string> *distance_search = (*search_values)["distance"];
            double latitud = stod((*distance_search)[0]);
            double longitud = stod((*distance_search)[1]);
            double max_distance = stod((*distance_search)[3]);
            Location* search_location = new Location();
            search_location->setLatitude(latitud);
            search_location->setLongitude(longitud);
            for(int i = 0; i < partial_result->size(); i++) {
                if ((*partial_result)[i] != nullptr) {
                    double distance = search_location->getDistanceFrom((*partial_result)[i]->getLocation());
                    if(distance >= max_distance) {
                        //busqueda del usuario
                        delete (*partial_result)[i];
                    }
                }
            }
            delete search_location;
        }
    }
    result = new vector<Person*>();
    if (partial_result == nullptr) partial_result = new std::vector<Person*>();
    for (int k = 0; k < partial_result->size(); k++) {
        if ((*partial_result)[k] != nullptr) {
            result->push_back((*partial_result)[k]);
        }
    }
    delete partial_result;
    return result;

}

vector<Person *> *PersonManager::searchByDistance(vector<string> *distance_search) {
    std::vector<Person*>* users_result = new vector<Person*>();
    leveldb::Slice startSlice = USER_UUID_ID;
    leveldb::Slice endSlice = USER_NAME_ID;
    double latitud = stod((*distance_search)[0]);
    double longitud = stod((*distance_search)[1]);
    double max_distance = stod((*distance_search)[3]);
    Location* search_location = new Location();
    search_location->setLatitude(latitud);
    search_location->setLongitude(longitud);
    Json::Value juser;
    shared_ptr<leveldb::Iterator> iterator(db->newIterator());
    for(iterator->Seek(startSlice); iterator->Valid() && ((iterator->key()).ToString().compare(endSlice.ToString())) > 0 ; iterator->Next())
    {
        // Read the record
        if( !iterator->value().empty() )
        {
            std::string user_information = iterator->value().ToString();
            juser = this->getJsonFromString(user_information);
            Person* user = new Person(juser);
            double distance = search_location->getDistanceFrom(user->getLocation());
            if(distance < max_distance) {
                //busqueda del usuario
                users_result->push_back(user);
            }

        }
    }
    delete search_location;
    return users_result;
}


