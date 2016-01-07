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
 * 06.Feb.2015, Holger Behrens, added default surfaces (panel, background)
 *                              introduced SIGUSR signal handling, pidfile
 * 09.Feb.2015, Holger Behrens, convert main loop into a glib main loop
 * 10.Feb.2015, Holger Behrens, added interface to systemd (via dbus-c++)
 * 16.Feb.2015, Holger Behrens, correct gdp_surfaces[] usage
 * 20.Feb.2015, Holger Behrens, reflect new layer assignment in comment made
 */

/*! \file gdp-hmi-controller.cpp
 *  \brief HMI controller for the GENIVI Demo Platform
 *   
 *   This component implements the HMI controller of the GENIVI Demo Platform
 *   using the GENIVI Alliance wayland-ivi-extension.
 */

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include <systemd/sd-journal.h>

#include <ilm/ilm_types.h>
#include <ilm/ilm_client.h>
#include <ilm/ilm_control.h>
#include <ilm/ilm_input.h>

#include "wayland-util.h"
#include "ivi-controller-client-protocol.h"
#include "gdp-hmi-surfaces.h"
#include "gdp-dbus-service.h"
#include "gdp-dbus-systemd.h"

// definitions

#define DEFAULT_SCREEN_WIDTH      1024
#define DEFAULT_SCREEN_HEIGHT      768
#define DEFAULT_PANEL_HEIGHT_LR     68  // low-res (default)
#define DEFAULT_PANEL_HEIGHT_HR     80  // high-res

// initialize global variables

static int verbose                 = 0;
static int gRunLoop                = 0;
static GMainLoop* gMainLoop        = NULL;
static t_ilm_uint screenID         = 0;
static t_ilm_uint screenWidth      = DEFAULT_SCREEN_WIDTH;
static t_ilm_uint screenHeight     = DEFAULT_SCREEN_HEIGHT;
static t_ilm_uint panelHeight      = DEFAULT_PANEL_HEIGHT_LR;
static int surfacesArrayCount      = 0;
static unsigned int* surfacesArray = NULL;
static const char *GDP_HMI_PID_FILENAME  = "/var/run/gdp-hmi-controller.pid";

DBus::Glib::BusDispatcher dispatcher;   // dbus-c++ bus dispatcher (glib)
SystemdService *gSystemdSession;        // systemd on session bus (d-bus)
SystemdService *gSystemd;               // systemd on system  bus (d-bus)

// database of well known surface and layer IDs as well as unit names
struct gdp_surface_context gdp_surfaces[] = {
    {   // 0 - GDP_PANEL
        ILM_FALSE,
        ILM_FALSE,
        GDP_PANEL_SURFACE_ID,
        GDP_PANEL_LAYER_ID,
        GDP_PANEL_UNIT
    },
    {   // 1 - GDP_LAUNCHER
        ILM_FALSE,
        ILM_FALSE,
        GDP_LAUNCHER_SURFACE_ID,
        GDP_LAUNCHER_LAYER_ID,
        GDP_LAUNCHER_UNIT
    },
    {   // 2 - GDP_BACKGROUND
        ILM_FALSE,
        ILM_FALSE,
        GDP_BACKGROUND_SURFACE_ID,
        GDP_BACKGROUND_LAYER_ID,
        GDP_BACKGROUND_UNIT
    },
    {   // 3 - QML_EXAMPLE
        ILM_FALSE,
        ILM_FALSE,
        QML_EXAMPLE_SURFACE_ID,
        QML_EXAMPLE_LAYER_ID,
        QML_EXAMPLE_UNIT
    },
    {   // 4 - AM_DEMO
        ILM_FALSE,
        ILM_FALSE,
        AM_DEMO_SURFACE_ID,
        AM_DEMO_LAYER_ID,
        AM_DEMO_UNIT
    },
    {   // 5 - BROWSER_POC
        ILM_FALSE,
        ILM_FALSE,
        BROWSER_POC_SURFACE_ID,
        BROWSER_POC_LAYER_ID,
        BROWSER_POC_UNIT
    },
    {   // 6 - FSA 
        ILM_FALSE,
        ILM_FALSE,
        FSA_SURFACE_ID,
        FSA_LAYER_ID,
        FSA_UNIT
    },
    {   // 7 - MOCK_NAVIGATION
        ILM_FALSE,
        ILM_FALSE,
        MOCK_NAVIGATION_SURFACE_ID,
        MOCK_NAVIGATION_LAYER_ID,
        MOCK_NAVIGATION_UNIT
    },
    {   // 8 - INPUT_EVENT_EXAMPLE
        ILM_FALSE,
        ILM_FALSE,
        INPUT_EVENT_EXAMPLE_SURFACE_ID,
        INPUT_EVENT_EXAMPLE_LAYER_ID,
        INPUT_EVENT_EXAMPLE_UNIT
    },
};
const int gdp_surfaces_num = sizeof gdp_surfaces / sizeof gdp_surfaces[0];

/**
 * \brief creates a PID file
 *
 * This function creates a file and write this process' ID into it.
 *
 * \param[IN] progName  name of the calling program (i.e., argv[0] or similar)
 * \param[IN] pidFile   name of file to be created (fully qualified)
 * \return              file descriptor referring the file created
 */
int create_pid_file(const char *progName, const char *pidFile)
{
    int fd;
    char stringBuffer[256];

    fd = open(pidFile, O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR);
    if (-1 == fd) {
        strerror_r(errno, stringBuffer, 256);
        sd_journal_print(LOG_ERR,
            "Error: create_pid_file (open) - %s. Exiting.\n", stringBuffer);
        exit(EXIT_FAILURE);
    }

    if (-1 == ftruncate(fd, 0)) {
        strerror_r(errno, stringBuffer, 256);
        sd_journal_print(LOG_ERR,
            "Error: create_pid_file (trunc) - %s. Exiting.\n", stringBuffer);
        exit(EXIT_FAILURE);
    }

    snprintf(stringBuffer, 256, "%ld\n", (long) getpid());
    if (write(fd, stringBuffer, strlen(stringBuffer)) != strlen(stringBuffer)) {
        sd_journal_print(LOG_ERR,
            "Error: Writing to PID file failed. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    return fd;
}

/**
 * \brief creates IVI layer
 *
 * This function creates the following layers with the given ID:
 *
 *  Layer Assignment   | Layer ID
 * ------------------: | :------:
 * panel               |    0
 * launcher            |   100
 * background          |   200
 * QML Example         |   300
 * AM PoC              |   400
 * Browser PoC         |   500
 * FSA PoC             |   600
 * MockNavigation      |   700
 * InputEventExample   |   800
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
        sd_journal_print(LOG_DEBUG,
            "Debug: ilm_getScreenIDs - %s. number of screens = %u\n", 
            ILM_ERROR_STRING(callResult), numberOfIDs);
        for (int i = 0; i < numberOfIDs; i++) {
            sd_journal_print(LOG_DEBUG, "Debug: Screen ID[%u] = %d\n",
                i, pIDs[i]);
        }
        screenID = 0;   // FIXME: always use screen with the ID 0
                        // (limitation of ivi-shell at time of this writing)
    }
    sd_journal_print(LOG_INFO,
        "Info: layer_create - in screen with ID = %u\n", screenID);

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
    if (1200 < screenWidth)
        panelHeight = DEFAULT_PANEL_HEIGHT_HR;

    // create panel layer - layer id '0'
    callResult = ilm_layerCreateWithDimension(&layerid,
        screenWidth, panelHeight);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: layer_create (id = %u) - %s\n",
            layerid, ILM_ERROR_STRING(callResult));
    } else {
        sd_journal_print(LOG_DEBUG,
            "Debug: layer_create (id = %u) - %s (%u x %u)\n",
            layerid, ILM_ERROR_STRING(callResult),
            screenWidth, panelHeight);
    }

    // create all other layers - layer id's '100..700'
    for(layerid  = GDP_LAUNCHER_LAYER_ID; layerid < GDP_MAX_LAYER_ID;
        layerid += GDP_LAYER_ID_INCR) {
        callResult = ilm_layerCreateWithDimension(&layerid, screenWidth,
            (GDP_LAUNCHER_LAYER_ID == layerid) ?
                screenHeight : (screenHeight - panelHeight));
        if (ILM_SUCCESS != callResult) {
            sd_journal_print(LOG_ERR,
                "Error: layer_create (id = %u) - %s\n",
                layerid, ILM_ERROR_STRING(callResult));
            break;
        } else {
            sd_journal_print(LOG_DEBUG,
                "Debug: layer_create (id = %u) - %s (%u x %u)\n",
                layerid, ILM_ERROR_STRING(callResult), screenWidth,
                (100 == layerid)?screenHeight:(screenHeight - panelHeight));
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
 * \brief mark surface as visible
 *
 * This function does mark the surface \p surfaceNum visible and all the
 * other surfaces available invisible.
 *
 * \param   surfaceNum  The GDP surface number to mark as visible.
 */
static void surface_mark_visible(const int surfaceNum)
{
    // handling special case of panel
    if (GDP_PANEL == surfaceNum)
        return;

    for (int count = 0; count < gdp_surfaces_num; count++) {
        if (count == surfaceNum) {
            gdp_surfaces[count].visible = ILM_TRUE;
        } else {
            gdp_surfaces[count].visible = ILM_FALSE;
        }
    }

    // exception handling (panel is visible too, if !launcher surface)
    if (GDP_LAUNCHER != surfaceNum) {
        gdp_surfaces[GDP_PANEL].visible = ILM_TRUE;
    }
}

/**
 * \brief show the launcher surface
 *
 * This function does control the launcher surface given by \p gdp_surface.
 * Currently the surface is added to its assigned layer with the ID '1'.
 *
 * \param   gdp_surfaces    The GDP surface/layer context used for launcher.
 */
static void launcher_show(const struct gdp_surface_context gdp_surface)
{
    ilmErrorTypes callResult = ILM_FAILED;
    t_ilm_surface surfaceIdArray[] = {GDP_LAUNCHER_SURFACE_ID};
    t_ilm_layer   layerIdArray[]   = {GDP_LAUNCHER_LAYER_ID};

    sd_journal_print(LOG_DEBUG, "launcher_show"
        "(surface = %u, layer = %u)\n",
        gdp_surface.id_surface, gdp_surface.id_layer);

    surfaceIdArray[0] = gdp_surface.id_surface;
    layerIdArray[0] = gdp_surface.id_layer;
    callResult = ilm_surfaceSetDestinationRectangle(
        gdp_surface.id_surface, 0, 0, screenWidth, screenHeight);
    callResult = ilm_surfaceSetVisibility(
        gdp_surface.id_surface, ILM_TRUE);
    callResult = ilm_surfaceSetOpacity(
        gdp_surface.id_surface, 1.0f);
    callResult = ilm_commitChanges();
    sd_journal_print(LOG_DEBUG, "launcher_show - input focus on\n");
    callResult = ilm_UpdateInputEventAcceptanceOn(
        gdp_surface.id_surface,
        ILM_INPUT_DEVICE_POINTER |
        ILM_INPUT_DEVICE_TOUCH   |
        ILM_INPUT_DEVICE_KEYBOARD,
        ILM_TRUE);
    callResult = ilm_setInputFocus(surfaceIdArray, 1, IVI_CONTROLLER_SURFACE_INPUT_DEVICE_KEYBOARD, ILM_TRUE);
    callResult = ilm_commitChanges();

    sd_journal_print(LOG_DEBUG, "launcher_show - render order - layer\n");
    callResult = ilm_layerSetRenderOrder(gdp_surface.id_layer,
        surfaceIdArray, 1);
    callResult = ilm_layerSetVisibility(gdp_surface.id_layer,
        ILM_TRUE);
    callResult = ilm_commitChanges();

    sd_journal_print(LOG_DEBUG, "launcher_show - render order - screen %u\n",
        screenID);
    callResult = ilm_displaySetRenderOrder((t_ilm_display)screenID,
        layerIdArray, 1);

    callResult = ilm_commitChanges();
    surface_mark_visible(GDP_LAUNCHER);
}

/**
 * \brief control the IVI surface
 *
 * This function does control the surface \p index.
 * Currently the surface is added to its assigned layer,
 * the layer together with the layer holding the 'panel'
 * are brought into view on the screen and assigned input focus.
 *
 *  ID  | Surface
 * :--: | :------
 *    0 | Panel
 *    1 | GDP HMI
 *    2 | Background
 *    3 | QML Example
 *   20 | AudioManager PoC
 *   30 | Browser PoC
 *   40 | FSA PoC
 *   10 | EGL Mock Navigation
 * 5100 | EGL Input Example
 *
 * \param   index           The index into the gdp_surfaces[] database.
 */
void surface_control(const int index)
{
    const struct gdp_surface_context gdp_surface = gdp_surfaces[index];
    ilmErrorTypes callResult = ILM_FAILED;
    t_ilm_surface surfaceIdArray[] = {GDP_BACKGROUND_SURFACE_ID};
    t_ilm_layer   layerIdArray[]   = {GDP_BACKGROUND_LAYER_ID,
                                      GDP_PANEL_LAYER_ID};

    sd_journal_print(LOG_DEBUG, "surface_control - index = %d"
        "(surface = %u, layer = %u)\n",
        index, gdp_surface.id_surface, gdp_surface.id_layer);

    surfaceIdArray[0] = gdp_surface.id_surface;
    layerIdArray[0] = gdp_surface.id_layer;

    switch(gdp_surface.id_surface) {
        case GDP_PANEL_SURFACE_ID:           // Panel
            callResult = ilm_surfaceSetDestinationRectangle(
                gdp_surface.id_surface, 0, 0, screenWidth, panelHeight);
            callResult = ilm_surfaceSetVisibility(
                gdp_surface.id_surface, ILM_TRUE);
            callResult = ilm_surfaceSetOpacity(
                gdp_surface.id_surface, 1.0f);
            callResult = ilm_commitChanges();
            sd_journal_print(LOG_DEBUG, "surface_control (0) - input focus on\n");
            callResult = ilm_UpdateInputEventAcceptanceOn(
                gdp_surface.id_surface,
                ILM_INPUT_DEVICE_POINTER |
                ILM_INPUT_DEVICE_TOUCH   |
                ILM_INPUT_DEVICE_KEYBOARD,
                ILM_TRUE);
            callResult = ilm_setInputFocus(surfaceIdArray, 1, IVI_CONTROLLER_SURFACE_INPUT_DEVICE_KEYBOARD, ILM_TRUE);
            callResult = ilm_commitChanges();
            sd_journal_print(LOG_DEBUG, "surface_control - render order - layer\n");
            callResult = ilm_layerSetDestinationRectangle(gdp_surface.id_layer,
                0, screenHeight - panelHeight, screenWidth, panelHeight);
            callResult = ilm_layerSetRenderOrder(gdp_surface.id_layer,
                surfaceIdArray, 1);
            callResult = ilm_layerSetVisibility(gdp_surface.id_layer,
                ILM_TRUE);
            callResult = ilm_commitChanges();
            break;
        case GDP_LAUNCHER_SURFACE_ID:        // GDP HMI / Launcher
            launcher_show(gdp_surface);
            break;
        case GDP_BACKGROUND_SURFACE_ID:      // Background / Logo
            // fall-through
        case QML_EXAMPLE_SURFACE_ID:         // QML Example
            // fall-through
        case AM_DEMO_SURFACE_ID:             // Audio Manager Demo
            // fall-through
        case BROWSER_POC_SURFACE_ID:         // Browser PoC
            // fall-through
        case FSA_SURFACE_ID:                 // FSA PoC
            // fall-through
        case MOCK_NAVIGATION_SURFACE_ID:     // EGL Mock Navigation
            // fall-through
        case INPUT_EVENT_EXAMPLE_SURFACE_ID: // EGL Input Example
            callResult = ilm_surfaceSetDestinationRectangle(
                gdp_surface.id_surface, 0, 0, screenWidth,
                screenHeight - panelHeight);
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

            callResult = ilm_setInputFocus(surfaceIdArray, 1, IVI_CONTROLLER_SURFACE_INPUT_DEVICE_KEYBOARD, ILM_TRUE);
            callResult = ilm_commitChanges();

            sd_journal_print(LOG_DEBUG, "surface_control - render order - layer\n");
            callResult = ilm_layerSetRenderOrder(gdp_surface.id_layer,
                surfaceIdArray, 1);
            callResult = ilm_layerSetVisibility(gdp_surface.id_layer,
                ILM_TRUE);
            callResult = ilm_commitChanges();

            sd_journal_print(LOG_DEBUG, "surface_control - render order - screen\n");
            callResult = ilm_displaySetRenderOrder((t_ilm_display)screenID,
                layerIdArray, 2);

            callResult = ilm_commitChanges();
            surface_mark_visible(index);
            break;
        default:
            sd_journal_print(LOG_DEBUG,
                "surface_control - unknown surface.\n");
            return;
            break;
    } // switch
}

/**
 * \brief retrieve surface ID(s) that appeared
 *
 * This function will check if, from the list of expected surfaces, any new
 * surface has appeared.
 *
 * \param   length      The length of the array
 * \param   pArray      Pointer to array of surface IDs
 */
static void surfaces_appear_check(t_ilm_int length, t_ilm_surface* pArray)
{
    if ((length <= 0) && (pArray == NULL))
        return;

    // check for appearance
    for (int i = 0; i < length; i++) {
        // check out list of expected surface IDs 'gdp_surfaces'
        for (int count = 0; count < gdp_surfaces_num; count++) {
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
                surface_control(count);
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
 * \param   length      The length of the array
 * \param   pArray      Pointer to array of surface IDs
 */
static void surfaces_disappear_check(t_ilm_int length, t_ilm_surface* pArray)
{
    if ((length <= 0) && (pArray == NULL))
        return;

    // check for disappearance
    for (int count = 0; count < gdp_surfaces_num; count++) {
        t_ilm_bool found = ILM_FALSE;

        for (int i = 0; i < length; i++) {
            if (pArray[i] == gdp_surfaces[count].id_surface) {
                found = ILM_TRUE;
                break;  // inner for-loop
            }
        } // inner for-loop

        if ((ILM_FALSE == found) && (ILM_TRUE == gdp_surfaces[count].created)) {
            gdp_surfaces[count].created = ILM_FALSE;
            sd_journal_print(LOG_DEBUG,
                "Debug: surface id: %i disappeared (from layer: %i)\n",
                gdp_surfaces[count].id_surface,
                gdp_surfaces[count].id_layer);
            // in case this surface was currently in sight
            if (ILM_TRUE == gdp_surfaces[count].visible) {
                // show the background instead
                gdp_surfaces[count].visible = ILM_FALSE;
                surface_control(GDP_BACKGROUND);
            }
        }
    } // outer for-loop
}

/**
 * \brief poll for surface appearance
 *
 * This function gets all surface IDs and compares them with a previous
 * copy to find out if a new surface did appear or an existing vanished.
 */
static gboolean surface_create_poll(gpointer data)
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

    return TRUE;
}

/**
 * \brief signal handler
 *
 * This function shall handle all signals of type 'SIGINT', 'SIGTERM',
 * 'SIGUSR1' and 'SIGUSR2'.
 *
 */
static void sig_handler(int signo)
{
    sd_journal_print(LOG_DEBUG, "Debug: sig_handler() - %d\n", signo);

    switch(signo) {
        case SIGINT:
            sd_journal_print(LOG_DEBUG, "Debug: Interrupt from keyboard.\n");
            // fall-through
        case SIGTERM:
            g_main_loop_quit(gMainLoop); // stop the main loop in main()
            break;
        case SIGUSR1:
            if (ILM_TRUE == gdp_surfaces[GDP_LAUNCHER].created) {
                launcher_show(gdp_surfaces[GDP_LAUNCHER]);
            }
            sd_journal_print(LOG_DEBUG, "Debug: show launcher (%s)\n",
                (ILM_TRUE == gdp_surfaces[GDP_LAUNCHER].created) ? "true" : "false");
            break;
        case SIGUSR2:
            if (ILM_TRUE == gdp_surfaces[GDP_BACKGROUND].created) {
                surface_control(GDP_BACKGROUND);
            }
            sd_journal_print(LOG_DEBUG, "Debug: show background\n");
            break;
        default:
            sd_journal_print(LOG_DEBUG, "Debug: signal (%d) unknown\n", signo);
            break;
    } // switch
}

/**
 * \brief print help message to stderr
 *
 * This function does print a help message to stderr explaining the usage
 * of the executable \p name.
 *
 * \param   name    The name of this executable.
 */
static void help(const char *name)
{
    fprintf(stderr, "Usage: %s [args...]\n", name);
    fprintf(stderr, "  -s, --surface   Use surface with specified ID\n");
    fprintf(stderr, "  -v, --verbose   Be verbose\n");
    fprintf(stderr, "  -h, --help      Display this help message\n");
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
    int fd = -1;
    int rtn = -1;
    t_ilm_surface surfaceId  = 0;
    ilmErrorTypes callResult = ILM_FAILED;

    sd_journal_print(LOG_INFO, "GDP HMI Controller (main)\n");

    // establish signal handling
    if ((void *)-1 == signal(SIGTERM, sig_handler)) {
        sd_journal_print(LOG_ERR, "Error: signal(SIGTERM)\n");
        exit(EXIT_FAILURE);
    }
    if ((void *)-1 == signal(SIGINT, sig_handler)) {
        sd_journal_print(LOG_ERR, "Error: signal(SIGINT)\n");
        exit(EXIT_FAILURE);
    }
    if ((void *)-1 == signal(SIGUSR1, sig_handler)) {
        sd_journal_print(LOG_ERR, "Error: signal(SIGUSR1)\n");
        exit(EXIT_FAILURE);
    }
    if ((void *)-1 == signal(SIGUSR2, sig_handler)) {
        sd_journal_print(LOG_ERR, "Error: signal(SIGUSR2)\n");
        exit(EXIT_FAILURE);
    }

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

    fd = create_pid_file(argv[0], GDP_HMI_PID_FILENAME);

    /*
     * make ourselves available on the d-bus session bus
     *
     * currently this interface is only used by the launcher to tell the
     * controller (us), to power off, which application (systemd unit) to
     * launch or give focus and bring to front/into view.
     *
     * The 'home' button activation is currently signaled via SIGUSR1.
     */

    DBus::default_dispatcher = &dispatcher;
    dispatcher.attach(NULL);
    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name(GDP_DBUS_SERVICE_NAME);
    HmiService service(conn);
    sd_journal_print(LOG_INFO, "GDP HMI Controller (dbus) - %s\n",
        GDP_DBUS_SERVICE_NAME);
    // make a connection to systemd on the session bus
    DBus::Connection sessionBus = DBus::Connection::SessionBus();
    SystemdService systemdSession(sessionBus,
        SYSTEMD_DBUS_SERVICE_PATH, SYSTEMD_DBUS_SERVICE_NAME);
    gSystemdSession = &systemdSession;
    std::string systemdVersion = systemdSession.Version();
    sd_journal_print(LOG_INFO, "GDP HMI Control -> systemd (session)"
        " systemd version: %s\n", systemdVersion.c_str());
    // make a connection to systemd on the system bus
    DBus::Connection systemBus = DBus::Connection::SystemBus();
    SystemdService systemd(systemBus,
        SYSTEMD_DBUS_SERVICE_PATH, SYSTEMD_DBUS_SERVICE_NAME);
    gSystemd = &systemd;
    systemdVersion = systemd.Version();
    sd_journal_print(LOG_INFO, "GDP HMI Control -> systemd  (system)"
        " systemd version: %s\n", systemdVersion.c_str());

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

    // fill 'surfacesArray' global with already existing surfaces
    callResult = ilm_getSurfaceIDs(&surfacesArrayCount, &surfacesArray);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR, "Error: ilm_getSurfaceIDs - %s.\n",
            ILM_ERROR_STRING(callResult));
    } else {
        sd_journal_print(LOG_DEBUG,
            "Debug: main surfaces = %i\n", surfacesArrayCount);
    }

    // go into a loop, poll for surfaces every second
    gMainLoop = g_main_loop_new (NULL, FALSE);
    g_timeout_add_seconds(1, surface_create_poll, NULL);
    g_main_loop_run(gMainLoop);

    // close and delete PID file
    if (-1 != fd)
        close (fd);
    unlink (GDP_HMI_PID_FILENAME);

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
