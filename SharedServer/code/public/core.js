// public/core.js
var scotchTodo = angular.module('SharedServerApi', []);

function mainController($scope, $http) {
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

}