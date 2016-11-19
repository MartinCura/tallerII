#include "PersonManager.h"
#include "../Exceptions/UserAlreadyExistsException.h"

#include <chrono>
#include <memory>

#define USER_MAIL_ID "user:mail_"
#define USER_UUID_ID "user:uuid_"
#define USER_PASSWORD "user:password_"

PersonManager::PersonManager(DBWrapper *db) {
    this->db = db;
}

PersonManager::~PersonManager() {}

long PersonManager::savePerson(long user_id, Json::Value person_json, long forceID) {
    std::string user_mail, user_password, user_information, person_string;
    Json::FastWriter fastWriter;
    long uniqueId;

    user_mail=  person_json["email"].asString();
    user_id = person_json["id"].asLargestInt();
    user_password = person_json["password"].asString();

    if (user_id == 0) {
        //Usuario nuevo
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
        uniqueId = user_id;
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

    std::string  user_mail;

    if (db->existsKey(USER_UUID_ID + std::to_string(id), &user_mail)) {
        return getPersonByMail(&user_mail);
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
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() );
    srand ((unsigned int)time(NULL));
    int rand = std::rand();
    return  labs(ms.count() << 3 + rand % 100);
}

bool PersonManager::userExists(long userId) {
    std::string result;
    return (db->existsKey(USER_UUID_ID + to_string(userId), &result));
}

bool PersonManager::userExists(string email) {
    string result;
    return (db->existsKey(USER_MAIL_ID + email, &result));
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
                Person* person = this->getPersonById(*iter);
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
