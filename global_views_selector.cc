/*
 * Copyright (C) 2017, Abdelhak Saouli
 * Laboratoire de Recherche des Systèmes Experts, Imagerie et leurs Applications dans l’ingénierie " LESIA"
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#include <algorithm>
#include <iterator>
#include <vector>

#include "math/vector.h"

#include "global_views_selector.h"

MVSDM_NAMESPACE_BEGIN

GlobalViewsSelector::GlobalViewsSelector(std::vector<SingleView::Ptr> _views,mve::Bundle::Features _features,Parameters _settings)
		:views(_views)
		,features(_features)
		,settings(_settings)
{
	if(views.empty())
		throw std::invalid_argument("invalid list of mvsdm views");
}


std::vector<std::size_t>
GlobalViewsSelector::compute_commun_features(int master, int view) /* we may have some problem  coz of sort or something*/
{
	std::vector<std::size_t> master_fs = views[master]->getFeatures();
	std::vector<std::size_t> view_fs   = views[view]->getFeatures();

	/*sort vector */
	std::sort(master_fs.begin(),master_fs.end());
	std::sort(view_fs.begin(),view_fs.end());

	std::vector<std::size_t> commun;
	std::set_intersection(master_fs.begin(),master_fs.end(),
						  view_fs.begin(),view_fs.end(),
						  std::back_inserter(commun));

	return commun;
}
void GlobalViewsSelector::select()
{
	bool foundOne = true;
	while(foundOne && (neighbors.size() < settings.maxNeighbors))
	{
		float max_score = 0.0;
		std::size_t best_view = 0;
		foundOne = false;
		std::cout<<"*"<<std::endl;
		for(std::size_t i = 0; i < views.size(); ++i)	/* go over all views and compute score and add neigbhors itertavely */
		{
			if(views[i]->isViewMaster() || views[i]->isViewNeighbor()) // this are invalide views to use */
				continue;

			float global_score_of_view = compute_score(master_indice,i);	/* the id of master and other view is diffrent from it's indice in views vector*/

			if(global_score_of_view > max_score)
			{
				max_score = global_score_of_view;
				best_view = i;
				foundOne = true;
			}
		}

		if(foundOne)
		{
			views[best_view]->setStatus(VIEW_iS_NEIGHBOR);
			neighbors.push_back(views[best_view]);
		}


	}
}

float GlobalViewsSelector::compute_score(int master, int view)
{
	float score = 0.0;

	std::vector<std::size_t> commun = compute_commun_features(master,view);

	/* iterate over all features visible in  a given view  and master view */
	for(std::size_t i = 0; i < commun.size(); ++i)
	{

		float Wn_f = 1.0;
		math::Vec3f featurePos(features[commun[i]].pos);

		/* compute parallex master with view */
		float alpha = views[master]->parallex(featurePos,views[view]);				/* compute parallex betewen master and the other view */
		Wn_f *= std::min(std::pow(alpha/settings.alphaMin,2.0),1.0);

		/* compute the simularity in resolution */
		float Sr_f = views[master]->pixelFootprint(featurePos);
		float Sv_f = views[view]->pixelFootprint(featurePos);

		float scale = 1.0;
		float ratio = Sr_f / Sv_f;

		if(2.0 <= ratio)
			scale = 2.0 / ratio;
		if(1 <= ratio ||  ratio < 2.0)
			scale = 1.0;
		if(ratio < 1)
			scale = ratio;

		Wn_f *= scale;

		/* Parallax with other selected views that see the same feature*/
		for(std::size_t j = 0; j < neighbors.size(); ++j)
		{
			if( !neighbors[j]->isFeatureVisible(commun[i]) )
				continue;

			alpha = views[j]->parallex(featurePos,views[view]);
			Wn_f *= std::min(std::pow(alpha/settings.alphaMin,2.0),1.0);

		}
		score += Wn_f;
	}

	views[view]->setScore(score);
	return score;
}
MVSDM_NAMESPACE_END
