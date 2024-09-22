#######################################
# Effect

set(luminvert_SOURCES
    luminvert.cpp
    luminvert.qrc
    main.cpp
)

# kconfig_add_kcfg_files(luminvert_SOURCES Config.kcfgc)
kcoreaddons_add_plugin(
    kwin_effect_luminvert
    SOURCES ${luminvert_SOURCES}
    INSTALL_NAMESPACE "kwin/effects/plugins/")
target_link_libraries(kwin_effect_luminvert PRIVATE
    KWin::kwin

    KF6::GlobalAccel
    KF6::I18n
)

#######################################
# Config

set(kwin_luminvert_config_SRCS luminvert_config.cpp)

if(${QT_MAJOR_VERSION} EQUAL 6)
    qt6_add_dbus_interface(kwin_luminvert_config_SRCS ${KWIN_EFFECTS_INTERFACE} kwineffects_interface)
    kcoreaddons_add_plugin(
        kwin_luminvert_config
        INSTALL_NAMESPACE "kwin/effects/configs"
        SOURCES ${kwin_luminvert_config_SRCS})

    target_link_libraries(kwin_luminvert_config
            Qt${QT_MAJOR_VERSION}::CorePrivate
            KF${QT_MAJOR_VERSION}::KCMUtils
    )
else()
    qt5_add_dbus_interface(kwin_luminvert_config_SRCS ${KWIN_EFFECTS_INTERFACE} kwineffects_interface)
    kcoreaddons_add_plugin(kwin_luminvert_config INSTALL_NAMESPACE "kwin/effects/configs" SOURCES ${kcm_SRCS})
endif ()

target_link_libraries(kwin_luminvert_config
    Qt${QT_MAJOR_VERSION}::DBus
    KF6::KCMUtils
    KF6::CoreAddons
    KF6::GlobalAccel
    KF6::I18n
    KF6::XmlGui
)
