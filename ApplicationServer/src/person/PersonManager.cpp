#include "PersonManager.h"
#include "../Exceptions/UserAlreadyExistsException.h"
#include "../Exceptions/InvalidPasswordException.h"

#include <chrono>
#include <memory>

#define USER_MAIL_ID "user:mail_"
#define USER_UUID_ID "user:uuid_"
#define USER_PASSWORD "user:password_"

PersonManager::PersonManager(std::string nameDB) {
    db = new DBWrapper();
    this->nameDB = nameDB;

    DBWrapper::ResponseCode status = db->openDb(nameDB);
    if (status == DBWrapper::ResponseCode::ERROR) {
        throw ErrorOpeningDatabaseException();
    }

}

PersonManager::~PersonManager() {
    if (db != nullptr) {
        db->deleteDB();
    }

}

long PersonManager::savePerson(Json::Value person_json, long forceID) {
    std::string user_mail, user_password, user_information, person_string;
    long user_id;
    Json::FastWriter fastWriter;
    long uniqueId;

    //if (!person_json.isMember("email")) throw InvalidRequestException("Missing email");

    user_mail=  person_json["email"].asString();
    user_id = person_json["id"].asLargestInt();
    user_password = person_json["password"].asString();

    if (user_id == 0) {

        if (db->existsKey(USER_MAIL_ID + user_mail, &user_information )) {
            //Ya existe un usuario con dicho mail
            throw  UserAlreadyExistsException();
        } else {

            if (forceID != -1) {
                //Quiero forzar a que el usuario se guarde con un id que se pase por parámetro
                uniqueId = forceID;
            } else {
                uniqueId = generateID();
            }
            person_json["id"] = uniqueId;
            std::string password = "password";
            person_json.removeMember(password.c_str());
            person_string = fastWriter.write(person_json);
            db->puTKey(USER_MAIL_ID + user_mail, &person_string);
            db->puTKey(USER_UUID_ID + std::to_string(uniqueId), &user_mail);
            db->puTKey(USER_PASSWORD + user_mail, &user_password);

            return uniqueId;
        }
    } else {
        //The person already exists in the system and it wants to refresh his information
        person_string = fastWriter.write(person_json);
        db->puTKey(USER_MAIL_ID + user_mail, &person_string);
        return uniqueId;
    }
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
void PersonManager::deletePerson(long id) {

    std::string user_mail, user_information;

    if (!db->existsKey(USER_UUID_ID+std::to_string(id), &user_mail)) {
        throw UserNotFoundException(id);
    }

    db->deleteKey(USER_UUID_ID + std::to_string(id));

    if (!db->existsKey(USER_MAIL_ID + user_mail, &user_information)) {
        throw  UserNotFoundException(user_mail);
    }

    db->deleteKey(USER_MAIL_ID + user_mail);
}


Person* PersonManager::getPersonById(long id) {

    std::string  user_mail, user;
    Json::Reader reader;
    Json::Value json_user;

    if (db->existsKey(USER_UUID_ID + std::to_string(id), &user_mail)) {
        try {
            return getPersonByMail(&user_mail);
        } catch (UserNotFoundException& exception1) {
            std::exception();
        }
        //TODO Y ESTO PARA QUE ESTÁ?
        reader.parse( user.c_str(), json_user );
        json_user["password"] = ""; //Todo: a lo wacho
        return new Person(json_user);

    } else {
        //No se encontro el usuario
        throw UserNotFoundException(id);
    }

}

Person* PersonManager::getPersonByMail(std::string* user_mail) {
    std::string result;
    Json::Value json_user;
    Json::Reader reader;

    if (!db->existsKey(USER_MAIL_ID + *user_mail, &result)) {
        throw UserNotFoundException(*user_mail);
    }

    reader.parse( result.c_str(), json_user );
    std::string password = "password";
    json_user.removeMember(password.c_str()); //TODO a lo wacho
    return new Person(json_user);
}

vector<Contact*> PersonManager::getContactsByUserId(long id) {
    if (!this->userExists(id)) {
        throw UserNotFoundException(id);
    }
    RelationsManager *relationsManager = new RelationsManager(this->db);
    vector<Contact *> contacts = relationsManager->getContactsByUserId(id);
    delete relationsManager;
    return contacts;
}

void PersonManager::saveOrUpdateRelation(Json::Value relation) {
    if (!relation.isMember("action")) throw InvalidRequestException("Missing action");
    if (!relation.isMember("author_id")) throw InvalidRequestException("Missing author_id");
    if (!relation.isMember("contact_id")) throw InvalidRequestException("Missing contact_id");

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
    if (!recommendation.isMember("from")) throw InvalidRequestException("Missing from user");
    if (!recommendation.isMember("to")) throw InvalidRequestException("Missing to user");

    long fromUserId = recommendation["from"].asLargestInt();
    long toUserId = recommendation["to"].asLargestInt();
    if (!this->userExists(fromUserId)) throw UserNotFoundException(fromUserId);
    if (!this->userExists(toUserId)) throw UserNotFoundException(toUserId);
    if (fromUserId == toUserId) throw InvalidRequestException("From user and to user cannot be equals");

    RecommendationsManager* recommendationsManager = new RecommendationsManager(this->db);
    recommendationsManager->addRecommendation(fromUserId, toUserId);
    delete recommendationsManager;
}

void PersonManager::login(std::string user_mail, std::string user_password) {

    std::string user_bdd_pasword;

    if (!db->existsKey(USER_PASSWORD + user_mail, &user_bdd_pasword )) {
        //No existe un usuario con dicho mail en la base
        throw  UserNotFoundException(user_mail);
    }

    if (user_bdd_pasword.compare(user_password) != 0) {
        //La contraseña almacenada y la de inicio de sesión no coinciden
        throw InvalidPasswordException();
    }

    //TODO: GUARDAR CONEXIÓN ABIERTA
    //TODO: DEVOLVER TOKEN DE SESIÓN

}

void PersonManager::destroyDB() {
    db->deleteDB();
    db->destroyDB(this->nameDB);

}

long PersonManager::generateID() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() );
    srand (time(NULL));
    int rand = std::rand();
    return  labs(ms.count() << 3 + rand % 100);
}

bool PersonManager::userExists(long userId) {
    std::string result;
    return (db->existsKey(USER_UUID_ID + to_string(userId), &result));
}
