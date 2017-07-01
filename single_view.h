/*
 * Copyright (C) 2017, Abdelhak Saouli
 * Laboratoire de Recherche des Systèmes Experts, Imagerie et leurs Applications dans l’ingénierie " LESIA"
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef SINGLE_VIEW_H
#define SINGLE_VIEW_H

#include<iostream>
#include <cassert>
#include <string>
#include <vector>
#include <memory>

#include "math/vector.h"
#include "math/matrix.h"

#include "mve/bundle.h"
#include "mve/view.h"
#include "mve/image.h"

/*MVSCPC includes */
#include "defines.h"
#include "parameters.h"

MVSDM_NAMESPACE_BEGIN

class SingleView
{
public:
	typedef std::shared_ptr<SingleView> Ptr;
	int width;
	int height;
public:
	static Ptr create(mve::View::Ptr view, mve::Bundle::Features features,
		Parameters settings);															/* create a MVSCPC view from MVE view */
	~SingleView();

	// TODO .......add more functions.....

	/*setter and getter */
	void setScore(float _score);
	void setSettings(Parameters _settings);
	void setStatus(ViewStatus _status);

	bool isViewMaster()const;
	bool isViewNeighbor()const;
	bool isViewNone()const;

	std::vector<std::size_t> getFeatures()const;

	bool isFeatureVisible(const std::size_t &f)const;								/* check whever a giving feature is visible for this view. use this function to compute commun feattures*/
	float getScore()const;

	mve::CameraInfo getViewCamraInfo()const;
	int getSingleViewID()const;
	/*.........................*/
	math::Vec2f compute_World_To_screen(const math::Vec3f &point);
	bool inFrustrum(const math::Vec3f& point)const;
	float pixelFootprint(const math::Vec3f& point)const;

	float parallex(const math::Vec3f& f, SingleView::Ptr v2);

private:
	SingleView(mve::View::Ptr _view, mve::Bundle::Features _features,
		Parameters _settings);
	void fill_features_in_view(const mve::Bundle::Features& _features);

private:
	mve::View::Ptr view;															  /* view contain all the data needed */
	ViewStatus     status;															  /* which state the view is in : none-master-neigbhor*/

	std::vector<std::size_t> features;												  /* indices of features visible only for this view */
	float score;																	  /* the final score after global selection */

	Parameters settings;


};
/* ------------------------ Implementation ------------------------ */
inline SingleView::Ptr
SingleView::create(mve::View::Ptr view, mve::Bundle::Features features,
		Parameters settings)
{
	return Ptr(new  SingleView(view,features,settings));
}

inline
void SingleView::setScore(float _score)
{
	this->score = _score;
}

inline
void SingleView::setSettings(Parameters _settings)
{
	this->settings = _settings;
}

inline
void SingleView::setStatus(ViewStatus _status)
{
	this->status = _status;
}

inline
bool SingleView::isViewMaster()const
{
	if(this->status == VIEW_IS_MASTER)
		return true;
	return false;
}

inline
bool SingleView::isViewNeighbor()const
{
	if(this->status == VIEW_iS_NEIGHBOR)
		return true;
	return false;
}

inline
bool SingleView::isViewNone()const
{
	if(this->status == VIEW_IS_NONE)
		return true;
	return false;
}

inline
std::vector<std::size_t>
SingleView::getFeatures()const
{
	return this->features;
}

inline
float SingleView::getScore()const
{
	return this->score;
}

inline
float SingleView::pixelFootprint(const math::Vec3f& point)const
{
	mve::CameraInfo cam = view->get_camera();

	math::Matrix4f worldTocam;
	cam.fill_world_to_cam(*worldTocam);

	math::Matrix3f inverse_proj;
	cam.fill_inverse_calibration(*inverse_proj,this->width,this->height);

	return (worldTocam.mult(point,1.0)[2] * inverse_proj[0]);
}

inline
mve::CameraInfo
SingleView::getViewCamraInfo()const
{
	return view->get_camera();
}
inline
int SingleView::getSingleViewID()const
{
	return view->get_id();
}
MVSDM_NAMESPACE_END
#endif /* SINGLE_VIEW_H_ */
