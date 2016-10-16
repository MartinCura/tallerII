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
  idleTimeoutMillis: 30000, // how long a client is allowed to remain idle before being closed,
  //host: "db"
  host: "db"
};


var db = new pg(process.env.DATABASE_URL? process.env.DATABASE_URL : config);
db.connect();

/*module.exports.db = db;*/

var cration_template = "INSERT INTO %s (%s) values(%s);";

module.exports.create_category = function(name, description){
  values_name = sprintf("%s,%s", "name", "description");
  values = sprintf("'%s','%s'", name, description);
  query_string = sprintf(cration_template, "categories", values_name, values);
  
  // Lo que está acá arriba iría en una función porque es siempre lo mismo para todas las tablas
  //console.log(query_string);
  return db.query(query_string);

};

module.exports.get_categories = function(name, description){
  return db.any('SELECT * from categories');
}

module.exports.modify_category = function(to_modify, name, description){
  return db.none('UPDATE categories set name=$1, description=$2 where name=$3',
  [name, description, to_modify]);
}

module.exports.delete_category = function(to_delete){
  query = sprintf("DELETE FROM categories where name='%s'", to_delete);
  return db.none(query);

}

module.exports.get_job_positions = function(){
  return db.any("SELECT * FROM job_positions");
}

module.exports.create_job_position = function(category, name, description) {
  return db.none("INSERT INTO job_positions(name, description, category) values ($1, $2, $3)",
    [name, description, category]);
}

module.exports.get_job_positions_by_category = function(category){
  return db.any("SELECT * FROM job_positions where category=$1", [category]);
}

module.exports.job_position_modify = function(category, position_name, new_position_name, new_description, new_category) {
  return db.none('UPDATE job_positions set name=$1, description=$2, category=$5 where name=$3 AND category=$4',
   [new_position_name, new_description, position_name, category, new_category]);
}

module.exports.job_positions_delete = function(category, position_name) {
  return db.result("DELETE FROM job_positions where name=$1 AND category=$2", [position_name, category]);
}

module.exports.skills_get = function(){
  return db.any("SELECT * FROM skills"); 
}

module.exports.create_skills = function(category, name, description){
  return db.none("INSERT INTO skills(name, description, category) values ($1, $2, $3)",
    [name, description, category]);
}

module.exports.get_skills_by_category = function function_name(category) {
  return db.any("SELECT * FROM skills where category=$1", [category]);

}

module.exports.skills_modify = function(category, skill_name, new_name, new_description, new_category){
  return  db.none('UPDATE skills set name=$1, description=$2, category=$5 where name=$3 AND category=$4',
   [new_name, new_description, skill_name, category, new_category]);
}

module.exports.delete_skills = function(category, skill_name){
  return db.result("DELETE FROM skills where name=$1 AND category=$2", [skill_name, category]);
}