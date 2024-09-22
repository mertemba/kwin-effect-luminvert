# Maintainer:
# Contributor: 2xsaiko <aur@dblsaiko.net>

_pkgname="kwin-effect-luminvert"
pkgname="$_pkgname-git"
pkgver=89d2b8
pkgrel=1
pkgdesc="KWin Desktop Effect to invert the luminance of window contents"
url="https://github.com/mertemba/kwin-effect-luminvert"
license=("GPL-3.0-only")
arch=('x86_64')

provides=("$_pkgname=${pkgver%%.r*}")
conflicts=("$_pkgname")

depends=(
  'kwin'
  'qt6-base'
  'kcoreaddons'
  'kglobalaccel'
  'kxmlgui'
  'kcmutils'
  'gcc-libs'
  'ki18n'
  'glibc'
)
makedepends=(
  'cmake'
  'extra-cmake-modules'
  'git'
  'ninja'
)

_pkgsrc="kwin-effect-luminvert"
source=("$_pkgsrc"::"git+$url.git")
sha256sums=("SKIP")

pkgver() {
  cd "$_pkgsrc"
  git describe --long --tags --abbrev=7 --always --exclude='*[a-zA-Z][a-zA-Z]*' \
    | sed -E 's/^[^0-9]*//;s/([^-]*-g)/r\1/;s/-/./g'
}

build() {
  cmake -B build \
    -S "$_pkgsrc" \
    -G Ninja \
    -Wno-dev
  cmake --build build
}

package() {
  DESTDIR="$pkgdir" cmake --install build
}
