
#ifndef APP_PROJECT_API_H
#define APP_PROJECT_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_PROJECT_API_VERSION 1u

typedef struct AppProjectHandle AppProjectHandle;

typedef enum AppProjectStatus
{
    APP_PROJECT_STATUS_SUCCESS = 0,
    APP_PROJECT_STATUS_FAILURE = 1
} AppProjectStatus;

typedef struct AppProjectAPI
{
    uint8_t api_version;

    AppProjectHandle* (*create_project)(void);

    AppProjectStatus (*initialize_project)(
        AppProjectHandle* project
    );

    AppProjectStatus (*update_project)(
        AppProjectHandle* project,
        double delta_time
    );

    AppProjectStatus (*shutdown_project)(
        AppProjectHandle* project
    );

    void (*destroy_project)(AppProjectHandle* project);

} AppProjectAPI;

typedef const AppProjectAPI* (*AppGetProjectAPI)(
    uint8_t host_api_version
);

const AppProjectAPI* getProjectAPI(
    uint8_t host_api_version
);

#ifdef __cplusplus
}
#endif

#endif
