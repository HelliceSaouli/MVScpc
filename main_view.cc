/*
 * Copyright (C) 2017, Abdelhak Saouli
 * Laboratoire de Recherche des Systèmes Experts, Imagerie et leurs Applications dans l’ingénierie " LESIA"
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#include <cassert>
#include <limits>

#include "math/vector.h"
#include "math/matrix.h"

#include "mve/bundle.h"
#include "mve/view.h"
#include "mve/image.h"

/*MVSCPC includes */
#include "defines.h"
#include "parameters.h"
#include "single_view.h"
#include "main_view.h"
#include "global_views_selector.h"

MVSDM_NAMESPACE_BEGIN

MainView::MainView(std::vector<SingleView::Ptr> _views,mve::Bundle::Features _features,Parameters _settings)
	:views(_views)
	,features(_features)
	,settings(_settings)
{
	if(views.empty())
		throw std::invalid_argument("invalid list of mvsdm views");


	/* set the default id of the main view from settings */
	master_id = settings.master_id;

	bool notfound = true;
	for(std::size_t i = 0; i < views.size(); ++i)
	{
		if(master_id == views[i]->getSingleViewID())
		{
			notfound = false;
			views[i]->setStatus(VIEW_IS_MASTER);
			break;
		}
	}
	if(notfound)
		throw std::invalid_argument("invalid refrence view");
	/* Create images for reconstruction. */
	int const width  = settings.width;
	int const height = settings.height;

	depthImage  = mve::FloatImage::create(width,height,1);
	normalImage = mve::FloatImage::create(width,height,3);
	confImage   = mve::FloatImage::create(width,height,1);
}

MainView::~MainView()
{

}
void
MainView::selectGlobalNeigbhors()
{
	GlobalViewsSelector selector(views,features,settings);
	std::cout<<" -Selector is ready: "<<std::endl;

	for(std::size_t i = 0; i < views.size(); ++i)
	{
		if(master_id == views[i]->getSingleViewID())
		{
			selector.master_indice = i;
			break;
		}
	}


	selector.select();
	std::cout<<" -Selector is done "<<std::endl;
	neighbors = selector.getselection();
}
MVSDM_NAMESPACE_END
