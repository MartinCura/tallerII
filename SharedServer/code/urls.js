views = require("./views.js");

module.exports.set_urls = function (router) {
	// API
		// job_positions
		

		// categories
		router.get('/categories', views.job_categories_get);
		router.post('/categories', views.job_categories_add);
		router.put('/categories/:category', views.job_categories_modify);
		router.delete('/categories/:category', views.job_categories_delete);


		router.get('/job_positions', views.job_positions_get);
		router.post('/job_positions/categories/:category', views.job_positions_add);
		//router.post('/job_positions/categories/:PositionCategory', views.job_positions_add);

	// Front-end
		// angular single-page website
};
