nameAndDescriptionFields = [
        {
            key: 'name',
            type: 'input',
            templateOptions: {
                type: 'text',
                label: 'Nombre',
                placeholder: 'Nombre',
                required: true
            }
        },
        {
            key: 'description',
            type: 'input',
            templateOptions: {
                type: 'text',
                label: 'Descipción',
                placeholder: 'Descipción',
                required: true
            }
        }
];



// create the module and name it sharedServerClient
var sharedServerClient = angular.module('sharedServerClient', ['ngRoute', 'formly', 'formlyBootstrap']);

// configure our routes
sharedServerClient.config(function($routeProvider) {
    $routeProvider

        // route for the home page
        .when('/categories', {
            templateUrl : 'categories.html',
            controller  : 'mainController',
            controllerAs: 'vm'
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


function get($scope, $http, url, name, vm){
    $http.get(url)
        .success(function(data) {
            console.log(data);
            mydata = data[name];
            $scope.items = mydata;
            console.log(mydata);
            vm.items = JSON.parse(JSON.stringify(mydata));
        })
        .error(function(data) {
            console.log('Error: ' + mydata);
    });
}

function update($http, url) {
    $http.put()

}

// create the controller and inject Angular's $scope
sharedServerClient.controller('mainController', function($scope, $http) {
    var vm = this;

    vm.formData = {};
    vm.fields = nameAndDescriptionFields;

    $scope.item_name = 'Categories';

    $scope.fields = ["name", "description"];
    
    get($scope, $http, '/categories', 'categories', vm);

    vm.create = function() {
            console.log({category: vm.formData});
            $http.post('/categories', {category: vm.formData})
                .success(function(data) {
                    // refresh
                    get($scope, $http, '/categories', 'categories', vm);
                    
                })
                .error(function(data) {
                    alert("Error al crear");
                    console.log('Error: ' + data);
                });
        };
    
    $scope.delete = function(name) {
        $http.delete('/categories/' + name)
            .success(function(){
                get($scope, $http, '/categories', 'categories', vm);
            })
    }

    vm.onSubmit = function(index){
        console.log($scope.items[index]);
        console.log(vm.items[index]);
        $http.put("/categories/" + $scope.items[index].name, {category: vm.items[index]})
            .success(function() {
                // refresh
                    get($scope, $http, '/categories', 'categories', vm);
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

