cmake_minimum_required(VERSION 4.3)
project(sway-plasma)

add_subdirectory(sway-extensions)
add_subdirectory(workspaces-widget)

install(PROGRAMS start-sway-plasma           DESTINATION bin)
install(PROGRAMS continue-kde-startup        DESTINATION libexec/sway-plasma)
install(FILES    sway-config                 DESTINATION share/sway-plasma)
install(FILES    plasma-kwin_wayland.service DESTINATION share/sway-plasma)
install(FILES    xdg-desktop-portal-env.conf DESTINATION share/sway-plasma)
install(FILES    sway-plasma-portals.conf    DESTINATION share/xdg-desktop-portal)
install(FILES    sway-plasma.desktop         DESTINATION share/wayland-sessions)
