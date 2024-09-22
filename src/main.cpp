/*
    SPDX-FileCopyrightText: 2021 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "luminvert.h"

namespace KWin
{

KWIN_EFFECT_FACTORY_SUPPORTED(LumInvertEffect,
                              "metadata.json",
                              return LumInvertEffect::supported();)

} // namespace KWin

#include "main.moc"
