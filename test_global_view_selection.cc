
#include <iostream>
#include <string>
#include <vector>

#include "mve/bundle.h"
#include "mve/scene.h"
#include "mve/view.h"

#include "MVScpc/defines.h"
#include "MVScpc/parameters.h"
#include "MVScpc/single_view.h"
#include "MVScpc/main_view.h"
#include "MVScpc/global_views_selector.h"


int main()
{

    /* Load scene */
    mve::Scene::Ptr scene = mve::Scene::create();
    scene->load_scene("scene_dino/");

    /* Create list of SingleView pointers from MVE views. */
    mvsdm::Parameters settings;
    settings.master_id = 36;
    /* 1-load features from bundler in the scene */
    mve::Bundle::ConstPtr bundle = scene->get_bundle();
    mve::Bundle::Features const& features = bundle->get_features();

    /* 2- load viewlist from the scene */
    mve::Scene::ViewList const& views(scene->get_views());

    /* create mvs-comunity photo collection views from mve views */
    std::vector<mvsdm::SingleView::Ptr> cpcViews;


    for(std::size_t i = 0; i < views.size(); ++i)
    {
    	if(views[i] == nullptr || !views[i]->is_camera_valid() ||
                !views[i]->has_image(settings.imgEmbedding,
                mve::IMAGE_TYPE_UINT8))
    		continue;
    	mvsdm::SingleView::Ptr v = mvsdm::SingleView::create(views[i],features,settings);
    	cpcViews.push_back(v);
    }
    std::cout<<"creating "<< cpcViews.size()<<" mvs-comunity photo collection view from mve views"<<std::endl;

    /* create main view which will be reconstructed */
    mvsdm::MainView::Ptr refView = mvsdm::MainView::create(cpcViews,features,settings);
    std::cout<<"Main view [" << refView->getViewid()<<"] created"<<std::endl;

    refView->selectGlobalNeigbhors();


	return 0;
}
