// create the module and name it sharedServerClient
var sharedServerClient = angular.module('sharedServerClient', ['ngRoute']);

// configure our routes
sharedServerClient.config(function($routeProvider) {
    $routeProvider

        // route for the home page
        .when('/', {
            templateUrl : 'categories.html',
            controller  : 'mainController'
        })

        // route for the about page
        .when('/skills', {
            templateUrl : 'categories.html',
            controller  : 'skillsController'
        })

        // route for the contact page
        .when('/job_positions', {
            templateUrl : 'categories.html',
            controller  : 'PositionController'
        });
});


function get($scope, $http, url, name){
    $http.get(url)
        .success(function(data) {
            console.log(data);
            mydata = data[name];
            $scope.items = mydata;
            console.log(mydata);
        })
        .error(function(data) {
            console.log('Error: ' + mydata);
    });
}

// create the controller and inject Angular's $scope
sharedServerClient.controller('mainController', function($scope, $http) {
    $scope.formData = {};

    $scope.item_name = 'Categories';

    $scope.fields = ["name", "description"];
    
    get($scope, $http, '/categories', 'categories');

    $scope.create = function() {
            console.log({category: $scope.formData});
            $http.post('/categories', {category: $scope.formData})
                .success(function(data) {
                    // refresh
                    get($scope, $http, '/categories', 'categories');
                })
                .error(function(data) {
                    alert("Error al crear");
                    console.log('Error: ' + data);
                });
        };
    
    $scope.delete = function(name) {
        $http.delete('/categories/' + name)
            .success(function(){
                get($scope, $http, '/categories', 'categories');
            })
    }
});

sharedServerClient.controller('skillsController', function($scope, $http) {
    $scope.item_name = 'Skills';
    $http.get('/skills')
        .success(function(data) {
            console.log(data);
            mydata = data["skills"];
            $scope.items = mydata;
            console.log(mydata);
        })
        .error(function(data) {
            console.log('Error: ' + mydata);
    });
});

sharedServerClient.controller('PositionController', function($scope, $http) {
    $scope.item_name = 'Job Positions';
    $http.get('/job_positions')
        .success(function(data) {
            console.log(data);
            mydata = data["job_positions"];
            $scope.items = mydata;
            console.log(mydata);
        })
        .error(function(data) {
            console.log('Error: ' + mydata);
    });
});

    
/*

var sharedServerApp = angular.module('sharedServerApp', ['ngRoute']);

sharedServerApp.config(function($routerProvider){
    $routerProvider.
        when('/', {

            templateUrl : 'categories.html',
            controller : 'mainController'
        });
});


sharedServerApp.controller('mainController', function($scope, $http) {
    $scope.formData = {};

    // when landing on the page, get all todos and show them
    $http.get('/job_positions')
        .success(function(data) {
            console.log(data);
            mydata = data["job_positions"];
            $scope.job_positions = mydata;
            console.log(mydata);
        })
        .error(function(data) {
            console.log('Error: ' + mydata);
    });

    $http.get('/skills')
        .success(function(data) {
            console.log(data);
            mydata = data["skills"];
            $scope.skills = mydata;
            console.log(mydata);
        })
        .error(function(data) {
            console.log('Error: ' + mydata);
        });


});*/



    /*// when submitting the add form, send the text to the node API
    $scope.createTodo = function() {
        $http.post('/api/todos', $scope.formData)
            .success(function(data) {
                $scope.formData = {}; // clear the form so our user is ready to enter another
                $scope.todos = data;
                console.log(data);
            })
            .error(function(data) {
                console.log('Error: ' + data);
            });
    };

    // delete a todo after checking it
    $scope.deleteTodo = function(id) {
        $http.delete('/api/todos/' + id)
            .success(function(data) {
                $scope.todos = data;
                console.log(data);
            })
            .error(function(data) {
                console.log('Error: ' + data);
            });
    };*/

//}
// public/core.js
//var scotchTodo = angular.module('SharedServerApi', []).controller('SharedSeverCtrl', SharedSeverCtrl);