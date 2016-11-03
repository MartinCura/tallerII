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
