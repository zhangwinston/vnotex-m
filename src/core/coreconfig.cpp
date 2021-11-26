#include "coreconfig.h"

#include <QMetaEnum>
#include <QLocale>

using namespace vnotex;

#define READSTR(key) readString(appObj, userObj, (key))
#define READINT(key) readInt(appObj, userObj, (key))
#define READBOOL(key) readBool(appObj, userObj, (key))
#define READSTRLIST(key) readStringList(appObj, userObj, (key))

QStringList CoreConfig::s_availableLocales;

CoreConfig::CoreConfig(ConfigMgr *p_mgr, IConfig *p_topConfig)
    : IConfig(p_mgr, p_topConfig)
{
    m_sessionName = QStringLiteral("core");
}

const QString &CoreConfig::getTheme() const
{
    return m_theme;
}

void CoreConfig::setTheme(const QString &p_name)
{
    updateConfig(m_theme, p_name, this);
}

void CoreConfig::init(const QJsonObject &p_app,
                      const QJsonObject &p_user)
{
    const auto appObj = p_app.value(m_sessionName).toObject();
    const auto userObj = p_user.value(m_sessionName).toObject();

    m_theme = READSTR(QStringLiteral("theme"));

    m_locale = READSTR(QStringLiteral("locale"));
    if (!m_locale.isEmpty() && !getAvailableLocales().contains(m_locale)) {
        m_locale = QStringLiteral("en_US");
    }

    loadShortcuts(appObj, userObj);

    m_toolBarIconSize = READINT(QStringLiteral("toolbar_icon_size"));
    if (m_toolBarIconSize <= 0) {
        m_toolBarIconSize = 18;
    }

    m_docksTabBarIconSize = READINT(QStringLiteral("docks_tabbar_icon_size"));
    if (m_docksTabBarIconSize <= 0) {
        m_docksTabBarIconSize = 20;
    }

    loadNoteManagement(appObj, userObj);

    m_recoverLastSessionOnStartEnabled = READBOOL(QStringLiteral("recover_last_session_on_start"));

    m_checkForUpdatesOnStartEnabled = READBOOL(QStringLiteral("check_for_updates_on_start"));

    m_historyMaxCount = READINT(QStringLiteral("history_max_count"));
    if (m_historyMaxCount < 0) {
        m_historyMaxCount = 100;
    }

    m_perNotebookHistoryEnabled = READBOOL(QStringLiteral("per_notebook_history"));
}

QJsonObject CoreConfig::toJson() const
{
    QJsonObject obj;
    obj[QStringLiteral("theme")] = m_theme;
    obj[QStringLiteral("locale")] = m_locale;
    obj[QStringLiteral("shortcuts")] = saveShortcuts();
    obj[QStringLiteral("toolbar_icon_size")] = m_toolBarIconSize;
    obj[QStringLiteral("docks_tabbar_icon_size")] = m_docksTabBarIconSize;
    obj[QStringLiteral("recover_last_session_on_start")] = m_recoverLastSessionOnStartEnabled;
    obj[QStringLiteral("check_for_updates_on_start")] = m_checkForUpdatesOnStartEnabled;
    obj[QStringLiteral("history_max_count")] = m_historyMaxCount;
    obj[QStringLiteral("per_notebook_history")] = m_perNotebookHistoryEnabled;
    return obj;
}

const QString &CoreConfig::getLocale() const
{
    return m_locale;
}

void CoreConfig::setLocale(const QString &p_locale)
{
    updateConfig(m_locale, p_locale, this);
}

QString CoreConfig::getLocaleToUse() const
{
    return QLocale().name();
}

const QStringList &CoreConfig::getAvailableLocales()
{
    if (s_availableLocales.isEmpty()) {
        s_availableLocales << QStringLiteral("en_US");
        s_availableLocales << QStringLiteral("zh_CN");
        s_availableLocales << QStringLiteral("ja_JP");
    }

    return s_availableLocales;
}

void CoreConfig::loadShortcuts(const QJsonObject &p_app, const QJsonObject &p_user)
{
    const auto appObj = p_app.value(QStringLiteral("shortcuts")).toObject();
    const auto userObj = p_user.value(QStringLiteral("shortcuts")).toObject();

    static const auto indexOfShortcutEnum = CoreConfig::staticMetaObject.indexOfEnumerator("Shortcut");
    Q_ASSERT(indexOfShortcutEnum >= 0);
    const auto metaEnum = CoreConfig::staticMetaObject.enumerator(indexOfShortcutEnum);
    // Skip the Max flag.
    for (int i = 0; i < metaEnum.keyCount() - 1; ++i) {
        m_shortcuts[i] = READSTR(metaEnum.key(i));
    }
}

void CoreConfig::loadNoteManagement(const QJsonObject &p_app, const QJsonObject &p_user)
{
    const auto topAppObj = p_app.value(QStringLiteral("note_management")).toObject();
    const auto topUserObj = p_user.value(QStringLiteral("note_management")).toObject();

    // External node.
    {
        const auto appObj = topAppObj.value(QStringLiteral("external_node")).toObject();
        const auto userObj = topUserObj.value(QStringLiteral("external_node")).toObject();

        m_externalNodeExcludePatterns = READSTRLIST(QStringLiteral("exclude_patterns"));
    }
}

QJsonObject CoreConfig::saveShortcuts() const
{
    QJsonObject obj;
    static const auto indexOfShortcutEnum = CoreConfig::staticMetaObject.indexOfEnumerator("Shortcut");
    Q_ASSERT(indexOfShortcutEnum >= 0);
    const auto metaEnum = CoreConfig::staticMetaObject.enumerator(indexOfShortcutEnum);
    // Skip the Max flag.
    for (int i = 0; i < metaEnum.keyCount() - 1; ++i) {
        obj[metaEnum.key(i)] = m_shortcuts[i];
    }
    return obj;
}

const QString &CoreConfig::getShortcut(Shortcut p_shortcut) const
{
    Q_ASSERT(p_shortcut < Shortcut::MaxShortcut);
    return m_shortcuts[p_shortcut];
}

int CoreConfig::getToolBarIconSize() const
{
    return m_toolBarIconSize;
}

void CoreConfig::setToolBarIconSize(int p_size)
{
    Q_ASSERT(p_size > 0);
    updateConfig(m_toolBarIconSize, p_size, this);
}

int CoreConfig::getDocksTabBarIconSize() const
{
    return m_docksTabBarIconSize;
}

void CoreConfig::setDocksTabBarIconSize(int p_size)
{
    Q_ASSERT(p_size > 0);
    updateConfig(m_docksTabBarIconSize, p_size, this);
}

const QStringList &CoreConfig::getExternalNodeExcludePatterns() const
{
    return m_externalNodeExcludePatterns;
}

bool CoreConfig::isRecoverLastSessionOnStartEnabled() const
{
    return m_recoverLastSessionOnStartEnabled;
}

void CoreConfig::setRecoverLastSessionOnStartEnabled(bool p_enabled)
{
    updateConfig(m_recoverLastSessionOnStartEnabled, p_enabled, this);
}

bool CoreConfig::isCheckForUpdatesOnStartEnabled() const
{
    return m_checkForUpdatesOnStartEnabled;
}

void CoreConfig::setCheckForUpdatesOnStartEnabled(bool p_enabled)
{
    updateConfig(m_checkForUpdatesOnStartEnabled, p_enabled, this);
}

int CoreConfig::getHistoryMaxCount() const
{
    return m_historyMaxCount;
}

bool CoreConfig::isPerNotebookHistoryEnabled() const
{
    return m_perNotebookHistoryEnabled;
}

void CoreConfig::setPerNotebookHistoryEnabled(bool p_enabled)
{
    updateConfig(m_perNotebookHistoryEnabled, p_enabled, this);
}
