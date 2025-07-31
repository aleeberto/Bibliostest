#ifndef STYLEUTILS_H
#define STYLEUTILS_H

#include <QString>

class StyleUtils
{
public:
    // Stili per pulsanti
    static QString getPrimaryButtonStyle();
    static QString getSecondaryButtonStyle();
    static QString getDangerButtonStyle();
    static QString getWarningButtonStyle();
    static QString getIconButtonStyle();
    
    // Stili per input
    static QString getLineEditStyle();
    static QString getSearchBarStyle();
    static QString getComboBoxStyle();
    
    // Stili per layout
    static QString getMainWindowStyle();
    static QString getSidebarStyle();
    static QString getContentAreaStyle();
    static QString getCardStyle();
    
    // Stili per etichette
    static QString getTitleLabelStyle();
    static QString getSubtitleLabelStyle();
    static QString getBodyLabelStyle();
    
    // Colori del tema
    static QString getPrimaryColor();
    static QString getSecondaryColor();
    static QString getAccentColor();
    static QString getBackgroundColor();
    static QString getSurfaceColor();
    static QString getTextColor();
    static QString getTextSecondaryColor();
    static QString getBorderColor();
    
private:
    StyleUtils() = default; // Classe utility statica
};

#endif // STYLEUTILS_H
