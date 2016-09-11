var db = require("./db.js");
var HttpStatus = require('http-status-codes');


module.exports.job_positions = function(request, response) {
    response.json("HELLO WORD!");
};


module.exports.job_categories_get = function(request, response) {
  db.get_categories()
    .then(function (data) {
      response.status(HttpStatus.OK)
        .json({
          categories: data
        });
    })
}

function json_of_item(name, description, itemName) {
  out = {};
  out[itemName] = {
                "name": name,
                "description": description
            };
  return out;
}

module.exports.job_categories_add = function(request, response) {
  
  db.create_category(request.body.category.name, request.body.category.description).then(function () {
      response.status(HttpStatus.OK)
        .json(json_of_item(request.body.category.name, request.body.category.description, "category"));
    })
}
module.exports.job_categories_modify = function(request, response) {
  db.modify_category(request.params.category, request.body.category.name, request.body.category.description).then(function(){
    response.status(HttpStatus.OK).json(json_of_item(request.body.category.name, request.body.category.description, "category"));
  });
}
module.exports.job_categories_delete = function(request, response) {
  db.delete_category(request.params.category).then(function(){
    response.status(HttpStatus.OK).json({});
  });
}

module.exports.job_positions_get = function(request, response){
  db.get_job_positions().then(function (data) {
    response.status(HttpStatus.OK).json({job_positions: data});
  })
}


module.exports.job_positions_add = function(request, response) {
  db.create_job_position(request.params.category, request.body.job_position.name, request.body.job_position.description).then(
    function(){
      response.status(HttpStatus.OK).json(json_of_item(request.body.job_position.name, request.body.job_position.description, "job_position"));
    }
  );
}

/*module.exports.job_positions_add = function(request, response) {

  name = "test name";
  description = "test description";
  category = "test category";

  //TODO // text formatting
  db.pool.query('INSERT INTO job_positions(name, description, category) ' + 
    "values('" + name + "','" + description + "','" + category + "')"
    ).then(function () {
      response.status(HttpStatus.OK)
        .json({
          status: 'success',
          message: 'Se agrego el trabajo'
        });
    })
    .catch(function (err) {
      return next(err);
    });

}*/
