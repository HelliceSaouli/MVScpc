/*
 * Copyright (C) 2017, Abdelhak Saouli
 * Laboratoire de Recherche des Systèmes Experts, Imagerie et leurs Applications dans l’ingénierie " LESIA"
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

/*MVSCPC includes */
#include "defines.h"

MVSDM_NAMESPACE_BEGIN

enum ViewStatus	/*the view can be in 4 diffrent status : Main, Neighbor, invalid or none */
{
	VIEW_IS_MASTER
   ,VIEW_iS_NEIGHBOR		/* it mean selected us niegbhor condidate */
   ,VIEW_IS_NONE
};
struct Parameters
{
	/* refrence view ID */
	std::size_t master_id = 0;
	/* status of the view */
	ViewStatus status = VIEW_IS_NONE;

	/* the output image dimontions */
	unsigned int width  = 512;
	unsigned int height = 424;

	/* image emmbeding */
	std::string imgEmbedding = "undistorted";

	/* max global neighbor */
	unsigned int maxNeighbors = 10;

	/* max local neighbors */
	unsigned int maxLocNeighbors = 4;

	//TODO add more

	/* min parallex */
	float alphaMin = 10.0;


};
MVSDM_NAMESPACE_END

#endif /* PARAMETERS_H_ */
