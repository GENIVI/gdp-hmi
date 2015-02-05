/* SPDXLicenseID: MPL-2.0
 *
 * Copyright (C) 2015, GENIVI Alliance
 *
 * This file is part of the GENIVI Demo Platform HMI.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * For further information see http://www.genivi.org/.
 *
 * List of changes:
 * 23.Jan.2015, Holger Behrens, written
 * 05.Feb.2015, Holger Behrens, added surface ID '3' handling
 */

/*! \file gdp-hmi-controller.c
 *  \brief HMI controller for the GENIVI Demo Platform
 *   
 *   This component implements the HMI controller of the GENIVI Demo Platform
 *   using the GENIVI Alliance wayland-ivi-extension.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <systemd/sd-journal.h>

#include <ilm/ilm_types.h>
#include <ilm/ilm_client.h>
#include <ilm/ilm_control.h>

#include "wayland-util.h"
#include "ivi-controller-client-protocol.h"


// definitions

#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 768

struct gdp_surface_context {
	t_ilm_bool created;
    t_ilm_uint id_surface;
    t_ilm_uint id_layer;
};

// initialize global variables

static int verbose                 = 0;
static int gRunLoop                = 0;
static t_ilm_uint screenID         = 0;
static t_ilm_uint screenWidth      = DEFAULT_SCREEN_WIDTH;
static t_ilm_uint screenHeight     = DEFAULT_SCREEN_HEIGHT;
static int surfacesArrayCount      = 0;
static unsigned int* surfacesArray = NULL;

static struct gdp_surface_context gdp_surfaces[] = {
	{ILM_FALSE,    1 /* Background / Logo   */, 100},
	{ILM_FALSE,    2 /* GDP HMI             */, 200},
	{ILM_FALSE,    3 /* QML Example         */, 300},
	{ILM_FALSE,   20 /* AudioManager PoC    */, 300},
	{ILM_FALSE,   30 /* Browser PoC         */, 400},
	{ILM_FALSE,   40 /* FSA PoC             */, 500},
	{ILM_FALSE,   10 /* EGL Mock Navigation */, 600},
	{ILM_FALSE, 5100 /* EGL Input Example   */, 700},
};

/**
 * \brief signal handler
 *
 * This function shall handle all signals of type 'SIGINT'.
 *
 */
static void sig_handler(int signo)
{
	signo = signo;
    printf(" signal caught\n");
    gRunLoop = 0;
}

/**
 * \brief creates IVI layer
 *
 * This function creates the following layers with the the given ID:
 *
 * Layer Assignment   | Layer ID
 * -----------------: | :------:
 * background w/ logo |   100
 * launcher			  |   200
 * AM PoC			  |   300
 * Browser PoC		  |   400
 * FSA PoC			  |   500
 * MockNavigation	  |   600
 * InputEventExample  |   700
 *
 */
static void layer_create(void)
{
    ilmErrorTypes callResult = ILM_FAILED;
    struct ilmScreenProperties screenProperties;
	t_ilm_layer  layerid = 0;
    t_ilm_uint*  pIDs = NULL;
    t_ilm_uint   numberOfIDs  = 0;

    if (!ilm_isInitialized())
    	return;

	callResult = ilm_getScreenIDs(&numberOfIDs, &pIDs);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
        	"Error: layer_create() ilm_getScreenIDs - %s. Exiting.\n",
        	ILM_ERROR_STRING(callResult));
		exit(EXIT_FAILURE);
    } else {
    	screenID = pIDs[0];
        sd_journal_print(LOG_DEBUG,
        	"Debug: ilm_getScreenIDs - %s. number of screens = %u\n", 
        	ILM_ERROR_STRING(callResult), numberOfIDs);
    }
    sd_journal_print(LOG_INFO,
    	"Info: layer_create - in screen with ID = %u\n", 
    	screenID);


    callResult = ilm_getPropertiesOfScreen(screenID, &screenProperties);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
        	"Error: layer_create() ilm_getPropertiesOfScreen - %s. Exiting.\n",
        	ILM_ERROR_STRING(callResult));
		exit(EXIT_FAILURE);
	}
	screenWidth  = screenProperties.screenWidth;
	screenHeight = screenProperties.screenHeight;
    sd_journal_print(LOG_INFO,
    	"Info: layer_create - screen size = %u x %u\n", 
    	screenWidth, screenHeight);
    if (0 == screenWidth)
    	screenWidth = DEFAULT_SCREEN_WIDTH;
    if (0 == screenHeight)
    	screenHeight = DEFAULT_SCREEN_HEIGHT;

	for(layerid = 100; layerid < 800; layerid += 100) {
	    callResult = ilm_layerCreateWithDimension(&layerid,
	    	screenWidth, screenHeight);
	    if (ILM_SUCCESS != callResult) {
	        sd_journal_print(LOG_ERR,
	        	"Error: layer_create (id = %u) - %s\n",
	        	layerid, ILM_ERROR_STRING(callResult));
	        break;
	    } else {
	        sd_journal_print(LOG_DEBUG,
	        	"Debug: layer_create (id = %u) - %s (%u x %u)\n",
	        	layerid, ILM_ERROR_STRING(callResult),
	        	screenWidth, screenHeight);
	    }
	} // for-loop

	callResult = ilm_commitChanges();
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
        	"Error: layer_create() ilm_commitChanges - %s. Exiting.\n",
        	ILM_ERROR_STRING(callResult));
		exit(EXIT_FAILURE);
	}
}

/**
 * \brief print help message to stderr
 *
 * This function does print a help message to stderr explaining the usage
 * of the executable \p name.
 *
 * \param	name	The name of this executable.
 */
static void help(const char *name)
{
	fprintf(stderr, "Usage: %s [args...]\n", name);
	fprintf(stderr, "  -s, --surface   Use surface with specified ID\n");
	fprintf(stderr, "  -v, --verbose   Be verbose\n");
	fprintf(stderr, "  -h, --help      Display this help message\n");
}

/**
 * \brief control the IVI surface
 *
 * This function does control the surface \p gdp_surface.
 * Currently the surface is added to its assigned layer,
 * the layer brought into view on the screen and assigned input focus.
 *
 * \param	gdp_surfaces	The name of this executable.
 */
static void surface_control(struct gdp_surface_context gdp_surface)
{
    ilmErrorTypes callResult = ILM_FAILED;
    t_ilm_surface surfaceIdArray[] = {1, 2, 3};
    t_ilm_layer   layerIdArray[]   = {100, 200, 300};

	sd_journal_print(LOG_DEBUG, "surface_control"
		"(surface = %u, layer = %u)\n",
		gdp_surface.id_surface, gdp_surface.id_layer);

	surfaceIdArray[2] = gdp_surface.id_surface;
	layerIdArray[2] = gdp_surface.id_layer;

	switch(gdp_surface.id_surface) {
		case 1:    // Background / Logo
			break;
		case 2:    // GDP HMI
			break;
		case 3:	   // QML Example
			// fall-through
		case 20:   // AudioManager PoC
			callResult = ilm_surfaceSetDestinationRectangle(
				gdp_surface.id_surface, 0, 0, screenWidth, screenHeight);
			callResult = ilm_surfaceSetVisibility(
				gdp_surface.id_surface, ILM_TRUE);
			callResult = ilm_surfaceSetOpacity(
				gdp_surface.id_surface, 1.0f);
    		callResult = ilm_commitChanges();
			sd_journal_print(LOG_DEBUG, "surface_control - input focus on\n");
    		callResult = ilm_UpdateInputEventAcceptanceOn(
    			gdp_surface.id_surface,
	            ILM_INPUT_DEVICE_POINTER |
	            ILM_INPUT_DEVICE_TOUCH   |
	            ILM_INPUT_DEVICE_KEYBOARD,
	            ILM_TRUE);
    		callResult = ilm_SetKeyboardFocusOn(gdp_surface.id_surface);
    		callResult = ilm_commitChanges();

			sd_journal_print(LOG_DEBUG, "surface_control - render order - layer\n");
    		callResult = ilm_layerSetRenderOrder(gdp_surface.id_layer,
    			surfaceIdArray, 3);
			callResult = ilm_layerSetVisibility(gdp_surface.id_layer,
				ILM_TRUE);
    		callResult = ilm_commitChanges();

			sd_journal_print(LOG_DEBUG, "surface_control - render order - screen\n");
			callResult = ilm_displaySetRenderOrder((t_ilm_display)screenID,
				layerIdArray, 3);

    		callResult = ilm_commitChanges();
			break;
		case 30:   // Browser PoC
			break;
		case 40:   // FSA PoC
			break;
		case 10:   // EGL Mock Navigation
			break;
		case 5100: // EGL Input Example
			break;
		default:
			sd_journal_print(LOG_DEBUG,
				"surface_control - unknown surface.\n");
			break;
	} // switch
}

/**
 * \brief retrieve surface ID(s) that appeared
 *
 * This function will check if, from the list of expected surfaces, any new
 * surface has appeared.
 *
 * \param	length		The length of the array
 * \param	pArray		Pointer to array of surface IDs
 */
static void surfaces_appear_check(t_ilm_int length, t_ilm_surface* pArray)
{
    if ((length <= 0) && (pArray == NULL))
    	return;

    // check for appearance
    for (int i = 0; i < length; i++) {
	    // check out list of expected surface IDs 'gdp_surfaces'
	    for (int count = 0;
	    	count < sizeof gdp_surfaces / sizeof gdp_surfaces[0]; count++) {
	    	if (pArray[i] == gdp_surfaces[count].id_surface) {
	    		// check if surface is already known
	    		if (ILM_TRUE == gdp_surfaces[count].created)
	    			continue; // inner for-loop
	    		// get to know *new* surface
	    		gdp_surfaces[count].created = ILM_TRUE;
		        sd_journal_print(LOG_DEBUG,
		        	"Debug: new surface id: %i (for layer: %i)\n",
		        	gdp_surfaces[count].id_surface,
		        	gdp_surfaces[count].id_layer);
		        surface_control(gdp_surfaces[count]);
	    	}
	    } // inner for-loop
    } // outer for-loop
}

/**
 * \brief retrieve surface ID(s) that disappeared
 *
 * This function will check if, from the list of expected surfaces, any
 * surface has disappeared.
 *
 * \param	length		The length of the array
 * \param	pArray		Pointer to array of surface IDs
 */
static void surfaces_disappear_check(t_ilm_int length, t_ilm_surface* pArray)
{
    if ((length <= 0) && (pArray == NULL))
    	return;

    // check for disappearance
    for (int count = 0;
    	count < sizeof gdp_surfaces / sizeof gdp_surfaces[0]; count++) {
    	t_ilm_bool found = ILM_FALSE;

	    for (int i = 0; i < length; i++) {
	    	if (pArray[i] == gdp_surfaces[count].id_surface) {
	    		found = ILM_TRUE;
	    		break;	// inner for-loop
	    	}
	    } // inner for-loop

	    if ((ILM_FALSE == found) && (ILM_TRUE == gdp_surfaces[count].created)) {
	    	gdp_surfaces[count].created = ILM_FALSE;
	        sd_journal_print(LOG_DEBUG,
	        	"Debug: surface id: %i disappeared (from layer: %i)\n",
	        	gdp_surfaces[count].id_surface,
	        	gdp_surfaces[count].id_layer);
	    }
	} // outer for-loop
}

/**
 * \brief poll for surface appearance
 *
 * This function gets all surface IDs and compares them with a previous
 * copy to find out if a new surface did appear or an existing vanished.
 */
static void surface_create_poll(void)
{
    int count = 0;
    unsigned int* array = NULL;
    ilmErrorTypes callResult = ILM_FAILED;

    // retrieve all surface ID(s)
    callResult = ilm_getSurfaceIDs(&count, &array);
    if (ILM_SUCCESS != callResult) {
	    sd_journal_print(LOG_ERR,
	    	"Error: surface_create_poll ilm_getSurfaceIDs - %s.\n",
        	ILM_ERROR_STRING(callResult));
    } else if (count != surfacesArrayCount) {
	    unsigned int* oldArray = surfacesArray;

    	// new previously unknown surface(s) appeared

	    if (surfacesArrayCount < count) {
		    sd_journal_print(LOG_DEBUG,
		    	"Debug: surface_create_poll surfaces = %i (++)\n", count);
 	    	surfaces_appear_check(count, array);
	    } else if (surfacesArrayCount > count) {
		    sd_journal_print(LOG_DEBUG,
		    	"Debug: surface_create_poll surfaces = %i (--)\n", count);
 	    	surfaces_disappear_check(count, array);
	    }

	    surfacesArray = array;
	    surfacesArrayCount = count;
	    array = oldArray;
    }

    // free 'array' that had been allocated by ilm_getSurfaceIDs()
    if (NULL != array)
    	free(array);
}

/**
 * \brief main thread
 *
 * This function is the main entry point of this executable.
 * It reads and interprets command line options given by user,
 * creates some layers, waits for surfaces to appear and once
 * it did, assign the surface to a layer and have it rendered.
 */
int main(int argc, char * const* argv)
{
    t_ilm_surface surfaceId  = 210;
    ilmErrorTypes callResult = ILM_FAILED;

    // read and interpret command line options
    while (1){
        struct option opts[] = {
            { "surface", required_argument, NULL, 's' },
			{ "verbose", no_argument,       NULL, 'v' },
			{ "help",    no_argument,       NULL, 'h' },
			{ 0,         0,                 NULL,  0  }
        };

        int c = getopt_long(argc, argv, "s:vh", opts, NULL);
        if (-1 == c) // no more options
            break;

        switch(c){
	        default:
		        sd_journal_print(LOG_ERR,
		        	"Error: Unrecognized option '%s'\n", optarg);
	            break;
	        case 's':
	            surfaceId = strtol(optarg, NULL, 0);
	            break;
			case 'v':
				verbose = 1;
				break;
			case 'h':
				help("gdp-hmi-controller");
				exit(EXIT_FAILURE);
				break;
        }
    }

    // initializes the IVI LayerManagement Client
    callResult = ilm_init();
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
        	"Error: ilm_init - %s. Exiting.\n",
        	ILM_ERROR_STRING(callResult));
		exit(EXIT_FAILURE);
    }

    // create all the layers needed
    layer_create();

	gRunLoop = 1;

    // establish signal handling
    if ((void *)-1 == signal(SIGINT, sig_handler)) {
	    sd_journal_print(LOG_ERR, "Error: signal()\n");
	    exit(EXIT_FAILURE);
    }

    // fill 'surfacesArray' global with already existing surfaces
    callResult = ilm_getSurfaceIDs(&surfacesArrayCount, &surfacesArray);
    if (ILM_SUCCESS != callResult) {
	    sd_journal_print(LOG_ERR, "Error: ilm_getSurfaceIDs - %s.\n",
        	ILM_ERROR_STRING(callResult));
    } else {
	    sd_journal_print(LOG_DEBUG,
	    	"Debug: main surfaces = %i\n", surfacesArrayCount);
    }

    // wait for SIGINT to happen
    while (gRunLoop) {
    	/*
    	 * poll for added surfaces as ilm_surfaceAddNotification()
    	 * only work for already existing surfaces
    	 */
    	surface_create_poll();
        sleep(1); // was usleep(50)
    }

    // destroy the IVI LayerManagement Client
	callResult = ilm_destroy();
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
        	"Error: ilm_destroy - %s. Exiting.\n",
        	ILM_ERROR_STRING(callResult));
		exit(EXIT_FAILURE);
    }

	return EXIT_SUCCESS;
}
