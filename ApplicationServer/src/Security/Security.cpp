//
// Created by milena on 03/11/16.
//

#include "Security.h"

bool Security::hasPermissionToEdit(long userIdSession, long userIdToEdit) {
    return userIdSession == userIdToEdit;
}

bool Security::hasPermissionToDeleteUser(long userIdSession, long userIdToDelete) {
    return userIdSession == userIdToDelete;
}

bool Security::hasPermissionToContactUser(long userIdSession, long userIdAuthor) {
    return userIdAuthor == userIdSession;
}

bool Security::hasPermissionToSendMessage(long userIdSession, long userIdAuthor) {
    return userIdSession == userIdAuthor;
}

bool Security::hasPermissionToReadMessage(long userIdSession, long userIdFrom) {
    return userIdFrom == userIdSession;
}

bool Security::hasPermissionToDeleteProfilePicture(long id, long userId) {
    return id == userId;
}

bool Security::hasPermissionToEditProfilePicture(long id, long userId) {
    return id == userId;
}

bool Security::hasPermissionToSendRecommendations(long id, long userId) {
    return id == userId;
}

bool Security::hasPermissionToDeleteRecommendations(long id, long userId) {
    return id == userId;
}
