/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>
    SPDX-FileCopyrightText: 2008 Lucas Murray <lmurray@undefinedfire.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "effect/offscreeneffect.h"

namespace KWin
{

class GLShader;

/**
 * LumInverts desktop's colors
 */
class LumInvertEffect : public OffscreenEffect
{
    Q_OBJECT
public:
    LumInvertEffect();
    ~LumInvertEffect() override;

    bool isActive() const override;
    bool provides(Feature) override;
    int requestedEffectChainPosition() const override;

    static bool supported();

public Q_SLOTS:
    void toggleScreenInversion();
    void toggleWindow();

    void slotWindowAdded(KWin::EffectWindow *w);
    void slotWindowClosed(KWin::EffectWindow *w);

protected:
    bool loadData();

private:
    bool isLumInvertable(EffectWindow *window) const;
    void luminvert(EffectWindow *window);
    void unluminvert(EffectWindow *window);

    bool m_inited;
    bool m_valid;
    std::unique_ptr<GLShader> m_shader;
    bool m_allWindows;
    QList<EffectWindow *> m_windows;
};

inline int LumInvertEffect::requestedEffectChainPosition() const
{
    return 99;
}

} // namespace
