SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_emoj1_png.c
    images/ui_img_emoj2_png.c
    images/ui_img_emoj3_png.c)

add_library(ui ${SOURCES})
