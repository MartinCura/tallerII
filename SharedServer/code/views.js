var db = require("./db.js");
var HttpStatus = require('http-status-codes');

/*Este archivo tiene código repetido, se hizo así por simplicidad y porque
tenemos solo 3 elementos (2 que on iguales) y el código repetido es trivial.

Hacer uso de cosas más genéricas hubiera llevado mucho trabajo de diseño, testing
e implementación, solo para 3 elementos.*/


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

function json_of_item(name, description, itemName, category) {
  out = {};
  inner_item = {
                "name": name,
                "description": description
  }
  if (category != null) {
    inner_item["category"] = category;
  }
  out[itemName] = inner_item;
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

module.exports.job_positions_get = function(request, response) {
  db.get_job_positions().then(function (data) {
    response.status(HttpStatus.OK).json({job_positions: data});
  })
}

module.exports.job_positions_get_by_category = function(request, response) {
  db.get_job_positions_by_category(request.params.category).then(function (data) {
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

module.exports.job_positions_modify = function(request, response) {
  db.job_position_modify(request.params.category, request.params.position, request.body.job_position.name, request.body.job_position.description, request.body.job_position.category).then(
    function(){
      response.status(HttpStatus.OK).json(json_of_item(request.body.job_position.name, request.body.job_position.description, "job_position", request.body.job_position.category));
    }
  );
}


module.exports.job_positions_delete = function(request, response) {
  db.job_positions_delete(request.params.category, request.params.position).then(
    function(){
     response.status(HttpStatus.OK).json({});
    }
  )
}

module.exports.skills_get = function(request, response){
  db.skills_get().then(function (data) {
    response.status(HttpStatus.OK).json({skills: data});
  })
}


module.exports.skills_add = function(request, response){
  db.create_skills(request.params.category, request.body.skill.name, request.body.skill.description).then(
    function(){
      response.status(HttpStatus.OK).json(json_of_item(request.body.skill.name, request.body.skill.description, "skill"));
    }
  );
}

module.exports.skills_get_by_category = function(request, response){
  db.get_skills_by_category(request.params.category).then(
    function(data) {
      response.status(HttpStatus.OK).json({skills: data});
    }
  );
}

module.exports.skills_modify = function(request, response){
  db.skills_modify(request.params.category, request.params.skill, request.body.skill.name, request.body.skill.description, request.body.skill.category).then(
    function(){
      response.status(HttpStatus.OK).json(json_of_item(request.body.skill.name, request.body.skill.description, "skill", request.body.skill.category));
    }
  );
}

module.exports.skills_delete = function(request, response){
  db.delete_skills(request.params.category, request.params.skill).then(
    function(){
     response.status(HttpStatus.OK).json({});
    }
  )
}