/*
 * Copyright (C) 2017, Abdelhak Saouli
 * Laboratoire de Recherche des Systèmes Experts, Imagerie et leurs Applications dans l’ingénierie " LESIA"
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef MAIN_VIEW_H_
#define MAIN_VIEW_H_

#include <string>
#include <vector>
#include <memory>

#include "math/vector.h"
#include "math/matrix.h"

#include "mve/bundle.h"
#include "mve/image.h"
#include "mve/scene.h"

/*MVSCPC includes */
#include "defines.h"
#include "parameters.h"
#include "single_view.h"

MVSDM_NAMESPACE_BEGIN

class MainView
{
public:
	typedef std::shared_ptr<MainView> Ptr;

public:
	static Ptr create(std::vector<SingleView::Ptr> views, mve::Bundle::Features features,
					Parameters settings);
	~MainView();

	// TODO..

	/*.....setter and getter......*/
	void setSettings(Parameters _settings);
	/*for debug*/ int getViewid()const;
	void selectGlobalNeigbhors();							/* global selection */
	//TODO add more functions like compute depth...etc

private:
	MainView(std::vector<SingleView::Ptr> _views,mve::Bundle::Features _features,Parameters _settings);
public:
    mve::FloatImage::Ptr depthImage;						/* this are the output of the algorithm */
    mve::FloatImage::Ptr normalImage;
    mve::FloatImage::Ptr confImage;

private:
	int master_id;
	Parameters settings;

	mve::Bundle::Features features;								/* all features */
	std::vector<SingleView::Ptr> views;
	std::vector<SingleView::Ptr> neighbors;						/* this are the output of global selection*/
	//std::vector<SingleView::Ptr> localNeigbhors;
};

/* ------------------------ Implementation ------------------------ */
inline
MainView::Ptr
MainView::create(std::vector<SingleView::Ptr> views,mve::Bundle::Features features,Parameters settings)
{
	return Ptr(new MainView(views,features,settings));
}

inline
void MainView::setSettings(Parameters _settings)
{
	this->settings = _settings;
}

inline
int
MainView::getViewid()const
{
	return this->master_id;
}
MVSDM_NAMESPACE_END
#endif /* MAIN_VIEW_H_ */
