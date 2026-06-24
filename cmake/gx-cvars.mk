# CVar name-string compile defines for the console build.
#
# The desktop build injects these via cmake/ghostship-cvars.cmake +
# cmake/lus-cvars.cmake (add_compile_definitions). The game's cvar_prefixes.h and
# the Fast3D interpreter/window code use them as bare macros, so they must be -D
# defines. This mirrors those cmake files with the values resolved.

# Resource/UI prefixes (ghostship-cvars.cmake)
CVAR_DEFS	:=	-DCVAR_PREFIX_RANDOMIZER_ENHANCEMENT='"gRandoEnhancements"' \
				-DCVAR_PREFIX_RANDOMIZER_SETTING='"gRandoSettings"' \
				-DCVAR_PREFIX_COSMETIC='"gCosmetics"' \
				-DCVAR_PREFIX_AUDIO='"gAudioEditor"' \
				-DCVAR_PREFIX_CHEAT='"gCheats"' \
				-DCVAR_PREFIX_ENHANCEMENT='"gEnhancements"' \
				-DCVAR_PREFIX_SETTING='"gSettings"' \
				-DCVAR_PREFIX_WINDOW='"gOpenWindows"' \
				-DCVAR_PREFIX_TRACKER='"gTrackers"' \
				-DCVAR_PREFIX_DEVELOPER_TOOLS='"gDeveloperTools"' \
				-DCVAR_PREFIX_GENERAL='"gGeneral"' \
				-DCVAR_PREFIX_REMOTE='"gRemote"' \
				-DCVAR_PREFIX_GAMEPLAY_STATS='"gGameplayStats"' \
				-DCVAR_PREFIX_TIME_DISPLAY='"gTimeDisplay"' \
				-DCVAR_PREFIX_CONTROLLERS='"gSettings.Controllers"' \
				-DCVAR_PREFIX_ADVANCED_RESOLUTION='"gSettings.AdvancedResolution"'

# libultraship setting/window cvars (lus-cvars.cmake), values resolved.
CVAR_DEFS	+=	-DCVAR_VSYNC_ENABLED='"gSettings.VsyncEnabled"' \
				-DCVAR_Z_FIGHTING_MODE='"gSettings.ZFightingMode"' \
				-DCVAR_INTERNAL_RESOLUTION='"gSettings.InternalResolution"' \
				-DCVAR_MSAA_VALUE='"gSettings.MSAAValue"' \
				-DCVAR_SDL_WINDOWED_FULLSCREEN='"gSettings.SdlWindowedFullscreen"' \
				-DCVAR_TEXTURE_FILTER='"gSettings.TextureFilter"' \
				-DCVAR_IMGUI_CONTROLLER_NAV='"gSettings.ControlNav"' \
				-DCVAR_CONSOLE_WINDOW_OPEN='"gOpenWindows.Console"' \
				-DCVAR_CONTROLLER_CONFIGURATION_WINDOW_OPEN='"gOpenWindows.ControllerConfiguration"' \
				-DCVAR_CONTROLLER_DISCONNECTED_WINDOW_OPEN='"gOpenWindows.ControllerDisconnected"' \
				-DCVAR_CONTROLLER_REORDERING_WINDOW_OPEN='"gOpenWindows.ControllerReordering"' \
				-DCVAR_GFX_DEBUGGER_WINDOW_OPEN='"gOpenWindows.GfxDebugger"' \
				-DCVAR_STATS_WINDOW_OPEN='"gOpenWindows.Stats"' \
				-DCVAR_ENABLE_MULTI_VIEWPORTS='"gSettings.EnableMultiViewports"' \
				-DCVAR_LOW_RES_MODE='"gSettings.LowResMode"' \
				-DCVAR_SIMULATED_INPUT_LAG='"gSettings.SimulatedInputLag"' \
				-DCVAR_GAME_OVERLAY_FONT='"gSettings.OverlayFont"' \
				-DCVAR_MENU_BAR_OPEN='"gSettings.OpenMenuBar"' \
				-DCVAR_SWITCH_PERF_MODE='"gSettings.SwitchPerfMode"'
