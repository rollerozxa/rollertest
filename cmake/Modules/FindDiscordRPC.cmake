# Use the bundled Discord RPC library in lib/ if RPC is

option(ENABLE_DISCORD "Enable Discord RPC support" TRUE)
set(USE_DISCORD FALSE)
if(ENABLE_DISCORD)
	set(USE_DISCORD TRUE)
endif()

if(USE_DISCORD)
	message(STATUS "Building with Discord RPC support.")
	set(DISCORD_LIBRARY discord-rpc)
	set(DISCORD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/discord-rpc/include)
	add_subdirectory(lib/discord-rpc)
endif()
