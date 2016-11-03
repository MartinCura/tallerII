//
// Created by milena on 03/11/16.
//

#ifndef APPLICATIONSERVER_SECURITY_H
#define APPLICATIONSERVER_SECURITY_H


class Security {
public:
    bool static hasPermissionToEdit(long userIdSession, long userIdToEdit);

    static bool hasPermissionToDeleteUser(long userIdSession, long userIdToDelete);

private:
    Security(){};

};


#endif //APPLICATIONSERVER_SECURITY_H
