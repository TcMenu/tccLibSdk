/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#ifndef TCCLIBSDK_TCTHEMEBUILDER_H
#define TCCLIBSDK_TCTHEMEBUILDER_H

#include "GfxMenuConfig.h"
#include <graphics/GraphicsDeviceRenderer.h>

namespace tcgfx {
    class TcThemeBuilder;

    class ThemePropertiesBuilder {
    public:
        enum ThemeLevel : uint8_t {
            THEME_GLOBAL, THEME_SUB, THEME_ITEM, THEME_ITEM_NEEDS_PROPS, THEME_ITEM_NEEDS_GRID, THEME_ITEM_NEEDS_BOTH
        };
    private:
        color_t palette[4]{};
        MenuPadding padding;
        const void* fontData{};
        GridPosition::GridJustification justification;
        MenuBorder border{};
        TcThemeBuilder *themeBuilder{};
        MenuItem* menuItem = nullptr;
        uint16_t gridHeight;
        ThemeLevel currentLevel;
        ItemDisplayProperties::ComponentType componentType;
        uint8_t fontMag{};
        uint8_t spacing{};
        GridPosition::GridDrawingMode drawingMode{};
        uint8_t row;
        uint8_t colData;
    public:
        void needsProps() {
            if(currentLevel == THEME_SUB || currentLevel == THEME_GLOBAL) return;
            if(currentLevel == THEME_ITEM) currentLevel = THEME_ITEM_NEEDS_PROPS;
            else currentLevel = THEME_ITEM_NEEDS_BOTH;
        }

        void needsGrid(bool propsOk) {
            if(currentLevel == THEME_SUB || currentLevel == THEME_GLOBAL) return;
            if(currentLevel == THEME_ITEM_NEEDS_PROPS && propsOk) return;
            if(currentLevel == THEME_ITEM) currentLevel = THEME_ITEM_NEEDS_GRID;
            else currentLevel = THEME_ITEM_NEEDS_BOTH;
        }

        ThemePropertiesBuilder& withBorder(const MenuBorder& b) {
            border = b;
            needsProps();
            return *this;
        }

        ThemePropertiesBuilder& withAdaFont(const GFXfont* font, int mag = 1) {
            fontData = font;
            fontMag = min(1, mag);
            needsProps();
            return *this;
        }

        ThemePropertiesBuilder& withTcUnicodeFont(const UnicodeFont* font) {
            fontData = font;
            fontMag = 0;
            needsProps();
            return *this;
        }

        ThemePropertiesBuilder& withNativeFont(void* data, uint8_t mag) {
            fontData = data;
            fontMag = mag;
            needsProps();
            return *this;
        }

        ThemePropertiesBuilder& withPadding(MenuPadding p) {
            padding = p;
            needsProps();
            return *this;
        }

        ThemePropertiesBuilder& withSpacing(uint8_t s) {
            spacing = s;
            needsGrid(true);
            return *this;
        }

        ThemePropertiesBuilder& withJustification(GridPosition::GridJustification j) {
            justification = j;
            needsGrid(true);
            return *this;
        }

        ThemePropertiesBuilder& withPalette(const color_t* p) {
            needsProps();
            memcpy(palette, p, sizeof(palette));
            return *this;
        }

        ThemePropertiesBuilder& withRowHeight(uint16_t height) {
            gridHeight = height;
            needsGrid(false);
            return *this;
        }

        ThemePropertiesBuilder& withImageXbmp(Coord size, const uint8_t* regIcon, const uint8_t* selIcon = nullptr);

        ThemePropertiesBuilder& withDrawingMode(GridPosition::GridDrawingMode dm) {
            drawingMode = dm;
            needsGrid(false);
            return *this;
        }

        ThemePropertiesBuilder& onRowCol(uint8_t actualRow, uint8_t numberOfCols, uint8_t columns) {
            row = actualRow;
            colData = (numberOfCols << 4) & columns;
            return *this;
        }

        ThemePropertiesBuilder& onRow(uint8_t actualRow) {
            colData = 0x10;
            row = actualRow;
            return *this;
        }

        void apply();

    protected:
        void initForLevel(TcThemeBuilder *b, ItemDisplayProperties::ComponentType compType, ThemeLevel level, MenuItem *item = nullptr);
        friend TcThemeBuilder;
    };

    class TcThemeBuilder {
    private:
        GraphicsDeviceRenderer &renderer;
        ConfigurableItemDisplayPropertiesFactory &factory;
        ThemePropertiesBuilder propertiesBuilder;
        color_t defaultPalette[4] = {};
        MenuPadding globalItemPadding = MenuPadding(1);
        MenuPadding globalTitlePadding = MenuPadding(2);
        const void *fontData = nullptr;
        uint8_t fontMag = 1;
        uint8_t defaultSpacing = 1;

    public:
        explicit TcThemeBuilder(GraphicsDeviceRenderer& renderer) : renderer(renderer), factory(renderer.getGraphicsPropertiesFactory()) {
            auto *drawable = renderer.getDeviceDrawable();
            renderer.setDisplayDimensions(drawable->getDisplayDimensions().x, drawable->getDisplayDimensions().y);
        }

        TcThemeBuilder &withSelectedColors(color_t bg, color_t fg) {
            factory.setSelectedColors(bg, fg);
            return *this;
        }

        TcThemeBuilder& withAdaFont(const GFXfont* font, int mag = 1) {
            fontData = font;
            fontMag = min(1, mag);
            return *this;
        }

        TcThemeBuilder& withTcUnicodeFont(const UnicodeFont* font) {
            fontData = font;
            fontMag = 0;
            return *this;
        }

        TcThemeBuilder& withNativeFont(void* data, uint8_t mag) {
            fontData = data;
            fontMag = mag;
            return *this;
        }

        TcThemeBuilder &withCursorIconsXbmp(Coord size, const uint8_t *editIcon, const uint8_t *activeIcon);
        TcThemeBuilder &withStandardLowResCursorIcons();
        TcThemeBuilder &withStandardMedResCursorIcons();

        TcThemeBuilder &withSpacing(uint8_t spacing) {
            defaultSpacing = spacing;
            return *this;
        }

        TcThemeBuilder &withItemPadding(MenuPadding padding) {
            globalItemPadding = padding;
            return *this;
        }

        TcThemeBuilder &withTitlePadding(MenuPadding padding) {
            globalTitlePadding = padding;
            return *this;
        }

        ThemePropertiesBuilder &defaultItemProperties() {
            propertiesBuilder.initForLevel(this, ItemDisplayProperties::COMPTYPE_ITEM, ThemePropertiesBuilder::THEME_GLOBAL);
            return propertiesBuilder;
        }
        ThemePropertiesBuilder &defaultTitleProperties() {
            propertiesBuilder.initForLevel(this, ItemDisplayProperties::COMPTYPE_TITLE, ThemePropertiesBuilder::THEME_GLOBAL);
            return propertiesBuilder;
        }
        ThemePropertiesBuilder &defaultActionProperties() {
            propertiesBuilder.initForLevel(this, ItemDisplayProperties::COMPTYPE_ACTION, ThemePropertiesBuilder::THEME_GLOBAL);
            return propertiesBuilder;
        }

        ThemePropertiesBuilder &menuItemOverride(MenuItem &item);

        ThemePropertiesBuilder &submenuPropertiesItemOverride(SubMenuItem &item) {
            propertiesBuilder.initForLevel(this, ItemDisplayProperties::COMPTYPE_ITEM, ThemePropertiesBuilder::THEME_SUB, &item);
            return propertiesBuilder;
        }
        ThemePropertiesBuilder &submenuPropertiesActionOverride(SubMenuItem &item) {
            propertiesBuilder.initForLevel(this, ItemDisplayProperties::COMPTYPE_ACTION, ThemePropertiesBuilder::THEME_SUB, &item);
            return propertiesBuilder;
        }
        ThemePropertiesBuilder &submenuPropertiesTitleOverride(SubMenuItem &item) {
            propertiesBuilder.initForLevel(this, ItemDisplayProperties::COMPTYPE_TITLE, ThemePropertiesBuilder::THEME_SUB, &item);
            return propertiesBuilder;
        }

        TcThemeBuilder& withPalette(const color_t* cols);

        TcThemeBuilder& enableCardLayoutWithXbmImages(Coord iconSize, const uint8_t* leftIcon, const uint8_t* rightIcon, bool isMono);
        TcThemeBuilder& setMenuAsCard(SubMenuItem& item);

        void apply();

        ConfigurableItemDisplayPropertiesFactory getItemFactory() { return factory; }

        MenuPadding getPaddingFor(ItemDisplayProperties::ComponentType type) {
            return type == ItemDisplayProperties::COMPTYPE_TITLE ? globalTitlePadding : globalItemPadding;
        }

        uint8_t getDefaultSpacing() const {
            return defaultSpacing;
        }

        GraphicsDeviceRenderer& getRenderer() {
            return renderer;
        }

        const color_t* getDefaultPalette() {
            return defaultPalette;
        }

        const void* getDefaultFontData() {
            return fontData;
        }

        uint8_t getDefaultFontMag() const {
            return fontMag;
        }

    };

}
#endif //TCCLIBSDK_TCTHEMEBUILDER_H
