#pragma once

#include <spdlog/spdlog.h>

#define DISP_ERROR( ... ) \
    spdlog::get( "console" )->error( __VA_ARGS__ )

#define DISP_INFO( ... ) \
    spdlog::get( "console" )->info( __VA_ARGS__ )

#define DISP_DEBUG( ... ) \
    spdlog::get( "console" )->debug( __VA_ARGS__ )
