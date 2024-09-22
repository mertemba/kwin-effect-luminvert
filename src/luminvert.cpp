/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>
    SPDX-FileCopyrightText: 2008 Lucas Murray <lmurray@undefinedfire.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "luminvert.h"

#include "effect/effecthandler.h"
#include "opengl/glplatform.h"
#include "opengl/glutils.h"
#include <KGlobalAccel>
#include <KLocalizedString>
#include <QAction>
#include <QFile>
#include <QStandardPaths>

#include <QMatrix4x4>

Q_LOGGING_CATEGORY(KWIN_LUMINVERT, "kwin_effect_luminvert", QtWarningMsg)

static void ensureResources()
{
    // Must initialize resources manually because the effect is a static lib.
    Q_INIT_RESOURCE(luminvert);
}

namespace KWin
{

LumInvertEffect::LumInvertEffect()
    : m_inited(false)
    , m_valid(true)
    , m_shader(nullptr)
    , m_allWindows(false)
{
    QAction *a = new QAction(this);
    a->setObjectName(QStringLiteral("LumInvert"));
    a->setText(i18n("Toggle LumInvert Effect"));
    KGlobalAccel::self()->setDefaultShortcut(a, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_I));
    KGlobalAccel::self()->setShortcut(a, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_I));
    connect(a, &QAction::triggered, this, &LumInvertEffect::toggleScreenInversion);

    QAction *b = new QAction(this);
    b->setObjectName(QStringLiteral("LumInvertWindow"));
    b->setText(i18n("Toggle LumInvert Effect on Window"));
    KGlobalAccel::self()->setDefaultShortcut(b, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_U));
    KGlobalAccel::self()->setShortcut(b, QList<QKeySequence>() << (Qt::CTRL | Qt::META | Qt::Key_U));
    connect(b, &QAction::triggered, this, &LumInvertEffect::toggleWindow);

    QAction *c = new QAction(this);
    c->setObjectName(QStringLiteral("LumInvert Screen Colors"));
    c->setText(i18n("LumInvert Screen Colors"));
    KGlobalAccel::self()->setDefaultShortcut(c, QList<QKeySequence>());
    KGlobalAccel::self()->setShortcut(c, QList<QKeySequence>());
    connect(c, &QAction::triggered, this, &LumInvertEffect::toggleScreenInversion);

    connect(effects, &EffectsHandler::windowAdded, this, &LumInvertEffect::slotWindowAdded);
    connect(effects, &EffectsHandler::windowClosed, this, &LumInvertEffect::slotWindowClosed);
}

LumInvertEffect::~LumInvertEffect() = default;

bool LumInvertEffect::supported()
{
    return effects->compositingType() == OpenGLCompositing;
}

bool LumInvertEffect::isLumInvertable(EffectWindow *window) const
{
    return m_allWindows != m_windows.contains(window);
}

void LumInvertEffect::luminvert(EffectWindow *window)
{
    if (m_valid && !m_inited) {
        m_valid = loadData();
    }

    redirect(window);
    setShader(window, m_shader.get());
}

void LumInvertEffect::unluminvert(EffectWindow *window)
{
    unredirect(window);
}

bool LumInvertEffect::loadData()
{
    ensureResources();
    m_inited = true;

    m_shader = ShaderManager::instance()->generateShaderFromFile(ShaderTrait::MapTexture, QString(), QStringLiteral(":/effects/luminvert/shaders/luminvert.frag"));
    if (!m_shader->isValid()) {
        qCCritical(KWIN_LUMINVERT) << "The shader failed to load!";
        return false;
    }

    return true;
}

void LumInvertEffect::slotWindowAdded(KWin::EffectWindow *w)
{
    if (isLumInvertable(w)) {
        luminvert(w);
    }
}

void LumInvertEffect::slotWindowClosed(EffectWindow *w)
{
    m_windows.removeOne(w);
}

void LumInvertEffect::toggleScreenInversion()
{
    m_allWindows = !m_allWindows;

    const auto windows = effects->stackingOrder();
    for (EffectWindow *window : windows) {
        if (isLumInvertable(window)) {
            luminvert(window);
        } else {
            unluminvert(window);
        }
    }

    effects->addRepaintFull();
}

void LumInvertEffect::toggleWindow()
{
    if (!effects->activeWindow()) {
        return;
    }
    if (!m_windows.contains(effects->activeWindow())) {
        m_windows.append(effects->activeWindow());
    } else {
        m_windows.removeOne(effects->activeWindow());
    }
    if (isLumInvertable(effects->activeWindow())) {
        luminvert(effects->activeWindow());
    } else {
        unluminvert(effects->activeWindow());
    }
    effects->activeWindow()->addRepaintFull();
}

bool LumInvertEffect::isActive() const
{
    return m_valid && (m_allWindows || !m_windows.isEmpty());
}

bool LumInvertEffect::provides(Feature f)
{
    return f == ScreenInversion;
}

} // namespace

#include "moc_luminvert.cpp"
