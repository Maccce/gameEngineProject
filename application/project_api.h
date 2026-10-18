#ifndef APP_PROJECT_API_H
#define APP_PROJECT_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_PROJECT_API_VERSION 1u

/*
 * Opaque handle.
 * Its concrete definition exists only inside the project library.
 */
typedef struct AppProjectHandle AppProjectHandle;

/*
 * C-compatible function table implemented by the project library.
 */
typedef struct AppProjectAPI
{
    uint8_t api_version;

    AppProjectHandle* (*create_project)(void);

    void (*initialize_project)(AppProjectHandle* project);

    void (*update_project)(
        AppProjectHandle* project,
        double delta_time
    );

    void (*shutdown_project)(AppProjectHandle* project);

    void (*destroy_project)(AppProjectHandle* project);

} AppProjectAPI;

/*
 * Entry point resolved by the runtime using dlsym.
 */
typedef const AppProjectAPI* (*AppGetProjectAPI)(
    uint8_t host_api_version
);

const AppProjectAPI* getProjectAPI(
    uint8_t host_api_version
);

#ifdef __cplusplus
}
#endif

#endif /* APP_PROJECT_API_H */
