/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "luminvert_config.h"
#include <kwineffects_interface.h>

#include <QAction>

#include <KActionCollection>
#include <KGlobalAccel>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KShortcutsEditor>

#include <QVBoxLayout>

K_PLUGIN_CLASS(KWin::LumInvertEffectConfig)

namespace KWin
{

LumInvertEffectConfig::LumInvertEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    QVBoxLayout *layout = new QVBoxLayout(widget());

    // Shortcut config. The shortcut belongs to the component "kwin"!
    KActionCollection *actionCollection = new KActionCollection(widget(), QStringLiteral("kwin"));
    actionCollection->setComponentDisplayName(i18n("KWin"));

    QAction *a = actionCollection->addAction(QStringLiteral("LumInvert"));
    a->setText(i18n("Toggle LumInvert Effect"));
    a->setProperty("isConfigurationAction", true);
    KGlobalAccel::self()->setDefaultShortcut(a, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_I));
    KGlobalAccel::self()->setShortcut(a, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_I));

    QAction *b = actionCollection->addAction(QStringLiteral("LumInvertWindow"));
    b->setText(i18n("Toggle LumInvert Effect on Window"));
    b->setProperty("isConfigurationAction", true);
    KGlobalAccel::self()->setDefaultShortcut(b, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_U));
    KGlobalAccel::self()->setShortcut(b, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_U));

    mShortcutEditor = new KShortcutsEditor(actionCollection, widget(),
                                           KShortcutsEditor::GlobalAction, KShortcutsEditor::LetterShortcutsDisallowed);
    connect(mShortcutEditor, &KShortcutsEditor::keyChange, this, &KCModule::markAsChanged);
    layout->addWidget(mShortcutEditor);
}

void LumInvertEffectConfig::load()
{
    KCModule::load();

    setNeedsSave(false);
}

void LumInvertEffectConfig::save()
{
    KCModule::save();

    mShortcutEditor->save(); // undo() will restore to this state from now on

    setNeedsSave(false);
    OrgKdeKwinEffectsInterface interface(QStringLiteral("org.kde.KWin"),
                                         QStringLiteral("/Effects"),
                                         QDBusConnection::sessionBus());
    interface.reconfigureEffect(QStringLiteral("luminvert"));
}

void LumInvertEffectConfig::defaults()
{
    mShortcutEditor->allDefault();

    setNeedsSave(true);
}

} // namespace

#include "luminvert_config.moc"

#include "moc_luminvert_config.cpp"
