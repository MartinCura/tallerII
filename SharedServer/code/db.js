var promise = require('bluebird');


var pg       = require('pg-promise')(); 
var sprintf = require("sprintf-js").sprintf;

// create a config to configure both dbing behavior
// and client options
// note: all config is optional and the environment variables
// will be read if the config is not present
var config = {
  user: 'postgres', //env var: PGUSER
  database: 'postgres', //env var: PGDATABASE
  password: 'postgres', //env var: PGPASSWORD
  port: 5432, //env var: PGPORT
  max: 10, // max number of clients in the db
  idleTimeoutMillis: 30000 // how long a client is allowed to remain idle before being closed
};


var db = new pg(config);
db.connect();

/*module.exports.db = db;*/

var cration_template = "INSERT INTO %s (%s) values(%s);";

module.exports.create_category = function(name, description){
  values_name = sprintf("%s,%s", "name", "description");
  values = sprintf("'%s','%s'", name, description);
  query_string = sprintf(cration_template, "categories", values_name, values);
  
  // Lo que está acá arriba iría en una función porque es siempre lo mismo para todas las tablas
  console.log(query_string);
  return db.query(query_string);

};

module.exports.get_categories = function(name, description){
  return db.any('select * from categories');
}

module.exports.modify_category = function(to_modify, name, description){
  return db.none('update categories set name=$1, description=$2 where name=$3',
  [name, description, to_modify]);
}

module.exports.delete_category = function(to_delete){
  query = sprintf("delete from categories where name='%s'", to_delete);
  console.log(query);
  // return db.query('delete from categories where name = "$1"', to_delete);
  return db.none(query);

}