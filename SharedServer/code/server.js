var express  = require('express');
var app      = express();                               // create our app w/ express
var morgan   = require('morgan');             // log requests to the console (express4)  
const path = require('path');
var bodyParser = require('body-parser');    // pull information from HTML POST (express4)

// configuration =================

app.use(express.static(path.join(__dirname, '/public')));                 // set the static files location /public/img will be /img for users
app.use(morgan('dev'));                                         // log every request to the console
app.use(bodyParser.urlencoded({'extended':'true'}));            // parse application/x-www-form-urlencoded
app.use(bodyParser.json());                                     // parse application/json
app.use(bodyParser.json({ type: 'application/vnd.api+json' })); // parse application/vnd.api+json as json

var router = express.Router();
var urls = require("./urls.js");
urls.set_urls(router);
app.use('/', router);

app.get('*', function(req, res) {
        res.sendfile('./public/index.html'); // load the single view file (angular will handle the page changes on the front-end)
    });


// listen (start app with node server.js) ====================================== 
app.listen(80);
console.log("App listening on port 80");