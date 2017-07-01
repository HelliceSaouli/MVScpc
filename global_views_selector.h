/*
 * Copyright (C) 2017, Abdelhak Saouli
 * Laboratoire de Recherche des Systèmes Experts, Imagerie et leurs Applications dans l’ingénierie " LESIA"
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef GLOBAL_VIEWS_SELECTOR_H_
#define GLOBAL_VIEWS_SELECTOR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "mve/bundle.h"

/*MVSCPC includes */
#include "defines.h"
#include "parameters.h"
#include "single_view.h"

MVSDM_NAMESPACE_BEGIN
class GlobalViewsSelector
{
public:
	GlobalViewsSelector(std::vector<SingleView::Ptr> _views,mve::Bundle::Features _features,Parameters _settings);

	void select();
	std::vector<SingleView::Ptr> getselection()const;									/* this is the final result */

private:
	float compute_score(int master, int view);
	std::vector<std::size_t> compute_commun_features(int master, int view);

public:
	unsigned int master_indice;
private:
	mve::Bundle::Features features;
	std::vector<SingleView::Ptr> views;
	std::vector<SingleView::Ptr> neighbors;
	Parameters settings;
};


inline
std::vector<SingleView::Ptr>
GlobalViewsSelector::getselection()const
{
	/* print for debugin perposes */
	std::vector<int> print;
	for(std::size_t i = 0; i < neighbors.size(); ++i )
		 print.push_back(neighbors[i]->getSingleViewID());

	std::sort(print.begin(),print.end());

	for(std::size_t i = 0; i < print.size(); ++i )
		std::cout<<print[i]<<" ";

	/*********************************************/
	return neighbors;
}

MVSDM_NAMESPACE_END
#endif /* GLOBAL_VIEWS_SELECTOR_H_ */
