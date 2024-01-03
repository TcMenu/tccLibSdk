#ifndef TCMENU_THEME_MONO_BORDER
#define TCMENU_THEME_MONO_BORDER

#include "graphics/TcThemeBuilder.h"

color_t defaultItemPaletteMono[] = {WHITE, BLACK, WHITE, WHITE};

#define TITLE_BORDER_THICKNESS 2
#define TITLE_SPACING 2

void setupTheme() {
    TcThemeBuilder themeBuilder(renderer);
    themeBuilder.withSelectedColors(0, 1)
        .withItemPadding(MenuPadding(1))
        .withTitlePadding(MenuPadding(1))
        .withStandardLowResCursorIcons()
        .withPalette(defaultItemPaletteMono)
        .withAdaFont(&Org_01, 1)
        .withSpacing(2);

    themeBuilder.defaultItemProperties()
        .withJustification(tcgfx::GridPosition::JUSTIFY_TITLE_LEFT_VALUE_RIGHT)
        .apply();

    themeBuilder.defaultActionProperties()
            .withBorder(MenuBorder(0, 0, 1, 0))
            .withJustification(tcgfx::GridPosition::JUSTIFY_CENTER_NO_VALUE)
            .apply();

    themeBuilder.defaultTitleProperties()
            .withBorder(MenuBorder(0, 0, 2, 0))
            .withJustification(tcgfx::GridPosition::JUSTIFY_CENTER_NO_VALUE)
            .apply();

    themeBuilder.apply();
}


#endif //TCMENU_THEME_MONO_BORDER
