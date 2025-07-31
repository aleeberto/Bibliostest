#include "styleUtils.h"

// Colori del tema moderno e minimal
QString StyleUtils::getPrimaryColor() { return "#2563eb"; }      // Blue-600
QString StyleUtils::getSecondaryColor() { return "#64748b"; }    // Slate-500
QString StyleUtils::getAccentColor() { return "#10b981"; }       // Emerald-500
QString StyleUtils::getBackgroundColor() { return "#ffffff"; }   // White
QString StyleUtils::getSurfaceColor() { return "#f8fafc"; }      // Slate-50
QString StyleUtils::getTextColor() { return "#0f172a"; }         // Slate-900
QString StyleUtils::getTextSecondaryColor() { return "#64748b"; } // Slate-500
QString StyleUtils::getBorderColor() { return "#e2e8f0"; }       // Slate-200

// Stili per pulsanti moderni
QString StyleUtils::getPrimaryButtonStyle()
{
    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 12px 24px;"
        "  font-weight: 600;"
        "  font-size: 14px;"
        "  min-height: 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1d4ed8;"
        "  transform: translateY(-1px);"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1e40af;"
        "  transform: translateY(0px);"
        "}"
        "QPushButton:disabled {"
        "  background-color: #cbd5e1;"
        "  color: #94a3b8;"
        "}"
    ).arg(getPrimaryColor());
}

QString StyleUtils::getSecondaryButtonStyle()
{
    return QString(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: %1;"
        "  border: 2px solid %2;"
        "  border-radius: 8px;"
        "  padding: 10px 22px;"
        "  font-weight: 600;"
        "  font-size: 14px;"
        "  min-height: 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %3;"
        "  color: %4;"
        "}"
        "QPushButton:pressed {"
        "  background-color: %5;"
        "}"
    ).arg(getSecondaryColor(), getBorderColor(), getSurfaceColor(), getTextColor(), getBorderColor());
}

QString StyleUtils::getDangerButtonStyle()
{
    return QString(
        "QPushButton {"
        "  background-color: #dc2626;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 12px 24px;"
        "  font-weight: 600;"
        "  font-size: 14px;"
        "  min-height: 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #b91c1c;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #991b1b;"
        "}"
    );
}

QString StyleUtils::getWarningButtonStyle()
{
    return QString(
        "QPushButton {"
        "  background-color: #f59e0b;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 12px 24px;"
        "  font-weight: 600;"
        "  font-size: 14px;"
        "  min-height: 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #d97706;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #b45309;"
        "}"
    );
}

QString StyleUtils::getIconButtonStyle()
{
    return QString(
        "QPushButton {"
        "  background-color: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 8px;"
        "  padding: 8px;"
        "  min-width: 40px;"
        "  min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %3;"
        "  border-color: %4;"
        "  transform: translateY(-1px);"
        "}"
        "QPushButton:pressed {"
        "  background-color: %5;"
        "  transform: translateY(0px);"
        "}"
    ).arg(getBackgroundColor(), getBorderColor(), getSurfaceColor(), getSecondaryColor(), getBorderColor());
}

// Stili per input moderni
QString StyleUtils::getLineEditStyle()
{
    return QString(
        "QLineEdit {"
        "  background-color: %1;"
        "  border: 2px solid %2;"
        "  border-radius: 8px;"
        "  padding: 12px 16px;"
        "  font-size: 14px;"
        "  color: %3;"
        "  min-height: 20px;"
        "}"
        "QLineEdit:focus {"
        "  border-color: %4;"
        "  background-color: %5;"
        "}"
        "QLineEdit:hover {"
        "  border-color: %6;"
        "}"
    ).arg(getBackgroundColor(), getBorderColor(), getTextColor(), getPrimaryColor(), getBackgroundColor(), getSecondaryColor());
}

QString StyleUtils::getSearchBarStyle()
{
    return QString(
        "QLineEdit {"
        "  background-color: %1;"
        "  border: 2px solid %2;"
        "  border-radius: 20px;"
        "  padding: 8px 16px;"
        "  font-size: 14px;"
        "  color: %3;"
        "  min-height: 16px;"
        "}"
        "QLineEdit:focus {"
        "  border-color: %4;"
        "}"
    ).arg(getSurfaceColor(), getBorderColor(), getTextColor(), getPrimaryColor());
}

QString StyleUtils::getComboBoxStyle()
{
    return QString(
        "QComboBox {"
        "  background-color: %1;"
        "  border: 2px solid %2;"
        "  border-radius: 8px;"
        "  padding: 12px 16px;"
        "  font-size: 14px;"
        "  color: %3;"
        "  min-height: 20px;"
        "}"
        "QComboBox:focus {"
        "  border-color: %4;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "  width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "  width: 12px;"
        "  height: 12px;"
        "}"
    ).arg(getBackgroundColor(), getBorderColor(), getTextColor(), getPrimaryColor());
}

// Stili per layout
QString StyleUtils::getMainWindowStyle()
{
    return QString(
        "QMainWindow {"
        "  background-color: %1;"
        "  color: %2;"
        "}"
        "QWidget {"
        "  font-family: 'Inter', 'Segoe UI', 'Roboto', sans-serif;"
        "}"
    ).arg(getBackgroundColor(), getTextColor());
}

QString StyleUtils::getSidebarStyle()
{
    return QString(
        "QWidget {"
        "  background-color: %1;"
        "  border-right: 1px solid %2;"
        "}"
    ).arg(getSurfaceColor(), getBorderColor());
}

QString StyleUtils::getContentAreaStyle()
{
    return QString(
        "QWidget {"
        "  background-color: %1;"
        "}"
        "QScrollArea {"
        "  border: none;"
        "  background: transparent;"
        "}"
    ).arg(getBackgroundColor());
}

QString StyleUtils::getCardStyle()
{
    return QString(
        "QWidget {"
        "  background-color: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 12px;"
        "  padding: 16px;"
        "  margin: 8px;"
        "}"
        "QWidget:hover {"
        "  border-color: %3;"
        "  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);"
        "}"
    ).arg(getBackgroundColor(), getBorderColor(), getSecondaryColor());
}

// Stili per etichette
QString StyleUtils::getTitleLabelStyle()
{
    return QString(
        "QLabel {"
        "  color: %1;"
        "  font-size: 18px;"
        "  font-weight: 700;"
        "  margin-bottom: 8px;"
        "}"
    ).arg(getTextColor());
}

QString StyleUtils::getSubtitleLabelStyle()
{
    return QString(
        "QLabel {"
        "  color: %1;"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "  margin-bottom: 4px;"
        "}"
    ).arg(getTextColor());
}

QString StyleUtils::getBodyLabelStyle()
{
    return QString(
        "QLabel {"
        "  color: %1;"
        "  font-size: 14px;"
        "  font-weight: 400;"
        "  line-height: 1.5;"
        "}"
    ).arg(getTextSecondaryColor());
}
