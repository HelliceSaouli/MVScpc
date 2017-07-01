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

#include "util/file_system.h"
#include "mve/image_io.h"
#include "mve/image_tools.h"
#include "mve/view.h"


#include "single_view.h"


MVSDM_NAMESPACE_BEGIN

SingleView::SingleView(mve::View::Ptr _view, mve::Bundle::Features _features,
		Parameters _settings)
		:view(_view)
		,settings(_settings)
{

	/* save checking */
	if(view == nullptr || !view->is_camera_valid())
		throw std::invalid_argument("Invalid view");
	if(_features.empty())
		throw std::invalid_argument("Empty bundle");
	if(settings.imgEmbedding.empty())
		throw std::invalid_argument("No embedding there");


	mve::View::ImageProxy const* proxy = view->get_image_proxy(settings.imgEmbedding);
	if(proxy == nullptr)
		throw std::invalid_argument("No image proxy there");

	width = proxy->width;
	height = proxy->height;
	score  = 0.0;
	status = settings.status;
	fill_features_in_view(_features);
}
SingleView::~SingleView()
{

}
void SingleView::fill_features_in_view(const mve::Bundle::Features &_features)
{
	int view_id =  view->get_id();
	for(std::size_t i = 0; i < _features.size(); ++i)
	{
		if(!_features[i].contains_view_id(view_id))
			continue;

		math::Vec3f ftpos(_features[i].pos);
		if(!inFrustrum(ftpos))
			continue;


		features.push_back(i);		/* the idx of feature visible only to this view is saved */
	}
}

bool
SingleView::inFrustrum(const math::Vec3f& point)const
{
	mve::CameraInfo cam = getViewCamraInfo();;
	math::Matrix4f worldTocam;
	cam.fill_world_to_cam(*worldTocam);

	math::Vec3f camera_point = worldTocam.mult(point,1.0);

	/* check if the resulting point is not behind camera */
	if(camera_point[2] <= 0.0)
		return false;


    math::Matrix3f proj;
    cam.fill_calibration(*proj,width,height);

    math::Vec3f screen_point = proj * camera_point;
    float u = screen_point[0] / screen_point[2] - 0.5;
    float v = screen_point[1] / screen_point[2] - 0.5;

    return u >= 0 && u <= width - 1
            && v >= 0 && v <= height - 1;

}

math::Vec2f
SingleView::compute_World_To_screen(const math::Vec3f& point)
{
	mve::CameraInfo cam = getViewCamraInfo();;
	math::Matrix4f worldTocam;
	cam.fill_world_to_cam(*worldTocam);

	math::Vec3f camera_point = worldTocam.mult(point,1.0);
    math::Matrix3f proj;
    cam.fill_calibration(*proj,width,height);

    math::Vec3f screen_point = proj * camera_point;
    float u = screen_point[0] / screen_point[2] - 0.5;
    float v = screen_point[1] / screen_point[2] - 0.5;

    math::Vec2f uv(u,v);

    return uv;
}

bool
SingleView::isFeatureVisible(const std::size_t &f)const		/* take the index of feature from global feature vector and look for it in local*/
{
	for(std::size_t i = 0; i < features.size(); ++i)
		if(f == features[i])
			return true;
	 return false;
}

float
SingleView::parallex(const math::Vec3f &f, SingleView::Ptr v2)
{
	mve::CameraInfo cam_v1 = getViewCamraInfo();
	mve::CameraInfo cam_v2 = v2->getViewCamraInfo();

	math::Vec3f cam_pos_v1;
	cam_v1.fill_camera_pos(*cam_pos_v1);

	math::Vec3f cam_pos_v2;
	cam_v2.fill_camera_pos(*cam_pos_v2);

	math::Vec3f dir1 = (f - cam_pos_v1).normalized();
	math::Vec3f dir2 = (f - cam_pos_v2).normalized();

	float cos_alpha = std::max(std::min(dir1.dot(dir2),1.0f),-1.0f);
	float alpha = std::acos(cos_alpha) * 180.0f / PI ;

	return alpha;

}
MVSDM_NAMESPACE_END
