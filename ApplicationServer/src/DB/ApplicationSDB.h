//
// The intent for this class is to bring a unique instance of levelDB while the Application is working,
// probably this will change in the future.
// For this purpose the class is configure as a Singleton Class
//

#ifndef APPLICATIONSERVER_APPLICATIONSDB_H
#define APPLICATIONSERVER_APPLICATIONSDB_H


#include <leveldb/db.h>

class ApplicationSDB {
public:
    ~ApplicationSDB();
    static leveldb::DB* getInstance();

private:
    static leveldb::DB* uniqueInstance;
    ApplicationSDB() {};



};


#endif //APPLICATIONSERVER_APPLICATIONSDB_H
