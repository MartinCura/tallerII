
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

function categoryFields(categories) {

    return [
    {
      key: "category",
      type: "select",
      templateOptions: {
        label: "Categories",
        valueProp: "name",
        options: categories
      }
    }
    ]
}


function error(data) {
    alert("Error al crear");
    console.log('Error: ' + data);
}

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
            controller  : 'skillsController',
            controllerAs: 'vm'
        })

        // route for the contact page
        .when('/job_positions', {
            templateUrl : 'categories.html',
            controller  : 'PositionController',
            controllerAs: 'vm'
        });
});



function get($scope, $http, url, name, vm, callback){
    $http.get(url)
        .success(function(data) {
            console.log(data);
            mydata = data[name];
            $scope.items = mydata;
            console.log(mydata);
            vm.items = JSON.parse(JSON.stringify(mydata));
            if (callback != undefined){
                callback($scope);
            }
        }).error(error);
}

function get_categories($scope, $http, url, name, vm){
    get($scope, $http, url, name, vm, 
        function($scope) {
                        $scope.categories = $scope.items;
                    }
    );
}

// create the controller and inject Angular's $scope
sharedServerClient.controller('mainController', function($scope, $http) {
    var vm = this;

    vm.formData = {};
    vm.fields = nameAndDescriptionFields;

    $scope.item_name = 'Categories';
    
    get_categories($scope, $http, '/categories', 'categories', vm);

    vm.create = function() {
            console.log({category: vm.formData});
            $http.post('/categories', {category: vm.formData})
                .success(function(data) {
                    // refresh
                    get_categories($scope, $http, '/categories', 'categories', vm);
                    
                }).error(error);
    };
    
    $scope.delete = function(name) {
        $http.delete('/categories/' + name)
            .success(function(){
                get_categories($scope, $http, '/categories', 'categories', vm);
            })
    };

    vm.onSubmit = function(index){
        console.log($scope.items[index]);
        console.log(vm.items[index]);
        $http.put("/categories/" + $scope.items[index].name, {category: vm.items[index]})
            .success(function() {
                // refresh
                    get_categories($scope, $http, '/categories', 'categories', vm);
            }).error(error);
    };

});

sharedServerClient.controller('skillsController', function($scope, $http) {


    $scope.item_name = 'Skills';

    var vm = this;

    vm.fields = nameAndDescriptionFields.concat(categoryFields($scope.categories));

    get($scope, $http, '/skills', 'skills', vm);

    vm.create = function() {

        console.log({skills: vm.formData});
        $http.post('/skills/categories/' + vm.formData["category"], {skill: vm.formData})
            .success(function(data) {
                // refresh
                get($scope, $http, '/skills', 'skills', vm);
                
            }).error(error);
    };

    $scope.delete = function(name, category) {
        $http.delete('/skills/categories/' + category + "/" + name)
            .success(function(){
                get($scope, $http, '/skills', 'skills', vm);
            })
    };

    vm.onSubmit = function(index){
        $http.put('/skills/categories/' + $scope.items[index].category + "/" + $scope.items[index].name, {skill: vm.items[index]})
            .success(function() {
                // refresh
                get($scope, $http, '/skills', 'skills', vm);
            }).error(error);
    };

});

sharedServerClient.controller('PositionController', function($scope, $http) {
    $scope.item_name = 'Job Positions';

    var vm = this;

    vm.fields = nameAndDescriptionFields.concat(categoryFields($scope.categories));


    get($scope, $http, '/job_positions', 'job_positions', vm);

    vm.create = function() {

        console.log({job_positions: vm.formData});
        $http.post('/job_positions/categories/' + vm.formData["category"], {job_position: vm.formData})
            .success(function(data) {
                // refresh
                get($scope, $http, '/job_positions', 'job_positions', vm);
                
            }).error(error);
    };

    $scope.delete = function(name, category) {
        $http.delete('/job_positions/categories/' + category + "/" + name)
            .success(function(){
                get($scope, $http, '/job_positions', 'job_positions', vm);
            })
    };

    vm.onSubmit = function(index){
        console.log(vm.items[index]);
        $http.put('/job_positions/categories/' + $scope.items[index].category + "/" + $scope.items[index].name, {job_position: vm.items[index]})
            .success(function() {
                // refresh
                get($scope, $http, '/job_positions', 'job_positions', vm);
            }).error(error);
    };
});

