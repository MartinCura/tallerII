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

void PersonManager::split2(const std::string &s, char delim, vector<string> *elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems->push_back(item);
    }
}
vector<string>* PersonManager::split(const std::string &s, char delim) {
    std::vector<std::string>* elems = new std::vector<std::string>();
    split2(s, delim, elems);
    return elems;
}
long PersonManager::generateID() {
    std::string last_id;
    db->getKey("lastID", &last_id);
    std::string new_last_id = std::to_string(std::stol(last_id) + 1);
    db->putKey("lastID", &new_last_id);
    return std::stol(last_id) + 1;

}

long PersonManager::updateUser(Json::Value juser_new_information) {
    //The person already exists in the system and it wants to refresh his information
    Person* new_user = new Person(juser_new_information);
    std::string user_mail = new_user->getEmail();
    Person* old_user;

    try {
        old_user = getUserByMail(user_mail);
    } catch (UserNotFoundException& exception1) {
        delete new_user;
        throw UserNotFoundException(user_mail);
    }

    //Actualizar nombre completo de usuario;
    updateName(new_user, old_user);

    //Actualizar skill
    updateSkills(new_user, old_user);

    //Actualizar WorkHistory
    updateWorkHistory(new_user, old_user);

    //No olvidar el numero de recomendaciones
    new_user->setTotalRecommendations(old_user->getTotalOfRecommendations());

    //Actualizar password
    if (new_user->getPassword().compare("") == 0){
        new_user->setPassword(old_user->getPassword());
    }else {
        updatePassword(new_user, old_user);
    }


    //No olvidear la ubicación
    double latitude = (new_user->getLocation()->isEmpty()) ? old_user->getLocation()->getLatitude() : new_user->getLocation()->getLatitude();
    double longitude = (new_user->getLocation()->isEmpty()) ? old_user->getLocation()->getLongitude() : new_user->getLocation()->getLongitude();
    new_user->setLocation(latitude, longitude);

    saveUser(new_user);
    delete new_user;
    return new_user->getId();
}
void PersonManager::updateName(Person* new_user, Person* old_user) {
    string user_oldName = old_user->getFullName();
    string user_newName = new_user->getFullName();
    if (user_oldName.compare(user_newName) != 0) {
        deleteUserNameKey(old_user);
        saveUserNameKey(new_user);
    }
}
void PersonManager::updateWorkHistory(Person *new_user, Person *old_user) {
    std::vector<WorkHistory*> new_positions = new_user->getWorkHistory();
    std::vector<WorkHistory*> old_positions = old_user->getWorkHistory();
    std::string user_mail = new_user->getEmail();
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
void PersonManager::updateSkills(Person *new_user, Person *old_user) {

    std::vector<Skill*> new_skills = new_user->getSkills();
    std::vector<Skill*> old_skills = old_user->getSkills();

    //Controla nuevos skills.
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
        if (!skill_exists) saveSkill(new_skills[i], new_user->getEmail());
    }

    //Controla borrado de skills
    for (int j = 0; j < old_skills.size(); j++) {

        bool skill_exists = false;
        for (int j2 = 0; j2 < new_skills.size(); j2 ++) {
            std::string skill_name = new_skills[j]->getName();
            if (old_skills[j]->getName().compare(skill_name) == 0) {
                skill_exists = true;
                break;
            }
        }
        if (!skill_exists) deleteUserFromSkill(old_skills[j]->getName(), new_user->getEmail());

    }

}
void PersonManager::updatePassword(Person *new_user, Person *old_user) {
    string user_oldPassword = old_user->getPassword();
    string user_newPassword = new_user->getPassword();
    if (user_oldPassword.compare(user_newPassword) != 0) {
        deleteUserPasswordKey(old_user);
        saveUserPasswordKey(new_user);
    }
}

long PersonManager::savePerson(Json::Value juser_new_information, long forceID) {
    std::string user_mail, user_password, user_name;
    Person* user = new Person(juser_new_information);
    long uniqueId;

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
    //user->setPassword(""); //delete password before saving.

    saveUser(user);
    saveUserNameKey(user);
    saveUserIdKey(user);
    saveUserPasswordKey(user);

    saveSkills(user->getSkills(), user_mail);
    savePositions(user->getWorkHistory(), user_mail);

    if (user->getCurrentJob() != nullptr) {
        savePosition(user->getCurrentJob()->getPositionTitle(), user_mail);
    }

    delete user;
    return uniqueId;

}
void PersonManager::saveSkills(std::vector<Skill *> user_newSkills, string user_mail) {

    for (int index2 = 0; index2 < user_newSkills.size(); index2++) {
        saveSkill(user_newSkills[index2], user_mail);
    }
}
void PersonManager::saveSkill(Skill *skill, string user_mail) {
    string users_mail;
    std::string skill_name = skill->getName();
    std::transform(skill_name.begin(), skill_name.end(), skill_name.begin(), ::tolower);
    try {
        users_mail = getUserSkillKey(skill_name);
        users_mail += ",";
        users_mail += user_mail;
    } catch (KeyNotFound& exception1){
        users_mail = user_mail;
    }
    saveUserSkillKey(skill_name, users_mail);

}
void PersonManager::savePositions(std::vector<WorkHistory*> positions, string user_mail) {
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
        users_mail += ",";
        users_mail += user_mail;

    } catch (KeyNotFound& keyNotFound) {
        users_mail = user_mail;
    }
    saveUserJobPositionKey(position_title, users_mail);
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

            Json::Value juser = getJsonFromString(iterator->value().ToString());
            Person* user = new Person(juser);
            all_users->push_back(user);
        }
    }
    return all_users;
}

void PersonManager::deletePerson(long id) {

    Person *user = getUserById(id);
    try {
        deleteUserIdKey(user);
        deleteUserNameKey(user);
        deleteUserPasswordKey(user);
        for(int i = 0; i< user->getSkills().size(); i++) {
            std::string skill_name = user->getSkills()[i]->getName();
            deleteUserFromSkill(skill_name, user->getEmail());
        }
        for(int i = 0; i < user->getWorkHistory().size(); i++) {
            std::string job_title = user->getWorkHistory()[i]->getPositionTitle();
            deleteUserFromJobPosition(job_title, user->getEmail());
        }
        deleteUser(user);
    } catch (exception &exception1) {
        throw UserNotFoundException(id);
    }
}
void PersonManager::deleteUserFromSkill(string skill_name, string user_mail) {
    std::string output;
    try {
        output = getUserSkillKey(skill_name);
        std::vector<string>* users_mails = split(output, ',');
        string new_users = "";
        for (int i = 0; i < users_mails->size(); i++) {
            if (user_mail.compare((*users_mails)[i]) != 0) {
                if (new_users.compare("") == 0) new_users += (*users_mails)[i];
                else {
                    new_users += ",";
                    new_users += (*users_mails)[i];
                }
            }
        }
        delete users_mails;
        if (new_users.compare("") == 0) deleteUserSkillKey(skill_name);
        else saveUserSkillKey(skill_name, new_users);
    } catch (KeyNotFound& exception1) {
        throw BadImplementationException();
    }
}
void PersonManager::deleteUserFromJobPosition(string job_position, string user_mail) {
    std::string output;
    try {
        output = getUserJobPositionKey(job_position);
        std::vector<string>* users_position = split(output, ',');
        string new_users = "";
        for (int i = 0; i < users_position->size(); i++) {
            if (user_mail.compare((*users_position)[i]) != 0) {
                if (new_users.compare("") == 0) new_users += (*users_position)[i];
                else {
                    new_users += ",";
                    new_users += (*users_position)[i];
                }
            }
        }
        delete users_position;
        if (new_users.compare("") == 0) deleteUserSkillKey(job_position);
        else saveUserJobPositionKey(job_position, new_users);
    } catch (KeyNotFound& keyNotFound) {
        throw BadImplementationException();
    }
}


vector<Person *> *PersonManager::search_users_by(map<string, std::vector<string>*> *search_values) {
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
/// Busca por nombre completo del skill que entra por parámetro.
/// \param skills_search
/// \return lista de usuarios que tienen el skill.
vector<Person *> * PersonManager::searchBySkill(vector<string> *skills_search) {
    //fixme
    std::vector<string>* users_withSkill;
    std::vector<Person*>* users_result = new vector<Person*>();

    //Se filtra por uno solo de los skills primero, obteniendo todos los usuarios que tienen dicho skill.
    try {
        //Todos los usuarios con el skill en forma persona1, persona2, persona3
        users_withSkill = split(getUserSkillKey((*skills_search)[0]),',');
    } catch (KeyNotFound& exception1) {
        return users_result;
    }

    for(int i = 0; i < users_withSkill->size(); i++) {
        Person* user = getUserByMail((*users_withSkill)[i]);
        users_result->push_back(user);
    }

    std::vector<Person*>* result = new std::vector<Person*>();

    for (int i = 0; i < users_result->size(); i++) {
        if ((*users_result)[i]->has_every_skill(skills_search)) result->push_back((*users_result)[i]);
        else {
            delete (*users_result)[i];
        };
    }

    delete (users_result);
    delete users_withSkill;
    return result;

}
vector<Person *> *PersonManager::searchByJobPosition(vector<string> *job_positions) {
    std::vector<string>* users_withJobPosition;
    std::vector<Person*>* users_result = new vector<Person*>();
    for(int i = 0; i < job_positions->size(); i++) {
        std::transform((*job_positions)[i].begin(), (*job_positions)[i].end(), (*job_positions)[i].begin(), ::tolower);
    }
    try {
        users_withJobPosition = split(getUserJobPositionKey((*job_positions)[0]),',');
    } catch (KeyNotFound& keyNotFound) {
        return users_result;
    }

    for(int i = 0; i < users_withJobPosition->size(); i++) {
        Person* user = getUserByMail((*users_withJobPosition)[i]);
        users_result->push_back(user);
    }
    std::vector<Person*>* result = new std::vector<Person*>();

    for (int i = 0; i < users_result->size(); i++) {
        if ((*users_result)[i]->has_every_position(job_positions)) result->push_back((*users_result)[i]);
        else {
            delete (*users_result)[i];
        };
    }

    delete (users_result);
    delete users_withJobPosition;
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
vector<Person *> *PersonManager::searchByDistance(vector<string> *distance_search) {
    std::vector<Person*>* users_result = new vector<Person*>();
    leveldb::Slice startSlice = USER_MAIL_ID;
    leveldb::Slice endSlice = USER_NAME_ID;
    double latitud = stod((*distance_search)[0]);
    double longitud = stod((*distance_search)[1]);
    double max_distance = stod((*distance_search)[2]);
    Location* search_location = new Location();
    search_location->setLatitude(latitud);
    search_location->setLongitude(longitud);
    Json::Value juser;
    shared_ptr<leveldb::Iterator> iterator(db->newIterator());
    for(iterator->Seek(startSlice); iterator->Valid() && ((iterator->key()).ToString().compare(endSlice.ToString())) < 0 ; iterator->Next())
    {
        // Read the record
        if( !iterator->value().empty() )
        {
            std::string user_information = iterator->value().ToString();
            juser = this->getJsonFromString(user_information);
            Person* user = new Person(juser);
            double distance = search_location->getDistanceFrom(user->getLocation());
            if(distance < std::pow(max_distance, 2)) {
                //busqueda del usuario
                users_result->push_back(user);
            }

        }
    }
    delete search_location;
    return users_result;
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

            if(regex_match(iterator->key().ToString(), e)) {
                users_result->push_back(getUserByMail(iterator->value().ToString()));
            }
        }
    }
    return users_result;
}


vector<Contact*> PersonManager::getContactsByUserId(long id) {
    if (!this->userExists(id)) {
        throw UserNotFoundException(id);
    }
    RelationsManager *relationsManager = new RelationsManager(this->db);
    std::vector<Contact*> contacts = relationsManager->getContactsByUserId(id);
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

vector<Conversation*> PersonManager::getAllConversations(long userId) {
    vector<Conversation*> conversations;
    MessagesManager* messagesManager = new MessagesManager(this->db);
    std::vector<long>* ids = this->getAllUsersIds();
    for (vector<long>::iterator iter = ids->begin() ; iter != ids->end() ; iter++) {
        if (userId != *iter) {
            vector<Message*> messages = messagesManager->getMessages(userId, *iter);
            if (messages.size() != 0) {
                Conversation* conversation = new Conversation();
                conversation->setWith(*iter);
                Person* person = this->getUserById(*iter);
                conversation->setFirstName(person->getFirstName());
                conversation->setLastName(person->getLastName());
                delete person;
                conversation->setUnreadCount(messagesManager->getUnreadCount(userId, *iter));
                conversations.push_back(conversation);
            }
        }
    }
    delete messagesManager;
    delete ids;
    return conversations;
}

void PersonManager::setMessagesAsReceived(long fromUserId, long toUserId, vector<Message*> deliveredMessages) {
    MessagesManager* messagesManager = new MessagesManager(this->db);
    messagesManager->setMessagesAsReceived(fromUserId, toUserId, deliveredMessages);
    delete messagesManager;
}

vector<long> * PersonManager::getAllUsersIds() {
    // Get a database iterator
    std::vector<long>* ids = new vector<long>();

    leveldb::Slice startSlice = USER_UUID_ID;
    leveldb::Slice endSlice = USER_MAIL_ID;

    shared_ptr<leveldb::Iterator> iterator(db->newIterator());

    for(iterator->Seek(startSlice); iterator->Valid() && (iterator->key()).ToString().compare(endSlice.ToString()) ; iterator->Next())
    {
        // Read the record
        if( !iterator->value().empty() )
        {
            ids->push_back(std::stol(iterator->key().ToString().replace(0,strlen(USER_UUID_ID),"")));
        }
    }
    return ids;
}