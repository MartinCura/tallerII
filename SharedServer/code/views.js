var db = require("./db.js");

module.exports.job_positions = function(request, response) {
    response.json("HELLO WORD!");
};

module.exports.job_positions_add = function(request, response) {

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

module.exports.job_categories_get = function(request, response) {
  db.get_categories()
    .then(function (data) {
      response.status(200)
        .json({
          categories: data.rows
        });
    })
}

function json_of_categories(name, description) {
  return {"category" :  {
                "name": name,
                "description": description
            }};
}

module.exports.job_categories_add = function(request, response) {
  
  db.create_category(request.body.category.name, request.body.category.description).then(function () {
      response.status(200)
        .json(json_of_categories(request.body.category.name, request.body.category.description));
    })
}
module.exports.job_categories_modify = function(request, response) {
  db.modify_category(request.params.category, request.body.category.name, request.body.category.description).then(function(data){
    response.status(200).json(json_of_categories(request.body.category.name, request.body.category.description));
  });
}
module.exports.job_categories_delete = function(request, response) {
  response.status(501);
}