#include "generated/ControlAppOledKeyboardWifi_menu.h"
#include "TcThemeBuilder.h"
#include "Fonts/Org_01.h"
#include <PlatformDetermination.h>
#include <TaskManagerIO.h>

// This variable is the RAM data for scroll choice item Choices
char ramChoiceData[] = "1\0        2\0        3\0        4\0        5\0        ~";


void setup() {
    setupMenu();

}

int main() {
    setup();
    while(1) {
        taskManager.runLoop();
    }
}

void setupTheme() {
    color_t defaultItemPaletteMono[] = {WHITE, BLACK, WHITE, WHITE};
    TcThemeBuilder themeBuilder(renderer);
    serlogF2(SER_DEBUG, "Loading themes, size = ", sizeof(themeBuilder));

    themeBuilder.withSelectedColors(0, 1)
            .withStandardLowResCursorIcons()
            .withItemPadding(MenuPadding(2))
            .withTitlePadding(MenuPadding(1))
            .withPalette(defaultItemPaletteMono)
            .withSpacing(1);

    themeBuilder.defaultTitleProperties()
            .withBorder(MenuBorder(0, 0, 2, 0))
            .withAdaFont(&Org_01)
            .withJustification(tcgfx::GridPosition::JUSTIFY_CENTER_NO_VALUE)
            .apply();
    themeBuilder.defaultItemProperties()
            .withAdaFont(&Org_01)
            .withJustification(tcgfx::GridPosition::JUSTIFY_TITLE_LEFT_VALUE_RIGHT)
            .apply();
    themeBuilder.defaultActionProperties()
            .withAdaFont(&Org_01)
            .withBorder(MenuBorder(1))
            .withJustification(tcgfx::GridPosition::JUSTIFY_CENTER_NO_VALUE)
            .apply();

    themeBuilder.menuItemOverride(menuAnalog)
        .withImageXbmp(Coord(20, 20), defActiveIcon)
        .onRowCol(2, 3, 1);

    themeBuilder.apply();
}