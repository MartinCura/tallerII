#include "RelationsManager.h"

const string RelationsManager::CONTACT_KEY = "contact_";
const string RelationsManager::ACTIVE = "active";
const string RelationsManager::RECEIVED = "received";
const string RelationsManager::REQUESTED = "requested";
const string RelationsManager::ADD_CONTACT_ACTION = "add_contact";
const string RelationsManager::ACCEPT_CONTACT_ACTION = "accept_contact";
const string RelationsManager::REMOVE_CONTACT_ACTION = "remove_contact";

RelationsManager::RelationsManager(DBWrapper* db) {
    this->db = db;
}

RelationsManager::~RelationsManager() {}

void RelationsManager::saveRelation(long authorId, long contactId, string action) {
    if (action == ADD_CONTACT_ACTION) {
        this->addOrUpdateRelation(authorId, contactId, REQUESTED);
        this->addOrUpdateRelation(contactId, authorId, RECEIVED);
    } else if (action == ACCEPT_CONTACT_ACTION) {
        this->addOrUpdateRelation(authorId, contactId, ACTIVE);
        this->addOrUpdateRelation(contactId, authorId, ACTIVE);
    } else if (action == REMOVE_CONTACT_ACTION) {
        this->removeRelation(authorId, contactId);
        this->removeRelation(contactId, authorId);
    } else {
        throw InvalidRequestException(action + " is not a valid action");
    }
}

void RelationsManager::addOrUpdateRelation(long keyUserId, long contactId, string status) {
    vector<Contact*> currentRelationships = this->getContactsByUserId(keyUserId);
    vector<Contact*> newRelationships;
    bool updated = false;
    for (vector<Contact *>::size_type i = 0; i < currentRelationships.size(); i++) {
        Contact* contact = currentRelationships[i];
        if (contact->getUserId() == contactId) {
            contact->setStatus(status);
            updated = true;
        }
        newRelationships.push_back(contact);
    }
    if (!updated) {
        Contact* contact = new Contact();
        contact->setUserId(contactId);
        contact->setStatus(status);
        newRelationships.push_back(contact);
    }
    Json::Value allContactsAsJson;
    for (vector<Contact *>::size_type i = 0; i < newRelationships.size(); i++) {
        Contact* contact = newRelationships[i];
        Json::Value contactAsJson;
        contactAsJson["user_id"] = contact->getUserId();
        contactAsJson["status"] = contact->getStatus();
        allContactsAsJson.append(contactAsJson);
        delete contact;
    }
    Json::FastWriter fastWriter;
    string allContactsAsString = fastWriter.write(allContactsAsJson);
    this->db->putKey(CONTACT_KEY + to_string(keyUserId), &allContactsAsString);
}

void RelationsManager::removeRelation(long keyUserId, long contactId) {
    vector<Contact*> currentRelationships = this->getContactsByUserId(keyUserId);
    vector<Contact*> newRelationships;
    for (vector<Contact *>::size_type i = 0; i < currentRelationships.size(); i++) {
        Contact* contact = currentRelationships[i];
        if (contact->getUserId() != contactId) {
            newRelationships.push_back(contact);
        } else {
            delete contact;
        }
    }
    Json::Value allContactsAsJson;
    for (vector<Contact *>::size_type i = 0; i < newRelationships.size(); i++) {
        Contact* contact = newRelationships[i];
        Json::Value contactAsJson;
        contactAsJson["user_id"] = contact->getUserId();
        contactAsJson["status"] = contact->getStatus();
        allContactsAsJson.append(contactAsJson);
        delete contact;
    }
    Json::FastWriter fastWriter;
    string allContactsAsString = fastWriter.write(allContactsAsJson);
    this->db->putKey(CONTACT_KEY + to_string(keyUserId), &allContactsAsString);
}

vector<Contact*> RelationsManager::getContactsByUserId(long userId) {
    vector<Contact*> contacts;
    std::string result;
    if (!this->db->existsKey(CONTACT_KEY + to_string(userId), &result)) {
        return contacts;
    }
    Json::Reader reader;
    Json::Value contactsAsJson;
    reader.parse(result.c_str(), contactsAsJson);
    for (int index = 0; index < contactsAsJson.size(); index++) {
        Contact* contact = new Contact(contactsAsJson[index]);
        contacts.push_back(contact);
    }
    return contacts;
}
