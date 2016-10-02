#include "PersonManager.h"
#include "../Exceptions/UserAlreadyExistsException.h"

#include <chrono>
#include <cstdlib>

PersonManager::PersonManager(std::string nameDB) {
    db = new DBWrapper();
    this->nameDB = nameDB;
    DBWrapper::ResponseCode status = db->openDb(nameDB);
    if (status == DBWrapper::ResponseCode::ERROR) {
        throw exception();
    }

    this->uniqueId = 0;

}

PersonManager::~PersonManager() {
    if (db != nullptr) {
        db->deleteDB();
    }

}

long PersonManager::savePerson(Json::Value person_json, long forceID) {
    std::string user_mail, user_information, output;
    long user_id;
    Json::FastWriter fastWriter;

    if (!person_json.isMember("email")) throw InvalidRequestException("Missing email");

    user_mail=  person_json["email"].asString();
    user_id = person_json["id"].asLargestInt();

    if (user_id == 0) {

        if (db->existsKey("user_" + user_mail, &user_information )) {
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
            output = fastWriter.write(person_json);
            db->puTKey("user_" + user_mail, &output);
            db->puTKey("user_" + std::to_string(uniqueId), &user_mail);
            return uniqueId;
        }
    } else {
        //The person already exists in the system and it wants to refresh his information
        output = fastWriter.write(person_json);
        db->puTKey("user_" + user_mail, &output);
        return uniqueId;
    }
}


void PersonManager::deletePerson(long id) {

    std::string user_mail, user_information;

    if (!db->existsKey("user_"+std::to_string(id), &user_mail)) {
        throw UserNotFoundException(UserNotFoundException::Message::id);
    }

    db->deleteKey("user_" + std::to_string(id));

    if (!db->existsKey("user_" + user_mail, &user_information)) {
        throw  UserNotFoundException(UserNotFoundException::Message::mail);
    }

    db->deleteKey("user_" + user_mail);
}


Person* PersonManager::getPersonById(long id) {

    std::string  user_mail, user;
    Json::Reader reader;
    Json::Value json_user;

    if (db->existsKey("user_" + std::to_string(id), &user_mail)) {
        try {
            return getPersonByMail(&user_mail);
        } catch (UserNotFoundException& exception1) {
            std::exception();
        }

        reader.parse( user.c_str(), json_user );
        return new Person(json_user);

    } else {
        //No se encontro el usuario
        throw UserNotFoundException(UserNotFoundException::Message::id);
    }

}

Person* PersonManager::getPersonByMail(std::string* user_mail) {
    std::string result;
    Json::Value json_user;
    Json::Reader reader;

    if (!db->existsKey("user_" + *user_mail, &result)) {
        throw UserNotFoundException(UserNotFoundException::Message::mail);
    }

    reader.parse( result.c_str(), json_user );
    return new Person(json_user);
}

//TODO: SEE IF IT WORKS
vector<Person*> PersonManager::getPersonFriendsById(long id) {
    std::string  user_mail, user, friend_user;
    Json::Reader reader;
    Json::Value json_friends;
    std::string friends_result;
    vector<Person*> friends;



    if (!db->existsKey("user_" + std::to_string(id), &user_mail)) {
        throw UserNotFoundException(UserNotFoundException::Message::id);
    }

    if (!db->existsKey("user_" + user_mail, &user)) {
        throw std::exception();
    }

    db->getKey( "user_friends_" + std::to_string(id), &friends_result);
    //Friends result has de id of every friend of the user_id
    reader.parse(friends_result.c_str(), json_friends);
    const Json::Value jfriends = json_friends["friends"];
    for (int index = 0; index < jfriends.size(); index++) {
        std::string friend_id = jfriends[index].asString();

        friends.push_back(this->getPersonById(std::stol(friend_id)));
    }
    return friends;
}

void PersonManager::destroyDB() {
    db->deleteDB();
    db->destroyDB(this->nameDB);

}

long PersonManager::generateID() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() );
    int rand = std::rand();
    return  ms.count() << 3 + rand % 100;
}
