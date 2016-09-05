var db = require("./db.js");

module.exports.job_positions = function (request, response) {
    response.json("HELLO WORD!");
};

module.exports.job_positions_add = function (request, response) {

  name = "test name";
  description = "test description";
  category = "test category";

  //TODO // text formatting
  db.pool.query('INSERT INTO job_positions(name, description, category) ' + 
    "values('" + name + "','" + description + "','" + category + "')"
    ).then(function () {
      response.status(200)
        .json({
          status: 'success',
          message: 'Se agrego el trabajo'
        });
    })
    .catch(function (err) {
      return next(err);
    });

}

module.exports.job_categories_get = function function_name(request, response) {
  response.status(200).json({
                          "categories": [{
                            "name": "sport_name1s",
                            "description": "description1"
                          }]
              });
}
module.exports.job_categories_add = function function_name(request, response) {
  
  db.create_category(request.body.category.name, request.body.category.description).then(function () {
      response.status(200)
        .json({
                name: request.body.category.name,
                description: request.body.category.description
            });
    })
}
module.exports.job_categories_modify = function function_name(request, response) {
  response.status(501);
}
module.exports.job_categories_delete = function function_name(request, response) {
  response.status(501);
}