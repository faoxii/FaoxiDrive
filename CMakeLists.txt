cmake_minimum_required(VERSION 3.22)
project(FaoxiDrive VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(JUCE_DIR "$ENV{HOME}/Documents/JUCE" CACHE PATH "Chemin vers JUCE")
add_subdirectory(${JUCE_DIR} JUCE)

juce_add_plugin(FaoxiDrive
        COMPANY_NAME "Faoxi"
        PLUGIN_MANUFACTURER_CODE Faox
        PLUGIN_CODE Fdrv
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "FaoxiDrive"
        COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(FaoxiDrive PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/distorsion.cpp
        Source/filtre.cpp
)

target_link_libraries(FaoxiDrive PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
)

target_compile_definitions(FaoxiDrive PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)