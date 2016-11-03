//
// Created by milena on 03/11/16.
//

#include "Security.h"

bool Security::hasPermissionToEdit(long userIdSession, long userIdToEdit) {
    return userIdSession == userIdToEdit;
}
